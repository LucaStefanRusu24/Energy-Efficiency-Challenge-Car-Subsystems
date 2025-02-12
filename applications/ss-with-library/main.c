#include <libpynq.h>
#include <carlib.h>
#include <sys/time.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#define PULSE_COUNTER_ID PULSECOUNTER1
#define PULSES_PER_REVOLUTION 2


// double current_speed(uint32_t *count, uint32_t *previous_count, 
//                      uint32_t *timestamp, uint32_t *previous_timestamp) {
//     int rpm = 0;
//     double wheel_speed = 0.0;

//     // Get the current pulse count and timestamp
//     *count = pulsecounter_get_count(PULSE_COUNTER_ID, timestamp);

//     // Ensure a previous timestamp exists for a valid calculation
//     if (*previous_timestamp != 0) {
//         uint32_t count_difference = *count - *previous_count;
//         uint32_t time_difference = *timestamp - *previous_timestamp;

//         // Convert time difference to seconds (assuming timestamp is in nanoseconds)
//         double time_difference_in_seconds = time_difference / 1e8;

//         if (time_difference_in_seconds > 0) {
//             // Calculate RPM: (count_difference * 60) / (pulses_per_revolution * time in seconds)
//             rpm = (count_difference * 60) / (PULSES_PER_REVOLUTION * time_difference_in_seconds);

//             // Calculate wheel speed in km/h
//             // wheel_speed = (rpm * pi * diameter of wheel in meters * 60) / (gear ratio * 1000)
//             wheel_speed = (rpm * 3.1416 * 0.125 * 60) / (3 * 1000);
//         }
//     }

//     return wheel_speed; // Return speed or 0 if no valid data
// }



int main(void) {
    // Initialize the PYNQ environment and pulse counter
    pynq_init();

    uint32_t count = 0, previous_count = 0;
    uint32_t timestamp = 0, previous_timestamp = 0;

    double speed = 0.0;

    pulseCounterInit(); // Initialize the pulse counter

    while (1) {
        // Calculate current speed
        speed = current_speed(&count, &previous_count, &timestamp, &previous_timestamp);

        // Display the speed
        printf("Speed: %.2f km/h\n", speed);

        // Update previous count and timestamp for the next iteration
        previous_count = count;
        previous_timestamp = timestamp;

        sleep(1); // Delay for 1 second
    }

    pynq_destroy(); // Clean up the PYNQ environment
    return EXIT_SUCCESS;
}

