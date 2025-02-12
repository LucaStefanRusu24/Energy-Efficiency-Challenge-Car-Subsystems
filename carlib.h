/*
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
  including short description of each method 

  @version   1.0.0
  @author    Lorenzo Ceccarelli
  @author    Martijn Koedam
  @author    Zar Eggermont
  @date      2024-10-11

  Structure:
  * LED Bars
  * UART Communication with Control Board
  * IR receiver
  * nRF24 transceiver
  * Pulse counter (speed sensor)
*/

#ifndef CARLIB_H
#define CARLIB_H

#include <libpynq.h>
#include <time.h>

#include <glib-unix.h>
#include <glib.h>

#include "driver_nrf24l01_basic.h"
#include <stdbool.h>
#include <unistd.h>
#include <pulsecounter.h>

#undef LOG_DOMAIN
#define LOG_DOMAIN "nrf"

/* LED Bars */

// LED bar pins

  #define clkPin1 IO_AR0    // the number of the pin connected to the LED bar 1 CLK
  #define sdiPin1 IO_AR1    // the number of the pin connected to the LED bar 1 SDI
  #define latchPin1 IO_AR2      // the number of the pin connected to the LED bar 1 LATCH
  #define clkPin2 IO_AR3    // the number of the pin connected to the LED bar 2 CLK
  #define sdiPin2 IO_AR4    // the number of the pin connected to the LED bar 2 SDI
  #define latchPin2 IO_AR5      // the number of the pin connected to the LED bar 2 LATCH
  #define clkPin3 IO_AR6    // the number of the pin connected to the LED bar 3 CLK
  #define sdiPin3 IO_AR7    // the number of the pin connected to the LED bar 3 SDI
  #define latchPin3 IO_AR8      // the number of the pin connected to the LED bar 3 LATCH
  #define clkPin4 IO_AR9    // the number of the pin connected to the LED bar 4 CLK
  #define sdiPin4 IO_AR10    // the number of the pin connected to the LED bar 4 SDI
  #define latchPin4 IO_AR11      // the number of the pin connected to the LED bar 4 LATCH
  
// methods 

/* 
Here you will have to write the  prototypes of the functions that you will write in the carlib.c file.
You can find more information about the LED bars in the documentation on oncourse.
*/

void init_pin_direction();
void led_ON();
void led_OFF();
void latch();
void blue();
void green();
void red();
void purple();
void speed_lights(char colour);

// This is an example function to introduce you to the concept of writing functions in a library.
// This function will be called in applications/example-program/main.c
void exampleFunction(int exampleArgument, char *exampleString);

//*=======================================================   UART Communication  ==============================================================*/

/** @defgroup UART_Car UART Communication
 * @brief Methods for UART communication with the control board.
 * @details
 * The UART communication is done via the PYNQ GPIOs.
 * The UART communication is done via the UART protocol.
 *
 * @section UART_PROTOCOL UART protocol
 * The UART communication is controlled by the UartPacket data structure.
 * The UartPacket data structure contains the following fields:
 * * state: the state of the UART communication
 * * id: the ID code of the UART packet (0x00 for requesting, 0x10 for sending)
 * * size: the size of the packet
 * * data: the packet data
 * * checksum: the checksum of the packet
 *
 * UART packets are sent as follows:
 * | Start | Message ID | Size | Data          | Checksum (sum of all data bytes) | Stop |
 * |-------|------------|------|---------------|----------------------------------|------|
 * | 0x0D  | 0x01       | 0x.. | Data in bytes | 0x..                             | 0x0A |
 *
 * Transmit:
 * | Start | Message ID | Size | Data | Checksum | Stop | Description |
 * |-------|------------|------|------|----------|------|-|
 * | 0xD0  | 0x00       | 0x01 | 0xYY | 0xYY     | 0x0A | --> request message, data=msg requested (00 = energy, 01 = position, 02=throttle, FF=testdata)|
 * | 0xD0  | 0x10       | 0x01 | 0xYY | 0xYY     | 0x0A | --> throttle setpoint value submit|
 *
 * Receive:
 * | Start | Message ID | Size | Data | Checksum | Stop | Description |
 * |-------|------------|------|------|----------|------|-|
 * | 0xD0  | 0x00       | 0xZZ | DATA | 0xYY     | 0x0A | --> energy data|
 * | 0xD0  | 0x01       | 0xZZ | DATA | 0xYY     | 0x0A | --> accelerometer/gyro data|
 * | 0xD0  | 0x02       | 0x01 | 0xYY | 0xYY     | 0x0A | --> throttle setpoint value response (YY=0x00 for OK 0x01 for ERROR)|
 *
 * @{
 **/

#define UART_STARTBYTE 0xD0
#define UART_STOPBYTE 0x0A
#define UART_MSGID_REQUEST 0x00
#define UART_MSGID_SEND 0x10

#define UART_MSGID_THROTTLE_OK 0x00
#define UART_MSGID_THROTTLE_ERROR 0x01

#define UART_TIMEOUT 0.1 // seconds timeout before state is reset
#define uartRX IO_RBPI36
#define uartTX IO_RBPI38

extern char UART_MSGID_ENERGY;
extern char UART_MSGID_POSITION;
extern char UART_MSGID_THROTTLE;
extern char UART_MSGID_TESTDATA;

/**
 * @brief Structure for UART packets
 **/
typedef struct s_UartPacket
{
  /** @brief state: 0=no packet, 1=start, 2=msg id, 3=size, 4=data, 5=checksum, 6=valid packet in buffer **/
  uint8_t state;
  /** @brief msg id: 0x00 for requesting, 0x10 for sending **/
  uint8_t id;
  /** @brief length of msg **/
  uint8_t size;
  /** @brief packet data **/
  char *data;
  /** @brief checksum: the sum of all the data bytes. Will be checked when a packet is received **/
  char checksum;
} s_UartPacket;

/** @brief Structure for test values **/
typedef struct s_UartTestValues
{
  char s[21];
  char c;
  int i;
  float f;
} s_UartTestValues;

/** @brief Structure for the readings of voltage/current/power/energy **/
typedef struct s_EnergyValues
{
  float I0, V0, P0, E0;
  float I1, V1, P1, E1;
} s_EnergyValues;

/** @brief Structure the readings of the accelerometer/gyro/position **/
typedef struct s_PositionValues
{
  float a_x, a_y, a_z;
  float v_abs;
  float v_x, v_y, v_z;
  float s_abs;
  float s_x, s_y, s_z;

  float gyro_x, gyro_y, gyro_z;
  float angle_z;

  float offset_a_x, offset_a_y, offset_a_z;
  float offset_gyro_x, offset_gyro_y, offset_gyro_z;
} s_PositionValues;

/** @brief Structure for throttle values **/
typedef struct s_ThrottleValues
{
  /** @brief throttle values from remote and PYNQ */
  float in_remote, in_pynq;
  /** @brief throttle values to ESC */
  float out;
} s_ThrottleValues;

extern s_UartPacket UartRecvPacket;
extern s_UartPacket UartTranPacket;
extern s_EnergyValues EnergyValues;
extern s_PositionValues PositionValues;
extern s_ThrottleValues ThrottleValues;
extern s_UartTestValues UartTestValues; // = {"EPE Energy Challenge", 0xAB, 1337, 3.14};

// UART methods

/**
 * @brief Initailizes 2 PYNQ GPIOs as UART RX and TX, flushes FIFOs
 * @param uart the UART designator name
 */
void initUART(const uart_index_t uart);

/**
 * @brief Receives data from a UART connection with a timeout.
 *
 * This function receives data from the specified UART connection with a timeout.
 * It takes the UART designator name and a pointer to the buffer for the received data as parameters.
 * If no data is received within the timeout period, it returns 0.
 * If data is received, it returns 1.
 *
 * @param uart The UART designator name.
 * @param data Pointer to the buffer for the received data.
 * @return int Returns 0 if no data is received, 1 if data is received.
 */
int uart_recv_timeout(const uart_index_t uart, uint8_t *data);

/**
 *  @brief waits for incoming UART packets, reads and allocates the content
 *  @param uart the UART designator name
 *  @param packet_ptr points to the packet data structure to fill with the incoming bytes
 *  @returns 0 if no packet is received, 1 if a packet is received
 *  @warning the method times out after UART_TIMEOUT ms if nothing is received
 **/
int handleUartReceive(const uart_index_t uart, struct s_UartPacket *packet_ptr);

/**
 * @brief Fills a UART packet from a data structure.
 *
 * @param packet_ptr Pointer to the packet data structure to fill with the desired data.
 * @param id The ID code of the UART packet (0x00 for requesting, 0x10 for sending).
 * @param data_ptr Pointer to the allocated packet data.
 * @param size The size of the packet.
 */
void UartFillPacketFromStruct(struct s_UartPacket *packet_ptr, int id, char *data_ptr, int size);

/**
 * @brief Fills a data structure with the data from a UART packet.
 *
 * @param packet_ptr Pointer to the data structure to fill with the packet data.
 * @param data_ptr Pointer to the allocated data.
 * @param size Size of the packet.
 * @returns 0 if the packet is invalid, 1 if the packet is valid.
 */
int UartFillStructFromPacket(struct s_UartPacket *packet_ptr, char *data_ptr, int size);

/**

 * @brief Transmits a data packet via UART.
 *
 * This function transmits a data packet via UART.
 *
 * @param packet Pointer to the filled data packet.
 * @param uart The UART designator name.
 */
void UartTransmitPacket(const uart_index_t uart, struct s_UartPacket *packet);

/**
 * @brief Requests and processes test data from the control board via UART.
 * @param packet_ptr Pointer to the data structure to fill with the packet data.
 * @param uart The UART designator name.
 */
void requestTestData(const uart_index_t uart, struct s_UartPacket *packet_ptr);

/**
 * @brief Requests and processes energy data from the control board via UART.
 * @param packet_ptr Pointer to the data structure to fill with the packet data.
 * @param uart The UART designator name.
 */
void requestEnergyData(const uart_index_t uart, struct s_UartPacket *packet_ptr);

/**
 * @brief Requests and processes position data from the control board via UART.
 * @param packet_ptr Pointer to the data structure to fill with the packet data.
 * @param uart The UART designator name.
 */
void requestPositionData(const uart_index_t uart, struct s_UartPacket *packet_ptr);

/**
 * @brief Requests throttle data from the specified UART interface.
 *
 * This function sends a request to retrieve throttle data via the given UART interface.
 * The retrieved data will be stored in the provided UART packet structure.
 *
 * @param uart The index of the UART interface to use for the request.
 * @param packet_ptr Pointer to the structure where the retrieved UART packet data will be stored.
 */
void requestThrottleData(const uart_index_t uart, struct s_UartPacket *packet_ptr);

/**
 * @brief Sends a throttle setpoint (between 0.0 and 1.0) to the control board.
 * @param packet_ptr Pointer to the data structure to fill with the packet data.
 * @param uart The UART designator name.
 * @param throttleValue The desired throttle setpoint.
 * @warning Any value outside [0.0, 1.0] will be rejected with an error.
 */
void sendThrottleSetpoint(const uart_index_t uart, struct s_UartPacket *packet_ptr, float throttleValue);

/** @}*/
/*=======================================================   Pulsecounter  ==============================================================*/

/**
  * @brief Initializes the pulse counter.
  * @param pulse_counter The pulse counter index.
  * @param edges The number of edges to trigger on. 0 = none, 1 = rising, 2 = falling, 3 = both.
  * @param filter_length The length of the glitch filter (Use 15).
 */
void initPulseCounter(const pulsecounter_index_t pulse_counter, int edges, int filter_length);

// /*=======================================================   IR Receiver  ==============================================================*/

/* these parameter define the duration of transmitted bits in the IR communication protocol */
#define MARGIN 0.0002
#define MARKZERO 0.0006
#define MARKONE 0.0012
#define STARTBIT 0.0024
#define GAP 0.0006
#define IRpin IO_RBPI08 // IR receiver pin

enum Order {
  IDLE = 0,
  BITS = 1,
};

/* @brief data structure to setup IR decoder*/
typedef struct {
  io_t pin;
  pulsecounter_index_t pulsecounter;
  bool gap;
  uint32_t value;
  uint32_t nbits;
  gpio_direction_t dir;
  enum Order state;
  gpio_level_t prev_level;
} IRDecoder;

/**
 * @brief Creates an IRDecoder instance.
 *
 * This function initializes and returns a pointer to an IRDecoder object.
 *
 * @param pin The IO pin to which the IR receiver is connected.
 * @param pulsecounter The index of the pulse counter to be used.
 * @return A pointer to the created IRDecoder instance.
 */
IRDecoder *IRDecoderCreate(io_t pin, pulsecounter_index_t pulsecounter);

/**
 * @brief Destroys the given IRDecoder instance and frees associated resources.
 * 
 * This function should be called to properly dispose of an IRDecoder instance
 * when it is no longer needed. It ensures that all allocated memory and resources
 * are released to prevent memory leaks.
 * 
 * @param decoder A pointer to the IRDecoder instance to be destroyed.
 */
void IRDecoderDestroy(IRDecoder *decoder);

/**
 * @brief Processes a pulse for the IR decoder and updates the value.
 *
 * This function takes a pulse duration and processes it using the given IR decoder.
 * If the pulse is valid, it updates the provided value with the decoded result.
 *
 * @param decoder A pointer to the IRDecoder instance.
 * @param pulse The duration of the pulse to be processed.
 * @param value A pointer to a uint32_t variable where the decoded value will be stored.
 * @return true if the pulse was successfully processed and the value was updated, false otherwise.
 */
bool IRDecoderPulse(IRDecoder *decoder, double pulse, uint32_t *value);


/**
 * @brief Receives an IR message and decodes it.
 * 
 * This function receives an IR message and decodes it using the given IR decoder.
 * If a complete message is received, it updates the provided value with the decoded result.
 *
 * @param decoder A pointer to the IRDecoder instance.
 * @param value A pointer to a uint32_t variable where the decoded value will be stored.
 */
bool IRDecoderReceive(IRDecoder *decoder, uint32_t *value);

// /**
//  * @brief Initializes the IR sensor on the specified pin and associates it with a pulse counter.
//  * 
//  * @param pin The I/O pin to which the IR sensor is connected.
//  * @param pulse_counter The index of the pulse counter to be used for counting pulses from the IR sensor.
//  */
// void IRInit(const io_t pin, const pulsecounter_index_t pulse_counter);


// /**
//  * @brief Processes an IR pulse received by the decoder.
//  * 
//  * @param decoder Pointer to the IRDecoder structure.
//  * @param t The time at which the pulse was received.
//  * @param value Pointer to a variable where the decoded value will be stored.
//  * @param pulse_counter The index of the pulse counter to be used.
//  * @return int Status code indicating success or failure of the operation. 0 = success, 1 = failure.
//  */
// int IRReceivedPulse(IRDecoder *decoder, double t, uint32_t *value, const pulsecounter_index_t pulse_counter);

// /**
//     @brief receives the end of the IR transmission
//     @returns TRUE if transmission is over
//     @param *decoder points to the decoder data structure IRDecoder
//     @param *value points to the value of the received message
// */
// gboolean IRrecv(IRDecoder *decoder, uint32_t *value);

// /**
//     @brief sets up the IR decoder
//     @returns a decoder structure
//     @param the pin name for the IR receiver (IO_RBPI08 on PYNQ)
// */
// IRDecoder *IRDecoderCreate(io_t pin);

// /** 
//     @brief deletes the IR decoder
//     @param *decoder points to the decoder data structure IRDecoder
// */
// void IRDecoderDestroy(IRDecoder *decoder);

// /** 
//     @brief decodes the IR message
//     @param *decoder points to the decoder data structure IRDecoder
// */
// void IRDecode(IRDecoder *decoder);

// /** 
//     @brief decodes the IR message
//     @param *o points to the decoder data structure IRDecoder
//     @param pulse is the pulse duration
//     @param *value points to the value of the received message
// */
// void IRDecodePulse(IRDecoder *o, double pulse, uint32_t *value);


// /*===================================================   nRF24 Radio Transceiver  ========================================================*/

/* @brief handles the interrupt for the nRF24 radio transceiver */
void handleRadio(uint8_t type, uint8_t num, uint8_t *buf, uint8_t len);

/* @brief initializes the nRF24 radio transceiver */
void initRadio(void);

/* 
  @brief sends a byte via the nRF24 radio
  @param address[] is the receiver radio address
  @param car is the car number
*/
void radioSend(uint8_t address[], uint8_t car);

/* @brief deactivates the nRF24 radio */
void radioDestroy(void);

// /*===================================================   Pulse Counter  =================================================================*/

#define hallPin IO_RBPI32 // hall sensor pin

void pulseCounterInit(void);
double current_speed(uint32_t *count, uint32_t *previous_count, uint32_t *timestamp, uint32_t *previous_timestamp);

#endif // CARLIB_H