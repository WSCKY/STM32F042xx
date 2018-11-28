#ifndef __COMMPROTOCOL_H
#define __COMMPROTOCOL_H

#include "ComIf_conf.h"
#include "ComVersion.h"

#define Header1              (0x55)
#define Header2              (0xAA)

#define FILE_DATA_CACHE      (32) /* 16 * n */
#define PACKET_CACHE         (FILE_DATA_CACHE + 10)
#define PACKET_MAX_LENGTH    (PACKET_CACHE - 3)

__packed typedef enum {
	DECODE_STATE_UNSYNCED = 0,
	DECODE_STATE_GOT_STX1 = 1,
	DECODE_STATE_GOT_STX2 = 2,
	DECODE_STATE_GOT_LEN  = 3,
	DECODE_STATE_GOT_TYPE = 4,
	DECODE_STATE_GOT_DATA = 5
} DECODE_STATE;

__packed typedef enum {
	/* Communication Heartbeat */
	TYPE_COM_HEARTBEAT = 0x01,
	/* Protocol Version */
	TYPE_VERSION_REQUEST = 0x02,
	TYPE_VERSION_Response = 0x03,
#if defined(COM_USER_TYPE)
  COM_USER_TYPE
#endif
} PACKET_TYPE;

__packed typedef struct {
	uint8_t _Cnt;
} HeartBeatDef;

__packed typedef struct {
	uint16_t v;
} VersionResponseDef;

__packed typedef union {
	uint8_t pData[PACKET_CACHE - 5];
	HeartBeatDef Heartbeat;
//	DevStateResponseDef DevSta;
	VersionResponseDef Version;
#if defined(COM_USER_TYPE_DATA)
  COM_USER_TYPE_DATA
#endif
} PacketDataUnion;

__packed typedef struct {
	uint8_t stx1;
	uint8_t stx2;
	uint8_t len;
	uint8_t type;
	PacketDataUnion PacketData;
	uint8_t crc;
} PackageStructDef;

__packed typedef union {
	PackageStructDef Packet;
	uint8_t RawData[PACKET_CACHE];
} CommPackageDef;

#define FC_ACK_PACKET_LEN    (sizeof(FCResponseDef) + 2)

uint8_t GotNewData(void);
CommPackageDef* GetRxPacket(void);
void SendTxPacket(CommPackageDef* pPacket);
void ComIfRecDataCallBack(uint8_t Data);

#ifndef COM_IF_TX_CHECK
  #define COM_IF_TX_CHECK()    (1)
#endif /* COM_IF_TX_CHECK */

#ifndef COM_IF_TX_BYTES
  #define COM_IF_TX_BYTES(...) ((void)0)
#endif /* COM_IF_TX_BYTES */

#endif /* __COMMPROTOCOL */
