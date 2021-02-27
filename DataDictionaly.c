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

//*****************************************************************************

USHORT dd_Get_USHORT (USHORT group, DD_ITEM_ID item_ID)
{
	switch (item_ID)
	{
	case DD_GROUP:						return (g_Group);
	case DD_DEVICE_TYPE:				return (g_GroupInfo[group].m_DeviceType);
	case DD_RNET_ENABLE:				return g_RNet_Active;
	case DD_POWER_ONOFF_AVAILABLE:		return (g_GroupInfo[group].m_PowerOnOff_Feature.m_Available);
	case DD_POWER_ONOFF_ENABLE:			return (g_GroupInfo[group].m_PowerOnOff_Feature.m_Enabled);
	case DD_NEXT_FUNCTION_AVAILABLE:	return (g_GroupInfo[group].m_NextFunction_Feature.m_Available);
	case DD_NEXT_FUNCTION_ENABLE:		return (g_GroupInfo[group].m_NextFunction_Feature.m_Enabled);
	case DD_NEXT_PROFILE_AVAILABLE:		return (g_GroupInfo[group].m_NextProfile_Feature.m_Available);
	case DD_NEXT_PROFILE_ENABLE:		return (g_GroupInfo[group].m_NextProfile_Feature.m_Enabled);
	case DD_BLUETOOTH_AVAILABLE:		return (g_GroupInfo[group].m_Bluetooth_Feature.m_Available);
	case DD_BLUETOOTH_ENABLE:			return (g_GroupInfo[group].m_Bluetooth_Feature.m_Enabled);
	case DD_RNET_USER_MENU_AVAILABLE:	return (g_GroupInfo[group].m_RNet_UserMenu_Feature.m_Available);
	case DD_RNET_USER_MENU_ENABLE:		return (g_GroupInfo[group].m_RNet_UserMenu_Feature.m_Enabled);
	case DD_RNET_TOGGLE_AVAILABLE:		return (g_GroupInfo[group].m_RNet_Toggle_Feature.m_Available);
	case DD_RNET_TOGGLE_ENABLE:			return (g_GroupInfo[group].m_RNet_Toggle_Feature.m_Enabled);
	case DD_RNET_SEATING_AVAILABLE:		return (g_GroupInfo[group].m_RNet_Seating_Feature.m_Available);
	case DD_RNET_SEATING_ENABLE:		return (g_GroupInfo[group].m_RNet_Seating_Feature.m_Enabled);
	case DD_RNET_SLEEP_AVAILABLE:		return (g_GroupInfo[group].m_RNet_Sleep_Feature.m_Available);
	case DD_RNET_SLEEP_ENABLE:			return (g_GroupInfo[group].m_RNet_Sleep_Feature.m_Enabled);
	case DD_NEXT_GROUP_AVAILABLE:		return (g_GroupInfo[group].m_NextGroup_Feature.m_Available);
	case DD_NEXT_GROUP_ENABLE:			return (g_GroupInfo[group].m_NextGroup_Feature.m_Enabled);
	case DD_AUDIBLE_OUT_AVAILABLE:		return (g_GroupInfo[group].m_Audible_Feature.m_Available);
	case DD_AUDIBLE_OUT_ENABLE:			return (g_GroupInfo[group].m_NextGroup_Feature.m_Enabled);
	case DD_SEATING_AVAILABLE:			return (g_GroupInfo[group].m_Seating_Feature.m_Available);
	case DD_SEATING_ENABLE:				return (g_GroupInfo[group].m_Seating_Feature.m_Enabled);
	case DD_TECLA_E_AVAILABLE:			return (g_GroupInfo[group].m_TeclaE_Feature.m_Available);
	case DD_TECLA_E_ENABLE:				return (g_GroupInfo[group].m_TeclaE_Feature.m_Enabled);
	}

	return 0;
}

//*****************************************************************************

VOID dd_Set_USHORT (USHORT group, DD_ITEM_ID item_ID, USHORT value)
{
	switch (item_ID)
	{
		case DD_GROUP:	g_Group = value; return;
	} // end switch
	
	return;
}

