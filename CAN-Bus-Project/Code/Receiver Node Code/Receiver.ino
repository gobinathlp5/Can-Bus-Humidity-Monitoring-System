#include <SPI.h>
#include <mcp_can.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Pin definitions
#define CAN_CS 10
#define CAN_INT 2
#define RESET_BTN 7

// Objects
MCP_CAN CAN(CAN_CS);
LiquidCrystal_I2C lcd(0x27, 16, 2);

// LCD reset function
void lcdReset() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("CAN Ready");
  lcd.setCursor(0,1);
  lcd.print("Waiting Msg...");
}

void setup() {
  Serial.begin(115200);

  // Initialize LCD
  lcd.begin(16, 2);
  lcd.backlight();
  lcdReset();

  pinMode(CAN_INT, INPUT);
  pinMode(RESET_BTN, INPUT_PULLUP);

  // Initialize CAN bus
  while (CAN_OK != CAN.begin(MCP_ANY, CAN_250KBPS, MCP_8MHZ)) {
    Serial.println("CAN init fail, retry...");
    delay(500);
  }
  CAN.setMode(MCP_NORMAL);
  Serial.println("CAN RX ready");
}

void loop() {
  // Reset LCD if button pressed
  if (digitalRead(RESET_BTN) == LOW) {
    lcdReset();
    delay(200);
  }

  // Check for CAN message
  if (CAN.checkReceive() == CAN_MSGAVAIL) {
    long unsigned int rxId;
    unsigned char len = 0;
    unsigned char buf[8];

    if (CAN.readMsgBuf(&rxId, &len, buf) == CAN_OK) {
      if (len >= 4) { // temp + humidity
        int temp10 = word(buf[0], buf[1]);
        int hum10  = word(buf[2], buf[3]);

        float temp = temp10 / 10.0;
        float hum  = hum10  / 10.0;

        Serial.print("RX ID 0x"); Serial.print(rxId, HEX);
        Serial.print(" Temp: "); Serial.print(temp);
        Serial.print("C Hum: "); Serial.println(hum);

        // Display on LCD
        lcd.setCursor(0,0);
        lcd.print("T:"); lcd.print(temp); lcd.print("C   ");
        lcd.setCursor(0,1);
        lcd.print("H:"); lcd.print(hum); lcd.print("%   ");
      }
    }
  }
}
