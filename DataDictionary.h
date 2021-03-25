//*****************************************************************************
// Filename: DataDictionry.h
// Description: This file describes the Data Dictionary and its accessor functions.
// Date: 2/24/21
// 
//*****************************************************************************

#include "ASL4321_System.h"

typedef enum DATA_DICTIONARY_IDS_ENUM {
	DD_TEST_ID = 0,
	DD_GROUP,
	DD_DEVICE_TYPE,
	DD_RNET_ENABLE,
	DD_ACTIVE_FEATURE,
	DD_ACTIVE_FEATURE_SUBITEM,
	DD_ACTIVE_SPEAKER_SUBITEM_FORWARD,
	DD_ACTIVE_SPEAKER_SUBITEM_REVERSE,
	DD_ACTIVE_SPEAKER_SUBITEM_LEFT,
	DD_ACTIVE_SPEAKER_SUBITEM_RIGHT,
	DD_SOUNDBITE_NAME,
	DD_SOUNDBITE_FILENAME,
	DD_SCAN_MODE,
	DD_GROUP_FEATURE_ID,		// Requires subscript parameter
	DD_GROUP_FEATURE_ATTRIBUTE,	// Requires subscript parameter
	DD_PAD_MINIMUM_DRIVE_SPEED,	// Requires subscript parameter
	DD_PAD_DIRECTION			// Requires subscript parameter
} DD_ITEM_ID;

//*****************************************************************************
// Globally used data, singular about the system.
//*****************************************************************************

USHORT dd_Get_USHORT (USHORT, DD_ITEM_ID);
VOID dd_Set_USHORT (USHORT, DD_ITEM_ID, USHORT value);
USHORT dd_GetSubItem_USHORT (USHORT group, DD_ITEM_ID item_ID, USHORT subItem);
VOID dd_SetSubItem_USHORT (USHORT group, DD_ITEM_ID item_ID, USHORT subItem, USHORT value);
USHORT dd_GetString (USHORT group, DD_ITEM_ID item_ID, USHORT subItem, char *returnStr);
USHORT dd_SetString (USHORT group, DD_ITEM_ID item_ID, USHORT subItem, char *returnStr);


