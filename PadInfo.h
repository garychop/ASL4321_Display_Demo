//*****************************************************************************
// Filename: PadInfo.h
// Description: This file declares function, typedefs and macros to support
//		the Pad information
//
// Date: Aug 28, 2020.
//
// Author: G. Chopcinski, Kg Solutions, LLC
// 
//*****************************************************************************

#ifndef PAD_INFORMATON_H
#define PAD_INFORMATON_H

typedef enum PHYSICAL_PAD {LEFT_PAD, RIGHT_PAD, FORWARD_PAD, REVERSE_PAD, MAX_PHYSICAL_PADS} PHYSICAL_PAD_ENUM;
typedef enum PAD_DIRECTION {OFF_DIRECTION = 0, FORWARD_DIRECTION, LEFT_DIRECTION, RIGHT_DIRECTION, REVERSE_DIRECTION, MAX_DIRECTION} PAD_DIRECTION_ENUM;
typedef enum PAD_TYPE {PROPORTIONAL_PADTYPE, DIGITAL_PADTYPE, INVALID_PAD_TYPE} PAD_TYPE_ENUM;

typedef struct PAD_INFO_STRUCT_NAME
{
	PAD_TYPE_ENUM m_PadType;
	PAD_DIRECTION_ENUM m_PadDirection;
	unsigned short m_MinimumDriveValue;
	char m_MinimuDriveString[8];
	int m_PadMinimumCalibrationValue;
	int m_PadMaximumCalibrationValue;
	//int m_DiagnosticOff_ID;
	GX_PIXELMAP_BUTTON *m_DiagnosticOff_Widget;
	//int m_DiagnosticDigital_ID;
	GX_PIXELMAP_BUTTON *m_DiagnosticDigital_Widget;
	//int m_DiagnosticProportional_ID;
	GX_PIXELMAP_BUTTON *m_DiagnosticProportional_Widget;
	GX_RECTANGLE m_DiagnosticWidigetLocation;
//	GX_PIXELMAP_BUTTON *m_DirectionIcons[4];
} PAD_INFO_STRUCT;

//*****************************************************************************
// Global Variables
//*****************************************************************************

extern PAD_INFO_STRUCT g_PadSettings[];

//*****************************************************************************
// Function Prototypes
//*****************************************************************************

VOID InitializePadInformation();

#endif // PAD_INFORMATON_H


