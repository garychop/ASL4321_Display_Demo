//*****************************************************************************
// Filename: PadDirectionScreen.cpp
//
// Date: Aug 28, 2020
//
// Author: G. Chopcinski, Kg Solutions, LLC
// 
//*****************************************************************************

#include "ASL4321_System.h"
#include "PadInfo.h"

//*************************************************************************************
// Local Macros
//*************************************************************************************

//*************************************************************************************
// External References
//*************************************************************************************

extern GX_RECTANGLE g_HiddenRectangle;

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

//*************************************************************************************

VOID SetNextPadDirection (UINT group, PHYSICAL_PAD_ENUM pad)
{
	if (pad < MAX_PHYSICAL_PADS)
	{
		if (++g_PadSettings[pad].m_PadDirection >= MAX_DIRECTION)
			g_PadSettings[pad].m_PadDirection = (PAD_DIRECTION_ENUM) 0;
	}
}

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

	if (GetGroupDeviceType() == DEVICE_TYPE_HEAD_ARRAY)
	{
		// Left Button
		gx_utility_rectangle_define (&rectangle, 64, 120, 64+88, 120+70);	// Left, top, right, bottom
		gx_widget_resize (&SetPadDirectionScreen.SetPadDirectionScreen_LeftPad_Off_Button, &rectangle);
		// Right Button
		gx_utility_rectangle_define (&rectangle, 242, 120, 242+88, 120+70);	// Left, top, right, bottom
		gx_widget_resize (&SetPadDirectionScreen.SetPadDirectionScreen_RightPad_Off_Button, &rectangle);
		// Center/Forward Pad
		gx_utility_rectangle_define (&rectangle, 152, 190, 152+88, 190+70);	// Left, top, right, bottom
		gx_widget_resize (&SetPadDirectionScreen.SetPadDirectionScreen_ForwardPad_Off_Button, &rectangle);
		// Hide the revese button
		gx_widget_hide (&SetPadDirectionScreen.SetPadDirectionScreen_ReversePad_Off_Button);
	}
	else	// Must be a joystick, show all 4 pads.
	{
		// Left Button
		gx_utility_rectangle_define (&rectangle, 64, 120, 64+88, 120+70);	// Left, top, right, bottom
		gx_widget_resize (&SetPadDirectionScreen.SetPadDirectionScreen_LeftPad_Off_Button, &rectangle);
		// Right Button
		gx_utility_rectangle_define (&rectangle, 242, 120, 242+88, 120+70);	// Left, top, right, bottom
		gx_widget_resize (&SetPadDirectionScreen.SetPadDirectionScreen_RightPad_Off_Button, &rectangle);
		// Center/Forward Pad
		gx_utility_rectangle_define (&rectangle, 152, 50, 152+88, 50+70);	// Left, top, right, bottom
		gx_widget_resize (&SetPadDirectionScreen.SetPadDirectionScreen_ForwardPad_Off_Button, &rectangle);
		// Reverse Button
		gx_utility_rectangle_define (&rectangle, 152, 190, 152+88, 190+70);	// Left, top, right, bottom
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
	UINT myErr;
//	UINT pads, icons;
	GX_RESOURCE_ID pixelID;

	myErr = gx_window_event_process(window, event_ptr);

	switch (event_ptr->gx_event_type)
	{
	case GX_EVENT_SHOW:
		SetGroupIcon (&SetPadDirectionScreen.SetPadDirectionScreen_GroupIconButton);

		pixelID = GetDirectionIcon (g_PadSettings[LEFT_PAD].m_PadDirection);
		gx_pixelmap_button_pixelmap_set (&SetPadDirectionScreen.SetPadDirectionScreen_LeftPad_Off_Button, pixelID, pixelID, pixelID);

		pixelID = GetDirectionIcon (g_PadSettings[RIGHT_PAD].m_PadDirection);
		gx_pixelmap_button_pixelmap_set (&SetPadDirectionScreen.SetPadDirectionScreen_RightPad_Off_Button, pixelID, pixelID, pixelID);

		pixelID = GetDirectionIcon (g_PadSettings[FORWARD_PAD].m_PadDirection);
		gx_pixelmap_button_pixelmap_set (&SetPadDirectionScreen.SetPadDirectionScreen_ForwardPad_Off_Button, pixelID, pixelID, pixelID);

		pixelID = GetDirectionIcon (g_PadSettings[REVERSE_PAD].m_PadDirection);
		gx_pixelmap_button_pixelmap_set (&SetPadDirectionScreen.SetPadDirectionScreen_ReversePad_Off_Button, pixelID, pixelID, pixelID);

		ShowPads();		// This hides or shows the Reverse Pad based upon Device Type.

		// Show the Device icon in the middle of the screen.
		SetDeviceIcon(&SetPadDirectionScreen.SetPadDirectionScreen_DeviceType_icon);

		//// Show correct settings for LEFT pad. Off, Right, Forward or Left.
		//// First let's hide all choices for all pads.
		//for (pads = 0; pads < 3; ++pads)
		//{
		//	for (icons = 0; icons < 4; ++icons)
		//	{
		//		myErr = gx_widget_resize ((GX_WIDGET*) g_PadSettings[pads].m_DirectionIcons[icons], &g_HiddenRectangle);
		//	}
		//	myErr = gx_widget_resize ((GX_WIDGET*) g_PadSettings[pads].m_DirectionIcons[g_PadSettings[pads].m_PadDirection], &g_PadDirectionLocation[pads]);
		//}

		break;

	case GX_SIGNAL(OK_BTN_ID, GX_EVENT_CLICKED):
        screen_toggle((GX_WINDOW *)&PadOptionsSettingsScreen, window);
		break;
		// Process LEFT button pushes

	case GX_SIGNAL(LEFT_PAD_OFF_BTN_ID, GX_EVENT_CLICKED):
		SetNextPadDirection (GetActiveGroup(), LEFT_PAD);
		pixelID = GetDirectionIcon (g_PadSettings[LEFT_PAD].m_PadDirection);
		gx_pixelmap_button_pixelmap_set (&SetPadDirectionScreen.SetPadDirectionScreen_LeftPad_Off_Button, pixelID, pixelID, pixelID);
		break;

	case GX_SIGNAL(RIGHT_PAD_OFF_BTN_ID, GX_EVENT_CLICKED):
		SetNextPadDirection (GetActiveGroup(), RIGHT_PAD);
		pixelID = GetDirectionIcon (g_PadSettings[RIGHT_PAD].m_PadDirection);
		gx_pixelmap_button_pixelmap_set (&SetPadDirectionScreen.SetPadDirectionScreen_RightPad_Off_Button, pixelID, pixelID, pixelID);
		break;

	case GX_SIGNAL(FORWARD_PAD_OFF_BTN_ID, GX_EVENT_CLICKED):
		SetNextPadDirection (GetActiveGroup(), FORWARD_PAD);
		pixelID = GetDirectionIcon (g_PadSettings[FORWARD_PAD].m_PadDirection);
		gx_pixelmap_button_pixelmap_set (&SetPadDirectionScreen.SetPadDirectionScreen_ForwardPad_Off_Button, pixelID, pixelID, pixelID);
		break;

	case GX_SIGNAL(REVERSE_PAD_OFF_BTN_ID, GX_EVENT_CLICKED):
		SetNextPadDirection (GetActiveGroup(), REVERSE_PAD);
		pixelID = GetDirectionIcon (g_PadSettings[REVERSE_PAD].m_PadDirection);
		gx_pixelmap_button_pixelmap_set (&SetPadDirectionScreen.SetPadDirectionScreen_ReversePad_Off_Button, pixelID, pixelID, pixelID);
		break;
#if 0
		// Process LEFT button pushes
	case GX_SIGNAL(LEFT_PAD_OFF_BTN_ID, GX_EVENT_CLICKED):
		myErr = gx_widget_resize ((GX_WIDGET*) g_PadSettings[LEFT_PAD].m_DirectionIcons[OFF_DIRECTION], &g_HiddenRectangle);
		myErr = gx_widget_resize ((GX_WIDGET*) g_PadSettings[LEFT_PAD].m_DirectionIcons[LEFT_DIRECTION], &g_PadDirectionLocation[LEFT_PAD]);
		g_PadSettings[LEFT_PAD].m_PadDirection = LEFT_DIRECTION;
		break;
	case GX_SIGNAL(LEFT_PAD_LEFT_ARROW_BTN_ID, GX_EVENT_CLICKED):
		myErr = gx_widget_resize ((GX_WIDGET*) g_PadSettings[LEFT_PAD].m_DirectionIcons[LEFT_DIRECTION], &g_HiddenRectangle);
		myErr = gx_widget_resize ((GX_WIDGET*) g_PadSettings[LEFT_PAD].m_DirectionIcons[FORWARD_DIRECTION], &g_PadDirectionLocation[LEFT_PAD]);
		g_PadSettings[LEFT_PAD].m_PadDirection = FORWARD_DIRECTION;
		break;
	case GX_SIGNAL(LEFT_PAD_FORWARD_ARROW_BTN_ID, GX_EVENT_CLICKED):
		myErr = gx_widget_resize ((GX_WIDGET*) g_PadSettings[LEFT_PAD].m_DirectionIcons[FORWARD_DIRECTION], &g_HiddenRectangle);
		myErr = gx_widget_resize ((GX_WIDGET*) g_PadSettings[LEFT_PAD].m_DirectionIcons[RIGHT_DIRECTION], &g_PadDirectionLocation[LEFT_PAD]);
		g_PadSettings[LEFT_PAD].m_PadDirection = RIGHT_DIRECTION;
		break;
	case GX_SIGNAL(LEFT_PAD_RIGHT_ARROW_BTN_ID, GX_EVENT_CLICKED):
		myErr = gx_widget_resize ((GX_WIDGET*) g_PadSettings[LEFT_PAD].m_DirectionIcons[RIGHT_DIRECTION], &g_HiddenRectangle);
		myErr = gx_widget_resize ((GX_WIDGET*) g_PadSettings[LEFT_PAD].m_DirectionIcons[OFF_DIRECTION], &g_PadDirectionLocation[LEFT_PAD]);
		g_PadSettings[LEFT_PAD].m_PadDirection = OFF_DIRECTION;
		break;
	// Process RIGHT button pushes
	case GX_SIGNAL(RIGHT_PAD_OFF_BTN_ID, GX_EVENT_CLICKED):
		myErr = gx_widget_resize ((GX_WIDGET*) g_PadSettings[RIGHT_PAD].m_DirectionIcons[OFF_DIRECTION], &g_HiddenRectangle);
		myErr = gx_widget_resize ((GX_WIDGET*) g_PadSettings[RIGHT_PAD].m_DirectionIcons[LEFT_DIRECTION], &g_PadDirectionLocation[RIGHT_PAD]);
		g_PadSettings[RIGHT_PAD].m_PadDirection = LEFT_DIRECTION;
		break;
	case GX_SIGNAL(RIGHT_PAD_LEFT_ARROW_BTN_ID, GX_EVENT_CLICKED):
		myErr = gx_widget_resize ((GX_WIDGET*) g_PadSettings[RIGHT_PAD].m_DirectionIcons[LEFT_DIRECTION], &g_HiddenRectangle);
		myErr = gx_widget_resize ((GX_WIDGET*) g_PadSettings[RIGHT_PAD].m_DirectionIcons[FORWARD_DIRECTION], &g_PadDirectionLocation[RIGHT_PAD]);
		g_PadSettings[RIGHT_PAD].m_PadDirection = FORWARD_DIRECTION;
		break;
	case GX_SIGNAL(RIGHT_PAD_FORWARD_ARROW_BTN_ID, GX_EVENT_CLICKED):
		myErr = gx_widget_resize ((GX_WIDGET*) g_PadSettings[RIGHT_PAD].m_DirectionIcons[FORWARD_DIRECTION], &g_HiddenRectangle);
		myErr = gx_widget_resize ((GX_WIDGET*) g_PadSettings[RIGHT_PAD].m_DirectionIcons[RIGHT_DIRECTION], &g_PadDirectionLocation[RIGHT_PAD]);
		g_PadSettings[RIGHT_PAD].m_PadDirection = RIGHT_DIRECTION;
		break;
	case GX_SIGNAL(RIGHT_PAD_RIGHT_ARROW_BTN_ID, GX_EVENT_CLICKED):
		myErr = gx_widget_resize ((GX_WIDGET*) g_PadSettings[RIGHT_PAD].m_DirectionIcons[RIGHT_DIRECTION], &g_HiddenRectangle);
		myErr = gx_widget_resize ((GX_WIDGET*) g_PadSettings[RIGHT_PAD].m_DirectionIcons[OFF_DIRECTION], &g_PadDirectionLocation[RIGHT_PAD]);
		g_PadSettings[RIGHT_PAD].m_PadDirection = OFF_DIRECTION;
		break;
	// Process CENTER PAD button pushes
	case GX_SIGNAL(CENTER_PAD_OFF_BTN_ID, GX_EVENT_CLICKED):
		myErr = gx_widget_resize ((GX_WIDGET*) g_PadSettings[CENTER_PAD].m_DirectionIcons[OFF_DIRECTION], &g_HiddenRectangle);
		myErr = gx_widget_resize ((GX_WIDGET*) g_PadSettings[CENTER_PAD].m_DirectionIcons[LEFT_DIRECTION], &g_PadDirectionLocation[CENTER_PAD]);
		g_PadSettings[CENTER_PAD].m_PadDirection = LEFT_DIRECTION;
		break;
	case GX_SIGNAL(CENTER_PAD_LEFT_ARROW_BTN_ID, GX_EVENT_CLICKED):
		myErr = gx_widget_resize ((GX_WIDGET*) g_PadSettings[CENTER_PAD].m_DirectionIcons[LEFT_DIRECTION], &g_HiddenRectangle);
		myErr = gx_widget_resize ((GX_WIDGET*) g_PadSettings[CENTER_PAD].m_DirectionIcons[FORWARD_DIRECTION], &g_PadDirectionLocation[CENTER_PAD]);
		g_PadSettings[CENTER_PAD].m_PadDirection = FORWARD_DIRECTION;
		break;
	case GX_SIGNAL(CENTER_PAD_FORWARD_ARROW_BTN_ID, GX_EVENT_CLICKED):
		myErr = gx_widget_resize ((GX_WIDGET*) g_PadSettings[CENTER_PAD].m_DirectionIcons[FORWARD_DIRECTION], &g_HiddenRectangle);
		myErr = gx_widget_resize ((GX_WIDGET*) g_PadSettings[CENTER_PAD].m_DirectionIcons[RIGHT_DIRECTION], &g_PadDirectionLocation[CENTER_PAD]);
		g_PadSettings[CENTER_PAD].m_PadDirection = RIGHT_DIRECTION;
		break;
	case GX_SIGNAL(CENTER_PAD_RIGHT_ARROW_BTN_ID, GX_EVENT_CLICKED):
		myErr = gx_widget_resize ((GX_WIDGET*) g_PadSettings[CENTER_PAD].m_DirectionIcons[RIGHT_DIRECTION], &g_HiddenRectangle);
		myErr = gx_widget_resize ((GX_WIDGET*) g_PadSettings[CENTER_PAD].m_DirectionIcons[OFF_DIRECTION], &g_PadDirectionLocation[CENTER_PAD]);
		g_PadSettings[CENTER_PAD].m_PadDirection = OFF_DIRECTION;
		break;
#endif 

	} // end switch on event


	return GX_SUCCESS;
}













