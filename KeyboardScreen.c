//*****************************************************************************
// Filename: KeyboardScreen.cpp
//
// Date: Mar 18, 2021
//
// Author: G. Chopcinski, Kg Solutions, LLC
// 
//*****************************************************************************

#include "ASL4321_System.h"
#include "asl4321_display_demo_resources.h"

static char g_Keyboard_Data[32];
static char *g_ValuePtr;
static GX_WINDOW *g_ReturnWindow;
static UINT g_StringLength = 0;

//***********************************************************************************
VOID SetReturnWindow (GX_WINDOW *window)
{
	g_ReturnWindow = window;
}

//***********************************************************************************
// Get and Set keyboard Dat provides the Keyboard user with methods to
// set and get the keyboard information.

VOID SetKeyboardData (char *str, UINT length)
{
	g_ValuePtr = str;
	strcpy_s (g_Keyboard_Data, sizeof (g_Keyboard_Data), g_ValuePtr); // *str);
	g_StringLength = length;
}

VOID GetKeyboardData (char *str)
{
	strcpy_s (str, sizeof (g_StringLength), g_Keyboard_Data);
}

//***********************************************************************************
// Append passed Letter to string
//***********************************************************************************

UINT AppendKeyboardValue (GX_CHAR chr)
{
	UINT myErr;
	GX_CHAR *myStringPtr;
	UINT bufferSize, contentSize;

	myErr = gx_single_line_text_input_buffer_get (&KeyboardScreen.KeyboardScreen_Keyboard_InputField, &myStringPtr, &bufferSize, &contentSize);
	if (bufferSize < (g_StringLength-1))
		gx_single_line_text_input_character_insert (&KeyboardScreen.KeyboardScreen_Keyboard_InputField, &chr, 1);

	return myErr;
}

UINT KeyboardScreen_event_function (GX_WINDOW *window, GX_EVENT *event_ptr)
{
	UINT myErr;
	GX_CHAR *myStringPtr;
	UINT bufferSize, contentSize;

	switch (event_ptr->gx_event_type)
	{
	case GX_EVENT_SHOW:
		myErr = gx_single_line_text_input_text_set(&KeyboardScreen.KeyboardScreen_Keyboard_InputField, g_Keyboard_Data);
	    gx_single_line_text_input_style_set(&KeyboardScreen.KeyboardScreen_Keyboard_InputField, GX_STYLE_ENABLED | GX_STYLE_CURSOR_BLINK | GX_STYLE_CURSOR_ALWAYS);
		gx_single_line_text_input_end (&KeyboardScreen.KeyboardScreen_Keyboard_InputField);
		break;

	case GX_SIGNAL(BACKSPACE_BTN_ID, GX_EVENT_CLICKED):
		gx_single_line_text_input_backspace (&KeyboardScreen.KeyboardScreen_Keyboard_InputField);
		break;

	case GX_SIGNAL(KEY_1_BTN_ID, GX_EVENT_CLICKED): AppendKeyboardValue('1'); break;
	case GX_SIGNAL(KEY_2_BTN_ID, GX_EVENT_CLICKED): AppendKeyboardValue('2'); break;
	case GX_SIGNAL(KEY_3_BTN_ID, GX_EVENT_CLICKED): AppendKeyboardValue('3'); break;
	case GX_SIGNAL(KEY_4_BTN_ID, GX_EVENT_CLICKED): AppendKeyboardValue('4'); break;
	case GX_SIGNAL(KEY_5_BTN_ID, GX_EVENT_CLICKED): AppendKeyboardValue('5'); break;
	case GX_SIGNAL(KEY_6_BTN_ID, GX_EVENT_CLICKED): AppendKeyboardValue('6'); break;
	case GX_SIGNAL(KEY_7_BTN_ID, GX_EVENT_CLICKED): AppendKeyboardValue('7'); break;
	case GX_SIGNAL(KEY_8_BTN_ID, GX_EVENT_CLICKED): AppendKeyboardValue('8'); break;
	case GX_SIGNAL(KEY_9_BTN_ID, GX_EVENT_CLICKED): AppendKeyboardValue('9'); break;
	case GX_SIGNAL(KEY_0_BTN_ID, GX_EVENT_CLICKED): AppendKeyboardValue('0'); break;
	case GX_SIGNAL(KEY_A_BTN_ID, GX_EVENT_CLICKED): AppendKeyboardValue('A'); break;
	case GX_SIGNAL(KEY_B_BTN_ID, GX_EVENT_CLICKED): AppendKeyboardValue('B'); break;
	case GX_SIGNAL(KEY_C_BTN_ID, GX_EVENT_CLICKED): AppendKeyboardValue('C'); break;
	case GX_SIGNAL(KEY_D_BTN_ID, GX_EVENT_CLICKED): AppendKeyboardValue('D'); break;
	case GX_SIGNAL(KEY_E_BTN_ID, GX_EVENT_CLICKED): AppendKeyboardValue('E'); break;
	case GX_SIGNAL(KEY_F_BTN_ID, GX_EVENT_CLICKED): AppendKeyboardValue('F'); break;
	case GX_SIGNAL(KEY_G_BTN_ID, GX_EVENT_CLICKED): AppendKeyboardValue('G'); break;
	case GX_SIGNAL(KEY_H_BTN_ID, GX_EVENT_CLICKED): AppendKeyboardValue('H'); break;
	case GX_SIGNAL(KEY_I_BTN_ID, GX_EVENT_CLICKED): AppendKeyboardValue('I'); break;
	case GX_SIGNAL(KEY_J_BTN_ID, GX_EVENT_CLICKED): AppendKeyboardValue('J'); break;
	case GX_SIGNAL(KEY_K_BTN_ID, GX_EVENT_CLICKED): AppendKeyboardValue('K'); break;
	case GX_SIGNAL(KEY_L_BTN_ID, GX_EVENT_CLICKED): AppendKeyboardValue('L'); break;
	case GX_SIGNAL(KEY_M_BTN_ID, GX_EVENT_CLICKED): AppendKeyboardValue('M'); break;
	case GX_SIGNAL(KEY_N_BTN_ID, GX_EVENT_CLICKED): AppendKeyboardValue('N'); break;
	case GX_SIGNAL(KEY_O_BTN_ID, GX_EVENT_CLICKED): AppendKeyboardValue('O'); break;
	case GX_SIGNAL(KEY_P_BTN_ID, GX_EVENT_CLICKED): AppendKeyboardValue('P'); break;
	case GX_SIGNAL(KEY_Q_BTN_ID, GX_EVENT_CLICKED): AppendKeyboardValue('Q'); break;
	case GX_SIGNAL(KEY_R_BTN_ID, GX_EVENT_CLICKED): AppendKeyboardValue('R'); break;
	case GX_SIGNAL(KEY_S_BTN_ID, GX_EVENT_CLICKED): AppendKeyboardValue('S'); break;
	case GX_SIGNAL(KEY_T_BTN_ID, GX_EVENT_CLICKED): AppendKeyboardValue('T'); break;
	case GX_SIGNAL(KEY_U_BTN_ID, GX_EVENT_CLICKED): AppendKeyboardValue('U'); break;
	case GX_SIGNAL(KEY_V_BTN_ID, GX_EVENT_CLICKED): AppendKeyboardValue('V'); break;
	case GX_SIGNAL(KEY_W_BTN_ID, GX_EVENT_CLICKED): AppendKeyboardValue('W'); break;
	case GX_SIGNAL(KEY_X_BTN_ID, GX_EVENT_CLICKED): AppendKeyboardValue('X'); break;
	case GX_SIGNAL(KEY_Y_BTN_ID, GX_EVENT_CLICKED): AppendKeyboardValue('Y'); break;
	case GX_SIGNAL(KEY_Z_BTN_ID, GX_EVENT_CLICKED): AppendKeyboardValue('Z'); break;
	case GX_SIGNAL(KEY_SPACE_BTN_ID, GX_EVENT_CLICKED): AppendKeyboardValue(' '); break;

	case GX_SIGNAL(OK_BTN_ID, GX_EVENT_CLICKED):
		myErr = gx_single_line_text_input_buffer_get (&KeyboardScreen.KeyboardScreen_Keyboard_InputField, &myStringPtr, &bufferSize, &contentSize);
		strcpy_s (g_ValuePtr, g_StringLength, myStringPtr);
        screen_toggle(g_ReturnWindow, window);
		break;
	}

    myErr = gx_window_event_process(window, event_ptr);

	return GX_SUCCESS;
}

