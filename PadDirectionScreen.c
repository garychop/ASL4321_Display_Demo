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

extern VOID LocatePadPosition (PHYSICAL_PAD_ENUM pad, DEVICE_TYPE_ENUM device, GX_BOOL showPrompt, GX_ICON_BUTTON *icon, GX_PROMPT *prompt);
extern VOID LocateDevicePosition (DEVICE_TYPE_ENUM device, GX_ICON *icon);

//*************************************************************************************
// Local/Global variables
//*************************************************************************************

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
// Function Name: SetPadDirectionScreen_event_process
//
// Description: This functions process the event of the Set Pad Direction screen.
//
//*************************************************************************************

UINT SetPadDirectionScreen_event_process (GX_WINDOW *window, GX_EVENT *event_ptr)
{
	GX_RESOURCE_ID pixelID;
	PAD_DIRECTION_ENUM padDirection;
	DEVICE_TYPE_ENUM thisDevice;

	gx_window_event_process(window, event_ptr);

	switch (event_ptr->gx_event_type)
	{
	case GX_EVENT_SHOW:
		g_GroupNumber = dd_Get_USHORT (0, DD_GROUP);	// Get currently selected group.
		thisDevice = (DEVICE_TYPE_ENUM) dd_Get_USHORT (g_GroupNumber, DD_DEVICE_TYPE);
		
		SetGroupIcon (&SetPadDirectionScreen.SetPadDirectionScreen_GroupIconButton);

		padDirection = (PAD_DIRECTION_ENUM) dd_GetSubItem_USHORT (g_GroupNumber, DD_PAD_DIRECTION, LEFT_PAD); 
		pixelID = GetDirectionIcon (padDirection);
		gx_icon_button_pixelmap_set (&SetPadDirectionScreen.SetPadDirectionScreen_Left_IconButton, pixelID);

		padDirection = (PAD_DIRECTION_ENUM) dd_GetSubItem_USHORT (g_GroupNumber, DD_PAD_DIRECTION, RIGHT_PAD); 
		pixelID = GetDirectionIcon (padDirection);
		gx_icon_button_pixelmap_set (&SetPadDirectionScreen.SetPadDirectionScreen_Right_IconButton, pixelID);

		padDirection = (PAD_DIRECTION_ENUM) dd_GetSubItem_USHORT (g_GroupNumber, DD_PAD_DIRECTION, FORWARD_PAD); 
		pixelID = GetDirectionIcon (padDirection);
		gx_icon_button_pixelmap_set (&SetPadDirectionScreen.SetPadDirectionScreen_Forward_IconButton, pixelID);

		padDirection = (PAD_DIRECTION_ENUM) dd_GetSubItem_USHORT (g_GroupNumber, DD_PAD_DIRECTION, REVERSE_PAD); 
		pixelID = GetDirectionIcon (padDirection);
		gx_icon_button_pixelmap_set (&SetPadDirectionScreen.SetPadDirectionScreen_Reverse_IconButton, pixelID);

		LocatePadPosition (LEFT_PAD, thisDevice, FALSE, &SetPadDirectionScreen.SetPadDirectionScreen_Left_IconButton, NULL);
		LocatePadPosition (RIGHT_PAD, thisDevice, FALSE, &SetPadDirectionScreen.SetPadDirectionScreen_Right_IconButton, NULL);
		LocatePadPosition (FORWARD_PAD, thisDevice, FALSE, &SetPadDirectionScreen.SetPadDirectionScreen_Forward_IconButton, NULL);
		LocatePadPosition (REVERSE_PAD, thisDevice, FALSE, &SetPadDirectionScreen.SetPadDirectionScreen_Reverse_IconButton, NULL);

		SetDeviceIcon(&SetPadDirectionScreen.SetPadDirectionScreen_DeviceType_icon);		// Show the Device icon in the middle of the screen.
		break;

	case GX_SIGNAL(OK_BTN_ID, GX_EVENT_CLICKED):
        screen_toggle((GX_WINDOW *)&PadOptionsSettingsScreen, window);
		break;

	case GX_SIGNAL(LEFT_ICON_BTN_ID, GX_EVENT_CLICKED):
		padDirection = SetNextPadDirection (LEFT_PAD);
		pixelID = GetDirectionIcon (padDirection);
		gx_icon_button_pixelmap_set (&SetPadDirectionScreen.SetPadDirectionScreen_Left_IconButton, pixelID);
		break;

	case GX_SIGNAL(RIGHT_ICON_BTN_ID, GX_EVENT_CLICKED):
		padDirection = SetNextPadDirection (RIGHT_PAD);
		pixelID = GetDirectionIcon (padDirection);
		gx_icon_button_pixelmap_set (&SetPadDirectionScreen.SetPadDirectionScreen_Right_IconButton, pixelID);
		break;

	case GX_SIGNAL(FORWARD_ICON_BTN_ID, GX_EVENT_CLICKED):
		padDirection = SetNextPadDirection (FORWARD_PAD);
		pixelID = GetDirectionIcon (padDirection);
		gx_icon_button_pixelmap_set (&SetPadDirectionScreen.SetPadDirectionScreen_Forward_IconButton, pixelID);
		break;

	case GX_SIGNAL(REVERSE_ICON_BTN_ID, GX_EVENT_CLICKED):
		padDirection = SetNextPadDirection (REVERSE_PAD);
		pixelID = GetDirectionIcon (padDirection);
		gx_icon_button_pixelmap_set (&SetPadDirectionScreen.SetPadDirectionScreen_Reverse_IconButton, pixelID);
		break;

	case GX_EVENT_PEN_DOWN:	// We are going to determine if the PAD button is pressed and start a timer to increment the 
							// ... long time (2 seconds) and goto Programming if so.
		g_CyclePad = MAX_PHYSICAL_PADS;
		switch (event_ptr->gx_event_target->gx_widget_id)
		{
		case FORWARD_ICON_BTN_ID:
			padDirection = (PAD_DIRECTION_ENUM) dd_GetSubItem_USHORT (g_GroupNumber, DD_PAD_DIRECTION, FORWARD_PAD); 
			if (padDirection == CYCLE_DIRECTION)
				g_CyclePad = FORWARD_PAD;
			break;
		case LEFT_ICON_BTN_ID:
			padDirection = (PAD_DIRECTION_ENUM) dd_GetSubItem_USHORT (g_GroupNumber, DD_PAD_DIRECTION, LEFT_PAD); 
			if (padDirection == CYCLE_DIRECTION)
				g_CyclePad = LEFT_PAD;
			break;
		case RIGHT_ICON_BTN_ID:
			padDirection = (PAD_DIRECTION_ENUM) dd_GetSubItem_USHORT (g_GroupNumber, DD_PAD_DIRECTION, RIGHT_PAD); 
			if (padDirection == CYCLE_DIRECTION)
				g_CyclePad = RIGHT_PAD;
			break;
		case REVERSE_ICON_BTN_ID:
			padDirection = (PAD_DIRECTION_ENUM) dd_GetSubItem_USHORT (g_GroupNumber, DD_PAD_DIRECTION, REVERSE_PAD); 
			if (padDirection == CYCLE_DIRECTION)
				g_CyclePad = REVERSE_PAD;
			break;
		} // end switch
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













