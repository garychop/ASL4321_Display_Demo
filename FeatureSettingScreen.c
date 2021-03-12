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
#include "DataDictionary.h"
#include "asl4321_display_demo_resources.h"
#include "FeatureHandling.h"

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
VOID UpdateFeatureSettings (VOID);

//*************************************************************************************
// Function Name: FeatureSettingsScreen_event_process
//
// Description: This handles the Feature Settings Screen messages
//
//*************************************************************************************

UINT FeatureSettingsScreen_event_process (GX_WINDOW *window, GX_EVENT *event_ptr)
{
	UINT myErr = -1;
	int featureIndex;
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

			UpdateFeatureSettings();	// Update the settings in the Group

			// Delete all widgets so each time this screen gets accessed, we must re-establish the list becuase it might change
			// due to RNet Enabled/Disabled.
			for (featureIndex = 0; featureIndex < MAX_FEATURES; ++featureIndex)
			{
				if (&g_Feature_GUI[featureIndex].m_PromptWidget != NULL)
				{
					myErr = gx_widget_delete((GX_WIDGET*) &g_Feature_GUI[featureIndex].m_PromptWidget);
				}
				if (&g_Feature_GUI[featureIndex].m_Checkbox != NULL)
				{
					myErr = gx_widget_delete((GX_WIDGET*) &g_Feature_GUI[featureIndex].m_Checkbox);
				}
				if (&g_Feature_GUI[featureIndex].m_ItemWidget != NULL)
				{
					myErr = gx_widget_delete((GX_WIDGET*) &g_Feature_GUI[featureIndex].m_ItemWidget);
				}
			}
			FeatureWindowPtr->FeatureSettingsScreen_FeatureListBox.gx_vertical_list_child_count = 0;
			break;

	} // end switch

    myErr = gx_window_event_process(window, event_ptr);

	return 0;
}

//*************************************************************************************

VOID UpdateFeatureSettings (VOID)
{
	int featureIndex;
	USHORT group;
	int groupFeatureIndex;
	USHORT featureAttribute;
	USHORT tempFeatureID;

	group = dd_Get_USHORT (0, DD_GROUP);

	// For each feature in the group
	for (groupFeatureIndex = 0; groupFeatureIndex < MAX_FEATURES; ++groupFeatureIndex)
	{
		featureAttribute = dd_GetSubItem_USHORT (group, DD_GROUP_FEATURE_ATTRIBUTE, groupFeatureIndex);
		if ((featureAttribute & FEATURE_AVAILABLE) == FEATURE_AVAILABLE)
		//if (g_GroupInfo[group].m_GroupFeature[groupFeatureIndex].m_Available)
		{
			tempFeatureID = dd_GetSubItem_USHORT (group, DD_GROUP_FEATURE_ID, groupFeatureIndex);
			// Now locate the correct widget storage structure for this Feature. Search via Feature ID
			for (featureIndex = 0; featureIndex < MAX_FEATURES; ++featureIndex)
			{
				if (g_Feature_GUI[featureIndex].m_FeatureID == tempFeatureID)
				{
					if (g_Feature_GUI[featureIndex].m_Checkbox.gx_widget_style & GX_STYLE_BUTTON_PUSHED)
					{
						featureAttribute |= FEATURE_ENABLED;
					}
					else
					{
						featureAttribute ^= FEATURE_ENABLED;
					}
					dd_SetSubItem_USHORT (group, DD_GROUP_FEATURE_ATTRIBUTE, featureIndex, featureAttribute);
					break;
				}
			}
		}
	}
}

//*************************************************************************************
// Function Name: FeatureList_callback
// Description: This function handles drawing and processing the List Items.
// "widget" points to Specific Feature structure in the group,
// "index" contains the Feature ID.
//
//*************************************************************************************

VOID FeatureList_callback(GX_VERTICAL_LIST *list, GX_WIDGET *widget, INT featureID)
{
    GX_RECTANGLE childsize;
    FEATURE_GUI_STRUCT *feature = (FEATURE_GUI_STRUCT *)widget;
    GX_BOOL result;
	int featureIndex;

	// Locate Feature Structure that contains information about the Feature.
	for (featureIndex = 0; featureIndex < MAX_FEATURES; ++featureIndex)
	{
		if (g_Feature_GUI[featureIndex].m_FeatureID == featureID)
			break;
	}
	if (featureIndex < MAX_FEATURES)
	{
		gx_widget_created_test(&g_Feature_GUI[featureIndex].m_ItemWidget, &result);	// Test to see if the GUIX item has been created, I guess it's like a null test.

		if (!result)	// NOPE? Let create the item.
		{
			// First, make a Rectangle as a container for the Prompt and cool slider checkbox button.
			// The rectangle is an arbitrary location in space that contains all of the widgets for a given selection.
			gx_utility_rectangle_define(&childsize, 0, 0, 270, 52);	// 0,0,x,75 is too high... need to make a rectangle to declare it's space.
			gx_widget_create(&g_Feature_GUI[featureIndex].m_ItemWidget, NULL, (GX_WIDGET *)list, GX_STYLE_TRANSPARENT, GX_ID_NONE, &childsize);

			childsize.gx_rectangle_left = g_Feature_GUI[featureIndex].m_ItemWidget.gx_widget_size.gx_rectangle_right - 50;
			childsize.gx_rectangle_top = (52-27)/2;
			childsize.gx_rectangle_right = childsize.gx_rectangle_left + 58;	// "58" is width of background pic
			childsize.gx_rectangle_bottom = 47;
			custom_checkbox_create(&g_Feature_GUI[featureIndex].m_Checkbox, &g_Feature_GUI[featureIndex].m_ItemWidget, &checkbox_info, &childsize, TRUE);

			gx_utility_rectangle_define(&childsize, 0, 0, g_Feature_GUI[featureIndex].m_ItemWidget.gx_widget_size.gx_rectangle_right - 58, 52);
			gx_prompt_create(&g_Feature_GUI[featureIndex].m_PromptWidget, NULL, &g_Feature_GUI[featureIndex].m_ItemWidget, 0, GX_STYLE_TEXT_RIGHT | GX_STYLE_TRANSPARENT | GX_STYLE_BORDER_NONE | GX_STYLE_ENABLED, 0, &childsize);
			gx_prompt_text_color_set(&g_Feature_GUI[featureIndex].m_PromptWidget, GX_COLOR_ID_WHITE, GX_COLOR_ID_WHITE, GX_COLOR_ID_WHITE);
			gx_prompt_text_id_set(&g_Feature_GUI[featureIndex].m_PromptWidget, g_Feature_GUI[featureIndex].m_LargeDescriptionID);
		}
	}
	else
	{
		// ERROR!
	}
}

//*************************************************************************************
// This function populates the Feature List with this specific's groups enabled
// features.
//*************************************************************************************

void CreateFeatureWidgets (GX_VERTICAL_LIST *list)
{
 	int featureIndex;
	USHORT group, featureAttribute;
	int activeFeatureCount;
	USHORT tempFeatureID;

	group = dd_Get_USHORT (0, DD_GROUP);

	featureIndex = 0;
	activeFeatureCount = 0;
	for (featureIndex = 0; featureIndex < MAX_FEATURES; ++featureIndex)
	{
		featureAttribute = dd_GetSubItem_USHORT (group, DD_GROUP_FEATURE_ATTRIBUTE, featureIndex);
		if (featureAttribute & FEATURE_AVAILABLE)
		{
			tempFeatureID = dd_GetSubItem_USHORT (group, DD_GROUP_FEATURE_ID, featureIndex);
			FeatureList_callback (list, (GX_WIDGET*) &g_Feature_GUI[featureIndex], tempFeatureID);
			++activeFeatureCount;
		}
 	}
	list->gx_vertical_list_total_rows = activeFeatureCount;
}











