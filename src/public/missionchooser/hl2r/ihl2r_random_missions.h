#ifndef _INCLUDED_IHL2R_RANDOM_MISSIONS_H
#define _INCLUDED_IHL2R_RANDOM_MISSIONS_H
#ifdef _WIN32
#pragma once
#endif

#include "ihl2r_mission_chooser.h"

namespace vgui
{
	class Panel;
};

class Vector;
class KeyValues;
class IHL2R_Encounter;

enum
{
	HL2R_TILETYPE_UNKNOWN = 0,
	HL2R_TILETYPE_OUTDOOR1,
	HL2R_TILETYPE_OUTDOOR2,
	HL2R_TILETYPE_ARENA1,
	HL2R_TILETYPE_ARENA2,
	HL2R_TILETYPE_ARENA3,
	HL2R_TILETYPE_ROOM2,
	HL2R_TILETYPE_ROOM1,
	HL2R_TILETYPE_CORRIDOR1,
	HL2R_TILETYPE_CORRIDOR2,
	HL2R_TILETYPE_VENTS,

	HL2R_TILETYPE_COUNT
};

static const char *g_szHL2RTileTypeStrings[HL2R_TILETYPE_COUNT] = 
{
	"Unknown",
	"Outdoor1",
	"Outdoor2",
	"Arena1",
	"Arena2",
	"Arena3",
	"Room1",
	"Room2",
	"Corridor1",
	"Corridor2",
	"Vents"
};

class IHL2R_Room_Details
{
public:
	// tags
	virtual bool			HasTag( const char *szTag ) = 0;
	virtual int				GetNumTags() = 0;
	virtual const char*		GetTag( int i ) = 0;
	virtual int				GetSpawnWeight() = 0;
	virtual int				GetNumExits() = 0;
	virtual IHL2R_Room_Details*	GetAdjacentRoom( int nExit ) = 0;
	virtual bool			GetThumbnailName( char* szOut, int iBufferSize ) = 0;
	virtual bool			GetFullRoomName( char* szOut, int iBufferSize ) = 0;
	virtual void			GetSoundscape( char* szOut, int iBufferSize ) = 0;
	virtual void			GetTheme( char* szOut, int iBufferSize ) = 0;
	virtual const Vector&	GetAmbientLight() = 0;
	virtual bool			HasAlienEncounter() = 0;
	virtual int				GetTileType() = 0;
	virtual const char*		GetTileTypeName( int nType ) = 0;
	virtual int				GetRoomIndex() const = 0;

	// location
	virtual void			GetWorldBounds( Vector *vecWorldMins, Vector *vecWorldMaxs ) = 0;
	virtual const Vector&	WorldSpaceCenter() = 0;
};

class IHL2R_Random_Missions
{
public:
	virtual vgui::Panel* CreateTileGenFrame( vgui::Panel *parent ) = 0;

	virtual void LevelInitPostEntity( const char *pszMapName ) = 0;
	virtual bool ValidMapLayout() = 0;
	virtual IHL2R_Room_Details* GetRoomDetails( const Vector &vecPos ) = 0;
	virtual IHL2R_Room_Details* GetRoomDetails( int iRoomIndex ) = 0;
	virtual IHL2R_Room_Details* GetStartRoomDetails() = 0;
	virtual int GetNumRooms() = 0;
	virtual void GetMapBounds( Vector *vecWorldMins, Vector *vecWorldMaxs ) = 0;
	virtual KeyValues* GetGenerationOptions() = 0;		// returns the generation options for the currently loaded random map
	virtual int				GetNumEncounters() = 0;
	virtual IHL2R_Encounter* GetEncounter( int i ) = 0;

	virtual bool CheckAndCleanDirtyLayout( void ) = 0;
};

#endif // _INCLUDED_IHL2R_RANDOM_MISSIONS_H
