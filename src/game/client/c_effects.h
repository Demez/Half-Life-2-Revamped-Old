//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#ifndef C_EFFECTS_H
#define C_EFFECTS_H
#ifdef _WIN32
#pragma once
#endif

#ifdef C17
#include "env_wind_shared.h"
#endif

// Draw rain effects.
void DrawPrecipitation();

#ifdef C17
//-----------------------------------------------------------------------------
// EnvWind - global wind info
//-----------------------------------------------------------------------------
class C_EnvWind : public C_BaseEntity
{
public:
	C_EnvWind();

	DECLARE_CLIENTCLASS();
	DECLARE_CLASS(C_EnvWind, C_BaseEntity);

	virtual void	OnDataChanged(DataUpdateType_t updateType);
	virtual bool	ShouldDraw(void) { return false; }

	virtual void	ClientThink();

	CEnvWindShared m_EnvWindShared;

private:
	C_EnvWind(const C_EnvWind &);
};
#endif

#endif // C_EFFECTS_H
