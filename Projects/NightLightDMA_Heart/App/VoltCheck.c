#include "VoltCheck.h"

static uint32_t _task_cnt = 0;
static float _volt = 0.0f;
static uint8_t LowPowerCnt = 0;
static uint8_t LowPowerFlag = 0;

void VoltCheckTask(void)
{
	if(_task_cnt % 20 == 0) {
		_volt = ReadBatVoltage() * 0.1f + _volt * 0.9f;
		if(_volt > 3.9f) {
			LowPowerCnt = 0;
			LowPowerFlag = 0;
		} else if(_volt < 3.4f) {
			if(LowPowerCnt < 50)
				LowPowerCnt ++;
			else {
				LowPowerFlag = 1;
			}
		} else {
			LowPowerCnt = 0;
		}
	}

	_task_cnt ++;
}

inline uint8_t getLowPowerFlag(void) { return LowPowerFlag; }
