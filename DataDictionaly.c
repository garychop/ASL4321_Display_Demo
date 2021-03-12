//*****************************************************************************
// Filename: DataDictionry.c
// Description: This file describes the Data Dictionary and its accessor functions.
// Date: 2/25/21
// 
//*****************************************************************************

#include "ASL4321_System.h"
#include "DataDictionary.h"
#include "HUB.h"

//*****************************************************************************

static USHORT g_RNet_Active = TRUE;
static USHORT g_Group = 0;
static USHORT g_ActiveFeature = 0;
static USHORT g_ActiveSubitem = 0;

//*****************************************************************************
// This allows the caller to use the "active" group by sending in MAX_GROUPS in the group variable.

static USHORT NormalizeGroup (USHORT grp)
{
	if (grp == MAX_GROUPS)
		return g_Group;
	else
		return grp;
}

//*****************************************************************************

USHORT dd_Get_USHORT (USHORT group, DD_ITEM_ID item_ID)
{
	USHORT myGroup;

	myGroup = NormalizeGroup (group);	// This allows the caller to send in a specific group or to use the active group

	switch (item_ID)
	{
	case DD_GROUP:				return (g_Group);
	case DD_DEVICE_TYPE:		return (g_GroupInfo[myGroup].m_DeviceType);
	case DD_RNET_ENABLE:		return g_RNet_Active;
	case DD_ACTIVE_FEATURE:		return g_ActiveFeature;
	case DD_ACTIVE_FEATURE_SUBITEM: return g_ActiveSubitem;
	case DD_SCAN_MODE:			return (g_GroupInfo[myGroup].m_ScanMode);
	}

	return 0;
}

//*****************************************************************************

VOID dd_Set_USHORT (USHORT group, DD_ITEM_ID item_ID, USHORT value)
{
	USHORT myGroup;

	myGroup = NormalizeGroup (group);	// This allows the caller to send in a specific group or to use the active group

	switch (item_ID)
	{
	case DD_GROUP:				g_Group = value; return;
	case DD_DEVICE_TYPE:		g_GroupInfo[myGroup].m_DeviceType = (DEVICE_TYPE_ENUM) value; return;
	case DD_RNET_ENABLE:		g_RNet_Active = value; return;
	case DD_ACTIVE_FEATURE:		g_ActiveFeature = value; return;
	case DD_ACTIVE_FEATURE_SUBITEM: g_ActiveSubitem = value; return;
	case DD_SCAN_MODE:			g_GroupInfo[myGroup].m_ScanMode = (SCAN_MODE_ENUM) value; return;
	} // end switch
	
	return;
}

//*****************************************************************************

USHORT dd_GetSubItem_USHORT (USHORT group, DD_ITEM_ID item_ID, USHORT subItem)
{
	USHORT myGroup;

	myGroup = NormalizeGroup (group);
	switch (item_ID)
	{
	case DD_GROUP_FEATURE_ATTRIBUTE:	return (g_GroupInfo[myGroup].m_GroupFeature[subItem].m_Attribute); break;
	case DD_GROUP_FEATURE_ID:			return (g_GroupInfo[myGroup].m_GroupFeature[subItem].m_FeatureID); break;
	case DD_ACTIVE_FEATURE_SUBITEM:		return (g_GroupInfo[myGroup].m_GroupFeature[subItem].m_SelectedSubItem); break;
	case DD_PAD_MINIMUM_DRIVE_SPEED:	return (g_GroupInfo[myGroup].m_GroupPadInfo[subItem].m_MinimumDriveValue); break;
	case DD_PAD_DIRECTION:				return (g_GroupInfo[myGroup].m_GroupPadInfo[subItem].m_PadDirection); break;
	case DD_ACTIVE_SPEAKER_SUBITEM_FORWARD:		return (g_GroupInfo[myGroup].m_GroupFeature[AUDIBLE_OUT_FEATURE_ID].m_Subitem.m_Audible_Subitem[subItem].m_ForwardPad_SoundID); break;
	case DD_ACTIVE_SPEAKER_SUBITEM_REVERSE:		return (g_GroupInfo[myGroup].m_GroupFeature[AUDIBLE_OUT_FEATURE_ID].m_Subitem.m_Audible_Subitem[subItem].m_ReversePad_SoundID); break;
	case DD_ACTIVE_SPEAKER_SUBITEM_LEFT:		return (g_GroupInfo[myGroup].m_GroupFeature[AUDIBLE_OUT_FEATURE_ID].m_Subitem.m_Audible_Subitem[subItem].m_LeftPad_SoundID); break;
	case DD_ACTIVE_SPEAKER_SUBITEM_RIGHT:		return (g_GroupInfo[myGroup].m_GroupFeature[AUDIBLE_OUT_FEATURE_ID].m_Subitem.m_Audible_Subitem[subItem].m_RightPad_SoundID); break;
	} // end switch item_ID

	return 0;
}

//*****************************************************************************

VOID dd_SetSubItem_USHORT (USHORT group, DD_ITEM_ID item_ID, USHORT subItem, USHORT value)
{
	USHORT myGroup;

	myGroup = NormalizeGroup (group);
	switch (item_ID)
	{
	case DD_GROUP_FEATURE_ATTRIBUTE:	g_GroupInfo[myGroup].m_GroupFeature[subItem].m_Attribute = value; break;
	case DD_GROUP_FEATURE_ID:			g_GroupInfo[myGroup].m_GroupFeature[subItem].m_FeatureID = (FEATURE_ID_ENUM) value; break;
	case DD_ACTIVE_FEATURE_SUBITEM:		g_GroupInfo[myGroup].m_GroupFeature[subItem].m_SelectedSubItem = value; break;
	case DD_PAD_MINIMUM_DRIVE_SPEED:	g_GroupInfo[myGroup].m_GroupPadInfo[subItem].m_MinimumDriveValue = value; break;
	case DD_PAD_DIRECTION:				g_GroupInfo[myGroup].m_GroupPadInfo[subItem].m_PadDirection = (PAD_DIRECTION_ENUM) value; break;
	case DD_ACTIVE_SPEAKER_SUBITEM_FORWARD:		g_GroupInfo[myGroup].m_GroupFeature[AUDIBLE_OUT_FEATURE_ID].m_Subitem.m_Audible_Subitem[subItem].m_ForwardPad_SoundID = value; break;
	case DD_ACTIVE_SPEAKER_SUBITEM_REVERSE:		g_GroupInfo[myGroup].m_GroupFeature[AUDIBLE_OUT_FEATURE_ID].m_Subitem.m_Audible_Subitem[subItem].m_ReversePad_SoundID = value; break;
	case DD_ACTIVE_SPEAKER_SUBITEM_LEFT:		g_GroupInfo[myGroup].m_GroupFeature[AUDIBLE_OUT_FEATURE_ID].m_Subitem.m_Audible_Subitem[subItem].m_LeftPad_SoundID = value; break;
	case DD_ACTIVE_SPEAKER_SUBITEM_RIGHT:		g_GroupInfo[myGroup].m_GroupFeature[AUDIBLE_OUT_FEATURE_ID].m_Subitem.m_Audible_Subitem[subItem].m_RightPad_SoundID = value; break;
	} // end switch item_ID
}

