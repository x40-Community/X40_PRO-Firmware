/**
* Copyright (C) 2021 Wiibooxtech Perron
*/


#ifndef WTDUGSSCREEN_FILAMENT_H
#define WTDUGSSCREEN_FILAMENT_H

#include "../WTDGUSScreenBase.h"
#include "../WTDGUSConfig.h"

#if defined MACHINE_X40V1_HT || defined MACHINE_X40V2_HT || defined MACHINE_X40V1_BMG_HT || defined MACHINE_X40V2_BMG_HT

class DGUS_Screen_Filament : public DGUS_Screen_Base
{
public:
	DGUS_Screen_Filament(void) { operation = FILAMENT_OPERATION_LOAD; }
	void Init(void);
	void Update(void);
	void KeyProcess(void);

	FILAMENT_OPERATION_ENUM operation;

private:

	enum FILAMENT_SCREEN_STATUS_ENUM
	{
		FSSE_CHOICETYPE = 0,
		FSSE_HEATTING = 1,
		FSSE_WAITTING = 2,
		FSSE_LOADING = 3,
		FSSE_COMPLETE = 4,
		FSSE_CHOICE_EXTRUDER = 5,
		FSSE_RETRACT = 6
	};

	enum FILAMENT_TYPE
	{
		FT_PLA = 0,
		FT_TPU = 1,
		FT_PETG = 2,
		FT_ASA = 3,
		FT_PCTG = 4,
		FT_CPE = 5
	};


	void ShowProcess(void);

	int heattime;
	FILAMENT_TYPE filamentType;
	int targetTemp;
	int16_t temp_nozzle0;
	int percent_current;
	int percent_last;
	uint8_t act_extruder;
	enum FILAMENT_SCREEN_STATUS_ENUM filaopsts;
};

#else

class DGUS_Screen_Filament : public DGUS_Screen_Base
{
public:
	DGUS_Screen_Filament(void) { operation = FILAMENT_OPERATION_LOAD; }
	void Init(void);
	void Update(void);
	void KeyProcess(void);

	FILAMENT_OPERATION_ENUM operation;

private:

	enum FILAMENT_SCREEN_STATUS_ENUM
	{
		FSSE_CHOICETYPE = 0,
		FSSE_HEATTING = 1,
		FSSE_WAITTING = 2,
		FSSE_LOADING = 3,
		FSSE_COMPLETE = 4,
		FSSE_CHOICE_EXTRUDER = 5,
		FSSE_RETRACT = 6
	};

	enum FILAMENT_TYPE
	{
		FT_NONOILEN = 0,
		FT_PLA = 1,
		FT_TPU = 2,
		FT_PETG = 3,
		FT_ASA = 4,
		FT_PCTG = 5
	};


	void ShowProcess(void);

	int heattime;
	FILAMENT_TYPE filamentType;
	int targetTemp;
	int16_t temp_nozzle0;
	int percent_current;
	int percent_last;
	uint8_t act_extruder;
	enum FILAMENT_SCREEN_STATUS_ENUM filaopsts;
};
#endif

#endif


