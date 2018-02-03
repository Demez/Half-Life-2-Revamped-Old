#ifndef MISSION_CHOOSER_INT_H
#define MISSION_CHOOSER_INT_H
#ifdef _WIN32
#pragma once
#endif

#include "appframework/IAppSystem.h"
#include "utlvector.h"

class IHL2R_Random_Missions;
class IHL2R_Mission_Chooser_Source;
class IHL2R_Map_Builder;
class KeyValues;
class CUniformRandomStream;
class Color;
class IHL2RSpawnSelection;

class IHL2R_Location;

class IHL2R_Location_Group
{
public:
	virtual const char* GetGroupName() = 0;
	virtual const char* GetTitleText() = 0;
	virtual const char* GetDescriptionText() = 0;
	virtual const char* GetImageName() = 0;	
	virtual Color& GetColor() = 0;
	virtual bool IsGroupLocked( CUtlVector<int> &completedMissions ) = 0;

	virtual int GetHighestUnlockMissionID() = 0;
	virtual int GetNumLocations() = 0;
	virtual IHL2R_Location* GetLocation( int iIndex ) = 0;
};

enum HL2R_Reward_Type
{
	HL2R_REWARD_MONEY=0,
	HL2R_REWARD_XP,
	HL2R_REWARD_ITEM,
	HL2R_REWARD_SKILL_SLOT,
};

class IHL2R_Reward
{
public:
	virtual HL2R_Reward_Type GetRewardType() = 0;
	virtual int GetRewardAmount() = 0;

	// for item rewards
	virtual const char* GetRewardName() = 0;
	virtual int GetRewardLevel() = 0;
	virtual int GetRewardQuality() = 0;	
};

class IHL2R_Location
{
public:
	virtual int GetID() = 0;
	virtual IHL2R_Location_Group* GetGroup() = 0;
	virtual int GetDifficulty() = 0;
	virtual bool GetCompleted() = 0;
	virtual int GetXPos() = 0;
	virtual int GetYPos() = 0;
	// Mission Settings are settings not directly related to the level layout, but which have other effects on the mission.
	// (This is the "mission_settings" sub-node of the mission definition).
	virtual KeyValues* GetMissionSettings() = 0;
	// Mission Definition is the key-values block fed to the layout system.
	virtual KeyValues* GetMissionDefinition() = 0;
	
	virtual const char* GetMapName() = 0;
	virtual const char* GetStoryScene() = 0;
	virtual const char* GetImageName() = 0;
	virtual const char* GetCompanyName() = 0;
	virtual const char* GetCompanyImage() = 0;
	virtual int GetNumRewards() = 0;
	virtual IHL2R_Reward* GetReward( int iRewardIndex ) = 0;
	virtual int GetMoneyReward() = 0;
	virtual int GetXPReward() = 0;
	virtual int IsMissionOptional() = 0;
	virtual bool IsLocationLocked( CUtlVector<int> &completedMissions ) = 0;
	virtual void SetPos( int x, int y ) = 0;
};

class IHL2R_Location_Grid
{
public:
	virtual int GetNumGroups() = 0;
	virtual IHL2R_Location_Group* GetGroup( int iIndex ) = 0;
	virtual IHL2R_Location_Group* GetGroupByName( const char *szName ) = 0;
	virtual IHL2R_Location* GetLocationByID( int iLocationID ) = 0;

	virtual void SetLocationComplete( int iLocationID ) = 0;	
};

class IHL2R_Mission_Text_Database
{
public:
	virtual const char *GetShortDescriptionByID( unsigned int id ) = 0;	
	virtual const char *GetLongDescriptionByID( unsigned int id ) = 0;

	/// a unique identifier for each mission spec. 
	/// if they're loaded from the same files in the same
	/// order by the database on both server and client,
	/// we can refer to specs by this id across the network.
	// if you change the size of this, also update the m_nObjectiveTextIdx
	// datatable field in HL2R_objective.cpp. 
	typedef uint16 ID_t;
	enum { INVALID_INDEX = ((ID_t)(~0)) };
	static inline bool IsIDValid( ID_t idx ) { return idx != INVALID_INDEX; }

	/// find the mission text id for a given entity name, mission filename, other criteria.
	/// you can resolve the ID to textual descriptions with GetShortDescriptionByID() etc.
	virtual ID_t FindMissionTextID( const char *pEntityName, const char *pMissionName ) = 0;

};

//-----------------------------------------------------------------------------
// Purpose: Interface exposed from the mission chooser .dll (to the game dlls and tilegen.exe)
//-----------------------------------------------------------------------------
abstract_class IHL2R_Mission_Chooser : public IAppSystem
{
public:
	virtual bool GetCurrentTimeAndDate(int *year, int *month, int *dayOfWeek, int *day, int *hour, int *minute, int *second) = 0;

	//virtual IHL2R_Random_Missions *RandomMissions() = 0;
	virtual IHL2R_Mission_Chooser_Source *LocalMissionSource() = 0;	
	//virtual IHL2R_Location_Grid *LocationGrid() = 0;
	//virtual IHL2R_Mission_Text_Database *MissionTextDatabase() = 0;
	//virtual IHL2R_Map_Builder *MapBuilder() = 0;
	//virtual IHL2RSpawnSelection *SpawnSelection() = 0;
};

#define HL2R_MISSION_CHOOSER_VERSION		"VHL2RMissionChooser001"

#endif // MISSION_CHOOSER_INT_H
