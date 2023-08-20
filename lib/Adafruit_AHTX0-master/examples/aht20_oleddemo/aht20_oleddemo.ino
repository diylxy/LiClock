#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <Adafruit_AHTX0.h>
#include <Fonts/FreeSans9pt7b.h>


Adafruit_SH1107 display = Adafruit_SH1107(64, 128, &Wire);

Adafruit_AHTX0 aht;

void setup() {
  Serial.begin(115200);
  //while (!Serial);
  
  Serial.println("128x64 OLED FeatherWing test");
  display.begin(0x3C, true); // Address 0x3C default

  Serial.println("OLED begun");

  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();
  delay(3000);

  // Clear the buffer.
  display.clearDisplay();
  display.display();

  display.setRotation(1);
  display.setFont(&FreeSans9pt7b);
  if (aht.begin()) {
    Serial.println("Found AHT20");
  } else {
    Serial.println("Didn't find AHT20");
  }  

  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);  
}

void loop() {
  display.clearDisplay();
  sensors_event_t humidity, temp;
  
  aht.getEvent(&humidity, &temp);// populate temp and humidity objects with fresh data
  display.setCursor(0,20);
  display.print("AHT20 Demo");
  display.setCursor(0,40);
  display.print("Temp: "); display.print(temp.temperature); display.println(" C");
  display.setCursor(0,60);
  display.print("Hum: "); display.print(humidity.relative_humidity); display.println(" %");
  Serial.print("Temperature: ");Serial.print(temp.temperature);Serial.println(" degrees C");
  Serial.print("Pressure: ");Serial.print(humidity.relative_humidity);Serial.println(" RH %");
  
  yield();
  display.display();
  delay(100);
}
