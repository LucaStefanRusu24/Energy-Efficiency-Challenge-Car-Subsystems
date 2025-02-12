#include <libpynq.h>
#include <carlib.h>
#include <sys/time.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>




int main(void) {
  pynq_init();






  int onoff;








  printf("On or off: ");








  scanf(" %i", &onoff);








  init_pin_direction();




  if(onoff == 1){
    for(int i = 0 ; i < 32 ; i++){
      if(i < 11 && i >= 0){
        red();
        latch();
        sleep_msec(50);
      }
      else if(i < 22 && i >= 11){
        green();
        latch();
        sleep_msec(50);
      }
      else if(i < 32 && i >= 22){
        purple();
        latch();
        sleep_msec(50);
      }
    }
  }
  else if(onoff == 0){
    for(int i = 0 ; i < 96 ; i++){
      led_OFF();
    }
    latch();
  }








  // // Here I will call the example function in the carlib library. The function prototype is in carlib.h. And the function is defined in carlib.c.
  // //exampleFunction(42, "Hello, world!");
  // // Make sure to check the output of the program to see if the function is called correctly.
  // // If you want to change the function to get used to writing functions in a library, you can do that in the carlib.c file.
  // // Just make sure to recompile the library after you have made changes to the carlib.c file.
  // // This is done by going to the root of the directory and running the following command:
  // //   make install








  // gpio_init();








  // // clk pins
  // gpio_set_direction(IO_AR0, GPIO_DIR_OUTPUT); // front
  // gpio_set_direction(IO_AR3, GPIO_DIR_OUTPUT); // left
  // gpio_set_direction(IO_AR6, GPIO_DIR_OUTPUT);
  // gpio_set_direction(IO_AR9, GPIO_DIR_OUTPUT);








  // // sdi pins
  // gpio_set_direction(IO_AR1, GPIO_DIR_OUTPUT);
  // gpio_set_direction(IO_AR4, GPIO_DIR_OUTPUT);
  // gpio_set_direction(IO_AR7, GPIO_DIR_OUTPUT);
  // gpio_set_direction(IO_AR10, GPIO_DIR_OUTPUT);








  // // latch pins
  // gpio_set_direction(IO_AR2, GPIO_DIR_OUTPUT);
  // gpio_set_direction(IO_AR5, GPIO_DIR_OUTPUT);
  // gpio_set_direction(IO_AR8, GPIO_DIR_OUTPUT);
  // gpio_set_direction(IO_AR11, GPIO_DIR_OUTPUT);








  // if(onoff == 1){
  //   for(int i = 0 ; i < 96 ; i++){
  //     gpio_set_level(IO_AR1, GPIO_LEVEL_LOW);








  //     gpio_set_level(IO_AR0, GPIO_LEVEL_HIGH);
  //     sleep_msec(1);
  //     gpio_set_level(IO_AR0, GPIO_LEVEL_LOW);
  //     sleep_msec(1);








  //   }








  //   gpio_set_level(IO_AR2, GPIO_LEVEL_HIGH);
  //   sleep_msec(1);
  //   gpio_set_level(IO_AR2, GPIO_LEVEL_LOW);








   
  //   for(int i = 0 ; i < 96 ; i++){
  //     gpio_set_level(IO_AR4, GPIO_LEVEL_LOW);








  //     gpio_set_level(IO_AR3, GPIO_LEVEL_HIGH);
  //     sleep_msec(1);
  //     gpio_set_level(IO_AR3, GPIO_LEVEL_LOW);
  //     sleep_msec(1);








  //   }








  //   gpio_set_level(IO_AR5, GPIO_LEVEL_HIGH);
  //   sleep_msec(1);
  //   gpio_set_level(IO_AR5, GPIO_LEVEL_LOW);
  // }








  pynq_destroy();
  return EXIT_SUCCESS;
}























