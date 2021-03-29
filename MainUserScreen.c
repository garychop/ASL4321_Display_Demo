//*****************************************************************************
// Filename: MainUserScreen.c
// Description: This file handles the main screen functionality.
// Date: 2/24/21
// 
//*****************************************************************************

#include "ASL4321_Simulator_Main.h"
#include "ASL4321_Display_Demo_specifications.h"
#include "ASL4321_System.h"
#include "DataDictionary.h"
#include "FeatureHandling.h"

//*****************************************************************************
typedef struct FEATURE_GROUP_STRUCTURE {
	FEATURE_ID_ENUM m_FeatureID;
	USHORT m_Available;		// This is TRUE if this feature is available for enabling.
	USHORT m_Enabled;
	USHORT m_Priority;			// This is a zero-based number priority
} GUI_FEATURE_GROUP_STRUCT;

typedef struct //GROUP_STRUCT_DEFINE
{
	DEVICE_TYPE_ENUM m_DeviceType;
	SCAN_MODE_ENUM m_ScanMode;
	PAD_INFO_STRUCT m_GroupPadInfo[MAX_PHYSICAL_PADS];
	GUI_FEATURE_GROUP_STRUCT m_GroupFeature[MAX_FEATURES];		// This contains the features and enabled/disabled settings.
} GUI_GROUP_STRUCT;

typedef struct MAIN_SCREEN_FEATURE_STRUCT
{
	FEATURE_ID_ENUM m_FeatureID;
    int m_Location;     // This indicates the Main Screen location, 0=Top most, 3=bottom most
} MAIN_SCREEN_FEATURE;

//*****************************************************************************

unsigned char g_Inhibit_UpButtonResponse = FALSE;
static int g_GroupIcon = 0;
static DEVICE_TYPE_ENUM g_DeviceType;
static SCAN_MODE_ENUM g_ScanType;
MAIN_SCREEN_FEATURE g_MainScreenFeatureInfo[MAX_FEATURES];
int g_ScanState = 0;
SCAN_DIRECTION_ENUM g_ScanPosition;
USHORT g_Group;

//*****************************************************************************
// Forawrd Declaration, Prototypes

VOID StartScanning (BOOL atStart);
VOID NextScan (BOOL increment);

//*****************************************************************************

VOID Initialize_MainScreenInfo()
{
	int feature, featureIndex, priority, featureCount;
	USHORT rnet_active;
	USHORT featureAttribute;
	USHORT tempFeatureID;

	rnet_active = (dd_Get_USHORT(MAX_GROUPS, DD_RNET_ENABLE));
	g_Group = dd_Get_USHORT (MAX_GROUPS, DD_GROUP);
	dd_SetSubItem_USHORT (0, DD_ACTIVE_FEATURE_SUBITEM, (USHORT) AUDIBLE_OUT_FEATURE_ID, 0);		// Set the active feature to zero.

    // Populate the screen stuff.
	priority = 0;
	featureCount = 0;
	for (feature = 0; feature < MAX_FEATURES; ++ feature)
	{
		g_MainScreenFeatureInfo[feature].m_FeatureID = MAX_FEATURES;
		g_MainScreenFeatureInfo[feature].m_Location = 0;

		for (featureIndex = 0; featureIndex < MAX_FEATURES; ++ featureIndex)
		{
			featureAttribute = dd_GetSubItem_USHORT (g_Group, DD_GROUP_FEATURE_ATTRIBUTE, featureIndex);
			if ((featureAttribute & 0x0f) == (FEATURE_AVAILABLE | FEATURE_ENABLED))
			{
				if ((featureAttribute & FEATURE_PRIORITY_MASK) == (priority << 4))
				{
					tempFeatureID = dd_GetSubItem_USHORT (g_Group, DD_GROUP_FEATURE_ID, featureIndex);
					if (priority == 0)	// Is this the active feature?
						dd_Set_USHORT (MAX_GROUPS, DD_ACTIVE_FEATURE, (featureAttribute & FEATURE_PRIORITY_MASK) >> 4);	// Store in Data Dictionary
					g_MainScreenFeatureInfo[featureCount].m_FeatureID = (FEATURE_ID_ENUM) tempFeatureID;
					g_MainScreenFeatureInfo[featureCount].m_Location = featureCount;
					++featureCount;
					break;
				}
			}
		}
		++priority;
	}
}

//*************************************************************************************

UINT GetPadIcon (PAD_DIRECTION_ENUM direction)
{
	switch (direction)
	{
		case FORWARD_DIRECTION:
			return GX_PIXELMAP_ID_UPWHITEARROW;
			break;
		case LEFT_DIRECTION:
			return GX_PIXELMAP_ID_LEFTWHITEARROW;
			break;
		case RIGHT_DIRECTION:
			return GX_PIXELMAP_ID_RIGHTWHITEARROW;
			break;
		case REVERSE_DIRECTION:
			return GX_PIXELMAP_ID_DOWNWHITEARROW;
			break;
		case CYCLE_DIRECTION:
			return GX_PIXELMAP_ID_CYCLEBUTTON;
			break;
		default:
		case OFF_DIRECTION:
			return GX_PIXELMAP_ID_PAD_OFF;
			break;
	} // end switch
}

//*************************************************************************************

VOID AdvanceToNextFeature (VOID)
{
	UINT feature;
	UINT selectedFeature;
	UINT lineNumber;

	// This is necessary to prevent the subsequent "Clicked" message from advancing the feature when we are changing to the Programming screen.
	if (g_ChangeScreen_WIP)
	{
		g_ChangeScreen_WIP = FALSE;
		return;
	}
	// Count the number of active features to set a limit on location
	for (feature = 0; feature < MAX_FEATURES; ++feature)
	{
		if (g_MainScreenFeatureInfo[feature].m_Location == 1)		// We found the active one
			selectedFeature = feature;
	}

	// Move active feature to bottom of list and move all feature upward.
	lineNumber = 0;
	for (feature = 0; feature < MAX_FEATURES; ++feature)
	{
		if (g_MainScreenFeatureInfo[selectedFeature].m_FeatureID != MAX_FEATURES)
		{
			g_MainScreenFeatureInfo[selectedFeature].m_Location = lineNumber;
			if (lineNumber == 0)
				dd_Set_USHORT (MAX_GROUPS, DD_ACTIVE_FEATURE, g_MainScreenFeatureInfo[selectedFeature].m_FeatureID);
			++lineNumber;
			// Set to look at the next feature
		}
		++selectedFeature;
		if (selectedFeature >= MAX_FEATURES)
			selectedFeature = 0;
	}
}

//*************************************************************************************

VOID AdvanceToPreviousFeature(VOID)
{
	UINT feature;
	int selectedFeature;
	int lineNumber;
	int highestLineNumber;

	// This is necessary to prevent the subsequent "Clicked" message from advancing the feature when we are changing to the Programming screen.
	if (g_ChangeScreen_WIP)
	{
		g_ChangeScreen_WIP = FALSE;
		return;
	}
	// look for the item with the highest line number. That will become the new active item.
	highestLineNumber = 0;
	for (feature = 0; feature < MAX_FEATURES; ++feature)
	{
		if (g_MainScreenFeatureInfo[feature].m_Location > highestLineNumber)
		{
			highestLineNumber = g_MainScreenFeatureInfo[feature].m_Location;
			selectedFeature = feature;	// remember this items location in the array.
		}
	}

	lineNumber = 0;
	for (feature = 0; feature < MAX_FEATURES; ++feature)
	{
		if (g_MainScreenFeatureInfo[selectedFeature].m_FeatureID != MAX_FEATURES)
		{
			g_MainScreenFeatureInfo[selectedFeature].m_Location = lineNumber;
			if (lineNumber == 0)
				dd_Set_USHORT (MAX_GROUPS, DD_ACTIVE_FEATURE, g_MainScreenFeatureInfo[selectedFeature].m_FeatureID);
			++lineNumber;
		}
		// Set to look at the next feature
		++selectedFeature;
		if (selectedFeature >= MAX_FEATURES)
			selectedFeature = 0;
	}
}

//*****************************************************************************
// This locates the NEXT GROUP FEATURE and sets it active.
VOID SetToNextGroupFeature (VOID)
{
	int selectedFeature, feature;
	int lineNumber;

	selectedFeature = 0;
	for (feature = 0; feature < MAX_FEATURES; ++feature)
	{
		if (g_MainScreenFeatureInfo[feature].m_FeatureID == NEXT_GROUP_ID)
		{
			selectedFeature = feature;
			break;
		}
	}

	lineNumber = 0;
	for (feature = 0; feature < MAX_FEATURES; ++feature)
	{
		if (g_MainScreenFeatureInfo[selectedFeature].m_FeatureID != MAX_FEATURES)
		{
			g_MainScreenFeatureInfo[selectedFeature].m_Location = lineNumber;
			if (lineNumber == 0)
				dd_Set_USHORT (0, DD_ACTIVE_FEATURE, g_MainScreenFeatureInfo[selectedFeature].m_FeatureID);
			++lineNumber;
		}
		// Set to look at the next feature
		++selectedFeature;
		if (selectedFeature >= MAX_FEATURES)
			selectedFeature = 0;
	}
}

//*************************************************************************************

static VOID PositionPads (VOID)
{
	GX_RECTANGLE rectangle;

	if (dd_Get_USHORT (g_Group, DD_DEVICE_TYPE) == DEVICE_TYPE_HEAD_ARRAY)
	{
		// Feature Icon
		gx_utility_rectangle_define (&rectangle, 164, 98, 164+88, 98+70);	// Left, top, right, bottom
		gx_widget_resize (&MainUserScreen.MainUserScreen_FeatureIcon, &rectangle);
		// Left Button
		gx_utility_rectangle_define (&rectangle, 64, 90, 64+88, 90+70);	// Left, top, right, bottom
		gx_widget_resize (&MainUserScreen.MainUserScreen_Left_Icon, &rectangle);
		gx_utility_rectangle_define (&rectangle, 68, 120, 68+80, 120+36);	// Left, top, right, bottom
		gx_widget_resize (&MainUserScreen.MainUserScreen_LeftPad_SoundName, &rectangle);
		// Right Button
		gx_utility_rectangle_define (&rectangle, 242, 90, 242+88, 90+70);	// Left, top, right, bottom
		gx_widget_resize (&MainUserScreen.MainUserScreen_Right_Icon, &rectangle);
		gx_utility_rectangle_define (&rectangle, 246, 120, 246+80, 120+36);	// Left, top, right, bottom
		gx_widget_resize (&MainUserScreen.MainUserScreen_RightPad_SoundName, &rectangle);
		// Center/Forward Pad
		gx_utility_rectangle_define (&rectangle, 154, 160, 152+88, 160+70);	// Left, top, right, bottom
		gx_widget_resize (&MainUserScreen.MainUserScreen_Forward_Icon, &rectangle);
		gx_utility_rectangle_define (&rectangle, 158, 190, 158+80, 190+36);	// Left, top, right, bottom
		gx_widget_resize (&MainUserScreen.MainUserScreen_ForwardPad_SoundName, &rectangle);
		// Hide the revese button
		gx_widget_hide ((GX_WIDGET*) &MainUserScreen.MainUserScreen_ReversePad_SoundName);
		gx_widget_hide (&MainUserScreen.MainUserScreen_Reverse_Icon);
	}
	else	// Must be a joystick, show all 4 pads.
	{
		// Feature Icon
		gx_utility_rectangle_define (&rectangle, 164, 128, 164+88, 128+70);	// Left, top, right, bottom
		gx_widget_resize (&MainUserScreen.MainUserScreen_FeatureIcon, &rectangle);
		// Left Button
		gx_utility_rectangle_define (&rectangle, 66, 122, 66+88, 122+70);	// Left, top, right, bottom
		gx_widget_resize (&MainUserScreen.MainUserScreen_Left_Icon, &rectangle);
		gx_utility_rectangle_define (&rectangle, 70, 152, 70+80, 152+36);	// Left, top, right, bottom
		gx_widget_resize (&MainUserScreen.MainUserScreen_LeftPad_SoundName, &rectangle);
		// Right Button
		gx_utility_rectangle_define (&rectangle, 242, 122, 242+88, 122+70);	// Left, top, right, bottom
		gx_widget_resize (&MainUserScreen.MainUserScreen_Right_Icon, &rectangle);
		gx_utility_rectangle_define (&rectangle, 246, 152, 246+80, 152+36);	// Left, top, right, bottom
		gx_widget_resize (&MainUserScreen.MainUserScreen_RightPad_SoundName, &rectangle);
		// Center/Forward Pad
		gx_utility_rectangle_define (&rectangle, 154, 54, 154+88, 54+70);	// Left, top, right, bottom
		gx_widget_resize (&MainUserScreen.MainUserScreen_Forward_Icon, &rectangle);
		gx_utility_rectangle_define (&rectangle, 158, 84, 158+80, 84+36);	// Left, top, right, bottom
		gx_widget_resize (&MainUserScreen.MainUserScreen_ForwardPad_SoundName, &rectangle);
		// Reverse Button
		gx_utility_rectangle_define (&rectangle, 154, 190, 154+88, 190+70);	// Left, top, right, bottom
		gx_widget_resize (&MainUserScreen.MainUserScreen_Reverse_Icon, &rectangle);
		gx_utility_rectangle_define (&rectangle, 158, 220, 158+80, 220+36);	// Left, top, right, bottom
		gx_widget_resize (&MainUserScreen.MainUserScreen_ReversePad_SoundName, &rectangle);
		gx_widget_show (&MainUserScreen.MainUserScreen_Reverse_Icon);
		gx_widget_show ((GX_WIDGET*) &MainUserScreen.MainUserScreen_ReversePad_SoundName);
	}

}

//*****************************************************************************

VOID SetPadFeaturesIDs (GX_RESOURCE_ID forwardID, GX_RESOURCE_ID reverseID, GX_RESOURCE_ID leftID, GX_RESOURCE_ID rightID)
{
	if (forwardID == 0)
	{
		forwardID = GX_PIXELMAP_ID_BLANKBOXA_88X70;
	}
	if (reverseID == 0)
	{
		reverseID = GX_PIXELMAP_ID_BLANKBOXA_88X70;
	}
	if (leftID == 0)
	{
		leftID = GX_PIXELMAP_ID_BLANKBOXA_88X70;
	}
	if (rightID == 0)
	{
		rightID = GX_PIXELMAP_ID_BLANKBOXA_88X70;
	}
	gx_icon_pixelmap_set (&MainUserScreen.MainUserScreen_Forward_Icon, forwardID, forwardID);
	gx_icon_pixelmap_set (&MainUserScreen.MainUserScreen_Reverse_Icon, reverseID, reverseID);
	gx_icon_pixelmap_set (&MainUserScreen.MainUserScreen_Left_Icon, leftID, leftID);
	gx_icon_pixelmap_set (&MainUserScreen.MainUserScreen_Right_Icon, rightID, rightID);

	PositionPads();	// This positions the pads based upon 3 or 4 quadrant operation.
}

//*****************************************************************************
// DisplayDrivingPads displays the pads when driving.
//*****************************************************************************

VOID DisplayDrivingPads (VOID)
{
	PAD_DIRECTION_ENUM padDirection;
	USHORT forwardID, reverseID, leftID, rightID;

	gx_widget_show (&MainUserScreen.MainUserScreen_Forward_Icon);
	gx_widget_show (&MainUserScreen.MainUserScreen_Reverse_Icon);
	gx_widget_show (&MainUserScreen.MainUserScreen_Left_Icon);
	gx_widget_show (&MainUserScreen.MainUserScreen_Right_Icon);

	padDirection = (PAD_DIRECTION_ENUM) dd_GetSubItem_USHORT (g_Group, DD_PAD_DIRECTION, FORWARD_PAD);
	forwardID = GetPadIcon (padDirection);
	padDirection = (PAD_DIRECTION_ENUM) dd_GetSubItem_USHORT (g_Group, DD_PAD_DIRECTION, REVERSE_PAD);
	reverseID = GetPadIcon (padDirection);
	padDirection = (PAD_DIRECTION_ENUM) dd_GetSubItem_USHORT (g_Group, DD_PAD_DIRECTION, LEFT_PAD);
	leftID = GetPadIcon (padDirection);
	padDirection = (PAD_DIRECTION_ENUM) dd_GetSubItem_USHORT (g_Group, DD_PAD_DIRECTION, RIGHT_PAD);
	rightID = GetPadIcon (padDirection);

	SetPadFeaturesIDs (forwardID, reverseID, leftID, rightID);
}

//*****************************************************************************
// Display Audio information in pads
//*****************************************************************************
VOID HideAudioNames (VOID)
{
	gx_widget_hide ((GX_WIDGET*) &MainUserScreen.MainUserScreen_ForwardPad_SoundName);
	gx_widget_hide ((GX_WIDGET*) &MainUserScreen.MainUserScreen_ReversePad_SoundName);
	gx_widget_hide ((GX_WIDGET*) &MainUserScreen.MainUserScreen_LeftPad_SoundName);
	gx_widget_hide ((GX_WIDGET*) &MainUserScreen.MainUserScreen_RightPad_SoundName);
}

// Need permanent storage for these strings as dictated by the GUIX prompt constraints.
char g_SoundNames[MAX_PHYSICAL_PADS][SOUND_BITE_NAME_LENGTH];

VOID DisplayAudibleOutPads (VOID)
{
	USHORT activeSpeakerSubitem;
	USHORT soundID;

	gx_widget_show (&MainUserScreen.MainUserScreen_ForwardPad_SoundName);
	gx_widget_show (&MainUserScreen.MainUserScreen_ReversePad_SoundName);
	gx_widget_show (&MainUserScreen.MainUserScreen_LeftPad_SoundName);
	gx_widget_show (&MainUserScreen.MainUserScreen_RightPad_SoundName);

	// Get the Active Audio Level 1-8.
	activeSpeakerSubitem = dd_GetSubItem_USHORT (g_Group, DD_ACTIVE_FEATURE_SUBITEM, AUDIBLE_OUT_FEATURE_ID);

	// Get the pads assigned audio ID's (number);
	// Populate the forward pad with the Sound Bite Label
	soundID = dd_GetSubItem_USHORT (MAX_GROUPS, DD_ACTIVE_SPEAKER_SUBITEM_FORWARD, activeSpeakerSubitem);
	if (soundID < MAX_SOUND_BITES)
		dd_GetString (0, DD_SOUNDBITE_NAME, soundID, g_SoundNames[FORWARD_PAD]);
	else
		strcpy_s (g_SoundNames[FORWARD_PAD], 8, "---");
	gx_prompt_text_set ((GX_PROMPT*)&MainUserScreen.MainUserScreen_ForwardPad_SoundName, g_SoundNames[FORWARD_PAD]);

	// Populate the Reverse Pad
	soundID = dd_GetSubItem_USHORT (MAX_GROUPS, DD_ACTIVE_SPEAKER_SUBITEM_REVERSE, activeSpeakerSubitem);
	if (soundID < MAX_SOUND_BITES)
		dd_GetString (0, DD_SOUNDBITE_NAME, soundID, g_SoundNames[REVERSE_PAD]);
	else
		strcpy_s (g_SoundNames[REVERSE_PAD], 8, "---");
	gx_prompt_text_set ((GX_PROMPT*)&MainUserScreen.MainUserScreen_ReversePad_SoundName, g_SoundNames[REVERSE_PAD]);
	
	// Populate the Left Pad
	soundID = dd_GetSubItem_USHORT (MAX_GROUPS, DD_ACTIVE_SPEAKER_SUBITEM_LEFT, activeSpeakerSubitem);
	if (soundID < MAX_SOUND_BITES)
		dd_GetString (0, DD_SOUNDBITE_NAME, soundID, g_SoundNames[LEFT_PAD]);
	else
		strcpy_s (g_SoundNames[LEFT_PAD], 8, "---");
	gx_prompt_text_set ((GX_PROMPT*)&MainUserScreen.MainUserScreen_LeftPad_SoundName, g_SoundNames[LEFT_PAD]);

	// Populate the Rigt Pad
	soundID = dd_GetSubItem_USHORT (MAX_GROUPS, DD_ACTIVE_SPEAKER_SUBITEM_RIGHT, activeSpeakerSubitem);
	if (soundID < MAX_SOUND_BITES)
		dd_GetString (0, DD_SOUNDBITE_NAME, soundID, g_SoundNames[RIGHT_PAD]);
	else
		strcpy_s (g_SoundNames[RIGHT_PAD], 8, "---");
	gx_prompt_text_set ((GX_PROMPT*)&MainUserScreen.MainUserScreen_RightPad_SoundName, g_SoundNames[RIGHT_PAD]);

	SetPadFeaturesIDs (GX_PIXELMAP_ID_SPEAKER_88X70, GX_PIXELMAP_ID_SPEAKER_88X70, GX_PIXELMAP_ID_SPEAKER_88X70, GX_PIXELMAP_ID_SPEAKER_88X70);
}

//*****************************************************************************

void DisplayPadFeatures()
{
	UINT err;
	//SHORT featureCount;
	FEATURE_ID_ENUM featureID;

	//featureID = MAX_FEATURES;
	//for (featureCount = 0; featureCount < MAX_FEATURES; ++featureCount)
	//{
	//	if (g_MainScreenFeatureInfo[featureCount].m_FeatureID != MAX_FEATURES)
	//		if (g_MainScreenFeatureInfo[featureCount].m_Location == 0)
	//			featureID = g_MainScreenFeatureInfo[featureCount].m_FeatureID;	// use "featureID" as an abreviated variable
	//}
	featureID = (FEATURE_ID_ENUM) dd_Get_USHORT (g_Group, DD_ACTIVE_FEATURE);	// Get currently active feature

	switch (featureID)
	{
	case AUDIBLE_OUT_FEATURE_ID:
		DisplayAudibleOutPads();
		break;
	case SEATING_FEATURE_ID:
		HideAudioNames();		// Hide the Audio Prompt fields which are inside the Pad icons.
		gx_widget_show (&MainUserScreen.MainUserScreen_Forward_Icon);
		gx_widget_show (&MainUserScreen.MainUserScreen_Reverse_Icon);
		gx_widget_show (&MainUserScreen.MainUserScreen_Left_Icon);
		gx_widget_show (&MainUserScreen.MainUserScreen_Right_Icon);
		switch (g_ActiveSeatingGroup)
		{
		case 0:
			SetPadFeaturesIDs (GX_PIXELMAP_ID_PADUPARROW_DISABLED, GX_PIXELMAP_ID_PADDOWNARROW_DISABLED, GX_PIXELMAP_ID_TILTDOWN_88X70, GX_PIXELMAP_ID_TILTUP_88X70);
			break;
		case 1:
			SetPadFeaturesIDs (GX_PIXELMAP_ID_PADUPARROW_DISABLED, GX_PIXELMAP_ID_PADDOWNARROW_DISABLED, GX_PIXELMAP_ID_RECLINEDOWN_88X70, GX_PIXELMAP_ID_RECLINEUP_88X70);
			break;
		case 2:
			SetPadFeaturesIDs (GX_PIXELMAP_ID_PADUPARROW_DISABLED, GX_PIXELMAP_ID_PADDOWNARROW_DISABLED, GX_PIXELMAP_ID_SEATING_ELEVATE_DOWN_88X70, GX_PIXELMAP_ID_SEATING_ELEVATE_UP_88X70);
			break;
		} // end switch 
		break;
	case BLUETOOTH_ID:
		HideAudioNames();		// Hide the Audio Prompt fields which are inside the Pad icons.
		gx_widget_show (&MainUserScreen.MainUserScreen_Forward_Icon);
		gx_widget_show (&MainUserScreen.MainUserScreen_Reverse_Icon);
		gx_widget_show (&MainUserScreen.MainUserScreen_Left_Icon);
		gx_widget_show (&MainUserScreen.MainUserScreen_Right_Icon);
		switch (g_BluetoothGroup)
		{
		case 0:
			SetPadFeaturesIDs (GX_PIXELMAP_ID_MOUSEUPDOWN_88X70, GX_PIXELMAP_ID_PADDOWNARROW_DISABLED, GX_PIXELMAP_ID_MOUSELEFTCLICK_88X70, GX_PIXELMAP_ID_MOUSELEFT_88X70);
			break;
		case 1:
			SetPadFeaturesIDs (GX_PIXELMAP_ID_PADUPARROW_DISABLED, GX_PIXELMAP_ID_PADDOWNARROW_DISABLED, GX_PIXELMAP_ID_LEFTWHITEARROW, GX_PIXELMAP_ID_RIGHTWHITEARROW);
			break;
		} // end switch on Bluetooth group.
		break;
	case TECLA_E_FEATURE_ID:
		HideAudioNames();		// Hide the Audio Prompt fields which are inside the Pad icons.
		gx_widget_hide (&MainUserScreen.MainUserScreen_Forward_Icon);
		gx_widget_hide (&MainUserScreen.MainUserScreen_Reverse_Icon);
		gx_widget_hide (&MainUserScreen.MainUserScreen_Left_Icon);
		gx_widget_hide (&MainUserScreen.MainUserScreen_Right_Icon);
		switch (GetTeclaGroup())
		{
		case 0:
			err = gx_prompt_text_id_set ((GX_PROMPT*) &MainUserScreen.MainUserScreen_TeclaMain_Prompt, GX_STRING_ID_STRING_123);
			break;
		case 1:
			err = gx_prompt_text_id_set ((GX_PROMPT*) &MainUserScreen.MainUserScreen_TeclaMain_Prompt, GX_STRING_ID_STRING_124);
			break;
		case 2:
			err = gx_prompt_text_id_set ((GX_PROMPT*) &MainUserScreen.MainUserScreen_TeclaMain_Prompt, GX_STRING_ID_STRING_125);
			break;
		} // end swtich
		break;
	default:
		HideAudioNames();		// Hide the Audio Prompt fields which are inside the Pad icons.
		DisplayDrivingPads();
		break;
	}
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
	int enabledCount, lineCount;
	int feature, featureID;
	UINT myErr = GX_SUCCESS;

	// Preset the Function List iconds to blank
	gx_icon_pixelmap_set (&MainUserScreen.MainUserScreen_FeatureIcon, GX_PIXELMAP_ID_BLANK_60X50, GX_PIXELMAP_ID_BLANK_60X50);
    gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_Function0_IconButton, GX_PIXELMAP_ID_BLANK_30X30);
    gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_Function1_IconButton, GX_PIXELMAP_ID_BLANK_30X30);
    gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_Function2_IconButton, GX_PIXELMAP_ID_BLANK_30X30);
    gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_Function3_IconButton, GX_PIXELMAP_ID_BLANK_30X30);
    gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_Function4_IconButton, GX_PIXELMAP_ID_BLANK_30X30);

    // Count the number of active items so we can populate appropriately.
    // Hide the Non-Active features.
    enabledCount = 0;

    // Locate the first feature to display
	for (lineCount = 0; lineCount < 5; ++lineCount)
	{
	    for (feature = 0; feature < MAX_FEATURES; ++feature)
		{
			if (g_MainScreenFeatureInfo[feature].m_Location == lineCount)
			{
				++enabledCount;
				featureID = g_MainScreenFeatureInfo[feature].m_FeatureID;	// abreviated variable
				switch (g_MainScreenFeatureInfo[feature].m_Location)
				{
				case 0: // Show the first line
					myErr = gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_Function0_IconButton, g_Feature_GUI[featureID].m_LargeIcon);
					gx_icon_pixelmap_set (&MainUserScreen.MainUserScreen_FeatureIcon, g_Feature_GUI[featureID].m_FeatureIconID, g_Feature_GUI[featureID].m_FeatureIconID);
					if (featureID == TECLA_E_FEATURE_ID)
					{
						gx_widget_show (&MainUserScreen.MainUserScreen_TeclaMain_Prompt);
						gx_widget_hide (&MainUserScreen.MainUserScreen_FeatureIcon);
					}
					else	// It's something other than Tecla
					{
						gx_widget_hide (&MainUserScreen.MainUserScreen_TeclaMain_Prompt);
						gx_widget_show (&MainUserScreen.MainUserScreen_FeatureIcon);
					}
					break;
				case 1: // Show second line item
					myErr = gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_Function1_IconButton, g_Feature_GUI[featureID].m_SmallIcon);
					break;
				case 2: // Show third line item
					myErr = gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_Function2_IconButton, g_Feature_GUI[featureID].m_SmallIcon);
					break;
				case 3: // Show fourth line item
					myErr = gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_Function3_IconButton, g_Feature_GUI[featureID].m_SmallIcon);
					break;
				case 4: // Show fifth line item
					myErr = gx_icon_button_pixelmap_set (&MainUserScreen.MainUserScreen_Function4_IconButton, g_Feature_GUI[featureID].m_SmallIcon);
					break;
				}
				break;
			}
		}
	}

	// Show the Fusion Logo if no features are enabled.... We are just driving.
	MainUserScreen.MainUserScreen_Fusion_Button.gx_widget_size.gx_rectangle_top = 60;
	MainUserScreen.MainUserScreen_Fusion_Button.gx_widget_size.gx_rectangle_right = 310;
	MainUserScreen.MainUserScreen_Fusion_Button.gx_widget_size.gx_rectangle_bottom = 160;
	if (enabledCount == 0)
		MainUserScreen.MainUserScreen_Fusion_Button.gx_widget_size.gx_rectangle_left = 10;
	else
		MainUserScreen.MainUserScreen_Fusion_Button.gx_widget_size.gx_rectangle_left = 500;

	// Show the Device icon in the middle of the screen.
	SetDeviceIcon(&MainUserScreen.MainUserScreen_DeviceType_icon);
	
	// Display the group icon and set the function box color
	SetGroupIcon (&MainUserScreen.MainUserScreen_GroupIconButton);
	switch (g_Group)
	{
	case 0:
		gx_widget_fill_color_set (&MainUserScreen.MainUserScreen_FunctionWindow, GX_COLOR_ID_GREEN_ISH, GX_COLOR_ID_GREEN_ISH, GX_COLOR_ID_GREEN_ISH);
		break;
	case 1:
		gx_widget_fill_color_set (&MainUserScreen.MainUserScreen_FunctionWindow, GX_COLOR_ID_SELECTED_FILL, GX_COLOR_ID_SELECTED_FILL, GX_COLOR_ID_SELECTED_FILL);
		break;
	case 2:
		gx_widget_fill_color_set (&MainUserScreen.MainUserScreen_FunctionWindow, GX_COLOR_ID_BRIGHT_ORANGE, GX_COLOR_ID_BRIGHT_ORANGE, GX_COLOR_ID_BRIGHT_ORANGE);
		break;
	case 3:
		gx_widget_fill_color_set (&MainUserScreen.MainUserScreen_FunctionWindow, GX_COLOR_ID_MAROON, GX_COLOR_ID_MAROON, GX_COLOR_ID_MAROON);
		break;
	default:
		break;
	} // end of switch

	DisplayPadFeatures();

    return myErr;
}

//*************************************************************************************

UINT MainUserScreen_EventFunction (GX_WINDOW *window, GX_EVENT *event_ptr)
{
	UINT myErr;
	GX_RECTANGLE myRect;

	switch (event_ptr->gx_event_type)
	{
	case GX_EVENT_SHOW:
		Initialize_MainScreenInfo();
		InitializeFeature_GUI_Info();
		DisplayMainScreenActiveFeatures();

		// Determine if scanning is active.
		g_DeviceType = (DEVICE_TYPE_ENUM) dd_Get_USHORT (MAX_GROUPS, DD_DEVICE_TYPE);
		if ((g_DeviceType == DEVICE_TYPE_SINGLE_SWITCH) || (g_DeviceType == DEVICE_TYPE_TWO_SWITCH))
		{
			g_ScanType = (SCAN_MODE_ENUM) dd_Get_USHORT (MAX_GROUPS, DD_SCAN_MODE);
			gx_system_timer_start(window, SCAN_TIMER_ID, 120, 0);	// re-arm timer
			g_ScanState = 0;		// We are scanning.
			//gx_widget_show ((GX_WIDGET*) &MainUserScreen.MainUserScreen_TargetOrangeIcon);
			//gx_widget_show ((GX_WIDGET*) &MainUserScreen.MainUserScreen_UserPortIcon);
	        gx_utility_rectangle_define(&myRect, 288, 188, 288+88, 188+70);
			//gx_widget_resize ((GX_WIDGET*) &MainUserScreen.MainUserScreen_TargetOrangeIcon, &myRect);
			gx_widget_resize ((GX_WIDGET*) &MainUserScreen.MainUserScreen_UserPortIcon, &myRect);
			StartScanning (TRUE);
		}
		else
		{
	        gx_utility_rectangle_define(&myRect, 500,300,600,400);
//			gx_widget_hide ((GX_WIDGET*) &MainUserScreen.MainUserScreen_TargetOrangeIcon);
//			gx_widget_hide ((GX_WIDGET*) &MainUserScreen.MainUserScreen_UserPortIcon);
			gx_widget_resize ((GX_WIDGET*) &MainUserScreen.MainUserScreen_TargetOrangeIcon, &myRect);
			gx_widget_resize ((GX_WIDGET*) &MainUserScreen.MainUserScreen_UserPortIcon, &myRect);
			gx_system_timer_stop(window, SCAN_TIMER_ID);
		}
		break;

	case GX_SIGNAL (CHANGE_SCREEN_BTN_ID, GX_EVENT_CLICKED):
		screen_toggle((GX_WINDOW *)&MainUserScreen_3, window);
		break;

	case GX_SIGNAL (GROUP_ICON_BUTTON, GX_EVENT_CLICKED):
		SelectNextGroup();
		g_Group = dd_Get_USHORT (0, DD_GROUP);
		Initialize_MainScreenInfo();
		SetToNextGroupFeature();
		DisplayMainScreenActiveFeatures();
		break;

	case GX_EVENT_PEN_DOWN:	// We are going to determine if the PAD button is pressed and start a timer to increment the 
							// ... long time (2 seconds) and goto Programming if so.
		switch (event_ptr->gx_event_target->gx_widget_id)
		{
		case UP_ARROW_BTN_ID:
		case DOWN_ARROW_BTN_ID:
			gx_system_timer_start(window, ARROW_PUSHED_TIMER_ID, 50, 0);
			break;
		case USER_PORT_BUTTON_ID:
			gx_system_timer_start(window, USER_PORT_PUSHED_TIMER_ID, 40, 0);
			break;
		case JS_UP_BUTTON:
		case JS_LEFT_BUTTON:
		case JS_DOWN_BUTTON:
		case JS_RIGHT_BUTTON:
			StartJoystickOperation (event_ptr->gx_event_target->gx_widget_id);
			break;
		}
		break;
    case GX_EVENT_TIMER:
        if (event_ptr->gx_event_payload.gx_event_timer_id == ARROW_PUSHED_TIMER_ID)
		{
	        screen_toggle((GX_WINDOW *)&HHP_Start_Screen, window);
		}
        else if (event_ptr->gx_event_payload.gx_event_timer_id == USER_PORT_PUSHED_TIMER_ID)
		{
			AdvanceToNextFeature();
			DisplayMainScreenActiveFeatures();
			g_Inhibit_UpButtonResponse = TRUE;
		}
        else if (event_ptr->gx_event_payload.gx_event_timer_id == SCAN_TIMER_ID)
		{
			if (g_ScanState == 0)	// we are scanning for the next operation
			{
				gx_system_timer_start(window, SCAN_TIMER_ID, 60, 0);	// re-arm timer
				NextScan(TRUE);
			}
			else	// we have timed out of the operation, change back to scanning
			{
				gx_icon_pixelmap_set (&MainUserScreen.MainUserScreen_TargetOrangeIcon , GX_PIXELMAP_ID_TARGETORANGE_100X100, GX_PIXELMAP_ID_TARGETORANGE_100X100);
				gx_system_timer_start(window, SCAN_TIMER_ID, 120, 0);	// re-arm timer
				g_ScanState = 0;
			}
		}
		break;

	case GX_EVENT_PEN_UP:
		{
			// Just stop all timers.
			gx_system_timer_stop(window, USER_PORT_PUSHED_TIMER_ID);
			gx_system_timer_stop(window, ARROW_PUSHED_TIMER_ID);
			if (g_Inhibit_UpButtonResponse)
			{
				g_Inhibit_UpButtonResponse = FALSE;
			}
			else
			{
				switch (event_ptr->gx_event_target->gx_widget_id)
				{
				case USER_PORT_BUTTON_ID:
					switch (dd_Get_USHORT (0, DD_ACTIVE_FEATURE))
					{
					case POWER_ONOFF_ID:
						screen_toggle((GX_WINDOW *)&ReadyScreen, window);
						break;
					case NEXT_GROUP_ID:
						SelectNextGroup();
						g_Group = dd_Get_USHORT (0, DD_GROUP);
						Initialize_MainScreenInfo();
						SetToNextGroupFeature();
						DisplayMainScreenActiveFeatures();
						break;
					case AUDIBLE_OUT_FEATURE_ID:
						SelectNextAudioLevel();
						DisplayMainScreenActiveFeatures();
						break;
					case SEATING_FEATURE_ID:
						++g_ActiveSeatingGroup;
						if (g_ActiveSeatingGroup > 2)
							g_ActiveSeatingGroup = 0;
						DisplayMainScreenActiveFeatures();
						break;
					case BLUETOOTH_ID:
						++g_BluetoothGroup;
						if (g_BluetoothGroup > 1)
							g_BluetoothGroup = 0;
						DisplayMainScreenActiveFeatures();
						break;
					case TECLA_E_FEATURE_ID:
						AdvanceToNextTeclaGroup();
						DisplayMainScreenActiveFeatures();
						break;
					default:
						break;
					} // end switch
					break;
				case JS_UP_BUTTON:
				case JS_LEFT_BUTTON:
				case JS_DOWN_BUTTON:
				case JS_RIGHT_BUTTON:
					StopJoystickOperation (event_ptr->gx_event_target->gx_widget_id);
					break;
				} // end switch
			}
		}
		break;

	case GX_SIGNAL (UP_ARROW_BTN_ID, GX_EVENT_CLICKED):
		AdvanceToPreviousFeature();
		DisplayMainScreenActiveFeatures();
		break;

	case GX_SIGNAL (DOWN_ARROW_BTN_ID, GX_EVENT_CLICKED):
		AdvanceToNextFeature();
		DisplayMainScreenActiveFeatures();
		break;

	case GX_SIGNAL (USER_PORT_BUTTON_ID, GX_EVENT_CLICKED):
		gx_system_timer_stop(window, USER_PORT_PUSHED_TIMER_ID);
		break;

	case GX_SIGNAL (MODE_PORT_BTN_ID, GX_EVENT_CLICKED):
		if (g_ScanType != SCAN_OFF)
		{
			if (g_ScanState == 0)
			{
				gx_icon_pixelmap_set (&MainUserScreen.MainUserScreen_TargetOrangeIcon , GX_PIXELMAP_ID_TARGETGREEN_100X100, GX_PIXELMAP_ID_TARGETGREEN_100X100);
				gx_system_timer_stop(window, SCAN_TIMER_ID);
				gx_system_timer_start(window, SCAN_TIMER_ID, 180, 0);	// re-arm timer
				g_ScanState = 1;
			}
			else
			{
				gx_system_timer_start(window, SCAN_TIMER_ID, 120, 0);	// re-arm timer
			} // end switch (g_ScanState)
		}
		break;
	}

    myErr = gx_window_event_process(window, event_ptr);

	return myErr;
}

//*************************************************************************************
//*************************************************************************************
//							SCANNING ROUTINES
//*************************************************************************************
//*************************************************************************************

VOID DisplayScanningIcon (SCAN_DIRECTION_ENUM scanPosition)
{
	GX_RECTANGLE myRect;


	switch (scanPosition)
	{
	case SCAN_FORWARD:		// Left, Top, Right, Bottom
		//gx_widget_client_get ((GX_WIDGET*) &MainUserScreen.MainUserScreen_Forward_Icon, 0, &myRect);
		gx_widget_client_get ((GX_WIDGET*) &MainUserScreen.MainUserScreen_Forward_Icon, 0, &myRect);
		myRect.gx_rectangle_left -= 6;
		myRect.gx_rectangle_top -= 12;
		myRect.gx_rectangle_right = myRect.gx_rectangle_left + 100;
		myRect.gx_rectangle_bottom = myRect.gx_rectangle_top + 100;
        //gx_utility_rectangle_define(&myRect, 128, 10, 228, 110);
		break;
	case SCAN_LEFT:		// Left, Top, Right, Bottom
		gx_widget_client_get ((GX_WIDGET*) &MainUserScreen.MainUserScreen_Left_Icon, 0, &myRect);
		myRect.gx_rectangle_left -= 9;
		myRect.gx_rectangle_top -= 14;
		myRect.gx_rectangle_right = myRect.gx_rectangle_left + 100;
		myRect.gx_rectangle_bottom = myRect.gx_rectangle_top + 100;
        //gx_utility_rectangle_define(&myRect, 28, 88, 128, 188);
		break;
	case SCAN_RIGHT:		// Left, Top, Right, Bottom
		gx_widget_client_get ((GX_WIDGET*) &MainUserScreen.MainUserScreen_Right_Icon, 0, &myRect);
		myRect.gx_rectangle_left -= 9;
		myRect.gx_rectangle_top -= 14;
		myRect.gx_rectangle_right = myRect.gx_rectangle_left + 100;
		myRect.gx_rectangle_bottom = myRect.gx_rectangle_top + 100;
        //gx_utility_rectangle_define(&myRect, 228, 88, 328, 188);
		break;
	case SCAN_REVERSE:
		gx_widget_client_get ((GX_WIDGET*) &MainUserScreen.MainUserScreen_Reverse_Icon, 0, &myRect);
		myRect.gx_rectangle_left -= 6;
		myRect.gx_rectangle_top -= 12;
		myRect.gx_rectangle_right = myRect.gx_rectangle_left + 100;
		myRect.gx_rectangle_bottom = myRect.gx_rectangle_top + 100;
        //gx_utility_rectangle_define(&myRect, 128, 166, 228, 266);
		break;
	//case SCAN_VEER_FORWARD_LEFT:
	//	//gx_widget_client_get ((GX_WIDGET*) &MainUserScreen.MainUserScreen_Forward_Icon, 0, &myRect);
 //       gx_utility_rectangle_define(&myRect, 48, 16, 148, 116);
	//	break;
	//case SCAN_VEER_FORWARD_RIGHT:
	//	//gx_widget_client_get ((GX_WIDGET*) &MainUserScreen.MainUserScreen_Forward_Icon, 0, &myRect);
 //       gx_utility_rectangle_define(&myRect, 208, 16, 308, 116);
	//	break;
	case SCAN_USER_PORT:
		gx_widget_client_get ((GX_WIDGET*) &MainUserScreen.MainUserScreen_UserPortIcon, 0, &myRect);
		myRect.gx_rectangle_left -= 6;
		myRect.gx_rectangle_top -= 12;
		myRect.gx_rectangle_right = myRect.gx_rectangle_left + 100;
		myRect.gx_rectangle_bottom = myRect.gx_rectangle_top + 100;
        //gx_utility_rectangle_define(&myRect, 282, 172, 382, 272);
		break;
	default:
        gx_utility_rectangle_define(&myRect, 0, 0, 100, 100);
		break;
	} // end switch
	gx_widget_resize ((GX_WIDGET*) &MainUserScreen.MainUserScreen_TargetOrangeIcon, &myRect);

}

//*************************************************************************************

VOID StartScanning (BOOL atStart)
{
	gx_icon_pixelmap_set (&MainUserScreen.MainUserScreen_TargetOrangeIcon , GX_PIXELMAP_ID_TARGETORANGE_100X100, GX_PIXELMAP_ID_TARGETORANGE_100X100);

	if (atStart)
	{
		g_ScanPosition = SCAN_FORWARD;
		DisplayScanningIcon (g_ScanPosition);
	}
}

//*************************************************************************************
// Advances to the next scan item.
VOID NextScan (BOOL increment)
{
	if (increment)
		++g_ScanPosition;
	if (g_ScanPosition >= SCAN_MAX)
		g_ScanPosition = (SCAN_DIRECTION_ENUM) 0;

	DisplayScanningIcon (g_ScanPosition);
}

