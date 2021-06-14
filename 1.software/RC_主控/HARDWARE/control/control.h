#ifndef __CONTROL_H
#define __CONTROL_H
#include "sys.h"

extern int coordinate1_X, coordinate1_Y;//(200,16000,0)
extern int coordinate2_X, coordinate2_Y;//(8200,16000,0)
extern int coordinate3_X, coordinate3_Y;//(16200,16000,0)
extern int coordinate4_X, coordinate4_Y;//(200,8500,0)
extern int coordinate5_X, coordinate5_Y;//(8200,8500,0)
extern int coordinate6_X, coordinate6_Y;//(16200,8500,0)
extern int coordinate7_X, coordinate7_Y;//(200,1000,0)
extern int coordinate8_X, coordinate8_Y;//(8200,1000,0)
extern int coordinate9_X, coordinate9_Y;//(16200,1000,0)
extern int coordinate10_Z;//(0,0,47000)

void User_Modification(u8 dat);
void remote_control(void);
void speech_control(void);
void proxy_software(void);
void preceding_page(void);
void After_the_pagee(void);

#endif
