#include <MAX30100_PulseOximeter.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define REPORTING_PERIOD_MS 1000
PulseOximeter pox;
float BPM, SpO2;
uint32_t tsLastReport = 0;
int buzzer =D2;

LiquidCrystal_I2C lcd(0x27, 16, 2);  

void onBeatDetected()
{
    digitalWrite(D4, HIGH);
   
}

void setup()
{
    pinMode(D4, OUTPUT);
    Serial.begin(9600);
    lcd.begin(16, 2);  
    lcd.backlight();   

    pinMode(buzzer,OUTPUT);

    if (!pox.begin())
    {
        lcd.print("Error initializing");
        lcd.setCursor(0, 1);
        lcd.print("MAX30100!");
        while (1);
    }
    else
    {
        pox.setOnBeatDetectedCallback(onBeatDetected);
    }
    pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
}

void loop()
{
    pox.update();
    BPM = pox.getHeartRate();
    SpO2 = pox.getSpO2();
    if (millis() - tsLastReport > REPORTING_PERIOD_MS)
    {
        digitalWrite(D4, LOW);

    
        Serial.print("Heart Rate:");
        Serial.print(BPM);
        Serial.print(",");
        Serial.print("SpO2:");
        Serial.println(SpO2);

      if (SpO2 < 90 && BPM < 100 )
      {

          Serial.println("patient is in danger");

          digitalWrite(buzzer,1);

      }
      else
      {
            Serial.println("patient is good");

            digitalWrite(buzzer,0);
      }


        
        lcd.clear();
        lcd.print("BPM: ");
        lcd.print(BPM);
        lcd.setCursor(0, 1);
        lcd.print("SpO2: ");
        lcd.print(SpO2);

        tsLastReport = millis();
    }
}
