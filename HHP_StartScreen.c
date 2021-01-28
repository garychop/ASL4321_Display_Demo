//*****************************************************************************
// Filename: HHP_StartScreen.cpp
// Description: This file supports the Programming Main Screen.
//
// Date: Aug 28, 2020
//
// Author: G. Chopcinski, Kg Solutions, LLC
// 
//*****************************************************************************

#include "ASL4321_System.h"

//*************************************************************************************

typedef struct CUSTOM_MENU_BUTTON_STRUCT{
    GX_BUTTON_MEMBERS_DECLARE
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

typedef enum MENU_POSITION_E
{
	PAD_SETTINGS_MENU_POS,
	USER_SETTINGS_MENU_POS,
	FEATURE_LIST_MENU_POS,
	BLUETOOTH_SETUP_MENU_POS,
	SEATING_SETUP_MENU_POS,
	AUDIBLE_USAGE_MENU_POS,
//	SOUNDS_MENU_POS,
	PERFORMANCE_MENU_POS,
	DIAGNOSTICS_MENU_POS,
	END_OF_LIST_MENU_POS
} MENU_POSITION_ENUM;

CUSTOM_MENU_BUTTON g_MenuItems[10];


//*************************************************************************************

VOID MenuItem_draw(GX_BUTTON *checkbox);
UINT MenuItem_event_process(GX_BUTTON *checkbox, GX_EVENT *event_ptr);
VOID MenuItem_select(GX_BUTTON *checkbox);


//*************************************************************************************
#if 0
VOID CustomMenu_Item_Create(CUSTOM_MENU_BUTTON *menuItem, GX_WIDGET *parent, CUSTOM_CHECKBOX_INFO *info, GX_RECTANGLE *size, int enabled)
{
    menuItem->gx_button_select_handler = (VOID (*)(GX_WIDGET *))MenuItem_select;
    menuItem->gx_widget_draw_function = (VOID (*)(GX_WIDGET *))MenuItem_draw;
    menuItem->gx_widget_event_process_function = (UINT (*)(GX_WIDGET *, GX_EVENT *))MenuItem_event_process;
    //menuItem->background_id = info->background_id;
    menuItem->cur_offset = info->start_offset;
    menuItem->start_offset = info->start_offset;
    menuItem->end_offset = info->end_offset;
}


void CreateMenuWidgets (GX_VERTICAL_LIST *list)
{
}

VOID MenuItem_draw(GX_BUTTON *checkbox)
{
}

//*************************************************************************************

UINT MenuItem_event_process(GX_BUTTON *button, GX_EVENT *event_ptr)
{
    gx_window_event_process((GX_WINDOW*)button, event_ptr);

	return 0;
}

//*************************************************************************************

VOID MenuItem_select(GX_BUTTON *checkbox)
{
}
#endif 

//*************************************************************************************

void PopulateMenuItems(void)
{
	// PAD SETTINGS 
	g_MenuItems[PAD_SETTINGS_MENU_POS].m_ButtonID = 1000;
	g_MenuItems[PAD_SETTINGS_MENU_POS].m_TextID = GX_STRING_ID_STRING_46;
	g_MenuItems[PAD_SETTINGS_MENU_POS].m_Enabled = TRUE;
	// USER SETTINGS
	g_MenuItems[USER_SETTINGS_MENU_POS].m_ButtonID = 1001;
	g_MenuItems[USER_SETTINGS_MENU_POS].m_TextID = GX_STRING_ID_STRING_45;
	g_MenuItems[USER_SETTINGS_MENU_POS].m_Enabled = TRUE;
	// FEATURE LIST
	g_MenuItems[FEATURE_LIST_MENU_POS].m_ButtonID = 1002;
	g_MenuItems[FEATURE_LIST_MENU_POS].m_TextID = GX_STRING_ID_STRING_67;
	g_MenuItems[FEATURE_LIST_MENU_POS].m_Enabled = TRUE;
	// BLUETOOTH SETUP
	g_MenuItems[BLUETOOTH_SETUP_MENU_POS].m_ButtonID = 1003;
	g_MenuItems[BLUETOOTH_SETUP_MENU_POS].m_TextID = GX_STRING_ID_STRING_31;
	g_MenuItems[BLUETOOTH_SETUP_MENU_POS].m_Enabled = TRUE;
	// SEATING SETUP
	g_MenuItems[SEATING_SETUP_MENU_POS].m_ButtonID = 1004;
	g_MenuItems[SEATING_SETUP_MENU_POS].m_TextID = GX_STRING_ID_SEATING;
	g_MenuItems[SEATING_SETUP_MENU_POS].m_Enabled = TRUE;
	// AUDIBLE USAGE
	g_MenuItems[AUDIBLE_USAGE_MENU_POS].m_ButtonID = 1005;
	g_MenuItems[AUDIBLE_USAGE_MENU_POS].m_TextID = GX_STRING_ID_AUDIBLE_USAGE;
	g_MenuItems[AUDIBLE_USAGE_MENU_POS].m_Enabled = TRUE;
	// SOUNDS
	//g_MenuItems[SOUNDS_MENU_POS].m_ButtonID = 1006;
	//g_MenuItems[SOUNDS_MENU_POS].m_TextID = GX_STRING_ID_SOUNDS;
	//g_MenuItems[SOUNDS_MENU_POS].m_Enabled = TRUE;
	// PERFORMANCE
	g_MenuItems[PERFORMANCE_MENU_POS].m_ButtonID = 1007;
	g_MenuItems[PERFORMANCE_MENU_POS].m_TextID = GX_STRING_ID_STRING_94;
	g_MenuItems[PERFORMANCE_MENU_POS].m_Enabled = TRUE;
	// DIAGNOSTICS
	g_MenuItems[DIAGNOSTICS_MENU_POS].m_ButtonID = 1008;
	g_MenuItems[DIAGNOSTICS_MENU_POS].m_TextID = GX_STRING_ID_STRING_10;
	g_MenuItems[DIAGNOSTICS_MENU_POS].m_Enabled = TRUE;
	// Not used, must be last
	g_MenuItems[END_OF_LIST_MENU_POS].m_ButtonID = 0;
	g_MenuItems[END_OF_LIST_MENU_POS].m_TextID = GX_STRING_ID_BLANK;
	g_MenuItems[END_OF_LIST_MENU_POS].m_Enabled = FALSE;
}

//*************************************************************************************

UINT MenuItem_event_function (GX_WINDOW *window, GX_EVENT *event_ptr)
{
    gx_window_event_process(window, event_ptr);

	return GX_SUCCESS;
}

//*************************************************************************************
#if 0
int myInt;

UINT List_event_function (GX_VERTICAL_LIST *list, GX_EVENT *event_ptr)
{

	switch (event_ptr->gx_event_type)
	{
	case GX_SIGNAL(1000, GX_EVENT_CLICKED):
		myInt = 1000;
		break;
	case GX_SIGNAL(FEATURE_VERTICAL_LIST, GX_EVENT_LIST_SELECT):
		myInt = 0;
		break;
	} // end switch

    gx_vertical_list_event_process(list, event_ptr);

	return GX_SUCCESS;
}
#endif
	
//*************************************************************************************

VOID MenuItem_Callback (GX_VERTICAL_LIST *pList, GX_WIDGET *pWidget, INT pIndex)
{
	GX_BOOL result;
	CUSTOM_MENU_BUTTON *thisItem;
	GX_RECTANGLE childsize;

	thisItem = (CUSTOM_MENU_BUTTON*) pWidget;

	gx_widget_created_test(&thisItem->m_MenuWidget, &result);	// Test to see if the GUIX item has been created, I guess it's like a null test.

	if (!result)	// "!result" means, item has not been created yet.
	{
		//thisItem->gx_button_select_handler = (VOID (*)(GX_WIDGET *))MenuItem_select;
		//thisItem->gx_widget_draw_function = (VOID (*)(GX_WIDGET *))MenuItem_draw;
		//thisItem->gx_widget_event_process_function = (UINT (*)(GX_WIDGET *, GX_EVENT *))MenuItem_event_process;

		gx_utility_rectangle_define(&childsize, 0, 0, pList->gx_widget_size.gx_rectangle_right - 35 , 74);	// 0,0,x,75 is too high... need to make a rectangle to declare it's space.
        gx_widget_create(&thisItem->m_MenuWidget, NULL, (GX_WIDGET *)pList, GX_STYLE_TRANSPARENT | GX_STYLE_ENABLED/* | GX_STYLE_TEXT_LEFT */, GX_ID_NONE, &childsize);

#if 0
        //gx_utility_rectangle_define(&childsize, 130, (52-27)/2, 188, 47); // Left, Top, Right, Botton
		childsize.gx_rectangle_left = thisItem->m_MenuWidget.gx_widget_size.gx_rectangle_right - 58 - 14; // "58" is size of pixel  140;
		childsize.gx_rectangle_top = (52-27)/2;
		childsize.gx_rectangle_right = childsize.gx_rectangle_left + 58;	// "58" is width of background pic
		childsize.gx_rectangle_bottom = 47;

		// This draws a rectanble type button
        gx_utility_rectangle_define(&childsize, 0, 4, thisItem->m_MenuWidget.gx_widget_size.gx_rectangle_right, 56);
//		gx_prompt_create(&thisItem->m_PromptWidget, NULL, &thisItem->m_MenuWidget, 0, GX_STYLE_TRANSPARENT | GX_STYLE_BORDER_NONE | GX_STYLE_ENABLED, 0, &childsize);
		gx_prompt_create(&thisItem->m_PromptWidget, NULL, &thisItem->m_MenuWidget, 0, GX_STYLE_TRANSPARENT | GX_STYLE_BORDER_THIN | GX_STYLE_ENABLED, 0, &childsize);
        //gx_widget_fill_color_set(&feature->m_PromptWidget, GX_COLOR_ID_DARK_GRAY, GX_COLOR_ID_DARK_GRAY, GX_COLOR_ID_DARK_GRAY);
        gx_prompt_text_color_set(&thisItem->m_PromptWidget, GX_COLOR_ID_WHITE, GX_COLOR_ID_WHITE, GX_COLOR_ID_WHITE);
		gx_prompt_text_id_set(&thisItem->m_PromptWidget, thisItem->m_TextID); // thisItem->m_LargeDescriptionID);
#endif 
		// This draws an actual GUIX button
        gx_utility_rectangle_define(&childsize, 8, 4, thisItem->m_MenuWidget.gx_widget_size.gx_rectangle_right-4, 68);
		gx_text_button_create (&thisItem->m_ButtonWidget, NULL, &thisItem->m_MenuWidget, thisItem->m_TextID, GX_STYLE_ENABLED | GX_STYLE_BORDER_THIN, 0, &childsize);
		gx_text_button_text_color_set (&thisItem->m_ButtonWidget, GX_COLOR_ID_BTN_TEXT, GX_COLOR_ID_BTN_TEXT, GX_COLOR_ID_DISABLED_TEXT);
		gx_widget_fill_color_set ((GX_WIDGET*) &thisItem->m_ButtonWidget, GX_COLOR_ID_TEXT_INPUT_FILL, GX_COLOR_ID_TEXT_INPUT_TEXT, GX_COLOR_ID_DISABLED_FILL);
	}
}

//*************************************************************************************
// Function Name: HHP_Start_Screen_event_process
//
// Description: This handles the Startup Screen messages
//
//*************************************************************************************

UINT HHP_Start_Screen_event_process_new (GX_WINDOW *window, GX_EVENT *event_ptr)
{
	ULONG myStyle = GX_STYLE_BORDER_THICK;
	//GX_RECTANGLE myRectangle = {1,1,100, 100};
	//UINT myErr;
	//UINT feature;
	//GX_BOOL result;
	int index;
	int numberOfActiveMenuItems;

	HHP_START_SCREEN_CONTROL_BLOCK *StartingWindowPtr = (HHP_START_SCREEN_CONTROL_BLOCK*) window;

	switch (event_ptr->gx_event_type)
	{
	case GX_EVENT_SHOW:
		// This sets the correct Group Icon in the Group Button on this screen.
		SetGroupIcon (&StartingWindowPtr->HHP_Start_Screen_GroupIconButton);
		PopulateMenuItems();

		numberOfActiveMenuItems = 0;
		for (index = 0; g_MenuItems[index].m_ButtonID > 0; ++index)
		{
			if (g_MenuItems[index].m_Enabled)
			{
				MenuItem_Callback (&StartingWindowPtr->HHP_Start_Screen_FeatureList, (GX_WIDGET*) &g_MenuItems[index], index);
				++numberOfActiveMenuItems;
			}
		}
		StartingWindowPtr->HHP_Start_Screen_FeatureList.gx_vertical_list_total_rows = numberOfActiveMenuItems;
        gx_vertical_list_selected_set(&StartingWindowPtr->HHP_Start_Screen_FeatureList, -1);

		if (StartingWindowPtr->HHP_Start_Screen_FeatureList.gx_vertical_list_total_rows < 5)
			gx_widget_hide ((GX_WIDGET*) &StartingWindowPtr->HHP_Start_Screen_FeatureList_vertical_scroll);

		break;

    case GX_SIGNAL(FEATURE_VERTICAL_LIST, GX_EVENT_LIST_SELECT):
		gx_vertical_list_selected_index_get (&StartingWindowPtr->HHP_Start_Screen_FeatureList, &index);
		switch (index)
		{
		case PAD_SETTINGS_MENU_POS: // PAD SETTINGS
			screen_toggle ((GX_WINDOW*) &PadOptionsSettingsScreen, window);
			break;
		case USER_SETTINGS_MENU_POS: // USER SETTINGS
			screen_toggle ((GX_WINDOW*) &UserSettingsScreen, window);
			break;
		case FEATURE_LIST_MENU_POS:	// FEATURE LIST
			screen_toggle ((GX_WINDOW*) &FeatureSettingsScreen, window);
			break;
		case BLUETOOTH_SETUP_MENU_POS:
			break;
		case SEATING_SETUP_MENU_POS:	// SEATING SETUP
		//	screen_toggle ((GX_WINDOW*) &UserSettingsScreen, window);
			break;
		case AUDIBLE_USAGE_MENU_POS:	// AUDIBLE USAGE
			screen_toggle ((GX_WINDOW*) &SoundOptionScreen, window);
			break;
		//case SOUNDS_MENU_POS:	// SOUNDS
		////	screen_toggle ((GX_WINDOW*) &UserSettingsScreen, window);
		//	break;
		case PERFORMANCE_MENU_POS:	// PERFORMANCE
			screen_toggle ((GX_WINDOW*) &PerformanceSelectionScreen, window);
			break;
		case DIAGNOSTICS_MENU_POS:	// DIAGNOSTICS
			screen_toggle ((GX_WINDOW*) &DiagnosticScreen, window);
			break;
		default:
			break;
		}
		break;

	case GX_SIGNAL(OK_BTN_ID, GX_EVENT_CLICKED):
		gx_window_event_process(window, event_ptr);
        screen_toggle((GX_WINDOW *)&MainUserScreen, window);
		break;

	default:
		numberOfActiveMenuItems = 100;
		break;
	}
	
	gx_window_event_process(window, event_ptr);


	return GX_SUCCESS;
}

