//*****************************************************************************
// Filename: PadDirectionScreen.cpp
//
// Date: Aug 28, 2020
//
// Author: G. Chopcinski, Kg Solutions, LLC
// 
//*****************************************************************************

#include "ASL4321_System.h"
#include "asl4321_display_demo_resources.h"
#include "DataDictionary.h"
#include "PadInfo.h"

//*************************************************************************************
// Local Macros
//*************************************************************************************

//*************************************************************************************
// External References
//*************************************************************************************

extern GX_RECTANGLE g_HiddenRectangle;
PHYSICAL_PAD_ENUM g_CyclePad;

//*************************************************************************************
// Local/Global variables
//*************************************************************************************

GX_RECTANGLE g_PadDirectionLocation[] =
{
	{28, 55, 28+88, 55+70},
	{205, 55, 205+88, 55+70},
	{116, 150, 116+88, 150+70},
	{0,0,0,0}
};

USHORT g_GroupNumber;

//*************************************************************************************

PAD_DIRECTION_ENUM SetNextPadDirection (PHYSICAL_PAD_ENUM pad)
{
	PAD_DIRECTION_ENUM padDirection;

	padDirection = (PAD_DIRECTION_ENUM) dd_GetSubItem_USHORT (g_GroupNumber, DD_PAD_DIRECTION, pad); 

	if (pad < MAX_PHYSICAL_PADS)		// Pad Number validity check
	{
		if (++padDirection >= MAX_DIRECTION)
			padDirection = (PAD_DIRECTION_ENUM) 0;
	}
	dd_SetSubItem_USHORT (g_GroupNumber, DD_PAD_DIRECTION, pad, padDirection); 

	return (padDirection);
}

//*************************************************************************************

UINT GetDirectionIcon (PAD_DIRECTION_ENUM direction)
{
	switch (direction)
	{
		case FORWARD_DIRECTION:
			return GX_PIXELMAP_ID_UPWHITEARROW;
			break;
		case LEFT_DIRECTION:
			return GX_PIXELMAP_ID_LEFTWHITEARROW;
			break;
		case RIGHT_DIRECTION:
			return GX_PIXELMAP_ID_RIGHTWHITEARROW;
			break;
		case REVERSE_DIRECTION:
			return GX_PIXELMAP_ID_DOWNWHITEARROW;
			break;
		case CYCLE_DIRECTION:
			return GX_PIXELMAP_ID_CYCLEBUTTON;
			break;
		default:
		case OFF_DIRECTION:
			return GX_PIXELMAP_ID_PAD_OFF;
			break;
	} // end switch
}

//*************************************************************************************
// Position the keypad buttons based upon the Driver Type. If Head Array then
// only 3 pads are available. If Proportional or Digital Joystick then all 4 
// buttons are shown.
//*************************************************************************************

static VOID ShowPads (VOID)
{
	GX_RECTANGLE rectangle;

	if (dd_Get_USHORT (dd_Get_USHORT (0, DD_GROUP), DD_DEVICE_TYPE) == DEVICE_TYPE_HEAD_ARRAY)
	{
		// Move device icon
		gx_utility_rectangle_define (&rectangle, 154, 90, 154+88, 90+70);	// Left, top, right, bottom
		gx_widget_resize (&SetPadDirectionScreen.SetPadDirectionScreen_DeviceType_icon, &rectangle);
		// Left Button
		gx_utility_rectangle_define (&rectangle, 64, 90, 64+88, 90+70);	// Left, top, right, bottom
		gx_widget_resize (&SetPadDirectionScreen.SetPadDirectionScreen_LeftPad_Off_Button, &rectangle);
		// Right Button
		gx_utility_rectangle_define (&rectangle, 242, 90, 242+88, 90+70);	// Left, top, right, bottom
		gx_widget_resize (&SetPadDirectionScreen.SetPadDirectionScreen_RightPad_Off_Button, &rectangle);
		// Center/Forward Pad
		gx_utility_rectangle_define (&rectangle, 154, 160, 152+88, 160+70);	// Left, top, right, bottom
		gx_widget_resize (&SetPadDirectionScreen.SetPadDirectionScreen_ForwardPad_Off_Button, &rectangle);
		// Hide the revese button
		gx_widget_hide (&SetPadDirectionScreen.SetPadDirectionScreen_ReversePad_Off_Button);
	}
	else	// Must be a joystick, show all 4 pads.
	{
		// Move device icon
		gx_utility_rectangle_define (&rectangle, 154, 120, 154+88, 120+70);	// Left, top, right, bottom
		gx_widget_resize (&SetPadDirectionScreen.SetPadDirectionScreen_DeviceType_icon, &rectangle);
		// Left Button
		gx_utility_rectangle_define (&rectangle, 64, 120, 64+88, 120+70);	// Left, top, right, bottom
		gx_widget_resize (&SetPadDirectionScreen.SetPadDirectionScreen_LeftPad_Off_Button, &rectangle);
		// Right Button
		gx_utility_rectangle_define (&rectangle, 242, 120, 242+88, 120+70);	// Left, top, right, bottom
		gx_widget_resize (&SetPadDirectionScreen.SetPadDirectionScreen_RightPad_Off_Button, &rectangle);
		// Center/Forward Pad
		gx_utility_rectangle_define (&rectangle, 154, 50, 154+88, 50+70);	// Left, top, right, bottom
		gx_widget_resize (&SetPadDirectionScreen.SetPadDirectionScreen_ForwardPad_Off_Button, &rectangle);
		// Reverse Button
		gx_utility_rectangle_define (&rectangle, 154, 190, 154+88, 190+70);	// Left, top, right, bottom
		gx_widget_resize (&SetPadDirectionScreen.SetPadDirectionScreen_ReversePad_Off_Button, &rectangle);
		gx_widget_show (&SetPadDirectionScreen.SetPadDirectionScreen_ReversePad_Off_Button);
	}

}

//*************************************************************************************
// Function Name: SetPadDirectionScreen_event_process
//
// Description: This functions process the event of the Set Pad Direction screen.
//
//*************************************************************************************

UINT SetPadDirectionScreen_event_process (GX_WINDOW *window, GX_EVENT *event_ptr)
{
	GX_RESOURCE_ID pixelID;
	PAD_DIRECTION_ENUM padDirection;

	gx_window_event_process(window, event_ptr);

	switch (event_ptr->gx_event_type)
	{
	case GX_EVENT_SHOW:
		g_GroupNumber = dd_Get_USHORT (0, DD_GROUP);	// Get currently selected group.
		
		SetGroupIcon (&SetPadDirectionScreen.SetPadDirectionScreen_GroupIconButton);

		padDirection = (PAD_DIRECTION_ENUM) dd_GetSubItem_USHORT (g_GroupNumber, DD_PAD_DIRECTION, LEFT_PAD); 
		pixelID = GetDirectionIcon (padDirection);
		gx_pixelmap_button_pixelmap_set (&SetPadDirectionScreen.SetPadDirectionScreen_LeftPad_Off_Button, pixelID, pixelID, pixelID);

		padDirection = (PAD_DIRECTION_ENUM) dd_GetSubItem_USHORT (g_GroupNumber, DD_PAD_DIRECTION, RIGHT_PAD); 
		pixelID = GetDirectionIcon (padDirection);
		gx_pixelmap_button_pixelmap_set (&SetPadDirectionScreen.SetPadDirectionScreen_RightPad_Off_Button, pixelID, pixelID, pixelID);

		padDirection = (PAD_DIRECTION_ENUM) dd_GetSubItem_USHORT (g_GroupNumber, DD_PAD_DIRECTION, FORWARD_PAD); 
		pixelID = GetDirectionIcon (padDirection);
		gx_pixelmap_button_pixelmap_set (&SetPadDirectionScreen.SetPadDirectionScreen_ForwardPad_Off_Button, pixelID, pixelID, pixelID);

		padDirection = (PAD_DIRECTION_ENUM) dd_GetSubItem_USHORT (g_GroupNumber, DD_PAD_DIRECTION, REVERSE_PAD); 
		pixelID = GetDirectionIcon (padDirection);
		gx_pixelmap_button_pixelmap_set (&SetPadDirectionScreen.SetPadDirectionScreen_ReversePad_Off_Button, pixelID, pixelID, pixelID);

		ShowPads();		// This hides or shows the Reverse Pad based upon Device Type.

		SetDeviceIcon(&SetPadDirectionScreen.SetPadDirectionScreen_DeviceType_icon);		// Show the Device icon in the middle of the screen.

		break;

	case GX_SIGNAL(OK_BTN_ID, GX_EVENT_CLICKED):
        screen_toggle((GX_WINDOW *)&PadOptionsSettingsScreen, window);
		break;
		// Process LEFT button pushes

	case GX_SIGNAL(LEFT_PAD_OFF_BTN_ID, GX_EVENT_CLICKED):
		padDirection = SetNextPadDirection (LEFT_PAD);
		pixelID = GetDirectionIcon (padDirection);
		gx_pixelmap_button_pixelmap_set (&SetPadDirectionScreen.SetPadDirectionScreen_LeftPad_Off_Button, pixelID, pixelID, pixelID);
		break;

	case GX_SIGNAL(RIGHT_PAD_OFF_BTN_ID, GX_EVENT_CLICKED):
		padDirection = SetNextPadDirection (RIGHT_PAD);
		pixelID = GetDirectionIcon (padDirection);
		gx_pixelmap_button_pixelmap_set (&SetPadDirectionScreen.SetPadDirectionScreen_RightPad_Off_Button, pixelID, pixelID, pixelID);
		break;

	case GX_SIGNAL(FORWARD_PAD_OFF_BTN_ID, GX_EVENT_CLICKED):
		padDirection = SetNextPadDirection (FORWARD_PAD);
		pixelID = GetDirectionIcon (padDirection);
		gx_pixelmap_button_pixelmap_set (&SetPadDirectionScreen.SetPadDirectionScreen_ForwardPad_Off_Button, pixelID, pixelID, pixelID);
		break;

	case GX_SIGNAL(REVERSE_PAD_OFF_BTN_ID, GX_EVENT_CLICKED):
		padDirection = SetNextPadDirection (REVERSE_PAD);
		pixelID = GetDirectionIcon (padDirection);
		gx_pixelmap_button_pixelmap_set (&SetPadDirectionScreen.SetPadDirectionScreen_ReversePad_Off_Button, pixelID, pixelID, pixelID);
		break;

	case GX_EVENT_PEN_DOWN:	// We are going to determine if the PAD button is pressed and start a timer to increment the 
							// ... long time (2 seconds) and goto Programming if so.
		g_CyclePad = MAX_PHYSICAL_PADS;
		if ((event_ptr->gx_event_target->gx_widget_name == "ForwardPad_Off_Button") && (g_PadSettings[FORWARD_PAD].m_PadDirection == CYCLE_DIRECTION))
			g_CyclePad = FORWARD_PAD;
		else if ((event_ptr->gx_event_target->gx_widget_name == "LeftPad_Off_Button") && (g_PadSettings[LEFT_PAD].m_PadDirection == CYCLE_DIRECTION))
			g_CyclePad = LEFT_PAD;
		else if ((event_ptr->gx_event_target->gx_widget_name == "RightPad_Off_Button") && (g_PadSettings[RIGHT_PAD].m_PadDirection == CYCLE_DIRECTION))
			g_CyclePad = RIGHT_PAD;
		else if ((event_ptr->gx_event_target->gx_widget_name == "ReversePad_Off_Button") && (g_PadSettings[REVERSE_PAD].m_PadDirection == CYCLE_DIRECTION))
			g_CyclePad = REVERSE_PAD;
		if (g_CyclePad != MAX_PHYSICAL_PADS)
		{
			gx_system_timer_start(window, ARROW_PUSHED_TIMER_ID, 50, 0);
		}
		break;

    case GX_EVENT_TIMER:
        if (event_ptr->gx_event_payload.gx_event_timer_id == ARROW_PUSHED_TIMER_ID)
		{
	        screen_toggle((GX_WINDOW *)&PadAdvancedScreen, window);
		}
		break;

	case GX_EVENT_PEN_UP:
		gx_system_timer_stop(window, ARROW_PUSHED_TIMER_ID);
		break;

	} // end switch on event


	return GX_SUCCESS;
}













