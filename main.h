/*
 * File:   main.h
 * Author: admin
 *
 * Created on June 21, 2025, 12:53 PM
 */


#ifndef MAIN_H
#define MAIN_H
#define LONG_PRESS_SW4    0x08  // Example value; use correct one per your keypad design

#include<xc.h>
#include"adc.h"
#include"clcd.h"
#include"i2c.h"
#include"ds1307.h"
#include"digital_keypad.h"
#include"fun.h"
#include"string.h"
#include"EEprom.h"
#include"timer.h"



#define DASHBOARD_SCREEN 0x01
#define LOGIN_SCREEN     0x02
#define MAIN_MENU_SCREEN 0x03
#define RESET_PASSWORD   0x11
#define RESET_NOTHING    0x22
#define RETURN_BACK      0x33
#define LOGIN_SUCCESS    0X44
#define RESET_MENU       0x55

#define VIEW_LOG_SCREEN           0x08
#define CLEAR_LOG_SCREEN          0x0A
#define CHANGE_PASSWORD_SCREEN    0x0B
#define RESET_CHANGE_PASSWORD      0x19
#define RESET_LOGIN_MENU        0x13
#define VIEW_LOG_RESET          0x15
#define RESET_MEMEORY           0x17



#define TASK_SUCCESS            0x11
#define TASK_FAIL               0x22 
        
#endif
