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

#define SCREEN_STACK_SIZE  5
#define SCRATCHPAD_PIXELS  (PRIMARYDISPLAY_X_RESOLUTION * PRIMARYDISPLAY_Y_RESOLUTION * 2)
#define FEATURE_LIST_VISIBLE_ROWS 5

//--------------------------------------------------------------------
// Define prototypes.
VOID start_guix(VOID);

//--------------------------------------------------------------------
// Global Variables
TX_BYTE_POOL       memory_pool;
GX_COLOR           scratchpad[SCRATCHPAD_PIXELS];

// The following hold the Digital (non0) vs Proportional (0) setting for each pad.
typedef enum PHYSICAL_PAD {LEFT_PAD, RIGHT_PAD, CENTER_PAD, INVALID_PAD} PHYSICAL_PAD_ENUM;
typedef enum PAD_DIRECTION {OFF_DIRECTION = 0, LEFT_DIRECTION, FORWARD_DIRECTION, RIGHT_DIRECTION, INVALID_DIRECTION} PAD_DIRECTION_ENUM;
typedef enum PAD_TYPE {PROPORTIONAL_PADTYPE, DIGITAL_PADTYPE, INVALID_PAD_TYPE} PAD_TYPE_ENUM;
typedef enum FEATURE_ID {POWER_ONOFF_ID, BLUETOOTH_ID, NEXT_FUNCTION_OR_TOGGLE_ID, NEXT_PROFILE_OR_USER_MENU_ID, RNET_ID, NUM_FEATURES} FEATURE_ID_ENUM; // "invalid" must be last enum
typedef enum SEND_DATA_BOOL {STOP_SENDING_DATA = 0, START_SENDING_DATA} SEND_DATA_ENUM;

enum ENUM_TIMER_IDS {ARROW_PUSHED_TIMER_ID = 1, CALIBRATION_TIMER_ID, PAD_ACTIVE_TIMER_ID, PAD_ADVANCE_TIMER_ID};

int g_ClicksActive = FALSE;
int g_PowerUpInIdle = FALSE;

struct PadInfoStruct
{
	enum PAD_TYPE m_PadType;
	enum PAD_DIRECTION m_PadDirection;
	int m_PadMinimumCalibrationValue;
	int m_PadMaximumCalibrationValue;
	int m_DiagnosticOff_ID;
	GX_PIXELMAP_BUTTON *m_DiagnosticOff_Widget;
	int m_DiagnosticDigital_ID;
	GX_PIXELMAP_BUTTON *m_DiagnosticDigital_Widget;
	int m_DiagnosticProportional_ID;
	GX_PIXELMAP_BUTTON *m_DiagnosticProportional_Widget;
	GX_RECTANGLE m_DiagnosticWidigetLocation;
	GX_PIXELMAP_BUTTON *m_DirectionIcons[4];
} g_PadSettings[3];

typedef struct MAIN_SCREEN_FEATURE_STRUCT
{
    int m_Location;     // This indicates the Main Screen location, 0=Top most, 3=bottom most
    int m_Enabled;      // Indicates if this feature is active.
    GX_RESOURCE_ID m_SmallIcon;
    GX_RESOURCE_ID m_LargeIcon;
    GX_RESOURCE_ID m_SmallDescriptionID;
    GX_RESOURCE_ID m_LargeDescriptionID;
	GX_PROMPT m_PromptWidget;
	GX_CHECKBOX m_ButtonWidget;
} MAIN_SCREEN_FEATURE;
MAIN_SCREEN_FEATURE g_MainScreenFeatureInfo[NUM_FEATURES];

GX_RECTANGLE g_HiddenRectangle = {0,0,0,0};

GX_RECTANGLE g_DiagnosticWidgetLocations[] = {
	{35, 32, 35+62, 32+90},
	{183, 32, 183+62, 32+90},
	{66, 140, 66+145, 140+42}};

//GX_RECTANGLE g_FeatureLocation[] = {
//	{10, 16, 300, 86},
//	{30, 94, 290, 130},
//	{30, 130, 290, 162},
//	{30, 166, 290, 198},
//	{0,0,0,0}};

GX_RECTANGLE g_PadDirectionLocation[] = {
	{28, 55, 28+88, 55+70},
	{205, 55, 205+88, 55+70},
	{116, 150, 116+88, 150+70},
	{0,0,0,0}};

int g_ChangeScreen_WIP;

// Timeout information
int g_TimeoutValue;

GX_WINDOW *g_GoBackScreen = GX_NULL;
GX_WINDOW *g_CalibrationScreen = GX_NULL;

int g_CalibrationPadNumber;
int g_CalibrationStepNumber;
int g_PadValue = 35;
int g_DeltaValue;
int g_AdvancePadNumber;

#define GRAPH_CENTER_PT_XPOS 139	// From Left of screen
#define GRAPH_CENTER_PT_YPOS 130	// From Top of screen

GX_RECTANGLE g_CalibrationPadLocations[] = {
	{36, 32, 36+62, 32+98},			// Left Pad location
	{184, 32, 184+62, 32+98},		// Right Pad location
	{67, 140, 67+145, 140+42},		// Center Pad Location
	{0,0,0,0}};
GX_RECTANGLE g_CalibrationPromptLocations[] = {
	{20, 4, 38+239, 4+33},			// Max and Min Prompt location
	{GRAPH_CENTER_PT_XPOS-25, GRAPH_CENTER_PT_YPOS-26-60, GRAPH_CENTER_PT_XPOS-25+50, GRAPH_CENTER_PT_YPOS-26-60+26},		// Pad Value prompt location
	{0,0,0,0}};


int g_SettingsChanged;
int g_MinimumDriveValue = 20;		// Percentage, Minimum Drive value
char g_MinimuDriveString[8] = "20%";
char g_TimeoutValueString[8] = "OFF";
GX_CHAR ASL110_DISPLAY_VERSION_STRING[] = "Display: 1.6.0";
GX_CHAR g_HeadArrayVersionString[] = "ASL110: 1.x.x";

#define FEATURE_TOGGLE_BUTTON_ID 1000	// this is used as the dynamically created buttons in the feature list.

//*************************************************************************************
// Prototypes
void CreateFeatureWidgets (GX_VERTICAL_LIST *list);

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

    // Populate the screen stuff.
    // "Power ON/OFF" information and description
    g_MainScreenFeatureInfo[POWER_ONOFF_ID].m_Enabled = TRUE;
    g_MainScreenFeatureInfo[POWER_ONOFF_ID].m_Location = 0;
    g_MainScreenFeatureInfo[POWER_ONOFF_ID].m_LargeDescriptionID = GX_STRING_ID_POWER_ONOFF; //"POWER ON/OFF"
    g_MainScreenFeatureInfo[POWER_ONOFF_ID].m_SmallDescriptionID = GX_STRING_ID_POWER_ONOFF;
    g_MainScreenFeatureInfo[POWER_ONOFF_ID].m_SmallIcon = GX_PIXELMAP_ID_POWERICON_30X30;
    g_MainScreenFeatureInfo[POWER_ONOFF_ID].m_LargeIcon = GX_PIXELMAP_ID_POWERICON_LARGE;

    // "Bluetooth" information and description
    g_MainScreenFeatureInfo[BLUETOOTH_ID].m_Enabled = TRUE;
    g_MainScreenFeatureInfo[BLUETOOTH_ID].m_Location = 1;
    g_MainScreenFeatureInfo[BLUETOOTH_ID].m_LargeDescriptionID = GX_STRING_ID_BLUETOOTH;
    g_MainScreenFeatureInfo[BLUETOOTH_ID].m_SmallDescriptionID = GX_STRING_ID_BLUETOOTH;
    g_MainScreenFeatureInfo[BLUETOOTH_ID].m_SmallIcon = GX_PIXELMAP_ID_BLUETOOTH_30X30;
    g_MainScreenFeatureInfo[BLUETOOTH_ID].m_LargeIcon = GX_PIXELMAP_ID_BLUETOOTH_70X70;

    // "Next Function" information and description
    g_MainScreenFeatureInfo[NEXT_FUNCTION_OR_TOGGLE_ID].m_Enabled = TRUE;
    g_MainScreenFeatureInfo[NEXT_FUNCTION_OR_TOGGLE_ID].m_Location = 2;
    g_MainScreenFeatureInfo[NEXT_FUNCTION_OR_TOGGLE_ID].m_LargeDescriptionID = GX_STRING_ID_NEXT_FUNCTION; // "NEXT FUNCTION")
    g_MainScreenFeatureInfo[NEXT_FUNCTION_OR_TOGGLE_ID].m_SmallDescriptionID = GX_STRING_ID_NEXT_FUNCTION;
    g_MainScreenFeatureInfo[NEXT_FUNCTION_OR_TOGGLE_ID].m_SmallIcon = GX_PIXELMAP_ID_FUNCTIONNEXT_30X30;
    g_MainScreenFeatureInfo[NEXT_FUNCTION_OR_TOGGLE_ID].m_LargeIcon = GX_PIXELMAP_ID_FUNCTIONNEXT_70X70;

    // "Next Profile" information and description
    g_MainScreenFeatureInfo[NEXT_PROFILE_OR_USER_MENU_ID].m_Enabled = TRUE;
    g_MainScreenFeatureInfo[NEXT_PROFILE_OR_USER_MENU_ID].m_Location = 3;
    g_MainScreenFeatureInfo[NEXT_PROFILE_OR_USER_MENU_ID].m_LargeDescriptionID = GX_STRING_ID_NEXT_PROFILE; // "NEXT PROFILE"
    g_MainScreenFeatureInfo[NEXT_PROFILE_OR_USER_MENU_ID].m_SmallDescriptionID = GX_STRING_ID_NEXT_PROFILE;
    g_MainScreenFeatureInfo[NEXT_PROFILE_OR_USER_MENU_ID].m_SmallIcon = GX_PIXELMAP_ID_PROFILENEXT_30X30;
    g_MainScreenFeatureInfo[NEXT_PROFILE_OR_USER_MENU_ID].m_LargeIcon = GX_PIXELMAP_ID_PROFILENEXT_70X70;

    // "RNet SEATING" information and description
    g_MainScreenFeatureInfo[RNET_ID].m_Enabled = TRUE;
    g_MainScreenFeatureInfo[RNET_ID].m_Location = 4;
    g_MainScreenFeatureInfo[RNET_ID].m_LargeDescriptionID = GX_STRING_ID_RNET_SEATING;
    g_MainScreenFeatureInfo[RNET_ID].m_SmallDescriptionID = GX_STRING_ID_RNET_SEATING;
    g_MainScreenFeatureInfo[RNET_ID].m_SmallIcon = GX_PIXELMAP_ID_RNET_SEATING_30X30;
    g_MainScreenFeatureInfo[RNET_ID].m_LargeIcon = GX_PIXELMAP_ID_RNET_LOGO_70X70;

	// Populate the default Pad settings.
	g_PadSettings[LEFT_PAD].m_PadDirection = LEFT_DIRECTION;
	g_PadSettings[LEFT_PAD].m_PadType = PROPORTIONAL_PADTYPE;
	g_PadSettings[LEFT_PAD].m_DirectionIcons[OFF_DIRECTION] = &SetPadDirectionScreen.SetPadDirectionScreen_LeftPad_Off_Button;
	g_PadSettings[LEFT_PAD].m_DirectionIcons[RIGHT_DIRECTION] = &SetPadDirectionScreen.SetPadDirectionScreen_LeftPad_RightArrow_Button;
	g_PadSettings[LEFT_PAD].m_DirectionIcons[LEFT_DIRECTION] = &SetPadDirectionScreen.SetPadDirectionScreen_LeftPad_LeftArrow_Button;
	g_PadSettings[LEFT_PAD].m_DirectionIcons[FORWARD_DIRECTION] = &SetPadDirectionScreen.SetPadDirectionScreen_LeftPad_ForwardArrow_Button;
	g_PadSettings[LEFT_PAD].m_PadMinimumCalibrationValue = 0;
	g_PadSettings[LEFT_PAD].m_PadMaximumCalibrationValue = 100;
	g_PadSettings[LEFT_PAD].m_DiagnosticWidigetLocation = g_DiagnosticWidgetLocations[LEFT_PAD];
	g_PadSettings[LEFT_PAD].m_DiagnosticOff_ID = LEFT_PAD_OFF_BTN_ID;
	g_PadSettings[LEFT_PAD].m_DiagnosticDigital_ID = LEFT_PAD_DIGITAL_BTN_ID;
	g_PadSettings[LEFT_PAD].m_DiagnosticProportional_ID = LEFT_PAD_PROP_BTN_ID;
	g_PadSettings[LEFT_PAD].m_DiagnosticOff_Widget = &DiagnosticScreen.DiagnosticScreen_LeftPadOff_Button;
	g_PadSettings[LEFT_PAD].m_DiagnosticProportional_Widget = &DiagnosticScreen.DiagnosticScreen_LeftPadProp_Button;
	g_PadSettings[LEFT_PAD].m_DiagnosticDigital_Widget = &DiagnosticScreen.DiagnosticScreen_LeftPadDigital_Button;

	g_PadSettings[RIGHT_PAD].m_PadDirection = RIGHT_DIRECTION;
	g_PadSettings[RIGHT_PAD].m_PadType = PROPORTIONAL_PADTYPE;
	g_PadSettings[RIGHT_PAD].m_DirectionIcons[OFF_DIRECTION] = &SetPadDirectionScreen.SetPadDirectionScreen_RightPad_Off_Button;
	g_PadSettings[RIGHT_PAD].m_DirectionIcons[RIGHT_DIRECTION] = &SetPadDirectionScreen.SetPadDirectionScreen_RightPad_RightArrow_Button;
	g_PadSettings[RIGHT_PAD].m_DirectionIcons[LEFT_DIRECTION] = &SetPadDirectionScreen.SetPadDirectionScreen_RightPad_LeftArrow_Button;
	g_PadSettings[RIGHT_PAD].m_DirectionIcons[FORWARD_DIRECTION] = &SetPadDirectionScreen.SetPadDirectionScreen_RightPad_ForwardArrow_Button;
	g_PadSettings[RIGHT_PAD].m_PadMinimumCalibrationValue = 5;
	g_PadSettings[RIGHT_PAD].m_PadMaximumCalibrationValue = 95;
	g_PadSettings[RIGHT_PAD].m_DiagnosticWidigetLocation = g_DiagnosticWidgetLocations[RIGHT_PAD];
	g_PadSettings[RIGHT_PAD].m_DiagnosticOff_ID = RIGHT_PAD_OFF_BTN_ID;
	g_PadSettings[RIGHT_PAD].m_DiagnosticDigital_ID = RIGHT_PAD_DIGITAL_BTN_ID;
	g_PadSettings[RIGHT_PAD].m_DiagnosticProportional_ID = RIGHT_PAD_PROP_BTN_ID;
	g_PadSettings[RIGHT_PAD].m_DiagnosticOff_Widget = &DiagnosticScreen.DiagnosticScreen_RightPadOff_Button;
	g_PadSettings[RIGHT_PAD].m_DiagnosticProportional_Widget = &DiagnosticScreen.DiagnosticScreen_RightPadProp_Button;
	g_PadSettings[RIGHT_PAD].m_DiagnosticDigital_Widget = &DiagnosticScreen.DiagnosticScreen_RightPadDigital_Button;

	g_PadSettings[CENTER_PAD].m_PadDirection = FORWARD_DIRECTION;
	g_PadSettings[CENTER_PAD].m_PadType = PROPORTIONAL_PADTYPE;
	g_PadSettings[CENTER_PAD].m_DirectionIcons[OFF_DIRECTION] = &SetPadDirectionScreen.SetPadDirectionScreen_CenterPad_Off_Button;
	g_PadSettings[CENTER_PAD].m_DirectionIcons[RIGHT_DIRECTION] = &SetPadDirectionScreen.SetPadDirectionScreen_CenterPad_RightArrow_Button;
	g_PadSettings[CENTER_PAD].m_DirectionIcons[LEFT_DIRECTION] = &SetPadDirectionScreen.SetPadDirectionScreen_CenterPad_LeftArrow_Button;
	g_PadSettings[CENTER_PAD].m_DirectionIcons[FORWARD_DIRECTION] = &SetPadDirectionScreen.SetPadDirectionScreen_CenterPad_ForwardArrow_Button;
	g_PadSettings[CENTER_PAD].m_PadMinimumCalibrationValue = 10;
	g_PadSettings[CENTER_PAD].m_PadMaximumCalibrationValue = 90;
	g_PadSettings[CENTER_PAD].m_DiagnosticWidigetLocation = g_DiagnosticWidgetLocations[CENTER_PAD];
	g_PadSettings[CENTER_PAD].m_DiagnosticOff_ID = CENTER_PAD_OFF_BTN_ID;
	g_PadSettings[CENTER_PAD].m_DiagnosticDigital_ID = CENTER_PAD_DIGITAL_BTN_ID;
	g_PadSettings[CENTER_PAD].m_DiagnosticProportional_ID = CENTER_PAD_PROP_BTN_ID;
	g_PadSettings[CENTER_PAD].m_DiagnosticOff_Widget = &DiagnosticScreen.DiagnosticScreen_CenterPadOff_Button;
	g_PadSettings[CENTER_PAD].m_DiagnosticProportional_Widget = &DiagnosticScreen.DiagnosticScreen_CenterPadProp_Button;
	g_PadSettings[CENTER_PAD].m_DiagnosticDigital_Widget = &DiagnosticScreen.DiagnosticScreen_CenterPadDigital_Button;

	g_TimeoutValue = 20;
	myError = -1;
    myError = gx_studio_display_configure(PRIMARYDISPLAY, win32_graphics_driver_setup_24xrgb, 
        LANGUAGE_ENGLISH, PRIMARYDISPLAY_THEME_1, &root);

//    myError = gx_studio_named_widget_create("PrimaryTemplate", (GX_WIDGET *)root, GX_NULL);
	myError = gx_studio_named_widget_create("DiagnosticScreen", GX_NULL, GX_NULL);
	myError = gx_studio_named_widget_create("FeatureSettingsScreen", GX_NULL, GX_NULL);
	CreateFeatureWidgets (&FeatureSettingsScreen.FeatureSettingsScreen_FeatureListBox);
	myError = gx_studio_named_widget_create("HHP_Start_Screen", GX_NULL, GX_NULL);
	//myError = gx_studio_named_widget_create("MainUserScreen", , GX_NULL, GX_NULL);
	myError = gx_studio_named_widget_create("MainUserScreen_1", (GX_WIDGET *)root, GX_NULL);
	myError = gx_studio_named_widget_create("MainUserScreen_3", GX_NULL, GX_NULL);
	myError = gx_studio_named_widget_create("MinimumDriveScreen", GX_NULL, GX_NULL);
	myError = gx_studio_named_widget_create("MoreSelectionScreen", GX_NULL, GX_NULL);
	myError = gx_studio_named_widget_create("PerformanceSelectionScreen", GX_NULL, GX_NULL);
	myError = gx_studio_named_widget_create("OON_Screen", GX_NULL, GX_NULL);
	myError = gx_studio_named_widget_create("PadCalibrationScreen", GX_NULL, GX_NULL);
	myError = gx_studio_named_widget_create("PadOptionsSettingsScreen", GX_NULL, GX_NULL);
	myError = gx_studio_named_widget_create("ResetScreen", GX_NULL, GX_NULL);
	myError = gx_studio_named_widget_create("ResetFinishScreen", GX_NULL, GX_NULL);
	myError = gx_studio_named_widget_create("StartupScreen", GX_NULL, GX_NULL);
	myError = gx_studio_named_widget_create("SetPadDirectionScreen", GX_NULL, GX_NULL);
	myError = gx_studio_named_widget_create("SetPadTypeScreen", GX_NULL, GX_NULL);
	myError = gx_studio_named_widget_create("UserSettingsScreen", GX_NULL, GX_NULL);
	myError = gx_studio_named_widget_create("UserSelectionScreen", GX_NULL, GX_NULL);
	myError = gx_studio_named_widget_create("VeerAdjustScreen", GX_NULL, GX_NULL);
	myError = gx_studio_named_widget_create("AdvancePadSettingsScreen", GX_NULL, GX_NULL);


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
// Function Name: Template_event_function
//
// Description: This handles any messages sent to the template window
//
//*************************************************************************************

UINT Template_event_function (GX_WINDOW *window, GX_EVENT *event_ptr)
{
	UINT myErr = -1;

	switch (event_ptr->gx_event_type)
	{
	case GX_SIGNAL(DOWN_ARROW_BTN_ID, GX_EVENT_CLICKED):
		if (window->gx_widget_name == "PadCalibrationScreen")
		{
			if (g_CalibrationStepNumber == 0)		// We are doing minimum
			{
				if (g_PadSettings[g_CalibrationPadNumber].m_PadMinimumCalibrationValue > 4)
					g_PadSettings[g_CalibrationPadNumber].m_PadMinimumCalibrationValue -= 5;
				myErr = gx_numeric_prompt_value_set (&PadCalibrationScreen.PadCalibrationScreen_Value_Prompt, g_PadSettings[g_CalibrationPadNumber].m_PadMinimumCalibrationValue);
			}
			else if (g_CalibrationStepNumber == 1)	// Doing maximum
			{
				if (g_PadSettings[g_CalibrationPadNumber].m_PadMaximumCalibrationValue > 4)
					g_PadSettings[g_CalibrationPadNumber].m_PadMaximumCalibrationValue -= 5;
				myErr = gx_numeric_prompt_value_set (&PadCalibrationScreen.PadCalibrationScreen_Value_Prompt, g_PadSettings[g_CalibrationPadNumber].m_PadMaximumCalibrationValue);
			}
			gx_system_dirty_mark(g_CalibrationScreen);		// This forces the gauge to be updated and redrawn
		}
		break;
	case GX_SIGNAL(UP_ARROW_BTN_ID, GX_EVENT_CLICKED):
		if (window->gx_widget_name == "PadCalibrationScreen")
		{
			if (g_CalibrationStepNumber == 0)		// We are doing minimum
			{
				if (g_PadSettings[g_CalibrationPadNumber].m_PadMinimumCalibrationValue < 100)
					g_PadSettings[g_CalibrationPadNumber].m_PadMinimumCalibrationValue += 5;
				myErr = gx_numeric_prompt_value_set (&PadCalibrationScreen.PadCalibrationScreen_Value_Prompt, g_PadSettings[g_CalibrationPadNumber].m_PadMinimumCalibrationValue);
			}
			else if (g_CalibrationStepNumber == 1)	// Doing maximum
			{
				if (g_PadSettings[g_CalibrationPadNumber].m_PadMaximumCalibrationValue < 100)
					g_PadSettings[g_CalibrationPadNumber].m_PadMaximumCalibrationValue += 5;
				myErr = gx_numeric_prompt_value_set (&PadCalibrationScreen.PadCalibrationScreen_Value_Prompt, g_PadSettings[g_CalibrationPadNumber].m_PadMaximumCalibrationValue);
			}
			gx_system_dirty_mark(g_CalibrationScreen);		// This forces the gauge to be updated and redrawn
		}
//		else if (window->gx_widget_name == "StartupScreen")
//		{
//	        screen_toggle((GX_WINDOW *)&Main_User_Screen, window);
//		}
		break;
	case GX_EVENT_PEN_DOWN:	// We are going to determine if the PAD button is pressed and start a timer to increment the 
							// ... long time (2 seconds) and goto Programming if so.
		if (event_ptr->gx_event_target->gx_widget_name == "PadActiveButton")
		{
			g_DeltaValue = +5;
			gx_system_timer_start(window, PAD_ACTIVE_TIMER_ID, 8, 8);
			//myErr = gx_slider_value_set((GX_SLIDER*)&PadCalibrationScreen.PadCalibrationScreen_PadValue_Slider, &PadCalibrationScreen.PadCalibrationScreen_PadValue_Slider.gx_slider_info, g_PadValue);
		}
		break;
    case GX_EVENT_TIMER:
        if (event_ptr->gx_event_payload.gx_event_timer_id == PAD_ACTIVE_TIMER_ID)
		{
			g_PadValue += g_DeltaValue;
			if (g_PadValue > 100)
				g_PadValue = 100;
			if (g_PadValue <= 0)
				gx_system_timer_stop(window, PAD_ACTIVE_TIMER_ID);
			if (g_PadValue > 0)
			{
				myErr = gx_widget_resize ((GX_WIDGET*) &PadCalibrationScreen.PadCalibrationScreen_PadValue_Prompt, &g_CalibrationPromptLocations[1]);
				myErr = gx_numeric_prompt_value_set (&PadCalibrationScreen.PadCalibrationScreen_PadValue_Prompt, g_PadValue);
			}
			else
			{
				myErr = gx_widget_resize ((GX_WIDGET*) &PadCalibrationScreen.PadCalibrationScreen_PadValue_Prompt, &g_HiddenRectangle);
				myErr = gx_numeric_prompt_value_set (&PadCalibrationScreen.PadCalibrationScreen_PadValue_Prompt, g_PadValue);
			}
			gx_system_dirty_mark(g_CalibrationScreen);		// This forces the gauge to be updated and redrawn
		}
		break;
	case GX_EVENT_PEN_UP:
		{
			g_DeltaValue = -5;
		}
		break;
	} // end switch

    myErr = gx_window_event_process(window, event_ptr);

	return myErr;
}

//*************************************************************************************
// Function Name: HHP_Start_Screen_event_process
//
// Description: This handles the Startup Screen messages
//
//*************************************************************************************

UINT HHP_Start_Screen_event_process (GX_WINDOW *window, GX_EVENT *event_ptr)
{
	UINT myErr = -1;

    myErr = gx_window_event_process(window, event_ptr);

	switch (event_ptr->gx_event_type)
	{
	case GX_SIGNAL(PAD_SETTINGS_BTN_ID, GX_EVENT_CLICKED):		// When selected, goto Main Pad Setting scree.
        screen_toggle((GX_WINDOW *)&PadOptionsSettingsScreen, window);
		break;
	case GX_SIGNAL(SETTINGS_BTN_ID, GX_EVENT_CLICKED):
        screen_toggle((GX_WINDOW *)&UserSelectionScreen, window);
		break;
	case GX_SIGNAL(MORE_BTN_ID, GX_EVENT_CLICKED):
        screen_toggle((GX_WINDOW *)&MoreSelectionScreen, window);
		break;
	case GX_SIGNAL(OK_BTN_ID, GX_EVENT_CLICKED):
        screen_toggle((GX_WINDOW *)g_GoBackScreen, window);
		break;
	}
	return myErr;
}

//*************************************************************************************
// Function Name: StartupScreen_event_handler
// 
//*************************************************************************************

VOID StartScreen_draw_function (GX_WINDOW *window)
{
//	UINT myErr;
//	char *myString;

//	myErr = gx_prompt_text_get ((GX_PROMPT*)&StartupScreen.StartupScreen_StatusPrompt, &myString);
//	myString = "Starting...";
//	myErr = gx_prompt_text_set (&StartupScreen.StartupScreen_StatusPrompt, myString);
//	myErr = gx_prompt_text_color_set ((GX_PROMPT*) &StartupScreen.StartupScreen_StatusPrompt, GX_COLOR_ID_MAROON, GX_COLOR_ID_MAROON, GX_COLOR_ID_MAROON);

	gx_window_draw(window);
}

UINT StartupScreen_event_handler (GX_WINDOW *widget, GX_EVENT *event_ptr)
{
	UINT myErr;

    myErr = gx_window_event_process(widget, event_ptr);
	
	return GX_SUCCESS;
}

//*************************************************************************************
// Function Name: DisplayMainScreenActiveFeatures
//
// Description: This displays the features that are active in the order specificed
//	in the Screen Prompts "objects".
//
//*************************************************************************************

UINT DisplayMainScreenActiveFeatures ()
{
	int enabledCount;
	int feature;
	UINT myErr = GX_SUCCESS;

	// Adjust the displayed information based upon the RNet setting.
	// .. If RNet is enabled, the NEXT FUNCTION feature becomes RNet TOGGLE
	// .. and NEXT PROFILE feature become RNet MENU.
    if (g_MainScreenFeatureInfo[RNET_ID].m_Enabled)
	{
		// Display as "RNet TOGGLE"
		g_MainScreenFeatureInfo[NEXT_FUNCTION_OR_TOGGLE_ID].m_LargeDescriptionID = GX_STRING_ID_RNET_TOGGLE;
		g_MainScreenFeatureInfo[NEXT_FUNCTION_OR_TOGGLE_ID].m_SmallDescriptionID = GX_STRING_ID_RNET_TOGGLE;
		g_MainScreenFeatureInfo[NEXT_FUNCTION_OR_TOGGLE_ID].m_SmallIcon = GX_PIXELMAP_ID_RNET_TOGGLEFR_30X30;
		g_MainScreenFeatureInfo[NEXT_FUNCTION_OR_TOGGLE_ID].m_LargeIcon = GX_PIXELMAP_ID_RNET_LOGO_70X70;

		// Display as "RNet MENU"
		g_MainScreenFeatureInfo[NEXT_PROFILE_OR_USER_MENU_ID].m_LargeDescriptionID = GX_STRING_ID_RNET_MENU;
		g_MainScreenFeatureInfo[NEXT_PROFILE_OR_USER_MENU_ID].m_SmallDescriptionID = GX_STRING_ID_RNET_MENU;
		g_MainScreenFeatureInfo[NEXT_PROFILE_OR_USER_MENU_ID].m_SmallIcon = GX_PIXELMAP_ID_RNET_MENU_30X30;
		g_MainScreenFeatureInfo[NEXT_PROFILE_OR_USER_MENU_ID].m_LargeIcon = GX_PIXELMAP_ID_RNET_LOGO_70X70;
	}
	else
	{
		// Display as NEXT FUNCTION
		g_MainScreenFeatureInfo[NEXT_FUNCTION_OR_TOGGLE_ID].m_LargeDescriptionID = GX_STRING_ID_NEXT_FUNCTION; // "NEXT FUNCTION")
		g_MainScreenFeatureInfo[NEXT_FUNCTION_OR_TOGGLE_ID].m_SmallDescriptionID = GX_STRING_ID_NEXT_FUNCTION;
		g_MainScreenFeatureInfo[NEXT_FUNCTION_OR_TOGGLE_ID].m_SmallIcon = GX_PIXELMAP_ID_FUNCTIONNEXT_30X30;
		g_MainScreenFeatureInfo[NEXT_FUNCTION_OR_TOGGLE_ID].m_LargeIcon = GX_PIXELMAP_ID_FUNCTIONNEXT_70X70;

		// Display as NEXT PROFILE
		g_MainScreenFeatureInfo[NEXT_PROFILE_OR_USER_MENU_ID].m_LargeDescriptionID = GX_STRING_ID_NEXT_PROFILE; // "NEXT PROFILE"
		g_MainScreenFeatureInfo[NEXT_PROFILE_OR_USER_MENU_ID].m_SmallDescriptionID = GX_STRING_ID_NEXT_PROFILE;
		g_MainScreenFeatureInfo[NEXT_PROFILE_OR_USER_MENU_ID].m_SmallIcon = GX_PIXELMAP_ID_PROFILENEXT_30X30;
		g_MainScreenFeatureInfo[NEXT_PROFILE_OR_USER_MENU_ID].m_LargeIcon = GX_PIXELMAP_ID_PROFILENEXT_70X70;
	}
    // Count the number of active items so we can populate appropriately.
    // Hide the Non-Active features.
    enabledCount = 0;

    // Locate the first feature to display
    for (feature = 0; feature < NUM_FEATURES; ++feature)
    {
        if (g_MainScreenFeatureInfo[feature].m_Enabled)
        {
            ++enabledCount;
            switch (g_MainScreenFeatureInfo[feature].m_Location)
            {
            case 0: // Show the first line
                myErr = gx_prompt_text_id_set (&MainUserScreen.MainUserScreen_FirstPrompt, g_MainScreenFeatureInfo[feature].m_LargeDescriptionID);
                myErr = gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_FirstIcon, g_MainScreenFeatureInfo[feature].m_LargeIcon);
                break;
            case 1: // Show second line item
                myErr = gx_prompt_text_id_set (&MainUserScreen.MainUserScreen_SecondPrompt, g_MainScreenFeatureInfo[feature].m_SmallDescriptionID);
                myErr = gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_SecondIcon, g_MainScreenFeatureInfo[feature].m_SmallIcon);
                break;
            case 2: // Show third line item
                myErr = gx_prompt_text_id_set (&MainUserScreen.MainUserScreen_ThirdPrompt, g_MainScreenFeatureInfo[feature].m_SmallDescriptionID);
                myErr = gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_ThirdIcon, g_MainScreenFeatureInfo[feature].m_SmallIcon);
                break;
            case 3: // Show fourth line item
                myErr = gx_prompt_text_id_set (&MainUserScreen.MainUserScreen_FourthPrompt, g_MainScreenFeatureInfo[feature].m_SmallDescriptionID);
                myErr = gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_FourthIcon, g_MainScreenFeatureInfo[feature].m_SmallIcon);
                break;
            case 4: // Show fifth line item
                myErr = gx_prompt_text_id_set (&MainUserScreen.MainUserScreen_FifthPrompt, g_MainScreenFeatureInfo[feature].m_SmallDescriptionID);
                myErr = gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_FifthIcon, g_MainScreenFeatureInfo[feature].m_SmallIcon);
                break;
            }
        }
    }
    // Now blank any unused items.
    for ( ; enabledCount < NUM_FEATURES; ++enabledCount)   // Start with the number of items that are enabled.
    {
        switch (enabledCount)
        {
        case 0: // Show the first line
            myErr = gx_prompt_text_id_set (&MainUserScreen.MainUserScreen_FirstPrompt, GX_STRING_ID_BLANK);
            myErr = gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_FirstIcon, GX_PIXELMAP_ID_BLANK_30X30);
            break;
        case 1: // Show second line item
            myErr = gx_prompt_text_id_set (&MainUserScreen.MainUserScreen_SecondPrompt, GX_STRING_ID_BLANK);
            myErr = gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_SecondIcon, GX_PIXELMAP_ID_BLANK_30X30);
            break;
        case 2: // Process third line item, move to the 2nd line
            myErr = gx_prompt_text_id_set (&MainUserScreen.MainUserScreen_ThirdPrompt, GX_STRING_ID_BLANK);
            myErr = gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_ThirdIcon, GX_PIXELMAP_ID_BLANK_30X30);
            break;
        case 3: // Process fourth line item, move to the 3rd line.
            myErr = gx_prompt_text_id_set (&MainUserScreen.MainUserScreen_FourthPrompt, GX_STRING_ID_BLANK);
            myErr = gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_FourthIcon, GX_PIXELMAP_ID_BLANK_30X30);
            break;
        case 4: // Show fifth line item
            myErr = gx_prompt_text_id_set (&MainUserScreen.MainUserScreen_FifthPrompt, GX_STRING_ID_BLANK);
            myErr = gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_FifthIcon, GX_PIXELMAP_ID_BLANK_30X30);
        } // end of switch
    } // end of for
    return myErr;
}

//*************************************************************************************
// Function Name: MainUserScreen_event_process
//
// Description: This handles the User Screen messages.
//
//*************************************************************************************

VOID MainUserScreen_draw_function(GX_WINDOW *window)
{
//    g_ActiveScreen = (GX_WIDGET*) window;

    DisplayMainScreenActiveFeatures();  // Redraw the items.

    gx_window_draw(window);
}

//*************************************************************************************
// Function Name: Main_User_Screen_event_process
//
// Description: This handles the User Screen messages.
//
//*************************************************************************************
UINT MainUserScreen_event_process (GX_WINDOW *window, GX_EVENT *event_ptr)
{
	UINT myErr = -1;
	UINT feature;
	int activeCount;

	switch (event_ptr->gx_event_type)
	{
    case GX_EVENT_TIMER:
        if (event_ptr->gx_event_payload.gx_event_timer_id == ARROW_PUSHED_TIMER_ID)
		{
	        screen_toggle((GX_WINDOW *)&HHP_Start_Screen, window);
			g_ChangeScreen_WIP = TRUE;
		}
		break;
	case GX_EVENT_PEN_DOWN:	// We are going to determine if the Up or Down arrow buttons have been held for a
							// ... long time (2 seconds) and goto Programming if so.

		if ((event_ptr->gx_event_target->gx_widget_name == "DownArrowButton") || (event_ptr->gx_event_target->gx_widget_name == "UpArrowButton"))
		{
			gx_system_timer_start(window, ARROW_PUSHED_TIMER_ID, 100, 0);
			g_ChangeScreen_WIP = FALSE;
		}
		break;
	case GX_EVENT_PEN_UP:
			gx_system_timer_stop(window, ARROW_PUSHED_TIMER_ID);
		break;

	case GX_EVENT_SHOW:
		g_GoBackScreen = window;		// Set the going back window.
		DisplayMainScreenActiveFeatures();
		break;

	case GX_SIGNAL (USER_PORT_BUTTON_ID, GX_EVENT_CLICKED):
		// TODO: Add code to process long presses.
		//DisplayMainScreenActiveFeatures();
        //Use the following to show Out Of Neutral screen
		screen_toggle((GX_WINDOW *)&MainUserScreen_3, window);
		//screen_toggle((GX_WINDOW *)&OON_Screen, window);
		break;

    case GX_SIGNAL (DOWN_ARROW_BTN_ID, GX_EVENT_CLICKED):
        // This is necessary to prevent the subsequent "Clicked" message from advancing the feature when we are changing to the Programming screen.
        if (g_ChangeScreen_WIP)
        {
            g_ChangeScreen_WIP = FALSE;
            break;
        }
        // Count the number of active features to set a limit on location
        activeCount = 0;
        for (feature = 0; feature < NUM_FEATURES; ++feature)
        {
            if (g_MainScreenFeatureInfo[feature].m_Enabled)
                ++activeCount;
        }
        // Move Top Feature to Bottom and move Bottom upward.
        for (feature = 0; feature < NUM_FEATURES; ++feature)
        {
            if (g_MainScreenFeatureInfo[feature].m_Enabled)
            {
                if (g_MainScreenFeatureInfo[feature].m_Location == 0)
                    g_MainScreenFeatureInfo[feature].m_Location = activeCount-1;
                else if (g_MainScreenFeatureInfo[feature].m_Location == 1)
                {
                    g_MainScreenFeatureInfo[feature].m_Location = 0;
                    //SendModeChangeCommand (feature);        // Send this to the Head Array
                }
                else if (g_MainScreenFeatureInfo[feature].m_Location == 2)
                    g_MainScreenFeatureInfo[feature].m_Location = min (1, activeCount-1);
                else if (g_MainScreenFeatureInfo[feature].m_Location == 3)
                    g_MainScreenFeatureInfo[feature].m_Location = min (2, activeCount-1);
                else if (g_MainScreenFeatureInfo[feature].m_Location == 4)
                    g_MainScreenFeatureInfo[feature].m_Location = min (3, activeCount-1);
            }
        }
        DisplayMainScreenActiveFeatures();
        break;

    case GX_SIGNAL(UP_ARROW_BTN_ID, GX_EVENT_CLICKED):
        // This is necessary to prevent the subsequent "Clicked" message from advancing the feature when we are changing to the Programming screen.
        if (g_ChangeScreen_WIP)
        {
            g_ChangeScreen_WIP = FALSE;
            break;
        }
        // Count the number of active features to set a limit on location
        activeCount = 0;
        for (feature = 0; feature < NUM_FEATURES; ++feature)
        {
            if (g_MainScreenFeatureInfo[feature].m_Enabled)
                ++activeCount;
        }
        --activeCount;  // Translate the Number of items to Based Zero line number.

        // Move the features downward, limiting the movement by the number of Active Features.
        for (feature = 0; feature < NUM_FEATURES; ++feature)
        {
            if (g_MainScreenFeatureInfo[feature].m_Enabled)
            {
                if (g_MainScreenFeatureInfo[feature].m_Location == activeCount)
                {
                    g_MainScreenFeatureInfo[feature].m_Location = 0;
                   // SendModeChangeCommand (feature);        // Send this to the Head Array
                }
                else if (g_MainScreenFeatureInfo[feature].m_Location == 0)
                    g_MainScreenFeatureInfo[feature].m_Location = min (1, activeCount);
                else if (g_MainScreenFeatureInfo[feature].m_Location == 1)
                    g_MainScreenFeatureInfo[feature].m_Location = min (2, activeCount);
                else if (g_MainScreenFeatureInfo[feature].m_Location == 2)
                    g_MainScreenFeatureInfo[feature].m_Location = min (3, activeCount);
                else if (g_MainScreenFeatureInfo[feature].m_Location == 3)
                    g_MainScreenFeatureInfo[feature].m_Location = min (4, activeCount);
            }
        }
        DisplayMainScreenActiveFeatures();
        break;

	}

    myErr = gx_window_event_process(window, event_ptr);

	return myErr;
}
//VOID Main_User_Screen_draw_function(GX_WINDOW *window)
//{
////	UINT myErr;
////	GX_CONST GX_CHAR *myString;
//
//	gx_window_draw(window);
////	myErr = gx_prompt_text_get ((GX_PROMPT*)&Main_User_Screen.Main_User_Screen_BluetoothLargePrompt, &myString);
//
//}

UINT Main_User_Screen_event_process (GX_WINDOW *window, GX_EVENT *event_ptr)
{
	UINT myErr = -1;
	UINT feature;
	int activeCount;

	switch (event_ptr->gx_event_type)
	{
    case GX_EVENT_TIMER:
        if (event_ptr->gx_event_payload.gx_event_timer_id == ARROW_PUSHED_TIMER_ID)
		{
	        screen_toggle((GX_WINDOW *)&HHP_Start_Screen, window);
			g_ChangeScreen_WIP = TRUE;
		}
		break;
	case GX_EVENT_PEN_DOWN:	// We are going to determine if the Up or Down arrow buttons have been held for a
							// ... long time (2 seconds) and goto Programming if so.

		if ((event_ptr->gx_event_target->gx_widget_name == "DownArrowButton") || (event_ptr->gx_event_target->gx_widget_name == "UpArrowButton"))
		{
			gx_system_timer_start(window, ARROW_PUSHED_TIMER_ID, 100, 0);
			g_ChangeScreen_WIP = FALSE;
		}
		break;
	case GX_EVENT_PEN_UP:
			gx_system_timer_stop(window, ARROW_PUSHED_TIMER_ID);
		break;

	case GX_EVENT_SHOW:
		g_GoBackScreen = window;		// Set the going back window.
		DisplayMainScreenActiveFeatures();
		break;

	case GX_SIGNAL (USER_PORT_BUTTON_ID, GX_EVENT_CLICKED):
		// TODO: Adde LONG PRESS handling.
		DisplayMainScreenActiveFeatures();
		break;

	case GX_SIGNAL (DOWN_ARROW_BTN_ID, GX_EVENT_CLICKED):
		// This is necessary to prevent the subsequent "Clicked" message from advancing the feature when we are changing to the Programming screen.
		if (g_ChangeScreen_WIP)
		{
			g_ChangeScreen_WIP = FALSE;
			break;
		}
		// Count the number of active features to set a limit on location
		activeCount = 0;
		for (feature = 0; feature < 4; ++feature)
		{
			if (g_MainScreenFeatureInfo[feature].m_Enabled)
				++activeCount;
		}
		// Move Top Feature to Bottom and move Bottom upward.
		for (feature = 0; feature < 4; ++feature)
		{
			if (g_MainScreenFeatureInfo[feature].m_Enabled)
			{
				if (g_MainScreenFeatureInfo[feature].m_Location == 0)
					g_MainScreenFeatureInfo[feature].m_Location = activeCount-1;
				else if (g_MainScreenFeatureInfo[feature].m_Location == 1)
					g_MainScreenFeatureInfo[feature].m_Location = 0;
				else if (g_MainScreenFeatureInfo[feature].m_Location == 2)
					g_MainScreenFeatureInfo[feature].m_Location = min (1, activeCount-1);
				else if (g_MainScreenFeatureInfo[feature].m_Location == 3)
					g_MainScreenFeatureInfo[feature].m_Location = min (2, activeCount-1);
			}
		}
		DisplayMainScreenActiveFeatures();
		break;

	case GX_SIGNAL(UP_ARROW_BTN_ID, GX_EVENT_CLICKED):
		// This is necessary to prevent the subsequent "Clicked" message from advancing the feature when we are changing to the Programming screen.
		if (g_ChangeScreen_WIP)
		{
			g_ChangeScreen_WIP = FALSE;
			break;
		}
		// Count the number of active features to set a limit on location
		activeCount = 0;
		for (feature = 0; feature < 4; ++feature)
		{
			if (g_MainScreenFeatureInfo[feature].m_Enabled)
				++activeCount;
		}

		// Move the features downward, limiting the movement by the number of Active Features.
		for (feature = 0; feature < 4; ++feature)
		{
			if (g_MainScreenFeatureInfo[feature].m_Enabled)
			{
				if (g_MainScreenFeatureInfo[feature].m_Location == 0)
					g_MainScreenFeatureInfo[feature].m_Location = min (1, activeCount);
				else if (g_MainScreenFeatureInfo[feature].m_Location == 1)
					g_MainScreenFeatureInfo[feature].m_Location = min (2, activeCount);
				else if (g_MainScreenFeatureInfo[feature].m_Location == 2)
					g_MainScreenFeatureInfo[feature].m_Location = min (3, activeCount);
				else if (g_MainScreenFeatureInfo[feature].m_Location == 3)
					g_MainScreenFeatureInfo[feature].m_Location = 0;
				if (g_MainScreenFeatureInfo[feature].m_Location == activeCount)
					g_MainScreenFeatureInfo[feature].m_Location = 0;
			}
		}
		DisplayMainScreenActiveFeatures();
		break;
	}

    myErr = gx_window_event_process(window, event_ptr);

	return myErr;
}

//*************************************************************************************
// Function Name: OON_Window_event_function
//
// Description: This handles the Out of nuetral screen messages
//
//*************************************************************************************

UINT OON_Window_event_function (GX_WINDOW *window, GX_EVENT *event_ptr)
{
	gx_window_event_process(window, event_ptr);

	return GX_SUCCESS;
}

//*************************************************************************************
// Function Name: PadOptionsSettingsScreen_event_process
//
// Description: This dispatches the Pad Option Settings Screen messages
//
//*************************************************************************************

UINT PadOptionsSettingsScreen_event_process (GX_WINDOW *window, GX_EVENT *event_ptr)
{
	UINT myErr;

	switch (event_ptr->gx_event_type)
	{
	case GX_SIGNAL(OK_BTN_ID, GX_EVENT_CLICKED):
        screen_toggle((GX_WINDOW *)&HHP_Start_Screen, window);
		break;

	case GX_SIGNAL(PAD_TYPE_BTN_ID, GX_EVENT_CLICKED):
        screen_toggle((GX_WINDOW *)&SetPadTypeScreen, window);
		break;

	case GX_SIGNAL(PAD_DIRECTIONS_BTN_ID, GX_EVENT_CLICKED):
        screen_toggle((GX_WINDOW *)&SetPadDirectionScreen, window);
		break;
	}

    myErr = gx_window_event_process(window, event_ptr);

	return GX_SUCCESS;
}

//*************************************************************************************
// Function Name: PerformanceSelectionScreen_event_process
//
// Description: This dispatches the Pad Option Settings Screen messages
//
//*************************************************************************************

UINT PerformanceSelectionScreen_event_process (GX_WINDOW *window, GX_EVENT *event_ptr)
{
	UINT myErr;

	switch (event_ptr->gx_event_type)
	{
	case GX_SIGNAL(OK_BTN_ID, GX_EVENT_CLICKED):
        screen_toggle((GX_WINDOW *)&UserSelectionScreen, window);
		break;

	case GX_SIGNAL(VEER_ADJUST_BTN_ID, GX_EVENT_CLICKED):
        screen_toggle((GX_WINDOW *)&VeerAdjustScreen, window);
		break;

	case GX_SIGNAL(MINIMUM_DRIVE_BTN_ID, GX_EVENT_CLICKED):
        screen_toggle((GX_WINDOW *)&MinimumDriveScreen, window);
		break;
	}

    myErr = gx_window_event_process(window, event_ptr);

	return GX_SUCCESS;
}

//*************************************************************************************
// Function Name: MinimumDriveScreen_event_process
//
// Description: This dispatches the Pad Option Settings Screen messages
//
//*************************************************************************************

UINT MinimumDriveScreen_event_process (GX_WINDOW *window, GX_EVENT *event_ptr)
{
	UINT myErr;

	switch (event_ptr->gx_event_type)
	{
	case GX_SIGNAL(OK_BTN_ID, GX_EVENT_CLICKED):
        screen_toggle((GX_WINDOW *)&PerformanceSelectionScreen, window);
		break;

	case GX_SIGNAL (DRIVE_PERCENTAGE_BTN_ID, GX_EVENT_CLICKED):
		if (g_MinimumDriveValue >= 30)
		{
			g_MinimumDriveValue = 0;
			strcpy_s (g_MinimuDriveString, sizeof (g_MinimuDriveString), "OFF");
		}
		else
		{
			g_MinimumDriveValue += 5;
			sprintf_s (g_MinimuDriveString, sizeof (g_MinimuDriveString), "%d%%", g_MinimumDriveValue);
			gx_text_button_text_set (&MinimumDriveScreen.MinimumDriveScreen_DriverPencentage_Button, g_MinimuDriveString);
		}
		break;
	}

    myErr = gx_window_event_process(window, event_ptr);

	return GX_SUCCESS;
}

//************************************************************************************* 
// Function Name: VeerAdjust_Screen_event_handler
//
// Description: These functions support the Veer Adjustment feature.
//
//*************************************************************************************

int g_MyValues[20];

VOID VeerAdjust_Screen_draw_function (GX_WINDOW *window)
{
	gx_window_draw(window);
}

UINT g_NeutralDAC_Constant = 2048;
UINT g_NeutralDAC_Value = 2020;
UINT g_NeutralDAC_RangeValue = 410;

char g_SliderValue[20] = "gc";

UINT VeerSlider_event_function(GX_PIXELMAP_SLIDER *widget, GX_EVENT *event_ptr)
{
	int myNewVal, myOffset;
	int totalSteps;
	int increment;

    gx_pixelmap_slider_event_process(widget, event_ptr);

	switch (event_ptr->gx_event_type)
	{
	case GX_EVENT_SHOW:
		break;
	case GX_EVENT_PEN_DOWN:
	case GX_EVENT_PEN_DRAG:
	case GX_EVENT_PEN_UP:
		myNewVal = widget->gx_slider_info.gx_slider_info_current_val;
		totalSteps = widget->gx_slider_info.gx_slider_info_max_val - widget->gx_slider_info.gx_slider_info_min_val;
		myNewVal = myNewVal - (totalSteps / 2);

		increment = g_NeutralDAC_RangeValue / totalSteps / 2;	// This is how many going Plus and Minus.
		myOffset = myNewVal * increment;
		myNewVal = g_NeutralDAC_Constant + myOffset;
		g_NeutralDAC_Value = myNewVal;
		break;
	default:
		break;
	} // end swtich

	return GX_SUCCESS;
}

VOID Slider_Draw_Function (GX_PIXELMAP_SLIDER *slider)
{
	int myVal, totalSteps;
	int increment;

	totalSteps = slider->gx_slider_info.gx_slider_info_max_val - slider->gx_slider_info.gx_slider_info_min_val;

	increment = g_NeutralDAC_RangeValue / totalSteps / 2;	// This is how many going Plus and Minus.
	myVal = (int)(g_NeutralDAC_Value - g_NeutralDAC_Constant) / increment;

	slider->gx_slider_info.gx_slider_info_current_val = myVal + (totalSteps / 2);
	gx_pixelmap_slider_draw (slider);
	
	myVal = slider->gx_slider_info.gx_slider_info_current_val - (slider->gx_slider_info.gx_slider_info_max_val/2);
	sprintf_s (g_SliderValue, sizeof (g_SliderValue), "%2d%", myVal);

	gx_text_button_text_set (&VeerAdjustScreen.VeerAdjustScreen_SliderValue_Button, g_SliderValue);
}

UINT VeerAdjust_Screen_event_handler (GX_WINDOW *window, GX_EVENT *event_ptr)
{
	switch (event_ptr->gx_event_type)
	{
	case GX_SIGNAL(OK_BTN_ID, GX_EVENT_CLICKED):
        screen_toggle((GX_WINDOW *)&PerformanceSelectionScreen, window);
		break;

	case GX_EVENT_PEN_DOWN:
		break;

	case GX_EVENT_PEN_UP:
		break;
	}

    gx_window_event_process(window, event_ptr);

	return GX_SUCCESS;
}

//*************************************************************************************
// Function Name: UserSelectionScreen_event_process
//
// Description: This handles the User Settings Screen messages
//
//*************************************************************************************

UINT UserSelectionScreen_event_process (GX_WINDOW *window, GX_EVENT *event_ptr)
{
	switch (event_ptr->gx_event_type)
	{
	case GX_SIGNAL(OK_BTN_ID, GX_EVENT_CLICKED):
        screen_toggle((GX_WINDOW *)&HHP_Start_Screen, window);
		break;

	case GX_SIGNAL(USER_SETTINGS_BTN_ID, GX_EVENT_CLICKED):
        screen_toggle((GX_WINDOW *)&UserSettingsScreen, window);
		break;

	case GX_SIGNAL(FEATURE_BTN_ID, GX_EVENT_CLICKED):
        screen_toggle((GX_WINDOW *)&FeatureSettingsScreen, window);
		break;

	case GX_SIGNAL(PERFORMANCE_BTN_ID, GX_EVENT_CLICKED):
        screen_toggle((GX_WINDOW *)&PerformanceSelectionScreen, window);
		break;
	}

    gx_window_event_process(window, event_ptr);

	return GX_SUCCESS;
}

//*************************************************************************************
// Function Name: UserSettingsScreen_event_process
//
// Description: This handles the User Settings Screen messages
//
//*************************************************************************************

void ShowUserSettingsItems (void)
{
    char tmpChar[8];

    if (g_ClicksActive)
    {
		gx_button_select ((GX_BUTTON*) &UserSettingsScreen.UserSettingsScreen_ClicksToggleBtn);
    }

    // Power Up in Idle
    if (g_PowerUpInIdle)    // If powering up in idle state is enable
    {
		gx_button_select ((GX_BUTTON*) &UserSettingsScreen.UserSettingsScreen_PowerUpToggleBtn);
    }

    // RNet Enabled setting
    if (g_MainScreenFeatureInfo[RNET_ID].m_Enabled)
    {
		gx_button_select ((GX_BUTTON*) &UserSettingsScreen.UserSettingsScreen_RNET_ToggleBtn);
    }

    // Populate the Timeout button with the current setting or "OFF".
    if (g_TimeoutValue == 0)
        strcpy_s (g_TimeoutValueString, sizeof (g_TimeoutValueString), "OFF");
    else
    {
        // sprintf (g_TimeoutValueString, "%1.1g", (float) (g_TimeoutValue / 10.0f));
        // Floating point doesn't work for some odd reason.
        // I'm doing a hack to display the value in a X.X format.
        sprintf_s (g_TimeoutValueString, sizeof(g_TimeoutValueString), "%d.", g_TimeoutValue / 10);
        sprintf_s (tmpChar, sizeof (g_TimeoutValueString), "%d", g_TimeoutValue % 10);
        strcat_s (g_TimeoutValueString, sizeof (g_TimeoutValueString), tmpChar);
    }
    gx_text_button_text_set (&UserSettingsScreen.UserSettingsScreen_Timeout_Button, g_TimeoutValueString);
}

//*************************************************************************************

UINT UserSettingsScreen_event_process (GX_WINDOW *window, GX_EVENT *event_ptr)
{
	UINT myErr = -1;
    char tmpChar[8];

	switch (event_ptr->gx_event_type)
	{
	case GX_EVENT_SHOW:
		if (g_ClicksActive)
		{
			gx_button_select ((GX_BUTTON*) &UserSettingsScreen.UserSettingsScreen_ClicksToggleBtn);
		}
		// Power Up in Idle
		if (g_PowerUpInIdle)    // If powering up in idle state is enable
		{
			gx_button_select ((GX_BUTTON*) &UserSettingsScreen.UserSettingsScreen_PowerUpToggleBtn);
		}
		// RNet Enabled setting
		if (g_MainScreenFeatureInfo[RNET_ID].m_Enabled)
		{
			gx_button_select ((GX_BUTTON*) &UserSettingsScreen.UserSettingsScreen_RNET_ToggleBtn);
		}
		// Populate the Timeout button with the current setting or "OFF".
		if (g_TimeoutValue == 0)
			strcpy_s (g_TimeoutValueString, sizeof (g_TimeoutValueString), "OFF");
		else
		{
			// sprintf (g_TimeoutValueString, "%1.1g", (float) (g_TimeoutValue / 10.0f));
			// Floating point doesn't work for some odd reason.
			// I'm doing a hack to display the value in a X.X format.
			sprintf_s (g_TimeoutValueString, sizeof (g_TimeoutValueString), "%d.", g_TimeoutValue / 10);
			sprintf_s (tmpChar, sizeof (tmpChar), "%d", g_TimeoutValue % 10);
			strcat_s (g_TimeoutValueString, sizeof (g_TimeoutValueString), tmpChar);
		}
		gx_text_button_text_set (&UserSettingsScreen.UserSettingsScreen_Timeout_Button, g_TimeoutValueString);
		break;

	case GX_SIGNAL(OK_BTN_ID, GX_EVENT_CLICKED):
        screen_toggle((GX_WINDOW *)&UserSelectionScreen, window);
		break;

	//----------------------------------------------------------
	// CLICK toggle button processing
	case GX_SIGNAL(CLICKS_TOGGLE_BTN, GX_EVENT_TOGGLE_ON):
		g_ClicksActive = TRUE;
		break;
	case GX_SIGNAL(CLICKS_TOGGLE_BTN, GX_EVENT_TOGGLE_OFF):
		g_ClicksActive = FALSE;
		break;

	//----------------------------------------------------------
	// Power Up in IDLE toggle button
	case GX_SIGNAL(POWER_UP_TOGGLE_BTN, GX_EVENT_TOGGLE_ON):
		g_PowerUpInIdle = TRUE;
		break;
	case GX_SIGNAL(POWER_UP_TOGGLE_BTN, GX_EVENT_TOGGLE_OFF):
		g_PowerUpInIdle = FALSE;
		break;

	//----------------------------------------------------------
	// RNet Enable toggle button
	case GX_SIGNAL(RNET_TOGGLE_BTN, GX_EVENT_TOGGLE_ON):
		g_MainScreenFeatureInfo[RNET_ID].m_Enabled = TRUE;
		break;
	case GX_SIGNAL(RNET_TOGGLE_BTN, GX_EVENT_TOGGLE_OFF):
		g_MainScreenFeatureInfo[RNET_ID].m_Enabled = FALSE;
		break;

	//----------------------------------------------------------
	case GX_SIGNAL(TIMEOUT_BTN_ID, GX_EVENT_CLICKED):
		switch (g_TimeoutValue)
		{
			case 0:
				g_TimeoutValue = 10;
				break;
			case 10:
			case 15:
			case 20:
			case 25:
				g_TimeoutValue += 5;
				break;
			case 30:
			case 40:
				g_TimeoutValue += 10;
				break;
			case 50:
				g_TimeoutValue = 0;
				break;
		} // end switch
		if (g_TimeoutValue == 0)
			strcpy_s (g_TimeoutValueString, sizeof (g_TimeoutValueString), "OFF");
		else 
			sprintf_s (g_TimeoutValueString, sizeof(g_TimeoutValueString), "%1.1f", (float) (g_TimeoutValue / 10.0));
		gx_text_button_text_set (&UserSettingsScreen.UserSettingsScreen_Timeout_Button, g_TimeoutValueString);
		break;
	}

    //ShowUserSettingsItems();

    myErr = gx_window_event_process(window, event_ptr);

	gx_system_dirty_mark (window);

	return GX_SUCCESS;
}

//*************************************************************************************
// Function Name: FeatureSettingsScreen_event_process
//
// Description: This handles the Feature Settings Screen messages
//
//*************************************************************************************

UINT FeatureSettingsScreen_event_process (GX_WINDOW *window, GX_EVENT *event_ptr)
{
	UINT myErr = -1;
	int feature;
	int numActive;
	FEATURESETTINGSSCREEN_CONTROL_BLOCK *FeatureWindowPtr = (FEATURESETTINGSSCREEN_CONTROL_BLOCK*) window;

	switch (event_ptr->gx_event_type)
	{
		case GX_EVENT_SHOW:
			//CreateFeatureWidgets (&FeatureWindowPtr->FeatureSettingsScreen_FeatureListBox);
			break;

	    case GX_SIGNAL(FEATURE_LIST_BOX_ID, GX_EVENT_LIST_SELECT):
			for (myErr = 0; myErr < 5; ++myErr)
				feature = myErr;
	        break;

		case GX_SIGNAL(OK_BTN_ID, GX_EVENT_CLICKED):
			screen_toggle((GX_WINDOW *)&UserSelectionScreen, window);
			if (g_SettingsChanged)
			{
				numActive = 0;
				for (feature = 0; feature < 4; ++feature)
				{
					if (g_MainScreenFeatureInfo[feature].m_Enabled)
					{
						g_MainScreenFeatureInfo[feature].m_Location = numActive;
						++numActive;
					}
				}
			}
			break;

	} // end switch

    myErr = gx_window_event_process(window, event_ptr);

	return 0;
}

void CreateFeatureWidgets (GX_VERTICAL_LIST *list)
{
 	int index;

	for (index = 0; index < NUM_FEATURES; ++index)
	{
		FeatureList_callback (list, (GX_WIDGET*) &g_MainScreenFeatureInfo[index], index);
 	}
}

//*************************************************************************************
//*************************************************************************************
int g_MyCount = 0;

VOID FeatureList_callback(GX_VERTICAL_LIST *list, GX_WIDGET *widget, INT index)
{
    GX_RECTANGLE childsize;
    MAIN_SCREEN_FEATURE *feature = (MAIN_SCREEN_FEATURE *)widget;
    GX_BOOL result;
	UINT status;

	gx_widget_created_test(&feature->m_PromptWidget, &result);
    if (!result)
    {
        //gx_utility_rectangle_define(&childsize, 0, 5, 215, 35);
        //gx_prompt_create(&feature->m_PromptWidget, NULL, (GX_WIDGET *)list, 0, GX_STYLE_TRANSPARENT | GX_STYLE_BORDER_NONE | GX_STYLE_ENABLED, 0, &childsize);
        //gx_widget_fill_color_set(&feature->m_PromptWidget, GX_COLOR_ID_DARK_GRAY, GX_COLOR_ID_DARK_GRAY, GX_COLOR_ID_DARK_GRAY);

        gx_utility_rectangle_define(&childsize, 0, 0, 100, 52);
        gx_prompt_create(&feature->m_PromptWidget, NULL, (GX_WIDGET *)list, 0,GX_STYLE_TRANSPARENT | GX_STYLE_BORDER_THIN | GX_STYLE_ENABLED | GX_STYLE_TEXT_LEFT, 0, &childsize);
        gx_prompt_text_color_set(&feature->m_PromptWidget, GX_COLOR_ID_WHITE, GX_COLOR_ID_WHITE, GX_COLOR_ID_WHITE);
        gx_prompt_font_set(&feature->m_PromptWidget, GX_FONT_ID_PROMPT);

        gx_utility_rectangle_define(&childsize, 210-80, 8, 210-80+63, 37);
		status = gx_checkbox_create(&feature->m_ButtonWidget, NULL, (GX_WIDGET*)&feature->m_PromptWidget/*list*/, GX_STRING_ID_STRING_104, GX_STYLE_ENABLED | GX_STYLE_TRANSPARENT, FEATURE_TOGGLE_BUTTON_ID, &childsize);
		status = gx_checkbox_pixelmap_set (&feature->m_ButtonWidget, GX_PIXELMAP_ID_TOGGLE_GRAY, GX_PIXELMAP_ID_TOGGLE_GREEN, GX_PIXELMAP_ID_TOGGLE_GRAY, GX_PIXELMAP_ID_TOGGLE_GREEN);
		//gx_checkbox_draw (&feature->m_ButtonWidget);
	}

	gx_prompt_text_id_set(&feature->m_PromptWidget, feature->m_LargeDescriptionID);
}

//*************************************************************************************

#ifdef IF_USING_OLD_FEATURE

UINT FeatureSettingsScreen_event_process (GX_WINDOW *window, GX_EVENT *event_ptr)
{
	UINT myErr = -1;
	int feature;
	int numActive;
	FEATURESETTINGSSCREEN_CONTROL_BLOCK *FeatureWindowPtr = (FEATURESETTINGSSCREEN_CONTROL_BLOCK*) window;

	switch (event_ptr->gx_event_type)
	{
	case GX_EVENT_SHOW:
		// Show RNet or NEXT FUNCITON and NEXT PROFILE depending on RNet setting.
		// Adjust the displayed information based upon the RNet setting.
		// .. If RNet is enabled, the NEXT FUNCTION feature becomes RNet TOGGLE
		// .. and NEXT PROFILE feature become RNet MENU.
		if (g_MainScreenFeatureInfo[RNET_ID].m_Enabled)
		{
			// Display as "RNet TOGGLE"
			myErr = gx_prompt_text_id_set (&FeatureSettingsScreen.FeatureSettingsScreen_NextFunctionPrompt, GX_STRING_ID_RNET_TOGGLE);
			// Display as "RNET USER MENU"
			myErr = gx_prompt_text_id_set (&FeatureSettingsScreen.FeatureSettingsScreen_NextProfilePrompt, GX_STRING_ID_RNET_MENU);
		}
		else
		{
			// Display as NEXT FUNCTION
			myErr = gx_prompt_text_id_set (&FeatureSettingsScreen.FeatureSettingsScreen_NextFunctionPrompt, GX_STRING_ID_NEXT_FUNCTION);
			// Display as NEXT PROFILE
			myErr = gx_prompt_text_id_set (&FeatureSettingsScreen.FeatureSettingsScreen_NextProfilePrompt, GX_STRING_ID_NEXT_PROFILE);
		}

		// Power status
		if (g_MainScreenFeatureInfo[0].m_Enabled)
		{
			gx_button_select ((GX_BUTTON*) &FeatureSettingsScreen.FeatureSettingsScreen_PowerToggleBtn);
		}

		// Bluetooth
		if (g_MainScreenFeatureInfo[1].m_Enabled)
		{
			gx_button_select ((GX_BUTTON*) &FeatureSettingsScreen.FeatureSettingsScreen_BluetoothToggleBtn);
		}

		// Next Function
		if (g_MainScreenFeatureInfo[2].m_Enabled)
		{
			gx_button_select ((GX_BUTTON*) &FeatureSettingsScreen.FeatureSettingsScreen_NextFunctionToggleBtn);
		}

		// Next Profile
		if (g_MainScreenFeatureInfo[3].m_Enabled)
		{
			gx_button_select ((GX_BUTTON*) &FeatureSettingsScreen.FeatureSettingsScreen_NextProfileToggleBtn);
		}

		g_SettingsChanged = FALSE;
		break;

	case GX_SIGNAL(OK_BTN_ID, GX_EVENT_CLICKED):
        screen_toggle((GX_WINDOW *)&UserSelectionScreen, window);
		if (g_SettingsChanged)
		{
			numActive = 0;
			for (feature = 0; feature < 4; ++feature)
			{
				if (g_MainScreenFeatureInfo[feature].m_Enabled)
				{
					g_MainScreenFeatureInfo[feature].m_Location = numActive;
					++numActive;
				}
			}
		}
		break;

	//----------------------------------------------------------------------
	// Power Button
	case GX_SIGNAL(POWER_TOGGLE_BTN, GX_EVENT_TOGGLE_ON):
		g_MainScreenFeatureInfo[0].m_Enabled = TRUE;
		g_SettingsChanged = TRUE;
		break;
	case GX_SIGNAL(POWER_TOGGLE_BTN, GX_EVENT_TOGGLE_OFF):
		g_MainScreenFeatureInfo[0].m_Enabled = FALSE;
		g_SettingsChanged = TRUE;
		break;

	//----------------------------------------------------------------------
	// Bluetooth button
	case GX_SIGNAL(BLUETOOTH_TOGGLE_BTN, GX_EVENT_TOGGLE_ON):
		g_MainScreenFeatureInfo[1].m_Enabled = TRUE;
		g_SettingsChanged = TRUE;
		break;
	case GX_SIGNAL(BLUETOOTH_TOGGLE_BTN, GX_EVENT_TOGGLE_OFF):
		g_MainScreenFeatureInfo[1].m_Enabled = FALSE;
		g_SettingsChanged = TRUE;
		break;

	//----------------------------------------------------------------------
	// Next Function button
	case GX_SIGNAL(NEXT_FUNCTION_TOGGLE_BTN, GX_EVENT_TOGGLE_ON):
		g_MainScreenFeatureInfo[2].m_Enabled = TRUE;
		g_SettingsChanged = TRUE;
		break;
	case GX_SIGNAL(NEXT_FUNCTION_TOGGLE_BTN, GX_EVENT_TOGGLE_OFF):
		g_MainScreenFeatureInfo[2].m_Enabled = FALSE;
		g_SettingsChanged = TRUE;
		break;

	//----------------------------------------------------------------------
	// Next Profile Button
	case GX_SIGNAL(NEXT_PROFILE_TOGGLE_BTN, GX_EVENT_TOGGLE_ON):
		g_MainScreenFeatureInfo[3].m_Enabled = TRUE;
		g_SettingsChanged = TRUE;
		break;
	case GX_SIGNAL(NEXT_PROFILE_TOGGLE_BTN, GX_EVENT_TOGGLE_OFF):
		g_MainScreenFeatureInfo[3].m_Enabled = FALSE;
		g_SettingsChanged = TRUE;
		break;
	}

    myErr = gx_window_event_process(window, event_ptr);

	return 0;
}
#endif // IF_USING_OLD_FEATURE

//*************************************************************************************
// Function Name: SetPadTypeScreen_event_process
//
// Description: This handles the Set Pad Screen messages
//
//*************************************************************************************

UINT SetPadTypeScreen_event_process (GX_WINDOW *window, GX_EVENT *event_ptr)
{
	UINT myErr = -1;

	switch (event_ptr->gx_event_type)
	{
	case GX_EVENT_SHOW:
		g_ChangeScreen_WIP = FALSE;
		if (g_PadSettings[LEFT_PAD].m_PadType)	// Digital?
		{
			gx_widget_hide ((GX_WIDGET*) &SetPadTypeScreen.SetPadTypeScreen_LeftPadProportional_Button);
			gx_widget_show ((GX_WIDGET*) &SetPadTypeScreen.SetPadTypeScreen_LeftPadDigital_Button);
		}
		else
		{
			gx_widget_show ((GX_WIDGET*) &SetPadTypeScreen.SetPadTypeScreen_LeftPadProportional_Button);
			gx_widget_hide ((GX_WIDGET*) &SetPadTypeScreen.SetPadTypeScreen_LeftPadDigital_Button);
		}
		if (g_PadSettings[RIGHT_PAD].m_PadType)	// Digital?
		{
			gx_widget_hide ((GX_WIDGET*) &SetPadTypeScreen.SetPadTypeScreen_RightPadProportional_Button);
			gx_widget_show ((GX_WIDGET*) &SetPadTypeScreen.SetPadTypeScreen_RightPadDigital_Button);
		}
		else
		{
			gx_widget_show ((GX_WIDGET*) &SetPadTypeScreen.SetPadTypeScreen_RightPadProportional_Button);
			gx_widget_hide ((GX_WIDGET*) &SetPadTypeScreen.SetPadTypeScreen_RightPadDigital_Button);
		}
		if (g_PadSettings[CENTER_PAD].m_PadType)	// Digital?
		{
			gx_widget_hide ((GX_WIDGET*) &SetPadTypeScreen.SetPadTypeScreen_CenterPadProportional_Button);
			gx_widget_show ((GX_WIDGET*) &SetPadTypeScreen.SetPadTypeScreen_CenterPadDigital_Button);
		}
		else
		{
			gx_widget_show ((GX_WIDGET*) &SetPadTypeScreen.SetPadTypeScreen_CenterPadProportional_Button);
			gx_widget_hide ((GX_WIDGET*) &SetPadTypeScreen.SetPadTypeScreen_CenterPadDigital_Button);
		}
		break;
	case GX_SIGNAL(OK_BTN_ID, GX_EVENT_CLICKED):
        screen_toggle((GX_WINDOW *)&PadOptionsSettingsScreen, window);
		break;
	case GX_SIGNAL(RIGHT_PAD_DIGITAL_BTN_ID, GX_EVENT_CLICKED):
		if (!g_ChangeScreen_WIP)
		{
			myErr = gx_widget_hide ((GX_WIDGET*) &SetPadTypeScreen.SetPadTypeScreen_RightPadDigital_Button);
			myErr = gx_widget_show ((GX_WIDGET*) &SetPadTypeScreen.SetPadTypeScreen_RightPadProportional_Button);
			g_PadSettings[RIGHT_PAD].m_PadType = PROPORTIONAL_PADTYPE;
		}
		break;
	case GX_SIGNAL(RIGHT_PAD_PROPORTIONAL_BTN_ID, GX_EVENT_CLICKED):
		if (!g_ChangeScreen_WIP)
		{
			myErr = gx_widget_hide ((GX_WIDGET*) &SetPadTypeScreen.SetPadTypeScreen_RightPadProportional_Button);
			myErr = gx_widget_show ((GX_WIDGET*) &SetPadTypeScreen.SetPadTypeScreen_RightPadDigital_Button);
			g_PadSettings[RIGHT_PAD].m_PadType = DIGITAL_PADTYPE;
		}
		break;
	case GX_SIGNAL(LEFT_PAD_DIGITAL_BTN_ID, GX_EVENT_CLICKED):
		if (!g_ChangeScreen_WIP)
		{
			myErr = gx_widget_hide ((GX_WIDGET*) &SetPadTypeScreen.SetPadTypeScreen_LeftPadDigital_Button);
			myErr = gx_widget_show ((GX_WIDGET*) &SetPadTypeScreen.SetPadTypeScreen_LeftPadProportional_Button);
			g_PadSettings[LEFT_PAD].m_PadType = PROPORTIONAL_PADTYPE;
		}
		break;
	case GX_SIGNAL(LEFT_PAD_PROPORTIONAL_BTN_ID, GX_EVENT_CLICKED):
		if (!g_ChangeScreen_WIP)
		{
			myErr = gx_widget_hide ((GX_WIDGET*) &SetPadTypeScreen.SetPadTypeScreen_LeftPadProportional_Button);
			myErr = gx_widget_show ((GX_WIDGET*) &SetPadTypeScreen.SetPadTypeScreen_LeftPadDigital_Button);
			g_PadSettings[LEFT_PAD].m_PadType = DIGITAL_PADTYPE;
		}
		break;
	case GX_SIGNAL(CENTER_PAD_DIGITAL_BTN_ID, GX_EVENT_CLICKED):
		if (!g_ChangeScreen_WIP)
		{
			myErr = gx_widget_hide ((GX_WIDGET*) &SetPadTypeScreen.SetPadTypeScreen_CenterPadDigital_Button);
			myErr = gx_widget_show ((GX_WIDGET*) &SetPadTypeScreen.SetPadTypeScreen_CenterPadProportional_Button);
			g_PadSettings[CENTER_PAD].m_PadType = PROPORTIONAL_PADTYPE;
		}
		break;
	case GX_SIGNAL(CENTER_PAD_PROPORTIONAL_BTN_ID, GX_EVENT_CLICKED):
		if (!g_ChangeScreen_WIP)
		{
			myErr = gx_widget_hide ((GX_WIDGET*) &SetPadTypeScreen.SetPadTypeScreen_CenterPadProportional_Button);
			myErr = gx_widget_show ((GX_WIDGET*) &SetPadTypeScreen.SetPadTypeScreen_CenterPadDigital_Button);
			g_PadSettings[CENTER_PAD].m_PadType = DIGITAL_PADTYPE;
		}
		break;

	case GX_EVENT_TIMER:
        if (event_ptr->gx_event_payload.gx_event_timer_id == CALIBRATION_TIMER_ID)
		{
			gx_system_timer_stop(window, CALIBRATION_TIMER_ID);
	        screen_toggle((GX_WINDOW *)&PadCalibrationScreen, window);
			g_ChangeScreen_WIP = TRUE;
		}
		break;
	case GX_EVENT_PEN_DOWN:	// We are going to determine if the Up or Down arrow buttons have been held for a
							// ... long time (2 seconds) and goto calibration if so.

		if (event_ptr->gx_event_target->gx_widget_name == "CenterPadProportional_Button")
		{
			g_CalibrationPadNumber = CENTER_PAD; 
			gx_system_timer_start(window, CALIBRATION_TIMER_ID, 100, 0);
		}
		else if (event_ptr->gx_event_target->gx_widget_name == "LeftPadProportional_Button")
		{
			g_CalibrationPadNumber = LEFT_PAD; 
			gx_system_timer_start(window, CALIBRATION_TIMER_ID, 100, 0);
		}
		else if (event_ptr->gx_event_target->gx_widget_name == "RightPadProportional_Button")
		{
			g_CalibrationPadNumber = RIGHT_PAD; 
			gx_system_timer_start(window, CALIBRATION_TIMER_ID, 100, 0);
		}
		break;
	case GX_EVENT_PEN_UP:
			gx_system_timer_stop(window, CALIBRATION_TIMER_ID);
		break;

	}

    myErr = gx_window_event_process(window, event_ptr);

	return myErr;
}

//*************************************************************************************
// Function Name: CalibrationScreen_draw
//
// Description: This callback function is called when Drawing is required by GUIX.
//		This function draws the Guage.
//
//*************************************************************************************

VOID CalibrationScreen_draw (GX_WINDOW *window)
{
	GX_BRUSH *brush;
	GX_BRUSH originalBrush;
	UINT myErr;
	INT raw100, pieSide;

	gx_window_draw(window);

	myErr = gx_context_brush_get(&brush);
	originalBrush = *brush;

	// Draw the background
	brush->gx_brush_line_color = GX_COLOR_LIGHTGRAY;
	brush->gx_brush_width = 3;
	brush->gx_brush_fill_color = GX_COLOR_DARKGRAY;
	myErr = gx_canvas_pie_draw (GRAPH_CENTER_PT_XPOS, GRAPH_CENTER_PT_YPOS, 55, -5, 185);

	// Draw the Pad pie
	if (g_PadValue > 0)				// Anything less than 175-180 is too small of a pie to see; if it's 180 it draws a full circle.
	{
		raw100 = 100 - g_PadValue;
		raw100 *= 100;					// Integer math, yuch!
		raw100 *= 18;					// This converts the percentage to degrees which is a factor of 1.8 
		pieSide = raw100 / 1000;		// This is includes the decimal shift.
		brush->gx_brush_width = 2;
		brush->gx_brush_fill_color = GX_COLOR_GREEN;
		brush->gx_brush_line_color = GX_COLOR_GREEN;
		myErr = gx_canvas_pie_draw (GRAPH_CENTER_PT_XPOS, GRAPH_CENTER_PT_YPOS, 54, pieSide, 180);
	}

	// Draw the minimum pie
	// Calculate the position of the upper side of the pie.
	// The arc is drawn as follows:
	//		 0 degrees = 3:00 clock time
	//		90 degrees = 12:00 noon clock time
	//		180 degrees = 9:00 clock time
	//		270 degrees = 6:00 clock time.
	raw100 = 100 - g_PadSettings[g_CalibrationPadNumber].m_PadMinimumCalibrationValue;
	raw100 *= 100;					// Integer math, yuch!
	raw100 *= 18;					// This converts the percentage to degrees which is a factor of 1.8 
	pieSide = raw100 / 1000;		// This is includes the decimal shift.
	if (pieSide > 175)				// Anything less than 175-180 is too small of a pie to see.
		pieSide = 175;
	brush->gx_brush_fill_color = GX_COLOR_YELLOW;	// Draw in yellow.
	brush->gx_brush_line_color = GX_COLOR_YELLOW;
	//brush->gx_brush_style = GX_BRUSH_OUTLINE;
	brush->gx_brush_width = 1;
	myErr = gx_context_brush_set(brush);		// Not really required. It seems to change the color to yellow without this call.
	myErr = gx_canvas_pie_draw (GRAPH_CENTER_PT_XPOS, GRAPH_CENTER_PT_YPOS, 40, pieSide, 180);

	// Draw the Maximum Pie
	raw100 = 100 - g_PadSettings[g_CalibrationPadNumber].m_PadMaximumCalibrationValue;		// Integer math, yuch!
	raw100 *= 100;
	raw100 *= 18;					// This converts the percentage to degrees which is a factor of 1.8 
	pieSide = raw100 / 1000;		// This is includes the decimal shift.
	if (pieSide < 5)						// Anything less than 0-5 is too small of a sliver to see.
		pieSide = 5;
	brush->gx_brush_fill_color = 0xff6a00;	// Orange
	brush->gx_brush_line_color = 0xff6a00;
	myErr = gx_context_brush_set(brush);		// Not really required. It seems to change the color to yellow without this call.
	myErr = gx_canvas_pie_draw (GRAPH_CENTER_PT_XPOS, GRAPH_CENTER_PT_YPOS, 40, 0, pieSide);

	*brush = originalBrush;
}

//*************************************************************************************
// Function Name: CalibrationScreen_event_process
//
// Description: This handles the Set Pad Screen messages
//
//*************************************************************************************

UINT CalibrationScreen_event_process (GX_WINDOW *window, GX_EVENT *event_ptr)
{
	UINT myErr;

	switch (event_ptr->gx_event_type)
	{
	case GX_EVENT_SHOW:
		myErr = gx_widget_resize ((GX_WIDGET*) &PadCalibrationScreen.PadCalibrationScreen_MaximumInstructionsText, &g_HiddenRectangle);
		myErr = gx_widget_resize ((GX_WIDGET*) &PadCalibrationScreen.PadCalibrationScreen_MinimumInstructionsText, &g_CalibrationPromptLocations[0]);
		g_CalibrationStepNumber = 0;

		myErr = gx_numeric_prompt_value_set (&PadCalibrationScreen.PadCalibrationScreen_Value_Prompt, g_PadSettings[g_CalibrationPadNumber].m_PadMinimumCalibrationValue);

		// show the PAD under calibration and hide the other PADs.
		myErr = gx_widget_resize ((GX_WIDGET*) &PadCalibrationScreen.PadCalibrationScreen_CenterPadON_Button, &g_HiddenRectangle);
		myErr = gx_widget_resize ((GX_WIDGET*) &PadCalibrationScreen.PadCalibrationScreen_LeftPadON_Button, &g_HiddenRectangle);
		myErr = gx_widget_resize ((GX_WIDGET*) &PadCalibrationScreen.PadCalibrationScreen_RightPadON_Button, &g_HiddenRectangle);
		switch (g_CalibrationPadNumber)
		{
		case LEFT_PAD:
			myErr = gx_widget_resize ((GX_WIDGET*) &PadCalibrationScreen.PadCalibrationScreen_LeftPadON_Button, &g_CalibrationPadLocations[0]);
			break;
		case RIGHT_PAD:
			myErr = gx_widget_resize ((GX_WIDGET*) &PadCalibrationScreen.PadCalibrationScreen_RightPadON_Button, &g_CalibrationPadLocations[1]);
			break;
		case CENTER_PAD:
			myErr = gx_widget_resize ((GX_WIDGET*) &PadCalibrationScreen.PadCalibrationScreen_CenterPadON_Button, &g_CalibrationPadLocations[2]);
			break;
		} // end switch (g_CalibrationPadNumber)

		g_CalibrationScreen = window;		// Store for use by screen update process.
		break;

	case GX_SIGNAL(OK_BTN_ID, GX_EVENT_CLICKED):
		if (g_CalibrationStepNumber == 0)			// Let's do maximum calibration
		{
			myErr = gx_widget_resize ((GX_WIDGET*) &PadCalibrationScreen.PadCalibrationScreen_MaximumInstructionsText, &g_CalibrationPromptLocations[0]);
			myErr = gx_widget_resize ((GX_WIDGET*) &PadCalibrationScreen.PadCalibrationScreen_MinimumInstructionsText, &g_HiddenRectangle);
			myErr = gx_numeric_prompt_value_set (&PadCalibrationScreen.PadCalibrationScreen_Value_Prompt, g_PadSettings[g_CalibrationPadNumber].m_PadMaximumCalibrationValue);
			++g_CalibrationStepNumber;
			g_PadValue = 85;
		}
		else if (g_CalibrationStepNumber == 1)
		{
	        screen_toggle((GX_WINDOW *)&SetPadTypeScreen, window);
		}
		break;

	}
    myErr = gx_window_event_process(window, event_ptr);

	return GX_SUCCESS;
}

//*************************************************************************************
// Function Name: SetPadDirectionScreen_event_process
//
// Description: This functions process the event of the Set Pad Direction screen.
//
//*************************************************************************************

UINT SetPadDirectionScreen_event_process (GX_WINDOW *window, GX_EVENT *event_ptr)
{
	UINT myErr;
	UINT pads, icons;
	switch (event_ptr->gx_event_type)
	{
	case GX_EVENT_SHOW:
		// Show correct settings for LEFT pad. Off, Right, Forward or Left.
		// First let's hide all choices for all pads.
		for (pads = 0; pads < 3; ++pads)
		{
			for (icons = 0; icons < 4; ++icons)
			{
				myErr = gx_widget_resize ((GX_WIDGET*) g_PadSettings[pads].m_DirectionIcons[icons], &g_HiddenRectangle);
			}
			myErr = gx_widget_resize ((GX_WIDGET*) g_PadSettings[pads].m_DirectionIcons[g_PadSettings[pads].m_PadDirection], &g_PadDirectionLocation[pads]);
		}
		break;

	case GX_SIGNAL(OK_BTN_ID, GX_EVENT_CLICKED):
        screen_toggle((GX_WINDOW *)&PadOptionsSettingsScreen, window);
		break;
	// Process LEFT button pushes
	case GX_SIGNAL(LEFT_PAD_OFF_BTN_ID, GX_EVENT_CLICKED):
		myErr = gx_widget_resize ((GX_WIDGET*) g_PadSettings[LEFT_PAD].m_DirectionIcons[OFF_DIRECTION], &g_HiddenRectangle);
		myErr = gx_widget_resize ((GX_WIDGET*) g_PadSettings[LEFT_PAD].m_DirectionIcons[LEFT_DIRECTION], &g_PadDirectionLocation[LEFT_PAD]);
		g_PadSettings[LEFT_PAD].m_PadDirection = LEFT_DIRECTION;
		break;
	case GX_SIGNAL(LEFT_PAD_LEFT_ARROW_BTN_ID, GX_EVENT_CLICKED):
		myErr = gx_widget_resize ((GX_WIDGET*) g_PadSettings[LEFT_PAD].m_DirectionIcons[LEFT_DIRECTION], &g_HiddenRectangle);
		myErr = gx_widget_resize ((GX_WIDGET*) g_PadSettings[LEFT_PAD].m_DirectionIcons[FORWARD_DIRECTION], &g_PadDirectionLocation[LEFT_PAD]);
		g_PadSettings[LEFT_PAD].m_PadDirection = FORWARD_DIRECTION;
		break;
	case GX_SIGNAL(LEFT_PAD_FORWARD_ARROW_BTN_ID, GX_EVENT_CLICKED):
		myErr = gx_widget_resize ((GX_WIDGET*) g_PadSettings[LEFT_PAD].m_DirectionIcons[FORWARD_DIRECTION], &g_HiddenRectangle);
		myErr = gx_widget_resize ((GX_WIDGET*) g_PadSettings[LEFT_PAD].m_DirectionIcons[RIGHT_DIRECTION], &g_PadDirectionLocation[LEFT_PAD]);
		g_PadSettings[LEFT_PAD].m_PadDirection = RIGHT_DIRECTION;
		break;
	case GX_SIGNAL(LEFT_PAD_RIGHT_ARROW_BTN_ID, GX_EVENT_CLICKED):
		myErr = gx_widget_resize ((GX_WIDGET*) g_PadSettings[LEFT_PAD].m_DirectionIcons[RIGHT_DIRECTION], &g_HiddenRectangle);
		myErr = gx_widget_resize ((GX_WIDGET*) g_PadSettings[LEFT_PAD].m_DirectionIcons[OFF_DIRECTION], &g_PadDirectionLocation[LEFT_PAD]);
		g_PadSettings[LEFT_PAD].m_PadDirection = OFF_DIRECTION;
		break;
	// Process RIGHT button pushes
	case GX_SIGNAL(RIGHT_PAD_OFF_BTN_ID, GX_EVENT_CLICKED):
		myErr = gx_widget_resize ((GX_WIDGET*) g_PadSettings[RIGHT_PAD].m_DirectionIcons[OFF_DIRECTION], &g_HiddenRectangle);
		myErr = gx_widget_resize ((GX_WIDGET*) g_PadSettings[RIGHT_PAD].m_DirectionIcons[LEFT_DIRECTION], &g_PadDirectionLocation[RIGHT_PAD]);
		g_PadSettings[RIGHT_PAD].m_PadDirection = LEFT_DIRECTION;
		break;
	case GX_SIGNAL(RIGHT_PAD_LEFT_ARROW_BTN_ID, GX_EVENT_CLICKED):
		myErr = gx_widget_resize ((GX_WIDGET*) g_PadSettings[RIGHT_PAD].m_DirectionIcons[LEFT_DIRECTION], &g_HiddenRectangle);
		myErr = gx_widget_resize ((GX_WIDGET*) g_PadSettings[RIGHT_PAD].m_DirectionIcons[FORWARD_DIRECTION], &g_PadDirectionLocation[RIGHT_PAD]);
		g_PadSettings[RIGHT_PAD].m_PadDirection = FORWARD_DIRECTION;
		break;
	case GX_SIGNAL(RIGHT_PAD_FORWARD_ARROW_BTN_ID, GX_EVENT_CLICKED):
		myErr = gx_widget_resize ((GX_WIDGET*) g_PadSettings[RIGHT_PAD].m_DirectionIcons[FORWARD_DIRECTION], &g_HiddenRectangle);
		myErr = gx_widget_resize ((GX_WIDGET*) g_PadSettings[RIGHT_PAD].m_DirectionIcons[RIGHT_DIRECTION], &g_PadDirectionLocation[RIGHT_PAD]);
		g_PadSettings[RIGHT_PAD].m_PadDirection = RIGHT_DIRECTION;
		break;
	case GX_SIGNAL(RIGHT_PAD_RIGHT_ARROW_BTN_ID, GX_EVENT_CLICKED):
		myErr = gx_widget_resize ((GX_WIDGET*) g_PadSettings[RIGHT_PAD].m_DirectionIcons[RIGHT_DIRECTION], &g_HiddenRectangle);
		myErr = gx_widget_resize ((GX_WIDGET*) g_PadSettings[RIGHT_PAD].m_DirectionIcons[OFF_DIRECTION], &g_PadDirectionLocation[RIGHT_PAD]);
		g_PadSettings[RIGHT_PAD].m_PadDirection = OFF_DIRECTION;
		break;
	// Process CENTER PAD button pushes
	case GX_SIGNAL(CENTER_PAD_OFF_BTN_ID, GX_EVENT_CLICKED):
		myErr = gx_widget_resize ((GX_WIDGET*) g_PadSettings[CENTER_PAD].m_DirectionIcons[OFF_DIRECTION], &g_HiddenRectangle);
		myErr = gx_widget_resize ((GX_WIDGET*) g_PadSettings[CENTER_PAD].m_DirectionIcons[LEFT_DIRECTION], &g_PadDirectionLocation[CENTER_PAD]);
		g_PadSettings[CENTER_PAD].m_PadDirection = LEFT_DIRECTION;
		break;
	case GX_SIGNAL(CENTER_PAD_LEFT_ARROW_BTN_ID, GX_EVENT_CLICKED):
		myErr = gx_widget_resize ((GX_WIDGET*) g_PadSettings[CENTER_PAD].m_DirectionIcons[LEFT_DIRECTION], &g_HiddenRectangle);
		myErr = gx_widget_resize ((GX_WIDGET*) g_PadSettings[CENTER_PAD].m_DirectionIcons[FORWARD_DIRECTION], &g_PadDirectionLocation[CENTER_PAD]);
		g_PadSettings[CENTER_PAD].m_PadDirection = FORWARD_DIRECTION;
		break;
	case GX_SIGNAL(CENTER_PAD_FORWARD_ARROW_BTN_ID, GX_EVENT_CLICKED):
		myErr = gx_widget_resize ((GX_WIDGET*) g_PadSettings[CENTER_PAD].m_DirectionIcons[FORWARD_DIRECTION], &g_HiddenRectangle);
		myErr = gx_widget_resize ((GX_WIDGET*) g_PadSettings[CENTER_PAD].m_DirectionIcons[RIGHT_DIRECTION], &g_PadDirectionLocation[CENTER_PAD]);
		g_PadSettings[CENTER_PAD].m_PadDirection = RIGHT_DIRECTION;
		break;
	case GX_SIGNAL(CENTER_PAD_RIGHT_ARROW_BTN_ID, GX_EVENT_CLICKED):
		myErr = gx_widget_resize ((GX_WIDGET*) g_PadSettings[CENTER_PAD].m_DirectionIcons[RIGHT_DIRECTION], &g_HiddenRectangle);
		myErr = gx_widget_resize ((GX_WIDGET*) g_PadSettings[CENTER_PAD].m_DirectionIcons[OFF_DIRECTION], &g_PadDirectionLocation[CENTER_PAD]);
		g_PadSettings[CENTER_PAD].m_PadDirection = OFF_DIRECTION;
		break;

	case GX_EVENT_TIMER:
        if (event_ptr->gx_event_payload.gx_event_timer_id == PAD_ADVANCE_TIMER_ID)
		{
			gx_system_timer_stop(window, PAD_ADVANCE_TIMER_ID);
	        screen_toggle((GX_WINDOW *)&AdvancePadSettingsScreen, window);
		}
		break;

	case GX_EVENT_PEN_DOWN:	// We are going to determine if the Diretion Buttons have been held for a
							// ... long time (2 seconds) and goto calibration if so.

		if ((event_ptr->gx_event_target->gx_widget_name == "CenterPad_ForwardArrow_Button")
			|| (event_ptr->gx_event_target->gx_widget_name == "CenterPad_LeftArrow_Button")
			|| (event_ptr->gx_event_target->gx_widget_name == "CenterPad_Off_Button")
			|| (event_ptr->gx_event_target->gx_widget_name == "CenterPad_RightArrow_Button"))
		{
			g_AdvancePadNumber = CENTER_PAD; 
			gx_system_timer_start(window, PAD_ADVANCE_TIMER_ID, 100, 0);
		}
		else if ((event_ptr->gx_event_target->gx_widget_name == "LeftPad_ForwardArrow_Button")
			|| (event_ptr->gx_event_target->gx_widget_name == "LeftPad_LeftArrow_Button")
			|| (event_ptr->gx_event_target->gx_widget_name == "LeftPad_Off_Button")
			|| (event_ptr->gx_event_target->gx_widget_name == "LeftPad_RightArrow_Button"))
		{
			g_AdvancePadNumber = LEFT_PAD; 
			gx_system_timer_start(window, PAD_ADVANCE_TIMER_ID, 100, 0);
		}
		else if ((event_ptr->gx_event_target->gx_widget_name == "RightPad_ForwardArrow_Button")
			|| (event_ptr->gx_event_target->gx_widget_name == "RightPad_LeftArrow_Button")
			|| (event_ptr->gx_event_target->gx_widget_name == "RightPad_Off_Button")
			|| (event_ptr->gx_event_target->gx_widget_name == "RightPad_RightArrow_Button"))
		{
			g_AdvancePadNumber = RIGHT_PAD; 
			gx_system_timer_start(window, PAD_ADVANCE_TIMER_ID, 100, 0);
		}
		break;
	case GX_EVENT_PEN_UP:
			gx_system_timer_stop(window, PAD_ADVANCE_TIMER_ID);
		break;


	}

	myErr = gx_window_event_process(window, event_ptr);

	return GX_SUCCESS;
}

//*************************************************************************************
// Function Name: MoreSelectionScreen_event_process
//
// Description: This functions handles the More Selection screen and dispatches
//		to the Diagnostic Screen or the Reset System screen.
//
//*************************************************************************************

UINT MoreSelectionScreen_event_process(GX_WINDOW *window, GX_EVENT *event_ptr)
{
    switch (event_ptr->gx_event_type)
    {
	case GX_EVENT_SHOW:
        gx_prompt_text_set ((GX_PROMPT*)&MoreSelectionScreen.MoreSelectionScreen_VersionPrompt, ASL110_DISPLAY_VERSION_STRING);
        gx_prompt_text_set ((GX_PROMPT*)&MoreSelectionScreen.MoreSelectionScreen_HeadArray_VersionPrompt, g_HeadArrayVersionString);
		break;

	case GX_SIGNAL(GOTO_DIAGNOSTICS_BTN_ID, GX_EVENT_CLICKED):
        screen_toggle((GX_WINDOW *)&DiagnosticScreen, window);
		break;

	case GX_SIGNAL(GOTO_RESET_SCREEN_BTN_ID, GX_EVENT_CLICKED):
        screen_toggle((GX_WINDOW *)&ResetScreen, window);
		break;

	case GX_SIGNAL(OK_BTN_ID, GX_EVENT_CLICKED):
        screen_toggle((GX_WINDOW *)&HHP_Start_Screen, window);
		break;

    } // end switch
    return gx_window_event_process(window, event_ptr);
}


//*************************************************************************************
// Function Name: ShowHidePad
//
// Description: This functions displays an green-illuminated pad simulating an active pad.
//
//*************************************************************************************

UINT ShowHidePad (GX_EVENT *event_ptr)
{
	UINT myError = GX_SUCCESS;
	int pad;
//	GX_WIDGET *myWidget;
	GX_VALUE xPos, yPos;

	// We can only look at the PEN UP and PEN DOWN events and it only provides 
	if (event_ptr->gx_event_target->gx_widget_first_child == NULL)
		return GX_FAILURE;
	// We are going to identify the "press" pad by its location.
	xPos = event_ptr->gx_event_payload.gx_event_pointdata.gx_point_x;
	yPos = event_ptr->gx_event_payload.gx_event_pointdata.gx_point_y;

	// Now traverse the entire set of widgets for this window looking for the the "Pressed" icon.
	for (pad = 0; pad <= 2; ++pad)
	{
		if ((yPos >= g_PadSettings[pad].m_DiagnosticWidigetLocation.gx_rectangle_top)
			&& (yPos <= g_PadSettings[pad].m_DiagnosticWidigetLocation.gx_rectangle_bottom)
			&& (xPos >= g_PadSettings[pad].m_DiagnosticWidigetLocation.gx_rectangle_left)
			&& (xPos <= g_PadSettings[pad].m_DiagnosticWidigetLocation.gx_rectangle_right))
		// If we got the right pad, then do something.
		if (g_PadSettings[pad].m_PadDirection != OFF_DIRECTION)
		{
			// Determine if we are going to show it or hide it.
			if (event_ptr->gx_event_type == GX_EVENT_PEN_DOWN)
			{
				// Determine if we show the Proportional (Orange) or Digital (Green)
				if (g_PadSettings[pad].m_PadType == PROPORTIONAL_PADTYPE)
					myError = gx_widget_resize ((GX_WIDGET*)g_PadSettings[pad].m_DiagnosticProportional_Widget , &g_PadSettings[pad].m_DiagnosticWidigetLocation);
				else
					myError = gx_widget_resize ((GX_WIDGET*)g_PadSettings[pad].m_DiagnosticDigital_Widget , &g_PadSettings[pad].m_DiagnosticWidigetLocation);
			}
			else // PEN UP
			{
				// Now we are going to hide it.
				if (g_PadSettings[pad].m_PadType == PROPORTIONAL_PADTYPE)
					myError = gx_widget_resize ((GX_WIDGET*)g_PadSettings[pad].m_DiagnosticProportional_Widget , &g_HiddenRectangle);
				else
					myError = gx_widget_resize ((GX_WIDGET*)g_PadSettings[pad].m_DiagnosticDigital_Widget , &g_HiddenRectangle);
			}
		} // endif !OFF
	} // end while
	return (myError);
}

//*************************************************************************************
// Function Name: DiagnosticScreen_event_handler
//
// Description: This handles the Diagnostic Screen messages
//
//*************************************************************************************

UINT DiagnosticScreen_event_handler(GX_WINDOW *window, GX_EVENT *event_ptr)
{
	UINT myError;
	int pads;

    switch (event_ptr->gx_event_type)
    {
	case GX_EVENT_SHOW:
		for (pads = 0; pads < 3; ++pads)
		{
			myError = gx_widget_resize ((GX_WIDGET*) g_PadSettings[pads].m_DiagnosticDigital_Widget, &g_HiddenRectangle);
			myError = gx_widget_resize ((GX_WIDGET*) g_PadSettings[pads].m_DiagnosticProportional_Widget, &g_HiddenRectangle);
			if (g_PadSettings[pads].m_PadDirection == OFF_DIRECTION)
				myError = gx_widget_resize ((GX_WIDGET*)g_PadSettings[pads].m_DiagnosticOff_Widget, &g_PadSettings[pads].m_DiagnosticWidigetLocation);
			else
				myError = gx_widget_resize ((GX_WIDGET*)g_PadSettings[pads].m_DiagnosticOff_Widget, &g_HiddenRectangle);
		}
		break;

	case GX_SIGNAL(OK_BTN_ID, GX_EVENT_CLICKED):
        screen_toggle((GX_WINDOW *)&MoreSelectionScreen, window);
		break;

	case GX_EVENT_PEN_DOWN:
	case GX_EVENT_PEN_UP:
		ShowHidePad (event_ptr);
		break;
    } // end switch
    return gx_window_event_process(window, event_ptr);
}

//*************************************************************************************
// Function Name: ResetScreen_event_process
//
// Description: This handles the Diagnostic Screen messages
//
//*************************************************************************************

UINT ResetScreen_event_process(GX_WINDOW *window, GX_EVENT *event_ptr)
{

    switch (event_ptr->gx_event_type)
    {
	case GX_SIGNAL(OK_BTN_ID, GX_EVENT_CLICKED):
        screen_toggle((GX_WINDOW *)&MoreSelectionScreen, window);
		break;

	case GX_SIGNAL(CONTINUE_BTN_ID, GX_EVENT_CLICKED):
        screen_toggle((GX_WINDOW *)&ResetFinishScreen, window);
		break;
    } // end switch
    return gx_window_event_process(window, event_ptr);
}

//*************************************************************************************

UINT ResetFinishScreen_event_process(GX_WINDOW *window, GX_EVENT *event_ptr)
{
	switch (event_ptr->gx_event_type)
    {
	case GX_SIGNAL(OK_BTN_ID, GX_EVENT_CLICKED):
        screen_toggle((GX_WINDOW *)&HHP_Start_Screen, window);
		break;

    } // end switch
    return gx_window_event_process(window, event_ptr);
}

