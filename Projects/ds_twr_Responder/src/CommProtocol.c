#include "CommProtocol.h"
#include "Maths.h"

uint8_t GotDataFlag = 0;
CommPackageDef rxPacket = {0};

uint8_t _rx_length = 0;
CommPackageDef _rx_packet = {0};
DECODE_STATE _decode_state = DECODE_STATE_UNSYNCED;

static void rx_decode(uint8_t data);

void ComIfRecDataCallBack(uint8_t Data)
{
	rx_decode(Data);
}

uint8_t GotNewData(void)
{
	if(GotDataFlag == 1) {
		GotDataFlag = 0;
		return 1;
	}
	return 0;
}

CommPackageDef* GetRxPacket(void)
{
	return &rxPacket;
}

void SendTxPacket(CommPackageDef* pPacket)
{
	pPacket->Packet.crc = Get_CRC8_Check_Sum(&(pPacket->RawData[2]), pPacket->Packet.len, CRC_INIT);
	pPacket->Packet.PacketData.pData[pPacket->Packet.len - 2] = pPacket->Packet.crc;
  if(COM_IF_TX_CHECK())
    COM_IF_TX_BYTES(pPacket->RawData, pPacket->Packet.len + 3);
}

static void rx_decode(uint8_t data)
{
	switch(_decode_state) {
		case DECODE_STATE_UNSYNCED:
			if(data == Header1) {
				_rx_packet.Packet.stx1 = Header1;
				_decode_state = DECODE_STATE_GOT_STX1;
			}
		break;
		case DECODE_STATE_GOT_STX1:
			if(data == Header2) {
				_rx_packet.Packet.stx2 = Header2;
				_decode_state = DECODE_STATE_GOT_STX2;
			} else
				_decode_state = DECODE_STATE_UNSYNCED;
		break;
		case DECODE_STATE_GOT_STX2:
			if(data <= PACKET_MAX_LENGTH) {
				_rx_packet.Packet.len = data;
				_rx_length = 0;
				_decode_state = DECODE_STATE_GOT_LEN;
			} else {
				_decode_state = DECODE_STATE_UNSYNCED;
			}
		break;
		case DECODE_STATE_GOT_LEN:
			_rx_packet.Packet.type = data;
			_rx_length ++;
			_decode_state = DECODE_STATE_GOT_TYPE;
		break;
		case DECODE_STATE_GOT_TYPE:
			_rx_packet.Packet.PacketData.pData[_rx_length - 1] = data;
			_rx_length ++;
			if(_rx_length == _rx_packet.Packet.len - 1)
				_decode_state = DECODE_STATE_GOT_DATA;
		break;
		case DECODE_STATE_GOT_DATA:
			_rx_packet.Packet.crc = data;
			_rx_length ++;
			if(Get_CRC8_Check_Sum(&(_rx_packet.RawData[2]), _rx_length, CRC_INIT) == data) {
				rxPacket = _rx_packet;
				GotDataFlag = 1;
			} else {}
			_decode_state = DECODE_STATE_UNSYNCED;
		break;
		default:
			_decode_state = DECODE_STATE_UNSYNCED;
		break;
	}
}
