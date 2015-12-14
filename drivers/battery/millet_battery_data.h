#define CAPACITY_MAX      1000
#define CAPACITY_MAX_MARGIN 50
#define CAPACITY_MIN      0

int Temperature_fn(void)
{
	return 0;
}

static struct battery_data_t stc3117_battery_data[] = {
	{
		.Vmode= 0,		 /*REG_MODE, BIT_VMODE 1=Voltage mode, 0=mixed mode */
		.Alm_SOC = 1,		/* SOC alm level %*/
		.Alm_Vbat = 3000,	/* Vbat alm level mV*/
		.CC_cnf = 885,		/* nominal CC_cnf, coming from battery characterisation*/
		.VM_cnf = 897,		/* nominal VM cnf , coming from battery characterisation*/
		.Rint = 200,		/* nominal internal impedance*/
		.Cnom = 4450,		/* nominal capacity in mAh, coming from battery characterisation*/
		.Rsense = 10,		/* sense resistor mOhms*/
		.RelaxCurrent = 223, /* current for relaxation in mA (< C/20) */
		.Adaptive = 1,	   /* 1=Adaptive mode enabled, 0=Adaptive mode disabled */

		/* Elentec Co Ltd Battery pack - 80 means 8% */
		.CapDerating[6] = 270,				/* capacity derating in 0.1%, for temp = -20C */
		.CapDerating[5] = 150,				/* capacity derating in 0.1%, for temp = -10C */
		.CapDerating[4] = 75,			   /* capacity derating in 0.1%, for temp = 0C */
		.CapDerating[3] = 60,			   /* capacity derating in 0.1%, for temp = 10C */
		.CapDerating[2] = 0,			  /* capacity derating in 0.1%, for temp = 25C */
		.CapDerating[1] = 0,			  /* capacity derating in 0.1%, for temp = 40C */
		.CapDerating[0] = 0,			  /* capacity derating in 0.1%, for temp = 60C */

		.OCVValue[15] = 4298,			 /* OCV curve adjustment */
		.OCVValue[14] = 4189,			 /* OCV curve adjustment */
		.OCVValue[13] = 4093,			 /* OCV curve adjustment */
		.OCVValue[12] = 3994,			 /* OCV curve adjustment */
		.OCVValue[11] = 3962,			 /* OCV curve adjustment */
		.OCVValue[10] = 3898,			 /* OCV curve adjustment */
		.OCVValue[9] = 3842,			 /* OCV curve adjustment */
		.OCVValue[8] = 3804,			 /* OCV curve adjustment */
		.OCVValue[7] = 3773,			 /* OCV curve adjustment */
		.OCVValue[6] = 3756,			 /* OCV curve adjustment */
		.OCVValue[5] = 3736,			 /* OCV curve adjustment */
		.OCVValue[4] = 3697,			 /* OCV curve adjustment */
		.OCVValue[3] = 3684,			 /* OCV curve adjustment */
		.OCVValue[2] = 3680,			 /* OCV curve adjustment */
		.OCVValue[1] = 3612,			 /* OCV curve adjustment */
		.OCVValue[0] = 3300,			 /* OCV curve adjustment */

		/*if the application temperature data is preferred than the STC3117 temperature*/
		.ExternalTemperature = Temperature_fn, /*External temperature fonction, return C*/
		.ForceExternalTemperature = 0, /* 1=External temperature, 0=STC3117 temperature */
	}
};

static sec_bat_adc_table_data_t temp_table[] = {
  {26437, 900},
  {27225, 800},
  {28019, 700},
  {28378, 650},
  {28738, 620},
  {28917, 600},
  {29126, 580},
  {29576, 550},
  {30027, 500},
  {30420, 470},
  {30675, 450},
  {30953, 430},
  {31375, 400},
  {32149, 350},
  {32924, 300},
  {33849, 250},
  {34775, 200},
  {35694, 150},
  {36613, 100},
  {37216, 50},
  {37820, 20},
  {38170, 0},
  {38727, -30},
  {38878, -50},
  {39589, -100},
  {40303, -150},
  {40584, -200},
};

#define TEMP_HIGH_THRESHOLD_EVENT  510
#define TEMP_HIGH_RECOVERY_EVENT   460
#define TEMP_LOW_THRESHOLD_EVENT   (-50)
#define TEMP_LOW_RECOVERY_EVENT    0
#define TEMP_HIGH_THRESHOLD_NORMAL 510
#define TEMP_HIGH_RECOVERY_NORMAL  460
#define TEMP_LOW_THRESHOLD_NORMAL  (-50)
#define TEMP_LOW_RECOVERY_NORMAL   0
#define TEMP_HIGH_THRESHOLD_LPM    510
#define TEMP_HIGH_RECOVERY_LPM     460
#define TEMP_LOW_THRESHOLD_LPM     (-50)
#define TEMP_LOW_RECOVERY_LPM      0

#if defined(CONFIG_BATTERY_SWELLING)
#define BATT_SWELLING_HIGH_TEMP_BLOCK		550
#define BATT_SWELLING_HIGH_TEMP_RECOV		450
#define BATT_SWELLING_LOW_TEMP_BLOCK		100
#define BATT_SWELLING_LOW_TEMP_RECOV		150
#define BATT_SWELLING_RECHG_VOLTAGE		4150
#define BATT_SWELLING_BLOCK_TIME	10 * 60 /* 10 min */
#endif
