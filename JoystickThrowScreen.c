//***********************************************************************************
// Project: ASL110 Display Demo package
//
// Filename: JoystickThrowScreen.c
//
// Description: This file supports the Joystick Throw screen
//
// Author: G. Chopcinski
//
// Creation Date: 2/11/21
//
//***********************************************************************************

#define GX_ARC_DRAWING_SUPPORT

#include "time.h"			// Windows supported timed functions.
#include "ASL4321_Simulator_Main.h"
#include "ASL4321_System.h"

//-----------------------------------------------------------------------------

UINT JoystickThrow_event_function (GX_WINDOW *window, GX_EVENT *event_ptr)
{
	switch (event_ptr->gx_event_type)
	{
	case GX_EVENT_SHOW:
		SetGroupIcon (&JoystickThrowScreen.JoystickThrowScreen_GroupIconButton);
		// Show the Device icon
		SetDeviceIcon(&JoystickThrowScreen.JoystickThrowScreen_DeviceType_icon);
		break;

	case GX_SIGNAL(OK_BTN_ID, GX_EVENT_CLICKED):
        screen_toggle((GX_WINDOW *)&PadOptionsSettingsScreen, window);
		break;
	} // end switch

    gx_window_event_process(window, event_ptr);

	return GX_SUCCESS;
}
//-----------------------------------------------------------------------------
