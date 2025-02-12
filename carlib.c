ode;/*
      ___           ___           ___                    ___                   ___     
     /\  \         /\  \         /\  \                  /\__\      ___        /\  \    
    /::\  \       /::\  \       /::\  \                /:/  /     /\  \      /::\  \   
   /:/\:\  \     /:/\:\  \     /:/\:\  \              /:/  /      \:\  \    /:/\:\  \  
  /:/  \:\  \   /::\~\:\  \   /::\~\:\  \            /:/  /       /::\__\  /::\~\:\__\ 
 /:/__/ \:\__\ /:/\:\ \:\__\ /:/\:\ \:\__\          /:/__/     __/:/\/__/ /:/\:\ \:|__|
 \:\  \  \/__/ \/__\:\/:/  / \/_|::\/:/  /          \:\  \    /\/:/  /    \:\~\:\/:/  /
  \:\  \            \::/  /     |:|::/  /            \:\  \   \::/__/      \:\ \::/  / 
   \:\  \           /:/  /      |:|\/__/              \:\  \   \:\__\       \:\/:/  /  
    \:\__\         /:/  /       |:|  |                 \:\__\   \/__/        \::/__/   
     \/__/         \/__/         \|__|                  \/__/                 ~~       

                                _.-="_-         _
                            _.-="   _-          | ||"""""""---._______     __..
                ___.===""""-.______-,,,,,,,,,,,,`-''----" """""       """""  __'
        __.--""     __        ,'                   o \           __        [__|
    __-""=======.--""  ""--.=================================.--""  ""--.=======:
    ]       [w] : /        \ : |========================|    : /        \ :  [w] :
    V___________:|          |: |========================|    :|          |:   _-"
    V__________: \        / :_|=======================/_____: \        / :__-"
    -----------'  "-____-"  `-------------------------------'  "-____-"    
*/

/* Copyright
    Copyright (c) 2023 Eindhoven University of Technology

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

/* 
  Welcome to the Energy Challenge!
  In this library you can find (almost) all you need to program the on-board PYNQ 
  including a short description of each method in carlib.h

  @version   2.0.0
  @author    Lorenzo Ceccarelli
  @author    Martijn Koedam
  @author    Zar Eggermont
  @date      2024-10-10

  Structure:
  * LED Bars
  * UART Communication with Control Board
  * IR receiver
  * nRF24 transceiver
  * Pulse counter (speed sensor)
*/

#include "carlib.h"
#include <libpynq.h>

/* LED Bars */

/*
  Here you are going to write the functions to control the LED bars. You as a student will have to write these yourself!! :))))
  You can find more about these LED bars in the documentation on oncourse. Make sure to write the functions here in carlib.c and the prototypes in carlib.h.
  
  Your program must be in the application folder and you can use the functions in this library to be able to use all the functions in this file, 
  by including the carlib.h file.

  If you have any questions about how to write these functions in a library, you can ask the TA's for help. 
  They are here to help you and make sure you can finish the challenge.

  When you are done changing anything in the carlib.c or any other file in the library for that matter, you will need to recompile the library.
  This is done by going to the root of the directory and running the following command:
    make install
  You can find more information on the pynq.tue.nl website.

  This will recompile the library and make it available for you to use in your application.

  Good luck! :D
*/


void init_pin_direction(){
  // clk pins
  gpio_set_direction(clkPin1, GPIO_DIR_OUTPUT);
  gpio_set_direction(clkPin2, GPIO_DIR_OUTPUT);
  gpio_set_direction(clkPin3, GPIO_DIR_OUTPUT); 
  gpio_set_direction(clkPin4, GPIO_DIR_OUTPUT);

  // sdi pins
  gpio_set_direction(sdiPin1, GPIO_DIR_OUTPUT);
  gpio_set_direction(sdiPin2, GPIO_DIR_OUTPUT);
  gpio_set_direction(sdiPin3, GPIO_DIR_OUTPUT);
  gpio_set_direction(sdiPin4, GPIO_DIR_OUTPUT);

  // latch pins 
  gpio_set_direction(latchPin1, GPIO_DIR_OUTPUT);
  gpio_set_direction(latchPin2, GPIO_DIR_OUTPUT);
  gpio_set_direction(latchPin3, GPIO_DIR_OUTPUT);
  gpio_set_direction(latchPin4, GPIO_DIR_OUTPUT);
}

void led_ON(){
  gpio_set_level(sdiPin1, GPIO_LEVEL_HIGH);
  gpio_set_level(sdiPin2, GPIO_LEVEL_HIGH);
  gpio_set_level(sdiPin3, GPIO_LEVEL_HIGH);
  gpio_set_level(sdiPin4, GPIO_LEVEL_HIGH);

  gpio_set_level(clkPin1, GPIO_LEVEL_HIGH);
  sleep_msec(1);
  gpio_set_level(clkPin1, GPIO_LEVEL_LOW);

  gpio_set_level(clkPin2, GPIO_LEVEL_HIGH);
  sleep_msec(1);
  gpio_set_level(clkPin2, GPIO_LEVEL_LOW);

  gpio_set_level(clkPin3, GPIO_LEVEL_HIGH);
  sleep_msec(1);
  gpio_set_level(clkPin3, GPIO_LEVEL_LOW);

  gpio_set_level(clkPin4, GPIO_LEVEL_HIGH);
  sleep_msec(1);
  gpio_set_level(clkPin4, GPIO_LEVEL_LOW);

}


void led_OFF(){
  gpio_set_level(sdiPin1, GPIO_LEVEL_LOW);
  gpio_set_level(sdiPin2, GPIO_LEVEL_LOW);
  gpio_set_level(sdiPin3, GPIO_LEVEL_LOW);
  gpio_set_level(sdiPin4, GPIO_LEVEL_LOW);

  gpio_set_level(clkPin1, GPIO_LEVEL_HIGH);
  sleep_msec(1);
  gpio_set_level(clkPin1, GPIO_LEVEL_LOW);

  gpio_set_level(clkPin2, GPIO_LEVEL_HIGH);
  sleep_msec(1);
  gpio_set_level(clkPin2, GPIO_LEVEL_LOW);

  gpio_set_level(clkPin3, GPIO_LEVEL_HIGH);
  sleep_msec(1);
  gpio_set_level(clkPin3, GPIO_LEVEL_LOW);

  gpio_set_level(clkPin4, GPIO_LEVEL_HIGH);
  sleep_msec(1);
  gpio_set_level(clkPin4, GPIO_LEVEL_LOW);

}


void latch(){
  gpio_set_level(latchPin1, GPIO_LEVEL_HIGH);
  sleep_msec(1);
  gpio_set_level(latchPin1, GPIO_LEVEL_LOW);

  gpio_set_level(latchPin2, GPIO_LEVEL_HIGH);
  sleep_msec(1);
  gpio_set_level(latchPin2, GPIO_LEVEL_LOW);

  gpio_set_level(latchPin3, GPIO_LEVEL_HIGH);
  sleep_msec(1);
  gpio_set_level(latchPin3, GPIO_LEVEL_LOW);

  gpio_set_level(latchPin4, GPIO_LEVEL_HIGH);
  sleep_msec(1);
  gpio_set_level(latchPin4, GPIO_LEVEL_LOW);
}


void blue(){
  led_ON();
  led_OFF();
  led_OFF();
}

void green(){
  led_OFF();
  led_ON();
  led_OFF();
}

void red(){
  led_OFF();
  led_OFF();
  led_ON();
}

void purple(){
  led_ON();
  led_OFF();
  led_ON();
}

void speed_lights(char colour){
 for(int i = 0 ; i < 32 ; i++){
    switch(colour){
      case 'r': red(); break;
      case 'p': purple(); break;
      case 'g': green(); break;
      case 'n': led_OFF();
    }
  }
}

// This is an example function to introduce you to the concept of writing functions in a library.
// This function will be called in applications/example-program/main.c
void exampleFunction(int exampleArgument, char *exampleString) {
  printf("This is an example function in the library. The first argument is an int: %d and the second argument is a string: %s.\n", exampleArgument, exampleString);
}


/*=======================================================   UART Communication  ==============================================================*/

s_UartPacket UartRecvPacket;
s_UartPacket UartTranPacket;
s_EnergyValues EnergyValues;
s_PositionValues PositionValues;
s_ThrottleValues ThrottleValues;
s_UartTestValues UartTestValues;   // = {"EPE Energy Challenge", 0xAB, 1337, 3.14};

char test_data = 0xFF;
char energy_data = 0x00;
char position_data = 0x01;
char throttle_data = 0x10;

/*
  UART protocol:
  0x0D (start byte) - 0x01 (message ID) - 0x.. (size of data in bytes) - DATA/data in bytes - 0x.. (checksum, sum of all data bytes) - 0x0A

  TRANSMIT
  (start) (msg ID)  (size)  (data)  (checksum) (stop)
  0xD0    0x00      0x01    0xYY    0xYY       0x0A     --> request message, data=msg requested (00 = energy, 01 = position, 02=throttle, FF=testdata)
  0xD0    0x10      0x01    0xYY    0xYY       0x0A     --> throttle setpoint value submit

  RECEIVE
  (start) (msg ID)  (size)  (data)  (checksum) (stop)
  0xD0    0x00      0xZZ    DATA    0xYY       0x0A     --> energy data
  0xD0    0x01      0xZZ    DATA    0xYY       0x0A     --> accelerometer/gyro data
  0xD0    0x02      0x01    0xYY    0xYY       0x0A     --> throttle setpoint value response (YY=0x00 for OK 0x01 for ERROR)

*/

/*
  For the UART communication you will need to write the following functions:
  - requestEnergyData
  - requestPositionData
  - requestTestData
  - requestThrottleData
  - sendThrottleSetpoint
  - UartTransmitPacket

  It is adviced to write these functions in carlib.c instead of in your own application file. 
  This is because you can then use it in multiple files without having to copy paste. This will help you to keep your code clean and structured.

  To find out more about how to write functions in a so called 'library', you can look at the documentation on oncourse or come ask the TA's for help.

  When you are done changing anything in the carlib.c or any other file in the library for that matter, you will need to recompile the library.
  This is done by going to the root of the directory and running the following command:
    make install
  You can find more information on the pynq.tue.nl website.

  This will recompile the library and make it available for you to use in your application.

  Good luck! :D
*/


void initUART(const uart_index_t uart) {

  switchbox_set_pin(uartRX, SWB_UART0_RX);
  switchbox_set_pin(uartTX, SWB_UART0_TX);

  printf("UART0_RX -> %d\r\n", uartRX);
  printf("UART0_TX -> %d\r\n", uartTX);

  uart_init(uart);

  sleep_msec(100);
  printf("Resetting FIFOs...\r\n");

  uart_reset_fifos(uart);

}

// Is it better to have GTimer timer as an argument or to create it in the function?
int uart_recv_timeout(const uart_index_t uart, uint8_t *data)
{
  GTimer *timer = g_timer_new();
  while (g_timer_elapsed(timer, NULL) < 0.1)
  {
    if (uart_has_data(uart))
    {
      *data = uart_recv(uart);
      g_timer_destroy(timer);
      return 1;
    }
  }
  g_timer_destroy(timer);
  return 0;
}

int handleUartReceive(const uart_index_t uart, struct s_UartPacket *packet_ptr)
{

  static char buffer_checksum = 0x00; // checksum of data
  static int buffer_bytes_written = 0;

  uint8_t data = 0x00;

  bool break_loop = false;
  GTimer *timer = g_timer_new();

  while (!break_loop)
  {
    /* UART timeout watchdog */
    g_timer_start(timer);
    while (!uart_has_data(uart) && !break_loop)
    {

      if (g_timer_elapsed(timer, NULL) > UART_TIMEOUT)
      {

        pynq_warning("UART timeout, resetting...\r\n");
        if (packet_ptr->state != 0)
        {
          packet_ptr->state = 0;
        }
        uart_reset_fifos(uart);
        g_timer_destroy(timer);
        return false;
      }
    }
    // printf("UART: data available\r\n");

    // packet state machine
    switch (packet_ptr->state)
    {
    default:
    case 0:
      if (!uart_recv_timeout(uart, &data))
      {
        pynq_warning("uart_recv_timeout has timed out.\n");
        packet_ptr->state = 0;
        break_loop = true;
        break;
      }
      if (data == UART_STARTBYTE)
      {

        packet_ptr->state = 1; // start received
        packet_ptr->checksum = 0x00;
        packet_ptr->id = 0x00;
        packet_ptr->size = 0;
        buffer_bytes_written = 0;
        buffer_checksum = 0;
        if (packet_ptr->data != NULL)
        {
          free(packet_ptr->data);
          packet_ptr->data = NULL;
        }
        // printf("UART: start byte received\n");
      }
      break;
    case 1:
      if (!uart_recv_timeout(uart, &data))
      {
        pynq_warning("uart_recv_timeout has timed out.\n");
        packet_ptr->state = 0;
        break_loop = true;
        break;
      }
      packet_ptr->id = data;
      // Maybe uart_recv_timeout(uart, packet_ptr->id); and then error handling ?

      // printf("UART: received packet id = %d\n", packet_ptr->id);
      packet_ptr->state = 2; // msg id received
      break;

    case 2:
      if (!uart_recv_timeout(uart, &data))
      {
        pynq_warning("uart_recv_timeout has timed out.\n");
        packet_ptr->state = 0;
        break_loop = true;
        break;
      }
      packet_ptr->size = data;
      // Maybe uart_recv_timeout(uart, packet_ptr->size); and then error handling ?

      // printf("UART: received packet size = %d\n", packet_ptr->size);

      // check if size > 0, then wait for data, otherwise go to checksum directly
      if (packet_ptr->size > 0)
      {
        packet_ptr->data = (char *)malloc(packet_ptr->size);
        packet_ptr->state = 3; // size received
      }
      else
      {
        packet_ptr->state = 4; // size + data received
      }
      break;

    case 3:
      while ((buffer_bytes_written < packet_ptr->size))
      {
        // printf("buffer_bytes_written: %d, packet_ptr->size: %d\n", buffer_bytes_written, packet_ptr->size);
        if (!uart_recv_timeout(uart, &data))
        {
          pynq_warning("uart_recv_timeout has timed out.\n");
          break_loop = true;
          break;
        }
        packet_ptr->data[buffer_bytes_written] = data;
        // Maybe uart_recv_timeout(uart, packet_ptr->data[buffer_bytes_written]); and then error handling ?

        // printf("UART: received = %d\n", packet_ptr->data[buffer_bytes_written]);
        buffer_checksum += packet_ptr->data[buffer_bytes_written]; // keep track of checksum
        buffer_bytes_written++;

        // printf("calculated checksum: %d\r\n", buffer_checksum);
      }
      // printf("UART: data received = %s\n", packet_ptr->data);
      if (buffer_bytes_written == packet_ptr->size)
      {
        // printf("UART: data complete\n");
        packet_ptr->state = 4; // data received
      }
      else
      {
        packet_ptr->state = 0;
      }
      break;

    case 4:
      if (!uart_recv_timeout(uart, &data))
      {
        pynq_warning("uart_recv_timeout has timed out.\n");
        packet_ptr->state = 0;
        break_loop = true;
        break;
      }
      packet_ptr->checksum = data;
      // Maybe uart_recv_timeout(uart, packet_ptr->checksum); and then error handling ?

      // printf("received checksum: %d\ncalculated checksum: %d\r\n", packet_ptr->checksum, buffer_checksum);
      if (buffer_checksum == packet_ptr->checksum)
      {
        // printf("UART: checksum OK\n");
        packet_ptr->state = 5; // size received
      }
      else
      {
        pynq_warning("UART: checksum error\n");
        packet_ptr->state = 0; // discard packet
        break_loop = true;
      }
      break;

    case 5:
      if (!uart_recv_timeout(uart, &data))
      {
        pynq_warning("uart_recv_timeout has timed out.\n");
        packet_ptr->state = 0;
        break_loop = true;
        break;
      }
      if (data == UART_STOPBYTE)
      {
        //  printf("UART: first data byte = %d\n", packet_ptr->data[0]);
        packet_ptr->state = 6; // packet complete and OK

        g_timer_destroy(timer);
        return (true); // we got a complete packet
      }
      else
      {
        packet_ptr->state = 0; // discard packet
      }
      break_loop = true;
      break;
    }
  }
  g_timer_destroy(timer);
  return (false); // nothing to report
}

void requestTestData(const uart_index_t uart, struct s_UartPacket * packet_ptr) {
  UartFillPacketFromStruct(packet_ptr, 0x00, &test_data, 1);
  UartTransmitPacket(uart, packet_ptr);
  handleUartReceive(uart, &UartRecvPacket);
  UartFillStructFromPacket(&UartRecvPacket, (char *) &UartTestValues, UartRecvPacket.size);
  printf("This is the test data: %s\n", UartTestValues.s);
}

void requestEnergyData(const uart_index_t uart, struct s_UartPacket * packet_ptr) {
  UartFillPacketFromStruct(packet_ptr, 0x00, &energy_data, 1);
  UartTransmitPacket(uart, packet_ptr);
  handleUartReceive(uart, &UartRecvPacket);
  UartFillStructFromPacket(&UartRecvPacket, (char *) &EnergyValues, UartRecvPacket.size);
  printf("I0: %.2f\n", EnergyValues.I0);
}

void requestPositionData(const uart_index_t uart, struct s_UartPacket * packet_ptr) {
  UartFillPacketFromStruct(packet_ptr, 0x00, &position_data, 1);
  UartTransmitPacket(uart, packet_ptr);
  handleUartReceive(uart, &UartRecvPacket);
  UartFillStructFromPacket(&UartRecvPacket, (char *) &PositionValues, UartRecvPacket.size);
  printf("a_x: %.2f\n", PositionValues.a_x);
}

void requestThrottleData(const uart_index_t uart, struct s_UartPacket * packet_ptr) {
  UartFillPacketFromStruct(packet_ptr, 0x00, &throttle_data, 1);
  UartTransmitPacket(uart, packet_ptr);
  handleUartReceive(uart, &UartRecvPacket);
  UartFillStructFromPacket(&UartRecvPacket, (char *) &ThrottleValues, UartRecvPacket.size);
  printf("Throttle value: %.2f\n", ThrottleValues.in_remote);
}

void sendThrottleSetpoint(const uart_index_t uart, struct s_UartPacket * packet_ptr, float throttleValue) {
  UartFillPacketFromStruct(packet_ptr, 0x10, (char *)&throttleValue, sizeof(float));
  UartTransmitPacket(uart, packet_ptr);
}

int UartFillStructFromPacket(struct s_UartPacket * packet_ptr, char * data_ptr, int size){
  //printf("UART: filling struct from packet with size: %d\r\n", size);
  if(packet_ptr->size == size){ // to test
    memcpy(data_ptr, packet_ptr->data, size);
    return(false); // success
  }else{
    return(true);
  }

}

void UartFillPacketFromStruct(struct s_UartPacket * packet_ptr, int id, char * data_ptr, int size){
  //printf("UART: filling packet from struct with size: %d\r\n", size);
  packet_ptr->id=id;
  packet_ptr->size=size;
  if(packet_ptr->data != NULL){
    free(packet_ptr->data);
  }
  packet_ptr->data=(char*) malloc(packet_ptr->size);
  memcpy(packet_ptr->data, data_ptr, packet_ptr->size);
  packet_ptr->checksum = 0x00;
  for(int i=0;i<packet_ptr->size;i++){ // calc checksum of data
    packet_ptr->checksum+=packet_ptr->data[i];
    //printf("checksum: %d\n data: %d\r\n", (char) packet_ptr->checksum, packet_ptr->data[i]);
    
  }
}

void UartTransmitPacket(const uart_index_t uart, struct s_UartPacket * packet_ptr){
  uart_send(uart, UART_STARTBYTE);
  uart_send(uart, packet_ptr->id);
  uart_send(uart, packet_ptr->size);
  for(int i = 0 ; i < packet_ptr->size ; i++){
    uart_send(uart, packet_ptr->data[i]);
  }
  uart_send(uart, packet_ptr->checksum);
  uart_send(uart, UART_STOPBYTE);
}

/*=======================================================   Pulsecounter  ==============================================================*/


void initPulseCounter(const pulsecounter_index_t pulsecounter, int edges, int filter_length) {

  // Initialize pulsecounter.
  pulsecounter_init(pulsecounter);
  // Trigger on both edges.
  pulsecounter_set_edge(pulsecounter, edges);
  // Set filter to max glitch filtering.
  pulsecounter_set_filter_length(pulsecounter, filter_length);

}


/*=======================================================   IR Receiver  ==============================================================*/

IRDecoder *IRDecoderCreate(io_t pin, pulsecounter_index_t pulsecounter){
  IRDecoder *decoder = (IRDecoder *)malloc(sizeof(IRDecoder));
  decoder->pin = pin;
  decoder->pulsecounter = pulsecounter;
  decoder->state = IDLE;
  decoder->gap = false;

  gpio_set_direction(decoder->pin, GPIO_DIR_INPUT);

  if (pulsecounter == PULSECOUNTER0)
  {
    switchbox_set_pin(decoder->pin, SWB_TIMER_IC0);
  }
  else if(pulsecounter == PULSECOUNTER1)
  {
    switchbox_set_pin(decoder->pin, SWB_TIMER_IC1);
  }
  // Initialize pulsecounter. With both edges and a filter of 15.
  initPulseCounter(pulsecounter, 3, 15);

  return decoder;
}

void IRDecoderDestroy(IRDecoder *decoder){
  pulsecounter_destroy(decoder->pulsecounter);
  free(decoder);
}

bool IRDecoderPulse(IRDecoder *decoder, double pulse, uint32_t *value){
  if (pulse >= 0) {
    if (decoder->gap) {
      if (pulse > (GAP - MARGIN) && pulse < (GAP + MARGIN)) {
        decoder->gap = false;
      } else {
        if (decoder->nbits == 2) {
          *value = decoder->value;
          decoder->gap = false;
          decoder->state = IDLE;
          decoder->value = 0;
          decoder->nbits = 0;
          return true;
        }
        decoder->gap = false;
        decoder->state = IDLE;
        decoder->value = 0;
        decoder->nbits = 0;
      }

    } else {
      // end of mark.
      if (decoder->state == IDLE) {
        if (pulse > (STARTBIT - MARGIN) && pulse < (STARTBIT + MARGIN)) {
          decoder->state = BITS;
          decoder->value = 0;
          decoder->nbits = 0;
          decoder->gap = true;
        }
      } else if (decoder->state == BITS) {
        if (pulse > (MARKZERO - MARGIN) && pulse < (MARKZERO + MARGIN)) {
          // 0
          decoder->nbits++;
          decoder->gap = true;
        } else if (pulse > (MARKONE - MARGIN) && pulse < (MARKONE + MARGIN)) {
          // 1
          decoder->nbits++;
          decoder->value <<= 1;
          decoder->value |= 1;
          decoder->gap = true;
        } else {
          // error
          decoder->state = IDLE;
          decoder->value = 0;
          decoder->nbits = 0;
          decoder->gap = false;
        }
      }
    }
  }
  return false;
}

bool IRDecoderReceive(IRDecoder *decoder, uint32_t *value)
{
  uint32_t l = pulsecounter_get_pulsetrain_count(PULSECOUNTER0);
    if (l > 1) {
      // Allocate a buffer for the train.
      uint32_t *train = (uint32_t*)malloc(l * sizeof(uint32_t));
      memset(train, 0, l * sizeof(uint32_t));
      // uint32_t train[64] = {
      //     0,
      // };
      // memset(train, 0, sizeof(train));
      // Receive the train and feed it to the decoder.
      uint32_t m = pulsecounter_get_pulsetrain(PULSECOUNTER0, &train[0], l);
      while (m > 1) {
        for (uint32_t j = 1; j < m && train[j] != 0; j++) {
          // Calculate the delay between pulses.
          // This will go wrong on between two sets of 'trains',
          // but for ir we don't care to much if we skip.
          uint32_t diff = 0;
          // Calulate diff, taking roll over (every 42 seconds)
          // into account.
          if (train[j - 1] > train[j]) {
            diff = train[j] + (UINT32_MAX - train[j - 1]);
          } else {
            diff = train[j] - train[j - 1];
          }
          printf("%.6f\n", diff / 100.0e6);
          // Feed.
          uint32_t value_ = 0;
          if (IRDecoderPulse(decoder, diff / 100.0e6, &value_)) {
            // printf("Received signal: %u\n", value_);
            *value = value_;
            return true;
          }
        }
        m = pulsecounter_get_pulsetrain(PULSECOUNTER0, &train[0], l);
      }
      free(train);
    }
    return false;
}

/*=======================================================   nRF24 Transceiver  ==============================================================*/

void handleRadio(uint8_t type, uint8_t num, uint8_t *buf, uint8_t len) {
  //printf("A callback\r\n");
  switch (type) {
  case NRF24L01_INTERRUPT_RX_DR: {
    uint8_t i;

    nrf24l01_interface_debug_print(
        "nRF24L01: IRQ receive with pipe %d with %d.\n", num, len);
    for (i = 0; i < len; i++) {
      nrf24l01_interface_debug_print("0x%02X ", buf[i]);
    }
    nrf24l01_interface_debug_print(".\n");

    break;
  }
  case NRF24L01_INTERRUPT_TX_DS: {
    nrf24l01_interface_debug_print("nRF24L01: IRQ sent ok.\n");

    break;
  }
  case NRF24L01_INTERRUPT_MAX_RT: {
    nrf24l01_interface_debug_print("nRF24L01: IRQ reach max retry times.\n");

    break;
  }
  case NRF24L01_INTERRUPT_TX_FULL: {
    break;
  }
  default: {
    break;
  }
  }
}

void initRadio(void) {

  uint8_t res;
  
  res = nrf24l01_basic_init(NRF24L01_TYPE_TX, handleRadio);
  if (res != 0) {
    pynq_error("nRF24L01: basic init failed\r\n");
    //return EXIT_FAILURE;
  }
  else printf("Radio: ON\r\n");

}

void radioSend(uint8_t address[], uint8_t car) {

  uint8_t sserdda[5];

  for (int i = 0; i < (int) sizeof(sserdda); i++) {
    sserdda[4-i] = address[i]; //reverses the address bytes
  }

  if (nrf24l01_basic_sent((uint8_t *)sserdda, (uint8_t *)&car,
                        sizeof(car)) != 0) {
  //(void)nrf24l01_basic_deinit();
  printf("nRF24L01: basic sent failed\r\n");
  // return EXIT_FAILURE;
  }
  else printf("nRF24L01: message sent correctly\r\n");

}

void radioDestroy(void) {

  if (nrf24l01_basic_deinit() != 0) {
    pynq_error("nRF24L01: deinit failed\r\n");
  }
  else printf("Radio: OFF\r\n");

}

/*=======================================================   Pulse Counter  ==============================================================*/


void pulseCounterInit(void) {

  switchbox_set_pin(hallPin, SWB_TIMER_IC1);
  pulsecounter_init ( PULSECOUNTER1 );
  pulsecounter_reset_count( PULSECOUNTER1 );
  pulsecounter_set_edge( PULSECOUNTER1, GPIO_LEVEL_HIGH );
  pulsecounter_set_filter_length( PULSECOUNTER1, 10);

}

double current_speed(uint32_t *count, uint32_t *previous_count, 
                     uint32_t *timestamp, uint32_t *previous_timestamp) {
    int rpm = 0;
    double wheel_speed = 0.0;

    // Get the current pulse count and timestamp
    *count = pulsecounter_get_count(PULSECOUNTER1, timestamp);

    // Ensure a previous timestamp exists for a valid calculation
    if (*previous_timestamp != 0) {
        uint32_t count_difference = *count - *previous_count;
        uint32_t time_difference = *timestamp - *previous_timestamp;

        // Convert time difference to seconds (assuming timestamp is in nanoseconds)
        double time_difference_in_seconds = time_difference / 1e8;

        if (time_difference_in_seconds > 0) {
            // Calculate RPM: (count_difference * 60) / (pulses_per_revolution * time in seconds)
            rpm = (count_difference * 60) / (2 * time_difference_in_seconds);

            // Calculate wheel speed in km/h
            // wheel_speed = (rpm * pi * diameter of wheel in meters * 60) / (gear ratio * 1000)
            wheel_speed = (rpm * 3.1416 * 0.125 * 60) / (3 * 1000);
        }
    }

    return wheel_speed; // Return speed or 0 if no valid data
}

