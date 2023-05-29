/**
* Copyright (C) 2021 Wiibooxtech Perron
*/

#include "../../MarlinCore.h"
#include "../../module/temperature.h"
#include "../../sd/cardreader.h"
#include "../../module/planner.h"
#include "../../module/configuration_store.h"
#include "../../wtlib/WTCMD.h"
#include "../../gcode/parser.h"
#include "../../module/probe.h"
#include "../../module/stepper.h"
#include "WTDGUSScreen_RestoreSetting.h"
#include "../WTDGUSSerial.h"
#include "../WTDGUSConfig.h"
#include "../WTDGUSManager.h"

#ifdef DGUS_LCD

void DGUS_Screen_RestoreSetting::Init()
{
	dserial.LoadScreen(SCREEN_2BINFO);
	dserial.SendString(ADDR_2BINFO_TITLE, MMSG_FIRMWARE_PARAM_TITLE[wtvar_language], 30);
	dserial.SendInt16(ADDR_2BINFO_HELP_ICON, ENUM_HELP_ICON_NOHELP);
	dserial.SendString(ADDR_2BINFO_TEXT_BUTTON1, MMSG_FIRMWARE_PARAM_OPERATION[wtvar_language], 20);
	dserial.SendString(ADDR_2BINFO_TEXT_BUTTON2, MMSG_BACK[wtvar_language], 20);

	restoreConfirm = false;

	ShowReport();

	holdontime = getcurrenttime();
}

void DGUS_Screen_RestoreSetting::Update()
{


}

void DGUS_Screen_RestoreSetting::KeyProcess()
{
	if (gltouchpara.validflg)
	{
		if (gltouchpara.address == ADDR_2BINFO_KEY)
		{	
			gltouchpara.validflg = false;
			if (gltouchpara.value == KEY_2BINFO_ITEM2)
			{
				if (restoreConfirm)
				{
					settings.reset();
					settings.save();				
					wt_restart();
				}
				else
				{
					Goback();
				}
			}
			else if (gltouchpara.value == KEY_2BINFO_ITEM1)
			{
				if (restoreConfirm)
				{
					Goback();
				}
				else
				{
					LoadOperationMenu();
				}
			}
		}
		else if (gltouchpara.address == ADDR_HELPMENU_KEY)
		{	
			gltouchpara.validflg = false;
			if (gltouchpara.value == KEY_HELPMENU_BUTTON_RETURN)
			{
				Goback();
			}
			else if (gltouchpara.value == KEY_HELPMENU_BUTTON_ITEM1)
			{
				dgus.ShowLogMenu(this, MMSG_FIRMWARE_PARAM_SAVE[wtvar_language]);
				queue.enqueue_one_now("W216");
			}
			else if (gltouchpara.value == KEY_HELPMENU_BUTTON_ITEM2)
			{
				dgus.ShowLogMenu(this, MMSG_FIRMWARE_PARAM_SAVE[wtvar_language]);
				queue.enqueue_one_now("W217");
			}
			else if (gltouchpara.value == KEY_HELPMENU_BUTTON_ITEM3)
			{
				ShowConfirm();
			}
		}
		
	}
}

void DGUS_Screen_RestoreSetting::ShowReport(void)
{
	char* buffer = (char*)malloc(TEXTLEN_2BINFO_ITEM);

	memset(buffer, 0, TEXTLEN_2BINFO_ITEM);
	sprintf(buffer, "Home Offset: M218 T1 X%.2f Y%.2f Z%.2f\r\nZ-Probe Offset: M851 X%.2f Y%.2f Z%.2f\r\nE-Steps: M92 X%.2f Y%.2f Z%.2f E%.2f\r\nAcceleration: M201 X%.0f Y%.0f Z%.0f E%.0f\r\nMax Feedrate: M203 X%.0f Y%.0f Z%.0f E%.0f\r\nAcceleration: M204 P%.0f R%.0f T%.0f\r\nPID Settings: M301 P%.2f I%.2f D%.2f",
//	sprintf(buffer, "Home offset: M218 T1 X%.2f Y%.2f Z%.2f\r\nZ-Probe Offset: M851 X%.2f Y%.2f Z%.2f\r\nE-Steps: M92 X%.2f Y%.2f Z%.2f E%.2f\r\nAcceleration: M201 X%.0f Y%.0f Z%.0f E%.0f\r\nMax Feedrate: M203 X%.0f Y%.0f Z%.0f E%.0f\r\nAcceleration: M204 P%.0f R%.0f T%.0f\r\nLinear Advance: M900 K-factor %.2f",
					 LINEAR_UNIT(hotend_offset[1].x),
					 LINEAR_UNIT(hotend_offset[1].y),
					 LINEAR_UNIT(hotend_offset[1].z),
                     LINEAR_UNIT(probe.offset_xy.x),
					 LINEAR_UNIT(probe.offset_xy.y),
					 probe.offset.z,
                     LINEAR_UNIT(planner.settings.axis_steps_per_mm[X_AXIS]),
                     LINEAR_UNIT(planner.settings.axis_steps_per_mm[Y_AXIS]),
                     LINEAR_UNIT(planner.settings.axis_steps_per_mm[Z_AXIS]),
                     VOLUMETRIC_UNIT(planner.settings.axis_steps_per_mm[E_AXIS]),
					 LINEAR_UNIT(planner.settings.max_acceleration_mm_per_s2[X_AXIS]),
					 LINEAR_UNIT(planner.settings.max_acceleration_mm_per_s2[Y_AXIS]),
					 LINEAR_UNIT(planner.settings.max_acceleration_mm_per_s2[Z_AXIS]),
					 VOLUMETRIC_UNIT(planner.settings.max_acceleration_mm_per_s2[E_AXIS]),
                     LINEAR_UNIT(planner.settings.max_feedrate_mm_s[X_AXIS]),
					 LINEAR_UNIT(planner.settings.max_feedrate_mm_s[Y_AXIS]),
					 LINEAR_UNIT(planner.settings.max_feedrate_mm_s[Z_AXIS]),
					 VOLUMETRIC_UNIT(planner.settings.max_feedrate_mm_s[E_AXIS]),
					 LINEAR_UNIT(planner.settings.acceleration),
					 LINEAR_UNIT(planner.settings.retract_acceleration),
					 LINEAR_UNIT(planner.settings.travel_acceleration),
					 PID_PARAM(Kp, 0), 
					 unscalePID_i(PID_PARAM(Ki, 0)), 
					 unscalePID_d(PID_PARAM(Kd, 0)));
				//	 planner.extruder_advance_K[0]);
					 

	dserial.SendLongString(ADDR_2BINFO_TEXT_LINE, buffer, TEXTLEN_2BINFO_ITEM);

	free(buffer);
}

void DGUS_Screen_RestoreSetting::ShowConfirm(void)
{
	restoreConfirm = true;
	dserial.LoadScreen(SCREEN_2BINFO);
	dserial.SendLongString(ADDR_2BINFO_TEXT_LINE, MMSG_RESTORE_SETTING_LINE[wtvar_language], TEXTLEN_2BINFO_ITEM);

	dserial.SendString(ADDR_2BINFO_TEXT_BUTTON1, MMSG_NO[wtvar_language], TEXTLEN_2BINFO_BUTTON);
	dserial.SendString(ADDR_2BINFO_TEXT_BUTTON2, MMSG_YES[wtvar_language], TEXTLEN_2BINFO_BUTTON);
}

void DGUS_Screen_RestoreSetting::LoadOperationMenu(void)
{
	dserial.LoadScreen(SCREEN_HELPMENU);

	dserial.SendString(ADDR_HELPMENU_TEXT_TITLE, MMSG_FIRMWARE_PARAM_OPERATION[wtvar_language], TEXTLEN_HELPMENU_TITLE);
	dserial.SendString(ADDR_HELPMENU_TEXT_RETURN, MMSG_BACK[wtvar_language], TEXTLEN_HELPMENU_BUTTON);

	dserial.SendInt16(ADDR_HELPMENU_ICON1, ENUM_SETTING_TF_SAVE);
	dserial.SendString(ADDR_HELPMENU_ITEM1, MMSG_FIRMWARE_PARAM_SAVE[wtvar_language], TEXTLEN_HELPMENU_ITEM);

	dserial.SendInt16(ADDR_HELPMENU_ICON2, ENUM_SETTING_TF_LOAD);
	dserial.SendString(ADDR_HELPMENU_ITEM2, MMSG_FIRMWARE_PARAM_LOAD[wtvar_language], TEXTLEN_HELPMENU_ITEM);

	dserial.SendInt16(ADDR_HELPMENU_ICON3, ENUM_SETTING_RESTORE);
	dserial.SendString(ADDR_HELPMENU_ITEM3, MMSG_FIRMWARE_PARAM_RESTORE[wtvar_language], TEXTLEN_HELPMENU_ITEM);
}
#endif