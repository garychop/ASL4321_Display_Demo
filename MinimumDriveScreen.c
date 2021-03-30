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

#define MIN_SPEED_STRING_LEN (8)

char g_MinimuDriveString[MAX_PHYSICAL_PADS][MIN_SPEED_STRING_LEN];
char g_NeutralWindowString[MAX_PHYSICAL_PADS][MIN_SPEED_STRING_LEN];

//*************************************************************************************
// External declaractions
extern VOID LocatePadPosition (PHYSICAL_PAD_ENUM pad, DEVICE_TYPE_ENUM device, GX_BOOL showPrompt, GX_ICON_BUTTON *icon, GX_PROMPT *prompt);

//*************************************************************************************
// Forward Declarations and Prototypes
VOID CreateMinSpeedString (USHORT minSpeed, char *str);

//*************************************************************************************
// ShowPads populates widgets with the converted strings and then moves them into
// screen position based upon 3 or 4 quadrant operation.

static VOID ShowPads (VOID)
{
	USHORT minSpeed;
	USHORT padIndex;
	USHORT group;
	DEVICE_TYPE_ENUM thisDevice;

	group = dd_Get_USHORT (0, DD_GROUP);	// Get currently selected group.

	// Get Minimum Speed values, convert them into string and populate widgets.
	for (padIndex = 0; padIndex < (USHORT) MAX_PHYSICAL_PADS; ++padIndex)
	{
		minSpeed = dd_GetSubItem_USHORT (group, DD_PAD_MINIMUM_DRIVE_SPEED, padIndex);
		CreateMinSpeedString (minSpeed, g_MinimuDriveString[padIndex]);
	}
	// Put values in buttons.
	gx_prompt_text_set (&MinimumDriveScreen.MinimumDriveScreen_ForwardPad_Prompt, g_MinimuDriveString[FORWARD_PAD]);
	gx_prompt_text_set (&MinimumDriveScreen.MinimumDriveScreen_LeftPad_Prompt, g_MinimuDriveString[LEFT_PAD]);
	gx_prompt_text_set (&MinimumDriveScreen.MinimumDriveScreen_RightPad_Prompt, g_MinimuDriveString[RIGHT_PAD]);
	gx_prompt_text_set (&MinimumDriveScreen.MinimumDriveScreen_ReversePad_Prompt, g_MinimuDriveString[REVERSE_PAD]);

	thisDevice = (DEVICE_TYPE_ENUM) dd_Get_USHORT (group, DD_DEVICE_TYPE);

	LocatePadPosition (LEFT_PAD, thisDevice, TRUE, &MinimumDriveScreen.MinimumDriveScreen_Left_IconButton, &MinimumDriveScreen.MinimumDriveScreen_LeftPad_Prompt);
	LocatePadPosition (RIGHT_PAD, thisDevice, TRUE, &MinimumDriveScreen.MinimumDriveScreen_Right_IconButton, &MinimumDriveScreen.MinimumDriveScreen_RightPad_Prompt);
	LocatePadPosition (FORWARD_PAD, thisDevice, TRUE, &MinimumDriveScreen.MinimumDriveScreen_Forward_IconButton, &MinimumDriveScreen.MinimumDriveScreen_ForwardPad_Prompt);
	LocatePadPosition (REVERSE_PAD, thisDevice, TRUE, &MinimumDriveScreen.MinimumDriveScreen_Reverse_IconButton, &MinimumDriveScreen.MinimumDriveScreen_ReversePad_Prompt);

}

//*************************************************************************************
VOID CreateMinSpeedString (USHORT minSpeed, char *str)
{
	if (minSpeed == 0)
		strcpy_s (str, MIN_SPEED_STRING_LEN, "OFF");
	else
		sprintf_s (str, MIN_SPEED_STRING_LEN, "%d%%", minSpeed);
}

//*************************************************************************************

VOID AdjustMinimumSpeedValue (PHYSICAL_PAD_ENUM pad)
{
	USHORT minSpeed;

	// Get Min speed
	minSpeed = dd_GetSubItem_USHORT (MAX_GROUPS, DD_PAD_MINIMUM_DRIVE_SPEED, pad);

	if (minSpeed >= MAXIMUM_DRIVE_SPEED)
	{
		minSpeed = 0;
	}
	else
	{
		minSpeed += 5;
	}
	dd_SetSubItem_USHORT (MAX_GROUPS, DD_PAD_MINIMUM_DRIVE_SPEED, pad, minSpeed);
}

//*************************************************************************************
// Function Name: MinimumDriveScreen_event_process
//
// Description: This dispatches the Pad Option Settings Screen messages
//
//*************************************************************************************

UINT MinimumDriveScreen_event_process (GX_WINDOW *window, GX_EVENT *event_ptr)
{
	UINT myErr;

    myErr = gx_window_event_process(window, event_ptr);

	switch (event_ptr->gx_event_type)
	{
	case GX_EVENT_SHOW:
		SetGroupIcon (&MinimumDriveScreen.MinimumDriveScreen_GroupIconButton);
		// Show the Device icon
		SetDeviceIcon(&MinimumDriveScreen.MinimumDriveScreen_DeviceType_icon);
		break;

	case GX_SIGNAL(OK_BTN_ID, GX_EVENT_CLICKED):
        screen_toggle((GX_WINDOW *)&PadOptionsSettingsScreen, window);
		break;

	case GX_SIGNAL (FORWARD_ICON_BTN_ID, GX_EVENT_CLICKED):
		AdjustMinimumSpeedValue (FORWARD_PAD);
		break;

	case GX_SIGNAL (LEFT_ICON_BTN_ID, GX_EVENT_CLICKED):
		AdjustMinimumSpeedValue (LEFT_PAD);
		break;

	case GX_SIGNAL (RIGHT_ICON_BTN_ID, GX_EVENT_CLICKED):
		AdjustMinimumSpeedValue (RIGHT_PAD);
		break;

	case GX_SIGNAL (REVERSE_ICON_BTN_ID, GX_EVENT_CLICKED):
		AdjustMinimumSpeedValue (REVERSE_PAD);
		break;

	} // end switch

	ShowPads();	// Put buttons in proper screen location and populate percentage string

	return GX_SUCCESS;
}







