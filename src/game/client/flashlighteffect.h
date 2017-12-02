//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#ifndef FLASHLIGHTEFFECT_H
#define FLASHLIGHTEFFECT_H
#ifdef _WIN32
#pragma once
#endif

struct dlight_t;


class CFlashlightEffect
{
public:

	CFlashlightEffect(int nEntIndex = 0);
	virtual ~CFlashlightEffect();

#ifndef C17
	virtual void UpdateLight(const Vector &vecPos, const Vector &vecDir, const Vector &vecRight, const Vector &vecUp, int nDistance);
#else
	virtual void UpdateLight(const Vector &vecPos, const Vector &vecDir, const Vector &vecRight, const Vector &vecUp, int nDistance, bool bMuzzleFlash);
#endif
	void TurnOn();
	void TurnOff();
	bool IsOn( void ) { return m_bIsOn;	}

	ClientShadowHandle_t GetFlashlightHandle( void ) { return m_FlashlightHandle; }
	void SetFlashlightHandle( ClientShadowHandle_t Handle ) { m_FlashlightHandle = Handle;	}
	
protected:

	void LightOff();
	void LightOffOld();
	void LightOffNew();

#ifdef C17
	void UpdateLightNew(const Vector &vecPos, const Vector &vecDir, const Vector &vecRight, const Vector &vecUp, bool bMuzzleFlash);
#else
	void UpdateLightNew(const Vector &vecPos, const Vector &vecDir, const Vector &vecRight, const Vector &vecUp);
#endif
	void UpdateLightOld(const Vector &vecPos, const Vector &vecDir, int nDistance);

	bool m_bIsOn;
#ifdef C17
	bool m_bUseShadows;
#endif
	int m_nEntIndex;
	ClientShadowHandle_t m_FlashlightHandle;

	// Vehicle headlight dynamic light pointer
	dlight_t *m_pPointLight;
	float m_flDistMod;

	// Texture for flashlight
	CTextureReference m_FlashlightTexture;
#ifdef C17
	CTextureReference m_MuzzlelightTexture;
	CTextureReference m_ARMuzzlelightTexture;
#endif
};

class CHeadlightEffect : public CFlashlightEffect
{
public:
	
	CHeadlightEffect();
	~CHeadlightEffect();

	virtual void UpdateLight(const Vector &vecPos, const Vector &vecDir, const Vector &vecRight, const Vector &vecUp, int nDistance);
};



#endif // FLASHLIGHTEFFECT_H
