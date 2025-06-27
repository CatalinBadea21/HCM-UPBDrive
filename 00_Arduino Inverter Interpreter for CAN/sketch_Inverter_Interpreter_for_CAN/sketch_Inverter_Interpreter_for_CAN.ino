#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <mcp_can.h>
#include <SPI.h>

#define LCD_ADDRESS 0x27
#define INVERTER_RX_CAN_ID 0x0CF
#define MAX_CAN_NOREAD 20

LiquidCrystal_I2C lcd(LCD_ADDRESS, 16, 2);

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

uint8_t can_failed_attempts_ctr;

void setup() {
  Serial.begin(115200);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Initializing...");

  // CAN init
  if (CAN.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) == CAN_OK) {
    lcd.setCursor(0, 1);
    lcd.print("CAN Init OK");
  }
  else {
    lcd.setCursor(0, 1);
    lcd.print("CAN Init Fail");
    while (1);
  }

  delay(1000);
  lcd.clear();
  CAN.setMode(MCP_NORMAL);
}

void loop() {
  if (CAN_MSGAVAIL == CAN.checkReceive()) {
    long unsigned int rxId;
    unsigned char DLC = 0;
    unsigned char buf[8];

    CAN.readMsgBuf(&rxId, &DLC, buf);

    if (rxId == INVERTER_RX_CAN_ID && DLC == 4) {
      uint8_t hybrid_mode = buf[1];
      can_failed_attempts_ctr = 0;

      // Extract 15-bit duty cycle: buf[2] = MSB, buf[3] = LSB
      uint16_t rawDuty = ((buf[2] << 8) | buf[3]) & 0x7FFF;
      float percentage = (rawDuty / 32767.0) * 100.0;

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Mode: ");
      lcd.print(modeToString(hybrid_mode));

      lcd.setCursor(0, 1);
      lcd.print("Percentage: ");
      lcd.print(percentage, 0);
      lcd.print("%");

      Serial.print(buf[2]); Serial.print(", "); Serial.print(buf[3]); Serial.print(", percentage: "); Serial.print(percentage); Serial.print(", mode: "); Serial.println(hybrid_mode);
    }
    else
      can_failed_attempts_ctr++; 
  }

  if (can_failed_attempts_ctr >= MAX_CAN_NOREAD){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(" CAN messaging");
    lcd.setCursor(0, 1);
    lcd.print("   has FAILED");
  }

  delay (10);
}