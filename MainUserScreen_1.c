//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#include "ASL4321_Simulator_Main.h"
#include "ASL4321_Display_Demo_specifications.h"

static int mycounter=0;
static int g_ActiveFunction = 0;	// Start with POWER ON/OFF at top of Function Window

int g_FunctionIcon[][2] = 
{
	{GX_PIXELMAP_ID_POWERICON_30X30, GX_PIXELMAP_ID_POWERICON_LARGE}
};

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

void DrawFunctionWindow ()
{
	switch (g_ActiveFunction)
	{
	case 0:	// Top function is POWER ON/OFF
		gx_icon_pixelmap_set (&MainUserScreen_1.MainUserScreen_1_Function0_Icon , GX_PIXELMAP_ID_POWERICON_LARGE, GX_PIXELMAP_ID_POWERICON_LARGE);
		gx_icon_button_pixelmap_set (&MainUserScreen_1.MainUserScreen_1_Function1 , GX_PIXELMAP_ID_BLUETOOTH_30X30);
		gx_icon_button_pixelmap_set (&MainUserScreen_1.MainUserScreen_1_Function2 , GX_PIXELMAP_ID_FUNCTIONNEXT_30X30);
		gx_icon_button_pixelmap_set (&MainUserScreen_1.MainUserScreen_1_Function3 , GX_PIXELMAP_ID_PROFILENEXT_30X30);
		gx_icon_button_pixelmap_set (&MainUserScreen_1.MainUserScreen_1_Function4 , GX_PIXELMAP_ID_SPEAKER_30X30);
		break;
	case 1: // Top function is BLUETOOTH
		gx_icon_button_pixelmap_set (&MainUserScreen_1.MainUserScreen_1_Function4 , GX_PIXELMAP_ID_POWERICON_30X30);
		gx_icon_pixelmap_set (&MainUserScreen_1.MainUserScreen_1_Function0_Icon , GX_PIXELMAP_ID_BLUETOOTH_70X70, GX_PIXELMAP_ID_BLUETOOTH_70X70);
		gx_icon_button_pixelmap_set (&MainUserScreen_1.MainUserScreen_1_Function1 , GX_PIXELMAP_ID_FUNCTIONNEXT_30X30);
		gx_icon_button_pixelmap_set (&MainUserScreen_1.MainUserScreen_1_Function2 , GX_PIXELMAP_ID_PROFILENEXT_30X30);
		gx_icon_button_pixelmap_set (&MainUserScreen_1.MainUserScreen_1_Function3 , GX_PIXELMAP_ID_SPEAKER_30X30);
		break;
	case 2: // Top function is NEXT FUNCTION
		gx_icon_button_pixelmap_set (&MainUserScreen_1.MainUserScreen_1_Function3 , GX_PIXELMAP_ID_POWERICON_30X30);
		gx_icon_button_pixelmap_set (&MainUserScreen_1.MainUserScreen_1_Function4 , GX_PIXELMAP_ID_BLUETOOTH_30X30);
		gx_icon_pixelmap_set (&MainUserScreen_1.MainUserScreen_1_Function0_Icon , GX_PIXELMAP_ID_FUNCTIONNEXT_70X70, GX_PIXELMAP_ID_FUNCTIONNEXT_70X70);
		gx_icon_button_pixelmap_set (&MainUserScreen_1.MainUserScreen_1_Function1 , GX_PIXELMAP_ID_PROFILENEXT_30X30);
		gx_icon_button_pixelmap_set (&MainUserScreen_1.MainUserScreen_1_Function2 , GX_PIXELMAP_ID_SPEAKER_30X30);
		break;
	case 3: // Top function is NEXT PROFILE
		gx_icon_button_pixelmap_set (&MainUserScreen_1.MainUserScreen_1_Function2 , GX_PIXELMAP_ID_POWERICON_30X30);
		gx_icon_button_pixelmap_set (&MainUserScreen_1.MainUserScreen_1_Function3 , GX_PIXELMAP_ID_BLUETOOTH_30X30);
		gx_icon_button_pixelmap_set (&MainUserScreen_1.MainUserScreen_1_Function4 , GX_PIXELMAP_ID_FUNCTIONNEXT_30X30);
		gx_icon_pixelmap_set (&MainUserScreen_1.MainUserScreen_1_Function0_Icon , GX_PIXELMAP_ID_PROFILENEXT_70X70, GX_PIXELMAP_ID_PROFILENEXT_70X70);
		gx_icon_button_pixelmap_set (&MainUserScreen_1.MainUserScreen_1_Function1 , GX_PIXELMAP_ID_SPEAKER_30X30);
		break;
	case 4: // Top function is AUDIBLE OUT
		gx_icon_button_pixelmap_set (&MainUserScreen_1.MainUserScreen_1_Function1 , GX_PIXELMAP_ID_POWERICON_30X30);
		gx_icon_button_pixelmap_set (&MainUserScreen_1.MainUserScreen_1_Function2 , GX_PIXELMAP_ID_BLUETOOTH_30X30);
		gx_icon_button_pixelmap_set (&MainUserScreen_1.MainUserScreen_1_Function3 , GX_PIXELMAP_ID_FUNCTIONNEXT_30X30);
		gx_icon_button_pixelmap_set (&MainUserScreen_1.MainUserScreen_1_Function4 , GX_PIXELMAP_ID_PROFILENEXT_30X30);
		gx_icon_pixelmap_set (&MainUserScreen_1.MainUserScreen_1_Function0_Icon , GX_PIXELMAP_ID_SPEAKER_70X70, GX_PIXELMAP_ID_SPEAKER_70X70);
		break;
	default: 
		break;
	} // end switch

}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

UINT MainUserScreen_1_EventFunction (GX_WINDOW *window, GX_EVENT *event_ptr)
{
	UINT myErr;

	switch (event_ptr->gx_event_type)
	{
	case GX_EVENT_SHOW:
		DrawFunctionWindow();
		break;

	case GX_SIGNAL (CHANGE_SCREEN_BTN_ID, GX_EVENT_CLICKED):
		screen_toggle((GX_WINDOW *)&MainUserScreen_3, window);
		break;

	case GX_SIGNAL (LEFT_PAD_BUTTON, GX_EVENT_CLICKED):
		++mycounter;
		if (mycounter > 4)
			mycounter = 0;
		switch (mycounter)
		{
		case 0:
			gx_icon_button_pixelmap_set (&MainUserScreen_1.MainUserScreen_1_LeftPad_Button , GX_PIXELMAP_ID_RIGHTWHITEARROW); //GX_PIXELMAP_ID_RIGHTWHITEARROW, GX_PIXELMAP_ID_RIGHTWHITEARROW);
			break;
		case 1:
			gx_icon_button_pixelmap_set (&MainUserScreen_1.MainUserScreen_1_LeftPad_Button, GX_PIXELMAP_ID_LEFTWHITEARROW); // , GX_PIXELMAP_ID_LEFTWHITEARROW, GX_PIXELMAP_ID_LEFTWHITEARROW);
			break;
		case 2:
			gx_icon_button_pixelmap_set (&MainUserScreen_1.MainUserScreen_1_LeftPad_Button, GX_PIXELMAP_ID_UPWHITEARROW); // , GX_PIXELMAP_ID_UPWHITEARROW, GX_PIXELMAP_ID_UPWHITEARROW);
			break;
		case 3:
			gx_icon_button_pixelmap_set (&MainUserScreen_1.MainUserScreen_1_LeftPad_Button, GX_PIXELMAP_ID_PADDOWNARROW_ENABLED); // , GX_PIXELMAP_ID_PADDOWNARROW_ENABLED, GX_PIXELMAP_ID_PADDOWNARROW_ENABLED);
			break;
		case 4:
			gx_icon_button_pixelmap_set (&MainUserScreen_1.MainUserScreen_1_LeftPad_Button, GX_PIXELMAP_ID_FUNCTIONNEXT_70X70); // , GX_PIXELMAP_ID_PADDOWNARROW_ENABLED, GX_PIXELMAP_ID_PADDOWNARROW_ENABLED);
			break;
		default:
			break;
		} // end switch
		break;
	case GX_SIGNAL (RIGHT_PAD_RIGHT_ARROW_BTN_ID, GX_EVENT_CLICKED):
		break;

	case GX_SIGNAL (USER_PORT_BUTTON_ID, GX_EVENT_CLICKED):
		if (++g_ActiveFunction > 4)
			g_ActiveFunction = 0;
		DrawFunctionWindow();
		break;
	}

    myErr = gx_window_event_process(window, event_ptr);

	return myErr;
}
