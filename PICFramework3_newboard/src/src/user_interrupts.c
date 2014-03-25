// This is where the "user" interrupts handlers should go
// The *must* be declared in "user_interrupts.h"

#include "maindefs.h"
#ifndef __XC8
#include <timers.h>
#else
#include <plib/timers.h>
#endif
#include "user_interrupts.h"
#include "messages.h"

// A function called by the interrupt handler
// This one does the action I wanted for this program on a timer0 interrupt

void timer0_int_handler() {
    WriteTimer0(0);
//    unsigned int val;
//    int length, msgtype;

    // toggle an LED
#ifdef __USE18F2680
    LATBbits.LATB0 = !LATBbits.LATB0;
#endif
    // reset the timer
    
    // try to receive a message and, if we get one, echo it back
//    length = FromMainHigh_recvmsg(sizeof(val), (unsigned char *)&msgtype, (void *) &val);
//    if (length == sizeof (val)) {
//        ToMainHigh_sendmsg(sizeof (val), MSGT_TIMER0, (void *) &val);
//    }

    UART_timeout++;

    //unsigned char motorcomm[2] = {0x9F, 0x1F};
//    unsigned char motorcomm[2] = {0x00, 0x00};
//    uart_trans(2, motorcomm);
    //ConvertADC();
    //while( BusyADC()) {
        //LATBbits.LATB1 = 1;
    //}
    //LATBbits.LATB1 = 0;

    
}

// A function called by the interrupt handler
// This one does the action I wanted for this program on a timer1 interrupt

void timer1_int_handler() {
    //unsigned int result;

    // read the timer and then send an empty message to main()
#ifdef __USE18F2680
    LATBbits.LATB1 = !LATBbits.LATB1;
#endif

    //result = ReadTimer1();
    //ToMainLow_sendmsg(0, MSGT_TIMER1, (void *) 0);

    // reset the timer
    WriteTimer1(0);
}

void adc_int_handler(){

    // adcbuffer[0] stores the count
    if(adcbuffer[0] < 3)                 // increment counter
        adcbuffer[0] = adcbuffer[0] + 1;
    else
        adcbuffer[0] = 1;
        
    // Converts ADRESH to a voltage    
    int k = (int)ADRESH;
    float voltage = 3.4*k/256;
    int roundDist;
    
    // If the voltage is larger than .5 V this runs it through a simplifed equation to return the approximate distance
    // WARNING:  This equation is not completely accurate so results may vary until it is improved
    if (voltage > .5) {
        float dist = 24/(voltage - 0.1);
        roundDist = (int)(dist + 0.5);
    }
    // If the voltage is too small then the object is assumed to be too far away so we send back 0xFF
    else
        roundDist = 0xFF;

    // Reads the current channel from the count of the adcbuffer[0]
    // Puts the current distance of the respective sensor in the adcbuffer then changes to the next channel
    int channel = (int)adcbuffer[0] % 3;
    if (channel == 0) {
        adcbuffer[3] = roundDist;
        SetChanADC(ADC_CH1);
    }
    else if (channel == 1) {
        adcbuffer[1] = roundDist;
        SetChanADC(ADC_CH2);
    }
    else {
        adcbuffer[2] = roundDist;
        SetChanADC(ADC_CH3);
    }
}
