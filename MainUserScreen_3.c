//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#include "ASL4321_Simulator_Main.h"
#include "ASL4321_Display_Demo_specifications.h"

static int mycounter=0;

UINT MainUserScreen_3_EventFunction (GX_WINDOW *window, GX_EVENT *event_ptr)
{
	UINT myErr;

	switch (event_ptr->gx_event_type)
	{
	case GX_SIGNAL (CHANGE_SCREEN_BTN_ID, GX_EVENT_CLICKED):
		screen_toggle((GX_WINDOW *)&MainUserScreen, window);
		break;

	case GX_SIGNAL (LEFT_PAD_BUTTON, GX_EVENT_CLICKED):
		++mycounter;
		if (mycounter > 4)
			mycounter = 0;
		switch (mycounter)
		{
		case 0:
			gx_icon_button_pixelmap_set (&MainUserScreen_3.MainUserScreen_3_LeftPad_Button , GX_PIXELMAP_ID_RIGHTWHITEARROW); //GX_PIXELMAP_ID_RIGHTWHITEARROW, GX_PIXELMAP_ID_RIGHTWHITEARROW);
			break;
		case 1:
			gx_icon_button_pixelmap_set (&MainUserScreen_3.MainUserScreen_3_LeftPad_Button, GX_PIXELMAP_ID_LEFTWHITEARROW); // , GX_PIXELMAP_ID_LEFTWHITEARROW, GX_PIXELMAP_ID_LEFTWHITEARROW);
			break;
		case 2:
			gx_icon_button_pixelmap_set (&MainUserScreen_3.MainUserScreen_3_LeftPad_Button, GX_PIXELMAP_ID_UPWHITEARROW); // , GX_PIXELMAP_ID_UPWHITEARROW, GX_PIXELMAP_ID_UPWHITEARROW);
			break;
		case 3:
			gx_icon_button_pixelmap_set (&MainUserScreen_3.MainUserScreen_3_LeftPad_Button, GX_PIXELMAP_ID_PADDOWNARROW_ENABLED); // , GX_PIXELMAP_ID_PADDOWNARROW_ENABLED, GX_PIXELMAP_ID_PADDOWNARROW_ENABLED);
			break;
		case 4:
			gx_icon_button_pixelmap_set (&MainUserScreen_3.MainUserScreen_3_LeftPad_Button, GX_PIXELMAP_ID_FUNCTIONNEXT_70X70); // , GX_PIXELMAP_ID_PADDOWNARROW_ENABLED, GX_PIXELMAP_ID_PADDOWNARROW_ENABLED);
			break;
		default:
			break;
		} // end switch
		break;
	case GX_SIGNAL (RIGHT_PAD_RIGHT_ARROW_BTN_ID, GX_EVENT_CLICKED):
		break;
	}

    myErr = gx_window_event_process(window, event_ptr);

	return myErr;
}
