#include <SPI.h>
#include <mcp_can.h>
#include <DHT.h>

#define CAN_CS 10
#define DHT_PIN 4
#define DHTTYPE DHT11  // or DHT22

MCP_CAN CAN(CAN_CS);
DHT dht(DHT_PIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();
   
  // Initialize CAN bus
  while (CAN_OK != CAN.begin(MCP_ANY, CAN_250KBPS, MCP_8MHZ)) {
    Serial.println("CAN init fail, retry...");
    delay(500);
  }
  CAN.setMode(MCP_NORMAL);
  Serial.println("CAN TX ready");
}

void loop() {
  float temp = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temp) || isnan(humidity)) {
    Serial.println("Failed to read DHT!");
    delay(2000);
    return;
  }

  // Convert to 0.1 precision
  int temp10 = (int)(temp * 10);     // e.g., 31.7 -> 317
  int hum10  = (int)(humidity * 10); // e.g., 63.2 -> 632

  // Prepare 4-byte CAN message
  byte data[4];
  data[0] = highByte(temp10);
  data[1] = lowByte(temp10);
  data[2] = highByte(hum10);
  data[3] = lowByte(hum10);

  byte status = CAN.sendMsgBuf(0x123, 0, 4, data);
  if (status == CAN_OK) {
    Serial.print("TX Temp: "); Serial.print(temp); 
    Serial.print("C Hum: "); Serial.println(humidity);
  } else {
    Serial.println("TX failed");
  }

  delay(2000); // DHT11 safe interval
}
