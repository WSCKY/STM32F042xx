#include "KeyUI.h"

//#define DEG_TO_RAD 0.017453292519943295769236907684886f
//#define RAD_PI     3.141592653589793f

static uint8_t _init_flag = 0;

static uint8_t KeyState = 0;
static uint8_t LastKeyState = 0;
static uint32_t Key_TimeStamp = 0;
static uint32_t Key_CheckCnt = 0;
static uint8_t Key_ReleaseFlag = 0;

static uint8_t TPState = 0;
static uint8_t LastTPState = 0;
static uint32_t TP_TimeStamp = 0;
static uint32_t TP_CheckCnt = 0;
static uint8_t TP_ReleaseFlag = 0;

static int8_t breath_dir = 1;
static uint8_t curLuminance = 0;

static uint8_t PowerOffFlag = 0;

static void PowerOffCheckLoop(void);

void KeyCheckTask(void)
{
	if(_init_flag == 0) {
		_init_flag = 1;

		curLuminance = _LUMINANCE_LEVEL_1;//getLightLuminance();
		Key_TimeStamp = _Get_Millis();
		TP_TimeStamp = _Get_Millis();
	}

	TPState = GetTouchState(); if(TP_ReleaseFlag == 0) { if(TPState == 0) TP_ReleaseFlag = 1; }
	KeyState = ReadButtonStatus(PWR_KEY); if(Key_ReleaseFlag == 0) { if(KeyState == 0) Key_ReleaseFlag = 1; }

	if(TP_ReleaseFlag == 0 || Key_ReleaseFlag == 0) return;

	if(getLowPowerFlag()) {
		if(PowerOffFlag == 1)
			curLuminance = 0;
		else
			curLuminance = 12;

		setLightLuminance(curLuminance);
	} else {
		if(TPState == 1) {
			if(LastTPState == 0) {
				TP_CheckCnt = 0;
				TP_TimeStamp = _Get_Millis();
			} else {
				if(_Get_Millis() - TP_TimeStamp > 1000) {
					TP_CheckCnt ++;
					if(TP_CheckCnt % 15 == 0) {
						if(curLuminance == 255) breath_dir = -1;
						else if(curLuminance <= 20) breath_dir = 1;
						curLuminance += breath_dir;
						setLightLuminance(curLuminance);
					}
				}
			}
		} else {
			if(LastTPState == 1) {
				if(_Get_Millis() - TP_TimeStamp <= 1000) {
							 if(curLuminance < _LUMINANCE_LEVEL_1) { curLuminance = _LUMINANCE_LEVEL_1; }
					else if(curLuminance < _LUMINANCE_LEVEL_2) { curLuminance = _LUMINANCE_LEVEL_2; }
					else if(curLuminance < _LUMINANCE_LEVEL_3) { curLuminance = _LUMINANCE_LEVEL_3; }
					else { curLuminance = _LUMINANCE_LEVEL_0; }

					setLightLuminance(curLuminance);
				}
			}
		}
	}

	if(KeyState == 1) {
		if(LastKeyState == 0) {
			Key_CheckCnt = 0;
			Key_TimeStamp = _Get_Millis();
		} else {
			if(_Get_Millis() - Key_TimeStamp > 1000) {
				Key_CheckCnt ++;
				if(Key_CheckCnt % 10 == 0) {
					setLightColor(ColourSelectLoop(curLuminance));
				}
			}
		}
	} else {
		if(LastKeyState == 1) {
			if(_Get_Millis() - Key_TimeStamp <= 1000) {
				switchDotMode();
			}
		}
	}

	LastTPState = TPState;
	LastKeyState = KeyState;

	PowerOffCheckLoop();
}

static uint32_t PowerOffTimeLimit = 60000;
static void PowerOffCheckLoop(void)
{
	static uint32_t _poweroff_cnt = 0;
	if(getLowPowerFlag() == 1 || curLuminance == _LUMINANCE_LEVEL_0) {
		if(getLowPowerFlag() == 1) PowerOffTimeLimit = 30000;
		_poweroff_cnt ++;
		if(_poweroff_cnt >= PowerOffTimeLimit) { /* 1 min or 30s if low power. */
			PowerOffFlag = 1;
			if(_poweroff_cnt >= (PowerOffTimeLimit + 1000)) {
				HW_PWR_OFF();
				/* Enter Stop Mode */
				PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);
				for(;;);
			}
		}
	} else {
		_poweroff_cnt = 0;
	}
}
