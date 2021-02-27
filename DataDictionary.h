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
	DD_POWER_ONOFF_AVAILABLE,
	DD_POWER_ONOFF_ENABLE,	// Requires Group #
	DD_NEXT_FUNCTION_AVAILABLE,
	DD_NEXT_FUNCTION_ENABLE,
	DD_NEXT_PROFILE_AVAILABLE,
	DD_NEXT_PROFILE_ENABLE,
	DD_BLUETOOTH_AVAILABLE,
	DD_BLUETOOTH_ENABLE,
	DD_RNET_USER_MENU_AVAILABLE,
	DD_RNET_USER_MENU_ENABLE,
	DD_RNET_TOGGLE_AVAILABLE,
	DD_RNET_TOGGLE_ENABLE,
	DD_RNET_SEATING_AVAILABLE,
	DD_RNET_SEATING_ENABLE,
	DD_RNET_SLEEP_AVAILABLE,
	DD_RNET_SLEEP_ENABLE,
	DD_NEXT_GROUP_AVAILABLE,
	DD_NEXT_GROUP_ENABLE,
	DD_AUDIBLE_OUT_AVAILABLE,
	DD_AUDIBLE_OUT_ENABLE,
	DD_SEATING_AVAILABLE,
	DD_SEATING_ENABLE,
	DD_TECLA_E_AVAILABLE,
	DD_TECLA_E_ENABLE,
} DD_ITEM_ID;

//*****************************************************************************
// Globally used data, singular about the system.
//*****************************************************************************

USHORT dd_Get_USHORT (USHORT, DD_ITEM_ID);
VOID dd_Set_USHORT (USHORT, DD_ITEM_ID, USHORT value);


