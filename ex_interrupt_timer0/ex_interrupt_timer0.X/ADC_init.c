#include <xc.h>

void ADC_init(void)
{
/*
    //datasheet page 398 18f4550
    Tad:AD clack period min=0.7us ; max= 25us ; Tosc based , vref >= 3.0
    Tcnv: conversion time : min=11, max=12 Tad

    */
    //Tad for 48Mhz Fosc// datasheet page 265
    //ADCS2:ADCS0 = 110
    ADCS0 = 0;
    ADCS1 = 1;
    ADCS2 = 1;
    // ACQT2:ACQT0: A/D Acquisition Time Select bits
    // select acuisition to be at the max 20Tad
    ACQT0 = 1;
    ACQT1 = 1;
    ACQT2 = 1;
    //ADFM: A/D Result Format Select bit
    //1 = Right justified
    //0 = Left justified
    ADFM = 1;

//config AD to 5 inputs
//datasheet page 260
//PCFG3:PCFG0
//AN0 to AN4
PCFG0 = 0;
PCFG1 = 1;
PCFG2 = 0;
PCFG3 = 1;
/*
ADON: A/D On bit
1 = A/D converter module is enabled
0 = A/D converter module is disabled
*/
ADON = 1;
/*temps max de conversion pour cette config
12*Tad+20Tad = (12+20)*1/48Mhz *64=169us
*/
}



unsigned int start_convert(unsigned char channel)
{


  unsigned int  val;
  unsigned char *pval1, *pval2;
  pval1 = &val;
  pval2 = pval1+1;
ADIF =0; // remettre le flag à 0 par securité
//CHS3:CHS0: Analog Channel Select bits
switch(channel)
{
  case 0:
    CHS0 = 0;
    CHS1 = 0;
    CHS2 = 0;
    CHS3 = 0;
    break;
  case 1:
    CHS0 = 1;
    CHS1 = 0;
    CHS2 = 0;
    CHS3 = 0;
    break;
  case 2:
    CHS0 = 0;
    CHS1 = 1;
    CHS2 = 0;
    CHS3 = 0;
    break;
  case 3:
    CHS0 = 1;
    CHS1 = 1;
    CHS2 = 0;
    CHS3 = 0;
    break;
  case 4:
    CHS0 = 0;
    CHS1 = 0;
    CHS2 = 1;
    CHS3 = 0;
    break;
  default : return 0;

  }
  while (ADCON0bits.DONE == 1); // le code reste sue cette ligne tant que le ADC est occupé
  ADCON0bits.GO =1; // lancer la conversion
  while (ADCON0bits.DONE == 1);
  *pval1 = ADRESL;
  *pval2 = ADRESH;
return val;



}
