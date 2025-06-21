#include <mcp_can.h>
#include <SPI.h>

#define CAN_CS_PIN 10       /* MCP2515 CS pin */
#define BRAKE_PIN 8         /* Brake button input */
#define SWITCH_PIN 9        /* Pot / Pedal switch */
#define RPM_POT A0          /* RPM potentiometer input */
#define TPS_POT A2          /* TPS potentiometer input */
#define PEDAL_POT A4        /* TPS potentiometer input */
#define VSS_VALUE 50        /* Vehicle speed sensor value (byte) */
#define GEAR_VALUE 1        /* Current gear (byte) */
#define PEDAL_MIN_VALUE 180 /* Pedal minimum read value (8 bits) */
#define PEDAL_MAX_VALUE 860 /* Pedal maximum read value (8 bits) */
#define PEDAL_WINDOW 100    /* Window for fail-safe */
#define CAN_ID 0x3E8        /* EMU sending CAN ID */

MCP_CAN CAN(CAN_CS_PIN);

void setup()
{
  Serial.begin(115200);

  pinMode(BRAKE_PIN, INPUT_PULLUP);
  pinMode(SWITCH_PIN, INPUT_PULLUP);

  if (CAN.begin(MCP_STDEXT, CAN_500KBPS, MCP_8MHZ) == CAN_OK)
    Serial.println("CAN BUS Shield init ok!");
  else
    while (1)
    {
      Serial.println("CAN BUS Shield init fail - reset the board!");
      delay(500);
    }
  
  CAN.setMode(MCP_NORMAL);
}

void loop()
{
  int rawRpm = analogRead(RPM_POT);
  int rpm = map(rawRpm, 0, 1023, 0, 14000);
  uint8_t tps, rpmScaled = rpm / 100;

  bool switchPressed = digitalRead(SWITCH_PIN);
  int rawTps = analogRead(PEDAL_POT);

  if (switchPressed) /* Switch between Pedal and Potentiometer */
  {
    rawTps = analogRead(PEDAL_POT);
    tps = map(rawTps, PEDAL_MIN_VALUE, PEDAL_MAX_VALUE, 0, 100);
    
    if (tps > 100)
      tps = 100;

    Serial.println("Currently using PEDAL input!");

    if ((rawTps < PEDAL_MIN_VALUE - PEDAL_WINDOW) || (rawTps > PEDAL_MAX_VALUE + PEDAL_WINDOW))
    {
      tps = 0;
      Serial.println("!!! Pedal value out of range! Failsafe mode, TPS set to 0 !!!");
    }
  }
  else
  {
    rawTps = analogRead(TPS_POT);
    tps = map(rawTps, 0, 1023, 0, 100);
    Serial.println("Currently using POTENTIOMETER input!");
  }

  bool brakePressed = digitalRead(BRAKE_PIN) == LOW;
  uint8_t brake = brakePressed ? 1 : 0;

  uint8_t data[8] = {0};
  data[0] = rpmScaled;    /* RPM (Potentiometer) */
  data[1] = GEAR_VALUE;   /* Gear (fixed) */
  data[2] = tps;          /* TPS (Potentiometer) */
  data[3] = VSS_VALUE;    /* VSS (fixed) */
  data[4] = brake;        /* Brake (button) */
  data[5] = 0; data[6] = 0; data[7] = 0;  /* Reserved zero bytes */

  byte sendStatus = CAN.sendMsgBuf(CAN_ID, 0, 8, data);
  if (sendStatus == CAN_OK)
    Serial.println("CAN Message Sent");
  else
    Serial.println("Error Sending CAN Message");

  Serial.print("RPM: "); Serial.print(data[0]*100);
  Serial.print("    Gear: "); Serial.print(data[1]);
  Serial.print("    TPS: "); Serial.print(data[2]);
  Serial.print("%    VSS: "); Serial.print(data[3]);
  Serial.print("km/h    Brake: "); Serial.println(data[4]);

  delay(50); /* Send every 50ms */
}