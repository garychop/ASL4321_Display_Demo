//*****************************************************************************
// Filename: SetPadTypeScreen.cpp
//
// Date: Aug 28, 2020
//
// Author: G. Chopcinski, Kg Solutions, LLC
// 
//*****************************************************************************

#include "ASL4321_System.h"
#include "PadInfo.h"

//*************************************************************************************
// Local/Global variables
//*************************************************************************************

VOID DisplayPadType (VOID)
{
	if (g_PadSettings[LEFT_PAD].m_PadType)	// Digital?
	{
		gx_pixelmap_button_pixelmap_set (&SetPadTypeScreen.SetPadTypeScreen_LeftPadType_Button, GX_PIXELMAP_ID_DIGITAL_INDICATOR, GX_PIXELMAP_ID_DIGITAL_INDICATOR, GX_PIXELMAP_ID_DIGITAL_INDICATOR);
	}
	else
	{
		gx_pixelmap_button_pixelmap_set (&SetPadTypeScreen.SetPadTypeScreen_LeftPadType_Button, GX_PIXELMAP_ID_PROPORTIONAL_INDICATOR_ORANGE, GX_PIXELMAP_ID_PROPORTIONAL_INDICATOR_ORANGE, GX_PIXELMAP_ID_PROPORTIONAL_INDICATOR_ORANGE);
	}
	if (g_PadSettings[RIGHT_PAD].m_PadType)	// Digital?
	{
		gx_pixelmap_button_pixelmap_set (&SetPadTypeScreen.SetPadTypeScreen_RightPadType_Button, GX_PIXELMAP_ID_DIGITAL_INDICATOR, GX_PIXELMAP_ID_DIGITAL_INDICATOR, GX_PIXELMAP_ID_DIGITAL_INDICATOR);
	}
	else
	{
		gx_pixelmap_button_pixelmap_set (&SetPadTypeScreen.SetPadTypeScreen_RightPadType_Button, GX_PIXELMAP_ID_PROPORTIONAL_INDICATOR_ORANGE, GX_PIXELMAP_ID_PROPORTIONAL_INDICATOR_ORANGE, GX_PIXELMAP_ID_PROPORTIONAL_INDICATOR_ORANGE);
	}
	if (g_PadSettings[FORWARD_PAD].m_PadType)	// Digital?
	{
		gx_pixelmap_button_pixelmap_set (&SetPadTypeScreen.SetPadTypeScreen_ForwardPadType_Button, GX_PIXELMAP_ID_DIGITAL_INDICATOR, GX_PIXELMAP_ID_DIGITAL_INDICATOR, GX_PIXELMAP_ID_DIGITAL_INDICATOR);
	}
	else
	{
		gx_pixelmap_button_pixelmap_set (&SetPadTypeScreen.SetPadTypeScreen_ForwardPadType_Button, GX_PIXELMAP_ID_PROPORTIONAL_INDICATOR_ORANGE, GX_PIXELMAP_ID_PROPORTIONAL_INDICATOR_ORANGE, GX_PIXELMAP_ID_PROPORTIONAL_INDICATOR_ORANGE);
	}
}

//*************************************************************************************
// Function Name: SetPadTypeScreen_event_process
//
// Description: This handles the Set Pad Screen messages
//
//*************************************************************************************

UINT SetPadTypeScreen_event_process (GX_WINDOW *window, GX_EVENT *event_ptr)
{
	UINT myErr = -1;

	switch (event_ptr->gx_event_type)
	{
	case GX_EVENT_SHOW:
		// Display the active group.
		SetGroupIcon (&SetPadTypeScreen.SetPadTypeScreen_GroupIconButton);
		DisplayPadType();
		g_ChangeScreen_WIP = FALSE;
		break;

	case GX_SIGNAL(OK_BTN_ID, GX_EVENT_CLICKED):
        screen_toggle((GX_WINDOW *)&PadOptionsSettingsScreen, window);
		break;

	case GX_SIGNAL(RIGHT_PAD_TYPE_BTN_ID, GX_EVENT_CLICKED):
		if (g_PadSettings[RIGHT_PAD].m_PadType == DIGITAL_PADTYPE)
			g_PadSettings[RIGHT_PAD].m_PadType = PROPORTIONAL_PADTYPE;
		else
			g_PadSettings[RIGHT_PAD].m_PadType = DIGITAL_PADTYPE;
		DisplayPadType();
		break;
	case GX_SIGNAL(LEFT_PAD_TYPE_BTN_ID, GX_EVENT_CLICKED):
		if (g_PadSettings[LEFT_PAD].m_PadType == DIGITAL_PADTYPE)
			g_PadSettings[LEFT_PAD].m_PadType = PROPORTIONAL_PADTYPE;
		else
			g_PadSettings[LEFT_PAD].m_PadType = DIGITAL_PADTYPE;
		DisplayPadType();
		break;
	case GX_SIGNAL(FORWARD_PAD_TYPE_BTN_ID, GX_EVENT_CLICKED):
		if (g_PadSettings[FORWARD_PAD].m_PadType == DIGITAL_PADTYPE)
			g_PadSettings[FORWARD_PAD].m_PadType = PROPORTIONAL_PADTYPE;
		else
			g_PadSettings[FORWARD_PAD].m_PadType = DIGITAL_PADTYPE;
		DisplayPadType();
		break;

	case GX_EVENT_TIMER:
        if (event_ptr->gx_event_payload.gx_event_timer_id == CALIBRATION_TIMER_ID)
		{
			gx_system_timer_stop(window, CALIBRATION_TIMER_ID);
	        screen_toggle((GX_WINDOW *)&PadCalibrationScreen, window);
		}
		break;
	case GX_EVENT_PEN_DOWN:	// We are going to determine if the Up or Down arrow buttons have been held for a
							// ... long time (2 seconds) and goto calibration if so.
		if (event_ptr->gx_event_target->gx_widget_name == "ForwardPadType_Button")
		{
			if (g_PadSettings[FORWARD_PAD].m_PadType == PROPORTIONAL_PADTYPE)
			{
				g_CalibrationPadNumber = FORWARD_PAD; 
				gx_system_timer_start(window, CALIBRATION_TIMER_ID, 100, 0);
			}
		}
		else if (event_ptr->gx_event_target->gx_widget_name == "LeftPadType_Button")
		{
			if (g_PadSettings[LEFT_PAD].m_PadType == PROPORTIONAL_PADTYPE)
			{
				g_CalibrationPadNumber = LEFT_PAD; 
				gx_system_timer_start(window, CALIBRATION_TIMER_ID, 100, 0);
			}
		}
		else if (event_ptr->gx_event_target->gx_widget_name == "RightPadType_Button")
		{
			if (g_PadSettings[RIGHT_PAD].m_PadType == PROPORTIONAL_PADTYPE)
			{
				g_CalibrationPadNumber = RIGHT_PAD; 
				gx_system_timer_start(window, CALIBRATION_TIMER_ID, 100, 0);
			}
		}
		break;
	case GX_EVENT_PEN_UP:
		gx_system_timer_stop(window, CALIBRATION_TIMER_ID);
		break;

	}

    myErr = gx_window_event_process(window, event_ptr);

	return myErr;
}












