//***********************************************************************************
// Project: ASL110 Display Demo package
//
// Filename: ASL110_Display_Simulator_Main.c
//
// Description: This project is a Windows based project that is used to convey the
//		ASL110 Display Unit.
//
// Author: G. Chopcinski
//
// Creation Date: 7/18/19
//
//***********************************************************************************

#define GX_ARC_DRAWING_SUPPORT

#include "time.h"			// Windows supported timed functions.
#include "ASL4321_Simulator_Main.h"
#include "ASL4321_System.h"
#include "DataDictionary.h"
#include "UserMainScreen.h"
#include "PadInfo.h"
#include "HUB.h"
#include "FeatureHandling.h"

#define SCREEN_STACK_SIZE  5
#define SCRATCHPAD_PIXELS  (PRIMARYDISPLAY_X_RESOLUTION * PRIMARYDISPLAY_Y_RESOLUTION * 2)

//--------------------------------------------------------------------
// Define prototypes.
VOID start_guix(VOID);

//--------------------------------------------------------------------
// Global Variables
TX_BYTE_POOL       memory_pool;
GX_COLOR           scratchpad[SCRATCHPAD_PIXELS];

GX_RECTANGLE g_HiddenRectangle = {0,0,0,0};

//GX_RECTANGLE g_FeatureLocation[] = {
//	{10, 16, 300, 86},
//	{30, 94, 290, 130},
//	{30, 130, 290, 162},
//	{30, 166, 290, 198},
//	{0,0,0,0}};

int g_ChangeScreen_WIP;

GX_WINDOW *g_CalibrationScreen = GX_NULL;
//extern GX_RECTANGLE g_CalibrationPadLocations[];
extern GX_RECTANGLE g_CalibrationPromptLocations[];

int g_CalibrationPadNumber;
int g_CalibrationStepNumber;
int g_PadValue = 35;
int g_DeltaValue;

int g_ClicksActive = FALSE;
int g_PowerUpInIdle = FALSE;
int g_TimeoutValue = 20;
MODE_SWITCH_SCHEMA_ENUM g_Mode_Switch_Schema = MODE_SWITCH_PIN5;

int g_ActiveSeatingGroup = 0;
int g_BluetoothGroup = 0;

//*************************************************************************************
// GLOBAL VARIABLES
//*************************************************************************************

unsigned char g_HA_Version_Major, g_HA_Version_Minor, g_HA_Version_Build, g_HA_EEPROM_Version = 5;
extern VOID Initialize_MainScreenInfo();
char g_StatusMessage[64];

VOID ProcessDrivingFeature (GX_RESOURCE_ID gx_widget_id);

//*************************************************************************************
// Provided by GUIX to support Windows environment.
//*************************************************************************************

VOID *memory_allocate(ULONG size)
{
    VOID *memptr;

    if (tx_byte_allocate(&memory_pool, &memptr, size, TX_NO_WAIT) == TX_SUCCESS)
    {
        return memptr;
    }
    return NULL;
}

//*************************************************************************************
// Provided by GUIX to support Windows environment.
//*************************************************************************************
void memory_free(VOID *mem)
{
    tx_byte_release(mem);
}

//*************************************************************************************
// Provided by GUIX to support Windows environment.
//*************************************************************************************
int main(int argc, char ** argv)
{
    tx_kernel_enter();
    return(0);
}

//*************************************************************************************
// Provided by GUIX to support Windows environment.
//*************************************************************************************
VOID tx_application_define(void *first_unused_memory)
{
    /* create byte pool*/
    tx_byte_pool_create(&memory_pool, "scratchpad", scratchpad,
        SCRATCHPAD_PIXELS * sizeof(GX_COLOR));

    start_guix();
}

//*************************************************************************************
// Provided by GUIX to support Windows environment.
//*************************************************************************************
VOID  start_guix(VOID)
{
	UINT myError;

    /* Initialize GUIX. */
    gx_system_initialize();

    /* Assign memory alloc/free functions. */
    gx_system_memory_allocator_set(memory_allocate, memory_free);

	myError = -1;
    myError = gx_studio_display_configure(PRIMARYDISPLAY, win32_graphics_driver_setup_24xrgb, 
        LANGUAGE_ENGLISH, PRIMARYDISPLAY_THEME_1, &root);

	InitializeGroupInformation();

	dd_Set_USHORT (0, DD_ACTIVE_FEATURE, 0);

	//Initialize_MainScreenInfo();

	//InitializeFeature_GUI_Info();

	InitializePadInformation();

    //myError = gx_studio_named_widget_create("PrimaryTemplate", (GX_WIDGET *)GX_NULL, GX_NULL);
	myError = gx_studio_named_widget_create("DiagnosticScreen", GX_NULL, GX_NULL);
	myError = gx_studio_named_widget_create("FeatureSettingsScreen", GX_NULL, GX_NULL);
	myError = gx_studio_named_widget_create("HHP_Start_Screen", GX_NULL, GX_NULL);
	myError = gx_studio_named_widget_create("JoystickThrowScreen", GX_NULL, GX_NULL);
	//myError = gx_studio_named_widget_create("MainUserScreen", , GX_NULL, GX_NULL);
	myError = gx_studio_named_widget_create("MainUserScreen", (GX_WIDGET *)root, GX_NULL);
	myError = gx_studio_named_widget_create("MainUserScreen_3", (GX_WIDGET *)GX_NULL, GX_NULL);
	myError = gx_studio_named_widget_create("ManageSoundScreen", GX_NULL, GX_NULL);
	myError = gx_studio_named_widget_create("MinimumDriveScreen", GX_NULL, GX_NULL);
	myError = gx_studio_named_widget_create("NeutralWindowScreen", GX_NULL, GX_NULL);
	myError = gx_studio_named_widget_create("MoreSelectionScreen", GX_NULL, GX_NULL);
	myError = gx_studio_named_widget_create("OON_Screen", GX_NULL, GX_NULL);
	myError = gx_studio_named_widget_create("PerformanceSelectionScreen", GX_NULL, GX_NULL);
	myError = gx_studio_named_widget_create("PadAdvancedScreen", GX_NULL, GX_NULL);
	myError = gx_studio_named_widget_create("PadCalibrationScreen", GX_NULL, GX_NULL);
	myError = gx_studio_named_widget_create("PadOptionsSettingsScreen", GX_NULL, GX_NULL);
	myError = gx_studio_named_widget_create("ReadyScreen", GX_NULL, GX_NULL);
	myError = gx_studio_named_widget_create("ResetScreen", GX_NULL, GX_NULL);
	myError = gx_studio_named_widget_create("ResetFinishScreen", GX_NULL, GX_NULL);
	myError = gx_studio_named_widget_create("StartupScreen", GX_NULL, GX_NULL);
	myError = gx_studio_named_widget_create("SetPadDirectionScreen", GX_NULL, GX_NULL);
	myError = gx_studio_named_widget_create("SetPadTypeScreen", GX_NULL, GX_NULL);
	myError = gx_studio_named_widget_create("SoundOptionScreen", GX_NULL, GX_NULL);
	myError = gx_studio_named_widget_create("SoundSetupScreen", GX_NULL, GX_NULL);
	myError = gx_studio_named_widget_create("UserScanScreen", GX_NULL, GX_NULL);
	myError = gx_studio_named_widget_create("UserSettingsScreen", GX_NULL, GX_NULL);
	myError = gx_studio_named_widget_create("UserSelectionScreen", GX_NULL, GX_NULL);
	myError = gx_studio_named_widget_create("VeerAdjustScreen", GX_NULL, GX_NULL);

	/* Show the root window.  */
    gx_widget_show(root);

    /* start GUIX thread */
    gx_system_start();
}

//******************************************************************************************
// Detach one window and attach anothe window to root.
//******************************************************************************************

VOID screen_toggle(GX_WINDOW *new_win, GX_WINDOW *old_win)
{
	int status;
    if (!new_win->gx_widget_parent)
    {
        status = gx_widget_attach(root, (GX_WIDGET *)new_win);
    }
    else
    {
        status = gx_widget_show((GX_WIDGET *)new_win);
    }
    status = gx_widget_detach((GX_WIDGET *)old_win);
}

//*************************************************************************************

VOID SetGroupIcon (GX_ICON_BUTTON *icon_button)
{
	switch (dd_Get_USHORT (0, DD_GROUP))
	{
	case 0:
		gx_icon_button_pixelmap_set (icon_button, GX_PIXELMAP_ID_GROUPA_64X64);
		break;
	case 1:
		gx_icon_button_pixelmap_set (icon_button, GX_PIXELMAP_ID_GROUPB_64X64);
		break;
	case 2:
		gx_icon_button_pixelmap_set (icon_button, GX_PIXELMAP_ID_GROUPC_64X64);
		break;
	case 3:
		gx_icon_button_pixelmap_set (icon_button, GX_PIXELMAP_ID_GROUPD_64X64);
		break;
	default:
		break;
	} // end of switch
}

//******************************************************************************************

VOID SetDeviceIcon (GX_ICON *icon)
{
	switch (dd_Get_USHORT (dd_Get_USHORT (0, DD_GROUP), DD_DEVICE_TYPE))
	{
	case DEVICE_TYPE_HEAD_ARRAY:
		gx_icon_pixelmap_set (icon, GX_PIXELMAP_ID_DEVICETYPE_HEADARRAY_88X70, GX_PIXELMAP_ID_DEVICETYPE_HEADARRAY_88X70);
		break;
	case DEVICE_TYPE_PROPORTIONAL_JOYSTICK:
		gx_icon_pixelmap_set (icon, GX_PIXELMAP_ID_DEVICETYPE_JOYSTICK_88X70, GX_PIXELMAP_ID_DEVICETYPE_JOYSTICK_88X70);
		break;
	case DEVICE_TYPE_DIGITAL_JOYSTICK:
		gx_icon_pixelmap_set (icon, GX_PIXELMAP_ID_DEVICETYPE_DIGITALJOYSTICK_88X70, GX_PIXELMAP_ID_DEVICETYPE_DIGITALJOYSTICK_88X70);
		break;
	case DEVICE_TYPE_SINGLE_SWITCH:
		gx_icon_pixelmap_set (icon, GX_PIXELMAP_ID_DEVICETYPE_SINGLESWITCH_88X70, GX_PIXELMAP_ID_DEVICETYPE_SINGLESWITCH_88X70);
		break;
	case DEVICE_TYPE_TWO_SWITCH:
		gx_icon_pixelmap_set (icon, GX_PIXELMAP_ID_DEVICETYPE_TWOSWITCH_88X70, GX_PIXELMAP_ID_DEVICETYPE_TWOSWITCH_88X70);
		break;
	case END_OF_DEVICE_TYPES:
	default:
		gx_icon_pixelmap_set (icon, GX_PIXELMAP_ID_BLANK_60X50, GX_PIXELMAP_ID_BLANK_60X50);
		break;
	} // end switch
}

//******************************************************************************************

VOID SelectNextDevice (VOID)
{
	USHORT device;

	device = dd_Get_USHORT (MAX_GROUPS, DD_DEVICE_TYPE);
	++device;
	if (device == END_OF_DEVICE_TYPES)
		device = 0;
	dd_Set_USHORT (MAX_GROUPS, DD_DEVICE_TYPE, device);
}

//******************************************************************************************
// StartJoystickOperation
//	This function starts a timer to determine the appropriate action to effect when 
//	the joystick demand is initiated.
//******************************************************************************************
VOID StartJoystickOperation (GX_RESOURCE_ID gx_widget_id)
{
	//USHORT group;
	USHORT activeFeature;

	//group = dd_Get_USHORT (MAX_GROUPS, DD_GROUP);
	activeFeature = dd_Get_USHORT (MAX_GROUPS, DD_ACTIVE_FEATURE);

	switch (activeFeature)
	{
	case POWER_ONOFF_ID:
	case NEXT_FUNCTION_ID:
	case NEXT_PROFILE_ID:
	case NEXT_GROUP_ID:
		ProcessDrivingFeature (gx_widget_id);
		break;
	case BLUETOOTH_ID:
		break;
	}
}

//******************************************************************************************

VOID StopJoystickOperation (USHORT gx_widget_id)
{
	USHORT activeFeature;

	//group = dd_Get_USHORT (MAX_GROUPS, DD_GROUP);
	activeFeature = dd_Get_USHORT (MAX_GROUPS, DD_ACTIVE_FEATURE);

	switch (activeFeature)
	{
	case POWER_ONOFF_ID:
	case NEXT_FUNCTION_ID:
	case NEXT_PROFILE_ID:
	case NEXT_GROUP_ID:
		strcpy_s (g_StatusMessage, sizeof (g_StatusMessage), "Ready");
		gx_prompt_text_set (&MainUserScreen.base.PrimaryTemplate_StatusMessage, g_StatusMessage);
		break;
	case BLUETOOTH_ID:
		break;
	} // end switch
}

//******************************************************************************************
// Select next audio level
USHORT SelectNextAudioLevel (VOID)
{
	USHORT activeSubitem;
	USHORT counter;
	USHORT sounds;

	activeSubitem = dd_GetSubItem_USHORT (0, DD_ACTIVE_FEATURE_SUBITEM, (USHORT) AUDIBLE_OUT_FEATURE_ID);		// Get the active feature
	for (counter = 0; counter < MAX_AUDIBLE_SUBITEMS; ++counter)		// We need to find the next level (subitem) that has any sounds programmed for use.
	{
		++activeSubitem;		// Next level
		if (activeSubitem >= MAX_AUDIBLE_SUBITEMS)	// Rollover
			activeSubitem = 0;
		sounds = dd_GetSubItem_USHORT (MAX_GROUPS, DD_ACTIVE_SPEAKER_SUBITEM_FORWARD, activeSubitem);	// Get all 4 pads
		sounds += dd_GetSubItem_USHORT (MAX_GROUPS, DD_ACTIVE_SPEAKER_SUBITEM_REVERSE, activeSubitem);
		sounds += dd_GetSubItem_USHORT (MAX_GROUPS, DD_ACTIVE_SPEAKER_SUBITEM_LEFT, activeSubitem);
		sounds += dd_GetSubItem_USHORT (MAX_GROUPS, DD_ACTIVE_SPEAKER_SUBITEM_RIGHT, activeSubitem);
		if (sounds)	// Is any pad programmed for something?
			break;	// Yes? Then we are done looking.
	}
	dd_SetSubItem_USHORT (0, DD_ACTIVE_FEATURE_SUBITEM, (USHORT) AUDIBLE_OUT_FEATURE_ID, activeSubitem);		// save the active feature.

	return activeSubitem;
}

//******************************************************************************************
//VOID screen_switch(GX_WIDGET *parent, GX_WIDGET *new_screen)
//{
//    gx_widget_detach(current_screen);
//    gx_widget_attach(parent, new_screen);
//    current_screen = new_screen;
//}
//*************************************************************************************
// Function Name: Template_event_function
//
// Description: This handles any messages sent to the template window
//
//*************************************************************************************

UINT Template_event_function (GX_WINDOW *window, GX_EVENT *event_ptr)
{
	UINT myErr = GX_SUCCESS;
#if 0
	switch (event_ptr->gx_event_type)
	{
	case GX_EVENT_PEN_UP:
		{
			g_DeltaValue = -5;
		}
		break;
	} // end switch
    myErr = gx_window_event_process(window, event_ptr);
#endif 
	return myErr;
}

//*************************************************************************************

VOID ProcessDrivingFeature (GX_RESOURCE_ID gx_widget_id)
{
	switch (gx_widget_id)
	{
		case JS_UP_BUTTON:
			strcpy_s (g_StatusMessage, sizeof (g_StatusMessage), "Driving Forward");
			break;
		case JS_LEFT_BUTTON:
			strcpy_s (g_StatusMessage, sizeof (g_StatusMessage), "Driving Left");
			break;
		case JS_DOWN_BUTTON:
			strcpy_s (g_StatusMessage, sizeof (g_StatusMessage), "Driving in Reverse");
			break;
		case JS_RIGHT_BUTTON:
			strcpy_s (g_StatusMessage, sizeof (g_StatusMessage), "Driving Right");
			break;
		default:
			strcpy_s (g_StatusMessage, sizeof (g_StatusMessage), "Ready");
			break;
	}

	gx_prompt_text_set (&MainUserScreen.base.PrimaryTemplate_StatusMessage, g_StatusMessage);
}
