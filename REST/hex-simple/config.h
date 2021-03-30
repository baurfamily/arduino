#ifndef CONFIG_H
#define CONFIG_H

#include <EEPROM.h>
 // Handle the Application configuration parameters stored in EEPROM
 // Access to these parameters is through the object properties only

typedef struct pattern {
  uint8_t hue;
  uint16_t boundry;
  uint8_t  increment;
  uint8_t  boundry_diff;
  uint16_t inc_speed;
  uint8_t value;
} pattern;

typedef struct configData_t
{
  uint8_t signature[2];
  uint8_t version;
  // application config data starts below
  pattern currentPattern;
};

void printPatternToSerial(pattern p)
{
  Serial.print(p.hue);
  Serial.print(" / ");
  Serial.print(p.boundry);
  Serial.print(" / ");
  Serial.print(p.increment);
  Serial.print(" / ");
  Serial.print(p.boundry_diff);
  Serial.print(" / ");
  Serial.print(p.inc_speed);
  Serial.print(" / ");
  Serial.print(p.value);
  Serial.println("");
}
 
class AppConfig
{
public:
  inline pattern getPattern() { return(_D.currentPattern); };
  inline void setPattern(pattern n) {
    _D.currentPattern = n;
    dirty = true;
    lastSet = millis();
  };

  void begin()
  { 
    if (!configLoad())
    {
      Serial.println("Failed to load config, returning defaults");
      configDefault();
      dirty = true;
      lastSet = 0;
      configSave();
    }
  };
 
  void configDefault(void)
  {
    _D.signature[0] = EEPROM_SIG[0];
    _D.signature[1] = EEPROM_SIG[1];
    _D.version = CONFIG_VERSION;
    _D.currentPattern = DEFAULT_PATTERN; 
  }
  
  bool configLoad(void)
  {
    EEPROM.begin(sizeof(configData_t));
    EEPROM.get(EEPROM_ADDR, _D);
    EEPROM.end();
    if (_D.signature[0] != EEPROM_SIG[0] && _D.signature[1] != EEPROM_SIG[1])
    {
      Serial.println("Failed to match config signature.");  
      return(false);
    }
    
   // handle any version adjustments here
   if (_D.version != CONFIG_VERSION)
   {
     // do something here to update data...
     // ... don't need to worry about this until a version change
   }
 
   // update version number to current
   _D.version = CONFIG_VERSION;
 
   return(true);
 }
  
 bool configSave(void)
 {
  // nothing to do
  if (!dirty) return true;

  // wait a little while...
  if (lastSet + 10000 > millis()) return false;

  Serial.println("saving config");

  EEPROM.begin(sizeof(configData_t));
  EEPROM.put(EEPROM_ADDR, _D);
  EEPROM.end();
  
  dirty = false;
  
  return(true);
 }
 
private:
  const pattern DEFAULT_PATTERN = { .hue=0, .boundry=8, .increment=1, .boundry_diff=2, .inc_speed=1000, .value=255 };
  
  const uint16_t EEPROM_ADDR = 0;
  const uint8_t EEPROM_SIG[2] = { 0xee, 0x11 };
  const uint8_t CONFIG_VERSION = 0;
  configData_t _D;
  
  bool dirty = false;
  int lastSet = millis();
 };

 #endif
