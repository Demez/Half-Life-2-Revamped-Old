//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: Sunlight shadow control entity.
//
// $NoKeywords: $
//=============================================================================//
#include "cbase.h"

#include "c_baseplayer.h"
#include "vprof.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"


ConVar cl_sunlight_depthbias("cl_sunlight_depthbias", "0.00"); //setting this to 0 causes slight issues on models, but the shadow bleeding would be way too obivous and just look very bad
ConVar cl_sunlight_enabled("cl_sunlight_enabled", "1", FCVAR_ARCHIVE);
ConVar cl_sunlight_freeze("cl_sunlight_freeze", "0");
ConVar cl_sunlight_xoffset("cl_sunlight_xoffset", "0");
ConVar cl_sunlight_yoffset("cl_sunlight_yoffset", "0");
ConVar cl_sunlight_debug("cl_sunlight_debug", "0");
ConVar cl_sunlight_orthosize("cl_sunlight_orthosize", "1000", FCVAR_CHEAT, "Set to values greater than 0 for ortho view render projections.");

//i dont even know
//ConVar cl_sunlight_showpos("cl_sunlight_showpos", "0");
//ConVar cl_sunlight_xpos("cl_sunlight_xpos", "0");
//ConVar cl_sunlight_ypos("cl_sunlight_ypos", "0");

//is it even possible to have this in source 2013?
// We still rely on r_flashlightbrightness, but here we can adjust the multiplier of the global light.
//ConVar cl_sunlight_brightness("cl_sunlight_brightness_multiplier", "1.0");

//------------------------------------------------------------------------------
// Purpose : Sunlights shadow control entity
//------------------------------------------------------------------------------
class C_SunlightShadowControl : public C_BaseEntity
{
public:
	DECLARE_CLASS(C_SunlightShadowControl, C_BaseEntity);

	DECLARE_CLIENTCLASS();

	virtual ~C_SunlightShadowControl();

	void OnDataChanged(DataUpdateType_t updateType);
	void Spawn();
	bool ShouldDraw();

	void ClientThink();

private:
	Vector m_shadowDirection;
	bool m_bEnabled;
	char m_TextureName[MAX_PATH];
	CTextureReference m_SpotlightTexture;
	color32	m_LightColor;
	Vector m_CurrentLinearFloatLightColor;
	float m_flCurrentLinearFloatLightAlpha;
	float m_flColorTransitionTime;
	float m_flSunDistance;
	float m_flFOV;
	float m_flNearZ;
	float m_flNorthOffset;
	bool m_bEnableShadows;
	bool m_bOldEnableShadows;

	static ClientShadowHandle_t m_LocalFlashlightHandle;
};


ClientShadowHandle_t C_SunlightShadowControl::m_LocalFlashlightHandle = CLIENTSHADOW_INVALID_HANDLE;


IMPLEMENT_CLIENTCLASS_DT(C_SunlightShadowControl, DT_SunlightShadowControl, CSunlightShadowControl)
RecvPropVector(RECVINFO(m_shadowDirection)),
RecvPropBool(RECVINFO(m_bEnabled)),
RecvPropString(RECVINFO(m_TextureName)),
RecvPropInt(RECVINFO(m_LightColor), 0, RecvProxy_Int32ToColor32),
RecvPropFloat(RECVINFO(m_flColorTransitionTime)),
RecvPropFloat(RECVINFO(m_flSunDistance)),
RecvPropFloat(RECVINFO(m_flFOV)),
RecvPropFloat(RECVINFO(m_flNearZ)),
RecvPropFloat(RECVINFO(m_flNorthOffset)),
RecvPropBool(RECVINFO(m_bEnableShadows)),
END_RECV_TABLE()


C_SunlightShadowControl::~C_SunlightShadowControl()
{
	if (m_LocalFlashlightHandle != CLIENTSHADOW_INVALID_HANDLE)
	{
		g_pClientShadowMgr->DestroyFlashlight(m_LocalFlashlightHandle);
		m_LocalFlashlightHandle = CLIENTSHADOW_INVALID_HANDLE;
	}
}

void C_SunlightShadowControl::OnDataChanged(DataUpdateType_t updateType)
{
	if (updateType == DATA_UPDATE_CREATED)
	{
		m_SpotlightTexture.Init(m_TextureName, TEXTURE_GROUP_OTHER, true);
	}

	BaseClass::OnDataChanged(updateType);
}

void C_SunlightShadowControl::Spawn()
{
	BaseClass::Spawn();

	m_bOldEnableShadows = m_bEnableShadows;

	SetNextClientThink(CLIENT_THINK_ALWAYS);
}

//------------------------------------------------------------------------------
// We don't draw...
//------------------------------------------------------------------------------
bool C_SunlightShadowControl::ShouldDraw()
{
	return false;
}

void C_SunlightShadowControl::ClientThink()
{
	VPROF("C_SunlightShadowControl::ClientThink");

	bool bSupressWorldLights = false;

	if (cl_sunlight_freeze.GetBool() == true)
	{
		return;
	}
	//let us turn this shit on and off ingame
	m_bEnabled = cl_sunlight_enabled.GetBool();

	if (m_bEnabled)
	{
		Vector vLinearFloatLightColor(m_LightColor.r, m_LightColor.g, m_LightColor.b);
		float flLinearFloatLightAlpha = m_LightColor.a;

		if (m_CurrentLinearFloatLightColor != vLinearFloatLightColor || m_flCurrentLinearFloatLightAlpha != flLinearFloatLightAlpha)
		{
			float flColorTransitionSpeed = gpGlobals->frametime * m_flColorTransitionTime * 255.0f;

			m_CurrentLinearFloatLightColor.x = Approach(vLinearFloatLightColor.x, m_CurrentLinearFloatLightColor.x, flColorTransitionSpeed);
			m_CurrentLinearFloatLightColor.y = Approach(vLinearFloatLightColor.y, m_CurrentLinearFloatLightColor.y, flColorTransitionSpeed);
			m_CurrentLinearFloatLightColor.z = Approach(vLinearFloatLightColor.z, m_CurrentLinearFloatLightColor.z, flColorTransitionSpeed);
			m_flCurrentLinearFloatLightAlpha = Approach(flLinearFloatLightAlpha, m_flCurrentLinearFloatLightAlpha, flColorTransitionSpeed);
		}

		FlashlightState_t state;

		Vector vDirection = m_shadowDirection;
		VectorNormalize(vDirection);

		QAngle angView;
		engine->GetViewAngles(angView);

		//Vector vViewUp = Vector( 0.0f, 1.0f, 0.0f );
		Vector vSunDirection2D = vDirection;
		vSunDirection2D.z = 0.0f;

		HACK_GETLOCALPLAYER_GUARD("C_SunlightShadowControl::ClientThink");

		if (!C_BasePlayer::GetLocalPlayer())
			return;

		//Vector vPos = (C_BasePlayer::GetLocalPlayer()->GetAbsOrigin() + vSunDirection2D * m_flNorthOffset) - vDirection * m_flSunDistance;

		Vector vPos;
		QAngle EyeAngles;
		float flZNear, flZFar, flFov;

		C_BasePlayer::GetLocalPlayer()->CalcView(vPos, EyeAngles, flZNear, flZFar, flFov);
		//		Vector vPos = C_BasePlayer::GetLocalPlayer()->GetAbsOrigin();

		//		vPos = Vector( 0.0f, 0.0f, 500.0f );
		vPos = (vPos + vSunDirection2D * m_flNorthOffset) - vDirection * m_flSunDistance;
		vPos += Vector(cl_sunlight_xoffset.GetFloat(), cl_sunlight_yoffset.GetFloat(), 0.0f);

		/*
		if (cl_sunlight_showpos.GetBool() == true){	//ËÀË ß ÒÓÒÀ ÍÅÌÍÎÃÎ ÍÀØÊÎÄÈË, ÍÅ ÐÓÃÀÉÒÈÑ ÏËÇ ËÀÍÑÏÑ
			if (cl_sunlight_xpos.GetFloat() != 0 && cl_sunlight_ypos.GetFloat() != 0) {
				DevMsg("X = %3.0f\n Y = %3.0f\n", cl_sunlight_xpos.GetFloat(), cl_sunlight_ypos.GetFloat());
			}
			else
				DevMsg("X = %3.0f\n Y = %3.0f\n", vPos.x, vPos.y);
		}
		if (cl_sunlight_xpos.GetFloat() != 0 && cl_sunlight_ypos.GetFloat() != 0) {
			vPos.x = cl_sunlight_xpos.GetFloat();
			vPos.y = cl_sunlight_ypos.GetFloat();
		}
		*/

		QAngle angAngles;
		VectorAngles(vDirection, angAngles);

		Vector vForward, vRight, vUp;
		AngleVectors(angAngles, &vForward, &vRight, &vUp);

		state.m_fHorizontalFOVDegrees = m_flFOV;
		state.m_fVerticalFOVDegrees = m_flFOV;

		state.m_vecLightOrigin = vPos;
		BasisToQuaternion(vForward, vRight, vUp, state.m_quatOrientation);

		state.m_fQuadraticAtten = 0.0f;
		state.m_fLinearAtten = m_flSunDistance * 2.0f;
		state.m_fConstantAtten = 0.0f;
		state.m_FarZAtten = m_flSunDistance * 2.0f;
		//state.m_FarZAtten = m_flSunDistance + 300.0f;
		state.m_Color[0] = m_CurrentLinearFloatLightColor.x * (1.0f / 255.0f) * m_flCurrentLinearFloatLightAlpha;
		state.m_Color[1] = m_CurrentLinearFloatLightColor.y * (1.0f / 255.0f) * m_flCurrentLinearFloatLightAlpha;
		state.m_Color[2] = m_CurrentLinearFloatLightColor.z * (1.0f / 255.0f) * m_flCurrentLinearFloatLightAlpha;
		state.m_Color[3] = 0.0f; // fixme: need to make ambient work m_flAmbient;
		state.m_NearZ = fpmax(4.0f, m_flSunDistance - m_flNearZ); //fpmax is here so shadowbleeding isnt fucked
		state.m_FarZ = m_flSunDistance * 2.0f; //so that models can get lit and so shadows work properly
		//state.m_fBrightnessScale = 1.0f; //changing this does nothing apparently, so that sucks
		//state.m_fBrightnessScale = cl_sunlight_brightness.GetFloat();
		float flOrthoSize = cl_sunlight_orthosize.GetFloat();

		//any point to this?
		state.m_bSunlight = true;

		state.m_bOrtho = true;
		state.m_fOrthoLeft = -flOrthoSize;
		state.m_fOrthoTop = -flOrthoSize;
		state.m_fOrthoRight = flOrthoSize;
		state.m_fOrthoBottom = flOrthoSize;

		if (cl_sunlight_debug.GetBool())
		{
			// Draw where we are projecting.
			state.m_bDrawShadowFrustum = true;
		}

		state.m_flShadowSlopeScaleDepthBias = 1.0f;
		state.m_flShadowDepthBias = cl_sunlight_depthbias.GetFloat(); //This is better then the one below
		//state.m_flShadowDepthBias = g_pMaterialSystemHardwareConfig->GetShadowDepthBias();
		state.m_bEnableShadows = m_bEnableShadows;
		state.m_pSpotlightTexture = m_SpotlightTexture;
		state.m_pProjectedMaterial = NULL;
		state.m_nSpotlightTextureFrame = 0;
		//state.m_flShadowFilterSize = 0.2f;

		state.m_nShadowQuality = 1; // Allow entity to affect shadow quality
		state.m_bShadowHighRes = true;

		if (m_bOldEnableShadows != m_bEnableShadows)
		{
			// If they change the shadow enable/disable, we need to make a new handle
			if (m_LocalFlashlightHandle != CLIENTSHADOW_INVALID_HANDLE)
			{
				g_pClientShadowMgr->DestroyFlashlight(m_LocalFlashlightHandle);
				m_LocalFlashlightHandle = CLIENTSHADOW_INVALID_HANDLE;
			}

			m_bOldEnableShadows = m_bEnableShadows;
		}

		if (m_LocalFlashlightHandle == CLIENTSHADOW_INVALID_HANDLE)
		{
			m_LocalFlashlightHandle = g_pClientShadowMgr->CreateFlashlight(state);
		}
		else
		{
			g_pClientShadowMgr->UpdateFlashlightState(m_LocalFlashlightHandle, state);
			g_pClientShadowMgr->UpdateProjectedTexture(m_LocalFlashlightHandle, true);
		}

		bSupressWorldLights = m_bEnableShadows;
	}
	else if (m_LocalFlashlightHandle != CLIENTSHADOW_INVALID_HANDLE)
	{
		g_pClientShadowMgr->DestroyFlashlight(m_LocalFlashlightHandle);
		m_LocalFlashlightHandle = CLIENTSHADOW_INVALID_HANDLE;
	}
	g_pClientShadowMgr->SetShadowFromWorldLightsEnabled( !bSupressWorldLights );

	BaseClass::ClientThink();
}