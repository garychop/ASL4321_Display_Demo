//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#include "ASL4321_Simulator_Main.h"
#include "ASL4321_Display_Demo_specifications.h"
#include "ASL4321_System.h"

static int mycounter=0;
static unsigned char g_Inhibit_UpButtonResponse = FALSE;
static int g_GroupIcon = 0;

//*************************************************************************************
// Function Name: DisplayMainScreenActiveFeatures
//
// Description: This displays the features that are active in the order specificed
//	in the Screen Prompts "objects".
//
//*************************************************************************************

void DisplayPadFeatures()
{
	switch (g_ActiveFeature)
	{
	case AUDIBLE_OUT_FEATURE_ID:
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
		} // end switch 
		break;
	case BLUETOOTH_ID:
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
	default:
		gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_ForwardPad_Button, GX_PIXELMAP_ID_UPWHITEARROW);
		gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_ReversePad_Button, GX_PIXELMAP_ID_PADDOWNARROW_ENABLED);
		gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_LeftPad_Button, GX_PIXELMAP_ID_LEFTWHITEARROW);
		gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_RightPad_Button, GX_PIXELMAP_ID_RIGHTWHITEARROW);
		break;
	}
}

//*************************************************************************************

UINT DisplayMainScreenActiveFeatures ()
{
	int enabledCount;
	int feature;
	UINT myErr = GX_SUCCESS;

	// Adjust the displayed information based upon the RNet setting.
	// .. If RNet is enabled, the NEXT FUNCTION feature becomes RNet TOGGLE
	// .. and NEXT PROFILE feature become RNet MENU.
    if (g_RNet_Active)
	{
		// Display as "RNet TOGGLE"
		g_MainScreenFeatureInfo[NEXT_FUNCTION_OR_TOGGLE_ID].m_LargeDescriptionID = GX_STRING_ID_RNET_TOGGLE;
		g_MainScreenFeatureInfo[NEXT_FUNCTION_OR_TOGGLE_ID].m_SmallDescriptionID = GX_STRING_ID_RNET_TOGGLE;
		g_MainScreenFeatureInfo[NEXT_FUNCTION_OR_TOGGLE_ID].m_SmallIcon = GX_PIXELMAP_ID_RNET_TOGGLEFR_30X30;
		g_MainScreenFeatureInfo[NEXT_FUNCTION_OR_TOGGLE_ID].m_LargeIcon = GX_PIXELMAP_ID_RNET_TOGGLEFR_70X70;

		// Display as "RNet MENU"
		g_MainScreenFeatureInfo[NEXT_PROFILE_OR_USER_MENU_ID].m_LargeDescriptionID = GX_STRING_ID_RNET_MENU;
		g_MainScreenFeatureInfo[NEXT_PROFILE_OR_USER_MENU_ID].m_SmallDescriptionID = GX_STRING_ID_RNET_MENU;
		g_MainScreenFeatureInfo[NEXT_PROFILE_OR_USER_MENU_ID].m_SmallIcon = GX_PIXELMAP_ID_RNET_MENU_30X30;
		g_MainScreenFeatureInfo[NEXT_PROFILE_OR_USER_MENU_ID].m_LargeIcon = GX_PIXELMAP_ID_RNET_MENU_70X70;
	}
	else
	{
		// Display as NEXT FUNCTION
		g_MainScreenFeatureInfo[NEXT_FUNCTION_OR_TOGGLE_ID].m_LargeDescriptionID = GX_STRING_ID_NEXT_FUNCTION; // "NEXT FUNCTION")
		g_MainScreenFeatureInfo[NEXT_FUNCTION_OR_TOGGLE_ID].m_SmallDescriptionID = GX_STRING_ID_NEXT_FUNCTION;
		g_MainScreenFeatureInfo[NEXT_FUNCTION_OR_TOGGLE_ID].m_SmallIcon = GX_PIXELMAP_ID_FUNCTIONNEXT_30X30;
		g_MainScreenFeatureInfo[NEXT_FUNCTION_OR_TOGGLE_ID].m_LargeIcon = GX_PIXELMAP_ID_FUNCTIONNEXT_70X70;

		// Display as NEXT PROFILE
		g_MainScreenFeatureInfo[NEXT_PROFILE_OR_USER_MENU_ID].m_LargeDescriptionID = GX_STRING_ID_NEXT_PROFILE; // "NEXT PROFILE"
		g_MainScreenFeatureInfo[NEXT_PROFILE_OR_USER_MENU_ID].m_SmallDescriptionID = GX_STRING_ID_NEXT_PROFILE;
		g_MainScreenFeatureInfo[NEXT_PROFILE_OR_USER_MENU_ID].m_SmallIcon = GX_PIXELMAP_ID_PROFILENEXT_30X30;
		g_MainScreenFeatureInfo[NEXT_PROFILE_OR_USER_MENU_ID].m_LargeIcon = GX_PIXELMAP_ID_PROFILENEXT_70X70;
	}
    // Count the number of active items so we can populate appropriately.
    // Hide the Non-Active features.
    enabledCount = 0;

    // Locate the first feature to display
    for (feature = 0; feature < NUM_FEATURES; ++feature)
    {
		if (g_MainScreenFeatureInfo[feature].m_Enabled && g_MainScreenFeatureInfo[feature].m_Available)
        {
            ++enabledCount;
            switch (g_MainScreenFeatureInfo[feature].m_Location)
            {
            case 0: // Show the first line
                //myErr = gx_prompt_text_id_set (&MainUserScreen.MainUserScreen_FirstPrompt, g_MainScreenFeatureInfo[feature].m_LargeDescriptionID);
				//gx_icon_pixelmap_set (&MainUserScreen.MainUserScreen_Function0_Icon , g_MainScreenFeatureInfo[feature].m_LargeIcon, g_MainScreenFeatureInfo[feature].m_LargeIcon);
                myErr = gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_Function0_IconButton, g_MainScreenFeatureInfo[feature].m_LargeIcon);
				gx_icon_pixelmap_set (&MainUserScreen.MainUserScreen_FeatureIcon, g_MainScreenFeatureInfo[feature].m_FeatureIconID, g_MainScreenFeatureInfo[feature].m_FeatureIconID);
				g_ActiveFeature = (FEATURE_ID_ENUM) feature;
                break;
            case 1: // Show second line item
                //myErr = gx_prompt_text_id_set (&MainUserScreen.MainUserScreen_SecondPrompt, g_MainScreenFeatureInfo[feature].m_SmallDescriptionID);
                myErr = gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_Function1_IconButton, g_MainScreenFeatureInfo[feature].m_SmallIcon);
                break;
            case 2: // Show third line item
                //myErr = gx_prompt_text_id_set (&MainUserScreen.MainUserScreen_ThirdPrompt, g_MainScreenFeatureInfo[feature].m_SmallDescriptionID);
                myErr = gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_Function2_IconButton, g_MainScreenFeatureInfo[feature].m_SmallIcon);
                break;
            case 3: // Show fourth line item
                //myErr = gx_prompt_text_id_set (&MainUserScreen.MainUserScreen_FourthPrompt, g_MainScreenFeatureInfo[feature].m_SmallDescriptionID);
                myErr = gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_Function3_IconButton, g_MainScreenFeatureInfo[feature].m_SmallIcon);
                break;
            case 4: // Show fifth line item
                //myErr = gx_prompt_text_id_set (&MainUserScreen.MainUserScreen_FifthPrompt, g_MainScreenFeatureInfo[feature].m_SmallDescriptionID);
                myErr = gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_Function4_IconButton, g_MainScreenFeatureInfo[feature].m_SmallIcon);
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

	// Now blank any unused items.
    for ( ; enabledCount < NUM_FEATURES; ++enabledCount)   // Start with the number of items that are enabled.
    {
        switch (enabledCount)
        {
        case 0:
            //myErr = gx_prompt_text_id_set (&MainUserScreen.MainUserScreen_FirstPrompt, GX_STRING_ID_BLANK);
            myErr = gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_Function0_IconButton, GX_PIXELMAP_ID_BLANK_30X30);
			gx_icon_pixelmap_set (&MainUserScreen.MainUserScreen_FeatureIcon, GX_PIXELMAP_ID_BLANK_60X50, GX_PIXELMAP_ID_BLANK_60X50);
            break;
        case 1:
            //myErr = gx_prompt_text_id_set (&MainUserScreen.MainUserScreen_SecondPrompt, GX_STRING_ID_BLANK);
            myErr = gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_Function1_IconButton, GX_PIXELMAP_ID_BLANK_30X30);
            break;
        case 2:
            //myErr = gx_prompt_text_id_set (&MainUserScreen.MainUserScreen_ThirdPrompt, GX_STRING_ID_BLANK);
            myErr = gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_Function2_IconButton, GX_PIXELMAP_ID_BLANK_30X30);
            break;
        case 3:
            //myErr = gx_prompt_text_id_set (&MainUserScreen.MainUserScreen_FourthPrompt, GX_STRING_ID_BLANK);
            myErr = gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_Function3_IconButton, GX_PIXELMAP_ID_BLANK_30X30);
            break;
        case 4:
            //myErr = gx_prompt_text_id_set (&MainUserScreen.MainUserScreen_FifthPrompt, GX_STRING_ID_BLANK);
            myErr = gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_Function4_IconButton, GX_PIXELMAP_ID_BLANK_30X30);
        } // end of switch
    } // end of for

	
	// Display the group icon and set the function box color
	SetGroupIcon (&MainUserScreen.MainUserScreen_GroupIconButton);
	switch (g_ActiveGroup)
	{
	case 0:
		//myErr = gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_GroupIconButton, GX_PIXELMAP_ID_GROUPA);
		gx_widget_fill_color_set (&MainUserScreen.MainUserScreen_FunctionWindow, GX_COLOR_ID_GREEN_ISH, GX_COLOR_ID_GREEN_ISH, GX_COLOR_ID_GREEN_ISH);
		gx_icon_pixelmap_set (&MainUserScreen.MainUserScreen_DeviceType_icon, GX_PIXELMAP_ID_DEVICETYPE_HEADARRAY_88X70, GX_PIXELMAP_ID_DEVICETYPE_HEADARRAY_88X70);
		break;
	case 1:
		//myErr = gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_GroupIconButton, GX_PIXELMAP_ID_GROUPB);
		gx_widget_fill_color_set (&MainUserScreen.MainUserScreen_FunctionWindow, GX_COLOR_ID_SELECTED_FILL, GX_COLOR_ID_SELECTED_FILL, GX_COLOR_ID_SELECTED_FILL);
		gx_icon_pixelmap_set (&MainUserScreen.MainUserScreen_DeviceType_icon, GX_PIXELMAP_ID_DEVICETYPE_HEADARRAY_88X70, GX_PIXELMAP_ID_DEVICETYPE_HEADARRAY_88X70);
		break;
	case 2:
		//myErr = gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_GroupIconButton, GX_PIXELMAP_ID_GROUPC);
		gx_widget_fill_color_set (&MainUserScreen.MainUserScreen_FunctionWindow, GX_COLOR_ID_BRIGHT_ORANGE, GX_COLOR_ID_BRIGHT_ORANGE, GX_COLOR_ID_BRIGHT_ORANGE);
		gx_icon_pixelmap_set (&MainUserScreen.MainUserScreen_DeviceType_icon, GX_PIXELMAP_ID_DEVICETYPE_JOYSTICK_88X70, GX_PIXELMAP_ID_DEVICETYPE_JOYSTICK_88X70);
		break;
	case 3:
		//myErr = gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_GroupIconButton, GX_PIXELMAP_ID_GROUPD);
		gx_widget_fill_color_set (&MainUserScreen.MainUserScreen_FunctionWindow, GX_COLOR_ID_MAROON, GX_COLOR_ID_MAROON, GX_COLOR_ID_MAROON);
		gx_icon_pixelmap_set (&MainUserScreen.MainUserScreen_DeviceType_icon, GX_PIXELMAP_ID_DEVICETYPE_JOYSTICK_88X70, GX_PIXELMAP_ID_DEVICETYPE_JOYSTICK_88X70);
		break;
	default:
		break;
	} // end of switch

	DisplayPadFeatures();

    return myErr;
}

//-----------------------------------------------------------------------------

void AdvanceToNextFeature ()
{
	UINT feature;
	int activeCount;

	// This is necessary to prevent the subsequent "Clicked" message from advancing the feature when we are changing to the Programming screen.
	if (g_ChangeScreen_WIP)
	{
		g_ChangeScreen_WIP = FALSE;
		return;
	}
	// Count the number of active features to set a limit on location
	activeCount = 0;
	for (feature = 0; feature < NUM_FEATURES; ++feature)
	{
		if (g_MainScreenFeatureInfo[feature].m_Enabled)
			++activeCount;
	}
	// Move Top Feature to Bottom and move Bottom upward.
	for (feature = 0; feature < NUM_FEATURES; ++feature)
	{
		if (g_MainScreenFeatureInfo[feature].m_Enabled && g_MainScreenFeatureInfo[feature].m_Available)
		{
			if (g_MainScreenFeatureInfo[feature].m_Location == 0)
				g_MainScreenFeatureInfo[feature].m_Location = activeCount-1;
			else if (g_MainScreenFeatureInfo[feature].m_Location == 1)
				g_MainScreenFeatureInfo[feature].m_Location = 0;
			else if (g_MainScreenFeatureInfo[feature].m_Location == 2)
				g_MainScreenFeatureInfo[feature].m_Location = min (1, activeCount-1);
			else if (g_MainScreenFeatureInfo[feature].m_Location == 3)
				g_MainScreenFeatureInfo[feature].m_Location = min (2, activeCount-1);
            else if (g_MainScreenFeatureInfo[feature].m_Location == 4)
                g_MainScreenFeatureInfo[feature].m_Location = min (3, activeCount-1);
            else if (g_MainScreenFeatureInfo[feature].m_Location == 5)
                g_MainScreenFeatureInfo[feature].m_Location = min (4, activeCount-1);
            else if (g_MainScreenFeatureInfo[feature].m_Location == 6)
                g_MainScreenFeatureInfo[feature].m_Location = min (5, activeCount-1);
		}
	}
}

//-----------------------------------------------------------------------------

void AdvanceToPreviousFeature()
{
	UINT feature;
	int activeCount;

	// This is necessary to prevent the subsequent "Clicked" message from advancing the feature when we are changing to the Programming screen.
	if (g_ChangeScreen_WIP)
	{
		g_ChangeScreen_WIP = FALSE;
		return;
	}
	// Count the number of active features to set a limit on location
	activeCount = 0;
	for (feature = 0; feature < NUM_FEATURES; ++feature)
	{
		if (g_MainScreenFeatureInfo[feature].m_Enabled)
			++activeCount;
	}
	--activeCount;	// Convert from the number of items (1-based) to line number (0-based)

	// Move the features downward, limiting the movement by the number of Active Features.
	for (feature = 0; feature < NUM_FEATURES; ++feature)
	{
		if (g_MainScreenFeatureInfo[feature].m_Enabled && g_MainScreenFeatureInfo[feature].m_Available)
		{
			if (g_MainScreenFeatureInfo[feature].m_Location == activeCount)
				g_MainScreenFeatureInfo[feature].m_Location = 0;
			else if (g_MainScreenFeatureInfo[feature].m_Location == 0)
				g_MainScreenFeatureInfo[feature].m_Location = min (1, activeCount);
			else if (g_MainScreenFeatureInfo[feature].m_Location == 1)
				g_MainScreenFeatureInfo[feature].m_Location = min (2, activeCount);
			else if (g_MainScreenFeatureInfo[feature].m_Location == 2)
				g_MainScreenFeatureInfo[feature].m_Location = min (3, activeCount);
            else if (g_MainScreenFeatureInfo[feature].m_Location == 3)
                g_MainScreenFeatureInfo[feature].m_Location = min (4, activeCount);
            else if (g_MainScreenFeatureInfo[feature].m_Location == 4)
                g_MainScreenFeatureInfo[feature].m_Location = min (5, activeCount);
            else if (g_MainScreenFeatureInfo[feature].m_Location == 5)
                g_MainScreenFeatureInfo[feature].m_Location = min (6, activeCount);
		}
	}
}

//-----------------------------------------------------------------------------

UINT MainUserScreen_EventFunction (GX_WINDOW *window, GX_EVENT *event_ptr)
{
	UINT myErr;

	switch (event_ptr->gx_event_type)
	{
	case GX_EVENT_SHOW:
		DisplayMainScreenActiveFeatures();
		break;

	case GX_SIGNAL (CHANGE_SCREEN_BTN_ID, GX_EVENT_CLICKED):
		++g_GroupIcon;
		if (g_GroupIcon > 2)	// Only 3 selections.
			g_GroupIcon = 0;
		switch (g_GroupIcon)
		{
		case 1:
			g_MainScreenFeatureInfo[NEXT_GROUP_ID].m_SmallIcon = GX_PIXELMAP_ID_NEXTGROUP_30X30;
			g_MainScreenFeatureInfo[NEXT_GROUP_ID].m_LargeIcon = GX_PIXELMAP_ID_NEXTGROUP_70X70;
			break;
		case 2:
			g_MainScreenFeatureInfo[NEXT_GROUP_ID].m_SmallIcon = GX_PIXELMAP_ID_NEXTGROUP_30X30_C;
			g_MainScreenFeatureInfo[NEXT_GROUP_ID].m_LargeIcon = GX_PIXELMAP_ID_NEXTGROUP_70X70_C;
			break;
		default:
			g_MainScreenFeatureInfo[NEXT_GROUP_ID].m_SmallIcon = GX_PIXELMAP_ID_NEXTGROUP_30X30_E2;
			g_MainScreenFeatureInfo[NEXT_GROUP_ID].m_LargeIcon = GX_PIXELMAP_ID_NEXTGROUP_70X70_E2;
			break;
		} // end switch g_GroupIcon
		DisplayMainScreenActiveFeatures();
		//screen_toggle((GX_WINDOW *)&MainUserScreen_3, window);
		break;

	case GX_SIGNAL (LEFT_PAD_BUTTON, GX_EVENT_CLICKED):
		++mycounter;
		if (mycounter > 4)
			mycounter = 0;
		switch (mycounter)
		{
		case 0:
			gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_LeftPad_Button , GX_PIXELMAP_ID_RIGHTWHITEARROW); //GX_PIXELMAP_ID_RIGHTWHITEARROW, GX_PIXELMAP_ID_RIGHTWHITEARROW);
			break;
		case 1:
			gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_LeftPad_Button, GX_PIXELMAP_ID_LEFTWHITEARROW); // , GX_PIXELMAP_ID_LEFTWHITEARROW, GX_PIXELMAP_ID_LEFTWHITEARROW);
			break;
		case 2:
			gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_LeftPad_Button, GX_PIXELMAP_ID_UPWHITEARROW); // , GX_PIXELMAP_ID_UPWHITEARROW, GX_PIXELMAP_ID_UPWHITEARROW);
			break;
		case 3:
			gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_LeftPad_Button, GX_PIXELMAP_ID_PADDOWNARROW_ENABLED); // , GX_PIXELMAP_ID_PADDOWNARROW_ENABLED, GX_PIXELMAP_ID_PADDOWNARROW_ENABLED);
			break;
		case 4:
			gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_LeftPad_Button, GX_PIXELMAP_ID_FUNCTIONNEXT_70X70); // , GX_PIXELMAP_ID_PADDOWNARROW_ENABLED, GX_PIXELMAP_ID_PADDOWNARROW_ENABLED);
			break;
		default:
			break;
		} // end switch
		break;
	case GX_SIGNAL (RIGHT_PAD_RIGHT_ARROW_BTN_ID, GX_EVENT_CLICKED):
		break;

	case GX_EVENT_PEN_DOWN:	// We are going to determine if the PAD button is pressed and start a timer to increment the 
							// ... long time (2 seconds) and goto Programming if so.
		if ( (event_ptr->gx_event_target->gx_widget_name == "DownArrowButton") || (event_ptr->gx_event_target->gx_widget_name == "UpArrowButton"))
		{
			gx_system_timer_start(window, ARROW_PUSHED_TIMER_ID, 100, 0);
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
				switch (g_ActiveFeature)
				{
				case POWER_ONOFF_ID:
					screen_toggle((GX_WINDOW *)&ReadyScreen, window);
					break;
				case NEXT_GROUP_ID:
					++g_ActiveGroup;		// Advance to the next group
					if (g_ActiveGroup > 3)	// Ensure a proper rollover to the first group.
						g_ActiveGroup = 0;
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
					if (g_ActiveSeatingGroup > 1)
						g_ActiveSeatingGroup = 0;
					DisplayMainScreenActiveFeatures();
					break;
				case BLUETOOTH_ID:
					++g_BluetoothGroup;
					if (g_BluetoothGroup > 1)
						g_BluetoothGroup = 0;
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
