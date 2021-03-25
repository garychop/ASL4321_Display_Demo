//*****************************************************************************
// Filename: ASL4321_System.h
// Description: This file declares function, typedefs and macros to the 
//		system wide usage.
//
// Author: G. Chopcinski, Kg Solutions, LLC
// 
//*****************************************************************************

#ifndef ASL165_SYSTEM_H
#define ASL165_SYSTEM_H

#include <stdio.h>

//#include "asl4321_display_demo_resources.h"
#include "asl4321_display_demo_specifications.h"
//#include "custom_checkbox.h"
#include "PadInfo.h"
//#include "FeatureHandling.h"

#define MAXIMUM_DRIVE_SPEED (40)
#define MAX_AUDIBLE_SUBITEMS (8)
#define MAX_SOUND_BITES (32)
#define SOUND_BITE_NAME_LENGTH (8)
#define SOUND_BITE_FILENAME_LENGTH (12)

#define FEATURE_TOGGLE_BUTTON_ID 1000	// this is used as the dynamically created buttons in the feature list.
typedef enum BUTTON_IDS {
	PAD_SETTINGS_BUTTON_ID = FEATURE_TOGGLE_BUTTON_ID,
	USER_SETTINGS_BUTTON_ID,
	FEATURE_LIST_BUTTON_ID,
	BLUETOOTH_SETUP_BUTTON_ID,
	SEATING_SETUP_BUTTON_ID,
	AUDIBLE_USAGE_BUTTON_ID,
	PERFORMANCE_MENU_BUTTON_ID,
	DIAGNOSTICS_MENU_BUTTON_ID,
	PAD_OPTIONS_SET_PAD_TYPE_BUTTON_ID,
	PAD_OPTIONS_SET_PAD_DIRECTION_BUTTON_ID,
	PAD_OPTIONS_SET_MINIMUM_SPEED_BUTTON_ID,
	PAD_OPTIONS_SET_NEUTRAL_WINDOW_BUTTON_ID,
	PAD_OPTIONS_SET_JOYSTICK_THROW_MAX_BUTTON_ID,
	SOUND1_BUTTON_ID,	// These are expected to be sequential
	SOUND2_BUTTON_ID,
	SOUND3_BUTTON_ID,
	SOUND4_BUTTON_ID,
	SOUND5_BUTTON_ID,
	SOUND6_BUTTON_ID,
	SOUND7_BUTTON_ID,
	SOUND8_BUTTON_ID,
	SOUND9_BUTTON_ID,
	SOUND10_BUTTON_ID,
	SOUND11_BUTTON_ID,
	SOUND12_BUTTON_ID,
	SOUND13_BUTTON_ID,
	SOUND14_BUTTON_ID,
	SOUND15_BUTTON_ID,
	SOUND16_BUTTON_ID,
	SOUND17_BUTTON_ID,
	SOUND18_BUTTON_ID,
	SOUND19_BUTTON_ID,
	SOUND20_BUTTON_ID,
	SOUND21_BUTTON_ID,
	SOUND22_BUTTON_ID,
	SOUND23_BUTTON_ID,
	SOUND24_BUTTON_ID,
	SOUND25_BUTTON_ID,
	SOUND26_BUTTON_ID,
	SOUND27_BUTTON_ID,
	SOUND28_BUTTON_ID,
	SOUND29_BUTTON_ID,
	SOUND30_BUTTON_ID,
	SOUND31_BUTTON_ID,
	SOUND32_BUTTON_ID,
	LAST_MENU_BUTTON_ID // make sure this one is last.
} BUTTON_IDS_ENUM;

typedef enum GROUP_E {GROUP_A, GROUP_B, GROUP_C, GROUP_D, MAX_GROUPS} GROUP_ENUM;
typedef enum ENUM_TIMER_IDS {ARROW_PUSHED_TIMER_ID = 1, CALIBRATION_TIMER_ID, PAD_ACTIVE_TIMER_ID, USER_PORT_PUSHED_TIMER_ID, SCAN_TIMER_ID} ENUM_TIMER_IDS_ENUM;
typedef enum ENUM_MODE_SWITCH_SCHEMA {MODE_SWITCH_PIN5, MODE_SWITCH_REVERSE} MODE_SWITCH_SCHEMA_ENUM;
typedef enum ENUM_DEVICE_TYPE {
	DEVICE_TYPE_HEAD_ARRAY,
	DEVICE_TYPE_PROPORTIONAL_JOYSTICK,
	DEVICE_TYPE_DIGITAL_JOYSTICK,
	DEVICE_TYPE_SINGLE_SWITCH,
	DEVICE_TYPE_TWO_SWITCH,
	END_OF_DEVICE_TYPES
} DEVICE_TYPE_ENUM;
typedef enum SCAN_DIRECTION {SCAN_FORWARD, SCAN_LEFT, SCAN_RIGHT, SCAN_REVERSE, SCAN_USER_PORT, SCAN_MAX} SCAN_DIRECTION_ENUM;
//typedef enum SCAN_DIRECTION {SCAN_FORWARD, SCAN_LEFT, SCAN_RIGHT, SCAN_REVERSE, SCAN_VEER_FORWARD_LEFT, SCAN_VEER_FORWARD_RIGHT, SCAN_USER_PORT, SCAN_MAX} SCAN_DIRECTION_ENUM;
typedef enum SCAN_MODE {SCAN_OFF, AUTO_SCAN, MANUAL_SCAN} SCAN_MODE_ENUM;

typedef enum FEATURE_ID {
	POWER_ONOFF_ID,
	BLUETOOTH_ID,
	NEXT_FUNCTION_ID,
	NEXT_PROFILE_ID,
	NEXT_GROUP_ID,
	RNET_TOGGLE_ID,
	RNET_USER_MENU_ID,
	RNET_SEATING_ID,
	RNET_SLEEP_FEATURE_ID,
	AUDIBLE_OUT_FEATURE_ID,
	SEATING_FEATURE_ID,
	TECLA_E_FEATURE_ID,
	MAX_FEATURES} FEATURE_ID_ENUM; // NUM_FEATURES must be last enum

// The following defines the bits used in the Feature Settings.
typedef enum FEATURE_ATTRIBUTE {
	FEATURE_AVAILABLE = 0x01,		// D0
	FEATURE_ENABLED = 0x02,			// D1
	FEATURE_PRIORITY_MASK = 0xF0	// Upper nibble is group feature priority/order
} FEATURE_ATTRIBUTE_ENUM;

//*****************************************************************************
// GLOBAL VARIABLES
//*****************************************************************************

extern int g_ChangeScreen_WIP;
extern unsigned char g_HA_Version_Major, g_HA_Version_Minor, g_HA_Version_Build, g_HA_EEPROM_Version;
extern int g_CalibrationPadNumber;
extern int g_ClicksActive;
extern int g_PowerUpInIdle;
extern int g_TimeoutValue;
extern MODE_SWITCH_SCHEMA_ENUM g_Mode_Switch_Schema;

extern int g_ActiveSeatingGroup;
extern int g_BluetoothGroup;
extern int g_TeclaGroup;

extern SCAN_DIRECTION_ENUM gActiveScanDirection;

//*****************************************************************************
// EXTERNAL, GLOBALLY available functions
//*****************************************************************************

VOID screen_toggle(GX_WINDOW *new_win, GX_WINDOW *old_win);
VOID screen_switch(GX_WIDGET *parent, GX_WIDGET *new_screen);

// Group Handling Function Prototypes
VOID InitializeGroupInformation (VOID);
VOID SelectNextGroup(VOID);
VOID SetGroupIcon (GX_ICON_BUTTON *icon_button);

VOID SetDeviceIcon (GX_ICON *icon);
VOID SelectNextDevice (VOID);
VOID AdvanceToNextTeclaGroup (VOID);
UINT GetTeclaGroup (VOID);

VOID AdvanceToNextFeature (VOID);
VOID AdvanceToPreviousFeature(VOID);

USHORT SelectNextAudioLevel (VOID);

VOID StartJoystickOperation (GX_RESOURCE_ID gx_widget_id);
VOID StopJoystickOperation (USHORT gx_widget_id);

#endif // ASL165_SYSTEM_H