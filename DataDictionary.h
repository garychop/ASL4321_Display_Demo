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
	DD_SCAN_MODE,
} DD_ITEM_ID;

//*****************************************************************************
// Globally used data, singular about the system.
//*****************************************************************************

USHORT dd_Get_USHORT (USHORT, DD_ITEM_ID);
VOID dd_Set_USHORT (USHORT, DD_ITEM_ID, USHORT value);


