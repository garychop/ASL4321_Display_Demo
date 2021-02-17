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

//*************************************************************************************

GROUP_STRUCT g_GroupInfo[MAX_GROUPS];

//*************************************************************************************

VOID InitializeGroupInformation (VOID)
{
	g_GroupInfo[0].m_DeviceType = DEVICE_TYPE_HEAD_ARRAY;

	g_GroupInfo[1].m_DeviceType = DEVICE_TYPE_HEAD_ARRAY;

	g_GroupInfo[2].m_DeviceType = DEVICE_TYPE_PROPORTIONAL_JOYSTICK;

	g_GroupInfo[3].m_DeviceType = DEVICE_TYPE_DIGITAL_JOYSTICK;

}

//*************************************************************************************
// Goes the next group, adjusts global var and returns the active group
//******************************************************************************************

VOID SelectNextGroup(VOID)
{
	if (++g_ActiveGroup >= MAX_GROUPS)
		g_ActiveGroup = 0;
}

//******************************************************************************************
UINT GetActiveGroup (VOID)
{
	return g_ActiveGroup;
}

DEVICE_TYPE_ENUM GetGroupDeviceType (VOID)
{
	return (g_GroupInfo[g_ActiveGroup].m_DeviceType);
}