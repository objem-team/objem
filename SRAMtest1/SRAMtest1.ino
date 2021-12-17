/*
   Used the following components and wire routing:
   (1) Arduino Uno
   (2) Microchip 23LC1024
   (3) 10K Resistor
 */
#include <SPI.h>
 
//SRAM opcodes
#define RDSR        5
#define WRSR        1
#define READ        3
#define WRITE       2
  
//Byte transfer functions
uint8_t Spi23LC1024Read8(uint32_t address) {
  uint8_t read_byte;
 
  PORTB &= ~(1<<PORTB2);        //set SPI_SS low
  SPI.transfer(READ);
  SPI.transfer((uint8_t)(address >> 16) & 0xff);
  SPI.transfer((uint8_t)(address >> 8) & 0xff);
  SPI.transfer((uint8_t)address);
  read_byte = SPI.transfer(0x00);
  PORTB |= (1<<PORTB2);         //set SPI_SS high
  return read_byte;
}
  
void Spi23LC1024Write8(uint32_t address, uint8_t data_byte) {
  PORTB &= ~(1<<PORTB2);        //set SPI_SS low
  SPI.transfer(WRITE);
  SPI.transfer((uint8_t)(address >> 16) & 0xff);
  SPI.transfer((uint8_t)(address >> 8) & 0xff);
  SPI.transfer((uint8_t)address);
  SPI.transfer(data_byte);
  PORTB |= (1<<PORTB2);         //set SPI_SS high
}
  
void setup(void) {
  uint32_t i;
  uint8_t value;
 
  Serial.begin(9600);
  SPI.begin();
 
  for (i=0; i<32; i++) {
    Spi23LC1024Write8(i, (uint8_t)i);
    value = Spi23LC1024Read8(i);
    Serial.println((uint16_t)value);
  }
}
 
void loop() {
}
