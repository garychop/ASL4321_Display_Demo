//****************************************************************************
// Filename: KeyboardScreen.h
// Description: This file declares function, typedefs and macros to the 
//		for the Keyboard
//
// Author: G. Chopcinski, Kg Solutions, LLC
// 
//*****************************************************************************

#ifndef KEYBOARD_SCREEN_H
#define KEYBOARD_SCREEN_H

VOID SetReturnWindow (GX_WINDOW *window);

VOID SetKeyboardData (char **str, UINT length);
VOID GetKeyboardData (char *str);

#endif // KEYBOARD_SCREEN_H

