//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: Draws the normal TF2 or HL2 HUD.
//
// $Workfile:     $
// $Date:         $
// $NoKeywords: $
//=============================================================================//
#include "cbase.h"
#include "clientmode_hl2mpnormal.h"
#include "vgui_int.h"
#include "hud.h"
#include <vgui/IInput.h>
#include <vgui/IPanel.h>
#include <vgui/ISurface.h>
#include <vgui_controls/AnimationController.h>
#include "iinput.h"
#include "hl2mpclientscoreboard.h"
#include "hl2mptextwindow.h"
#include "ienginevgui.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"
//-----------------------------------------------------------------------------
// Globals
//-----------------------------------------------------------------------------
vgui::HScheme g_hVGuiCombineScheme = 0;

static IClientMode *g_pClientMode[MAX_SPLITSCREEN_PLAYERS];
IClientMode *GetClientMode()
{
	ASSERT_LOCAL_PLAYER_RESOLVABLE();
	return g_pClientMode[GET_ACTIVE_SPLITSCREEN_SLOT()];
}

// --------------------------------------------------------------------------------- //
// CHL2MPModeManager.
// --------------------------------------------------------------------------------- //

// Instance the singleton and expose the interface to it.
ClientModeHL2MPNormal g_ClientModeNormal[ MAX_SPLITSCREEN_PLAYERS ];
IClientMode *GetClientModeNormal()
{
	//static ClientModeHL2MPNormal g_ClientModeNormal[ MAX_SPLITSCREEN_PLAYERS ];
	//return &g_ClientModeNormal;

	Assert(engine->IsLocalPlayerResolvable());
	return &g_ClientModeNormal[engine->GetActiveSplitScreenPlayerSlot()];
}

ClientModeHL2MPNormal* GetClientModeHL2MPNormal()
{
	/*Assert( dynamic_cast< ClientModeHL2MPNormal* >( GetClientModeNormal() ) );
	return static_cast< ClientModeHL2MPNormal* >( GetClientModeNormal() );*/

	Assert(engine->IsLocalPlayerResolvable());
	return &g_ClientModeNormal[engine->GetActiveSplitScreenPlayerSlot()];
}

//-----------------------------------------------------------------------------
// Purpose: this is the viewport that contains all the hud elements
//-----------------------------------------------------------------------------
class CHudViewport : public CBaseViewport
{
private:
	DECLARE_CLASS_SIMPLE( CHudViewport, CBaseViewport );

protected:
	virtual void ApplySchemeSettings( vgui::IScheme *pScheme )
	{
		BaseClass::ApplySchemeSettings( pScheme );

		GetHud().InitColors(pScheme);

		SetPaintBackgroundEnabled( false );
	}

	virtual IViewPortPanel *CreatePanelByName( const char *szPanelName );
};

int ClientModeHL2MPNormal::GetDeathMessageStartHeight( void )
{
	return m_pViewport->GetDeathMessageStartHeight();
}

IViewPortPanel* CHudViewport::CreatePanelByName( const char *szPanelName )
{
	IViewPortPanel* newpanel = NULL;

	if ( Q_strcmp( PANEL_SCOREBOARD, szPanelName) == 0 )
	{
		newpanel = new CHL2MPClientScoreBoardDialog( this );
		return newpanel;
	}
	else if ( Q_strcmp(PANEL_INFO, szPanelName) == 0 )
	{
		newpanel = new CHL2MPTextWindow( this );
		return newpanel;
	}
	else if ( Q_strcmp(PANEL_SPECGUI, szPanelName) == 0 )
	{
		newpanel = new CHL2MPSpectatorGUI( this );	
		return newpanel;
	}

	
	return BaseClass::CreatePanelByName( szPanelName ); 
}

//-----------------------------------------------------------------------------
// ClientModeHLNormal implementation
//-----------------------------------------------------------------------------
ClientModeHL2MPNormal::ClientModeHL2MPNormal()
{
	m_pViewport = new CHudViewport();
	m_pViewport->Start( gameuifuncs, gameeventmanager );
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
ClientModeHL2MPNormal::~ClientModeHL2MPNormal()
{
}

//--------------------------------------------------------------------------------------------------------

// See interface.h/.cpp for specifics:  basically this ensures that we actually Sys_UnloadModule the dll and that we don't call Sys_LoadModule 
//  over and over again.
static CDllDemandLoader g_GameUI("gameui");

class FullscreenHL2MPViewport : public CHudViewport
{
private:
	DECLARE_CLASS_SIMPLE(FullscreenHL2MPViewport, CHudViewport);

private:
	virtual void InitViewportSingletons(void)
	{
		SetAsFullscreenViewportInterface();
	}
};

class ClientModeHL2MPNormalFullscreen : public	ClientModeHL2MPNormal
{
	DECLARE_CLASS_SIMPLE(ClientModeHL2MPNormalFullscreen, ClientModeHL2MPNormal);
public:
	virtual void InitViewport()
	{
		// Skip over BaseClass!!!
		BaseClass::BaseClass::InitViewport();
		m_pViewport = new FullscreenHL2MPViewport();
		m_pViewport->Start(gameuifuncs, gameeventmanager);
	}
	virtual void Init()
	{
		// 
		//CASW_VGUI_Debug_Panel *pDebugPanel = new CASW_VGUI_Debug_Panel( GetViewport(), "ASW Debug Panel" );
		//g_hDebugPanel = pDebugPanel;

		// Skip over BaseClass!!!
		BaseClass::BaseClass::Init();

		// Load up the combine control panel scheme
		if (!g_hVGuiCombineScheme)
		{
			g_hVGuiCombineScheme = vgui::scheme()->LoadSchemeFromFileEx(enginevgui->GetPanel(PANEL_CLIENTDLL), IsXbox() ? "resource/ClientScheme.res" : "resource/CombinePanelScheme.res", "CombineScheme");
			if (!g_hVGuiCombineScheme)
			{
				Warning("Couldn't load combine panel scheme!\n");
			}
		}
	}
	void Shutdown()
	{
	}
};

//--------------------------------------------------------------------------------------------------------
static ClientModeHL2MPNormalFullscreen g_FullscreenClientMode;
IClientMode *GetFullscreenClientMode(void)
{
	return &g_FullscreenClientMode;
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void ClientModeHL2MPNormal::Init()
{
	BaseClass::Init();

	// Load up the combine control panel scheme
	g_hVGuiCombineScheme = vgui::scheme()->LoadSchemeFromFileEx( enginevgui->GetPanel( PANEL_CLIENTDLL ), "resource/CombinePanelScheme.res", "CombineScheme" );
	if (!g_hVGuiCombineScheme)
	{
		Warning( "Couldn't load combine panel scheme!\n" );
	}
}

void ClientModeHL2MPNormal::DoPostScreenSpaceEffects(const CViewSetup *pSetup)
{

}

