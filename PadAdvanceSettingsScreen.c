//*****************************************************************************
// Filename: PadAdvanceSettingsScreen.cpp
//
// Date: Feb 18, 2021
//
// Author: G. Chopcinski, Kg Solutions, LLC
// 
//*****************************************************************************

#include "ASL4321_System.h"
#include "asl4321_display_demo_resources.h"
#include "custom_checkbox.h"

extern CUSTOM_CHECKBOX_INFO checkbox_info;
//{
//    TOGGLE_BUTTON_ID,
//    GX_PIXELMAP_ID_SWITCH_BG,
//    GX_PIXELMAP_ID_SWITCH_ACTIVE,
//    GX_PIXELMAP_ID_SWITCH_DISACTIVE,
//    4,
//    24 // 24
//};
typedef struct 
{
	GX_RESOURCE_ID m_StringID;
	GX_WIDGET m_ItemWidget;
	GX_PROMPT m_PromptWidget;
	GX_CHECKBOX m_ButtonWidget;
    CUSTOM_CHECKBOX m_Checkbox;
    int m_Enabled;      // Indicates if this feature is active.
} PAD_ADVANCE_STRUCT;

PAD_ADVANCE_STRUCT g_PadFeature_StringID[8];

//GX_RESOURCE_ID g_PadFeature_StringID[] =
//{
//	GX_STRING_ID_STRING_77,		// "OFF"
//	GX_STRING_ID_STRING_71,			// "FORWARD"
//	GX_STRING_ID_STRING_70,			// "LEFT"
//	GX_STRING_ID_STRING_72,			// "RIGHT"
//	GX_STRING_ID_REVERSE_STRING,	// "REVERSE"
//	GX_STRING_ID_POWER_ONOFF,		// "POWER ON/OFF"
//	GX_STRING_ID_NEXT_FUNCTION		// "NEXT FUNCTION"
//};

//*************************************************************************************
// Local/Global variables
//*************************************************************************************

//*************************************************************************************
// Forward Declarations
//*************************************************************************************

void CreateAdvancedPadFeatureWidgets (GX_VERTICAL_LIST *list);

//*************************************************************************************

VOID InitializeAdvancedFeatureStruct (VOID)
{
	// "OFF"
	g_PadFeature_StringID[0].m_Enabled = FALSE;
	// FORWARD
	g_PadFeature_StringID[1].m_Enabled = TRUE;
	g_PadFeature_StringID[1].m_StringID = GX_STRING_ID_STRING_71;
	// LEFT
	g_PadFeature_StringID[2].m_Enabled = TRUE;
	g_PadFeature_StringID[2].m_StringID = GX_STRING_ID_STRING_70;
	// RIGHT
	g_PadFeature_StringID[3].m_Enabled = TRUE;
	g_PadFeature_StringID[3].m_StringID = GX_STRING_ID_STRING_72;
	// REVERSE
	g_PadFeature_StringID[4].m_Enabled = TRUE;
	g_PadFeature_StringID[4].m_StringID = GX_STRING_ID_REVERSE_STRING;
	// POWER ON/OFF
	g_PadFeature_StringID[5].m_Enabled = TRUE;
	g_PadFeature_StringID[5].m_StringID = GX_STRING_ID_POWER_ONOFF;
	// NEXT FUNCITON
	g_PadFeature_StringID[6].m_Enabled = TRUE;
	g_PadFeature_StringID[6].m_StringID = GX_STRING_ID_NEXT_FUNCTION;
	// END OF LIST
	g_PadFeature_StringID[7].m_Enabled = FALSE;
	g_PadFeature_StringID[7].m_StringID = GX_STRING_ID_STRING_71;
}

//*************************************************************************************
// Function Name: FeatureList_callback
//
// Description: This function handles drawing and processing the List Items.
//
//*************************************************************************************

VOID AdvancedPadFeatureList_callback(GX_VERTICAL_LIST *list, GX_WIDGET *widget, INT index)
{
    GX_RECTANGLE childsize;
    PAD_ADVANCE_STRUCT *featureID = (PAD_ADVANCE_STRUCT *)widget;
    GX_BOOL result;

	gx_widget_created_test(&featureID->m_ItemWidget, &result);	// Test to see if the GUIX item has been created, I guess it's like a null test.

    if (!result)	// NOPE? Let create the item.
    {
		// First, make a Rectangle as a container for the Prompt and cool slider checkbox button.
		// The rectangle is an arbitrary location in space that contains all of the widgets for a given selection.
        gx_utility_rectangle_define(&childsize, 0, 0, 270, 52);	// 0,0,x,75 is too high... need to make a rectangle to declare it's space.
        gx_widget_create(&featureID->m_ItemWidget, NULL, (GX_WIDGET *)list, GX_STYLE_TRANSPARENT, GX_ID_NONE, &childsize);

		childsize.gx_rectangle_left = featureID->m_ItemWidget.gx_widget_size.gx_rectangle_right - 50;
		childsize.gx_rectangle_top = (52-27)/2;
		childsize.gx_rectangle_right = childsize.gx_rectangle_left + 58;	// "58" is width of background pic
		childsize.gx_rectangle_bottom = 47;
		custom_checkbox_create(&featureID->m_Checkbox, &featureID->m_ItemWidget, &checkbox_info, &childsize, featureID->m_Enabled);

        gx_utility_rectangle_define(&childsize, 0, 0, featureID->m_ItemWidget.gx_widget_size.gx_rectangle_right - 58, 52);
		gx_prompt_create(&featureID->m_PromptWidget, NULL, &featureID->m_ItemWidget, 0, GX_STYLE_TEXT_RIGHT | GX_STYLE_TRANSPARENT | GX_STYLE_BORDER_NONE | GX_STYLE_ENABLED, 0, &childsize);
        gx_prompt_text_color_set(&featureID->m_PromptWidget, GX_COLOR_ID_WHITE, GX_COLOR_ID_WHITE, GX_COLOR_ID_WHITE);
		gx_prompt_text_id_set(&featureID->m_PromptWidget, featureID->m_StringID);
	}

}

//*************************************************************************************
// This function gets the settings from the Feature List and populate the Feature
// settings.
//
//*************************************************************************************

void UpdateAdvancedPadFeatureSettings ()
{
}

//*************************************************************************************
// Function Name: FeatureSettingsScreen_event_process
//
// Description: This handles the Feature Settings Screen messages
//
//*************************************************************************************

UINT PadAdvanceScreen_event_process (GX_WINDOW *window, GX_EVENT *event_ptr)
{
	UINT myErr = -1;
	int feature;
	PADADVANCEDSCREEN_CONTROL_BLOCK *WindowPtr = (PADADVANCEDSCREEN_CONTROL_BLOCK*) window;

	//GX_EVENT myEvent;

	switch (event_ptr->gx_event_type)
	{
		case GX_EVENT_SHOW:
			InitializeAdvancedFeatureStruct();

			// This sets the correct Group Icon in the Group Button on this screen.
			SetGroupIcon (&PadAdvancedScreen.PadAdvancedScreen_GroupIconButton);

			CreateAdvancedPadFeatureWidgets (&WindowPtr->PadAdvancedScreen_FeatureListBox);
			if (WindowPtr->PadAdvancedScreen_FeatureListBox.gx_vertical_list_total_rows < 5)
				gx_widget_hide ((GX_WIDGET*) &WindowPtr->PadAdvancedScreen_FeatureList_vertical_scroll);
			break;

		case GX_SIGNAL(GROUP_ICON_BUTTON, GX_EVENT_CLICKED):
			break;

		case GX_SIGNAL(OK_BTN_ID, GX_EVENT_CLICKED):
			//gx_widget_show ((GX_WIDGET*) &FWindowPtr->PadAdvancedScreen_FeatureList_vertical_scroll);
	        screen_toggle((GX_WINDOW *)&SetPadDirectionScreen, window);
			// Delete all widgets so each time this screen gets accessed, we must re-establish the list becuase it might change
			// due to RNet Enabled/Disabled.
			for (feature = 0; feature < MAX_PAD_CYCLE_ENTRIES; ++feature)
			{
				if (&g_PadFeature_StringID[feature].m_PromptWidget != NULL)
				{
					myErr = gx_widget_delete((GX_WIDGET*) &g_PadFeature_StringID[feature].m_PromptWidget);
				}
				if (&g_PadFeature_StringID[feature].m_Checkbox != NULL)
				{
					myErr = gx_widget_delete((GX_WIDGET*) &g_PadFeature_StringID[feature].m_Checkbox);
				}
				if (&g_PadFeature_StringID[feature].m_ItemWidget != NULL)
				{
					myErr = gx_widget_delete((GX_WIDGET*) &g_PadFeature_StringID[feature].m_ItemWidget);
				}
			}
			WindowPtr->PadAdvancedScreen_FeatureListBox.gx_vertical_list_child_count = 0;
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

void CreateAdvancedPadFeatureWidgets (GX_VERTICAL_LIST *list)
{
 	int index;
	int activeFeatureCount;

	activeFeatureCount = 0;
	for (index = 0; index < MAX_PAD_CYCLE_ENTRIES; ++index)
	{
		if (g_PadFeature_StringID[index].m_Enabled)
		{
			AdvancedPadFeatureList_callback (list, (GX_WIDGET*) &g_PadFeature_StringID[index], index);
			++activeFeatureCount;
		}
 	}
	list->gx_vertical_list_total_rows = activeFeatureCount;
}

