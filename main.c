#include <msp430.h>

void LEDSetup();
void timerSetup();
void resetColor();
void UARTSetup();

volatile int byte = 0;
volatile int buffer = 0;

void LEDSetup(void)
{
    P1DIR |= BIT2;                              // Sets the Red LED to the output direction
    P1SEL |= BIT2;                              // BIT2 = TA0.1 output

    P1DIR |= BIT3;                              // Sets the Green LED to the output direction
    P1SEL |= BIT3;                              // BIT3 = TA0.2 output

    P1DIR |= BIT4;                              // Sets the Blue LED to the output direction
    P1SEL |= BIT4;                              // BIT4 = TA0.3 output
}

void TimerSetup(void)
{
    TA0CTL = TASSEL_2 + MC_1;                   // SMCLK set to UP mode
    TA0CCR0 = 255;                              // PWM Period

    TA0CCR1 = 0;                                // PWM of Red LED
    TA0CCTL1 = OUTMOD_2;                        // Toggle/Reset

    TA0CCR2 = 0;                                // PWM of Green LED
    TA0CCTL2 = OUTMOD_2;                        // Toggle/Reset

    TA0CCR3 = 0;                                //PWM of Blue LED
    TA0CCTL3 = OUTMOD_2;                        // Toggle/Reset
}

void UARTSetup(void)
{
    P4SEL |= (BIT4+BIT5);                       // BIT4 = TXD output || BIT5 = RXD input
    UCA1CTL1 |= UCSWRST;                        // State Machine Reset + Small Clock Initialization
    UCA1CTL1 |= UCSSEL_2;                       // Sets USCI Clock Source to SMCLK
    UCA1BR0 = 6;                                // Setting the Baud Rate to be 9600
    UCA1BR1 = 0;                                // ^
    UCA1MCTL |= UCBRS_0 + UCBRF_13 + UCOS16;
    UCA1CTL1 &= ~UCSWRST;                       // Initialize USCI State Machine
    UCA1IE |= UCRXIE;                           // Enable USCI_A0 RX interrupt

}

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;                   // Stop Watchdog Timer
    LEDSetup();
    TimerSetup();
    UARTSetup();

    __bis_SR_register(LPM0_bits + GIE);         // Low Power Mode
    __no_operation();
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
    #pragma vector=USCI_A1_VECTOR
    __interrupt void USCI_A1_ISR(void)
#elif defined(__GNUC__)
    void __attribute__ ((interrupt(USCI_A1_VECTOR))) USCI_A1_ISR (void)
#else
    #error Compiler not supported!
#endif
{
    switch(__even_in_range(UCA1IV,4))
    {
    case 0 :break;
    case 2:
          switch(byte)
              {
              case 0:
                  while(!(UCA1IFG & UCTXIFG));
                  buffer = UCA1RXBUF + 1;
                  UCA1TXBUF = buffer - 4;       // Removes three bytes and transfers the rest to the next device
                  break;

              case 1:
                  TA0CCR1 = (UCA1RXBUF);        // Sets PWM of Red LED
                  break;

              case 2:
                  TA0CCR2 = (UCA1RXBUF);        // Sets PWM of Green LED
                  break;

              case 3:
                  TA0CCR3 = (UCA1RXBUF);        // Sets PWM of Blue LED
                  break;

              default:
                  while(!(UCA1IFG & UCTXIFG));  // While no transmit interrupt and no USCI_A! interrupt
                  UCA1TXBUF = UCA1RXBUF;        // Transfers those bytes to the next device
                  break;
              }

              if(byte != buffer - 1)
              {
                  byte += 1;                    // Increments to next byte
              }

              else if (byte == buffer - 1)
              {
                   byte = 0;                    // Reset Count
              }
    break;
    case 4: break;
    default: break;
    }
}
