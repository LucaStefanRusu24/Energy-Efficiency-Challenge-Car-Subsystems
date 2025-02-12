#include <libpynq.h>
#include <carlib.h>
#include <sys/time.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#define PULSE_COUNTER_ID PULSECOUNTER1
#define PULSES_PER_REVOLUTION 2
#define PI 3.1416


int main(void) {
    // Initialize the PYNQ environment and pulse coun
    pynq_init();
    
    uint32_t count =  0, previous_count = 0;
    uint32_t timestamp = 0, previous_timestamp =  0;

    int rpm;
    double wheel_speed;

    pulseCounterInit();

    while(1){
        count = pulsecounter_get_count(PULSE_COUNTER_ID, &timestamp);

        if(previous_timestamp != 0){
            uint32_t count_difference = count - previous_count;
            uint32_t time_difference = timestamp - previous_timestamp;

            uint32_t time_difference_in_seconds = time_difference / 100000000;

            if(time_difference > 0){
                rpm = (count_difference * 60) / (PULSES_PER_REVOLUTION * (time_difference_in_seconds));
                wheel_speed = (rpm * PI * 0.125 * 60) / (3*1000);
                printf("Speed: %.2f\n", wheel_speed);
            }
            else{
                printf("NON POSITIVE TIME DIFFERENCE");
            }
        }
        else{
            printf("NOTHIN");
        }

        previous_count = count;
        previous_timestamp = timestamp;

        sleep(1);
    }

    pynq_destroy();
    return EXIT_SUCCESS;
}
