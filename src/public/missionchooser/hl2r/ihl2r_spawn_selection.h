//============ Copyright (c) Valve Corporation, All rights reserved. ============
//
// 
//
//===============================================================================
#ifndef IHL2R_SPAWN_SELECTION_H
#define IHL2R_SPAWN_SELECTION_H
#ifdef _WIN32
#pragma once
#endif

#include "tier1/strtools.h"
#include "mathlib/vector.h"

class IHL2RSpawnDefinitionEntry;

// NOTE: Text version of these enum types is in HL2R_spawn_selection.cpp!
enum
{
	HL2R_NPC_SPAWN_TYPE_INVALID = -1,
	HL2R_NPC_SPAWN_TYPE_ANY = 0,
	HL2R_NPC_SPAWN_TYPE_FIXED = 1,
	HL2R_NPC_SPAWN_TYPE_WANDERER,
	HL2R_NPC_SPAWN_TYPE_SWARM,
	HL2R_NPC_SPAWN_TYPE_BOSS,
	HL2R_NPC_SPAWN_TYPE_PROP,
	HL2R_NPC_SPAWN_TYPE_ARENAWAVE,
	HL2R_NPC_SPAWN_TYPE_CONSOLE,
	HL2R_NPC_SPAWN_TYPE_SPAWNER,
	HL2R_NPC_SPAWN_TYPE_BIFURCATE,
	HL2R_NPC_SPAWN_TYPE_SPECIAL1,
	HL2R_NPC_SPAWN_TYPE_SPECIAL2,

	// Has to be last.
	HL2R_NPC_SPAWN_TYPE_COUNT		
};

class IHL2RSpawnDefinitionEntry
{
public:

	virtual const char *GetNPCClassname() = 0;
	virtual void GetSpawnCountRange( int &nMin, int &nMax ) = 0;
	virtual float GetEliteNPCChance( void ) = 0;
	virtual bool UseSpawners() = 0;
};


class IHL2RSpawnDefinition
{
public:

	virtual int GetEntryCount() = 0;
	virtual IHL2RSpawnDefinitionEntry *GetEntry( int nEntry ) = 0;
};


class IHL2RSpawnSelection
{
public:

	virtual IHL2RSpawnDefinition *GetSpawnDefinition( int nSpawnType ) = 0;
	virtual bool IsAvailableNPC( const char *szName ) = 0;

	virtual void SetCurrentSpawnSet( int iMissionDifficulty ) = 0;
	virtual bool SetCurrentSpawnSet( const char *szSetName ) = 0;

	virtual void DumpCurrentSpawnSet() = 0;
};

// fixed spawn encounter in the mission
class IHL2R_Encounter
{
public:
	virtual const Vector&				GetEncounterPosition() = 0;
	virtual int							GetNumSpawnDefs() = 0;
	virtual IHL2RSpawnDefinition*		GetSpawnDef( int i ) = 0;
	virtual float						GetEncounterRadius() = 0;
};

#endif // IHL2R_SPAWN_SELECTION_H
