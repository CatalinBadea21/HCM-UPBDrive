#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <mcp_can.h>
#include <SPI.h>

// LCD config: address 0x27, 16 chars, 2 lines
LiquidCrystal_I2C lcd(0x27, 16, 2);

// CAN config
const int SPI_CS_PIN = 10;
MCP_CAN CAN(SPI_CS_PIN);

// Function to convert mode to string
String modeToString(uint8_t mode) {
  switch (mode) {
    case 0: return "Freewheel";
    case 2: return "Current";
    case 3: return "Speed";
    case 8: return "Brake";
    default: return "Unknown";
  }
}

void setup() {
  Serial.begin(115200);

  // LCD init
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Initializing...");

  // CAN init
  if (CAN.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) == CAN_OK) {
    lcd.setCursor(0, 1);
    lcd.print("CAN Init OK");
  } else {
    lcd.setCursor(0, 1);
    lcd.print("CAN Init Fail");
    while (1);
  }
  delay(1000);
  lcd.clear();
  CAN.setMode(MCP_NORMAL);  // Normal mode to receive
}

void loop() {
  if (CAN_MSGAVAIL == CAN.checkReceive()) {
    long unsigned int rxId;
    unsigned char len = 0;
    unsigned char buf[8];

    CAN.readMsgBuf(&rxId, &len, buf);

    if (rxId == 0x0CF && len == 4) {
      uint8_t mode = buf[1];

      // Extract 15-bit duty cycle: buf[2] = MSB, buf[3] = LSB
      uint16_t rawDuty = ((buf[2] << 8) | buf[3]) & 0x7FFF;
      float percentage = (rawDuty / 32767.0) * 100.0;

      // Update display
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Mode: ");
      lcd.print(modeToString(mode));

      lcd.setCursor(0, 1);
      lcd.print("Percentage: ");
      lcd.print(percentage, 1);
      lcd.print("%");

      // Also log to Serial
      Serial.print("Mode: ");
      Serial.print(mode);
      Serial.print(" (");
      Serial.print(modeToString(mode));
      Serial.print("), Duty: ");
      Serial.print(rawDuty);
      Serial.print(" -> ");
      Serial.print(percentage);
      Serial.println("%");
    }
  }
}