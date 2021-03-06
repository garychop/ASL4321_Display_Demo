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

#define MAX_NEUTRAL_WINDOW_VALUE (50)			// Percentage
#define MAX_ADVANCE_PAD_OPERATIONS (8)

typedef enum PHYSICAL_PAD {LEFT_PAD, RIGHT_PAD, FORWARD_PAD, REVERSE_PAD, MAX_PHYSICAL_PADS} PHYSICAL_PAD_ENUM;
typedef enum PAD_DIRECTION {OFF_DIRECTION = 0, FORWARD_DIRECTION, LEFT_DIRECTION, RIGHT_DIRECTION, REVERSE_DIRECTION, CYCLE_DIRECTION, MAX_DIRECTION} PAD_DIRECTION_ENUM;
typedef enum PAD_TYPE {PROPORTIONAL_PADTYPE, DIGITAL_PADTYPE, INVALID_PAD_TYPE} PAD_TYPE_ENUM;
typedef enum PAD_CYCLE_OPERATIONS {OFF_CYCLE, FORWARD_CYCLE, LEFT_CYCLE, RIGHT_CYCLE, REVERSE_CYCLE, POWER_ON_OFF_CYCLE, NEXT_FUNCTION_CYCLE, MAX_PAD_CYCLE_ENTRIES} PAD_CYCLE_OPERATIONS_ENUM;

typedef struct PAD_INFO_STRUCT_NAME
{
	PAD_TYPE_ENUM m_PadType;
	PAD_DIRECTION_ENUM m_PadDirection;
	PAD_CYCLE_OPERATIONS_ENUM m_LongPadOperations[MAX_ADVANCE_PAD_OPERATIONS];
	unsigned short m_MinimumDriveValue;
	unsigned short m_NeutralWindowValue;
	int m_PadMinimumCalibrationValue;
	int m_PadMaximumCalibrationValue;
	int m_Speed_Offset;					// This is an adjustment for Neutral offset a.k.a "Y offset"
	int m_Direction_Offset;				// This is veer adjustment, a.k.a. "X offset"
	//int m_DiagnosticOff_ID;
	//GX_PIXELMAP_BUTTON *m_DiagnosticOff_Widget;
	//int m_DiagnosticDigital_ID;
	//GX_PIXELMAP_BUTTON *m_DiagnosticDigital_Widget;
	//int m_DiagnosticProportional_ID;
	//GX_PIXELMAP_BUTTON *m_DiagnosticProportional_Widget;
	//GX_RECTANGLE m_DiagnosticWidigetLocation;
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


