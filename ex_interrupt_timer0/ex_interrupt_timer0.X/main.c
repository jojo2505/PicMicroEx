/*
 * File:   main.c
 * Author: aymen
 *
 * Created on 7/02/2017
 *
 * PIC18F4550 , quartz 20Mhz, Fosc = 48Mhz;
 le code controle 5 servomoteur avec les pins RB0..RB4
 la position sont donné par 5 potentiometre par A0..A4
 j'utilise une interruption qui bloque la  fct main chaque 20ms pendant 20ms
 ...
 */



#include <xc.h>
#include <stdlib.h>
#include <stdio.h>
#include "config.h"
#include "adconverter.h"


#define bitset(var, bitno) ((var) |= 1UL << (bitno))
#define bitclr(var, bitno) ((var) &= ~(1UL << (bitno)))

#define tservo0 TRISB0
#define tservo1 TRISB1
#define tservo2 TRISB2
#define tservo3 TRISB3
#define tservo4 TRISB4
//
#define servo0 RB0
#define servo1 RB1
#define servo2 RB2
#define servo3 RB3
#define servo4 RB4

unsigned char count_deg;
unsigned char deg0 = 15;
unsigned char deg1 = 30;
unsigned char deg2 = 60;
unsigned char deg3 = 90;
unsigned char deg4 = 120;

unsigned int val;
float val_inter;

void
main(void)
{

    int_converter();

    unsigned char count_deg;
    //config port B pour les servo4
    tservo0 = 0;
    tservo1 = 0;
    tservo2 = 0;
    tservo3 = 0;
    tservo4 = 0;
    //config interrupt
    //IPEN:
    //0: pas de priorit� entre les inter,
    //1: il ya preiorit� H et L
    IPEN = 0;
    //TMR1IP : priorite de de l'inetrruption timer 1
    //1 : H
    //0 : L
    //TMR1IP = 1;


    INTCONbits.GIE = 1;//activer les interruption
    // 1 = Enables all unmasked peripheral interrupts
    //0 = Disables all peripheral interrupts
    //PEIE = 1;
    // activer l'interruption timer 0
    TMR0IE = 1;
    /*
    T08BIT: Timer0 8-Bit/16-Bit Control bit
    1 = Timer0 is configured as an 8-bit timer/counter
    0 = Timer0 is configured as a 16-bit timer/counter
    */
    T08BIT = 0;
    /*
    T0CS: Timer0 Clock Source Select bit
    1 = Transition on T0CKI pin
    0 = Internal instruction cycle clock (CLKO)
    */
    T0CS = 0;
    /*
    T0SE: Timer0 Source Edge Select bit
    1 = Increment on high-to-low transition on T0CKI pin
    0 = Increment on low-to-high transition on T0CKI pin
    */
    //T0SE non utilisé dans notre car la source est le clock interne du PIC18F4550
    /*
    PSA: Timer0 Prescaler Assignment bit
    1 = TImer0 prescaler is NOT assigned. Timer0 clock input bypasses prescaler.
    0 = Timer0 prescaler is assigned. Timer0 clock input comes from prescaler output.
    */
    PSA = 0;
    /*
    T0PS2:T0PS0: Timer0 Prescaler Select bits
    111 = 1:256 Prescale value
    110 = 1:128 Prescale value
    101 = 1:64 Prescale value
    100 = 1:32 Prescale value
    011 = 1:16 Prescale value
    010 = 1:8 Prescale value
    001 = 1:4 Prescale value
    000 = 1:2 Prescale value
    */
    //128
    T0PS0 = 1;
    T0PS1 = 0;
    T0PS2 = 0;

    /*calcul pour avoir une interruption chaque 20ms
    Fosc : frequnce clock cpu dans notre cas 48Mhs
    Fcyc : Fosc/4
    prescaler :la valeur du prescaler du timer 1.
    OF :le nombre d'incrementation pour atteindre 0xff par defaut c'est (0xff+1)
    on peur le changer en ecrivant une valeur offset dans le timer dès qui 'il passe de 0xff
    à 0x00, dans ce cas OF sera egal à (0xffff+1)-offset => offset = (0xffff+1)-OF
    t_int = (1/Fosc) * 4 *OF*prescaler
    t_int  = (1/(48*10^6))*4*OF*prescaler =20ms
    OF = 20ms/((1/(48*10^6))*4*prescaler)
    offset = (0xffff+1)-20ms/((1/(48*10^6))*4*prescaler)
    pour un prescaler de 16.
    offset = 63661
    */
      //WRITETIMER0(50536);//16
      WRITETIMER0(5536);//4



    /*
    TMR0ON: Timer0 On/Off Control bit
    1 = Enables Timer0
    0 = Stops Timer0
    */
    TMR0ON = 1;


    val_inter = 180.0/1023.0;
    while(1)
    {

        val = start_convert(0);
        deg0 = val*val_inter;
        val = start_convert(1);
        deg1=val*val_inter;
        val = start_convert(2);
        deg2=val*val_inter;
        val = start_convert(3);
        deg3=val*val_inter;
        val = start_convert(4);
        deg4=val*val_inter;


    }



}
//


void interrupt CHECK()
{

    //t_int = tosc*4*pr�sclaer*(0xffff-X+1);X est la valeur qu'on ajoute au timer au debut de l'interruptio
    //dans notre cas c'est 0xff00
    //INTCONbits.PEIE=1;
    if (TMR0IF == 1)
    {
        WRITETIMER0(5536);
        //mettre tout les servo à 1
        servo0 = 1;
        servo1 = 1;
        servo2 = 1;
        servo3 = 1;
        servo4 = 1;
        __delay_us(1000);
        //1ms/180 = 0,005555556 = 5.5us
        //t_cyc = 0.083us = 83ns pour 48Mhz Fosc
        //5.5/0.083 = 66,265060241 = 66 cycles
        for(count_deg=0;count_deg<=180;count_deg++)
        {

          if(deg0 == count_deg) servo0 = 0;
          if(deg1 == count_deg) servo1 = 0;
          if(deg2 == count_deg) servo2 = 0;
          if(deg3 == count_deg) servo3 = 0;
          if(deg4 == count_deg) servo4 = 0;

          //------------------------------------------------------------
          _delay(40);
          // ce n'est pas les 66 cycles que j'ai calcuer car il ya
          // de cycles qui sont utilis� par la boucle for.
          //...


        }






        TMR0IF = 0;

        //return;
    }



}

/*
void high_priority interrupt CHECK2()
{

}
 */
