//*****************************************************************************
// Filename: SoundOptionScreen.cpp
//
// Date: Jan 28, 2021
//
// Author: G. Chopcinski, Kg Solutions, LLC
// 
//*****************************************************************************

#include "ASL4321_System.h"
#include "asl4321_display_demo_resources.h"
#include "DataDictionary.h"
#include "custom_checkbox.h"
#include "KeyboardScreen.h"

//*************************************************************************************
// Local typedefs
typedef struct SOUND_STRUCTURE {
	CUSTOM_MENU_BUTTON m_Menu;
	char m_SoundLabel[SOUND_BITE_NAME_LENGTH];
	char m_FileName[SOUND_BITE_FILENAME_LENGTH];
} SOUND_GUI_STRUCT;
static SOUND_GUI_STRUCT g_Sound_MenuItems[MAX_SOUND_BITES];

//*************************************************************************************
// Local Variables

static USHORT g_SelectedSoundIndex = 0;
static USHORT g_SoundBank = 0;
static USHORT g_Group;

//*************************************************************************************
// External function prototypes

extern VOID LocatePadPosition (PHYSICAL_PAD_ENUM pad, DEVICE_TYPE_ENUM device, GX_BOOL showPrompt, GX_ICON *icon, GX_PROMPT *prompt);

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
VOID ManageSoundItemList_callback (GX_VERTICAL_LIST *pList, GX_WIDGET *pWidget, INT pIndex)
{
	GX_BOOL result;
	SOUND_GUI_STRUCT *thisItem;
	GX_RECTANGLE childsize;

	thisItem = (SOUND_GUI_STRUCT*) pWidget;

	gx_widget_created_test(&thisItem->m_Menu.m_MenuWidget, &result);	// Test to see if the GUIX item has been created, I guess it's like a null test.

	if (!result)	// "!result" means, item has not been created yet.
	{
		gx_utility_rectangle_define(&childsize, 0, 0, pList->gx_widget_size.gx_rectangle_right - 35 , 74);	// 0,0,x,75 is too high... need to make a rectangle to declare it's space.
		gx_widget_create(&thisItem->m_Menu.m_MenuWidget, NULL, (GX_WIDGET *)pList, GX_STYLE_TRANSPARENT | GX_STYLE_ENABLED/* | GX_STYLE_TEXT_LEFT */, GX_ID_NONE, &childsize);

		// This draws an actual GUIX button
		gx_utility_rectangle_define(&childsize, 8, 4, thisItem->m_Menu.m_MenuWidget.gx_widget_size.gx_rectangle_right-44, 68);
		gx_text_button_create (&thisItem->m_Menu.m_ButtonWidget, NULL, &thisItem->m_Menu.m_MenuWidget, 0, GX_STYLE_ENABLED | GX_STYLE_BORDER_THIN, 0, &childsize);
		gx_text_button_text_color_set (&thisItem->m_Menu.m_ButtonWidget, GX_COLOR_ID_BTN_TEXT, GX_COLOR_ID_BTN_TEXT, GX_COLOR_ID_DISABLED_TEXT);
		gx_widget_fill_color_set ((GX_WIDGET*) &thisItem->m_Menu.m_ButtonWidget, GX_COLOR_ID_TEXT_INPUT_FILL, GX_COLOR_ID_TEXT_INPUT_TEXT, GX_COLOR_ID_DISABLED_FILL);
	}
	// Always update the Sound Label in the button
	gx_text_button_text_set (&thisItem->m_Menu.m_ButtonWidget, thisItem->m_SoundLabel);
}

//************************************************************************************

static VOID DeleteList (GX_WINDOW *window)
{
	int index;
	GX_BOOL result;
	MANAGESOUNDSCREEN_CONTROL_BLOCK *WindowPtr = (MANAGESOUNDSCREEN_CONTROL_BLOCK*) window;

	for (index = 0; index < MAX_SOUND_BITES; ++index)
	{
		gx_widget_created_test(&g_Sound_MenuItems[index].m_Menu.m_ButtonWidget, &result);	// Test to see if the GUIX item has been created, I guess it's like a null test.
		if (result)
		{
			gx_widget_delete((GX_WIDGET*) &g_Sound_MenuItems[index].m_Menu.m_ButtonWidget);
		}
		gx_widget_created_test(&g_Sound_MenuItems[index].m_Menu.m_MenuWidget, &result);	// Test to see if the GUIX item has been created, I guess it's like a null test.
		if (result)
		{
			gx_widget_delete((GX_WIDGET*) &g_Sound_MenuItems[index].m_Menu.m_MenuWidget);
		}
	}
	WindowPtr->ManageSoundScreen_SoundVerticalList.gx_vertical_list_total_rows = 0;
}


//************************************************************************************

UINT ManageSoundScreen_event_process (GX_WINDOW *window, GX_EVENT *event_ptr)
{
	UINT myErr;
	int numberOfActiveMenuItems, index;
	int selectedIndex;
	char myStr[16];

	MANAGESOUNDSCREEN_CONTROL_BLOCK *WindowPtr = (MANAGESOUNDSCREEN_CONTROL_BLOCK*) window;

	switch (event_ptr->gx_event_type)
	{
	case GX_EVENT_SHOW:
		numberOfActiveMenuItems = 0;
		for (index = 0; index < MAX_SOUND_BITES; ++index)
		{
			dd_GetString (0, DD_SOUNDBITE_NAME, index, myStr);
			strcpy_s (g_Sound_MenuItems[index].m_SoundLabel, sizeof (g_Sound_MenuItems[index].m_SoundLabel), myStr);
			dd_GetString (0, DD_SOUNDBITE_FILENAME, index, myStr);
			strcpy_s (g_Sound_MenuItems[index].m_FileName, sizeof (g_Sound_MenuItems[index].m_FileName), myStr);
			ManageSoundItemList_callback (&WindowPtr->ManageSoundScreen_SoundVerticalList, (GX_WIDGET*) &g_Sound_MenuItems[index], index);
			++numberOfActiveMenuItems;
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
		g_SelectedSoundIndex = selectedIndex;
        screen_toggle((GX_WINDOW *)&EditSoundScreen, window);
		break;

	case GX_SIGNAL(OK_BTN_ID, GX_EVENT_CLICKED):
		screen_toggle((GX_WINDOW *)&SoundOptionScreen, window);
		break;
	}

    myErr = gx_window_event_process(window, event_ptr);

	return GX_SUCCESS;
}

//*************************************************************************************
static VOID SetLevel (VOID)
{
	gx_numeric_prompt_value_set (&SoundSetupScreen.SoundSetupScreen_LevelNumber, g_SoundBank + 1);	// "+1" makes it one-based
}

static VOID GetNextSoundBite (USHORT *soundID)
{
	USHORT index;

	for (index = 0; index < MAX_SOUND_BITES; ++index)
	{
		++*soundID;
		if (*soundID > MAX_SOUND_BITES)
			*soundID = 0;
		if (g_Sound_MenuItems[*soundID].m_SoundLabel[0] != '<')
			break;	
	}
}

static VOID PositionPads (VOID)
{
	DEVICE_TYPE_ENUM thisDevice;

	thisDevice = (DEVICE_TYPE_ENUM) dd_Get_USHORT (g_Group, DD_DEVICE_TYPE);

	LocatePadPosition (LEFT_PAD, thisDevice, TRUE, &SoundSetupScreen.SoundSetupScreen_Left_Icon, &SoundSetupScreen.SoundSetupScreen_LeftPad_SoundName);
	LocatePadPosition (RIGHT_PAD, thisDevice, TRUE, &SoundSetupScreen.SoundSetupScreen_Right_Icon, &SoundSetupScreen.SoundSetupScreen_RightPad_SoundName);
	LocatePadPosition (FORWARD_PAD, thisDevice, TRUE, &SoundSetupScreen.SoundSetupScreen_Forward_Icon, &SoundSetupScreen.SoundSetupScreen_ForwardPad_SoundName);
	LocatePadPosition (REVERSE_PAD, thisDevice, TRUE, &SoundSetupScreen.SoundSetupScreen_Reverse_Icon, &SoundSetupScreen.SoundSetupScreen_ReversePad_SoundName);

}

//*************************************************************************************
// This function populates the 4 pads with the speaker bites names.

VOID PopulateSoundPad(VOID)
{
	USHORT group;
	USHORT soundID;

	group = dd_Get_USHORT (0, DD_GROUP);	// Get currently selected group.

	// Populate the forward pad with the Sound Bite Label
	soundID = dd_GetSubItem_USHORT (group, DD_ACTIVE_SPEAKER_SUBITEM_FORWARD, g_SoundBank);
	if (soundID < MAX_SOUND_BITES)
		gx_prompt_text_set ((GX_PROMPT*)&SoundSetupScreen.SoundSetupScreen_ForwardPad_SoundName, g_Sound_MenuItems[soundID].m_SoundLabel);
	else
		gx_prompt_text_set ((GX_PROMPT*)&SoundSetupScreen.SoundSetupScreen_ForwardPad_SoundName, "----");

	// Populate the reverse pad with the Sound Bite Label
	soundID = dd_GetSubItem_USHORT (group, DD_ACTIVE_SPEAKER_SUBITEM_REVERSE, g_SoundBank);
	if (soundID < MAX_SOUND_BITES)
		gx_prompt_text_set ((GX_PROMPT*)&SoundSetupScreen.SoundSetupScreen_ReversePad_SoundName, g_Sound_MenuItems[soundID].m_SoundLabel);
	else
		gx_prompt_text_set ((GX_PROMPT*)&SoundSetupScreen.SoundSetupScreen_ReversePad_SoundName, "----");

	// Populate the left pad with the Sound Bite Label
	soundID = dd_GetSubItem_USHORT (group, DD_ACTIVE_SPEAKER_SUBITEM_LEFT, g_SoundBank);
	if (soundID < MAX_SOUND_BITES)
		gx_prompt_text_set ((GX_PROMPT*)&SoundSetupScreen.SoundSetupScreen_LeftPad_SoundName, g_Sound_MenuItems[soundID].m_SoundLabel);
	else
		gx_prompt_text_set ((GX_PROMPT*)&SoundSetupScreen.SoundSetupScreen_LeftPad_SoundName, "----");

	// Populate the right pad with the Sound Bite Label
	soundID = dd_GetSubItem_USHORT (group, DD_ACTIVE_SPEAKER_SUBITEM_RIGHT, g_SoundBank);
	if (soundID < MAX_SOUND_BITES)
		gx_prompt_text_set ((GX_PROMPT*)&SoundSetupScreen.SoundSetupScreen_RightPad_SoundName, g_Sound_MenuItems[soundID].m_SoundLabel);
	else
		gx_prompt_text_set ((GX_PROMPT*)&SoundSetupScreen.SoundSetupScreen_RightPad_SoundName, "----");
}

//*************************************************************************************
// This is the screen that shows the Pads and each level for setting the sound
// bites associated with each.

UINT SoundSetupScreen_event_process (GX_WINDOW *window, GX_EVENT *event_ptr)
{
	UINT myErr;
	USHORT index;
	USHORT soundID;

	switch (event_ptr->gx_event_type)
	{
	case GX_EVENT_SHOW:
		g_Group = dd_Get_USHORT (0, DD_GROUP);

		for (index = 0; index < MAX_SOUND_BITES; ++index)
		{
			dd_GetString (0, DD_SOUNDBITE_NAME, index, g_Sound_MenuItems[index].m_SoundLabel);
			dd_GetString (0, DD_SOUNDBITE_FILENAME, index, g_Sound_MenuItems[index].m_FileName);
		}
		g_SoundBank = 0;		// This is the level or bank of which there are 8.
		SetLevel();
		SetGroupIcon (&SoundSetupScreen.SoundSetupScreen_GroupIconButton);
		PositionPads();
		PopulateSoundPad();
		break;

	case GX_SIGNAL(LEFT_LEVEL_BTN_ID, GX_EVENT_CLICKED):
		if (g_SoundBank > 0)
			--g_SoundBank;
		SetLevel();
		PopulateSoundPad();
		break;

	case GX_SIGNAL(RIGHT_LEVEL_BTN_ID, GX_EVENT_CLICKED):
		if (g_SoundBank < (MAX_AUDIBLE_SUBITEMS-1) )
			++g_SoundBank;
		SetLevel();
		PopulateSoundPad();
		break;

	case GX_SIGNAL(FORWARD_ICON_ID, GX_EVENT_CLICKED):
		soundID = dd_GetSubItem_USHORT (g_Group, DD_ACTIVE_SPEAKER_SUBITEM_FORWARD, g_SoundBank);
		GetNextSoundBite (&soundID);		// Search for next defined Sound Bite
		dd_SetSubItem_USHORT (g_Group, DD_ACTIVE_SPEAKER_SUBITEM_FORWARD, g_SoundBank, soundID);
		PopulateSoundPad();
		break;

	case GX_SIGNAL(REVERSE_ICON_ID, GX_EVENT_CLICKED):
		soundID = dd_GetSubItem_USHORT (g_Group, DD_ACTIVE_SPEAKER_SUBITEM_REVERSE, g_SoundBank);
		GetNextSoundBite (&soundID);		// Search for next defined Sound Bite
		dd_SetSubItem_USHORT (g_Group, DD_ACTIVE_SPEAKER_SUBITEM_REVERSE, g_SoundBank, soundID);
		PopulateSoundPad();
		break;

	case GX_SIGNAL(LEFT_ICON_ID, GX_EVENT_CLICKED):
		soundID = dd_GetSubItem_USHORT (g_Group, DD_ACTIVE_SPEAKER_SUBITEM_LEFT, g_SoundBank);
		GetNextSoundBite (&soundID);		// Search for next defined Sound Bite
		dd_SetSubItem_USHORT (g_Group, DD_ACTIVE_SPEAKER_SUBITEM_LEFT, g_SoundBank, soundID);
		PopulateSoundPad();
		break;

	case GX_SIGNAL(RIGHT_ICON_ID, GX_EVENT_CLICKED):
		soundID = dd_GetSubItem_USHORT (g_Group, DD_ACTIVE_SPEAKER_SUBITEM_RIGHT, g_SoundBank);
		GetNextSoundBite (&soundID);		// Search for next defined Sound Bite
		dd_SetSubItem_USHORT (g_Group, DD_ACTIVE_SPEAKER_SUBITEM_RIGHT, g_SoundBank, soundID);
		PopulateSoundPad();
		break;

	case GX_SIGNAL(OK_BTN_ID, GX_EVENT_CLICKED):
        screen_toggle((GX_WINDOW *)&SoundOptionScreen, window);
		break;
	}

    myErr = gx_window_event_process(window, event_ptr);

	return GX_SUCCESS;
}

//*************************************************************************************
// This is the screen that displays the Sound Bite "Name" and "file name"

UINT EditSoundScreen_event_process (GX_WINDOW *window, GX_EVENT *event_ptr)
{
	UINT myErr;
	USHORT index;

	switch (event_ptr->gx_event_type)
	{
	case GX_EVENT_SHOW:
		gx_numeric_prompt_value_set (&EditSoundScreen.EditSoundScreen_SoundNumber_Prompt, g_SelectedSoundIndex + 1);	// Make it "1" based.
		gx_prompt_text_set (&EditSoundScreen.EditSoundScreen_SoundName, g_Sound_MenuItems[g_SelectedSoundIndex].m_SoundLabel);
		gx_prompt_text_set (&EditSoundScreen.EditSoundScreen_SoundBite_Prompt, g_Sound_MenuItems[g_SelectedSoundIndex].m_FileName);
		break;

	case GX_SIGNAL(CHANGE_NAME_BTN_ID, GX_EVENT_CLICKED):
		SetReturnWindow (window);
		SetKeyboardData ((char**)&g_Sound_MenuItems[g_SelectedSoundIndex].m_SoundLabel, sizeof (g_Sound_MenuItems[0].m_SoundLabel));
        screen_toggle((GX_WINDOW *)&KeyboardScreen, window);
        break;

	case GX_SIGNAL(CHANGE_SOUND_BITE_BTN_ID, GX_EVENT_CLICKED):
        break;

	case GX_SIGNAL(OK_BTN_ID, GX_EVENT_CLICKED):
		// Store any changed Sound Labels and/or file names
		for (index = 0; index < MAX_SOUND_BITES; ++index)
		{
			dd_SetString (0, DD_SOUNDBITE_NAME, index, g_Sound_MenuItems[index].m_SoundLabel);
			dd_SetString (0, DD_SOUNDBITE_FILENAME, index, g_Sound_MenuItems[index].m_FileName);
		}
        screen_toggle((GX_WINDOW *)&ManageSoundScreen, window);
		break;
	}

    myErr = gx_window_event_process(window, event_ptr);

	return GX_SUCCESS;
}





