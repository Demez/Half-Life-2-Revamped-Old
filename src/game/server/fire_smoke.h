//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//
//=============================================================================//
#ifndef FIRE_SMOKE_H
#define FIRE_SMOKE_H
#pragma once

#include "baseparticleentity.h"

//==================================================
// CBaseFire
//==================================================

//NOTENOTE: Reserved for all descendants
#define	bitsFIRE_NONE	0x00000000
#define	bitsFIRE_ACTIVE	0x00000001

class CBaseFire : public CBaseEntity
{
public:
	DECLARE_DATADESC();
	DECLARE_CLASS( CBaseFire, CBaseEntity );

	CBaseFire( void );
	virtual	~CBaseFire( void );

	virtual void	Scale( float size, float time );
#ifdef C17
	virtual void	Scale(float start, float size, float time, int particlesize);
#else
	virtual void	Scale( float start, float size, float time );
#endif
	virtual void	Enable( int state = true );

	//Client-side
	CNetworkVar( float, m_flStartScale );
	CNetworkVar( float, m_flScale );
	CNetworkVar( float, m_flScaleTime );
	CNetworkVar( int, m_nFlags );
#ifdef C17
	CNetworkVar(int, m_nParticleScale);
#endif
};

//==================================================
// CFireSmoke
//==================================================

//NOTENOTE: Mirrored in cl_dll/c_fire_smoke.cpp
#define	bitsFIRESMOKE_SMOKE					0x00000002
#define	bitsFIRESMOKE_SMOKE_COLLISION		0x00000004
#define	bitsFIRESMOKE_GLOW					0x00000008
#define	bitsFIRESMOKE_VISIBLE_FROM_ABOVE	0x00000010

class CFireSmoke : public CBaseFire
{
public:
	DECLARE_CLASS( CFireSmoke, CBaseFire );

	CFireSmoke( void );
	virtual	~CFireSmoke( void );

	void	Spawn();
	void	Precache();
	void	EnableSmoke( int state = true );
#ifdef C17
	void	EnableCollidingSmoke(int state = true);
#endif
	void	EnableGlow( int state = true );
	void	EnableVisibleFromAbove( int state = true );
	
	DECLARE_SERVERCLASS();
	DECLARE_DATADESC();

public:

	//Client-side
	CNetworkVar( int, m_nFlameModelIndex );
	CNetworkVar( int, m_nFlameFromAboveModelIndex );

	//Server-side
};

#endif	//FIRE_SMOKE_H
