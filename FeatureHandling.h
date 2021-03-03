//*****************************************************************************
// Filename: FeatureHandling.h
// Description: This file declares function, typedefs and macros to the 
//		for Feature Specific Information.
//
// Author: G. Chopcinski, Kg Solutions, LLC
// 
//*****************************************************************************

#ifndef FEATURE_HANDLING_H
#define FEATURE_HANDLING_H

#include <stdio.h>

#include "asl4321_display_demo_resources.h"
#include "asl4321_display_demo_specifications.h"

typedef enum FEATURE_ID {
	POWER_ONOFF_ID,
	BLUETOOTH_ID,
	NEXT_FUNCTION_ID,
	NEXT_PROFILE_ID,
	NEXT_GROUP_ID,
	RNET_TOGGLE_ID,
	RNET_USER_MENU_ID,
	RNET_SEATING_ID,
	RNET_SLEEP_FEATURE_ID,
	AUDIBLE_OUT_FEATURE_ID,
	SEATING_FEATURE_ID,
	TECLA_E_FEATURE_ID,
	MAX_FEATURES} FEATURE_ID_ENUM; // NUM_FEATURES must be last enum

typedef struct FEATURE_GUI_STRUCTURE {
	FEATURE_ID_ENUM m_FeatureID;
    GX_RESOURCE_ID m_SmallIcon;
    GX_RESOURCE_ID m_LargeIcon;
    GX_RESOURCE_ID m_SmallDescriptionID;
    GX_RESOURCE_ID m_LargeDescriptionID;
    GX_RESOURCE_ID m_FeatureIconID;
	//union FEATURE_SUBSET {
	//	int myA;
	//	int myB;
	//} m_Subset;
} FEATURE_GUI_STRUCT;

//*****************************************************************************

extern FEATURE_GUI_STRUCT g_Feature_GUI[MAX_FEATURES];

//*****************************************************************************

extern VOID InitializeFeature_GUI_Info (VOID);

#endif // FEATURE_HANDLING_H

