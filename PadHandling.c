//*****************************************************************************
// Filename: ASL4321_System.h
// Description: This file declares function, typedefs and macros to the 
//		system wide usage.
//
// Author: G. Chopcinski, Kg Solutions, LLC
// 
//*****************************************************************************
#include <stdio.h>

#include "asl4321_display_demo_resources.h"
#include "asl4321_display_demo_specifications.h"
#include "custom_checkbox.h"
#include "PadInfo.h"

//*****************************************************************************

VOID InitializePadInformation()
{
	// Populate the default Pad settings.
	g_PadSettings[LEFT_PAD].m_PadDirection = LEFT_DIRECTION;
	g_PadSettings[LEFT_PAD].m_PadType = PROPORTIONAL_PADTYPE;
	g_PadSettings[LEFT_PAD].m_MinimumDriveValue = 5;
	strcpy_s (g_PadSettings[LEFT_PAD].m_MinimuDriveString, sizeof (g_PadSettings[LEFT_PAD].m_MinimuDriveString), "5%");
	g_PadSettings[LEFT_PAD].m_NeutralWindowValue = 5;
	strcpy_s (g_PadSettings[LEFT_PAD].m_NeutralWindowString, sizeof (g_PadSettings[LEFT_PAD].m_NeutralWindowString), "5%");
	g_PadSettings[LEFT_PAD].m_PadMinimumCalibrationValue = 2;
	g_PadSettings[LEFT_PAD].m_PadMaximumCalibrationValue = 30;
	g_PadSettings[LEFT_PAD].m_Speed_Offset = 0;
	g_PadSettings[LEFT_PAD].m_Direction_Offset = 0;
	//g_PadSettings[LEFT_PAD].m_DiagnosticWidigetLocation = g_DiagnosticWidgetLocations[LEFT_PAD];
	//g_PadSettings[LEFT_PAD].m_DiagnosticOff_Widget = &DiagnosticScreen.DiagnosticScreen_LeftPadOff_Button;
	//g_PadSettings[LEFT_PAD].m_DiagnosticProportional_Widget = &DiagnosticScreen.DiagnosticScreen_LeftPadProp_Button;
	//g_PadSettings[LEFT_PAD].m_DiagnosticDigital_Widget = &DiagnosticScreen.DiagnosticScreen_LeftPadDigital_Button;

	g_PadSettings[RIGHT_PAD].m_PadDirection = RIGHT_DIRECTION;
	g_PadSettings[RIGHT_PAD].m_PadType = PROPORTIONAL_PADTYPE;
	g_PadSettings[RIGHT_PAD].m_MinimumDriveValue = 10;
	strcpy_s (g_PadSettings[RIGHT_PAD].m_MinimuDriveString, sizeof (g_PadSettings[RIGHT_PAD].m_MinimuDriveString), "10%");
	g_PadSettings[RIGHT_PAD].m_NeutralWindowValue = 5;
	strcpy_s (g_PadSettings[RIGHT_PAD].m_NeutralWindowString, sizeof (g_PadSettings[RIGHT_PAD].m_NeutralWindowString), "N/A");
	g_PadSettings[RIGHT_PAD].m_PadMinimumCalibrationValue = 2;
	g_PadSettings[RIGHT_PAD].m_PadMaximumCalibrationValue = 30;
	g_PadSettings[RIGHT_PAD].m_Speed_Offset = 0;
	g_PadSettings[RIGHT_PAD].m_Direction_Offset = 0;
	//g_PadSettings[RIGHT_PAD].m_DiagnosticWidigetLocation = g_DiagnosticWidgetLocations[RIGHT_PAD];
	//g_PadSettings[RIGHT_PAD].m_DiagnosticOff_Widget = &DiagnosticScreen.DiagnosticScreen_RightPadOff_Button;
	//g_PadSettings[RIGHT_PAD].m_DiagnosticProportional_Widget = &DiagnosticScreen.DiagnosticScreen_RightPadProp_Button;
	//g_PadSettings[RIGHT_PAD].m_DiagnosticDigital_Widget = &DiagnosticScreen.DiagnosticScreen_RightPadDigital_Button;

	g_PadSettings[FORWARD_PAD].m_PadDirection = FORWARD_DIRECTION;
	g_PadSettings[FORWARD_PAD].m_PadType = PROPORTIONAL_PADTYPE;
	g_PadSettings[FORWARD_PAD].m_MinimumDriveValue = 15;
	strcpy_s (g_PadSettings[FORWARD_PAD].m_MinimuDriveString, sizeof (g_PadSettings[FORWARD_PAD].m_MinimuDriveString), "15%");
	g_PadSettings[FORWARD_PAD].m_NeutralWindowValue = 5;
	strcpy_s (g_PadSettings[FORWARD_PAD].m_NeutralWindowString, sizeof (g_PadSettings[FORWARD_PAD].m_NeutralWindowString), "N/A");
	g_PadSettings[FORWARD_PAD].m_PadMinimumCalibrationValue = 2;
	g_PadSettings[FORWARD_PAD].m_PadMaximumCalibrationValue = 30;
	g_PadSettings[FORWARD_PAD].m_Speed_Offset = 0;
	g_PadSettings[FORWARD_PAD].m_Direction_Offset = 0;
	//g_PadSettings[FORWARD_PAD].m_DiagnosticWidigetLocation = g_DiagnosticWidgetLocations[FORWARD_PAD];
	//g_PadSettings[FORWARD_PAD].m_DiagnosticOff_Widget = &DiagnosticScreen.DiagnosticScreen_CenterPadOff_Button;
	//g_PadSettings[FORWARD_PAD].m_DiagnosticProportional_Widget = &DiagnosticScreen.DiagnosticScreen_CenterPadProp_Button;
	//g_PadSettings[FORWARD_PAD].m_DiagnosticDigital_Widget = &DiagnosticScreen.DiagnosticScreen_CenterPadDigital_Button;

	g_PadSettings[REVERSE_PAD].m_PadDirection = REVERSE_DIRECTION;
	g_PadSettings[REVERSE_PAD].m_PadType = PROPORTIONAL_PADTYPE;
	g_PadSettings[REVERSE_PAD].m_MinimumDriveValue = 20;
	strcpy_s (g_PadSettings[REVERSE_PAD].m_MinimuDriveString, sizeof (g_PadSettings[RIGHT_PAD].m_MinimuDriveString), "20%");
	g_PadSettings[REVERSE_PAD].m_NeutralWindowValue = 5;
	strcpy_s (g_PadSettings[REVERSE_PAD].m_NeutralWindowString, sizeof (g_PadSettings[REVERSE_PAD].m_NeutralWindowString), "N/A");
	g_PadSettings[REVERSE_PAD].m_PadMinimumCalibrationValue = 2;
	g_PadSettings[REVERSE_PAD].m_PadMaximumCalibrationValue = 30;
	g_PadSettings[REVERSE_PAD].m_Speed_Offset = 0;
	g_PadSettings[REVERSE_PAD].m_Direction_Offset = 0;
	//g_PadSettings[REVERSE_PAD].m_DiagnosticWidigetLocation = g_DiagnosticWidgetLocations[RIGHT_PAD];
	//g_PadSettings[REVERSE_PAD].m_DiagnosticOff_Widget = &DiagnosticScreen.DiagnosticScreen_RightPadOff_Button;
	//g_PadSettings[REVERSE_PAD].m_DiagnosticProportional_Widget = &DiagnosticScreen.DiagnosticScreen_RightPadProp_Button;
	//g_PadSettings[REVERSE_PAD].m_DiagnosticDigital_Widget = &DiagnosticScreen.DiagnosticScreen_RightPadDigital_Button;

}

