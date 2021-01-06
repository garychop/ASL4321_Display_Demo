
#include <stdio.h>
#include "gx_api.h"

#include "ASL4321_Display_Demo_resources.h"
#include "ASL4321_Display_Demo_specifications.h"

//-----------------------------------------------------------------------------
// Global Function Declarations

extern VOID screen_toggle(GX_WINDOW *new_win, GX_WINDOW *old_win);

//extern DIAGNOSTICSCREEN_CONTROL_BLOCK DiagnosticScreen;
//
//-----------------------------------------------------------------------------

extern UINT win32_graphics_driver_setup_24xrgb(GX_DISPLAY *display);
VOID *memory_allocate(ULONG size);
void memory_free(VOID *mem);

//-----------------------------------------------------------------------------
// Global Variables

GX_WINDOW_ROOT           *root;

