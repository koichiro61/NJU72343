/*

 NJU72343 (audio volume control IC) evaluation No.2

 Changing the gain automatically and continuously
 between 0dB and -40dB
 
*/

#include <XantoI2C.h>

const uint8_t PIN_SCL = 19;
const uint8_t PIN_SDA = 18;
const uint8_t NJU72343 = 0x82; // ADR PIN to V+
const uint8_t REGADDRS = 0x02; // Cch Volumue

XantoI2C i2c(PIN_SCL, PIN_SDA, 4);  // 4usec delay. See XantoI2C.h

bool NJU72343_write(uint8_t device_address, uint8_t register_address, uint8_t value) {
  i2c.start();
  i2c.writeByte(device_address);
  if (i2c.readNack()) {
    Serial.println("Error wrting device ad1dress");
    return false;
  }
  //delay(10);
  i2c.writeByte(register_address);
  if (i2c.readNack()) {
    Serial.println("Error wrting regsiter address");
    return false;
  }
  //delay(10);
  i2c.writeByte(value);
  if (i2c.readNack()) {
    Serial.println("Error wrting the value to the register");
    return false;
  }
  i2c.stop();
  return true;
}

void setup() {
  Serial.begin(115200);
  Serial.println("Reset");
}

void loop() {
  for (uint8_t vol = 0x40; vol < 0x90; vol++) { // to set gain g(db), value is 0x40 - g x 2
    NJU72343_write(NJU72343, 0x02, vol);        // 0x40 for 0 dB, 0x90 for -40dB 
    delay(100);                                 // gain is changed by 0.5dB step
  }
  for (uint8_t vol = 0x90; vol > 0x40; vol--) {
    NJU72343_write(NJU72343, 0x02, vol);
    delay(100);
  }
}
