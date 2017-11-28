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

	virtual void OnDataChanged(DataUpdateType_t updateType);
	void	ShutDownLightHandle(void);

	virtual void Simulate(); //this was changed to a bool, but i dont even want to bother with this. its too much changing

	void	UpdateLight( void );

	C_EnvProjectedTexture();
	~C_EnvProjectedTexture();

	static C_EnvProjectedTexture *Create();

private:

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
	Vector	m_LinearFloatLightColor;
	float	m_flAmbient;
	float	m_flNearZ;
	float	m_flFarZ;
	char	m_SpotlightTextureName[MAX_PATH];
	int		m_nSpotlightTextureFrame;
	int		m_nShadowQuality;
};

//C_EnvProjectedTexture* GetEnvProjectedTextureList();

#endif // C_ENVPROJECTEDTEXTURE_H
