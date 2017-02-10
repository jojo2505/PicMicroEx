#include <xc.h>

void timer0_init(void)
{
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
  //4
  T0PS0 = 1;
  T0PS1 = 0;
  T0PS2 = 0;
  /*
    TMR0ON: Timer0 On/Off Control bit
    1 = Enables Timer0
    0 = Stops Timer0
    */
    TMR0ON = 1;



}
