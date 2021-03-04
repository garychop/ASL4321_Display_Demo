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
#include "DataDictionary.h"

//*************************************************************************************

GROUP_STRUCT g_GroupInfo[MAX_GROUPS];

//*************************************************************************************

static VOID InitializeIndividualGroup (USHORT group)
{
	g_GroupInfo[group].m_DeviceType = DEVICE_TYPE_HEAD_ARRAY;
	g_GroupInfo[group].m_ScanMode = SCAN_OFF;

	g_GroupInfo[group].m_GroupFeature[POWER_ONOFF_ID].m_FeatureID = POWER_ONOFF_ID;
	g_GroupInfo[group].m_GroupFeature[POWER_ONOFF_ID].m_Available = TRUE;
	g_GroupInfo[group].m_GroupFeature[POWER_ONOFF_ID].m_Enabled = TRUE;
	g_GroupInfo[group].m_GroupFeature[POWER_ONOFF_ID].m_Priority = POWER_ONOFF_ID;

	g_GroupInfo[group].m_GroupFeature[BLUETOOTH_ID].m_FeatureID = BLUETOOTH_ID;
	g_GroupInfo[group].m_GroupFeature[BLUETOOTH_ID].m_Available = TRUE;
	g_GroupInfo[group].m_GroupFeature[BLUETOOTH_ID].m_Enabled = TRUE;
	g_GroupInfo[group].m_GroupFeature[BLUETOOTH_ID].m_Priority = BLUETOOTH_ID;

	g_GroupInfo[group].m_GroupFeature[NEXT_FUNCTION_ID].m_FeatureID = NEXT_FUNCTION_ID;
	g_GroupInfo[group].m_GroupFeature[NEXT_FUNCTION_ID].m_Available = TRUE;
	g_GroupInfo[group].m_GroupFeature[NEXT_FUNCTION_ID].m_Enabled = TRUE;
	g_GroupInfo[group].m_GroupFeature[NEXT_FUNCTION_ID].m_Priority = NEXT_FUNCTION_ID;

	g_GroupInfo[group].m_GroupFeature[NEXT_PROFILE_ID].m_FeatureID = NEXT_PROFILE_ID;
	g_GroupInfo[group].m_GroupFeature[NEXT_PROFILE_ID].m_Available = TRUE;
	g_GroupInfo[group].m_GroupFeature[NEXT_PROFILE_ID].m_Enabled = TRUE;
	g_GroupInfo[group].m_GroupFeature[NEXT_PROFILE_ID].m_Priority = NEXT_PROFILE_ID;

	g_GroupInfo[group].m_GroupFeature[NEXT_GROUP_ID].m_FeatureID = NEXT_GROUP_ID;
	g_GroupInfo[group].m_GroupFeature[NEXT_GROUP_ID].m_Available = TRUE;
	g_GroupInfo[group].m_GroupFeature[NEXT_GROUP_ID].m_Enabled = TRUE;
	g_GroupInfo[group].m_GroupFeature[NEXT_GROUP_ID].m_Priority = NEXT_GROUP_ID;

	g_GroupInfo[group].m_GroupFeature[RNET_TOGGLE_ID].m_FeatureID = RNET_TOGGLE_ID;
	g_GroupInfo[group].m_GroupFeature[RNET_TOGGLE_ID].m_Available = FALSE;
	g_GroupInfo[group].m_GroupFeature[RNET_TOGGLE_ID].m_Enabled = TRUE;
	g_GroupInfo[group].m_GroupFeature[RNET_TOGGLE_ID].m_Priority = RNET_TOGGLE_ID;

	g_GroupInfo[group].m_GroupFeature[RNET_USER_MENU_ID].m_FeatureID = RNET_USER_MENU_ID;
	g_GroupInfo[group].m_GroupFeature[RNET_USER_MENU_ID].m_Available = FALSE;
	g_GroupInfo[group].m_GroupFeature[RNET_USER_MENU_ID].m_Enabled = TRUE;
	g_GroupInfo[group].m_GroupFeature[RNET_USER_MENU_ID].m_Priority = RNET_USER_MENU_ID;

	g_GroupInfo[group].m_GroupFeature[RNET_SEATING_ID].m_FeatureID = RNET_SEATING_ID;
	g_GroupInfo[group].m_GroupFeature[RNET_SEATING_ID].m_Available = FALSE;
	g_GroupInfo[group].m_GroupFeature[RNET_SEATING_ID].m_Enabled = TRUE;
	g_GroupInfo[group].m_GroupFeature[RNET_SEATING_ID].m_Priority = RNET_USER_MENU_ID;

	g_GroupInfo[group].m_GroupFeature[RNET_SLEEP_FEATURE_ID].m_FeatureID = RNET_SLEEP_FEATURE_ID;
	g_GroupInfo[group].m_GroupFeature[RNET_SLEEP_FEATURE_ID].m_Available = FALSE;
	g_GroupInfo[group].m_GroupFeature[RNET_SLEEP_FEATURE_ID].m_Enabled = TRUE;
	g_GroupInfo[group].m_GroupFeature[RNET_SLEEP_FEATURE_ID].m_Priority = RNET_SLEEP_FEATURE_ID;

	g_GroupInfo[group].m_GroupFeature[AUDIBLE_OUT_FEATURE_ID].m_FeatureID = AUDIBLE_OUT_FEATURE_ID;
	g_GroupInfo[group].m_GroupFeature[AUDIBLE_OUT_FEATURE_ID].m_Available = TRUE;
	g_GroupInfo[group].m_GroupFeature[AUDIBLE_OUT_FEATURE_ID].m_Enabled = TRUE;
	g_GroupInfo[group].m_GroupFeature[AUDIBLE_OUT_FEATURE_ID].m_Priority = AUDIBLE_OUT_FEATURE_ID;

	g_GroupInfo[group].m_GroupFeature[SEATING_FEATURE_ID].m_FeatureID = SEATING_FEATURE_ID;
	g_GroupInfo[group].m_GroupFeature[SEATING_FEATURE_ID].m_Available = TRUE;
	g_GroupInfo[group].m_GroupFeature[SEATING_FEATURE_ID].m_Enabled = TRUE;
	g_GroupInfo[group].m_GroupFeature[SEATING_FEATURE_ID].m_Priority = SEATING_FEATURE_ID;

	g_GroupInfo[group].m_GroupFeature[TECLA_E_FEATURE_ID].m_FeatureID = TECLA_E_FEATURE_ID;
	g_GroupInfo[group].m_GroupFeature[TECLA_E_FEATURE_ID].m_Available = TRUE;
	g_GroupInfo[group].m_GroupFeature[TECLA_E_FEATURE_ID].m_Enabled = TRUE;
	g_GroupInfo[group].m_GroupFeature[TECLA_E_FEATURE_ID].m_Priority = TECLA_E_FEATURE_ID;
}

//*************************************************************************************

VOID InitializeGroupInformation (VOID)
{
	InitializeIndividualGroup (0);
	g_GroupInfo[0].m_DeviceType = DEVICE_TYPE_HEAD_ARRAY;
	g_GroupInfo[0].m_GroupFeature[BLUETOOTH_ID].m_Enabled = FALSE;

	InitializeIndividualGroup (1);
	g_GroupInfo[1].m_DeviceType = DEVICE_TYPE_HEAD_ARRAY;
	g_GroupInfo[1].m_GroupFeature[POWER_ONOFF_ID].m_Enabled = FALSE;
	g_GroupInfo[1].m_GroupFeature[NEXT_FUNCTION_ID].m_Enabled = FALSE;
	g_GroupInfo[1].m_GroupFeature[NEXT_PROFILE_ID].m_Enabled = FALSE;
	g_GroupInfo[1].m_GroupFeature[TECLA_E_FEATURE_ID].m_Enabled = FALSE;
	g_GroupInfo[1].m_GroupFeature[SEATING_FEATURE_ID].m_Enabled = FALSE;

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

