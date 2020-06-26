#include "DHTSensor.h"
#include "Constants.h"

dht DHT;

void initializeDHTSensor() {

}



void readData() {
  // READ DATA
  Serial.print("DHT22, \t");

  //    uint32_t start = micros();
  int chk = DHT.read22(DHT22_PIN);
  //    uint32_t stop = micros();

  stat.total++;
  switch (chk)
  {
    case DHTLIB_OK:
      stat.ok++;
#ifdef DEBUG
      Serial.print("OK,\t");
#endif
      break;
    case DHTLIB_ERROR_CHECKSUM:
      stat.crc_error++;
#ifdef DEBUG
      Serial.print("Checksum error,\t");
#endif
      break;
    case DHTLIB_ERROR_TIMEOUT:
      stat.time_out++;
#ifdef DEBUG
      Serial.print("Time out error,\t");
#endif
      break;
    case DHTLIB_ERROR_CONNECT:
      stat.connect++;
#ifdef DEBUG
      Serial.print("Connect error,\t");
#endif
      break;
    case DHTLIB_ERROR_ACK_L:
      stat.ack_l++;
#ifdef DEBUG
      Serial.print("Ack Low error,\t");
#endif
      break;
    case DHTLIB_ERROR_ACK_H:
      stat.ack_h++;
#ifdef DEBUG
      Serial.print("Ack High error,\t");
#endif
      break;
    default:
      stat.unknown++;
#ifdef DEBUG
      Serial.print("Unknown error,\t");
#endif
      break;
  }
}


void displayData() {
  Serial.print(DHT.humidity, 1);
  Serial.print(",\t");
  Serial.print(DHT.temperature, 1);
  //    Serial.print(",\t");
  //    Serial.print(stop - start);
  Serial.println();

  if (stat.total % 20 == 0)
  {
    Serial.println("\nTOT\tOK\tCRC\tTO\tCON\tACK_L\tACK_H\tUNK");
    Serial.print(stat.total);
    Serial.print("\t");
    Serial.print(stat.ok);
    Serial.print("\t");
    Serial.print(stat.crc_error);
    Serial.print("\t");
    Serial.print(stat.time_out);
    Serial.print("\t");
    Serial.print(stat.connect);
    Serial.print("\t");
    Serial.print(stat.ack_l);
    Serial.print("\t");
    Serial.print(stat.ack_h);
    Serial.print("\t");
    Serial.print(stat.unknown);
    Serial.println("\n");
  }
}
