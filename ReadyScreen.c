//*****************************************************************************
// Filename: ReadyScreen.cpp
// Description: This file supports the "I'm ready or not ready screen".
//
// Date: Aug 28, 2020
//
// Author: G. Chopcinski, Kg Solutions, LLC
// 
//*****************************************************************************

#include "ASL4321_System.h"

//*************************************************************************************

//*************************************************************************************
// Description: This handles the Ready Screen message
//
//*************************************************************************************

UINT ReadyScreen_event_function (GX_WINDOW *window, GX_EVENT *event_ptr)
{
	switch (event_ptr->gx_event_type)
	{
		case GX_SIGNAL (USER_PORT_BUTTON_ID, GX_EVENT_CLICKED):
		{
	        screen_toggle((GX_WINDOW *)&MainUserScreen, window);
		}
		break;
	} // end switch

	gx_window_event_process(window, event_ptr);

	return GX_SUCCESS;
}




