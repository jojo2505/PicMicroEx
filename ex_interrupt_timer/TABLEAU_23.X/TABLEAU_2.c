
#include <xc.h>
#include "uart.h"
#include <stdlib.h>
#include <stdio.h>


#define bitset(var, bitno) ((var) |= 1UL << (bitno))
#define bitclr(var, bitno) ((var) &= ~(1UL << (bitno)))

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

// CONFIG1L
#pragma config PLLDIV = 5       // PLL Prescaler Selection bits (Divide by 5 (20 MHz oscillator input))
#pragma config CPUDIV = OSC1_PLL2// System Clock Postscaler Selection bits ([Primary Oscillator Src: /1][96 MHz PLL Src: /2])
#pragma config USBDIV = 1       // USB Clock Selection bit (used in Full-Speed USB mode only; UCFG:FSEN = 1) (USB clock source comes directly from the primary oscillator block with no postscale)

// CONFIG1H
#pragma config FOSC = HSPLL_HS  // Oscillator Selection bits (HS oscillator, PLL enabled (HSPLL))
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOR = ON         // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 3         // Brown-out Reset Voltage bits (Minimum setting)
#pragma config VREGEN = OFF     // USB Voltage Regulator Enable bit (USB voltage regulator disabled)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = ON      // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer 1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
#pragma config ICPRT = OFF      // Dedicated In-Circuit Debug/Programming Port (ICPORT) Enable bit (ICPORT disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-001FFFh) is not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (002000-003FFFh) is not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (004000-005FFFh) is not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (006000-007FFFh) is not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) is not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM is not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-001FFFh) is not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (002000-003FFFh) is not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (004000-005FFFh) is not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (006000-007FFFh) is not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) are not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot block (000000-0007FFh) is not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM is not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-001FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (002000-003FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (004000-005FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (006000-007FFFh) is not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot block (000000-0007FFh) is not protected from table reads executed in other blocks)






//
//#define _XTAL_FREQ 48000000
//
#define IN1 PORTBbits.RB4 // gauche 
#define TRIS_IN1 TRISBbits.RB4
#define IN2 PORTBbits.RB5  // gauche
#define TRIS_IN2 TRISBbits.RB5
#define IN3 PORTBbits.RB6  //droite
#define TRIS_IN3 TRISBbits.RB6
#define IN4 PORTBbits.RB7 //droite
#define TRIS_IN4 TRISBbits.RB7
//
#define servo1 PORTDbits.RD1
#define servo2 PORTDbits.RD2
#define tris_servo1 TRISDbits.RD1
#define tris_servo2 TRISDbits.RD2
#define trig PORTDbits.RD0
#define Tris_trig TRISDbits.RD0
//
#define cte 0.0057//3/(58)      
//
//#define MODE_TEST
#define MODE_UART
//
#define LINE 1
#define LEFT 2
#define RIGHT 3
#define B_RIGHT 4
#define B_LEFT 5
#define BACK 6
#define LINE2 7
#define STOP 8
//
#define PWM_LINEc_LEFTm 700
#define PWM_LINEc_RIGHTm 700
//
//
#define PWM_LINE2c_LEFTm 900
#define PWM_LINE2c_RIGHTm 900
//
#define PWM_RIGHTc_LEFTm 800
#define PWM_RIGHTc_RIGHTm 512
//
#define PWM_LEFTc_LEFTm 512
#define PWM_LEFTc_RIGHTm 800
//
#define PWM_BLEFTc_LEFTm 512
#define PWM_BLEFTc_RIGHTm 512
//
#define PWM_BRIGHTc_LEFTm 700
#define PWM_BRIGHTc_RIGHTm 700
//
#define PWM_BACKc_LEFTm  700
#define PWM_BACKc_RIGHTm 700
//
#define YES 1
#define NO 0



void InitPWMG(void);
void SetPWMDutyG(unsigned int DutyCycle);
void SetPWMDutyD(unsigned int DutyCycle);
void InitPWMD(void);
void in_config(unsigned char gauche, unsigned char droite);
void sendPulse(void);
//
unsigned int tab_command[50];
unsigned int tab_para_g[50];
unsigned int tab_para_d[50];
//
unsigned int i, N;
unsigned int countg;
unsigned int countd;
bit positionok1;
bit positionok2;
unsigned int count_s0 = 0;
unsigned int count_s1 = 0;
unsigned int count_s2 = 0;
unsigned int counts0_limit = 470;
unsigned int counts1_limit = 23;
unsigned int counts2_limit = 23;
unsigned char clk_second = 0;

unsigned int clk_count = 0;
//ultrason

float distance = 100;
//
float inter;
char *buf;
unsigned char new_value = NO;
unsigned int pulse_controle = 0;
int status;
//
//bit straight;
//bit left;
//bit right;
#define YES 1
#define NO 0 
#define DIRECT 1
#define INDIRECT 0
// Main function

void
main(void)
{

    Tris_trig = 0;
    tris_servo1 = 0;
    tris_servo2 = 0;
    //config interrupt
    //IPEN = 0// h and L priority
    TMR1IP = 1; //H for clock
    //TMR2 = 1;// low for ultra
    //INT1IP = 1// H encodeur 2 le premier tjr h

    INTCONbits.GIE = 1;
    PEIE = 1;
    INT0IE = 1;
    INTEDG0 = 1;
    INT1IE = 1;
    INTEDG1 = 1;
    TMR1IE = 1;
    //ultrason
    //config tmr0 ultra son
    T08BIT = 0; // 16bit
    T0CS = 0; //0 = Internal instruction cycle clock (CLKO)
    PSA = 0; //0 = Timer0 prescaler is assigned;
    T0PS0 = 1;
    T0PS1 = 0;
    T0PS2 = 0;


    //
    countg = 0;
    countd = 0;
    positionok1 = NO;
    positionok2 = NO;
    // straight = NO;
    //
    tab_command[0] = LINE;
    tab_para_g[0] = 30;
    tab_para_d[0] = 30;
    tab_command[1] = LINE2;
    tab_para_g[1] = 125;
    tab_para_d[1] = 125;
    //
    tab_command[2] = STOP;
    
    
    //
    tab_command[3] = BACK;
    tab_para_g[3] = 120;
    tab_para_d[3] = 120;
    //
    tab_command[4] = B_RIGHT;
    tab_para_g[4] = 35;
    tab_para_d[4] = 35;
    //
    tab_command[5] = BACK;
    tab_para_g[5] = 120;
    tab_para_d[5] = 120;
    N = 6;
    //
    //
    InitPWMG();
    InitPWMD();
    //TRISB = 0x00;
    TRIS_IN1 = 0;
    TRIS_IN2 = 0;
    TRIS_IN3 = 0;
    TRIS_IN4 = 0;
    tris_servo1 = 0;
    tris_servo2 = 0;


    PORTB = 0x00;

    IN1 = 1;

    IN2 = 0;

    IN3 = 1;

    IN4 = 0;

    //SetPWMDuty2(1000);
    i = 0;
#ifdef MODE_TEST   
    TRISCbits.RC0 = 0;
#endif
    //config timer temps 90s 
    T1CONbits.RD16 = 1; // 16bits
    T1CONbits.T1CKPS0 = 1;
    T1CONbits.T1CKPS1 = 0; // 2
    T1CONbits.T1OSCEN = 0; //= Timer1 oscillator is shut off
    T1CONbits.TMR1CS = 0; //fosc/4
    T1CONbits.TMR1ON = 1;
    //
    WRITETIMER1(0xFF00);

#ifdef MODE_UART
    UART_Init(9600);
#endif

    while (1)
    {

        while (i < N)

        {
            PORTCbits.RC0 = 1 - PORTCbits.RC0;
            //ultra son
            pulse_controle++;
HERE1:
            if (pulse_controle == 1000)
            {
                sendPulse();
                pulse_controle = 0;
            }
            if (new_value == YES)
            {
                distance = inter*cte;
                new_value = NO;
#ifdef MODE_UART

                buf = ftoa(distance, &status);
                UART_Write_Text(buf);
                UART_Write(' ');
#endif

            }
            if (distance < 10)
            {

                pulse_controle = 1000;
                SetPWMDutyD(0);
                SetPWMDutyG(0);
                counts1_limit = 46;

                goto HERE1;
            }
            else
            {
             counts1_limit = 23;
            }
             
            //
            if (tab_command[i] == LINE)
            {
                //straight = YES;

                if (countg >= tab_para_g[i])

                {
                    SetPWMDutyG(0);
                    positionok1 = YES;


                }
                else
                {
                    SetPWMDutyG(PWM_LINEc_LEFTm);
                    in_config(DIRECT, DIRECT);


                }
                if (countd >= tab_para_d[i])
                {

                    SetPWMDutyD(0);
                    positionok2 = YES;

                }
                else
                {
                    SetPWMDutyD(PWM_LINEc_RIGHTm);
                    in_config(DIRECT, DIRECT);

                }

                if ((positionok1 == YES)&&(positionok2 == YES))
                {
                    positionok1 = NO;
                    positionok2 = NO;

                    countg = 0;
                    countd = 0;
                    i++;
#ifdef MODE_TEST
                    __delay_us(2000);
#endif
                }

            }
            else if (tab_command[i] == STOP)
            {
                SetPWMDutyG(0);
                SetPWMDutyD(0);
                IN1 = 1;
                IN2= 1;
                IN3 = 1;
                IN4= 1;
                for(unsigned int jj=0;jj<50;jj++)__delay_ms(10);
                countg = 0;
                countd = 0;
                i++;
                
                
                
            }
            else if (tab_command[i] == LINE2)
            {
                //straight = YES;

                if (countg >= tab_para_g[i])

                {
                    SetPWMDutyG(0);
                    positionok1 = YES;


                }
                else
                {
                    SetPWMDutyG(PWM_LINE2c_LEFTm);
                    in_config(DIRECT, DIRECT);


                }
                if (countd >= tab_para_d[i])
                {

                    SetPWMDutyD(0);
                    positionok2 = YES;

                }
                else
                {
                    SetPWMDutyD(PWM_LINE2c_RIGHTm);
                    in_config(DIRECT, DIRECT);

                }

                if ((positionok1 == YES)&&(positionok2 == YES))
                {
                    positionok1 = NO;
                    positionok2 = NO;

                    countg = 0;
                    countd = 0;
                    i++;
#ifdef MODE_TEST
                    __delay_us(2000);
#endif
                }

            }

            else if (tab_command[i] == RIGHT)
            {




                if (countg >= tab_para_g[i])

                {
                    SetPWMDutyG(0);
                    positionok1 = YES;


                }
                else
                {
                    SetPWMDutyG(PWM_RIGHTc_LEFTm);
                    in_config(DIRECT, DIRECT);
                }
                if (countd >= tab_para_d[i])
                {

                    SetPWMDutyD(0);
                    positionok2 = YES;

                }
                else
                {
                    SetPWMDutyD(PWM_RIGHTc_RIGHTm);
                    in_config(DIRECT, DIRECT);
                }

                if ((positionok1 == YES)&&(positionok2 == YES))
                {
                    positionok1 = NO;
                    positionok2 = NO;

                    countg = 0;
                    countd = 0;
                    i++;
#ifdef MODE_TEST
                    __delay_us(2000);
#endif

                }

            }
            else if (tab_command[i] == LEFT)
            {




                if (countg >= tab_para_g[i])

                {
                    SetPWMDutyG(0);
                    positionok1 = YES;


                }
                else
                {
                    SetPWMDutyG(PWM_LEFTc_LEFTm);
                    in_config(DIRECT, DIRECT);
                }
                if (countd >= tab_para_d[i])
                {

                    SetPWMDutyD(0);
                    positionok2 = YES;

                }
                else
                {
                    SetPWMDutyD(PWM_LEFTc_RIGHTm);
                    in_config(DIRECT, DIRECT);
                }

                if ((positionok1 == YES)&&(positionok2 == YES))
                {
                    positionok1 = NO;
                    positionok2 = NO;

                    countg = 0;
                    countd = 0;
                    i++;
#ifdef MODE_TEST
                    __delay_us(2000);
#endif
                }
            }
            else if (tab_command[i] == B_LEFT)
            {


                if (countg >= tab_para_g[i])

                {
                    SetPWMDutyG(0);
                    positionok1 = YES;


                }
                else
                {
                    SetPWMDutyG(PWM_BLEFTc_LEFTm);
                    in_config(INDIRECT, DIRECT);
                }
                if (countd >= tab_para_d[i])
                {

                    SetPWMDutyD(0);
                    positionok2 = YES;

                }
                else
                {
                    SetPWMDutyD(PWM_BLEFTc_RIGHTm);
                    in_config(INDIRECT, INDIRECT);
                }

                if ((positionok1 == YES)&&(positionok2 == YES))
                {
                    positionok1 = NO;
                    positionok2 = NO;

                    countg = 0;
                    countd = 0;
                    i++;
#ifdef MODE_TEST
                    __delay_us(2000);
#endif
                }
            }
            else if (tab_command[i] == B_RIGHT)
            {


                if (countg >= tab_para_g[i])

                {
                    SetPWMDutyG(0);
                    positionok1 = YES;


                }
                else
                {
                    SetPWMDutyG(PWM_BRIGHTc_LEFTm);
                    in_config(INDIRECT, DIRECT);
                }
                if (countd >= tab_para_d[i])
                {

                    SetPWMDutyD(0);
                    positionok2 = YES;

                }
                else
                {
                    SetPWMDutyD(PWM_BRIGHTc_RIGHTm);
                    in_config(INDIRECT, DIRECT);
                }

                if ((positionok1 == YES)&&(positionok2 == YES))
                {
                    positionok1 = NO;
                    positionok2 = NO;

                    countg = 0;
                    countd = 0;
                    i++;
#ifdef MODE_TEST
                    __delay_us(2000);
#endif
                }
            }
            else if (tab_command[i] == BACK)
            {
                //straight = YES;

                if (countg >= tab_para_g[i])

                {
                    SetPWMDutyG(0);
                    positionok1 = YES;


                }
                else
                {
                    SetPWMDutyG(PWM_BACKc_LEFTm);
                    in_config(INDIRECT, INDIRECT);


                }
                if (countd >= tab_para_d[i])
                {

                    SetPWMDutyD(0);
                    positionok2 = YES;

                }
                else
                {
                    SetPWMDutyD(PWM_BACKc_RIGHTm);
                    in_config(INDIRECT, INDIRECT);

                }

                if ((positionok1 == YES)&&(positionok2 == YES))
                {
                    positionok1 = NO;
                    positionok2 = NO;

                    countg = 0;
                    countd = 0;
                    i++;
#ifdef MODE_TEST
                    __delay_us(2000);
#endif
                }

            }

        }
    }
}

void InitPWMD(void) // gauche ccp1 rc2
{
    TRISC2 = 0x00;
    CCP1CON = 0x0C;

    PR2 = 0xFF;
    T2CON = 0x01;

    SetPWMDutyG(0);

    T2CON |= 0x04;
}

void InitPWMG(void) // droite ccp2 rc1.
{
    TRISC1 = 0x00;
    CCP2CON = 0x0C;

    PR2 = 0xFF;
    T2CON = 0x01;
    SetPWMDutyD(0);

    T2CON |= 0x04;
}

void SetPWMDutyD(unsigned int DutyCycle)
{
    CCPR1L = DutyCycle >> 2;
    CCP1CON &= 0xCF;
    CCP1CON |= (0x30 & (DutyCycle << 4));
}

void SetPWMDutyG(unsigned int DutyCycle)
{
    CCPR2L = DutyCycle >> 2;
    CCP2CON &= 0xCF;
    CCP2CON |= (0x30 & (DutyCycle << 4));
}

void interrupt CHECK()
{
    //INTCONbits.PEIE=1;
    if (TMR1IF == 1)
    {
        WRITETIMER1(0xFF00);
        clk_count++;
        count_s0++;
        count_s1++;
        count_s2++;
        if (count_s0 == counts0_limit)
        {
            servo1 = 1;
            servo2 = 1;
            count_s0 = 0;
            count_s1 = 0;
            count_s2 = 0;
        }
        if (count_s1 == counts1_limit)
        {
            servo1 = 0;

        }
        if (count_s2 == counts2_limit)
        {
            servo2 = 0;

        }
        if (clk_count == 23531)
        {
            clk_second++;
            if (clk_second == 90)
            {
                while (1);
            }
        }
        TMR1IF = 0;

        //return;
    }
    if (INT0IF == 1)
    {

        countg++;
        INT0IF = 0;

    }
    //

    if (INT1IF == 1)
    {
        countd++;
        INT1IF = 0;
    }
    if (INT2IF == 1)
    {
        if (INTEDG2 == 1)
        {
            TMR0 = 0;
            INTEDG2 = 0;

        }
        else
        {
            inter = READTIMER0();
            /*
            buf = ftoa(inter, &status);
            UART_Write_Text(buf);
            UART_Write(' ');
             */
            //inter = inter*cte;


            //distance = inter;
            INTEDG2 = 1;
            new_value = YES;
        }
        INT2IF = 0;

    }


}

/*
void high_priority interrupt CHECK2()
{

}
 */



void in_config(unsigned char gauche, unsigned char droite)
{
    if (gauche == DIRECT)
    {
        IN1 = 0;
        IN2 = 1;
    }
    else
    {
        IN1 = 1;
        IN2 = 0;
    }
    //
    if (droite == DIRECT)
    {
        IN3 = 0;
        IN4 = 1;
    }
    else
    {
        IN3 = 1;
        IN4 = 0;
    }

}

void sendPulse(void)
{
    PORTDbits.RD0 = 1;
    __delay_us(10);
    PORTDbits.RD0 = 0;
}

