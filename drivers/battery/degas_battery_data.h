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
		.CC_cnf = 823,		/* nominal CC_cnf, coming from battery characterisation*/
		.VM_cnf = 632,		/* nominal VM cnf , coming from battery characterisation*/
		.Rint = 155,		/* nominal internal impedance*/
		.Cnom = 4000,		/* nominal capacity in mAh, coming from battery characterisation*/
		.Rsense = 10,		/* sense resistor mOhms*/
		.RelaxCurrent = 150, /* current for relaxation in mA (< C/20) */
		.Adaptive = 1,	   /* 1=Adaptive mode enabled, 0=Adaptive mode disabled */

		.CapDerating[6] = 600,				/* capacity derating in 0.1%, for temp = -20C */
		.CapDerating[5] = 300,				/* capacity derating in 0.1%, for temp = -10C */
		.CapDerating[4] = 80,			   /* capacity derating in 0.1%, for temp = 0C */
		.CapDerating[3] = 20,			   /* capacity derating in 0.1%, for temp = 10C */
		.CapDerating[2] = 0,			  /* capacity derating in 0.1%, for temp = 25C */
		.CapDerating[1] = 0,			  /* capacity derating in 0.1%, for temp = 40C */
		.CapDerating[0] = 0,			  /* capacity derating in 0.1%, for temp = 60C */

		.OCVValue[15] = 4312,			 /* OCV curve adjustment */
		.OCVValue[14] = 4187,			 /* OCV curve adjustment */
		.OCVValue[13] = 4089,			 /* OCV curve adjustment */
		.OCVValue[12] = 3990,			 /* OCV curve adjustment */
		.OCVValue[11] = 3959,			 /* OCV curve adjustment */
		.OCVValue[10] = 3906,			 /* OCV curve adjustment */
		.OCVValue[9] = 3839,			 /* OCV curve adjustment */
		.OCVValue[8] = 3801,			 /* OCV curve adjustment */
		.OCVValue[7] = 3772,			 /* OCV curve adjustment */
		.OCVValue[6] = 3756,			 /* OCV curve adjustment */
		.OCVValue[5] = 3738,			 /* OCV curve adjustment */
		.OCVValue[4] = 3707,			 /* OCV curve adjustment */
		.OCVValue[3] = 3685,			 /* OCV curve adjustment */
		.OCVValue[2] = 3681,			 /* OCV curve adjustment */
		.OCVValue[1] = 3629,			 /* OCV curve adjustment */
		.OCVValue[0] = 3400,			 /* OCV curve adjustment */

		/*if the application temperature data is preferred than the STC3117 temperature*/
		.ExternalTemperature = Temperature_fn, /*External temperature fonction, return C*/
		.ForceExternalTemperature = 0, /* 1=External temperature, 0=STC3117 temperature */
	}
};

static sec_bat_adc_table_data_t temp_table[] = {
  {26800, 650},
  {27704, 620},
  {27838, 600},
  {28126, 580},
  {28200, 550},
  {28820, 530},
  {28710, 510},
  {28869, 500},
  {28980, 490},
  {29497, 480},
  {29354, 460},
  {30126, 440},
  {30841, 400},
  {31766, 350},
  {32700, 300},
  {33827, 250},
  {34936, 200},
  {35990, 150},
  {36558, 130},
  {36800, 100},
  {37020, 80},
  {37455, 50},
  {38020, 30},
  {38563, 20},
  {38420, 10},
  {38917, 0},
  {38865, -10},
  {39515, -30},
  {39589, -50},
  {40108, -70},
  {40468, -100},
  {41039, -150},
  {41595, -200},
};

#define TEMP_HIGH_THRESHOLD_EVENT  615
#define TEMP_HIGH_RECOVERY_EVENT   480
#define TEMP_LOW_THRESHOLD_EVENT   (-50)
#define TEMP_LOW_RECOVERY_EVENT    0
#define TEMP_HIGH_THRESHOLD_NORMAL 515
#define TEMP_HIGH_RECOVERY_NORMAL  460
#define TEMP_LOW_THRESHOLD_NORMAL  (-50)
#define TEMP_LOW_RECOVERY_NORMAL   10
#define TEMP_HIGH_THRESHOLD_LPM    500
#define TEMP_HIGH_RECOVERY_LPM     495
#define TEMP_LOW_THRESHOLD_LPM     (-30)
#define TEMP_LOW_RECOVERY_LPM      20

#if defined(CONFIG_BATTERY_SWELLING)
#define BATT_SWELLING_HIGH_TEMP_BLOCK		550
#define BATT_SWELLING_HIGH_TEMP_RECOV		450
#define BATT_SWELLING_LOW_TEMP_BLOCK		100
#define BATT_SWELLING_LOW_TEMP_RECOV		150
#define BATT_SWELLING_RECHG_VOLTAGE		4150
#define BATT_SWELLING_BLOCK_TIME	10 * 60 /* 10 min */
#endif
