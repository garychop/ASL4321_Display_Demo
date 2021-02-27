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

#include "asl4321_display_demo_resources.h"
#include "asl4321_display_demo_specifications.h"
#include "custom_checkbox.h"
#include "PadInfo.h"

#define MAXIMUM_DRIVE_SPEED (40)

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
	LAST_MENU_BUTTON_ID // make sure this one is last.
} BUTTON_IDS_ENUM;

typedef enum FEATURE_ID {
	POWER_ONOFF_ID,
	BLUETOOTH_ID,
	NEXT_FUNCTION_ID,
	NEXT_PROFILE_ID,
	RNET_TOGGLE_ID,
	RNET_USER_MENU_ID,
	RNET_SEATING_ID,
	RNET_SLEEP_FEATURE_ID,
	NEXT_GROUP_ID,
	AUDIBLE_OUT_FEATURE_ID,
	SEATING_FEATURE_ID,
	TECLA_E_FEATURE_ID,
	NUM_FEATURES} FEATURE_ID_ENUM; // NUM_FEATURES must be last enum

typedef enum GROUP_E {GROUP_A, GROUP_B, GROUP_C, GROUP_D, MAX_GROUPS} GROUP_ENUM;
typedef enum ENUM_TIMER_IDS {ARROW_PUSHED_TIMER_ID = 1, CALIBRATION_TIMER_ID, PAD_ACTIVE_TIMER_ID, USER_PORT_PUSHED_TIMER_ID, SCAN_TIMER_ID} ENUM_TIMER_IDS_ENUM;
typedef enum ENUM_MODE_SWITCH_SCHEMA {MODE_SWITCH_PIN5, MODE_SWITCH_REVERSE} MODE_SWITCH_SCHEMA_ENUM;
typedef enum ENUM_DEVICE_TYPE {DEVICE_TYPE_HEAD_ARRAY, DEVICE_TYPE_PROPORTIONAL_JOYSTICK, DEVICE_TYPE_DIGITAL_JOYSTICK, END_OF_DEVICE_TYPES} DEVICE_TYPE_ENUM;
typedef enum SCAN_DIRECTION {SCAN_FORWARD, SCAN_LEFT, SCAN_RIGHT, SCAN_REVERSE, SCAN_VEER_FORWARD_LEFT, SCAN_VEER_FORWARD_RIGHT, SCAN_USER_PORT, SCAN_MAX} SCAN_DIRECTION_ENUM;

typedef struct CUSTOM_MENU_BUTTON_STRUCT{
//    GX_BUTTON_MEMBERS_DECLARE
    GX_WIDGET m_MenuWidget;
	GX_PROMPT m_PromptWidget;
	GX_TEXT_BUTTON m_ButtonWidget;
	USHORT m_ButtonID;
	GX_RESOURCE_ID m_TextID;
	USHORT m_Enabled;
	//INT start_offset;
 //   INT end_offset;
 //   INT cur_offset;
} CUSTOM_MENU_BUTTON;

typedef struct FEATURE_STRUCTURE {
	USHORT m_Available;		// This is TRUE if this feature is available for enabling.
	USHORT m_Enabled;
	USHORT m_Order;
	union FEATURE_SUBSET {
		int myA;
		int myB;
	} m_Subset;
} FEATURE_STRUCT;

typedef struct //GROUP_STRUCT_DEFINE
{
	DEVICE_TYPE_ENUM m_DeviceType;
	PAD_INFO_STRUCT m_GroupPadInfo[MAX_PHYSICAL_PADS];
	FEATURE_STRUCT m_PowerOnOff_Feature;
	FEATURE_STRUCT m_NextFunction_Feature;
	FEATURE_STRUCT m_NextProfile_Feature;
	FEATURE_STRUCT m_RNet_Toggle_Feature;
	FEATURE_STRUCT m_RNet_UserMenu_Feature;
	FEATURE_STRUCT m_RNet_Seating_Feature;
	FEATURE_STRUCT m_RNet_Sleep_Feature;
	FEATURE_STRUCT m_Bluetooth_Feature;
	FEATURE_STRUCT m_MouseEmulation_Feature;
	FEATURE_STRUCT m_Seating_Feature;
	FEATURE_STRUCT m_Audible_Feature;
	FEATURE_STRUCT m_NextGroup_Feature;
	FEATURE_STRUCT m_TeclaE_Feature;
} GROUP_STRUCT;
extern GROUP_STRUCT g_GroupInfo[MAX_GROUPS];

typedef struct MAIN_SCREEN_FEATURE_STRUCT
{
    int m_Location;     // This indicates the Main Screen location, 0=Top most, 3=bottom most
	int m_Available;	// This is true if this feature should be displayed for Enabling/Disabling. Typically based upon RNet setting.
    int m_Enabled;      // Indicates if this feature is active.
    GX_RESOURCE_ID m_SmallIcon;
    GX_RESOURCE_ID m_LargeIcon;
    GX_RESOURCE_ID m_SmallDescriptionID;
    GX_RESOURCE_ID m_LargeDescriptionID;
    GX_RESOURCE_ID m_FeatureIconID;
	GX_WIDGET m_ItemWidget;
	GX_PROMPT m_PromptWidget;
	GX_CHECKBOX m_ButtonWidget;
    CUSTOM_CHECKBOX m_Checkbox;
} MAIN_SCREEN_FEATURE;

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

extern MAIN_SCREEN_FEATURE g_MainScreenFeatureInfo[];
extern FEATURE_ID_ENUM g_ActiveFeature;
extern int g_ActiveSpeakerGroup;
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

#endif // ASL165_SYSTEM_H