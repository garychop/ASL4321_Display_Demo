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
//#include "asl4321_display_demo_specifications.h"
//#include "ASL4321_System.h"
#include "custom_checkbox.h"

typedef struct FEATURE_GUI_STRUCTURE {
	FEATURE_ID_ENUM m_FeatureID;
    GX_RESOURCE_ID m_SmallIcon;
    GX_RESOURCE_ID m_LargeIcon;
    GX_RESOURCE_ID m_SmallDescriptionID;
    GX_RESOURCE_ID m_LargeDescriptionID;
    GX_RESOURCE_ID m_FeatureIconID;
	GX_RESOURCE_ID m_StringID;
	GX_WIDGET m_ItemWidget;
	GX_PROMPT m_PromptWidget;
	GX_CHECKBOX m_ButtonWidget;
    CUSTOM_CHECKBOX m_Checkbox;
} FEATURE_GUI_STRUCT;

//*****************************************************************************

extern FEATURE_GUI_STRUCT g_Feature_GUI[MAX_FEATURES];

//*****************************************************************************

extern VOID InitializeFeature_GUI_Info (VOID);

#endif // FEATURE_HANDLING_H

