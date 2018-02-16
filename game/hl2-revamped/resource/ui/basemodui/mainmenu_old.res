"Resource/UI/MainMenu.res"
{
	"MainMenu"
	{
		"ControlName"			"Frame"
		"fieldName"				"MainMenu"
		"xpos"					"0"
		"ypos"					"0"
		"wide"					"f0"
		"tall"					"f0"
		"autoResize"			"0"
		"pinCorner"				"0"
		"visible"				"1"
		"enabled"				"1"
		"tabPosition"			"0"
		"PaintBackgroundType"	"0"
	}
	"BtnGameModes"
	{
		"ControlName"			"GameModes"
		"fieldName"				"BtnGameModes"

		"tabPosition"			"1"
		"navUp"					"BtnQuit"			[$X360]
		"navUp"					"PnlQuickJoin"		[$WIN32]
		"navDown"				"BtnChangeGamers"	[$X360]
		"navDown"				"BtnStatsAndAchievements"	[$WIN32]

		"xpos"					"0"
		"ypos"					"80"
		
		// needed to push the game modes carousel behind the other main menu buttons
		// that overlap into the carousel rect (which needs be oversized due to flyouts)
		// and would not get mouse hittests
		"zpos"					-10				[$WIN32]
		
		"borderimage"			"vgui/menu_mode_border"	

		"leftarrow"				"vgui/arrow_left"	
		"rightarrow"			"vgui/arrow_right"	
		"arrowwidth"			21
		"arrowheight"			21
		"arrowoffsety"			-3
		"rightarrowoffsetx"		7		[$WIN32]

		// main pic, vertically centered
		"picoffsetx"			100
		"picwidth"				156
		"picheight"				156

		// menu text, underneath pic
		"menutitlex"			100
		"menutitley"			16
		// the 360 doesn't clip text, these can be more exact to the fat main menu button
		"menutitlewide"			180	[$X360 && (!$GERMAN && !$FRENCH && !$POLISH)]
		"menutitlewide"			230	[$X360 && ($GERMAN || $FRENCH)]
		"menutitlewide"			240	[$X360 && $POLISH]
		// the pc clips text, these need to be wider for the hint, but then use wideatopen to foreshorten the flyout
		"menutitlewide"			500	[$WIN32]
		"wideatopen"			180	[$WIN32 && (!$GERMAN && !$FRENCH && !$POLISH)]
		"wideatopen"			230	[$WIN32 && ($GERMAN || $FRENCH)]
		"wideatopen"			240	[$WIN32 && $POLISH]
		"menutitletall"			80

		// small pics, vertically centered, to the right of the main pic
		"subpics"				5		[$X360WIDE || $WIN32WIDE]
		"subpics"				3		[!($X360WIDE || $WIN32WIDE)]
		"subpicgap"				6		// between pics
		"subpicoffsetx"			20		[$X360]
		"subpicoffsetx"			50		[$WIN32]
		"subpicoffsety"			-10
		"subpicwidth"			86	
		"subpicheight"			86
		"subpicnamefont"		"Default"
		"hidelabels"			"1"		[!$ENGLISH]			

		"mode"
		{
			"id"				"BtnSingleplayer"
			"name"				"#REVAMPEDUI_MainMenu_Singleplayer"
			"image"				"vgui/hl2/menu_mode_singleplayer"
			"command"			"FlmSingleplayerFlyout"
			"menutitle"			"#REVAMPEDUI_MainMenu_Singleplayer"
			"menuhint"			"#REVAMPEDUI_MainMenu_Singleplayer_Tip"
		}
		
		"mode"
		{
			"id"				"BtnMultiplayer"
			"name"				"#REVAMPEDUI_MainMenu_Coop"
			"image"				"vgui/hl2/menu_mode_coop"
			"command"			"FlmCoopFlyout"
			"menutitle"			"#REVAMPEDUI_MainMenu_Coop"
			"menuhint"			"#REVAMPEDUI_MainMenu_Coop_Tip"
		}
		
		"mode"
		{
			"id"				"BtnSplitScreen"
			"name"				"#REVAMPEDUI_MainMenu_SplitScreen"
			"image"				"vgui/hl2/menu_mode_splitscreen"
			"command"			"FlmSplitScreenFlyout"
			"menutitle"			"#REVAMPEDUI_MainMenu_SplitScreen"
			"menuhint"			"#REVAMPEDUI_MainMenu_Splitscreen_Tip"
		}
		
		"mode"
		{
			"id"				"BtnDeathmatch"
			"name"				"#REVAMPEDUI_MainMenu_DM"
			"image"				"vgui/hl2/menu_mode_deathmatch"
			"command"			"FlmDeathmatchFlyout"
			"menutitle"			"#REVAMPEDUI_MainMenu_DM"
			"menuhint"			"#REVAMPEDUI_MainMenu_DM_Tip"
		}
	}
	"BtnStatsAndAchievements"
	{
		"ControlName"			"BaseModHybridButton"
		"fieldName"				"BtnStatsAndAchievements"
		"xpos"					"80"
		"ypos"					"300"
		"wide"					"180"
		"tall"					"20"
		"autoResize"			"1"
		"pinCorner"				"0"
		"visible"				"0"		[$X360GUEST]
		"enabled"				"0"		[$X360GUEST]
		"visible"				"1"		[!$X360GUEST]		// jms: TEMP disabled
		"enabled"				"1"		[!$X360GUEST]
		"tabPosition"			"0"
		"navUp"					"BtnChangeGamers"
		"navDown"				"BtnOptions"
		"labelText"				"#L4D360UI_MainMenu_StatsAndAchievements"
		"tooltiptext"			"#L4D360UI_MainMenu_StatsAndAchievements_Tip"	[$X360]
		"tooltiptext"			"#L4D360UI_MainMenu_PCStatsAndAchievements_Tip"	[$WIN32]
		"style"					"AlienSwarmMenuButtonSmall"
		"command"				"StatsAndAchievements"
		"ActivationType"		"1"
		"EnableCondition"		"Never" [$DEMO]
	}
	"BtnOptions"
	{
		"ControlName"			"BaseModHybridButton"
		"fieldName"				"BtnOptions"
		"xpos"					"80"
		"ypos"					"320"
		"wide"					"100"
		"tall"					"20"
		"autoResize"			"1"
		"pinCorner"				"0"
		"visible"				"1"
		"enabled"				"1"
		"tabPosition"			"0"
		"navUp"					"BtnStatsAndAchievements"
		"navDown"				"BtnExtras"
		"labelText"				"#L4D360UI_MainMenu_Options"
		"tooltiptext"			"#L4D360UI_MainMenu_Options_Tip"
		"style"					"AlienSwarmMenuButtonSmall"
		"command"				"FlmOptionsFlyout"
		"ActivationType"		"1"
	}
	
	"BtnExtras"
	{
		"ControlName"			"BaseModHybridButton"
		"fieldName"				"BtnExtras"
		"xpos"					"80"
		"ypos"					"340" 
		"wide"					"180"
		"tall"					"20"
		"autoResize"			"1"
		"pinCorner"				"0"
		"visible"				"0"		[!$X360 || !$DEMO]
		"visible"				"0"		[$X360 && $DEMO]		
		"enabled"				"1"
		"tabPosition"			"0"
		"navUp"					"BtnOptions"
		"navDown"				"BtnQuit"
		"style"					"AlienSwarmMenuButtonSmall"
		"ActivationType"		"1"
		"labelText"				"#L4D360UI_MainMenu_Extras"
		"tooltiptext"			"#L4D360UI_MainMenu_Extras_Tip"
		"command"				"FlmExtrasFlyoutCheck"
		"EnableCondition"		"Never" [$DEMO]
	}
	
	"BtnQuit"
	{
		"ControlName"			"BaseModHybridButton"
		"fieldName"				"BtnQuit"
		"xpos"					"80"
		"ypos"					"360"	[$WIN32]
		"wide"					"180"
		"tall"					"12"
		"autoResize"			"1"
		"pinCorner"				"0"
		"visible"				"1"		[$WIN32]
		"enabled"				"1"
		"tabPosition"			"0"
		"navUp"					"BtnExtras"
		"navDown"				"PnlQuickJoin" [$WIN32]
		"navDown"				"BtnGameModes" [$X360]
		"style"					"AlienSwarmMenuButtonSmall"
		"ActivationType"		"1"
		"labelText"				"#L4D360UI_MainMenu_Quit"			[$WIN32]
		"tooltiptext"			"#L4D360UI_MainMenu_Quit_Tip"		[$WIN32]
		"command"				"QuitGame"
	}
	"FlmSplitScreenFlyout"
	{
		"ControlName"			"FlyoutMenu"
		"fieldName"				"FlmSplitScreenFlyout"
		"visible"				"0"
		"wide"					"0"
		"tall"					"0"
		"zpos"					"3"
		"InitialFocus"			"BtnCreateGame"
		"ResourceFile"			"resource/UI/basemodui/SingleplayerFlyout.res" // change this
	}
	"FlmDeathmatchFlyout"
	{
		"ControlName"			"FlyoutMenu"
		"fieldName"				"FlmDeathmatchFlyout"
		"visible"				"0"
		"wide"					"0"
		"tall"					"0"
		"zpos"					"3"
		"InitialFocus"			"BtnCreateGame"
		"ResourceFile"			"resource/UI/basemodui/CampaignFlyout.res" // change this
	}
	"FlmCoopFlyout"
	{
		"ControlName"			"FlyoutMenu"
		"fieldName"				"FlmCoopFlyout"
		"visible"				"0"
		"wide"					"0"
		"tall"					"0"
		"zpos"					"3"
		"InitialFocus"			"BtnCreateGame"
		"ResourceFile"			"resource/UI/basemodui/CampaignFlyout.res" // change this
	}
	"FlmSingleplayerFlyout"
	{
		"ControlName"			"FlyoutMenu"
		"fieldName"				"FlmSingleplayerFlyout"
		"visible"				"0"
		"wide"					"0"
		"tall"					"0"
		"zpos"					"3"
		"InitialFocus"			"BtnSingleplayer"
		"ResourceFile"			"resource/UI/basemodui/SingleplayerFlyout.res"
	}
	
	"FlmOptionsFlyout"
	{
		"ControlName"			"FlyoutMenu"
		"fieldName"				"FlmOptionsFlyout"
		"visible"				"0"
		"wide"					"0"
		"tall"					"0"
		"zpos"					"3"
		"InitialFocus"			"BtnGameplay"
		"ResourceFile"			"resource/UI/basemodui/OptionsFlyout.res"
	}

	"FlmExtrasFlyout_Simple"
	{
		"ControlName"			"FlyoutMenu"
		"fieldName"				"FlmExtrasFlyout_Simple"
		"visible"				"0"
		"wide"					"0"
		"tall"					"0"
		"zpos"					"3"
		"InitialFocus"			"BtnCommentary"
		"ResourceFile"			"resource/UI/basemodui/ExtrasFlyout.res"
	}

	"FlmExtrasFlyout_Live"
	{
		"ControlName"			"FlyoutMenu"
		"fieldName"				"FlmExtrasFlyout_Live"
		"visible"				"0"
		"wide"					"0"
		"tall"					"0"
		"zpos"					"3"
		"InitialFocus"			"BtnCommunity"
		"ResourceFile"			"resource/UI/basemodui/ExtrasFlyoutLive.res"
	}
}
