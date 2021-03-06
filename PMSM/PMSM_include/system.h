// step 1. Set the system parameters
#define SYS_CLK					(float)150000000
//#define Fsw						(float)20000
#define Deadtime				(float)0.000001		// 0.000003

 	typedef struct {
		float scale[16];
		float offset[16];
		float tmp[16];
		int conv_val[16];
		float tmp1[16];
	}	ADC;


#ifdef _SYSTEM_

	ADC	adc;

	int cal_offset_cnt = 0;

	void delay(int delay_cnt);
	void delay_long(long int delay_cnt);  
	void Init_dsc();
	void rst_chk();
	void ad_offset_calculation();
	void DAC_Setup();
#else

	extern ADC adc;

	extern int cal_offset_cnt;

	extern void delay(int delay_cnt);
	extern void delay_long(long int delay_cnt);
	extern void Init_dsc();
	extern void rst_chk();
	extern void ad_offset_calculation();
#endif

extern void relay_control(int relay_flag);



