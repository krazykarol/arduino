#ifndef _DHTSensor_h_
#define _DHTSensor_h_

#include <dht.h>

#define DHT22_PIN A3

struct
{
  uint32_t total;
  uint32_t ok;
  uint32_t crc_error;
  uint32_t time_out;
  uint32_t connect;
  uint32_t ack_l;
  uint32_t ack_h;
  uint32_t unknown;
} stat = { 0, 0, 0, 0, 0, 0, 0, 0};

void initializeSensor();
void readData();
void displayData();

#endif
