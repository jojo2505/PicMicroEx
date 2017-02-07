/*
 * File:   main.c
 * Author: aymen
 *
 * Created on 7/02/2017 
 * 
 * PIC18F4550 , quartz 20Mhz, Fosc = 48Mhz;
 */



#include <xc.h>
#include <stdlib.h>
#include <stdio.h>
#include "config.h"


#define bitset(var, bitno) ((var) |= 1UL << (bitno))
#define bitclr(var, bitno) ((var) &= ~(1UL << (bitno)))



void
main(void)
{

    //config port B
    TRISB=0x00; //output
    PORTB = 0x00;
    //config interrupt
    //IPEN: 
    //0: pas de priorité entre les inter, 
    //1: il ya preiorité H et L
    IPEN = 0;
    //TMR1IP : priorite de de l'inetrruption timer 1
    //1 : H
    //0 : L
    //TMR1IP = 1; 
    

    INTCONbits.GIE = 1;//activer les interruption
    // 1 = Enables all unmasked peripheral interrupts
    //0 = Disables all peripheral interrupts
    PEIE = 1;
    // activer l'interruption timer 1
    TMR1IE = 1;
    //config timer temps 90s 
    T1CONbits.RD16 = 1; // 1: 16bits 0: 8bits
    //
    /*
     T1CKPS1:T1CKPS0: Timer1 Input Clock Prescale Select bits
    11 = 1:8 Prescale value
    10 = 1:4 Prescale value
    01 = 1:2 Prescale value
    00 = 1:1 Prescale value
     
     */
    T1CONbits.T1CKPS0 = 1; 
    T1CONbits.T1CKPS1 = 0; 
    //
    T1CONbits.T1OSCEN = 0; //0= Timer1 oscillator is shut off; 1: ON 
    //TMR1CS:
    //0: internal clock fosc/4 ;
    //1 = External clock from RC0/T1OSO/T13CKI pin (on the rising edge)
    T1CONbits.TMR1CS = 0; 
    //
    T1CONbits.TMR1ON = 1;// activer le timer 
    //
    WRITETIMER1(0xFF00);// ecrire une  valeur  dans le timer
    
    while(1);
}



void interrupt CHECK()
{
    
    //t_int = tosc*4*présclaer*(0xffff-X+1);X est la valeur qu'on ajoute au timer au debut de l'interruptio
    //dans notre cas c'est 0xff00
    //INTCONbits.PEIE=1;
    if (TMR1IF == 1)
    {
        WRITETIMER1(0xFF00);
        // pour ecrie lasortie des pins pour les 18f on utilise le registre LAT
        //on peut le faire avec le registre portB mais ça peut causer des problèmes
        LATB =  ~LATB ; 
       
        TMR1IF = 0;

        //return;
    }
    
   

}

/*
void high_priority interrupt CHECK2()
{

}
 */



