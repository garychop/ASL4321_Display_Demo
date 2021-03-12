//*****************************************************************************
// Filename: HUB.c
// Description: This file contains the storage and interface representing
//		the HUB.
//
// Author: G. Chopcinski, Kg Solutions, LLC
// 
//*****************************************************************************

#include "HUB.h"

//*****************************************************************************

static USHORT g_RNet_Active = TRUE;
static USHORT g_Group = 0;
static USHORT g_ActiveFeature;

