//*****************************************************************************
// Filename: Custom_Checkbox.c
//
// Date: Sept 1, 2020
//
// Author: G. Chopcinski, Kg Solutions, LLC
// 
// This file was derived from the Express Logic Smart Watch demo code supplied
// with GUIX.
//
//*****************************************************************************

#ifndef CUSTOM_CHECKBOX_H
#define CUSTOM_CHECKBOX_H

#include "gx_api.h"

#define TOGGLE_BUTTON_ID		0xf0

/* Define custom checkbox structure. */
typedef struct CUSTOM_CHECBOX_STRUCT{
    GX_CHECKBOX_MEMBERS_DECLARE
    GX_RESOURCE_ID background_id;
    INT start_offset;
    INT end_offset;
    INT cur_offset;
}CUSTOM_CHECKBOX;

typedef struct CUSTOM_CHECKBOX_INFO_STRUCT{
    INT widget_id;
    GX_RESOURCE_ID background_id;
    GX_RESOURCE_ID checked_map_id;
    GX_RESOURCE_ID unchecked_map_id;
    INT start_offset;
    INT end_offset;
}CUSTOM_CHECKBOX_INFO;

typedef struct CUSTOM_MENU_BUTTON_STRUCT{
    GX_WIDGET m_MenuWidget;
	GX_PROMPT m_PromptWidget;
	GX_TEXT_BUTTON m_ButtonWidget;
	USHORT m_ButtonID;
	GX_RESOURCE_ID m_TextID;
	USHORT m_Enabled;
} CUSTOM_MENU_BUTTON;

VOID custom_checkbox_create(CUSTOM_CHECKBOX *button, GX_WIDGET *parent, CUSTOM_CHECKBOX_INFO *info, GX_RECTANGLE *size, int enabled);

#endif // CUSTOM_CHECKBOX_H