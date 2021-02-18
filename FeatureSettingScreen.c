//*****************************************************************************
// Filename: FeatureSettingsScreen.cpp
//
// Date: Aug 28, 2020
//
// Author: G. Chopcinski, Kg Solutions, LLC
// 
//*****************************************************************************

#include "ASL4321_System.h"
#include "custom_checkbox.h"

CUSTOM_CHECKBOX_INFO checkbox_info =
{
    TOGGLE_BUTTON_ID,
    GX_PIXELMAP_ID_SWITCH_BG,
    GX_PIXELMAP_ID_SWITCH_ACTIVE,
    GX_PIXELMAP_ID_SWITCH_DISACTIVE,
    4,
    24 // 24
};


//*************************************************************************************
// Local/Global variables
//*************************************************************************************

//*************************************************************************************
// Forward Declarations
//*************************************************************************************

void CreateFeatureWidgets (GX_VERTICAL_LIST *list);

//*************************************************************************************
// Function Name: FeatureList_callback
//
// Description: This function handles drawing and processing the List Items.
//
//*************************************************************************************

VOID FeatureList_callback(GX_VERTICAL_LIST *list, GX_WIDGET *widget, INT index)
{
    GX_RECTANGLE childsize;
    MAIN_SCREEN_FEATURE *feature = (MAIN_SCREEN_FEATURE *)widget;
    GX_BOOL result;

	gx_widget_created_test(&feature->m_ItemWidget, &result);	// Test to see if the GUIX item has been created, I guess it's like a null test.

    if (!result)	// NOPE? Let create the item.
    {
		// First, make a Rectangle as a container for the Prompt and cool slider checkbox button.
		// The rectangle is an arbitrary location in space that contains all of the widgets for a given selection.
        gx_utility_rectangle_define(&childsize, 0, 0, 270, 52);	// 0,0,x,75 is too high... need to make a rectangle to declare it's space.
        gx_widget_create(&feature->m_ItemWidget, NULL, (GX_WIDGET *)list, GX_STYLE_TRANSPARENT, GX_ID_NONE, &childsize);

		childsize.gx_rectangle_left = feature->m_ItemWidget.gx_widget_size.gx_rectangle_right - 50;
		childsize.gx_rectangle_top = (52-27)/2;
		childsize.gx_rectangle_right = childsize.gx_rectangle_left + 58;	// "58" is width of background pic
		childsize.gx_rectangle_bottom = 47;
		custom_checkbox_create(&feature->m_Checkbox, &feature->m_ItemWidget, &checkbox_info, &childsize, feature->m_Enabled);

        gx_utility_rectangle_define(&childsize, 0, 0, feature->m_ItemWidget.gx_widget_size.gx_rectangle_right - 58, 52);
		gx_prompt_create(&feature->m_PromptWidget, NULL, &feature->m_ItemWidget, 0, GX_STYLE_TEXT_RIGHT | GX_STYLE_TRANSPARENT | GX_STYLE_BORDER_NONE | GX_STYLE_ENABLED, 0, &childsize);
        gx_prompt_text_color_set(&feature->m_PromptWidget, GX_COLOR_ID_WHITE, GX_COLOR_ID_WHITE, GX_COLOR_ID_WHITE);
		gx_prompt_text_id_set(&feature->m_PromptWidget, feature->m_LargeDescriptionID);
	}

}

//*************************************************************************************
// This function gets the settings from the Feature List and populate the Feature
// settings.
//
//*************************************************************************************

void UpdateFeatureSettings ()
{
	int feature;
	int numActive;

	numActive = 0;
	for (feature = 0; feature < NUM_FEATURES; ++feature)
	{
		if (g_MainScreenFeatureInfo[feature].m_Available)
		{
			if (g_MainScreenFeatureInfo[feature].m_Checkbox.gx_widget_style & GX_STYLE_BUTTON_PUSHED)
			{
				g_MainScreenFeatureInfo[feature].m_Enabled = TRUE;
				g_MainScreenFeatureInfo[feature].m_Location = numActive;
				++numActive;
			}
			else
			{
				g_MainScreenFeatureInfo[feature].m_Enabled = FALSE;
			}
		}
	}
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
	FEATURESETTINGSSCREEN_CONTROL_BLOCK *FeatureWindowPtr = (FEATURESETTINGSSCREEN_CONTROL_BLOCK*) window;

	//GX_EVENT myEvent;

	switch (event_ptr->gx_event_type)
	{
		case GX_EVENT_SHOW:
			// This sets the correct Group Icon in the Group Button on this screen.
			SetGroupIcon (&FeatureSettingsScreen.FeatureSettingsScreen_GroupIconButton);

			CreateFeatureWidgets (&FeatureWindowPtr->FeatureSettingsScreen_FeatureListBox);
			if (FeatureWindowPtr->FeatureSettingsScreen_FeatureListBox.gx_vertical_list_total_rows < 5)
				gx_widget_hide ((GX_WIDGET*) &FeatureWindowPtr->FeatureSettingsScreen_FeatureList_vertical_scroll);
			// "Showing" the scroll bar doesn't work here. I put it in the processing of the OK button.
			//else
			//	gx_widget_show ((GX_WIDGET*) &FeatureWindowPtr->FeatureSettingsScreen_FeatureList_vertical_scroll);
			break;

		case GX_SIGNAL(GROUP_ICON_BUTTON, GX_EVENT_CLICKED):
			break;

		case GX_SIGNAL(OK_BTN_ID, GX_EVENT_CLICKED):
			gx_widget_show ((GX_WIDGET*) &FeatureWindowPtr->FeatureSettingsScreen_FeatureList_vertical_scroll);
	        screen_toggle((GX_WINDOW *)&HHP_Start_Screen, window);
			UpdateFeatureSettings();
			// Delete all widgets so each time this screen gets accessed, we must re-establish the list becuase it might change
			// due to RNet Enabled/Disabled.
			for (feature = 0; feature < NUM_FEATURES; ++feature)
			{
				if (&g_MainScreenFeatureInfo[feature].m_PromptWidget != NULL)
				{
					myErr = gx_widget_delete((GX_WIDGET*) &g_MainScreenFeatureInfo[feature].m_PromptWidget);
				}
				if (&g_MainScreenFeatureInfo[feature].m_Checkbox != NULL)
				{
					myErr = gx_widget_delete((GX_WIDGET*) &g_MainScreenFeatureInfo[feature].m_Checkbox);
				}
				if (&g_MainScreenFeatureInfo[feature].m_ItemWidget != NULL)
				{
					myErr = gx_widget_delete((GX_WIDGET*) &g_MainScreenFeatureInfo[feature].m_ItemWidget);
				}
			}
			FeatureWindowPtr->FeatureSettingsScreen_FeatureListBox.gx_vertical_list_child_count = 0;
			//{
			//	//myChildWidget = &FeatureWindowPtr->FeatureSettingsScreen_FeatureListBox.gx_widget_first_child;
			//	myErr = gx_widget_delete ((GX_WIDGET*) FeatureWindowPtr->FeatureSettingsScreen_FeatureListBox.gx_widget_first_child);
			//}
			// THe following did nothing for my cause.
			//myEvent.gx_event_type = GX_EVENT_DELETE;
			//myErr = gx_vertical_list_event_process (&FeatureSettingsScreen.FeatureSettingsScreen_FeatureListBox, &myEvent);
			break;

	} // end switch

    myErr = gx_window_event_process(window, event_ptr);

	return 0;
}

//*************************************************************************************
// This function populates the Feature List.
//*************************************************************************************

void CreateFeatureWidgets (GX_VERTICAL_LIST *list)
{
 	int index;
	int activeFeatureCount;

	if (g_RNet_Active)
	{
		// Display as "RNet TOGGLE"
		g_MainScreenFeatureInfo[NEXT_FUNCTION_OR_TOGGLE_ID].m_LargeDescriptionID = GX_STRING_ID_RNET_TOGGLE;
		// Display as "RNET USER MENU"
		g_MainScreenFeatureInfo[NEXT_PROFILE_OR_USER_MENU_ID].m_LargeDescriptionID = GX_STRING_ID_RNET_MENU;
	}
	else
	{
		// Display as NEXT FUNCTION
		g_MainScreenFeatureInfo[NEXT_FUNCTION_OR_TOGGLE_ID].m_LargeDescriptionID = GX_STRING_ID_NEXT_FUNCTION;
		// Display as NEXT PROFILE
		g_MainScreenFeatureInfo[NEXT_PROFILE_OR_USER_MENU_ID].m_LargeDescriptionID = GX_STRING_ID_NEXT_PROFILE;
	}

	activeFeatureCount = 0;
	for (index = 0; index < NUM_FEATURES; ++index)
	{
		if (g_MainScreenFeatureInfo[index].m_Available)
		{
			FeatureList_callback (list, (GX_WIDGET*) &g_MainScreenFeatureInfo[index], index);
			++activeFeatureCount;
		}
 	}
	list->gx_vertical_list_total_rows = activeFeatureCount;
}











