//*****************************************************************************
// Filename: SoundOptionScreen.cpp
//
// Date: Jam 28, 2021
//
// Author: G. Chopcinski, Kg Solutions, LLC
// 
//*****************************************************************************

#include "ASL4321_System.h"
#include "asl4321_display_demo_resources.h"
#include "custom_checkbox.h"


typedef struct SOUND_STRUCTURE {
	CUSTOM_MENU_BUTTON m_Menu;
	char m_Description[12];
} SOUND_STRUCT;

//*************************************************************************************
// Local Variables
static SOUND_STRUCT g_Sound_MenuItems[MAX_SOUND_BITES];

//*************************************************************************************
// Function Name: SoundOptionScreen_event_process
//
// Description: This dispatches the Sound Option
//
//*************************************************************************************

UINT SoundOptionScreen_event_process (GX_WINDOW *window, GX_EVENT *event_ptr)
{
	UINT myErr;

	switch (event_ptr->gx_event_type)
	{
	case GX_EVENT_SHOW:
		SetGroupIcon (&SoundOptionScreen.SoundOptionScreen_GroupIconButton);
		break;

	case GX_SIGNAL(OK_BTN_ID, GX_EVENT_CLICKED):
        screen_toggle((GX_WINDOW *)&HHP_Start_Screen, window);
		break;

	case GX_SIGNAL(SOUND_BTN_ID, GX_EVENT_CLICKED):
        screen_toggle((GX_WINDOW *)&ManageSoundScreen, window);
		break;

	case GX_SIGNAL(USER_SOUND_SETUP_BTN_ID, GX_EVENT_CLICKED):
        screen_toggle((GX_WINDOW *)&SoundSetupScreen, window);
		break;
	}

    myErr = gx_window_event_process(window, event_ptr);

	return GX_SUCCESS;
}

//*************************************************************************************
static VOID Populate_Sound_MenuItems(VOID)
{
	int i;

	for (i=0; i < MAX_SOUND_BITES; ++i)
	{
		g_Sound_MenuItems[i].m_Menu.m_ButtonID = SOUND1_BUTTON_ID + i;
		g_Sound_MenuItems[i].m_Menu.m_TextID = GX_STRING_ID_BLANK;
		g_Sound_MenuItems[i].m_Menu.m_Enabled = TRUE;
		sprintf_s (g_Sound_MenuItems[i].m_Description, sizeof (g_Sound_MenuItems[0].m_Description), "Spare %d", i+1);
	}
	g_Sound_MenuItems[0].m_Menu.m_ButtonID = SOUND1_BUTTON_ID;
	g_Sound_MenuItems[0].m_Menu.m_TextID = GX_STRING_ID_BLANK;
	g_Sound_MenuItems[0].m_Menu.m_Enabled = TRUE;
	strcpy_s (g_Sound_MenuItems[0].m_Description, sizeof (g_Sound_MenuItems[0].m_Description), "YES");

	g_Sound_MenuItems[1].m_Menu.m_ButtonID = SOUND2_BUTTON_ID;
	g_Sound_MenuItems[1].m_Menu.m_TextID = GX_STRING_ID_BLANK;
	g_Sound_MenuItems[1].m_Menu.m_Enabled = TRUE;
	strcpy_s (g_Sound_MenuItems[1].m_Description, sizeof (g_Sound_MenuItems[1].m_Description), "NO");
}

//*************************************************************************************

VOID ManageScreen_DrawButton (GX_VERTICAL_LIST *pList, SOUND_STRUCT *thisItem, INT pIndex)
{
	GX_BOOL result;
	//SOUND_STRUCT *thisItem;
	GX_RECTANGLE childsize;

	//thisItem = (SOUND_STRUCT*) pWidget;

	gx_widget_created_test(&thisItem->m_Menu.m_MenuWidget, &result);	// Test to see if the GUIX item has been created, I guess it's like a null test.

	if (!result)	// "!result" means, item has not been created yet.
	{
		gx_utility_rectangle_define(&childsize, 0, 0, pList->gx_widget_size.gx_rectangle_right - 35 , 74);	// 0,0,x,75 is too high... need to make a rectangle to declare it's space.
		gx_widget_create(&thisItem->m_Menu.m_MenuWidget, NULL, (GX_WIDGET *)pList, GX_STYLE_TRANSPARENT | GX_STYLE_ENABLED/* | GX_STYLE_TEXT_LEFT */, GX_ID_NONE, &childsize);

		// This draws an actual GUIX button
//		gx_utility_rectangle_define(&childsize, 8, 4, thisItem->m_Menu.m_MenuWidget.gx_widget_size.gx_rectangle_right-4, 68);
		gx_utility_rectangle_define(&childsize, 8, 4, thisItem->m_Menu.m_MenuWidget.gx_widget_size.gx_rectangle_right-44, 68);
		gx_text_button_create (&thisItem->m_Menu.m_ButtonWidget, NULL, &thisItem->m_Menu.m_MenuWidget, thisItem->m_Menu.m_TextID, GX_STYLE_ENABLED | GX_STYLE_BORDER_THIN, 0, &childsize);
		gx_text_button_text_set (&thisItem->m_Menu.m_ButtonWidget, thisItem->m_Description);
		gx_text_button_text_color_set (&thisItem->m_Menu.m_ButtonWidget, GX_COLOR_ID_BTN_TEXT, GX_COLOR_ID_BTN_TEXT, GX_COLOR_ID_DISABLED_TEXT);
		gx_widget_fill_color_set ((GX_WIDGET*) &thisItem->m_Menu.m_ButtonWidget, GX_COLOR_ID_TEXT_INPUT_FILL, GX_COLOR_ID_TEXT_INPUT_TEXT, GX_COLOR_ID_DISABLED_FILL);
	}
}

//*************************************************************************************

UINT ManageSoundScreen_event_process (GX_WINDOW *window, GX_EVENT *event_ptr)
{
	UINT myErr;
	int numberOfActiveMenuItems, index;
	int selectedIndex, runningIndex;

	MANAGESOUNDSCREEN_CONTROL_BLOCK *WindowPtr = (MANAGESOUNDSCREEN_CONTROL_BLOCK*) window;

	switch (event_ptr->gx_event_type)
	{
	case GX_EVENT_SHOW:
		// This sets the correct Group Icon in the Group Button on this screen.
		Populate_Sound_MenuItems();

		numberOfActiveMenuItems = 0;
		for (index = 0; index < MAX_SOUND_BITES; ++index)
		{
			if (g_Sound_MenuItems[index].m_Menu.m_Enabled)
			{
				ManageScreen_DrawButton (&WindowPtr->ManageSoundScreen_SoundVerticalList, &g_Sound_MenuItems[index], index);
				++numberOfActiveMenuItems;
			}
		}
		WindowPtr->ManageSoundScreen_SoundVerticalList.gx_vertical_list_total_rows = numberOfActiveMenuItems;
        gx_vertical_list_selected_set(&WindowPtr->ManageSoundScreen_SoundVerticalList, -1);

		if (WindowPtr->ManageSoundScreen_SoundVerticalList.gx_vertical_list_total_rows <= 3)
			gx_widget_hide ((GX_WIDGET*) &WindowPtr->ManageSoundScreen_FeatureList_vertical_scroll);
		break;

    case GX_SIGNAL(SOUND_VERTICAL_LIST_ID, GX_EVENT_LIST_SELECT):
		gx_vertical_list_selected_index_get (&WindowPtr->ManageSoundScreen_SoundVerticalList, &selectedIndex);
		// "index" refers to the item in the list... and not the item itself.
		// We will identify which pad option has been selected.
		runningIndex = 0;	// This will increment with each enabled items.
		for (index = 0; index < MAX_SOUND_BITES; ++index)
		{
			if (g_Sound_MenuItems[index].m_Menu.m_Enabled)
			{
				if (runningIndex == selectedIndex)
					break;				// we found the correct Pad Option button.
				else
					++runningIndex;		// We need to continue looking.
			}
		}
		break;

	case GX_SIGNAL(OK_BTN_ID, GX_EVENT_CLICKED):
        screen_toggle((GX_WINDOW *)&SoundOptionScreen, window);
		break;
	}

    myErr = gx_window_event_process(window, event_ptr);

	return GX_SUCCESS;
}

//*************************************************************************************

UINT SoundSetupScreen_event_process (GX_WINDOW *window, GX_EVENT *event_ptr)
{
	UINT myErr;

	switch (event_ptr->gx_event_type)
	{
	case GX_SIGNAL(OK_BTN_ID, GX_EVENT_CLICKED):
        screen_toggle((GX_WINDOW *)&SoundOptionScreen, window);
		break;
	}

    myErr = gx_window_event_process(window, event_ptr);

	return GX_SUCCESS;
}




