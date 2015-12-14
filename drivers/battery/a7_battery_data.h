#define CAPACITY_MAX			1000
#define CAPACITY_MAX_MARGIN     30
#define CAPACITY_MIN			0

#if defined(CONFIG_MACH_A7_CHN_CTC)
static sec_bat_adc_table_data_t temp_table[] = {
	{25990, 900},
	{26420, 800},
	{27100, 700},
	{27510, 650},
	{28070, 600},
	{28660, 550},
	{29280, 500},
	{30050, 450},
	{30910, 400},
	{32820, 300},
	{34930, 200},
	{37080, 100},
	{38030, 50},
	{38960, 0},
	{39770, -50},
	{40175, -100},
	{41335, -200},
};

#define TEMP_HIGH_THRESHOLD_EVENT  600
#define TEMP_HIGH_RECOVERY_EVENT   460
#define TEMP_LOW_THRESHOLD_EVENT   (-50)
#define TEMP_LOW_RECOVERY_EVENT    0
#define TEMP_HIGH_THRESHOLD_NORMAL 600
#define TEMP_HIGH_RECOVERY_NORMAL  460
#define TEMP_LOW_THRESHOLD_NORMAL  (-50)
#define TEMP_LOW_RECOVERY_NORMAL   0
#define TEMP_HIGH_THRESHOLD_LPM    600
#define TEMP_HIGH_RECOVERY_LPM     460
#define TEMP_LOW_THRESHOLD_LPM     (-50)
#define TEMP_LOW_RECOVERY_LPM      0

#else
static sec_bat_adc_table_data_t temp_table[] = {
	{27023, 700},
	{27926, 600},
	{29146, 500},
	{29889, 450},
	{30728, 400},
	{32642, 300},
	{34816, 200},
	{36982, 100},
	{38913, 0},
	{39725, -50},
	{40394, -100},
	{41462, -200},
};

#if defined(CONFIG_MACH_A7_CHN_TW)
#define TEMP_HIGH_THRESHOLD_EVENT  600
#define TEMP_HIGH_RECOVERY_EVENT   460
#define TEMP_LOW_THRESHOLD_EVENT   (-50)
#define TEMP_LOW_RECOVERY_EVENT    0
#define TEMP_HIGH_THRESHOLD_NORMAL 600
#define TEMP_HIGH_RECOVERY_NORMAL  460
#define TEMP_LOW_THRESHOLD_NORMAL  (-50)
#define TEMP_LOW_RECOVERY_NORMAL   0
#define TEMP_HIGH_THRESHOLD_LPM    600
#define TEMP_HIGH_RECOVERY_LPM     500
#define TEMP_LOW_THRESHOLD_LPM     (-50)
#define TEMP_LOW_RECOVERY_LPM      20
#else
#define TEMP_HIGH_THRESHOLD_EVENT  600
#define TEMP_HIGH_RECOVERY_EVENT   480
#define TEMP_LOW_THRESHOLD_EVENT   (-50)
#define TEMP_LOW_RECOVERY_EVENT    0
#define TEMP_HIGH_THRESHOLD_NORMAL 600
#define TEMP_HIGH_RECOVERY_NORMAL  480
#define TEMP_LOW_THRESHOLD_NORMAL  (-50)
#define TEMP_LOW_RECOVERY_NORMAL   0
#define TEMP_HIGH_THRESHOLD_LPM    600
#define TEMP_HIGH_RECOVERY_LPM     500
#define TEMP_LOW_THRESHOLD_LPM     (-50)
#define TEMP_LOW_RECOVERY_LPM      20
#endif

#endif

#if defined(CONFIG_BATTERY_SWELLING)
#define BATT_SWELLING_HIGH_TEMP_BLOCK		500
#define BATT_SWELLING_HIGH_TEMP_RECOV		450
#define BATT_SWELLING_LOW_TEMP_BLOCK		100
#define BATT_SWELLING_LOW_TEMP_RECOV		150
#define BATT_SWELLING_RECHG_VOLTAGE		4150
#define BATT_SWELLING_BLOCK_TIME		10
#endif