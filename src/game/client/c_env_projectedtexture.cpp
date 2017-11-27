//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================

#include "cbase.h"
#include "shareddefs.h"
#include "materialsystem/imesh.h"
#include "materialsystem/imaterial.h"
#include "view.h"
#include "iviewrender.h"
#include "view_shared.h"
#include "texture_group_names.h"
#include "tier0/icommandline.h"
#include "c_env_projectedtexture.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

extern ConVar mat_slopescaledepthbias_shadowmap;
extern ConVar mat_depthbias_shadowmap;

IMPLEMENT_CLIENTCLASS_DT( C_EnvProjectedTexture, DT_EnvProjectedTexture, CEnvProjectedTexture )
	RecvPropEHandle( RECVINFO( m_hTargetEntity )	),
	RecvPropBool(	 RECVINFO( m_bState )			),
	RecvPropFloat(	 RECVINFO( m_flLightFOV )		),
	//projtex caching
	RecvPropBool(	 RECVINFO( m_bAlwaysUpdate )	),
	//
	RecvPropBool(	 RECVINFO( m_bEnableShadows )	),
	RecvPropBool(	 RECVINFO( m_bLightOnlyTarget ) ),
	RecvPropBool(	 RECVINFO( m_bLightWorld )		),
	RecvPropBool(	 RECVINFO( m_bCameraSpace )		),
	RecvPropVector(	 RECVINFO( m_LinearFloatLightColor )		),
	//RecvPropInt(RECVINFO(m_LightColor), 0, RecvProxy_Int32ToColor32),
	RecvPropFloat(	 RECVINFO( m_flAmbient )		),
	RecvPropString(  RECVINFO( m_SpotlightTextureName ) ),
	RecvPropInt(	 RECVINFO( m_nSpotlightTextureFrame ) ),
	RecvPropFloat(	 RECVINFO( m_flNearZ )	),
	RecvPropFloat(	 RECVINFO( m_flFarZ )	),
	RecvPropInt(	 RECVINFO( m_nShadowQuality )	),
END_RECV_TABLE()

C_EnvProjectedTexture *C_EnvProjectedTexture::Create()
{
	C_EnvProjectedTexture *pEnt = new C_EnvProjectedTexture();

	pEnt->m_flNearZ = 4.0f;
	pEnt->m_flFarZ = 2000.0f;
	//	strcpy( pEnt->m_SpotlightTextureName, "particle/rj" );
	pEnt->m_bLightWorld = true;
	pEnt->m_bLightOnlyTarget = false;
	//pEnt->m_bSimpleProjection = false;
	pEnt->m_nShadowQuality = 1;
	pEnt->m_flLightFOV = 10.0f;
	//pEnt->m_LightColor.r = 255;
	//pEnt->m_LightColor.g = 255;
	//pEnt->m_LightColor.b = 255;
	//pEnt->m_LightColor.a = 255;
	pEnt->m_bEnableShadows = true;
	//pEnt->m_flColorTransitionTime = 1.0f;
	pEnt->m_bCameraSpace = false;
	pEnt->SetAbsAngles(QAngle(90, 0, 0));
	pEnt->m_bAlwaysUpdate = true;
	pEnt->m_bState = true;
	//pEnt->m_flProjectionSize = 500.0f;
	//pEnt->m_flRotation = 0.0f;

	return pEnt;
}

C_EnvProjectedTexture::C_EnvProjectedTexture( void )
{
	m_LightHandle = CLIENTSHADOW_INVALID_HANDLE;
	//projtexcaching
	m_bForceUpdate = true;
}

C_EnvProjectedTexture::~C_EnvProjectedTexture( void )
{
	ShutDownLightHandle();
}

void C_EnvProjectedTexture::ShutDownLightHandle( void )
{
	// Clear out the light
	if( m_LightHandle != CLIENTSHADOW_INVALID_HANDLE )
	{
		g_pClientShadowMgr->DestroyFlashlight( m_LightHandle );
		m_LightHandle = CLIENTSHADOW_INVALID_HANDLE;
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : updateType - 
//-----------------------------------------------------------------------------
void C_EnvProjectedTexture::OnDataChanged( DataUpdateType_t updateType )
{
	//projtex caching
	/*if (updateType == DATA_UPDATE_CREATED)
	{
		m_SpotlightTexture.Init(m_SpotlightTextureName, TEXTURE_GROUP_OTHER, true);
	}*/
	BaseClass::OnDataChanged(updateType);
	UpdateLight();
	m_bForceUpdate = true;
	//
}

void C_EnvProjectedTexture::UpdateLight( void )
{	
	//projtex caching
	if ( m_bState == false )
	{
		if ( m_LightHandle != CLIENTSHADOW_INVALID_HANDLE )
		{
			ShutDownLightHandle();
		}

		return;
	}
	
	if (m_bAlwaysUpdate)
	{
		m_bForceUpdate = true;
	}
	//

	Vector vForward, vRight, vUp, vPos = GetAbsOrigin();
	FlashlightState_t state;

	if ( m_hTargetEntity != NULL )
	{
		if ( m_bCameraSpace )
		{
			const QAngle &angles = GetLocalAngles();

			C_BasePlayer *pPlayer = C_BasePlayer::GetLocalPlayer();
			if( pPlayer )
			{
				const QAngle playerAngles = pPlayer->GetAbsAngles();
				
				Vector vPlayerForward, vPlayerRight, vPlayerUp;
				AngleVectors( playerAngles, &vPlayerForward, &vPlayerRight, &vPlayerUp );

            	matrix3x4_t	mRotMatrix;
				AngleMatrix( angles, mRotMatrix );

				VectorITransform( vPlayerForward, mRotMatrix, vForward );
				VectorITransform( vPlayerRight, mRotMatrix, vRight );
				VectorITransform( vPlayerUp, mRotMatrix, vUp );

				float dist = (m_hTargetEntity->GetAbsOrigin() - GetAbsOrigin()).Length();
				vPos = m_hTargetEntity->GetAbsOrigin() - vForward*dist;

				VectorNormalize( vForward );
				VectorNormalize( vRight );
				VectorNormalize( vUp );
			}
		}
		else
		{
			vForward = m_hTargetEntity->GetAbsOrigin() - GetAbsOrigin();
			VectorNormalize( vForward );

			// JasonM - unimplemented
			Assert (0);

			//Quaternion q = DirectionToOrientation( dir );


			//
			// JasonM - set up vRight, vUp
			//

//			VectorNormalize( vRight );
//			VectorNormalize( vUp );
		}
	}
	else
	{
		// VXP: Fixing targeting
		Vector vecToTarget;
		QAngle vecAngles;
		if ( m_hTargetEntity == NULL )
		{
			vecAngles = GetAbsAngles();
		}
		else
		{
			vecToTarget = m_hTargetEntity->GetAbsOrigin() - GetAbsOrigin();
			VectorAngles( vecToTarget, vecAngles );
		}
		AngleVectors( vecAngles, &vForward, &vRight, &vUp );
	}

	state.m_fHorizontalFOVDegrees = m_flLightFOV;
	state.m_fVerticalFOVDegrees = m_flLightFOV;

	state.m_vecLightOrigin = vPos;
	BasisToQuaternion( vForward, vRight, vUp, state.m_quatOrientation );

	state.m_fQuadraticAtten = 0.0;
	state.m_fLinearAtten = 100;
	state.m_fConstantAtten = 0.0f;
	state.m_Color[0] = m_LinearFloatLightColor.x;
	state.m_Color[1] = m_LinearFloatLightColor.y;
	state.m_Color[2] = m_LinearFloatLightColor.z;
	state.m_Color[3] = 0.0f; // fixme: need to make ambient work m_flAmbient;
	state.m_NearZ = m_flNearZ;
	state.m_FarZ = m_flFarZ;
	state.m_flShadowSlopeScaleDepthBias = mat_slopescaledepthbias_shadowmap.GetFloat();
	state.m_flShadowDepthBias = mat_depthbias_shadowmap.GetFloat();
	state.m_bEnableShadows = m_bEnableShadows;
	state.m_pSpotlightTexture = materials->FindTexture( m_SpotlightTextureName, TEXTURE_GROUP_OTHER, false );
	state.m_nSpotlightTextureFrame = m_nSpotlightTextureFrame;

	state.m_nShadowQuality = m_nShadowQuality; // Allow entity to affect shadow quality

	if( m_LightHandle == CLIENTSHADOW_INVALID_HANDLE )
	{
		m_LightHandle = g_pClientShadowMgr->CreateFlashlight( state );
	}
	else
	{
		if (m_hTargetEntity != NULL || m_bForceUpdate == true)
		{
			g_pClientShadowMgr->UpdateFlashlightState( m_LightHandle, state );
		}
	}

	if( m_bLightOnlyTarget )
	{
		g_pClientShadowMgr->SetFlashlightTarget( m_LightHandle, m_hTargetEntity );
	}
	else
	{
		g_pClientShadowMgr->SetFlashlightTarget( m_LightHandle, NULL );
	}

	g_pClientShadowMgr->SetFlashlightLightWorld( m_LightHandle, m_bLightWorld );

	//if ( m_bForceUpdate == false )
	if (m_bAlwaysUpdate == true) //DONT FUCKING TOUCH THIS OR I WILL KILL YOU
	{
		g_pClientShadowMgr->UpdateProjectedTexture( m_LightHandle, true );
	}
}

void C_EnvProjectedTexture::Simulate( void )
{	
	UpdateLight();
	BaseClass::Simulate();
}

