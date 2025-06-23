/* 
 * File:   main.c
 * Author: admin
 *
 * Created on June 21, 2025, 12:45 PM
 */

#include<xc.h>
#include"main.h"
#pragma config WDTE=OFF



void init_config()
{
    init_i2c(1000000);
    init_ds1307();
    init_clcd();
    init_adc();
    init_digital_keypad();
    init_timer2();
    
    PEIE=1;
    GIE=1;
}
void main(void)
{
    init_config();
    unsigned char control_flag = DASHBOARD_SCREEN,key;
    unsigned char event[3]="ON";
    unsigned char reset_flag,menu_pos;
    unsigned char *gear[]={"GN","GR","G1","G2","G3","G4"};
    unsigned char gr=0;
    unsigned char speed=0;
    log_event(event,speed);
    ext_eeprom_24C02_str_write(0x00, "1010");
    
    while(1)
    {
        speed=read_adc()/10.3;
        key=read_digital_keypad(STATE);
        
        if(key==SW1)
        {
            //collision
            strcpy(event,"CO");
            log_event(event,speed);
        }
        else  if(key==SW2&&gr<6)
        {
            //increment the gear
             strcpy(event,gear[gr]);
             gr++;
             log_event(event,speed);
        }
        else if(key==SW3&&gr>0)
        {
            //decrement the gear
            gr--;
            strcpy(event,gear[gr]);
            log_event(event,speed);
        }
        else if((key==SW4||key==SW5)&&control_flag == DASHBOARD_SCREEN)
        {
            control_flag=LOGIN_SCREEN;
            
            clcd_write(CLEAR_DISP_SCREEN, INST_MODE);
            __delay_us(500);
            
            clcd_print("Enter Password",LINE1(1));
            clcd_write(0xC4,INST_MODE);
            
            clcd_write(DISP_ON_AND_CURSOR_ON, INST_MODE);
             __delay_us(100);
             
             reset_flag=RESET_PASSWORD;
             TMR2ON=1;
        }
        else if(control_flag == MAIN_MENU_SCREEN && key == LONG_PRESS_SW4)

           {
            switch(menu_pos)
            {
                case 0:     //view log
                    clear_screen();
                    clcd_print("# TIME     E  SP",LINE1(0));
                    control_flag=VIEW_LOG_SCREEN;
                    break;
                case 1:     //clear log
                    log_event("CL",speed);
                    clear_screen();
                    control_flag=CLEAR_LOG_SCREEN;
                    
                    break;
                case 2:     //change password
                    log_event("CP",speed);
                    clear_screen();
                    control_flag=CHANGE_PASSWORD_SCREEN;
                    break;
            }
        }
      
        switch(control_flag)
        {
            case DASHBOARD_SCREEN:
                display_dashboard(event,speed);
                break;
            case LOGIN_SCREEN :
                switch (login(key,reset_flag))
                {
                    case RETURN_BACK:
                        control_flag = DASHBOARD_SCREEN;
                        clear_screen();
                        clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
                        __delay_us(100);
                        TMR2ON=0;
                    break;
                    case LOGIN_SUCCESS:
                        control_flag = MAIN_MENU_SCREEN;
                        clear_screen();
                        clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
                        __delay_us(100);
                        reset_flag=RESET_MENU;
                        continue;
                    break;
                }
                break;
            case MAIN_MENU_SCREEN:
                    menu_pos=menu_screen(key,reset_flag);
                        break;  
                case VIEW_LOG_SCREEN:
                    view_log(key,reset_flag);
                        break;      
                case CLEAR_LOG_SCREEN:
                    if(clear_log(reset_flag)==TASK_SUCCESS)
                    {
                        clear_screen();
                        control_flag=MAIN_MENU_SCREEN;
                        reset_flag=RESET_LOGIN_MENU;
                        continue;
                    }
                        break;
                case CHANGE_PASSWORD_SCREEN:
                    switch(change_password(key,reset_flag))
                    {
                        case TASK_SUCCESS:
                            clear_screen();
                            clcd_write(DISP_ON_AND_CURSOR_OFF,INST_MODE);
                            __delay_us(100);
                            control_flag=MAIN_MENU_SCREEN;
                            reset_flag=RESET_LOGIN_MENU;
                            continue;
                            break;
                    }

                    break;
        }
        reset_flag=RESET_NOTHING;
    }
    return;
}