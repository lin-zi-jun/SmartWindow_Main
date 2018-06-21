#ifndef __CTRIO_DRIVER_H__
#define __CTRIO_DRIVER_H__
///////////////////////////////////////////////////////////////////////////////
#include "stm32f10x.h"
///////////////////////////////////////////////////////////////////////////////
void CtrlIO_Init(void);
void BuzzerIO_Init(void);
void RelayIO_Init(void);
void SWIO_Init(void);
void EthIO_Init(void);

void BUZZER_ON(void);
void BUZZER_OFF(void);
void BUZZER_TOGGLE(void);

void RELAY_ON(void);
void RELAY_OFF(void);

u8 TCP_Connected(void);
void EthNetReset(void);
void EthNetRun(void);
void DOOR_Init(void);
void DOOR_out_right(void);
void DOOR_out_left(void);
void DOOR_in_right(void);
void DOOR_in_left(void);
void doorcheckstate_init(void);
///////////////////////////////////////////////////////////////////////////////
#endif


