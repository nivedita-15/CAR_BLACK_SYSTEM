/* 
 * File:   fun.h
 * Author: admin
 *
 * Created on June 21, 2025, 3:15 PM
 */

#ifndef FUN_H
#define	FUN_H

#define _XTAL_FREQ                  20000000

void display_dashboard(unsigned char event[],unsigned char speed);
void log_event(unsigned char event[],unsigned char speed);
unsigned char login(unsigned char key,unsigned char reset_flag);
unsigned char menu_screen(unsigned char key,unsigned char reset_flag);
void clear_screen();

void view_log(unsigned char key,unsigned char reset_flag);
char clear_log(unsigned char reset_flag);
char change_password(unsigned char key,unsigned char reset_flag);


#endif