#include <Wire.h>
 
#define SDA  4
#define SCL  5
 
void setup() {
 
  Wire.begin();
  Serial.begin(9600);
  while (!Serial);  
 
  Serial.println("     0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F");
  for (byte n = 0; n <= 0x7F; n++) {  // I2Cアドレスを指定
    adrCheck(n);                      // デバイス有無を調べ結果を出力
  }
  Serial.println();
}
 
void loop() {
 
}
 
void adrCheck(byte adr) {
  byte    dummy;
 
  if ((adr & 0x0F) == 0) {
    print_hex2chr(adr);
    Serial.print(":");
  }
  Serial.print(" ");
  
  if (adr < 8 || adr > 0x77) {
    Serial.print("xx");
  } else {
    Wire.beginTransmission(adr);
    Wire.write(&dummy, 0);
    if (Wire.endTransmission() == 0) {
      print_hex2chr(adr);
    } else {
      Serial.print("--");
    }
  }
  if ( (adr & 0x0F) == 0x0F) {
    Serial.println();
  }
}
  
void print_hex2chr(byte x) {
  Serial.print((x >> 4), HEX);
  Serial.print((x & 0x0F), HEX);
}
