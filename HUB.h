//*****************************************************************************
// Filename: HUB.h
// Description: This file contains the storage and interface representing
//		the HUB.
//
// Author: G. Chopcinski, Kg Solutions, LLC
// 
//*****************************************************************************

#ifndef HUB_H
#define HUB_H

#include <stdio.h>
#include "tx_port.h"
#include "ASL4321_System.h"

typedef struct AUDIBLE_SUBITEM_STRUCTURE {
	USHORT m_ForwardPad_SoundID;
	USHORT m_ReversePad_SoundID;
	USHORT m_LeftPad_SoundID;
	USHORT m_RightPad_SoundID;
} AUDIBLE_SUBITEM_STRUCT;

typedef struct FEATURE_GROUP_STRUCTURE {
	FEATURE_ID_ENUM m_FeatureID;
	USHORT m_Attribute;		// D0 = Available (1), D1 = Enabled (1), D3-D7 = Priority
	USHORT m_SelectedSubItem;
	union {
		AUDIBLE_SUBITEM_STRUCT m_Audible_Subitem[MAX_AUDIBLE_SUBITEMS];
	};
} FEATURE_GROUP_STRUCT;

typedef struct //GROUP_STRUCT_DEFINE
{
	DEVICE_TYPE_ENUM m_DeviceType;
	SCAN_MODE_ENUM m_ScanMode;
	PAD_INFO_STRUCT m_GroupPadInfo[MAX_PHYSICAL_PADS];
	FEATURE_GROUP_STRUCT m_GroupFeature[MAX_FEATURES];		// This contains the features and enabled/disabled settings.
} GROUP_STRUCT;
extern GROUP_STRUCT g_GroupInfo[MAX_GROUPS];


#endif // HUB_H
