#include <libpynq.h>
#include <carlib.h>
#include <sys/time.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <math.h>
// #include "PID.h"

#define PULSE_COUNTER_RPM PULSECOUNTER1
#define PULSE_COUNTER_IR PULSECOUNTER0
#define PULSES_PER_REVOLUTION 2

// PID constants
#define TARGET_SPEED_KMH 8.0     // Target speed in km/h
#define PID_KP 0.1               // Proportional gain
#define PID_KI 0.0              // Integral gain
#define PID_KD 0.0              // Derivative gain
#define PID_MAX 0.80              // Maximum throttle
#define PID_MIN 0.50             // Minimum throttle
#define PID_INTEGRAL_LIMIT 0.3   // Integral windup limit
#define DEAD_ZONE 0.5            // Dead zone to avoid oscillations

// PID Controller structure
typedef struct {
    double integrator;
    double prevError;
    double differentiator;
    double prevMeasurement;
    double throttle;  // Store current throttle value for smoothing
} PIDController;

// Initialize PID controller
void PID_Init(PIDController *pid) {
    pid->integrator = 0.0;
    pid->prevError = 0.0;
    pid->differentiator = 0.0;
    pid->prevMeasurement = 0.0;
    pid->throttle = PID_MIN;  // Initialize throttle to minimum value
}

// Update PID controller
double PID_Update(PIDController *pid, double setpoint, double measurement, double dt) {
    // Calculate error
    double error = setpoint - measurement;

    // Apply dead zone
    if (fabs(error) < DEAD_ZONE) {
        error = 0.0;
    }

    // Proportional term
    double proportional = PID_KP * error;

    // Integral term
    pid->integrator += 0.5 * PID_KI * dt * (error + pid->prevError);

    // Clamp integrator to avoid windup
    if (pid->integrator > PID_INTEGRAL_LIMIT) {
        pid->integrator = PID_INTEGRAL_LIMIT;
    } else if (pid->integrator < -PID_INTEGRAL_LIMIT) {
        pid->integrator = -PID_INTEGRAL_LIMIT;
    }

    // Derivative term (band-limited differentiator)
    double differentiator = -(2.0 * PID_KD * (measurement - pid->prevMeasurement)
                              + (2.0 * 0.02 - dt) * pid->differentiator) / (2.0 * 0.02 + dt);

    // Compute raw PID output
    double output = proportional + pid->integrator + differentiator;

    // Clamp output to throttle range
    if (output > PID_MAX) output = PID_MAX;
    if (output < PID_MIN) output = PID_MIN;

    // Smooth throttle transitions
    pid->throttle += (output - pid->throttle) * 0.05;  // Adjust smoothing factor as needed

    // Update previous values
    pid->prevError = error;
    pid->prevMeasurement = measurement;
    pid->differentiator = differentiator;

    return pid->throttle;
}

int main() {
    pynq_init();
    initUART(UART0);
    PIDController pid;
    PID_Init(&pid);

    // Simulation time step (e.g., 10 ms)
    const double dt = 0.01;

    pulseCounterInit();

    uint32_t count = 0, previous_count = 0;
    uint32_t timestamp = 0, previous_timestamp = 0;

    double currentSpeed, throttle;

    while (1) {
        // Get current speed
        currentSpeed = current_speed(&count, &previous_count, &timestamp, &previous_timestamp);

        // Update PID controller
        throttle = PID_Update(&pid, TARGET_SPEED_KMH, currentSpeed, dt);

        // Apply throttle
        sendThrottleSetpoint(UART0, &UartTranPacket, throttle);

        // Debug output
        printf("Target Speed: %.2f km/h, Current Speed: %.2f km/h, Throttle: %.2f\n",
               TARGET_SPEED_KMH, currentSpeed, throttle);
        
        previous_count = count;
        previous_timestamp = timestamp;

        // Wait for next time step (simulate delay)
        sleep(0.5);  // Convert dt to microseconds
    }

    pynq_destroy();
    return EXIT_SUCCESS;
}

// int main(void) {
//     // Initialize the PYNQ environment and pulse counter
//     pynq_init();
//     initUART(UART0);

//     PIDController pid;

//     pid.Kp = 0.3f;
//     pid.Ki = 0.01f;
//     pid.Kd = 0.01f;
//     pid.T = 0.01f;

//     pid.limMaxInt = 0.5f;
//     pid.limMinInt = -0.05f;

//     pid.limMax = 1.0f;
//     pid.limMin = 0.5f;

//     PIDController_Init(&pid);

//     uint32_t count = 0, previous_count = 0;
//     uint32_t timestamp = 0, previous_timestamp = 0;

//     double speed = 0.0;
//     float throttle, appliedThrottle;

//     pulseCounterInit(); // Initialize the pulse counter


//     while (1) {
//         // Calculate current speed
//         speed = current_speed(&count, &previous_count, &timestamp, &previous_timestamp);

//         throttle = PIDController_Update(&pid, TARGET_SPEED, (float)speed);

//         sendThrottleSetpoint(UART0, &UartTranPacket, appliedThrottle);
//         printf("Throttle: %.2f   Speed: %.2f\n", appliedThrottle, speed);

//         // Update previous count and timestamp for the next iteration
//         previous_count = count;
//         previous_timestamp = timestamp;

//         sleep(pid.T); // Delay for 1 second
//     }

//     pynq_destroy(); 
    
//     return EXIT_SUCCESS;
// }


