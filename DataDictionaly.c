//*****************************************************************************
// Filename: DataDictionry.c
// Description: This file describes the Data Dictionary and its accessor functions.
// Date: 2/25/21
// 
//*****************************************************************************

#include "ASL4321_System.h"
#include "DataDictionary.h"

//*****************************************************************************

static USHORT g_RNet_Active = TRUE;
static USHORT g_Group = 0;
static USHORT g_ActiveFeature;

//*****************************************************************************

USHORT dd_Get_USHORT (USHORT group, DD_ITEM_ID item_ID)
{
	USHORT myGroup;

	// This allows the caller to use the "active" group by sending in MAX_GROUPS in the group variable.
	myGroup = group;
	if (myGroup == MAX_GROUPS)
		myGroup = g_Group;

	switch (item_ID)
	{
	case DD_GROUP:				return (g_Group);
	case DD_DEVICE_TYPE:		return (g_GroupInfo[myGroup].m_DeviceType);
	case DD_RNET_ENABLE:		return g_RNet_Active;
	case DD_ACTIVE_FEATURE:		return g_ActiveFeature;
	case DD_SCAN_MODE:			return (g_GroupInfo[myGroup].m_ScanMode);
	}

	return 0;
}

//*****************************************************************************

VOID dd_Set_USHORT (USHORT group, DD_ITEM_ID item_ID, USHORT value)
{
	USHORT myGroup;

	// This allows the caller to use the "active" group by sending in MAX_GROUPS in the group variable.
	myGroup = group;
	if (myGroup == MAX_GROUPS)
		myGroup = g_Group;

	switch (item_ID)
	{
	case DD_GROUP:				g_Group = value; return;
	case DD_DEVICE_TYPE:		g_GroupInfo[myGroup].m_DeviceType = (DEVICE_TYPE_ENUM) value; return;
	case DD_RNET_ENABLE:		g_RNet_Active = value; return;
	case DD_ACTIVE_FEATURE:		g_ActiveFeature = value; return;
	case DD_SCAN_MODE:			g_GroupInfo[myGroup].m_ScanMode = (SCAN_MODE_ENUM) value; return;
	} // end switch
	
	return;
}

