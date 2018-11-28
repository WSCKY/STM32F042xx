#include "DataMonitor.h"

static uint8_t _init_flag = 0;
static COM_DATA_PACK_DEF ComDataBuf = {0};

static void InitDataPackage(COMM_DATA *pComData)
{
	pComData->head = 0x55;
	pComData->cmd = 0xDD;
	pComData->length = 36;
	pComData->checksum = 0;
}

static void DataPackageChkSum(COMM_DATA *pComData)
{
	uint8_t chk = 0, i = 0;
	for(; i < 36; i ++)
		chk += pComData->data[i / 4].c_data[i % 4];
	pComData->checksum = chk;
}

void SendDataToMonitor(void)
{
	if(_init_flag == 0) {
		_init_flag = 1;

		InitDataPackage(&ComDataBuf.ComData);
		return;
	}

	DataPackageChkSum(&ComDataBuf.ComData);

  if(MONITOR_IF_CHECK())
    MONITOR_IF_TX_BYTES(ComDataBuf.RawData, 40);
}

void MonitorUpdateDataPos(float f, uint8_t p)
{
  if(p < MONITOR_DATA_LENGTH) {
    ComDataBuf.ComData.data[p].f_data = f;
  }
}
