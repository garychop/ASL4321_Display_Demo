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
typedef struct SOUND_STRUCTURE {
	char m_SoundLabel[SOUND_BITE_NAME_LENGTH];
	char m_FileName[SOUND_BITE_FILENAME_LENGTH];
} SOUND_STRUCT;
static SOUND_STRUCT g_Sound_MenuItems[MAX_SOUND_BITES];

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
	case DD_ACTIVE_SPEAKER_SUBITEM_FORWARD:		return (g_GroupInfo[myGroup].m_GroupFeature[AUDIBLE_OUT_FEATURE_ID].m_Audible_Subitem[subItem].m_ForwardPad_SoundID); break;
	case DD_ACTIVE_SPEAKER_SUBITEM_REVERSE:		return (g_GroupInfo[myGroup].m_GroupFeature[AUDIBLE_OUT_FEATURE_ID].m_Audible_Subitem[subItem].m_ReversePad_SoundID); break;
	case DD_ACTIVE_SPEAKER_SUBITEM_LEFT:		return (g_GroupInfo[myGroup].m_GroupFeature[AUDIBLE_OUT_FEATURE_ID].m_Audible_Subitem[subItem].m_LeftPad_SoundID); break;
	case DD_ACTIVE_SPEAKER_SUBITEM_RIGHT:		return (g_GroupInfo[myGroup].m_GroupFeature[AUDIBLE_OUT_FEATURE_ID].m_Audible_Subitem[subItem].m_RightPad_SoundID); break;
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
	case DD_ACTIVE_SPEAKER_SUBITEM_FORWARD:		g_GroupInfo[myGroup].m_GroupFeature[AUDIBLE_OUT_FEATURE_ID].m_Audible_Subitem[subItem].m_ForwardPad_SoundID = value; break;
	case DD_ACTIVE_SPEAKER_SUBITEM_REVERSE:		g_GroupInfo[myGroup].m_GroupFeature[AUDIBLE_OUT_FEATURE_ID].m_Audible_Subitem[subItem].m_ReversePad_SoundID = value; break;
	case DD_ACTIVE_SPEAKER_SUBITEM_LEFT:		g_GroupInfo[myGroup].m_GroupFeature[AUDIBLE_OUT_FEATURE_ID].m_Audible_Subitem[subItem].m_LeftPad_SoundID = value; break;
	case DD_ACTIVE_SPEAKER_SUBITEM_RIGHT:		g_GroupInfo[myGroup].m_GroupFeature[AUDIBLE_OUT_FEATURE_ID].m_Audible_Subitem[subItem].m_RightPad_SoundID = value; break;
	} // end switch item_ID
}

//*****************************************************************************
// STRING functions

USHORT dd_GetString (USHORT group, DD_ITEM_ID item_ID, USHORT subItem, char *returnStr)
{
	USHORT myGroup;
	USHORT err = 0;	// "0" = OK

	myGroup = NormalizeGroup (group);
	switch (item_ID)
	{
	case DD_SOUNDBITE_NAME:	strcpy_s (returnStr, SOUND_BITE_NAME_LENGTH, g_Sound_MenuItems[subItem].m_SoundLabel); break;
	case DD_SOUNDBITE_FILENAME:	strcpy_s (returnStr, SOUND_BITE_FILENAME_LENGTH, g_Sound_MenuItems[subItem].m_FileName); break;
	default:
		err = 1; // 1 = "Item not found" error.
		break;
	} // end switch item_ID

	return (err);	
}

USHORT dd_SetString (USHORT group, DD_ITEM_ID item_ID, USHORT subItem, char *saveStr)
{
	USHORT myGroup;
	USHORT err = 0;	// "0" = OK

	myGroup = NormalizeGroup (group);
	switch (item_ID)
	{
	case DD_SOUNDBITE_NAME:	strcpy_s (g_Sound_MenuItems[subItem].m_SoundLabel, SOUND_BITE_NAME_LENGTH, saveStr); break;
	case DD_SOUNDBITE_FILENAME:	strcpy_s (g_Sound_MenuItems[subItem].m_FileName, SOUND_BITE_FILENAME_LENGTH, saveStr); break;
	default:
		err = 1; // 1 = "Item not found" error.
		break;
	} // end switch item_ID

	return (err);	
}

//*************************************************************************************
VOID Populate_Sound_MenuItems(VOID)
{
	int i;

	// Preload defaults for sound bits
	for (i=0; i < MAX_SOUND_BITES; ++i)
	{
		sprintf_s (g_Sound_MenuItems[i].m_SoundLabel, SOUND_BITE_NAME_LENGTH, "<%d>", i+1);
		strcpy_s (g_Sound_MenuItems[i].m_FileName, sizeof (g_Sound_MenuItems[0].m_FileName), "N/A");
	}
	// Get specific sound bites, i.e. "YES"
	strcpy_s (g_Sound_MenuItems[0].m_SoundLabel, SOUND_BITE_NAME_LENGTH, "YES");
	strcpy_s (g_Sound_MenuItems[0].m_FileName, SOUND_BITE_FILENAME_LENGTH, "YES.wav");
	// "NO"
	strcpy_s (g_Sound_MenuItems[1].m_SoundLabel, SOUND_BITE_NAME_LENGTH, "NO");
	strcpy_s (g_Sound_MenuItems[1].m_FileName, SOUND_BITE_FILENAME_LENGTH, "NO.wav");
	// "HELLO"
	strcpy_s (g_Sound_MenuItems[2].m_SoundLabel, SOUND_BITE_NAME_LENGTH, "HELLO");
	strcpy_s (g_Sound_MenuItems[2].m_FileName, SOUND_BITE_FILENAME_LENGTH, "Hello.wav");
	// "HELP"
	strcpy_s (g_Sound_MenuItems[3].m_SoundLabel, SOUND_BITE_NAME_LENGTH, "HELP");
	strcpy_s (g_Sound_MenuItems[3].m_FileName, SOUND_BITE_FILENAME_LENGTH, "Help.wav");
	// "SLEEP"
	strcpy_s (g_Sound_MenuItems[4].m_SoundLabel, SOUND_BITE_NAME_LENGTH, "SLEEP");
	strcpy_s (g_Sound_MenuItems[4].m_FileName, SOUND_BITE_FILENAME_LENGTH, "Sleep.wav");
	// "BED"
	strcpy_s (g_Sound_MenuItems[5].m_SoundLabel, SOUND_BITE_NAME_LENGTH, "BED");
	strcpy_s (g_Sound_MenuItems[5].m_FileName, SOUND_BITE_FILENAME_LENGTH, "Bed.wav");
	// "FOOD"
	strcpy_s (g_Sound_MenuItems[6].m_SoundLabel, SOUND_BITE_NAME_LENGTH, "FOOD");
	strcpy_s (g_Sound_MenuItems[6].m_FileName, SOUND_BITE_FILENAME_LENGTH, "Food.wav");
	// "WATER"
	strcpy_s (g_Sound_MenuItems[7].m_SoundLabel, SOUND_BITE_NAME_LENGTH, "WATER");
	strcpy_s (g_Sound_MenuItems[7].m_FileName, SOUND_BITE_FILENAME_LENGTH, "Water.wav");
	// "BEER"
	strcpy_s (g_Sound_MenuItems[8].m_SoundLabel, SOUND_BITE_NAME_LENGTH, "BEER");
	strcpy_s (g_Sound_MenuItems[8].m_FileName, SOUND_BITE_FILENAME_LENGTH, "Beer.wav");
	// "PAIN"
	strcpy_s (g_Sound_MenuItems[9].m_SoundLabel, SOUND_BITE_NAME_LENGTH, "PAIN");
	strcpy_s (g_Sound_MenuItems[9].m_FileName, SOUND_BITE_FILENAME_LENGTH, "Pain.wav");
	// "ITCH"
	strcpy_s (g_Sound_MenuItems[10].m_SoundLabel, SOUND_BITE_NAME_LENGTH, "ITCH");
	strcpy_s (g_Sound_MenuItems[10].m_FileName, SOUND_BITE_FILENAME_LENGTH, "Itch.wav");

}

