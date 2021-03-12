//***********************************************************************************
// Project: ASL110 Display Demo package
//
// Filename: NeutralWindowScreen.c
//
// Description: This file supports the Neutral Window features
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

VOID AdjustNeutralWindowValue (VOID)
{
	if (g_PadSettings[LEFT_PAD].m_NeutralWindowValue >= MAX_NEUTRAL_WINDOW_VALUE)
	{
		g_PadSettings[LEFT_PAD].m_NeutralWindowValue = 0;
	}
	else
	{
		g_PadSettings[LEFT_PAD].m_NeutralWindowValue += 5;
	}
//	sprintf_s (g_PadSettings[LEFT_PAD].m_NeutralWindowString, sizeof (g_PadSettings[LEFT_PAD].m_NeutralWindowString), "%d%%", g_PadSettings[LEFT_PAD].m_NeutralWindowValue);
//	gx_text_button_text_set (&NeutralWindowScreen.NeutralWindowScreen_NeutralWindowPercentage_Button, g_PadSettings[LEFT_PAD].m_NeutralWindowString);

	g_PadSettings[RIGHT_PAD].m_NeutralWindowValue = g_PadSettings[LEFT_PAD].m_NeutralWindowValue;
	g_PadSettings[FORWARD_PAD].m_NeutralWindowValue = g_PadSettings[LEFT_PAD].m_NeutralWindowValue;
	g_PadSettings[REVERSE_PAD].m_NeutralWindowValue = g_PadSettings[LEFT_PAD].m_NeutralWindowValue;

}

//-----------------------------------------------------------------------------

UINT NeutralWindow_event_function (GX_WINDOW *window, GX_EVENT *event_ptr)
{
	switch (event_ptr->gx_event_type)
	{
	case GX_EVENT_SHOW:
		SetGroupIcon (&NeutralWindowScreen.NeutralWindowScreen_GroupIconButton);
		// Show the Device icon in the middle of the screen.
		SetDeviceIcon(&NeutralWindowScreen.NeutralWindowScreen_DeviceType_icon);
//		gx_text_button_text_set (&NeutralWindowScreen.NeutralWindowScreen_NeutralWindowPercentage_Button, g_PadSettings[LEFT_PAD].m_NeutralWindowString);
		break;

	case GX_SIGNAL(NEUTRAL_WINDOW_BTN_ID, GX_EVENT_CLICKED):
		AdjustNeutralWindowValue();
		break;

	case GX_SIGNAL(OK_BTN_ID, GX_EVENT_CLICKED):
        screen_toggle((GX_WINDOW *)&PadOptionsSettingsScreen, window);
		break;
	} // end switch

    gx_window_event_process(window, event_ptr);

	return GX_SUCCESS;
}
//-----------------------------------------------------------------------------
