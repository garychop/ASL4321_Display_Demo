//-----------------------------------------------------------------------------
// Filename: UserScanScreen.c
//
// Description: Supports the User's Scanning Screen.
//-----------------------------------------------------------------------------

#include "ASL4321_Simulator_Main.h"
#include "ASL4321_Display_Demo_specifications.h"
#include "ASL4321_System.h"
#include "DataDictionary.h"

extern unsigned char g_Inhibit_UpButtonResponse;

static UINT DisplayScanScreenActiveFeatures ()
{
	int enabledCount;
	int feature;
	UINT myErr = GX_SUCCESS;

	// Adjust the displayed information based upon the RNet setting.
	// .. If RNet is enabled, the NEXT FUNCTION feature becomes RNet TOGGLE
	// .. and NEXT PROFILE feature become RNet MENU.
    //if (g_RNet_Active)
	//if (dd_Get_USHORT(0, DD_RNET_ENABLE))
	//{
	//	// Display as "RNet TOGGLE"
	//	g_MainScreenFeatureInfo[NEXT_FUNCTION_OR_TOGGLE_ID].m_LargeDescriptionID = GX_STRING_ID_RNET_TOGGLE;
	//	g_MainScreenFeatureInfo[NEXT_FUNCTION_OR_TOGGLE_ID].m_SmallDescriptionID = GX_STRING_ID_RNET_TOGGLE;
	//	g_MainScreenFeatureInfo[NEXT_FUNCTION_OR_TOGGLE_ID].m_SmallIcon = GX_PIXELMAP_ID_RNET_TOGGLEFR_30X30;
	//	g_MainScreenFeatureInfo[NEXT_FUNCTION_OR_TOGGLE_ID].m_LargeIcon = GX_PIXELMAP_ID_RNET_TOGGLEFR_70X70;

	//	// Display as "RNet MENU"
	//	g_MainScreenFeatureInfo[NEXT_PROFILE_OR_USER_MENU_ID].m_LargeDescriptionID = GX_STRING_ID_RNET_MENU;
	//	g_MainScreenFeatureInfo[NEXT_PROFILE_OR_USER_MENU_ID].m_SmallDescriptionID = GX_STRING_ID_RNET_MENU;
	//	g_MainScreenFeatureInfo[NEXT_PROFILE_OR_USER_MENU_ID].m_SmallIcon = GX_PIXELMAP_ID_RNET_MENU_30X30;
	//	g_MainScreenFeatureInfo[NEXT_PROFILE_OR_USER_MENU_ID].m_LargeIcon = GX_PIXELMAP_ID_RNET_MENU_70X70;
	//}
	//else
	//{
	//	// Display as NEXT FUNCTION
	//	g_MainScreenFeatureInfo[NEXT_FUNCTION_OR_TOGGLE_ID].m_LargeDescriptionID = GX_STRING_ID_NEXT_FUNCTION; // "NEXT FUNCTION")
	//	g_MainScreenFeatureInfo[NEXT_FUNCTION_OR_TOGGLE_ID].m_SmallDescriptionID = GX_STRING_ID_NEXT_FUNCTION;
	//	g_MainScreenFeatureInfo[NEXT_FUNCTION_OR_TOGGLE_ID].m_SmallIcon = GX_PIXELMAP_ID_FUNCTIONNEXT_30X30;
	//	g_MainScreenFeatureInfo[NEXT_FUNCTION_OR_TOGGLE_ID].m_LargeIcon = GX_PIXELMAP_ID_FUNCTIONNEXT_70X70;

	//	// Display as NEXT PROFILE
	//	g_MainScreenFeatureInfo[NEXT_PROFILE_OR_USER_MENU_ID].m_LargeDescriptionID = GX_STRING_ID_NEXT_PROFILE; // "NEXT PROFILE"
	//	g_MainScreenFeatureInfo[NEXT_PROFILE_OR_USER_MENU_ID].m_SmallDescriptionID = GX_STRING_ID_NEXT_PROFILE;
	//	g_MainScreenFeatureInfo[NEXT_PROFILE_OR_USER_MENU_ID].m_SmallIcon = GX_PIXELMAP_ID_PROFILENEXT_30X30;
	//	g_MainScreenFeatureInfo[NEXT_PROFILE_OR_USER_MENU_ID].m_LargeIcon = GX_PIXELMAP_ID_PROFILENEXT_70X70;
	//}

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
                myErr = gx_icon_button_pixelmap_set (&UserScanScreen.UserScanScreen_Function0_IconButton, g_MainScreenFeatureInfo[feature].m_LargeIcon);
				gx_icon_pixelmap_set (&UserScanScreen.UserScanScreen_FeatureIcon, g_MainScreenFeatureInfo[feature].m_FeatureIconID, g_MainScreenFeatureInfo[feature].m_FeatureIconID);
				g_ActiveFeature = (FEATURE_ID_ENUM) feature;
				if (g_ActiveFeature == TECLA_E_FEATURE_ID)
				{
					gx_widget_show (&UserScanScreen.UserScanScreen_TeclaMain_Prompt);
					gx_widget_hide (&UserScanScreen.UserScanScreen_FeatureIcon);
				}
				else	// It's something other than Tecla
				{
					gx_widget_hide (&UserScanScreen.UserScanScreen_TeclaMain_Prompt);
					gx_widget_show (&UserScanScreen.UserScanScreen_FeatureIcon);
				}
                break;
            case 1: // Show second line item
                myErr = gx_icon_button_pixelmap_set (&UserScanScreen.UserScanScreen_Function1_IconButton, g_MainScreenFeatureInfo[feature].m_SmallIcon);
                break;
            case 2: // Show third line item
                myErr = gx_icon_button_pixelmap_set (&UserScanScreen.UserScanScreen_Function2_IconButton, g_MainScreenFeatureInfo[feature].m_SmallIcon);
                break;
            case 3: // Show fourth line item
                myErr = gx_icon_button_pixelmap_set (&UserScanScreen.UserScanScreen_Function3_IconButton, g_MainScreenFeatureInfo[feature].m_SmallIcon);
                break;
            case 4: // Show fifth line item
                myErr = gx_icon_button_pixelmap_set (&UserScanScreen.UserScanScreen_Function4_IconButton, g_MainScreenFeatureInfo[feature].m_SmallIcon);
                break;
            }
        }
    }

	// Show the Fusion Logo if no features are enabled.... We are just driving.
	UserScanScreen.UserScanScreen_Fusion_Button.gx_widget_size.gx_rectangle_top = 60;
	UserScanScreen.UserScanScreen_Fusion_Button.gx_widget_size.gx_rectangle_right = 310;
	UserScanScreen.UserScanScreen_Fusion_Button.gx_widget_size.gx_rectangle_bottom = 160;
	if (enabledCount == 0)
		UserScanScreen.UserScanScreen_Fusion_Button.gx_widget_size.gx_rectangle_left = 10;
	else
		UserScanScreen.UserScanScreen_Fusion_Button.gx_widget_size.gx_rectangle_left = 500;

	// Now blank any unused items.
    for ( ; enabledCount < NUM_FEATURES; ++enabledCount)   // Start with the number of items that are enabled.
    {
        switch (enabledCount)
        {
        case 0:
            //myErr = gx_prompt_text_id_set (&UserScanScreen.UserScanScreen_FirstPrompt, GX_STRING_ID_BLANK);
            myErr = gx_icon_button_pixelmap_set (&UserScanScreen.UserScanScreen_Function0_IconButton, GX_PIXELMAP_ID_BLANK_30X30);
			gx_icon_pixelmap_set (&UserScanScreen.UserScanScreen_FeatureIcon, GX_PIXELMAP_ID_BLANK_60X50, GX_PIXELMAP_ID_BLANK_60X50);
            break;
        case 1:
            //myErr = gx_prompt_text_id_set (&UserScanScreen.UserScanScreen_SecondPrompt, GX_STRING_ID_BLANK);
            myErr = gx_icon_button_pixelmap_set (&UserScanScreen.UserScanScreen_Function1_IconButton, GX_PIXELMAP_ID_BLANK_30X30);
            break;
        case 2:
            //myErr = gx_prompt_text_id_set (&UserScanScreen.UserScanScreen_ThirdPrompt, GX_STRING_ID_BLANK);
            myErr = gx_icon_button_pixelmap_set (&UserScanScreen.UserScanScreen_Function2_IconButton, GX_PIXELMAP_ID_BLANK_30X30);
            break;
        case 3:
            //myErr = gx_prompt_text_id_set (&UserScanScreen.UserScanScreen_FourthPrompt, GX_STRING_ID_BLANK);
            myErr = gx_icon_button_pixelmap_set (&UserScanScreen.UserScanScreen_Function3_IconButton, GX_PIXELMAP_ID_BLANK_30X30);
            break;
        case 4:
            //myErr = gx_prompt_text_id_set (&UserScanScreen.UserScanScreen_FifthPrompt, GX_STRING_ID_BLANK);
            myErr = gx_icon_button_pixelmap_set (&UserScanScreen.UserScanScreen_Function4_IconButton, GX_PIXELMAP_ID_BLANK_30X30);
        } // end of switch
    } // end of for

	// Show the Device icon in the middle of the screen.
//	SetDeviceIcon(&UserScanScreen.UserScanScreen_DeviceType_icon);
	
	// Display the group icon and set the function box color
	SetGroupIcon (&UserScanScreen.UserScanScreen_GroupIconButton);
	switch (dd_Get_USHORT (0, DD_GROUP))
	{
	case 0:
		gx_widget_fill_color_set (&UserScanScreen.UserScanScreen_FunctionWindow, GX_COLOR_ID_GREEN_ISH, GX_COLOR_ID_GREEN_ISH, GX_COLOR_ID_GREEN_ISH);
		break;
	case 1:
		gx_widget_fill_color_set (&UserScanScreen.UserScanScreen_FunctionWindow, GX_COLOR_ID_SELECTED_FILL, GX_COLOR_ID_SELECTED_FILL, GX_COLOR_ID_SELECTED_FILL);
		break;
	case 2:
		gx_widget_fill_color_set (&UserScanScreen.UserScanScreen_FunctionWindow, GX_COLOR_ID_BRIGHT_ORANGE, GX_COLOR_ID_BRIGHT_ORANGE, GX_COLOR_ID_BRIGHT_ORANGE);
		break;
	case 3:
		gx_widget_fill_color_set (&UserScanScreen.UserScanScreen_FunctionWindow, GX_COLOR_ID_MAROON, GX_COLOR_ID_MAROON, GX_COLOR_ID_MAROON);
		break;
	default:
		break;
	} // end of switch

//	DisplayPadFeatures();

    return myErr;
}

//-----------------------------------------------------------------------------

VOID ShowPadAndAdvance(BOOL advance)
{
	gx_icon_button_pixelmap_set (&UserScanScreen.UserScanScreen_ForwardPad_Button, GX_PIXELMAP_ID_UPSHORTARROW_104X53);
	gx_icon_button_pixelmap_set (&UserScanScreen.UserScanScreen_LeftPad_Button, GX_PIXELMAP_ID_LEFTSHORTARROW_104X53);
	gx_icon_button_pixelmap_set (&UserScanScreen.UserScanScreen_RightPad_Button, GX_PIXELMAP_ID_RIGHTSHORTARROW_104X53);
	gx_icon_button_pixelmap_set (&UserScanScreen.UserScanScreen_ReversePad_Button, GX_PIXELMAP_ID_DOWNSHORTARROW_104X53);
	gx_icon_button_pixelmap_set (&UserScanScreen.UserScanScreen_VeerForwardLeft_Button, GX_PIXELMAP_ID_ARROWVEERFORWARDLEFTSHORT_104X104);
	gx_icon_button_pixelmap_set (&UserScanScreen.UserScanScreen_VeerForwardRight_Button, GX_PIXELMAP_ID_ARROWVEERFORWARDRIGHTSHORT_104X104);
	gx_icon_button_pixelmap_set (&UserScanScreen.UserScanScreen_UserPort_Button, GX_PIXELMAP_ID_USERPORTBUTTON_DISABLED);

	switch (gActiveScanDirection)
	{
	case SCAN_FORWARD:		// Just starting. set foward to selected, all other not-selected
		gx_icon_button_pixelmap_set (&UserScanScreen.UserScanScreen_ForwardPad_Button, GX_PIXELMAP_ID_UPARROW_104X53);
		if (advance)
			gActiveScanDirection = SCAN_LEFT;
		break;

	case SCAN_LEFT:
		gx_icon_button_pixelmap_set (&UserScanScreen.UserScanScreen_LeftPad_Button, GX_PIXELMAP_ID_LEFTARROW_104X53);
		if (advance)
			gActiveScanDirection = SCAN_RIGHT;
		break;

	case SCAN_RIGHT:
		gx_icon_button_pixelmap_set (&UserScanScreen.UserScanScreen_RightPad_Button, GX_PIXELMAP_ID_RIGHTARROW_104X53);
		if (advance)
			gActiveScanDirection = SCAN_REVERSE;
		break;

	case SCAN_REVERSE:
		gx_icon_button_pixelmap_set (&UserScanScreen.UserScanScreen_ReversePad_Button, GX_PIXELMAP_ID_DOWNARROW_104X53);
		if (advance)
			gActiveScanDirection = SCAN_VEER_FORWARD_LEFT;
		break;

	case SCAN_VEER_FORWARD_LEFT:
		gx_icon_button_pixelmap_set (&UserScanScreen.UserScanScreen_VeerForwardLeft_Button, GX_PIXELMAP_ID_ARROWVEERFORWARDLEFT_104X104);
		if (advance)
			gActiveScanDirection = SCAN_VEER_FORWARD_RIGHT;
		break;

	case SCAN_VEER_FORWARD_RIGHT:
		gx_icon_button_pixelmap_set (&UserScanScreen.UserScanScreen_VeerForwardRight_Button, GX_PIXELMAP_ID_ARROWVEERFORWARDRIGHT_104X104);
		if (advance)
			gActiveScanDirection = SCAN_USER_PORT;
		break;

	case SCAN_USER_PORT:
		gx_icon_button_pixelmap_set (&UserScanScreen.UserScanScreen_UserPort_Button, GX_PIXELMAP_ID_USERPORTBUTTON_ENABLED);
		if (advance)
			gActiveScanDirection = SCAN_FORWARD;
		break;

	default:
	case SCAN_MAX:
		break;

	} // end switch g_ActivePad
}

//-----------------------------------------------------------------------------

UINT UserScanScreen_EventFunction (GX_WINDOW *window, GX_EVENT *event_ptr)
{
	UINT myErr;

	switch (event_ptr->gx_event_type)
	{
	case GX_EVENT_SHOW:
		ShowPadAndAdvance (FALSE);
		DisplayScanScreenActiveFeatures();
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
			DisplayScanScreenActiveFeatures();
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
					SelectNextGroup();
					DisplayScanScreenActiveFeatures();
					break;
				case AUDIBLE_OUT_FEATURE_ID:
					++g_ActiveSpeakerGroup;
					if (g_ActiveSpeakerGroup > 3)
						g_ActiveSpeakerGroup = 0;
					DisplayScanScreenActiveFeatures();
					break;
				case SEATING_FEATURE_ID:
					++g_ActiveSeatingGroup;
					if (g_ActiveSeatingGroup > 2)
						g_ActiveSeatingGroup = 0;
					DisplayScanScreenActiveFeatures();
					break;
				case BLUETOOTH_ID:
					++g_BluetoothGroup;
					if (g_BluetoothGroup > 1)
						g_BluetoothGroup = 0;
					DisplayScanScreenActiveFeatures();
					break;
				case TECLA_E_FEATURE_ID:
					AdvanceToNextTeclaGroup();
					DisplayScanScreenActiveFeatures();
					break;
				default:
					break;
				} // end switch
			}
		}
		break;

	case GX_SIGNAL (UP_ARROW_BTN_ID, GX_EVENT_CLICKED):
		AdvanceToPreviousFeature();
		DisplayScanScreenActiveFeatures();
		break;

	case GX_SIGNAL (DOWN_ARROW_BTN_ID, GX_EVENT_CLICKED):
		AdvanceToNextFeature();
		DisplayScanScreenActiveFeatures();
		break;

	case GX_SIGNAL (USER_PORT_BUTTON_ID, GX_EVENT_CLICKED):
		gx_system_timer_stop(window, USER_PORT_PUSHED_TIMER_ID);
		ShowPadAndAdvance (TRUE);
		break;
	}

    myErr = gx_window_event_process(window, event_ptr);

	return myErr;
}

