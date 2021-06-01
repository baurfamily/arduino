//#include <SimpleDHT.h>
#include <LiquidCrystal.h>
#include <SPI.h>
#include <printf.h>
#include <RF24_config.h>
#include <RF24.h>

//#define pinDHT11 10

#define CE_PIN   10
#define CSN_PIN  9

#define LCD_DS 8
#define LCD_EN 7
#define LCD_D4 5
#define LCD_D5 4
#define LCD_D6 3
#define LCD_D7 2

#define DEGREE_SYM 0xDF

//SimpleDHT11 dht11(pinDHT11);

enum TX_RX {
  RX = 0,
  TX = 1
};

// Creates an LCD object. Parameters: (rs, enable, d4, d5, d6, d7)
LiquidCrystal lcd(LCD_DS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);
RF24 radio(10, 9); // using pin 7 for the CE pin, and pin 8 for the CSN pin
//uint8_t address[][6] = {"1Node", "2Node"};
const uint64_t pipe = 0xE8E8F0F0E1LL; // Define the transmit pipe

// to use different addresses on a pair of radios, we need a variable to
// uniquely identify which address this radio will use to transmit
//bool radioNumber; // 0 uses address[0] to transmit, 1 uses address[1] to transmit

// Used to control whether this node is sending or receiving
//bool role = RX; // true = TX node, false = RX node

typedef struct sensor_t {
  byte temp_c = 0;
  byte temp_f = 0;
  byte humidity = 0;
  byte sequence = 0;
} SENSOR;

SENSOR my_sensor;

void setup() {
  Serial.begin(115200);
//  while (!Serial) {
//    // some boards need to wait to ensure access to serial over USB
//  }

  SPI.begin();

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  // Clears the LCD screen
  lcd.clear();

  delay(1000);
  
  if (!radio.begin()) {
    Serial.println(F("radio hardware is not responding!!"));
    while (1) {} // hold in infinite loop
  }

  radio.setPALevel(RF24_PA_MIN);  // RF24_PA_MAX is default.
  radio.setDataRate(RF24_1MBPS);
  radio.setPayloadSize(sizeof(SENSOR));     // default value is the maximum 32 bytes

  // set the RX address of the TX node into a RX pipe
//  radio.openReadingPipe(1, address[!radioNumber]); // using pipe 1 
  radio.openReadingPipe(1, pipe);
  radio.startListening();  // put radio in RX mode

}

//bool readSensor() {
//  byte data[5] = {0};
//  int err = SimpleDHTErrSuccess;
//  
//  Serial.println("=================================");
//  Serial.println("Sample DHT11...");
//
//  if ((err = dht11.read(&temp_c, &humidity, data)) != SimpleDHTErrSuccess) {
//    Serial.print("Read DHT11 failed, err="); Serial.print(SimpleDHTErrCode(err));
//    Serial.print(","); Serial.println(SimpleDHTErrDuration(err)); delay(1000);
//    return false;
//  }
//
//  Serial.print("Sample RAW Bits: ");
//  for (int i = 0; i < 5; i++) {
//    Serial.print((int)data[i]);
//    if (i > 0 && ((i + 1) % 4) == 0) {
//      Serial.print(' ');
//    }
//  }
//  Serial.println("");
//
//  temp_f = ((temp_c * 9) / 5) + 32;
//
//  return true;
//  
//}

void loop() {
  char s[4] = {0};

//  if(!readSensor())
//    return;

//  struct sensor_t myTempData;

  if ( radio.available() ) {
    Serial.println("Data Received");
    uint8_t bytes = radio.getPayloadSize(); // get the size of the payload
    Serial.print("Payload Size: ");Serial.println(bytes);
    radio.read(&my_sensor, sizeof(my_sensor));            // fetch payload from FIFO

    Serial.print("Sequence: "); Serial.println(my_sensor.sequence);
    Serial.print("Sample OK: ");
    Serial.print(my_sensor.temp_c); Serial.print(" *C, "); 
    Serial.print(my_sensor.temp_f); Serial.print(" *F, ");
    Serial.print(my_sensor.humidity); Serial.println(" %");
  } else {
    Serial.println("No Data");    
  }
              
  lcd.clear();
  lcd.print("Temp: ");
  snprintf(s, 4, "%d", my_sensor.temp_c);
  lcd.print(s);
  lcd.print(char(DEGREE_SYM));
  lcd.print("C ");
  snprintf(s, 4, "%d", my_sensor.temp_f);
  lcd.print(s);
  lcd.print(char(DEGREE_SYM));
  lcd.print("F");

  lcd.setCursor(0, 1);
  lcd.print("Humidity: ");
  snprintf(s, 4, "%d", my_sensor.humidity);
  lcd.print(s);
  lcd.print("%");

  Serial.println();

  delay(2000);
}
