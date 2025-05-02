#include <SPI.h>

#define CS_PIN 10  // ADS1243-HT CS pin
#define RTD_TYPE_PIN 9  // J3 RTD_TYPE pin

void setup() {
  pinMode(CS_PIN, OUTPUT);
  digitalWrite(CS_PIN, HIGH);
  SPI.begin();
  SPI.setDataMode(SPI_MODE1);
  SPI.setClockDivider(SPI_CLOCK_DIV16);
  pinMode(RTD_TYPE_PIN, INPUT);  // Configure RTD_TYPE as input
  Serial.begin(9600);
}

uint32_t read_adc() {
  digitalWrite(CS_PIN, LOW);
  delayMicroseconds(10);
  SPI.transfer(0x08);  // RDATA command
  delayMicroseconds(10);
  uint32_t raw = 0;
  for (int i = 0; i < 3; i++) {
    raw = (raw << 8) | SPI.transfer(0xFF);
  }
  digitalWrite(CS_PIN, HIGH);
  return raw & 0xFFFFFF;
}

float rtd_to_temp(uint32_t raw, uint8_t is_pt100) {
  float R0 = is_pt100 ? 100.0 : 1000.0;
  float Vref = 2.0;
  float adc_max = 16777216.0;
  float V_rtd = (raw * Vref) / adc_max;
  float R_rtd = V_rtd / 0.0002;
  float delta = (R_rtd - R0) / R0;
  float temp = delta / 0.00385;
  return temp;
}

uint8_t get_rtd_type() {
  return digitalRead(RTD_TYPE_PIN);
}

void loop() {
  uint32_t raw = read_adc();
  uint8_t is_pt100 = get_rtd_type();
  float temp = rtd_to_temp(raw, is_pt100);
  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.println(" °C");
  delay(1000);
}
