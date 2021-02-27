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

VOID InitializeGroupInformation (VOID)
{
	g_GroupInfo[0].m_DeviceType = DEVICE_TYPE_HEAD_ARRAY;
	g_GroupInfo[0].m_PowerOnOff_Feature.m_Available = TRUE;
	g_GroupInfo[0].m_PowerOnOff_Feature.m_Enabled = TRUE;
	g_GroupInfo[0].m_NextFunction_Feature.m_Available = TRUE;
	g_GroupInfo[0].m_NextFunction_Feature.m_Enabled = TRUE;
	g_GroupInfo[0].m_NextProfile_Feature.m_Available = TRUE;
	g_GroupInfo[0].m_NextProfile_Feature.m_Enabled = TRUE;
	g_GroupInfo[0].m_RNet_UserMenu_Feature.m_Available = FALSE;
	g_GroupInfo[0].m_RNet_UserMenu_Feature.m_Enabled = TRUE;
	g_GroupInfo[0].m_RNet_Seating_Feature.m_Available = FALSE;
	g_GroupInfo[0].m_RNet_Seating_Feature.m_Enabled = TRUE;
	g_GroupInfo[0].m_RNet_Sleep_Feature.m_Available = FALSE;
	g_GroupInfo[0].m_RNet_Sleep_Feature.m_Enabled = TRUE;
	g_GroupInfo[0].m_MouseEmulation_Feature.m_Available = TRUE;
	g_GroupInfo[0].m_MouseEmulation_Feature.m_Enabled = TRUE;
	g_GroupInfo[0].m_Seating_Feature.m_Available = TRUE;
	g_GroupInfo[0].m_Seating_Feature.m_Enabled = TRUE;
	g_GroupInfo[0].m_TeclaE_Feature.m_Available = TRUE;
	g_GroupInfo[0].m_TeclaE_Feature.m_Enabled = TRUE;
	g_GroupInfo[0].m_Bluetooth_Feature.m_Available = TRUE;
	g_GroupInfo[0].m_Bluetooth_Feature.m_Enabled = TRUE;
	g_GroupInfo[0].m_NextGroup_Feature.m_Available = TRUE;
	g_GroupInfo[0].m_NextGroup_Feature.m_Enabled = TRUE;
	g_GroupInfo[0].m_Audible_Feature.m_Available = TRUE;
	g_GroupInfo[0].m_Audible_Feature.m_Enabled = TRUE;

	g_GroupInfo[1].m_DeviceType = DEVICE_TYPE_HEAD_ARRAY;
	g_GroupInfo[1].m_PowerOnOff_Feature.m_Available = TRUE;
	g_GroupInfo[1].m_PowerOnOff_Feature.m_Enabled = TRUE;
	g_GroupInfo[1].m_NextFunction_Feature.m_Available = TRUE;
	g_GroupInfo[1].m_NextFunction_Feature.m_Enabled = TRUE;
	g_GroupInfo[1].m_NextProfile_Feature.m_Available = TRUE;
	g_GroupInfo[1].m_NextProfile_Feature.m_Enabled = TRUE;
	g_GroupInfo[1].m_RNet_UserMenu_Feature.m_Available = FALSE;
	g_GroupInfo[1].m_RNet_UserMenu_Feature.m_Enabled = TRUE;
	g_GroupInfo[1].m_RNet_Seating_Feature.m_Available = FALSE;
	g_GroupInfo[1].m_RNet_Seating_Feature.m_Enabled = TRUE;
	g_GroupInfo[1].m_RNet_Sleep_Feature.m_Available = FALSE;
	g_GroupInfo[1].m_RNet_Sleep_Feature.m_Enabled = TRUE;
	g_GroupInfo[1].m_MouseEmulation_Feature.m_Available = TRUE;
	g_GroupInfo[1].m_MouseEmulation_Feature.m_Enabled = TRUE;
	g_GroupInfo[1].m_Seating_Feature.m_Available = TRUE;
	g_GroupInfo[1].m_Seating_Feature.m_Enabled = TRUE;
	g_GroupInfo[1].m_TeclaE_Feature.m_Available = TRUE;
	g_GroupInfo[1].m_TeclaE_Feature.m_Enabled = TRUE;
	g_GroupInfo[1].m_Bluetooth_Feature.m_Available = TRUE;
	g_GroupInfo[1].m_Bluetooth_Feature.m_Enabled = TRUE;
	g_GroupInfo[1].m_NextGroup_Feature.m_Available = TRUE;
	g_GroupInfo[1].m_NextGroup_Feature.m_Enabled = TRUE;
	g_GroupInfo[1].m_Audible_Feature.m_Available = TRUE;
	g_GroupInfo[1].m_Audible_Feature.m_Enabled = TRUE;

	g_GroupInfo[2].m_DeviceType = DEVICE_TYPE_PROPORTIONAL_JOYSTICK;
	g_GroupInfo[2].m_PowerOnOff_Feature.m_Available = TRUE;
	g_GroupInfo[2].m_PowerOnOff_Feature.m_Enabled = TRUE;
	g_GroupInfo[2].m_NextFunction_Feature.m_Available = TRUE;
	g_GroupInfo[2].m_NextFunction_Feature.m_Enabled = TRUE;
	g_GroupInfo[2].m_NextProfile_Feature.m_Available = TRUE;
	g_GroupInfo[2].m_NextProfile_Feature.m_Enabled = TRUE;
	g_GroupInfo[2].m_RNet_UserMenu_Feature.m_Available = FALSE;
	g_GroupInfo[2].m_RNet_UserMenu_Feature.m_Enabled = TRUE;
	g_GroupInfo[2].m_RNet_Seating_Feature.m_Available = FALSE;
	g_GroupInfo[2].m_RNet_Seating_Feature.m_Enabled = TRUE;
	g_GroupInfo[2].m_RNet_Sleep_Feature.m_Available = FALSE;
	g_GroupInfo[2].m_RNet_Sleep_Feature.m_Enabled = TRUE;
	g_GroupInfo[2].m_MouseEmulation_Feature.m_Available = TRUE;
	g_GroupInfo[2].m_MouseEmulation_Feature.m_Enabled = TRUE;
	g_GroupInfo[2].m_Seating_Feature.m_Available = TRUE;
	g_GroupInfo[2].m_Seating_Feature.m_Enabled = TRUE;
	g_GroupInfo[2].m_TeclaE_Feature.m_Available = TRUE;
	g_GroupInfo[2].m_TeclaE_Feature.m_Enabled = TRUE;
	g_GroupInfo[2].m_Bluetooth_Feature.m_Available = TRUE;
	g_GroupInfo[2].m_Bluetooth_Feature.m_Enabled = TRUE;
	g_GroupInfo[2].m_NextGroup_Feature.m_Available = TRUE;
	g_GroupInfo[2].m_NextGroup_Feature.m_Enabled = TRUE;
	g_GroupInfo[2].m_Audible_Feature.m_Available = TRUE;
	g_GroupInfo[2].m_Audible_Feature.m_Enabled = TRUE;

	g_GroupInfo[3].m_DeviceType = DEVICE_TYPE_DIGITAL_JOYSTICK;
	g_GroupInfo[3].m_PowerOnOff_Feature.m_Available = TRUE;
	g_GroupInfo[3].m_PowerOnOff_Feature.m_Enabled = TRUE;
	g_GroupInfo[3].m_NextFunction_Feature.m_Available = TRUE;
	g_GroupInfo[3].m_NextFunction_Feature.m_Enabled = TRUE;
	g_GroupInfo[3].m_NextProfile_Feature.m_Available = TRUE;
	g_GroupInfo[3].m_NextProfile_Feature.m_Enabled = TRUE;
	g_GroupInfo[3].m_RNet_UserMenu_Feature.m_Available = FALSE;
	g_GroupInfo[3].m_RNet_UserMenu_Feature.m_Enabled = TRUE;
	g_GroupInfo[3].m_RNet_Seating_Feature.m_Available = FALSE;
	g_GroupInfo[3].m_RNet_Seating_Feature.m_Enabled = TRUE;
	g_GroupInfo[3].m_RNet_Sleep_Feature.m_Available = FALSE;
	g_GroupInfo[3].m_RNet_Sleep_Feature.m_Enabled = TRUE;
	g_GroupInfo[3].m_MouseEmulation_Feature.m_Available = TRUE;
	g_GroupInfo[3].m_MouseEmulation_Feature.m_Enabled = TRUE;
	g_GroupInfo[3].m_Seating_Feature.m_Available = TRUE;
	g_GroupInfo[3].m_Seating_Feature.m_Enabled = TRUE;
	g_GroupInfo[3].m_TeclaE_Feature.m_Available = TRUE;
	g_GroupInfo[3].m_TeclaE_Feature.m_Enabled = TRUE;
	g_GroupInfo[3].m_Bluetooth_Feature.m_Available = TRUE;
	g_GroupInfo[3].m_Bluetooth_Feature.m_Enabled = TRUE;
	g_GroupInfo[3].m_NextGroup_Feature.m_Available = TRUE;
	g_GroupInfo[3].m_NextGroup_Feature.m_Enabled = TRUE;
	g_GroupInfo[3].m_Audible_Feature.m_Available = TRUE;
	g_GroupInfo[3].m_Audible_Feature.m_Enabled = TRUE;

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

