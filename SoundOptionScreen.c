//*****************************************************************************
// Filename: SoundOptionScreen.cpp
//
// Date: Jam 28, 2021
//
// Author: G. Chopcinski, Kg Solutions, LLC
// 
//*****************************************************************************

#include "ASL4321_System.h"

//*************************************************************************************
//*************************************************************************************

//*************************************************************************************
// Function Name: SoundOptionScreen_event_process
//
// Description: This dispatches the Sound Option
//
//*************************************************************************************

UINT SoundOptionScreen_event_process (GX_WINDOW *window, GX_EVENT *event_ptr)
{
	UINT myErr;

	switch (event_ptr->gx_event_type)
	{
	case GX_EVENT_SHOW:
		SetGroupIcon (&SoundOptionScreen.SoundOptionScreen_GroupIconButton);
		break;

	case GX_SIGNAL(OK_BTN_ID, GX_EVENT_CLICKED):
        screen_toggle((GX_WINDOW *)&HHP_Start_Screen, window);
		break;

	case GX_SIGNAL(SOUND_BTN_ID, GX_EVENT_CLICKED):
        screen_toggle((GX_WINDOW *)&ManageSoundScreen, window);
		break;

	case GX_SIGNAL(USER_SOUND_SETUP_BTN_ID, GX_EVENT_CLICKED):
        screen_toggle((GX_WINDOW *)&SoundSetupScreen, window);
		break;
	}

    myErr = gx_window_event_process(window, event_ptr);

	return GX_SUCCESS;
}

//*************************************************************************************

UINT ManageSoundScreen_event_process (GX_WINDOW *window, GX_EVENT *event_ptr)
{
	UINT myErr;

	switch (event_ptr->gx_event_type)
	{
	case GX_SIGNAL(OK_BTN_ID, GX_EVENT_CLICKED):
        screen_toggle((GX_WINDOW *)&SoundOptionScreen, window);
		break;
	}

    myErr = gx_window_event_process(window, event_ptr);

	return GX_SUCCESS;
}

//*************************************************************************************

UINT SoundSetupScreen_event_process (GX_WINDOW *window, GX_EVENT *event_ptr)
{
	UINT myErr;

	switch (event_ptr->gx_event_type)
	{
	case GX_SIGNAL(OK_BTN_ID, GX_EVENT_CLICKED):
        screen_toggle((GX_WINDOW *)&SoundOptionScreen, window);
		break;
	}

    myErr = gx_window_event_process(window, event_ptr);

	return GX_SUCCESS;
}




