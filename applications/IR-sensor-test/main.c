#include <libpynq.h>
#include <carlib.h>
#include <sys/time.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#define PULSE_COUNTER_RPM PULSECOUNTER1
#define PULSE_COUNTER_IR PULSECOUNTER0
#define PULSES_PER_REVOLUTION 2


int main(void) {
    // Initialize the PYNQ environment and pulse counter
    pynq_init();
    initRadio();

    uint32_t decode_value = 0;
    uint8_t car_number = 1;
    uint8_t course_code[7] = "5EWC0";

    IRDecoder *decoder = IRDecoderCreate(IRpin, PULSE_COUNTER_IR);

    while(1){
        if(IRDecoderReceive(decoder, &decode_value)){
            radioSend(course_code, car_number);
        }
        sleep_msec(10);
    }
    
    pynq_destroy(); // Clean up the PYNQ environment
    return EXIT_SUCCESS;
}


