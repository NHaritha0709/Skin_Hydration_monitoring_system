// e4161iot@airsworld.net
// Info@2025

#define BLYNK_TEMPLATE_ID "TMPL35xmZp6oj"
#define BLYNK_TEMPLATE_NAME "HYDRATION MONITRING"
#define BLYNK_AUTH_TOKEN "G7h-cacBvgEzBxrz15P-3rHbDKnPDkAu"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <LCD_I2C.h>
#include <HTTPClient.h>


LCD_I2C lcd(0x27, 16, 2);

const int IR_PIN = 33;
const int MOISTURE_PIN = 32;

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "IOT";
char pass[] = "123456789";

int button, iotsend;



void setup() {

  Serial.begin(9600);
  lcd.begin();
  lcd.backlight();

  lcd.setCursor(0,0);
  lcd.print("HYDRATION ");
  lcd.setCursor(0,1);
  lcd.print("MONITORING");

  pinMode(IR_PIN, INPUT);
  pinMode(MOISTURE_PIN, INPUT);

  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);

  delay(2000);
  lcd.clear();
}


void sendToThingSpeak(int irValue, int moistureValue) {

  HTTPClient http;

  String url = "http://api.thingspeak.com/update?api_key=YJ4OG8NX1ZXEBBFE";
  url += "&field1=" + String(irValue);
  url += "&field2=" + String(moistureValue);

  Serial.println("Sending to ThingSpeak:");
  Serial.println(url);

  http.begin(url);
  int httpCode = http.GET();

  Serial.print("HTTP Response: ");
  Serial.println(httpCode);

  http.end();
}

void loop() {

  int irvalue = analogRead(IR_PIN);
  Serial.print("--------------------------------------IR VALUE");
  Serial.println(irvalue);

  irvalue = map(irvalue,4095,0,100,0);
  Serial.println(irvalue);

  lcd.setCursor(0,1);
  lcd.print("T:");
  if(irvalue<=9){lcd.print("00"); lcd.print(irvalue);}
  else if(irvalue<=99){lcd.print("0"); lcd.print(irvalue);}
  else if(irvalue<=999){lcd.print(""); lcd.print(irvalue);}


  if(irvalue>30 && irvalue<=40)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("HYDRATED");
    Serial.print("dehydrated");
    Blynk.logEvent("msg", "HYDRATED ");
    Blynk.virtualWrite(V0,irvalue);
    delay(5000);
    lcd.clear();
  }

  if(irvalue>50 && irvalue<=80)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("DEHYDRATED");
    Serial.print("dehydrated");
    Blynk.logEvent("msg", "DEHYDRATED ");
    Blynk.virtualWrite(V0,irvalue);
    delay(5000);
    lcd.clear();
  }

  Serial.println("MOISTURE");
  Serial.println(analogRead(MOISTURE_PIN));

int rawMoisture = analogRead(MOISTURE_PIN);
// Your real calibration:
// Dry ≈ 4095
// Wet ≈ 1700
int moivalue = map(rawMoisture, 4095, 1700, 0, 100);
moivalue = constrain(moivalue, 0, 100);
Serial.print("Moisture %: ");
Serial.println(moivalue);

lcd.setCursor(0,0);
lcd.print("HY:");
if(moivalue<=9){lcd.print("00"); lcd.print(moivalue);}
else if(moivalue<=99){lcd.print("0"); lcd.print(moivalue);}
else{lcd.print(moivalue);}
lcd.print("%");


  if(moivalue>50&&moivalue<=80)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("DEHYDRATED");
    Serial.print("well hydrated");
    Blynk.logEvent("msg", "  DEHYDRATED");
    delay(5000);
    lcd.clear();
  }
  else if(moivalue>=30&&moivalue<=50)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("HYDRATED");
    Serial.print("HYDRATED");
    Blynk.logEvent("msg", "HYDRATED ");
    delay(5000);
    lcd.clear();
  }

  sendToThingSpeak(irvalue, moivalue);


  Blynk.run();
  delay(500);
}

BLYNK_WRITE(V2) {
  int button = param.asInt();
  if (button == 1)
  {
    iotsend=1;
    Serial.println("value send now....");
  }
  else{
    iotsend=0;
    Serial.print("value send off....");
  }
}