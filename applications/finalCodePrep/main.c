#include <libpynq.h>
#include <carlib.h>

#define PULSE_COUNTER_IR PULSECOUNTER0

int main(void) {
  pynq_init();
  pulseCounterInit();
  initRadio();
  init_pin_direction();
  initUART(UART0);

  // Initialize the PYNQ environment and pulse counter
  uint32_t count = 0, previous_count = 0;
  uint32_t timestamp = 0, previous_timestamp = 0;

  double speed = 0.0;
  double throttle;

  uint32_t decode_value = 0;
  uint8_t car_number = 1;
  uint8_t course_code[5] = "5EWC0";

  IRDecoder *decoder = IRDecoderCreate(IRpin, PULSE_COUNTER_IR);
  
  while (1) {
      if(IRDecoderReceive(decoder, &decode_value)){
          radioSend(course_code, car_number);
      }

      speed = current_speed(&count, &previous_count, &timestamp, &previous_timestamp);
      printf("speed: %.2f\n", speed);

      throttle = PController_Update(TARGET_SPEED_KMH, currentSpeed);
      sendThrottleSetpoint(UART0, &UartTranPacket, throttle);


      if(speed < 6){
        speed_lights('r');
      }
      else if(speed <= 9){
        speed_lights('g');
      }
      else if(speed > 9){
        speed_lights('p');
      }
      else if(speed == 0){
        speed_lights('n');
      }
      latch();

      // Update previous count and timestamp for the next iteration
      previous_count = count;
      previous_timestamp = timestamp;
      sleep_msec(100); 
    }

  pynq_destroy(); // Clean up the PYNQ environment
  return EXIT_SUCCESS;
}