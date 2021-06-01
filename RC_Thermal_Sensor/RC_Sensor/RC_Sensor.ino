#include <SimpleDHT.h>
#include <SPI.h>
#include <printf.h>
#include <nRF24L01.h>
#include <RF24_config.h>
#include <RF24.h>

#define pinDHT11 4

SimpleDHT11 dht11(pinDHT11);

// instantiate an object for the nRF24L01 transceiver
RF24 radio(10, 9); // using pin 7 for the CE pin, and pin 8 for the CSN pin

// Let these addresses be used for the pair
const uint64_t pipe = 0xE8E8F0F0E1LL; // Define the transmit pipe

byte temp_c = 0;
byte temp_f = 0;
byte humidity = 0;
byte sequence = 0;

typedef struct sensor_t {
  byte temp_c = 0;
  byte temp_f = 0;
  byte humidity = 0;
  byte sequence = 0;
} SENSOR;

SENSOR sense;

void setup() {
  // put your setup code here, to run once:
  pinMode(pinDHT11, INPUT);

  Serial.begin(115200);
//  while (!Serial) {
//    // some boards need to wait to ensure access to serial over USB
//  }

  SPI.begin();

  if (!radio.begin()) {
    Serial.println(F("radio hardware is not responding!!"));
    while (1) {} // hold in infinite loop if radio is not ready
  }
  
  radio.setPALevel(RF24_PA_MIN);         // RF24_PA_LOW / RF24_PA_MAX is default.
  radio.setDataRate(RF24_1MBPS);
  radio.setPayloadSize(sizeof(SENSOR));  // default value is the maximum 32 bytes

  // set the TX address of the RX node into the TX pipe
  radio.openWritingPipe(pipe);
  radio.stopListening();  // put radio in TX mode
  
}

bool readSensor() {
  byte data[5] = {0};
  int err = SimpleDHTErrSuccess;
  
  Serial.println("=================================");
  Serial.println("Sample DHT11...");

  if ((err = dht11.read(&temp_c, &humidity, data)) != SimpleDHTErrSuccess) {
    Serial.print("Read DHT11 failed, err="); Serial.print(SimpleDHTErrCode(err));
    Serial.print(","); Serial.println(SimpleDHTErrDuration(err)); delay(1000);
    return false;
  }

  Serial.print("Sample RAW Bits: ");
  for (int i = 0; i < 5; i++) {
    Serial.print((int)data[i]);
    if (i > 0 && ((i + 1) % 4) == 0) {
      Serial.print(' ');
    }
  }
  Serial.println("");

  temp_f = ((temp_c * 9) / 5) + 32;

  return true;
  
}

void loop() {

  if(!readSensor())
    return;

  Serial.print("Sample OK: ");
  Serial.print(temp_c); Serial.print(" *C, "); 
  Serial.print(temp_f); Serial.print(" *F, ");
  Serial.print(humidity); Serial.println(" %");

  sense.temp_c = temp_c;
  sense.temp_f = temp_f;
  sense.humidity = humidity;
  sense.sequence++;

  // This is a TX, so will send the thermal sensor data here
  bool rslt = radio.write(&sense, sizeof(SENSOR));
  if (rslt) {
    Serial.println("Transmit Success!");
  } else {
    Serial.println("Transmit Failed!");   
  } 
  
  delay(5000);
}
