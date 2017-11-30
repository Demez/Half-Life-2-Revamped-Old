//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifndef C_ENVPROJECTEDTEXTURE_H
#define C_ENVPROJECTEDTEXTURE_H
#ifdef _WIN32
#pragma once
#endif

#include "c_baseentity.h"
#include "basetypes.h"

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class C_EnvProjectedTexture : public C_BaseEntity
{
	DECLARE_CLASS(C_EnvProjectedTexture, C_BaseEntity);
public:
	DECLARE_CLIENTCLASS();

	//city17ep1 projtex
	//void SetLightColor(byte r, byte g, byte b, byte a);
	//

	virtual void OnDataChanged(DataUpdateType_t updateType);
	void	ShutDownLightHandle(void);

	virtual void Simulate(); //this was changed to a bool, but i dont even want to bother with this. its too much changing

	void	UpdateLight( void );

	C_EnvProjectedTexture();
	~C_EnvProjectedTexture();

	static void SetVisibleBBoxMinHeight(float flVisibleBBoxMinHeight) { m_flVisibleBBoxMinHeight = flVisibleBBoxMinHeight; }
	static float GetVisibleBBoxMinHeight(void) { return m_flVisibleBBoxMinHeight; }
	static C_EnvProjectedTexture *Create();

private:
	
	inline bool IsBBoxVisible(void);
	bool IsBBoxVisible(Vector vecExtentsMin,
					   Vector vecExtentsMax);

	ClientShadowHandle_t m_LightHandle;

	//projtex caching
	bool m_bForceUpdate;
	//

	EHANDLE	m_hTargetEntity;

	bool	m_bState;
	//projtex caching		
	bool	m_bAlwaysUpdate;
	//
	float	m_flLightFOV;
	bool	m_bEnableShadows;
	bool	m_bLightOnlyTarget;
	bool	m_bLightWorld;
	bool	m_bCameraSpace;
	//projtex brightness
	//float	m_flBrightnessScale;
	//
	Vector	m_LinearFloatLightColor;
	//city17ep1 projtex
	/*color32     m_LightColor;
	Vector		m_CurrentLinearFloatLightColor;
	float		m_flCurrentLinearFloatLightAlpha;
	float		m_flColorTransitionTime;
	float		m_fBrightness;*/
	//
	float	m_flAmbient;
	float	m_flNearZ;
	float	m_flFarZ;
	char	m_SpotlightTextureName[MAX_PATH];
	int		m_nSpotlightTextureFrame;
	int		m_nShadowQuality;

	Vector	m_vecExtentsMin;
	Vector	m_vecExtentsMax;

	static float m_flVisibleBBoxMinHeight;
};

bool C_EnvProjectedTexture::IsBBoxVisible(void)
{
	return IsBBoxVisible(GetAbsOrigin() + m_vecExtentsMin, GetAbsOrigin() + m_vecExtentsMax);
}

//C_EnvProjectedTexture* GetEnvProjectedTextureList();

#endif // C_ENVPROJECTEDTEXTURE_H
