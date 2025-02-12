#include <libpynq.h>
#include <carlib.h>

#include <stdio.h>
#include <math.h>

#define TARGET_SPEED_KMH 8.0    // Target speed in km/h
#define PID_KP 0.1             // Proportional gain
#define PID_MAX 1.0             // Maximum throttle value
#define PID_MIN 0.5            // Minimum throttle value
#define DEAD_ZONE 0.7           // Dead zone to prevent unnecessary corrections

// Proportional Controller Cruise Control Function
double PController_Update(double setpoint, double measurement) {
    double error = setpoint - measurement;

    // Apply dead zone to avoid small corrections
    if (fabs(error) < DEAD_ZONE) {
        error = 0.0;
    }

    // Proportional term
    double output = PID_KP * error;

    // Clamp the output to the throttle limits
    if (output > PID_MAX) output = PID_MAX;
    if (output < PID_MIN) output = PID_MIN;

    return output;
}

// Main function to simulate the cruise control loop
int main() {
    pynq_init();
    initUART(UART0);
    pulseCounterInit();

    uint32_t count = 0, previous_count = 0;
    uint32_t timestamp = 0, previous_timestamp = 0;

    while (1) {
        double currentSpeed = current_speed(&count, &previous_count, &timestamp, &previous_timestamp);  // Get the current speed
        double throttle = PController_Update(TARGET_SPEED_KMH, currentSpeed);

        sendThrottleSetpoint(UART0, &UartTranPacket, throttle);

        // Print debug information
        printf("Target Speed: %.2f km/h, Current Speed: %.2f km/h, Throttle: %.2f\n",
               TARGET_SPEED_KMH, currentSpeed, throttle);

        previous_count = count;
        previous_timestamp = timestamp;

        // Wait for the next control loop iteration (e.g., 100 ms)
#ifdef _WIN32
        Sleep(100);  // Windows: Delay in milliseconds
#else
        struct timespec ts;
        ts.tv_sec = 0;
        ts.tv_nsec = 100 * 1000000;  // Linux/Unix: 100 milliseconds
        nanosleep(&ts, NULL);
#endif
    }

  pynq_destroy();
  return EXIT_SUCCESS;
}
