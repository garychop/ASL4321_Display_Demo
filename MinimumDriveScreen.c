//*****************************************************************************
// Filename: MinimumDriveScreen.cpp
//
// Date: Aug 28, 2020
//
// Author: G. Chopcinski, Kg Solutions, LLC
// 
//*****************************************************************************

#include "ASL4321_System.h"
#include "DataDictionary.h"
#include "PadInfo.h"

//*************************************************************************************
// Function Name: MinimumDriveScreen_event_process
//
// Description: This dispatches the Pad Option Settings Screen messages
//
//*************************************************************************************
static VOID ShowPads (VOID)
{
	GX_RECTANGLE rectangle;

	if (dd_Get_USHORT (dd_Get_USHORT (0, DD_GROUP), DD_DEVICE_TYPE) == DEVICE_TYPE_HEAD_ARRAY)
	{
		// Move device icon
		gx_utility_rectangle_define (&rectangle, 154, 90, 154+88, 90+70);	// Left, top, right, bottom
		gx_widget_resize (&MinimumDriveScreen.MinimumDriveScreen_DeviceType_icon, &rectangle);
		// Left Button
		gx_utility_rectangle_define (&rectangle, 64, 90, 64+88, 90+70);	// Left, top, right, bottom
		gx_widget_resize (&MinimumDriveScreen.MinimumDriveScreen_LeftPadPercentage_Button, &rectangle);
		// Right Button
		gx_utility_rectangle_define (&rectangle, 242, 90, 242+88, 90+70);	// Left, top, right, bottom
		gx_widget_resize (&MinimumDriveScreen.MinimumDriveScreen_RightPadPercentage_Button, &rectangle);
		// Center/Forward Pad
		gx_utility_rectangle_define (&rectangle, 154, 160, 152+88, 160+70);	// Left, top, right, bottom
		gx_widget_resize (&MinimumDriveScreen.MinimumDriveScreen_ForwardPadPercentage_Button, &rectangle);
		// Hide the revese button
		gx_widget_hide (&MinimumDriveScreen.MinimumDriveScreen_ReversePadPercentage_Button);
	}
	else	// Must be a joystick, show all 4 pads.
	{
		// Move device icon
		gx_utility_rectangle_define (&rectangle, 154, 120, 154+88, 120+70);	// Left, top, right, bottom
		gx_widget_resize (&MinimumDriveScreen.MinimumDriveScreen_DeviceType_icon, &rectangle);
		// Left Button
		gx_utility_rectangle_define (&rectangle, 64, 120, 64+88, 120+70);	// Left, top, right, bottom
		gx_widget_resize (&MinimumDriveScreen.MinimumDriveScreen_LeftPadPercentage_Button, &rectangle);
		// Right Button
		gx_utility_rectangle_define (&rectangle, 242, 120, 242+88, 120+70);	// Left, top, right, bottom
		gx_widget_resize (&MinimumDriveScreen.MinimumDriveScreen_RightPadPercentage_Button, &rectangle);
		// Center/Forward Pad
		gx_utility_rectangle_define (&rectangle, 154, 50, 154+88, 50+70);	// Left, top, right, bottom
		gx_widget_resize (&MinimumDriveScreen.MinimumDriveScreen_ForwardPadPercentage_Button, &rectangle);
		// Reverse Button
		gx_utility_rectangle_define (&rectangle, 154, 190, 154+88, 190+70);	// Left, top, right, bottom
		gx_widget_resize (&MinimumDriveScreen.MinimumDriveScreen_ReversePadPercentage_Button, &rectangle);
		gx_widget_show (&MinimumDriveScreen.MinimumDriveScreen_ReversePadPercentage_Button);
	}
	// Display value in buttons.
	gx_text_button_text_set (&MinimumDriveScreen.MinimumDriveScreen_ForwardPadPercentage_Button, g_PadSettings[FORWARD_PAD].m_MinimuDriveString);
	gx_text_button_text_set (&MinimumDriveScreen.MinimumDriveScreen_LeftPadPercentage_Button, g_PadSettings[LEFT_PAD].m_MinimuDriveString);
	gx_text_button_text_set (&MinimumDriveScreen.MinimumDriveScreen_RightPadPercentage_Button, g_PadSettings[RIGHT_PAD].m_MinimuDriveString);
	gx_text_button_text_set (&MinimumDriveScreen.MinimumDriveScreen_ReversePadPercentage_Button, g_PadSettings[REVERSE_PAD].m_MinimuDriveString);
}

VOID AdjustMinimumSpeedValue (PHYSICAL_PAD_ENUM pad)
{
	if (g_PadSettings[pad].m_MinimumDriveValue >= MAXIMUM_DRIVE_SPEED)
	{
		g_PadSettings[pad].m_MinimumDriveValue = 0;
		strcpy_s (g_PadSettings[pad].m_MinimuDriveString, sizeof (g_PadSettings[pad].m_MinimuDriveString), "OFF");
	}
	else
	{
		g_PadSettings[pad].m_MinimumDriveValue += 5;
		sprintf_s (g_PadSettings[pad].m_MinimuDriveString, sizeof (g_PadSettings[pad].m_MinimuDriveString), "%d%%", g_PadSettings[pad].m_MinimumDriveValue);
	}
}

UINT MinimumDriveScreen_event_process (GX_WINDOW *window, GX_EVENT *event_ptr)
{
	UINT myErr;

    myErr = gx_window_event_process(window, event_ptr);

	switch (event_ptr->gx_event_type)
	{
	case GX_EVENT_SHOW:
		SetGroupIcon (&MinimumDriveScreen.MinimumDriveScreen_GroupIconButton);
		// Show the Device icon in the middle of the screen.
		SetDeviceIcon(&MinimumDriveScreen.MinimumDriveScreen_DeviceType_icon);
		break;

	case GX_SIGNAL(OK_BTN_ID, GX_EVENT_CLICKED):
        screen_toggle((GX_WINDOW *)&PadOptionsSettingsScreen, window);
		break;

	case GX_SIGNAL (FORWARD_PAD_PERCENTAGE_BTN_ID, GX_EVENT_CLICKED):
		AdjustMinimumSpeedValue (FORWARD_PAD);
		break;

	case GX_SIGNAL (LEFT_PAD_PERCENTAGE_BTN_ID, GX_EVENT_CLICKED):
		AdjustMinimumSpeedValue (LEFT_PAD);
		break;

	case GX_SIGNAL (RIGHT_PAD_PERCENTAGE_BTN_ID, GX_EVENT_CLICKED):
		AdjustMinimumSpeedValue (RIGHT_PAD);
		break;

	case GX_SIGNAL (REVERSE_PAD_PERCENTAGE_BTN_ID, GX_EVENT_CLICKED):
		AdjustMinimumSpeedValue (REVERSE_PAD);
		break;

	} // end switch

	ShowPads();	// Put buttons in proper screen location and populate percentage string

	return GX_SUCCESS;
}







