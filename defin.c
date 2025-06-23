    //fun definatio for black box
#include "main.h"


unsigned char clock_reg[3];
char time[7];
char log[11];
char log_pos=0;
unsigned char sec;
unsigned char return_time;
unsigned char *menu[]={"View log","Clear log","Change passwrd"};
unsigned char menu_pos; 
int event_count=0;
char pos=-1;

void clear_screen()
{
    clcd_write(CLEAR_DISP_SCREEN, INST_MODE);
    __delay_us(100);
}


static void get_time(void)
{
    clock_reg[0]=read_ds1307(HOUR_ADDR);
    clock_reg[1]=read_ds1307(MIN_ADDR);
    clock_reg[2]=read_ds1307(SEC_ADDR);
    
    time[0] = ((clock_reg[0] >> 4) & 0x03) + '0';
    time[1] = (clock_reg[0] & 0x0F) + '0';
    
    time[2] = ((clock_reg[1] >> 4) & 0x07) + '0';
    time[3] = (clock_reg[1] & 0x0F) + '0';
    
    time[4] = ((clock_reg[2] >> 4) & 0x07) + '0';
    time[5] = (clock_reg[2] & 0x0F) + '0';
    time[6] ='\0';
}

static void display_time()
{
    get_time();
    //HH:MM:SS
    clcd_putch(time[0],LINE2(2));
    clcd_putch(time[1],LINE2(3));
    clcd_putch(':',LINE2(4));
    clcd_putch(time[2],LINE2(5));
    clcd_putch(time[3],LINE2(6));
    clcd_putch(':',LINE2(7));
    clcd_putch(time[4],LINE2(8));
    clcd_putch(time[5],LINE2(9));
}



//display dashboad function
void display_dashboard(unsigned char event[],unsigned char speed)
{
    clcd_print("TIME     E  SP" , LINE1(2));
    
    //display_time
    display_time();
    
    //display event
    clcd_print(event, LINE2(11)); 
    
    //display speed
    clcd_putch((speed/10)+'0', LINE2(14));
    clcd_putch((speed/10)+'0', LINE2(15));
    
    
}
void store_event()
{
    //write log to external EEprom
    char addr;
    if(log_pos==10)
    {
        log_pos=0;
    }
    addr=0x05+log_pos*10;
    ext_eeprom_24C02_str_write(addr,log);
    log_pos++;
    
}

void log_event(unsigned char event[],unsigned char speed)
{
    //store time
    get_time();
    strncpy(log,time,6);
    
    //store event
     strncpy(&log[6],event,2);
     
     //store speed
     log[8]=(speed/10)+'0';
     log[9]=(speed%10)+'0';
     log[10]='\0';
    
     store_event();
    
}

unsigned char login(unsigned char key,unsigned char reset_flag)
{
    static char user_passwd[4];
    static unsigned char i;
    static unsigned char attempt_left;
    
    if(reset_flag==RESET_PASSWORD)
    {
        i=0;
        attempt_left=3;
        user_passwd[0]='\0';
        user_passwd[1]='\0';
        user_passwd[2]='\0';
        user_passwd[3]='\0';
        key=ALL_RELEASED;
        return_time=5;
    }
    if(return_time==0)
    {
        //change to dashboard
        return RETURN_BACK;
    }
    
    //sw4-1,sw5=0
    if(key==SW4 && i<4)
    {
        clcd_putch('*',LINE2(4+i));
        user_passwd[i]='1';
        i++;
        return_time=5;
    }
    else if(key==SW5 && i<4)
    {
       clcd_putch('*',LINE2(4+i));
       user_passwd[i]='0';
       i++; 
    }
    if(i==4)
    {
        char s_passwd[4];
        //read the stored password from external EEPROM
        for(int j=0;j<4;j++)
    {
        s_passwd[j]=ext_eeprom_24C02_read(j);
    }
    //compare stored password and user password
    if(strncmp(user_passwd,s_passwd,4)==0)
    {
        clear_screen();
        clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
        __delay_us(100);
        clcd_print("Login Success",LINE1(1));
        
        __delay_ms(3000);
        return LOGIN_SUCCESS;
        
        //change to menu screen
    }
    else
    {
        attempt_left--;
        if(attempt_left==0)
        {
            clear_screen();
            clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
            __delay_us(100);
            //display the block screen
            clcd_print("You are Blocked",LINE1(0));
            clcd_print("wait for",LINE2(0));
            clcd_print("secs",LINE2(12));
            //wait for 60 sec
            sec=60;
            while(sec!=0)
            {
                clcd_putch((sec/10)+'0',LINE2(9));
                clcd_putch((sec%10)+'0',LINE2(10));
            }
            
            __delay_ms(3000);
           
           attempt_left=3;
      
        }
        else
        {
            clear_screen();
            clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
            __delay_us(100);
            clcd_print("Wrong password",LINE1(0));
            clcd_print("attempts left",LINE2(2));
            clcd_putch(attempt_left+'0',LINE2(0));
             __delay_ms(3000);
           
                 
        }
        clear_screen();
        clcd_print("Enter Password",LINE1(1));
        clcd_write(LINE2(4),INST_MODE);
            
        clcd_write(DISP_ON_AND_CURSOR_ON, INST_MODE);
        __delay_us(100);
        i=0;
    }
  
   }  
    
}

unsigned char menu_screen(unsigned char key,unsigned char reset_flag)
{
    if(reset_flag==RESET_MENU)
    {
        return_time=5;
        menu_pos=0;
    }
    if(key==SW4&&menu_pos>0)
    {
        clear_screen();
        //decrement the menu
        menu_pos--;
    }
    else if(key==SW5&&menu_pos<4)
    {
        clear_screen();
        //increment the menu
        menu_pos++;
    }
    if(menu_pos==4)
    {
        clcd_putch('*',LINE2(0));
        clcd_print(menu[menu_pos-1] ,LINE1(2));
        clcd_print(menu[menu_pos] ,LINE2(2)); 
    }
    else
    {
      //display the menu
      clcd_putch('*',LINE1(0));
      clcd_print(menu[menu_pos ] ,LINE1(2));
      clcd_print(menu[menu_pos+1] ,LINE2(2));
    }
    return menu_pos;
}
void view_log(unsigned char key,unsigned char reset_flag)
{
    char rlog[11];
    unsigned char add;
    static unsigned char rpos;
    if(event_count ==-1)
    {
        clcd_print(" No logs",LINE2(0));
    }
    else
    {
        if (reset_flag==VIEW_LOG_RESET)
        {
            rpos=0;
        }
        if(key==SW5 && rpos< (event_count-1))
        {
            rpos++;
        }
        else if(key==SW4 && rpos > 0)
        {
            rpos--;
        }
        for(int i=0;i<10;i++)
        {
            add=rpos*10+5;
            rlog[i]=eeprom_read(add + i);
        }

        clcd_putch(rpos%10 + '0',LINE2(0));

        clcd_putch(rlog[0],LINE2(2));
        clcd_putch(rlog[1],LINE2(3));
        clcd_putch(':',LINE2(4));

        clcd_putch(rlog[2],LINE2(5));
        clcd_putch(rlog[3],LINE2(6));
        clcd_putch(':',LINE2(7));

        clcd_putch(rlog[4],LINE2(8));
        clcd_putch(rlog[5],LINE2(9));
        
        clcd_putch(rlog[6],LINE2(11));
        clcd_putch(rlog[7],LINE2(12));

        clcd_putch(rlog[8],LINE2(14));
        clcd_putch(rlog[9],LINE2(15));   
    }
}   

char clear_log(unsigned char reset_flag)
{


    if(reset_flag==RESET_MEMEORY )
    {
        event_count=-1;
        pos=-1;
        clcd_print(" Logs Cleared ",LINE1(0));
        clcd_print(" Successfully ",LINE2(0));
        __delay_ms(3000);
        return TASK_SUCCESS;
    }
    return TASK_FAIL;
}

char change_password(unsigned char key,unsigned char reset_flag)
{
    static char pwd[9];
    static int pos,once;
    if(reset_flag==RESET_CHANGE_PASSWORD)
    {
        pos=0;
        once=1;
    }
    if(pos<4 && once)
    {
        once=0;
        clcd_print("Enter New Pwd",LINE1(0));
        clcd_write(LINE2(0), INST_MODE);
        clcd_write(DISP_ON_AND_CURSOR_ON,INST_MODE);
    }
    else if(pos>=4 && (once==0))
    {
        once=1;
        clear_screen();
        clcd_print("Re-enter New Pwd",LINE1(0));
        clcd_write(LINE2(0), INST_MODE);
        clcd_write(DISP_ON_AND_CURSOR_ON,INST_MODE);
    }
    if(key==SW4)// '4'
    {
        pwd[pos]='4';
        clcd_putch('*',LINE2(pos%4));
        pos++;
    }
    else if(key==SW5)// '2'
    {
        pwd[pos]='2';
        clcd_putch('*',LINE2(pos%4));
        pos++;
    }
    if(pos==8)
    {
        if(strncmp(pwd,&pwd[4],4)==0)
        {
            
            for(int i=0;i<4;i++)
            {
                eeprom_write(i,pwd[i]);
            }
            clear_screen();
            clcd_print(" Password Change",LINE1(0));
            clcd_print(" Successfully ",LINE2(0));
            __delay_ms(3000);
            return TASK_SUCCESS;
        }
        else
        {
            clear_screen();
            clcd_print(" Password Change",LINE1(0));
            clcd_print(" Failed ",LINE2(0));
            __delay_ms(3000);
            return TASK_SUCCESS;
        }
        
    }
    return 0x10;
}