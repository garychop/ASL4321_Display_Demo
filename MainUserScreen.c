//*****************************************************************************
// Filename: MainUserScreen.c
// Description: This file handles the main screen functionality.
// Date: 2/24/21
// 
//*****************************************************************************

#include "ASL4321_Simulator_Main.h"
#include "ASL4321_Display_Demo_specifications.h"
#include "ASL4321_System.h"
#include "DataDictionary.h"

//*****************************************************************************

typedef struct MAIN_SCREEN_FEATURE_STRUCT
{
	FEATURE_ID_ENUM m_FeatureID;
    int m_Location;     // This indicates the Main Screen location, 0=Top most, 3=bottom most
//	int m_Available;	// This is true if this feature should be displayed for Enabling/Disabling. Typically based upon RNet setting.
//	int m_Enabled;      // Indicates if this feature is active.
//    GX_RESOURCE_ID m_SmallIcon;
 //   GX_RESOURCE_ID m_LargeIcon;
 //   GX_RESOURCE_ID m_SmallDescriptionID;
 //   GX_RESOURCE_ID m_LargeDescriptionID;
 //   GX_RESOURCE_ID m_FeatureIconID;
	//GX_WIDGET m_ItemWidget;
	//GX_PROMPT m_PromptWidget;
	//GX_CHECKBOX m_ButtonWidget;
 //   CUSTOM_CHECKBOX m_Checkbox;
} MAIN_SCREEN_FEATURE;

//*****************************************************************************

unsigned char g_Inhibit_UpButtonResponse = FALSE;
static int g_GroupIcon = 0;
MAIN_SCREEN_FEATURE g_MainScreenFeatureInfo[MAX_FEATURES];

//*****************************************************************************

VOID Initialize_MainScreenInfo()
{
	int feature, subFeature, priority, featureCount;
	USHORT rnet_active;
	USHORT group;

	rnet_active = (dd_Get_USHORT(0, DD_RNET_ENABLE));
	group = dd_Get_USHORT (0, DD_GROUP);

    // Populate the screen stuff.
	priority = 0;
	featureCount = 0;
	for (feature = 0; feature < MAX_FEATURES; ++ feature)
	{
		g_MainScreenFeatureInfo[feature].m_FeatureID = MAX_FEATURES;
		g_MainScreenFeatureInfo[feature].m_Location = 0;

		for (subFeature = 0; subFeature < MAX_FEATURES; ++ subFeature)
		{
			if ((g_GroupInfo[group].m_GroupFeature[subFeature].m_Available) && (g_GroupInfo[group].m_GroupFeature[subFeature].m_Enabled))
			{
				if (g_GroupInfo[group].m_GroupFeature[subFeature].m_Priority == priority)
				{
					if (priority == 0)	// Is this the active feature?
						dd_Set_USHORT (0, DD_ACTIVE_FEATURE, g_GroupInfo[group].m_GroupFeature[subFeature].m_FeatureID);
					g_MainScreenFeatureInfo[featureCount].m_FeatureID = g_GroupInfo[group].m_GroupFeature[subFeature].m_FeatureID;
					g_MainScreenFeatureInfo[featureCount].m_Location = featureCount;
					++featureCount;
					break;
				}
			}
		}
		++priority;
	}
}

//*************************************************************************************

VOID AdvanceToNextFeature (VOID)
{
	UINT feature;
	UINT selectedFeature;
	UINT lineNumber;

	// This is necessary to prevent the subsequent "Clicked" message from advancing the feature when we are changing to the Programming screen.
	if (g_ChangeScreen_WIP)
	{
		g_ChangeScreen_WIP = FALSE;
		return;
	}
	// Count the number of active features to set a limit on location
	for (feature = 0; feature < MAX_FEATURES; ++feature)
	{
		if (g_MainScreenFeatureInfo[feature].m_Location == 1)		// We found the active one
			selectedFeature = feature;
	}

	// Move active feature to bottom of list and move all feature upward.
	lineNumber = 0;
	for (feature = 0; feature < MAX_FEATURES; ++feature)
	{
		if (g_MainScreenFeatureInfo[selectedFeature].m_FeatureID != MAX_FEATURES)
		{
			g_MainScreenFeatureInfo[selectedFeature].m_Location = lineNumber;
			if (lineNumber == 0)
				dd_Set_USHORT (0, DD_ACTIVE_FEATURE, g_MainScreenFeatureInfo[selectedFeature].m_FeatureID);
			++lineNumber;
			// Set to look at the next feature
		}
		++selectedFeature;
		if (selectedFeature >= MAX_FEATURES)
			selectedFeature = 0;
	}
}

//*************************************************************************************

VOID AdvanceToPreviousFeature(VOID)
{
	UINT feature;
	int selectedFeature;
	int lineNumber;
	int highestLineNumber;

	// This is necessary to prevent the subsequent "Clicked" message from advancing the feature when we are changing to the Programming screen.
	if (g_ChangeScreen_WIP)
	{
		g_ChangeScreen_WIP = FALSE;
		return;
	}
	// look for the item with the highest line number. That will become the new active item.
	highestLineNumber = 0;
	for (feature = 0; feature < MAX_FEATURES; ++feature)
	{
		if (g_MainScreenFeatureInfo[feature].m_Location > highestLineNumber)
		{
			highestLineNumber = g_MainScreenFeatureInfo[feature].m_Location;
			selectedFeature = feature;	// remember this items location in the array.
		}
	}

	lineNumber = 0;
	for (feature = 0; feature < MAX_FEATURES; ++feature)
	{
		if (g_MainScreenFeatureInfo[selectedFeature].m_FeatureID != MAX_FEATURES)
		{
			g_MainScreenFeatureInfo[selectedFeature].m_Location = lineNumber;
			if (lineNumber == 0)
				dd_Set_USHORT (0, DD_ACTIVE_FEATURE, g_MainScreenFeatureInfo[selectedFeature].m_FeatureID);
			++lineNumber;
		}
		// Set to look at the next feature
		++selectedFeature;
		if (selectedFeature >= MAX_FEATURES)
			selectedFeature = 0;
	}
}

//*****************************************************************************
// This locates the NEXT GROUP FEATURE and sets it active.
VOID SetToNextGroupFeature (VOID)
{
	int selectedFeature, feature;
	int lineNumber;

	selectedFeature = 0;
	for (feature = 0; feature < MAX_FEATURES; ++feature)
	{
		if (g_MainScreenFeatureInfo[feature].m_FeatureID == NEXT_GROUP_ID)
		{
			selectedFeature = feature;
			break;
		}
	}

	lineNumber = 0;
	for (feature = 0; feature < MAX_FEATURES; ++feature)
	{
		if (g_MainScreenFeatureInfo[selectedFeature].m_FeatureID != MAX_FEATURES)
		{
			g_MainScreenFeatureInfo[selectedFeature].m_Location = lineNumber;
			if (lineNumber == 0)
				dd_Set_USHORT (0, DD_ACTIVE_FEATURE, g_MainScreenFeatureInfo[selectedFeature].m_FeatureID);
			++lineNumber;
		}
		// Set to look at the next feature
		++selectedFeature;
		if (selectedFeature >= MAX_FEATURES)
			selectedFeature = 0;
	}
}

//*****************************************************************************

void DisplayPadFeatures()
{
	UINT err;
	SHORT featureCount;
	FEATURE_ID_ENUM featureID;

	featureID = MAX_FEATURES;
	for (featureCount = 0; featureCount < MAX_FEATURES; ++featureCount)
	{
		if (g_MainScreenFeatureInfo[featureCount].m_FeatureID != MAX_FEATURES)
			if (g_MainScreenFeatureInfo[featureCount].m_Location == 0)
				featureID = g_MainScreenFeatureInfo[featureCount].m_FeatureID;	// abreviated variable
	}

	switch (featureID)
	{
	case AUDIBLE_OUT_FEATURE_ID:
		gx_widget_show (&MainUserScreen.MainUserScreen_ForwardPad_Button);
		gx_widget_show (&MainUserScreen.MainUserScreen_ReversePad_Button);
		gx_widget_show (&MainUserScreen.MainUserScreen_LeftPad_Button);
		gx_widget_show (&MainUserScreen.MainUserScreen_RightPad_Button);
		switch (g_ActiveSpeakerGroup)
		{
		default:
		case 0:
			gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_ForwardPad_Button, GX_PIXELMAP_ID_SPEAKER_A_88X70);
			gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_ReversePad_Button, GX_PIXELMAP_ID_PADDOWNARROW_DISABLED);
			gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_LeftPad_Button, GX_PIXELMAP_ID_SPEAKER_B_88X70);
			gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_RightPad_Button, GX_PIXELMAP_ID_SPEAKER_F_88X70);
			break;
		case 1:
			gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_ForwardPad_Button, GX_PIXELMAP_ID_PADUPARROW_DISABLED);
			gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_ReversePad_Button, GX_PIXELMAP_ID_PADDOWNARROW_DISABLED);
			gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_LeftPad_Button, GX_PIXELMAP_ID_SPEAKER_B_88X70);
			gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_RightPad_Button, GX_PIXELMAP_ID_SPEAKER_F_88X70);
			break;
		case 2:
			gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_ForwardPad_Button, GX_PIXELMAP_ID_PADUPARROW_DISABLED);
			gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_ReversePad_Button, GX_PIXELMAP_ID_PADDOWNARROW_DISABLED);
			gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_LeftPad_Button, GX_PIXELMAP_ID_SPEAKER_D_88X70);
			gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_RightPad_Button, GX_PIXELMAP_ID_SPEAKER_E_88X70);
			break;
		case 3:
			gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_ForwardPad_Button, GX_PIXELMAP_ID_PADUPARROW_DISABLED);
			gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_ReversePad_Button, GX_PIXELMAP_ID_PADDOWNARROW_DISABLED);
			gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_LeftPad_Button, GX_PIXELMAP_ID_SPEAKER_YES_88X70);
			gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_RightPad_Button, GX_PIXELMAP_ID_SPEAKER_NO_88X70);
			break;
		} // end switch
		break;
	case SEATING_FEATURE_ID:
		gx_widget_show (&MainUserScreen.MainUserScreen_ForwardPad_Button);
		gx_widget_show (&MainUserScreen.MainUserScreen_ReversePad_Button);
		gx_widget_show (&MainUserScreen.MainUserScreen_LeftPad_Button);
		gx_widget_show (&MainUserScreen.MainUserScreen_RightPad_Button);
		switch (g_ActiveSeatingGroup)
		{
		case 0:
			gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_ForwardPad_Button, GX_PIXELMAP_ID_PADUPARROW_DISABLED);
			gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_ReversePad_Button, GX_PIXELMAP_ID_PADDOWNARROW_DISABLED);
			gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_LeftPad_Button, GX_PIXELMAP_ID_TILTDOWN_88X70);
			gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_RightPad_Button, GX_PIXELMAP_ID_TILTUP_88X70);
			break;
		case 1:
			gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_ForwardPad_Button, GX_PIXELMAP_ID_PADUPARROW_DISABLED);
			gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_ReversePad_Button, GX_PIXELMAP_ID_PADDOWNARROW_DISABLED);
			gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_LeftPad_Button, GX_PIXELMAP_ID_RECLINEDOWN_88X70);
			gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_RightPad_Button, GX_PIXELMAP_ID_RECLINEUP_88X70);
			break;
		case 2:
			gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_ForwardPad_Button, GX_PIXELMAP_ID_PADUPARROW_DISABLED);
			gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_ReversePad_Button, GX_PIXELMAP_ID_PADDOWNARROW_DISABLED);
			gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_LeftPad_Button, GX_PIXELMAP_ID_SEATING_ELEVATE_DOWN_88X70);
			gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_RightPad_Button, GX_PIXELMAP_ID_SEATING_ELEVATE_UP_88X70);
			break;
		} // end switch 
		break;
	case BLUETOOTH_ID:
		gx_widget_show (&MainUserScreen.MainUserScreen_ForwardPad_Button);
		gx_widget_show (&MainUserScreen.MainUserScreen_ReversePad_Button);
		gx_widget_show (&MainUserScreen.MainUserScreen_LeftPad_Button);
		gx_widget_show (&MainUserScreen.MainUserScreen_RightPad_Button);
		switch (g_BluetoothGroup)
		{
		case 0:
			gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_ForwardPad_Button, GX_PIXELMAP_ID_MOUSEUPDOWN_88X70);
			gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_ReversePad_Button, GX_PIXELMAP_ID_PADDOWNARROW_DISABLED);
			gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_LeftPad_Button, GX_PIXELMAP_ID_MOUSELEFTCLICK_88X70);
			gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_RightPad_Button, GX_PIXELMAP_ID_MOUSELEFT_88X70);
			break;
		case 1:
			gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_ForwardPad_Button, GX_PIXELMAP_ID_PADUPARROW_DISABLED);
			gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_ReversePad_Button, GX_PIXELMAP_ID_PADDOWNARROW_DISABLED);
			gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_LeftPad_Button, GX_PIXELMAP_ID_LEFTWHITEARROW);
			gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_RightPad_Button, GX_PIXELMAP_ID_RIGHTWHITEARROW);
			break;
		} // end switch on Bluetooth group.
		break;
	case TECLA_E_FEATURE_ID:
		gx_widget_hide (&MainUserScreen.MainUserScreen_ForwardPad_Button);
		gx_widget_hide (&MainUserScreen.MainUserScreen_ReversePad_Button);
		gx_widget_hide (&MainUserScreen.MainUserScreen_LeftPad_Button);
		gx_widget_hide (&MainUserScreen.MainUserScreen_RightPad_Button);
		switch (GetTeclaGroup())
		{
		case 0:
			err = gx_prompt_text_id_set ((GX_PROMPT*) &MainUserScreen.MainUserScreen_TeclaMain_Prompt, GX_STRING_ID_STRING_123);
			break;
		case 1:
			err = gx_prompt_text_id_set ((GX_PROMPT*) &MainUserScreen.MainUserScreen_TeclaMain_Prompt, GX_STRING_ID_STRING_124);
			break;
		case 2:
			err = gx_prompt_text_id_set ((GX_PROMPT*) &MainUserScreen.MainUserScreen_TeclaMain_Prompt, GX_STRING_ID_STRING_125);
			break;
		} // end swtich
		break;
	default:
		gx_widget_show (&MainUserScreen.MainUserScreen_ForwardPad_Button);
		gx_widget_show (&MainUserScreen.MainUserScreen_ReversePad_Button);
		gx_widget_show (&MainUserScreen.MainUserScreen_LeftPad_Button);
		gx_widget_show (&MainUserScreen.MainUserScreen_RightPad_Button);
		gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_ForwardPad_Button, GX_PIXELMAP_ID_UPWHITEARROW);
		gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_ReversePad_Button, GX_PIXELMAP_ID_PADDOWNARROW_ENABLED);
		gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_LeftPad_Button, GX_PIXELMAP_ID_LEFTWHITEARROW);
		gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_RightPad_Button, GX_PIXELMAP_ID_RIGHTWHITEARROW);
		break;
	}
}

//*************************************************************************************
// Function Name: DisplayMainScreenActiveFeatures
//
// Description: This displays the features that are active in the order specificed
//	in the Screen Prompts "objects".
//
//*************************************************************************************

UINT DisplayMainScreenActiveFeatures ()
{
	int enabledCount, lineCount;
	int feature, featureID;
	UINT myErr = GX_SUCCESS;

	// Preset the Function List iconds to blank
	gx_icon_pixelmap_set (&MainUserScreen.MainUserScreen_FeatureIcon, GX_PIXELMAP_ID_BLANK_60X50, GX_PIXELMAP_ID_BLANK_60X50);
    gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_Function0_IconButton, GX_PIXELMAP_ID_BLANK_30X30);
    gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_Function1_IconButton, GX_PIXELMAP_ID_BLANK_30X30);
    gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_Function2_IconButton, GX_PIXELMAP_ID_BLANK_30X30);
    gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_Function3_IconButton, GX_PIXELMAP_ID_BLANK_30X30);
    gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_Function4_IconButton, GX_PIXELMAP_ID_BLANK_30X30);

    // Count the number of active items so we can populate appropriately.
    // Hide the Non-Active features.
    enabledCount = 0;

    // Locate the first feature to display
	for (lineCount = 0; lineCount < 5; ++lineCount)
	{
	    for (feature = 0; feature < MAX_FEATURES; ++feature)
		{
			if (g_MainScreenFeatureInfo[feature].m_Location == lineCount)
			{
				++enabledCount;
				featureID = g_MainScreenFeatureInfo[feature].m_FeatureID;	// abreviated variable
				switch (g_MainScreenFeatureInfo[feature].m_Location)
				{
				case 0: // Show the first line
					myErr = gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_Function0_IconButton, g_Feature_GUI[featureID].m_LargeIcon);
					gx_icon_pixelmap_set (&MainUserScreen.MainUserScreen_FeatureIcon, g_Feature_GUI[featureID].m_FeatureIconID, g_Feature_GUI[featureID].m_FeatureIconID);
					if (featureID == TECLA_E_FEATURE_ID)
					{
						gx_widget_show (&MainUserScreen.MainUserScreen_TeclaMain_Prompt);
						gx_widget_hide (&MainUserScreen.MainUserScreen_FeatureIcon);
					}
					else	// It's something other than Tecla
					{
						gx_widget_hide (&MainUserScreen.MainUserScreen_TeclaMain_Prompt);
						gx_widget_show (&MainUserScreen.MainUserScreen_FeatureIcon);
					}
					break;
				case 1: // Show second line item
					myErr = gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_Function1_IconButton, g_Feature_GUI[featureID].m_SmallIcon);
					break;
				case 2: // Show third line item
					myErr = gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_Function2_IconButton, g_Feature_GUI[featureID].m_SmallIcon);
					break;
				case 3: // Show fourth line item
					myErr = gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_Function3_IconButton, g_Feature_GUI[featureID].m_SmallIcon);
					break;
				case 4: // Show fifth line item
					myErr = gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_Function4_IconButton, g_Feature_GUI[featureID].m_SmallIcon);
					break;
				}
				break;
			}
		}
	}

	// Show the Fusion Logo if no features are enabled.... We are just driving.
	MainUserScreen.MainUserScreen_Fusion_Button.gx_widget_size.gx_rectangle_top = 60;
	MainUserScreen.MainUserScreen_Fusion_Button.gx_widget_size.gx_rectangle_right = 310;
	MainUserScreen.MainUserScreen_Fusion_Button.gx_widget_size.gx_rectangle_bottom = 160;
	if (enabledCount == 0)
		MainUserScreen.MainUserScreen_Fusion_Button.gx_widget_size.gx_rectangle_left = 10;
	else
		MainUserScreen.MainUserScreen_Fusion_Button.gx_widget_size.gx_rectangle_left = 500;

	// Show the Device icon in the middle of the screen.
	SetDeviceIcon(&MainUserScreen.MainUserScreen_DeviceType_icon);
	
	// Display the group icon and set the function box color
	SetGroupIcon (&MainUserScreen.MainUserScreen_GroupIconButton);
	switch (dd_Get_USHORT (0, DD_GROUP))
	{
	case 0:
		gx_widget_fill_color_set (&MainUserScreen.MainUserScreen_FunctionWindow, GX_COLOR_ID_GREEN_ISH, GX_COLOR_ID_GREEN_ISH, GX_COLOR_ID_GREEN_ISH);
		break;
	case 1:
		gx_widget_fill_color_set (&MainUserScreen.MainUserScreen_FunctionWindow, GX_COLOR_ID_SELECTED_FILL, GX_COLOR_ID_SELECTED_FILL, GX_COLOR_ID_SELECTED_FILL);
		break;
	case 2:
		gx_widget_fill_color_set (&MainUserScreen.MainUserScreen_FunctionWindow, GX_COLOR_ID_BRIGHT_ORANGE, GX_COLOR_ID_BRIGHT_ORANGE, GX_COLOR_ID_BRIGHT_ORANGE);
		break;
	case 3:
		gx_widget_fill_color_set (&MainUserScreen.MainUserScreen_FunctionWindow, GX_COLOR_ID_MAROON, GX_COLOR_ID_MAROON, GX_COLOR_ID_MAROON);
		break;
	default:
		break;
	} // end of switch

	DisplayPadFeatures();

    return myErr;
}

//*************************************************************************************

UINT MainUserScreen_EventFunction (GX_WINDOW *window, GX_EVENT *event_ptr)
{
	UINT myErr;

	switch (event_ptr->gx_event_type)
	{
	case GX_EVENT_SHOW:
		DisplayMainScreenActiveFeatures();
		break;

	case GX_SIGNAL (CHANGE_SCREEN_BTN_ID, GX_EVENT_CLICKED):
		screen_toggle((GX_WINDOW *)&MainUserScreen_3, window);
		
		//screen_toggle((GX_WINDOW *)&UserScanScreen, window);

		//++g_GroupIcon;
		//if (g_GroupIcon > 2)	// Only 3 selections.
		//	g_GroupIcon = 0;
		//switch (g_GroupIcon)
		//{
		//case 1:
		//	g_MainScreenFeatureInfo[NEXT_GROUP_ID].m_SmallIcon = GX_PIXELMAP_ID_NEXTGROUP_30X30;
		//	g_MainScreenFeatureInfo[NEXT_GROUP_ID].m_LargeIcon = GX_PIXELMAP_ID_NEXTGROUP_70X70;
		//	break;
		//case 2:
		//	g_MainScreenFeatureInfo[NEXT_GROUP_ID].m_SmallIcon = GX_PIXELMAP_ID_NEXTGROUP_30X30_C;
		//	g_MainScreenFeatureInfo[NEXT_GROUP_ID].m_LargeIcon = GX_PIXELMAP_ID_NEXTGROUP_70X70_C;
		//	break;
		//default:
		//	g_MainScreenFeatureInfo[NEXT_GROUP_ID].m_SmallIcon = GX_PIXELMAP_ID_NEXTGROUP_30X30_E2;
		//	g_MainScreenFeatureInfo[NEXT_GROUP_ID].m_LargeIcon = GX_PIXELMAP_ID_NEXTGROUP_70X70_E2;
		//	break;
		//} // end switch g_GroupIcon
		//DisplayMainScreenActiveFeatures();
		break;

	case GX_SIGNAL (RIGHT_PAD_BUTTON, GX_EVENT_CLICKED):
		break;

	case GX_EVENT_PEN_DOWN:	// We are going to determine if the PAD button is pressed and start a timer to increment the 
							// ... long time (2 seconds) and goto Programming if so.
		if ( (event_ptr->gx_event_target->gx_widget_name == "DownArrowButton") || (event_ptr->gx_event_target->gx_widget_name == "UpArrowButton"))
		{
			gx_system_timer_start(window, ARROW_PUSHED_TIMER_ID, 50, 0);
		}
		else if (event_ptr->gx_event_target->gx_widget_name == "UserPortButton")
		{
			gx_system_timer_start(window, USER_PORT_PUSHED_TIMER_ID, 40, 0);
		}
		break;
    case GX_EVENT_TIMER:
        if (event_ptr->gx_event_payload.gx_event_timer_id == ARROW_PUSHED_TIMER_ID)
		{
	        screen_toggle((GX_WINDOW *)&HHP_Start_Screen, window);
		}
        else if (event_ptr->gx_event_payload.gx_event_timer_id == USER_PORT_PUSHED_TIMER_ID)
		{
			AdvanceToNextFeature();
			DisplayMainScreenActiveFeatures();
			g_Inhibit_UpButtonResponse = TRUE;
		}
		break;

	case GX_EVENT_PEN_UP:
		{
			// Just stop all timers.
			gx_system_timer_stop(window, USER_PORT_PUSHED_TIMER_ID);
			gx_system_timer_stop(window, ARROW_PUSHED_TIMER_ID);
			if (g_Inhibit_UpButtonResponse)
			{
				g_Inhibit_UpButtonResponse = FALSE;
			}
			else if (event_ptr->gx_event_target->gx_widget_name == "UserPortButton")
			{
				switch (dd_Get_USHORT (0, DD_ACTIVE_FEATURE))
				{
				case POWER_ONOFF_ID:
					screen_toggle((GX_WINDOW *)&ReadyScreen, window);
					break;
				case NEXT_GROUP_ID:
					SelectNextGroup();
					Initialize_MainScreenInfo();
					SetToNextGroupFeature();
					DisplayMainScreenActiveFeatures();
					break;
				case AUDIBLE_OUT_FEATURE_ID:
					++g_ActiveSpeakerGroup;
					if (g_ActiveSpeakerGroup > 3)
						g_ActiveSpeakerGroup = 0;
					DisplayMainScreenActiveFeatures();
					break;
				case SEATING_FEATURE_ID:
					++g_ActiveSeatingGroup;
					if (g_ActiveSeatingGroup > 2)
						g_ActiveSeatingGroup = 0;
					DisplayMainScreenActiveFeatures();
					break;
				case BLUETOOTH_ID:
					++g_BluetoothGroup;
					if (g_BluetoothGroup > 1)
						g_BluetoothGroup = 0;
					DisplayMainScreenActiveFeatures();
					break;
				case TECLA_E_FEATURE_ID:
					AdvanceToNextTeclaGroup();
					DisplayMainScreenActiveFeatures();
					break;
				default:
					break;
				} // end switch
			}
		}
		break;

	case GX_SIGNAL (UP_ARROW_BTN_ID, GX_EVENT_CLICKED):
		AdvanceToPreviousFeature();
		DisplayMainScreenActiveFeatures();
		break;

	case GX_SIGNAL (DOWN_ARROW_BTN_ID, GX_EVENT_CLICKED):
		AdvanceToNextFeature();
		DisplayMainScreenActiveFeatures();
		break;

	case GX_SIGNAL (USER_PORT_BUTTON_ID, GX_EVENT_CLICKED):
		gx_system_timer_stop(window, USER_PORT_PUSHED_TIMER_ID);
		break;
	}

    myErr = gx_window_event_process(window, event_ptr);

	return myErr;
}
