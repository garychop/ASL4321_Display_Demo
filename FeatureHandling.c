//*****************************************************************************
// Filename: FeatureHandling.cpp
//
// Date: Feb 27, 2021
//
// Author: G. Chopcinski, Kg Solutions, LLC
// 
//*****************************************************************************

#include "ASL4321_System.h"
#include "custom_checkbox.h"
#include "DataDictionary.h"
#include "FeatureHandling.h"

//*****************************************************************************

FEATURE_GUI_STRUCT g_Feature_GUI[MAX_FEATURES];

//*****************************************************************************

VOID InitializeFeature_GUI_Info (VOID)
{
	g_Feature_GUI[POWER_ONOFF_ID].m_FeatureID = POWER_ONOFF_ID;
    g_Feature_GUI[POWER_ONOFF_ID].m_LargeDescriptionID = GX_STRING_ID_POWER_ONOFF; //"POWER ON/OFF"
    g_Feature_GUI[POWER_ONOFF_ID].m_SmallDescriptionID = GX_STRING_ID_POWER_ONOFF;
    g_Feature_GUI[POWER_ONOFF_ID].m_SmallIcon = GX_PIXELMAP_ID_POWERICON_30X30;
    g_Feature_GUI[POWER_ONOFF_ID].m_LargeIcon = GX_PIXELMAP_ID_POWERICON_LARGE;
	g_Feature_GUI[POWER_ONOFF_ID].m_FeatureIconID = GX_PIXELMAP_ID_DRIVING_60X50;

    // "Bluetooth" information and description
	g_Feature_GUI[BLUETOOTH_ID].m_FeatureID = BLUETOOTH_ID;
    g_Feature_GUI[BLUETOOTH_ID].m_LargeDescriptionID = GX_STRING_ID_BLUETOOTH;
    g_Feature_GUI[BLUETOOTH_ID].m_SmallDescriptionID = GX_STRING_ID_BLUETOOTH;
    g_Feature_GUI[BLUETOOTH_ID].m_SmallIcon = GX_PIXELMAP_ID_BLUETOOTH_30X30;
    g_Feature_GUI[BLUETOOTH_ID].m_LargeIcon = GX_PIXELMAP_ID_BLUETOOTH_70X70;
	g_Feature_GUI[BLUETOOTH_ID].m_FeatureIconID = GX_PIXELMAP_ID_BLUETOOTH_60X50;

    // "Next Function" information and description
	g_Feature_GUI[NEXT_FUNCTION_ID].m_FeatureID = NEXT_FUNCTION_ID;
    g_Feature_GUI[NEXT_FUNCTION_ID].m_LargeDescriptionID = GX_STRING_ID_NEXT_FUNCTION; // "NEXT FUNCTION")
    g_Feature_GUI[NEXT_FUNCTION_ID].m_SmallDescriptionID = GX_STRING_ID_NEXT_FUNCTION;
    g_Feature_GUI[NEXT_FUNCTION_ID].m_SmallIcon = GX_PIXELMAP_ID_FUNCTIONNEXT_30X30;
    g_Feature_GUI[NEXT_FUNCTION_ID].m_LargeIcon = GX_PIXELMAP_ID_FUNCTIONNEXT_70X70;
	g_Feature_GUI[NEXT_FUNCTION_ID].m_FeatureIconID = GX_PIXELMAP_ID_DRIVING_60X50;

    // "Next Profile" information and description
	g_Feature_GUI[NEXT_PROFILE_ID].m_FeatureID = NEXT_PROFILE_ID;
    g_Feature_GUI[NEXT_PROFILE_ID].m_LargeDescriptionID = GX_STRING_ID_NEXT_PROFILE; // "NEXT PROFILE"
    g_Feature_GUI[NEXT_PROFILE_ID].m_SmallDescriptionID = GX_STRING_ID_NEXT_PROFILE;
    g_Feature_GUI[NEXT_PROFILE_ID].m_SmallIcon = GX_PIXELMAP_ID_PROFILENEXT_30X30;
    g_Feature_GUI[NEXT_PROFILE_ID].m_LargeIcon = GX_PIXELMAP_ID_PROFILENEXT_70X70;
	g_Feature_GUI[NEXT_PROFILE_ID].m_FeatureIconID = GX_PIXELMAP_ID_DRIVING_60X50;

	// "RNet TOGGLE function" information and description
	g_Feature_GUI[RNET_TOGGLE_ID].m_FeatureID = RNET_TOGGLE_ID;
    g_Feature_GUI[RNET_TOGGLE_ID].m_LargeDescriptionID = GX_STRING_ID_RNET_TOGGLE; // "RNET TOGGLE"
    g_Feature_GUI[RNET_TOGGLE_ID].m_SmallDescriptionID = GX_STRING_ID_RNET_TOGGLE;
	g_Feature_GUI[RNET_TOGGLE_ID].m_SmallIcon = GX_PIXELMAP_ID_RNET_TOGGLEFR_30X30;
	g_Feature_GUI[RNET_TOGGLE_ID].m_LargeIcon = GX_PIXELMAP_ID_RNET_TOGGLEFR_70X70;
	g_Feature_GUI[RNET_TOGGLE_ID].m_FeatureIconID = GX_PIXELMAP_ID_DRIVING_60X50;

	// "RNet User Menu"
	g_Feature_GUI[RNET_USER_MENU_ID].m_FeatureID = RNET_USER_MENU_ID;
	g_Feature_GUI[RNET_USER_MENU_ID].m_LargeDescriptionID = GX_STRING_ID_RNET_MENU;
	g_Feature_GUI[RNET_USER_MENU_ID].m_SmallDescriptionID = GX_STRING_ID_RNET_MENU;
	g_Feature_GUI[RNET_USER_MENU_ID].m_SmallIcon = GX_PIXELMAP_ID_RNET_MENU_30X30;
	g_Feature_GUI[RNET_USER_MENU_ID].m_LargeIcon = GX_PIXELMAP_ID_RNET_MENU_70X70;
	g_Feature_GUI[RNET_USER_MENU_ID].m_FeatureIconID = GX_PIXELMAP_ID_BLANK_60X50;

	// "RNet SEATING" information and description
	g_Feature_GUI[RNET_SEATING_ID].m_FeatureID = RNET_SEATING_ID;
    g_Feature_GUI[RNET_SEATING_ID].m_LargeDescriptionID = GX_STRING_ID_RNET_SEATING;
    g_Feature_GUI[RNET_SEATING_ID].m_SmallDescriptionID = GX_STRING_ID_RNET_SEATING;
    g_Feature_GUI[RNET_SEATING_ID].m_SmallIcon = GX_PIXELMAP_ID_RNET_SEATING_30X30;
    g_Feature_GUI[RNET_SEATING_ID].m_LargeIcon = GX_PIXELMAP_ID_RNET_SEATING_A_70X70;
	g_Feature_GUI[RNET_SEATING_ID].m_FeatureIconID = GX_PIXELMAP_ID_BLANK_60X50;

    // "SLEEP" information and description
	g_Feature_GUI[RNET_SLEEP_FEATURE_ID].m_FeatureID = RNET_SLEEP_FEATURE_ID;
    g_Feature_GUI[RNET_SLEEP_FEATURE_ID].m_LargeDescriptionID = GX_STRING_ID_RNET_SLEEP;
    g_Feature_GUI[RNET_SLEEP_FEATURE_ID].m_SmallDescriptionID = GX_STRING_ID_RNET_SLEEP;
    g_Feature_GUI[RNET_SLEEP_FEATURE_ID].m_SmallIcon = GX_PIXELMAP_ID_RNET_SLEEP_30X30;
    g_Feature_GUI[RNET_SLEEP_FEATURE_ID].m_LargeIcon = GX_PIXELMAP_ID_RNET_SLEEP_70X70;
	g_Feature_GUI[RNET_SLEEP_FEATURE_ID].m_FeatureIconID = GX_PIXELMAP_ID_BLANK_60X50;

	// "NEXT GROUP" information and description
	g_Feature_GUI[NEXT_GROUP_ID].m_FeatureID = NEXT_GROUP_ID;
    g_Feature_GUI[NEXT_GROUP_ID].m_LargeDescriptionID = GX_STRING_ID_NEXT_GROUP;
    g_Feature_GUI[NEXT_GROUP_ID].m_SmallDescriptionID = GX_STRING_ID_NEXT_GROUP;
    g_Feature_GUI[NEXT_GROUP_ID].m_SmallIcon = GX_PIXELMAP_ID_NEXTGROUP_30X30_E2;
    g_Feature_GUI[NEXT_GROUP_ID].m_LargeIcon = GX_PIXELMAP_ID_NEXTGROUP_70X70_E2;
	g_Feature_GUI[NEXT_GROUP_ID].m_FeatureIconID = GX_PIXELMAP_ID_DRIVING_60X50;

	// "AUDIBLE OUT" information and description
	g_Feature_GUI[AUDIBLE_OUT_FEATURE_ID].m_FeatureID = AUDIBLE_OUT_FEATURE_ID;
    g_Feature_GUI[AUDIBLE_OUT_FEATURE_ID].m_LargeDescriptionID = GX_STRING_ID_AUDIBLE_OUTPUT;
    g_Feature_GUI[AUDIBLE_OUT_FEATURE_ID].m_SmallDescriptionID = GX_STRING_ID_BLANK;
    g_Feature_GUI[AUDIBLE_OUT_FEATURE_ID].m_SmallIcon = GX_PIXELMAP_ID_SPEAKER_30X30;
    g_Feature_GUI[AUDIBLE_OUT_FEATURE_ID].m_LargeIcon = GX_PIXELMAP_ID_SPEAKER_70X70;
	g_Feature_GUI[AUDIBLE_OUT_FEATURE_ID].m_FeatureIconID = GX_PIXELMAP_ID_SPEAKER_60X50;

	// SEATING informaton and description
	g_Feature_GUI[SEATING_FEATURE_ID].m_FeatureID = SEATING_FEATURE_ID;
    g_Feature_GUI[SEATING_FEATURE_ID].m_LargeDescriptionID = GX_STRING_ID_SEATING_ENABLE;
    g_Feature_GUI[SEATING_FEATURE_ID].m_SmallDescriptionID = GX_STRING_ID_BLANK;
    g_Feature_GUI[SEATING_FEATURE_ID].m_SmallIcon = GX_PIXELMAP_ID_SEATINGA_30X30;
    g_Feature_GUI[SEATING_FEATURE_ID].m_LargeIcon = GX_PIXELMAP_ID_SEATING_70X70;
	g_Feature_GUI[SEATING_FEATURE_ID].m_FeatureIconID = GX_PIXELMAP_ID_SEATING_60X50;
		
	// Tecla-E informaton and description
	g_Feature_GUI[TECLA_E_FEATURE_ID].m_FeatureID = TECLA_E_FEATURE_ID;
    g_Feature_GUI[TECLA_E_FEATURE_ID].m_LargeDescriptionID = GX_STRING_ID_TECLA_ENABLE;
    g_Feature_GUI[TECLA_E_FEATURE_ID].m_SmallDescriptionID = GX_STRING_ID_BLANK;
    g_Feature_GUI[TECLA_E_FEATURE_ID].m_SmallIcon = GX_PIXELMAP_ID_TECLAE_30X30;
    g_Feature_GUI[TECLA_E_FEATURE_ID].m_LargeIcon = GX_PIXELMAP_ID_TECLAE_70X70;
	g_Feature_GUI[TECLA_E_FEATURE_ID].m_FeatureIconID = GX_PIXELMAP_ID_TECLAE_60X50;


}
