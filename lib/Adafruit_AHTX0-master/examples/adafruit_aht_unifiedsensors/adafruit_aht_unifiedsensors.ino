// Demo for getting individual unified sensor data from the AHT Humidity and Temperature sensor

#include <Adafruit_AHTX0.h>

Adafruit_AHTX0 aht;

Adafruit_Sensor *aht_humidity, *aht_temp;

void setup(void) {
  Serial.begin(115200);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit AHT10/AHT20 test!");

  if (!aht.begin()) {
    Serial.println("Failed to find AHT10/AHT20 chip");
    while (1) {
      delay(10);
    }
  }

  Serial.println("AHT10/AHT20 Found!");
  aht_temp = aht.getTemperatureSensor();
  aht_temp->printSensorDetails();

  aht_humidity = aht.getHumiditySensor();
  aht_humidity->printSensorDetails();
}

void loop() {
  //  /* Get a new normalized sensor event */
  sensors_event_t humidity;
  sensors_event_t temp;
  aht_humidity->getEvent(&humidity);
  aht_temp->getEvent(&temp);

  Serial.print("\t\tTemperature ");
  Serial.print(temp.temperature);
  Serial.println(" deg C");

  /* Display the results (humidity is measured in % relative humidity (% rH) */
  Serial.print("\t\tHumidity: ");
  Serial.print(humidity.relative_humidity);
  Serial.println(" % rH");
  Serial.print("\t\tTemperature: ");
  Serial.print(temp.temperature);
  Serial.println(" degrees C");


  delay(100);

  /*//   serial plotter friendly format
  Serial.print(temp.temperature);
  Serial.print(",");

  Serial.print(humidity.relative_humidity);

  Serial.println();
  delay(10);
  */
}
