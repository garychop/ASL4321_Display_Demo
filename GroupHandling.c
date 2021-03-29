//*****************************************************************************
// Filename: GroupHandling.c
// Description: This file contains functions to support Group handling
//
// Date: Feb 9, 2021
//
// Author: G. Chopcinski, Kg Solutions, LLC
// 
//*****************************************************************************

#include "ASL4321_System.h"
#include "HUB.h"
#include "DataDictionary.h"

//*************************************************************************************

//*************************************************************************************
// Global Variables
GROUP_STRUCT g_GroupInfo[MAX_GROUPS];

//*************************************************************************************

VOID InitializeGroupPadInformation(PAD_INFO_STRUCT *padInfo)
{
	int i;

	// Populate the default Pad settings.
	padInfo[LEFT_PAD].m_PadDirection = LEFT_DIRECTION;
	padInfo[LEFT_PAD].m_PadType = PROPORTIONAL_PADTYPE;
	padInfo[LEFT_PAD].m_MinimumDriveValue = 5;
	padInfo[LEFT_PAD].m_NeutralWindowValue = 5;
	padInfo[LEFT_PAD].m_PadMinimumCalibrationValue = 2;
	padInfo[LEFT_PAD].m_PadMaximumCalibrationValue = 30;
	padInfo[LEFT_PAD].m_Speed_Offset = 0;
	padInfo[LEFT_PAD].m_Direction_Offset = 0;
	for (i=0; i<MAX_ADVANCE_PAD_OPERATIONS; ++i) padInfo[LEFT_PAD].m_LongPadOperations[i] = OFF_CYCLE;

	padInfo[RIGHT_PAD].m_PadDirection = RIGHT_DIRECTION;
	padInfo[RIGHT_PAD].m_PadType = PROPORTIONAL_PADTYPE;
	padInfo[RIGHT_PAD].m_MinimumDriveValue = 10;
	padInfo[RIGHT_PAD].m_NeutralWindowValue = 5;
	padInfo[RIGHT_PAD].m_PadMinimumCalibrationValue = 2;
	padInfo[RIGHT_PAD].m_PadMaximumCalibrationValue = 30;
	padInfo[RIGHT_PAD].m_Speed_Offset = 0;
	padInfo[RIGHT_PAD].m_Direction_Offset = 0;
	for (i=0; i<MAX_ADVANCE_PAD_OPERATIONS; ++i) padInfo[RIGHT_PAD].m_LongPadOperations[i] = OFF_CYCLE;

	padInfo[FORWARD_PAD].m_PadDirection = FORWARD_DIRECTION;
	padInfo[FORWARD_PAD].m_PadType = PROPORTIONAL_PADTYPE;
	padInfo[FORWARD_PAD].m_MinimumDriveValue = 15;
	padInfo[FORWARD_PAD].m_NeutralWindowValue = 5;
	padInfo[FORWARD_PAD].m_PadMinimumCalibrationValue = 2;
	padInfo[FORWARD_PAD].m_PadMaximumCalibrationValue = 30;
	padInfo[FORWARD_PAD].m_Speed_Offset = 0;
	padInfo[FORWARD_PAD].m_Direction_Offset = 0;
	for (i=0; i<MAX_ADVANCE_PAD_OPERATIONS; ++i) padInfo[FORWARD_PAD].m_LongPadOperations[i] = OFF_CYCLE;

	padInfo[REVERSE_PAD].m_PadDirection = REVERSE_DIRECTION;
	padInfo[REVERSE_PAD].m_PadType = PROPORTIONAL_PADTYPE;
	padInfo[REVERSE_PAD].m_MinimumDriveValue = 20;
	padInfo[REVERSE_PAD].m_NeutralWindowValue = 5;
	padInfo[REVERSE_PAD].m_PadMinimumCalibrationValue = 2;
	padInfo[REVERSE_PAD].m_PadMaximumCalibrationValue = 30;
	padInfo[REVERSE_PAD].m_Speed_Offset = 0;
	padInfo[REVERSE_PAD].m_Direction_Offset = 0;
	for (i=0; i<MAX_ADVANCE_PAD_OPERATIONS; ++i) padInfo[REVERSE_PAD].m_LongPadOperations[i] = OFF_CYCLE;

}

static VOID InitializeAudibleSubItems (FEATURE_GROUP_STRUCT *audible)
{
	int idx;

	for (idx = 0; idx < MAX_AUDIBLE_SUBITEMS; ++idx)
	{
		audible->m_Audible_Subitem[idx].m_ForwardPad_SoundID = MAX_SOUND_BITES;
		audible->m_Audible_Subitem[idx].m_ReversePad_SoundID = MAX_SOUND_BITES;
		audible->m_Audible_Subitem[idx].m_LeftPad_SoundID = MAX_SOUND_BITES;
		audible->m_Audible_Subitem[idx].m_RightPad_SoundID = MAX_SOUND_BITES;
	}
}

//*************************************************************************************

static VOID InitializeIndividualGroup (USHORT group)
{
	g_GroupInfo[group].m_DeviceType = DEVICE_TYPE_HEAD_ARRAY;
	g_GroupInfo[group].m_ScanMode = SCAN_OFF;

	InitializeGroupPadInformation (g_GroupInfo[group].m_GroupPadInfo);

	g_GroupInfo[group].m_GroupFeature[POWER_ONOFF_ID].m_FeatureID = POWER_ONOFF_ID;
	g_GroupInfo[group].m_GroupFeature[POWER_ONOFF_ID].m_Attribute = FEATURE_AVAILABLE | FEATURE_ENABLED | (POWER_ONOFF_ID << 4);
	g_GroupInfo[group].m_GroupFeature[POWER_ONOFF_ID].m_SelectedSubItem = 0;

	g_GroupInfo[group].m_GroupFeature[BLUETOOTH_ID].m_FeatureID = BLUETOOTH_ID;
	g_GroupInfo[group].m_GroupFeature[BLUETOOTH_ID].m_Attribute = FEATURE_AVAILABLE | FEATURE_ENABLED | (BLUETOOTH_ID << 4);
	g_GroupInfo[group].m_GroupFeature[BLUETOOTH_ID].m_SelectedSubItem = 0;

	g_GroupInfo[group].m_GroupFeature[NEXT_FUNCTION_ID].m_FeatureID = NEXT_FUNCTION_ID;
	g_GroupInfo[group].m_GroupFeature[NEXT_FUNCTION_ID].m_Attribute = FEATURE_AVAILABLE | FEATURE_ENABLED | (NEXT_FUNCTION_ID << 4);
	g_GroupInfo[group].m_GroupFeature[NEXT_FUNCTION_ID].m_SelectedSubItem = 0;

	g_GroupInfo[group].m_GroupFeature[NEXT_PROFILE_ID].m_FeatureID = NEXT_PROFILE_ID;
	g_GroupInfo[group].m_GroupFeature[NEXT_PROFILE_ID].m_Attribute = FEATURE_AVAILABLE | FEATURE_ENABLED | (NEXT_PROFILE_ID << 4);
	g_GroupInfo[group].m_GroupFeature[NEXT_PROFILE_ID].m_SelectedSubItem = 0;

	g_GroupInfo[group].m_GroupFeature[NEXT_GROUP_ID].m_FeatureID = NEXT_GROUP_ID;
	g_GroupInfo[group].m_GroupFeature[NEXT_GROUP_ID].m_Attribute = FEATURE_AVAILABLE | FEATURE_ENABLED | (NEXT_GROUP_ID << 4);
	g_GroupInfo[group].m_GroupFeature[NEXT_GROUP_ID].m_SelectedSubItem = 0;

	g_GroupInfo[group].m_GroupFeature[RNET_TOGGLE_ID].m_FeatureID = RNET_TOGGLE_ID;
	g_GroupInfo[group].m_GroupFeature[RNET_TOGGLE_ID].m_Attribute = FEATURE_AVAILABLE | (RNET_TOGGLE_ID << 4);
	g_GroupInfo[group].m_GroupFeature[RNET_TOGGLE_ID].m_SelectedSubItem = 0;

	g_GroupInfo[group].m_GroupFeature[RNET_USER_MENU_ID].m_FeatureID = RNET_USER_MENU_ID;
	g_GroupInfo[group].m_GroupFeature[RNET_USER_MENU_ID].m_Attribute = FEATURE_AVAILABLE | (RNET_USER_MENU_ID << 4);
	g_GroupInfo[group].m_GroupFeature[RNET_USER_MENU_ID].m_SelectedSubItem = 0;

	g_GroupInfo[group].m_GroupFeature[RNET_SEATING_ID].m_FeatureID = RNET_SEATING_ID;
	g_GroupInfo[group].m_GroupFeature[RNET_SEATING_ID].m_Attribute = (RNET_SEATING_ID << 4);
	g_GroupInfo[group].m_GroupFeature[RNET_SEATING_ID].m_SelectedSubItem = 0;

	g_GroupInfo[group].m_GroupFeature[RNET_SLEEP_FEATURE_ID].m_FeatureID = RNET_SLEEP_FEATURE_ID;
	g_GroupInfo[group].m_GroupFeature[RNET_SLEEP_FEATURE_ID].m_Attribute = (RNET_SLEEP_FEATURE_ID << 4);
	g_GroupInfo[group].m_GroupFeature[RNET_SLEEP_FEATURE_ID].m_SelectedSubItem = 0;

	g_GroupInfo[group].m_GroupFeature[AUDIBLE_OUT_FEATURE_ID].m_FeatureID = AUDIBLE_OUT_FEATURE_ID;
	g_GroupInfo[group].m_GroupFeature[AUDIBLE_OUT_FEATURE_ID].m_Attribute = FEATURE_AVAILABLE | FEATURE_ENABLED | (AUDIBLE_OUT_FEATURE_ID << 4);
	g_GroupInfo[group].m_GroupFeature[AUDIBLE_OUT_FEATURE_ID].m_SelectedSubItem = 0;
	InitializeAudibleSubItems(&g_GroupInfo[group].m_GroupFeature[AUDIBLE_OUT_FEATURE_ID]);
	g_GroupInfo[group].m_GroupFeature[AUDIBLE_OUT_FEATURE_ID].m_Audible_Subitem[0].m_ForwardPad_SoundID = 1;
	g_GroupInfo[group].m_GroupFeature[AUDIBLE_OUT_FEATURE_ID].m_Audible_Subitem[0].m_ReversePad_SoundID = MAX_SOUND_BITES;
	g_GroupInfo[group].m_GroupFeature[AUDIBLE_OUT_FEATURE_ID].m_Audible_Subitem[0].m_LeftPad_SoundID = 3;
	g_GroupInfo[group].m_GroupFeature[AUDIBLE_OUT_FEATURE_ID].m_Audible_Subitem[0].m_RightPad_SoundID = 4;
	g_GroupInfo[group].m_GroupFeature[AUDIBLE_OUT_FEATURE_ID].m_Audible_Subitem[1].m_ForwardPad_SoundID = MAX_SOUND_BITES;
	g_GroupInfo[group].m_GroupFeature[AUDIBLE_OUT_FEATURE_ID].m_Audible_Subitem[1].m_ReversePad_SoundID = MAX_SOUND_BITES;
	g_GroupInfo[group].m_GroupFeature[AUDIBLE_OUT_FEATURE_ID].m_Audible_Subitem[1].m_LeftPad_SoundID = 5;
	g_GroupInfo[group].m_GroupFeature[AUDIBLE_OUT_FEATURE_ID].m_Audible_Subitem[1].m_RightPad_SoundID = 6;
	g_GroupInfo[group].m_GroupFeature[AUDIBLE_OUT_FEATURE_ID].m_Audible_Subitem[7].m_ForwardPad_SoundID = 2;
	g_GroupInfo[group].m_GroupFeature[AUDIBLE_OUT_FEATURE_ID].m_Audible_Subitem[7].m_ReversePad_SoundID = MAX_SOUND_BITES;
	g_GroupInfo[group].m_GroupFeature[AUDIBLE_OUT_FEATURE_ID].m_Audible_Subitem[7].m_LeftPad_SoundID = 3;
	g_GroupInfo[group].m_GroupFeature[AUDIBLE_OUT_FEATURE_ID].m_Audible_Subitem[7].m_RightPad_SoundID = MAX_SOUND_BITES;

	g_GroupInfo[group].m_GroupFeature[SEATING_FEATURE_ID].m_FeatureID = SEATING_FEATURE_ID;
	g_GroupInfo[group].m_GroupFeature[SEATING_FEATURE_ID].m_Attribute = FEATURE_AVAILABLE | FEATURE_ENABLED | (SEATING_FEATURE_ID << 4);
	g_GroupInfo[group].m_GroupFeature[SEATING_FEATURE_ID].m_SelectedSubItem = 0;

	g_GroupInfo[group].m_GroupFeature[TECLA_E_FEATURE_ID].m_FeatureID = TECLA_E_FEATURE_ID;
	g_GroupInfo[group].m_GroupFeature[TECLA_E_FEATURE_ID].m_Attribute = FEATURE_AVAILABLE | FEATURE_ENABLED | (TECLA_E_FEATURE_ID << 4);
	g_GroupInfo[group].m_GroupFeature[TECLA_E_FEATURE_ID].m_SelectedSubItem = 0;
}

//*************************************************************************************
// This is where the group information is read in from the Storage Device.
VOID InitializeGroupInformation (VOID)
{
	InitializeIndividualGroup (0);
	g_GroupInfo[0].m_DeviceType = DEVICE_TYPE_HEAD_ARRAY;
	g_GroupInfo[0].m_GroupFeature[BLUETOOTH_ID].m_Attribute ^= FEATURE_ENABLED;
	//g_GroupInfo[0].m_GroupFeature[BLUETOOTH_ID].m_Enabled = FALSE;

	InitializeIndividualGroup (1);
	g_GroupInfo[1].m_DeviceType = DEVICE_TYPE_HEAD_ARRAY;
	g_GroupInfo[1].m_GroupFeature[POWER_ONOFF_ID].m_Attribute ^= FEATURE_ENABLED;
	g_GroupInfo[1].m_GroupFeature[NEXT_FUNCTION_ID].m_Attribute ^= FEATURE_ENABLED;
	g_GroupInfo[1].m_GroupFeature[NEXT_PROFILE_ID].m_Attribute ^= FEATURE_ENABLED;
	g_GroupInfo[1].m_GroupFeature[TECLA_E_FEATURE_ID].m_Attribute ^= FEATURE_ENABLED;
	g_GroupInfo[1].m_GroupFeature[SEATING_FEATURE_ID].m_Attribute ^= FEATURE_ENABLED;
	//g_GroupInfo[1].m_GroupFeature[POWER_ONOFF_ID].m_Enabled = FALSE;
	//g_GroupInfo[1].m_GroupFeature[NEXT_FUNCTION_ID].m_Enabled = FALSE;
	//g_GroupInfo[1].m_GroupFeature[NEXT_PROFILE_ID].m_Enabled = FALSE;
	//g_GroupInfo[1].m_GroupFeature[TECLA_E_FEATURE_ID].m_Enabled = FALSE;
	//g_GroupInfo[1].m_GroupFeature[SEATING_FEATURE_ID].m_Enabled = FALSE;

	InitializeIndividualGroup (2);
	g_GroupInfo[2].m_DeviceType = DEVICE_TYPE_PROPORTIONAL_JOYSTICK;

	InitializeIndividualGroup (3);
	g_GroupInfo[3].m_DeviceType = DEVICE_TYPE_DIGITAL_JOYSTICK;
}

//*************************************************************************************
// Goes the next group, adjusts global var and returns the active group
//******************************************************************************************

VOID SelectNextGroup(VOID)
{
	USHORT group;

	group = dd_Get_USHORT (0, DD_GROUP);
	if (++group >= MAX_GROUPS)
		group = 0;
	dd_Set_USHORT (0, DD_GROUP, group);
}

