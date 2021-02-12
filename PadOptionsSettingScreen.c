//*****************************************************************************
// Filename: PadOptionsSettingScreen.cpp
//
// Date: Aug 28, 2020
//
// Author: G. Chopcinski, Kg Solutions, LLC
// 
//*****************************************************************************

#include "ASL4321_System.h"
#include "PadInfo.h"

typedef enum PAD_OPTIONS_MENU_POSITION_E
{
	PAD_OPTIONS_SET_PAD_TYPE,
	PAD_OPTIONS_SET_PAD_DIRECTION,
	PAD_OPTIONS_SET_MINIMUM_SPEED,
	PAD_OPTIONS_SET_NEUTRAL_WINDOW,
	PAD_OPTIONS_SET_JOYSTICK_THROW_MAX,
	END_OF_PAD_OPTIONS_MENU_POS
} PAD_OPTIONSL_MENU_POSITION_ENUM;

PAD_INFO_STRUCT g_PadSettings[MAX_PHYSICAL_PADS];

GX_RECTANGLE g_DiagnosticWidgetLocations[] = {
	{35, 32, 35+62, 32+90},
	{183, 32, 183+62, 32+90},
	{66, 140, 66+145, 140+42}};

CUSTOM_MENU_BUTTON g_PadOptions_MenuItems[END_OF_PAD_OPTIONS_MENU_POS + 1];

//*************************************************************************************
VOID DrawOptionsScreen (GX_WINDOW *window);
VOID DeleteOptionsList (GX_WINDOW *window);

//*************************************************************************************
// InitializePadInformation
//*************************************************************************************

VOID InitializePadInformation()
{
	// Populate the default Pad settings.
	g_PadSettings[LEFT_PAD].m_PadDirection = LEFT_DIRECTION;
	g_PadSettings[LEFT_PAD].m_PadType = PROPORTIONAL_PADTYPE;
	g_PadSettings[LEFT_PAD].m_MinimumDriveValue = 20;
	strcpy_s (g_PadSettings[LEFT_PAD].m_MinimuDriveString, sizeof (g_PadSettings[LEFT_PAD].m_MinimuDriveString), "20%");
	g_PadSettings[LEFT_PAD].m_DirectionIcons[OFF_DIRECTION] = &SetPadDirectionScreen.SetPadDirectionScreen_LeftPad_Off_Button;
	g_PadSettings[LEFT_PAD].m_DirectionIcons[RIGHT_DIRECTION] = &SetPadDirectionScreen.SetPadDirectionScreen_LeftPad_RightArrow_Button;
	g_PadSettings[LEFT_PAD].m_DirectionIcons[LEFT_DIRECTION] = &SetPadDirectionScreen.SetPadDirectionScreen_LeftPad_LeftArrow_Button;
	g_PadSettings[LEFT_PAD].m_DirectionIcons[FORWARD_DIRECTION] = &SetPadDirectionScreen.SetPadDirectionScreen_LeftPad_ForwardArrow_Button;
	g_PadSettings[LEFT_PAD].m_PadMinimumCalibrationValue = 2;
	g_PadSettings[LEFT_PAD].m_PadMaximumCalibrationValue = 30;
	g_PadSettings[LEFT_PAD].m_DiagnosticWidigetLocation = g_DiagnosticWidgetLocations[LEFT_PAD];
	g_PadSettings[LEFT_PAD].m_DiagnosticOff_ID = LEFT_PAD_OFF_BTN_ID;
	g_PadSettings[LEFT_PAD].m_DiagnosticDigital_ID = LEFT_PAD_DIGITAL_BTN_ID;
	g_PadSettings[LEFT_PAD].m_DiagnosticProportional_ID = LEFT_PAD_PROP_BTN_ID;
	g_PadSettings[LEFT_PAD].m_DiagnosticOff_Widget = &DiagnosticScreen.DiagnosticScreen_LeftPadOff_Button;
	g_PadSettings[LEFT_PAD].m_DiagnosticProportional_Widget = &DiagnosticScreen.DiagnosticScreen_LeftPadProp_Button;
	g_PadSettings[LEFT_PAD].m_DiagnosticDigital_Widget = &DiagnosticScreen.DiagnosticScreen_LeftPadDigital_Button;

	g_PadSettings[RIGHT_PAD].m_PadDirection = RIGHT_DIRECTION;
	g_PadSettings[RIGHT_PAD].m_PadType = PROPORTIONAL_PADTYPE;
	g_PadSettings[RIGHT_PAD].m_MinimumDriveValue = 20;
	strcpy_s (g_PadSettings[RIGHT_PAD].m_MinimuDriveString, sizeof (g_PadSettings[RIGHT_PAD].m_MinimuDriveString), "20%");
	g_PadSettings[RIGHT_PAD].m_DirectionIcons[OFF_DIRECTION] = &SetPadDirectionScreen.SetPadDirectionScreen_RightPad_Off_Button;
	g_PadSettings[RIGHT_PAD].m_DirectionIcons[RIGHT_DIRECTION] = &SetPadDirectionScreen.SetPadDirectionScreen_RightPad_RightArrow_Button;
	g_PadSettings[RIGHT_PAD].m_DirectionIcons[LEFT_DIRECTION] = &SetPadDirectionScreen.SetPadDirectionScreen_RightPad_LeftArrow_Button;
	g_PadSettings[RIGHT_PAD].m_DirectionIcons[FORWARD_DIRECTION] = &SetPadDirectionScreen.SetPadDirectionScreen_RightPad_ForwardArrow_Button;
	g_PadSettings[RIGHT_PAD].m_PadMinimumCalibrationValue = 2;
	g_PadSettings[RIGHT_PAD].m_PadMaximumCalibrationValue = 30;
	g_PadSettings[RIGHT_PAD].m_DiagnosticWidigetLocation = g_DiagnosticWidgetLocations[RIGHT_PAD];
	g_PadSettings[RIGHT_PAD].m_DiagnosticOff_ID = RIGHT_PAD_OFF_BTN_ID;
	g_PadSettings[RIGHT_PAD].m_DiagnosticDigital_ID = RIGHT_PAD_DIGITAL_BTN_ID;
	g_PadSettings[RIGHT_PAD].m_DiagnosticProportional_ID = RIGHT_PAD_PROP_BTN_ID;
	g_PadSettings[RIGHT_PAD].m_DiagnosticOff_Widget = &DiagnosticScreen.DiagnosticScreen_RightPadOff_Button;
	g_PadSettings[RIGHT_PAD].m_DiagnosticProportional_Widget = &DiagnosticScreen.DiagnosticScreen_RightPadProp_Button;
	g_PadSettings[RIGHT_PAD].m_DiagnosticDigital_Widget = &DiagnosticScreen.DiagnosticScreen_RightPadDigital_Button;

	g_PadSettings[CENTER_PAD].m_PadDirection = FORWARD_DIRECTION;
	g_PadSettings[CENTER_PAD].m_PadType = PROPORTIONAL_PADTYPE;
	g_PadSettings[CENTER_PAD].m_MinimumDriveValue = 20;
	strcpy_s (g_PadSettings[CENTER_PAD].m_MinimuDriveString, sizeof (g_PadSettings[CENTER_PAD].m_MinimuDriveString), "20%");
	g_PadSettings[CENTER_PAD].m_DirectionIcons[OFF_DIRECTION] = &SetPadDirectionScreen.SetPadDirectionScreen_CenterPad_Off_Button;
	g_PadSettings[CENTER_PAD].m_DirectionIcons[RIGHT_DIRECTION] = &SetPadDirectionScreen.SetPadDirectionScreen_CenterPad_RightArrow_Button;
	g_PadSettings[CENTER_PAD].m_DirectionIcons[LEFT_DIRECTION] = &SetPadDirectionScreen.SetPadDirectionScreen_CenterPad_LeftArrow_Button;
	g_PadSettings[CENTER_PAD].m_DirectionIcons[FORWARD_DIRECTION] = &SetPadDirectionScreen.SetPadDirectionScreen_CenterPad_ForwardArrow_Button;
	g_PadSettings[CENTER_PAD].m_PadMinimumCalibrationValue = 2;
	g_PadSettings[CENTER_PAD].m_PadMaximumCalibrationValue = 30;
	g_PadSettings[CENTER_PAD].m_DiagnosticWidigetLocation = g_DiagnosticWidgetLocations[CENTER_PAD];
	g_PadSettings[CENTER_PAD].m_DiagnosticOff_ID = CENTER_PAD_OFF_BTN_ID;
	g_PadSettings[CENTER_PAD].m_DiagnosticDigital_ID = CENTER_PAD_DIGITAL_BTN_ID;
	g_PadSettings[CENTER_PAD].m_DiagnosticProportional_ID = CENTER_PAD_PROP_BTN_ID;
	g_PadSettings[CENTER_PAD].m_DiagnosticOff_Widget = &DiagnosticScreen.DiagnosticScreen_CenterPadOff_Button;
	g_PadSettings[CENTER_PAD].m_DiagnosticProportional_Widget = &DiagnosticScreen.DiagnosticScreen_CenterPadProp_Button;
	g_PadSettings[CENTER_PAD].m_DiagnosticDigital_Widget = &DiagnosticScreen.DiagnosticScreen_CenterPadDigital_Button;
}

//*************************************************************************************
//
//*************************************************************************************

static void Populate_PadOptions_MenuItems(void)
{
	// SET PAD TYPE 
	g_PadOptions_MenuItems[PAD_OPTIONS_SET_PAD_TYPE].m_ButtonID = PAD_OPTIONS_SET_PAD_TYPE_BUTTON_ID;
	g_PadOptions_MenuItems[PAD_OPTIONS_SET_PAD_TYPE].m_TextID = GX_STRING_ID_STRING_13;
	g_PadOptions_MenuItems[PAD_OPTIONS_SET_PAD_TYPE].m_Enabled = TRUE;
	// SET PAD DIRECTIONS
	g_PadOptions_MenuItems[PAD_OPTIONS_SET_PAD_DIRECTION].m_ButtonID = PAD_OPTIONS_SET_PAD_DIRECTION_BUTTON_ID;
	g_PadOptions_MenuItems[PAD_OPTIONS_SET_PAD_DIRECTION].m_TextID = GX_STRING_ID_STRING_16;
	g_PadOptions_MenuItems[PAD_OPTIONS_SET_PAD_DIRECTION].m_Enabled = TRUE;
	// SET MINIMUM DRIVE SPEED
	g_PadOptions_MenuItems[PAD_OPTIONS_SET_MINIMUM_SPEED].m_ButtonID = PAD_OPTIONS_SET_MINIMUM_SPEED_BUTTON_ID;
	g_PadOptions_MenuItems[PAD_OPTIONS_SET_MINIMUM_SPEED].m_TextID = GX_STRING_ID_STRING_30;
	g_PadOptions_MenuItems[PAD_OPTIONS_SET_MINIMUM_SPEED].m_Enabled = TRUE;
	// PAD_OPTIONS_SET_NEUTRAL_WINDOW,
	g_PadOptions_MenuItems[PAD_OPTIONS_SET_NEUTRAL_WINDOW].m_ButtonID = PAD_OPTIONS_SET_NEUTRAL_WINDOW_BUTTON_ID;
	g_PadOptions_MenuItems[PAD_OPTIONS_SET_NEUTRAL_WINDOW].m_TextID = GX_STRING_ID_STRING_35;
	g_PadOptions_MenuItems[PAD_OPTIONS_SET_NEUTRAL_WINDOW].m_Enabled = FALSE;
	//PAD_OPTIONS_SET_JOYSTICK_THROW_MAX,
	g_PadOptions_MenuItems[PAD_OPTIONS_SET_JOYSTICK_THROW_MAX].m_ButtonID = PAD_OPTIONS_SET_JOYSTICK_THROW_MAX_BUTTON_ID;
	g_PadOptions_MenuItems[PAD_OPTIONS_SET_JOYSTICK_THROW_MAX].m_TextID = GX_STRING_ID_STRING_58;
	g_PadOptions_MenuItems[PAD_OPTIONS_SET_JOYSTICK_THROW_MAX].m_Enabled = FALSE;
	// END OF LIST
	// Not used, must be last
	g_PadOptions_MenuItems[END_OF_PAD_OPTIONS_MENU_POS].m_ButtonID = LAST_MENU_BUTTON_ID;
	g_PadOptions_MenuItems[END_OF_PAD_OPTIONS_MENU_POS].m_TextID = GX_STRING_ID_BLANK;
	g_PadOptions_MenuItems[END_OF_PAD_OPTIONS_MENU_POS].m_Enabled = FALSE;

	switch (g_GroupInfo[g_ActiveGroup].m_DeviceType)
	{
	case DEVICE_TYPE_HEAD_ARRAY:
		g_PadOptions_MenuItems[PAD_OPTIONS_SET_PAD_TYPE].m_Enabled = TRUE;
		g_PadOptions_MenuItems[PAD_OPTIONS_SET_PAD_DIRECTION].m_Enabled = TRUE;
		g_PadOptions_MenuItems[PAD_OPTIONS_SET_MINIMUM_SPEED].m_Enabled = TRUE;
		g_PadOptions_MenuItems[PAD_OPTIONS_SET_NEUTRAL_WINDOW].m_Enabled = FALSE;
		g_PadOptions_MenuItems[PAD_OPTIONS_SET_JOYSTICK_THROW_MAX].m_Enabled = FALSE;
		break;
	case DEVICE_TYPE_PROPORTIONAL_JOYSTICK:
		g_PadOptions_MenuItems[PAD_OPTIONS_SET_PAD_TYPE].m_Enabled = FALSE;
		g_PadOptions_MenuItems[PAD_OPTIONS_SET_PAD_DIRECTION].m_Enabled = TRUE;
		g_PadOptions_MenuItems[PAD_OPTIONS_SET_MINIMUM_SPEED].m_Enabled = TRUE;
		g_PadOptions_MenuItems[PAD_OPTIONS_SET_NEUTRAL_WINDOW].m_Enabled = TRUE;
		g_PadOptions_MenuItems[PAD_OPTIONS_SET_JOYSTICK_THROW_MAX].m_Enabled = TRUE;
		break;
	case DEVICE_TYPE_DIGITAL_JOYSTICK:
		g_PadOptions_MenuItems[PAD_OPTIONS_SET_PAD_TYPE].m_Enabled = FALSE;
		g_PadOptions_MenuItems[PAD_OPTIONS_SET_PAD_DIRECTION].m_Enabled = TRUE;
		g_PadOptions_MenuItems[PAD_OPTIONS_SET_MINIMUM_SPEED].m_Enabled = TRUE;
		g_PadOptions_MenuItems[PAD_OPTIONS_SET_NEUTRAL_WINDOW].m_Enabled = FALSE;
		g_PadOptions_MenuItems[PAD_OPTIONS_SET_JOYSTICK_THROW_MAX].m_Enabled = FALSE;
		break;

	} // end switch
}

//*************************************************************************************

VOID PadOptionsMenuList_Callback (GX_VERTICAL_LIST *pList, GX_WIDGET *pWidget, INT pIndex)
{
	GX_BOOL result;
	UINT err;
	CUSTOM_MENU_BUTTON *thisItem;
	GX_RECTANGLE childsize;

	thisItem = (CUSTOM_MENU_BUTTON*) pWidget;

	gx_widget_created_test(&thisItem->m_MenuWidget, &result);	// Test to see if the GUIX item has been created, I guess it's like a null test.

	if (!result)	// "!result" means, item has not been created yet.
	{
		// Draw the Rectangle that reaches from the left to the right of the List Box.
		gx_utility_rectangle_define(&childsize, 0, 0, pList->gx_widget_size.gx_rectangle_right - 35 , 74);
        err = gx_widget_create(&thisItem->m_MenuWidget, NULL, (GX_WIDGET *)pList, GX_STYLE_TRANSPARENT | GX_STYLE_ENABLED/* | GX_STYLE_TEXT_LEFT */, GX_ID_NONE, &childsize);

		// This draws an actual GUIX button
        gx_utility_rectangle_define(&childsize, 8, 0, thisItem->m_MenuWidget.gx_widget_size.gx_rectangle_right-10, 64);
		if (thisItem->m_Enabled)
			err = gx_text_button_create (&thisItem->m_ButtonWidget, NULL, &thisItem->m_MenuWidget, thisItem->m_TextID, GX_STYLE_ENABLED | GX_STYLE_BORDER_THIN, 0, &childsize);
		else
			err = gx_text_button_create (&thisItem->m_ButtonWidget, NULL, &thisItem->m_MenuWidget, thisItem->m_TextID, GX_STYLE_BORDER_THIN, 0, &childsize);
		gx_text_button_text_color_set (&thisItem->m_ButtonWidget, GX_COLOR_ID_BTN_TEXT, GX_COLOR_ID_BTN_TEXT, GX_COLOR_ID_DISABLED_TEXT);
		gx_widget_fill_color_set ((GX_WIDGET*) &thisItem->m_ButtonWidget, GX_COLOR_ID_TEXT_INPUT_FILL, GX_COLOR_ID_TEXT_INPUT_TEXT, GX_COLOR_ID_DISABLED_FILL);
	}
}

VOID PadOptionsSettingsScreen_draw_function (GX_WINDOW *window)
{
	DeleteOptionsList(window);
	DrawOptionsScreen (window);
	gx_window_draw (window);
}

VOID DeleteOptionsList (GX_WINDOW *window)
{
	int index;
	GX_BOOL result;
	PADOPTIONSSETTINGSSCREEN_CONTROL_BLOCK *WindowPtr = (PADOPTIONSSETTINGSSCREEN_CONTROL_BLOCK*) window;

	for (index = 0; index < END_OF_PAD_OPTIONS_MENU_POS; ++index)
	{
		gx_widget_created_test(&g_PadOptions_MenuItems[index].m_ButtonWidget, &result);	// Test to see if the GUIX item has been created, I guess it's like a null test.
		if (result)
		{
			gx_widget_delete((GX_WIDGET*) &g_PadOptions_MenuItems[index].m_ButtonWidget);
		}
		gx_widget_created_test(&g_PadOptions_MenuItems[index].m_MenuWidget, &result);	// Test to see if the GUIX item has been created, I guess it's like a null test.
		if (result)
		{
			gx_widget_delete((GX_WIDGET*) &g_PadOptions_MenuItems[index].m_MenuWidget);
		}
	}
	WindowPtr->PadOptionsSettingsScreen_PadOptionsMenuList.gx_vertical_list_total_rows = 0;
}

VOID DrawOptionsScreen (GX_WINDOW *window)
{
	int index;
	int numberOfActiveMenuItems;

	PADOPTIONSSETTINGSSCREEN_CONTROL_BLOCK *WindowPtr = (PADOPTIONSSETTINGSSCREEN_CONTROL_BLOCK*) window;

	// This displays the GROUP A, B, C or D icon.
	SetGroupIcon (&PadOptionsSettingsScreen.PadOptionsSettingsScreen_GroupIconButton);

	ShowDeviceIcon (&PadOptionsSettingsScreen.PadOptionsSettingsScreen_DeviceType_icon);

	// Populate the button list based upon the Device Type
	Populate_PadOptions_MenuItems();

	// Populate the GUI's List Box with the available buttons.
	numberOfActiveMenuItems = 0;
	for (index = 0; g_PadOptions_MenuItems[index].m_ButtonID != LAST_MENU_BUTTON_ID; ++index)
	{
		if (g_PadOptions_MenuItems[index].m_Enabled)
		{
			PadOptionsMenuList_Callback (&WindowPtr->PadOptionsSettingsScreen_PadOptionsMenuList, (GX_WIDGET*) &g_PadOptions_MenuItems[index], numberOfActiveMenuItems);
			++numberOfActiveMenuItems;
		}
	}
	WindowPtr->PadOptionsSettingsScreen_PadOptionsMenuList.gx_vertical_list_total_rows = numberOfActiveMenuItems;

	// This set the selected item to "-1" and it must be -1 or else an event is generated and triggers an undesired action.
    gx_vertical_list_selected_set(&WindowPtr->PadOptionsSettingsScreen_PadOptionsMenuList, -1);

	// Show or hide the Scroll bar based upon the number of buttons.
	if (WindowPtr->PadOptionsSettingsScreen_PadOptionsMenuList.gx_vertical_list_total_rows <= 2)
		gx_widget_hide ((GX_WIDGET*) &WindowPtr->PadOptionsSettingsScreen_FeatureList_vertical_scroll);

	// The following (re) aligns the buttons in the list box... Don't know why I need it this window but it works.
	// It may have something to do with redrawing the window without leaving and re-entering this window.
	gx_vertical_list_children_position (&WindowPtr->PadOptionsSettingsScreen_PadOptionsMenuList);

}

//*************************************************************************************
// Function Name: PadOptionsSettingsScreen_event_process
//
// Description: This dispatches the Pad Option Settings Screen messages
//
//*************************************************************************************

UINT PadOptionsSettingsScreen_event_process (GX_WINDOW *window, GX_EVENT *event_ptr)
{
	UINT myErr;
	int index;
	int selectedIndex;
	int runningIndex;
	int okToDelete;

	PADOPTIONSSETTINGSSCREEN_CONTROL_BLOCK *WindowPtr = (PADOPTIONSSETTINGSSCREEN_CONTROL_BLOCK*) window;

	switch (event_ptr->gx_event_type)
	{
	case GX_SIGNAL (GROUP_ICON_BUTTON, GX_EVENT_CLICKED):
		SelectNextGroup();
		DeleteOptionsList(window);
		DrawOptionsScreen (window);
		break;

	case GX_EVENT_SHOW:
//		DeleteOptionsList(window);
		DrawOptionsScreen (window);
		break;

	case GX_SIGNAL(OK_BTN_ID, GX_EVENT_CLICKED):
		DeleteOptionsList(window);
        screen_toggle((GX_WINDOW *)&HHP_Start_Screen, window);
		break;

	case GX_SIGNAL (DEVICE_TYPE_BTN_ID, GX_EVENT_CLICKED):
		SelectNextDevice();
		DeleteOptionsList(window);
		DrawOptionsScreen (window);
		//ShowDeviceIcon (&PadOptionsSettingsScreen.PadOptionsSettingsScreen_DeviceType_icon);
		break;

    case GX_SIGNAL(PAD_OPTIONS_VERTICAL_LIST, GX_EVENT_LIST_SELECT):
		gx_vertical_list_selected_index_get (&WindowPtr->PadOptionsSettingsScreen_PadOptionsMenuList, &selectedIndex);
		// "index" refers to the item in the list... and not the item itself.
		// We will identify which pad option has been selected.
		runningIndex = 0;	// This will increment with each enabled items.
		for (index = 0; index < END_OF_PAD_OPTIONS_MENU_POS; ++index)
		{
			if (g_PadOptions_MenuItems[index].m_Enabled)
			{
				if (runningIndex == selectedIndex)
					break;				// we found the correct Pad Option button.
				else
					++runningIndex;		// We need to continue looking.
			}
		}
		okToDelete = TRUE;
		switch (index)
		{
		case PAD_OPTIONS_SET_PAD_TYPE: // PAD SETTINGS
			screen_toggle ((GX_WINDOW*) &SetPadTypeScreen, window);
			break;
		case PAD_OPTIONS_SET_PAD_DIRECTION: // USER SETTINGS
			screen_toggle ((GX_WINDOW*) &SetPadDirectionScreen, window);
			break;
		case PAD_OPTIONS_SET_MINIMUM_SPEED:	// FEATURE LIST
			screen_toggle ((GX_WINDOW*) &MinimumDriveScreen, window);
			break;
		case PAD_OPTIONS_SET_NEUTRAL_WINDOW:
			screen_toggle ((GX_WINDOW*) &NeutralWindowScreen, window);
			break;
		case PAD_OPTIONS_SET_JOYSTICK_THROW_MAX:
			screen_toggle ((GX_WINDOW*) &JoystickThrowScreen, window);
			break;
		default:
			okToDelete = FALSE;
			break;
		}
		if (okToDelete)
			DeleteOptionsList(window);
		break;

	    myErr = gx_window_event_process(window, event_ptr);
	}	// end of switch.

    myErr = gx_window_event_process(window, event_ptr);

	return GX_SUCCESS;
}





