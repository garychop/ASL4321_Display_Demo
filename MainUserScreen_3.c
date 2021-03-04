//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#include "ASL4321_Simulator_Main.h"
#include "ASL4321_Display_Demo_specifications.h"
#include "ASL4321_System.h"

#define MAX_SCAN_POSITION (8)

//static int mycounter=0;
static int g_ScanState = 0;
static SCAN_DIRECTION_ENUM gActiveScanDirection = SCAN_FORWARD;


//-----------------------------------------------------------------------------

static VOID NextScan (BOOL increment)
{
	GX_RECTANGLE myRect;

	if (increment)
		++gActiveScanDirection;
	if (gActiveScanDirection >= SCAN_MAX)
		gActiveScanDirection = (SCAN_DIRECTION_ENUM) 0;

//typedef enum SCAN_DIRECTION {SCAN_FORWARD, SCAN_LEFT, SCAN_RIGHT, SCAN_REVERSE, SCAN_VEER_FORWARD_LEFT, SCAN_VEER_FORWARD_RIGHT, SCAN_USER_PORT, SCAN_MAX} SCAN_DIRECTION_ENUM;
	switch (gActiveScanDirection)
	{
	case SCAN_FORWARD:		// Left, Top, Right, Bottom
        gx_utility_rectangle_define(&myRect, 128, 10, 228, 110);
		break;
	case SCAN_LEFT:		// Left, Top, Right, Bottom
        gx_utility_rectangle_define(&myRect, 28, 88, 128, 188);
		break;
	case SCAN_RIGHT:		// Left, Top, Right, Bottom
        gx_utility_rectangle_define(&myRect, 228, 88, 328, 188);
		break;
	case SCAN_REVERSE:
        gx_utility_rectangle_define(&myRect, 128, 166, 228, 266);
		break;
	//case SCAN_VEER_FORWARD_LEFT:
 //       gx_utility_rectangle_define(&myRect, 48, 16, 148, 116);
	//	break;
	//case SCAN_VEER_FORWARD_RIGHT:
 //       gx_utility_rectangle_define(&myRect, 208, 16, 308, 116);
	//	break;
	case SCAN_USER_PORT:
        gx_utility_rectangle_define(&myRect, 282, 172, 382, 272);
		break;
	default:
        gx_utility_rectangle_define(&myRect, 0, 0, 100, 100);
		break;
	} // end switch
	gx_widget_resize ((GX_WIDGET*) &MainUserScreen_3.MainUserScreen_3_TargetOrangeIcon, &myRect);

}

//-----------------------------------------------------------------------------

UINT MainUserScreen_3_EventFunction (GX_WINDOW *window, GX_EVENT *event_ptr)
{
	UINT myErr;

	switch (event_ptr->gx_event_type)
	{
	case GX_EVENT_SHOW:
		gx_system_timer_start(window, SCAN_TIMER_ID, 100, 0);
		NextScan(FALSE);
		break;

	case GX_SIGNAL (CHANGE_SCREEN_BTN_ID, GX_EVENT_CLICKED):
		gx_system_timer_stop(window, SCAN_TIMER_ID);
		screen_toggle((GX_WINDOW *)&MainUserScreen, window);
		break;

    case GX_EVENT_TIMER:
		if (g_ScanState == 0)	// we are scanning for the next operation
		{
			gx_system_timer_start(window, SCAN_TIMER_ID, 60, 0);	// re-arm timer
			NextScan(TRUE);
		}
		else	// we have timed out of the operation, change back to scanning
		{
			gx_icon_pixelmap_set (&MainUserScreen_3.MainUserScreen_3_TargetOrangeIcon , GX_PIXELMAP_ID_TARGETORANGE_100X100, GX_PIXELMAP_ID_TARGETORANGE_100X100);
			gx_system_timer_start(window, SCAN_TIMER_ID, 120, 0);	// re-arm timer
			g_ScanState = 0;
		}
		break;

	case GX_SIGNAL (SCAN_BTN_ID, GX_EVENT_CLICKED):
		NextScan(TRUE);
		break;

	case GX_SIGNAL (MODE_PORT_BTN_ID, GX_EVENT_CLICKED):
		switch (g_ScanState)
		{
		case 0:	// Change to Green target and arm timer.
			gx_icon_pixelmap_set (&MainUserScreen_3.MainUserScreen_3_TargetOrangeIcon , GX_PIXELMAP_ID_TARGETGREEN_100X100, GX_PIXELMAP_ID_TARGETGREEN_100X100);
			gx_system_timer_stop(window, SCAN_TIMER_ID);
			gx_system_timer_start(window, SCAN_TIMER_ID, 120, 0);	// re-arm timer
			g_ScanState = 1;
			break;
		case 1:
			gx_system_timer_start(window, SCAN_TIMER_ID, 120, 0);	// re-arm timer
			break;
		} // end switch
		break;

	case GX_SIGNAL (LEFT_PAD_BUTTON, GX_EVENT_CLICKED):
		//++mycounter;
		//if (mycounter > 4)
		//	mycounter = 0;
		//switch (mycounter)
		//{
		//case 0:
		//	gx_icon_button_pixelmap_set (&MainUserScreen_3.MainUserScreen_3_LeftPad_Button , GX_PIXELMAP_ID_RIGHTWHITEARROW); //GX_PIXELMAP_ID_RIGHTWHITEARROW, GX_PIXELMAP_ID_RIGHTWHITEARROW);
		//	break;
		//case 1:
		//	gx_icon_button_pixelmap_set (&MainUserScreen_3.MainUserScreen_3_LeftPad_Button, GX_PIXELMAP_ID_LEFTWHITEARROW); // , GX_PIXELMAP_ID_LEFTWHITEARROW, GX_PIXELMAP_ID_LEFTWHITEARROW);
		//	break;
		//case 2:
		//	gx_icon_button_pixelmap_set (&MainUserScreen_3.MainUserScreen_3_LeftPad_Button, GX_PIXELMAP_ID_UPWHITEARROW); // , GX_PIXELMAP_ID_UPWHITEARROW, GX_PIXELMAP_ID_UPWHITEARROW);
		//	break;
		//case 3:
		//	gx_icon_button_pixelmap_set (&MainUserScreen_3.MainUserScreen_3_LeftPad_Button, GX_PIXELMAP_ID_PADDOWNARROW_ENABLED); // , GX_PIXELMAP_ID_PADDOWNARROW_ENABLED, GX_PIXELMAP_ID_PADDOWNARROW_ENABLED);
		//	break;
		//case 4:
		//	gx_icon_button_pixelmap_set (&MainUserScreen_3.MainUserScreen_3_LeftPad_Button, GX_PIXELMAP_ID_FUNCTIONNEXT_70X70); // , GX_PIXELMAP_ID_PADDOWNARROW_ENABLED, GX_PIXELMAP_ID_PADDOWNARROW_ENABLED);
		//	break;
		//default:
		//	break;
		//} // end switch
		break;
	case GX_SIGNAL (RIGHT_PAD_BUTTON, GX_EVENT_CLICKED):
		break;
	}

    myErr = gx_window_event_process(window, event_ptr);

	return myErr;
}
