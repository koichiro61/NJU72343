/*
 * NJU72343 (Audio volume control IC) 
 * Evaluatoin sketch No.1
 * 
 * Get the contorol value input from keynboard and
 * set the value to the register
 * 
 * volume control value value for gain g (dB) is 
 * 0x40 - (g x 2)
 * when g = 0dB, the value is 0x40
 * when g = -40dB, the value is 0x40 - (-40 x 2) = 0x90
 * 
 */

#include <XantoI2C.h>

const uint8_t PIN_SCL = 19;
const uint8_t PIN_SDA = 18;
const uint8_t NJU72343 = 0x82; // ADR PIN to V+
const uint8_t REGADDRS = 0x02; // Cch Volumue

XantoI2C i2c(PIN_SCL, PIN_SDA, 4);

bool NJU72343_write(uint8_t device_address, uint8_t register_address, uint8_t value) {
  i2c.start();
  i2c.writeByte(device_address);
  if (i2c.readNack()) {
    Serial.println("Error wrting device address");
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
  String line;    // 受信文字列
  char line_buf[3];
  int line_len;   // 受信文字列の長さ
  uint8_t vol_value;
  char sprintf_buf[16];

  line = Serial.readStringUntil('\n');
  // 1行の文字数を取得する
  line_len = line.length();

  if (line_len != 2) {
    // nop
  } else {
    // 文字列を整数に変換する
    line.toCharArray(line_buf, 3);
    sscanf(line_buf, "%02x", &vol_value);
    sprintf(sprintf_buf, "value: %02x", vol_value);
    Serial.println(sprintf_buf);

    NJU72343_write(NJU72343, 0x02, vol_value);
  }
}
