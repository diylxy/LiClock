/*!
 *  @file Adafruit_AHTX0.cpp
 *
 *  @mainpage Adafruit AHTX0 Humidity and Temperature Sensor library
 *
 *  @section intro_sec Introduction
 *
 * 	I2C Driver for the Adafruit AHTX0 Humidity and Temperature Sensor
 * library
 *
 * 	This is a library for the Adafruit AHT20 breakout:
 * 	https://www.adafruit.com/product/4566
 *
 * 	Adafruit invests time and resources providing this open source code,
 *  please support Adafruit and open-source hardware by purchasing products from
 * 	Adafruit!
 *
 *  @section dependencies Dependencies
 *  This library depends on the Adafruit BusIO library
 *
 *  This library depends on the Adafruit Unified Sensor library
 *
 *  @section author Author
 *
 *  Limor Fried (Adafruit Industries)
 *
 * 	@section license License
 *
 * 	BSD (see license.txt)
 *
 * 	@section  HISTORY
 *
 *     v1.0 - First release
 */

#include "Arduino.h"

#include "Adafruit_AHTX0.h"

/*!
 *    @brief  Instantiates a new AHTX0 class
 */
Adafruit_AHTX0::Adafruit_AHTX0(void) {}

Adafruit_AHTX0::~Adafruit_AHTX0(void) {
  if (temp_sensor) {
    delete temp_sensor;
  }
  if (humidity_sensor) {
    delete humidity_sensor;
  }
}

/*!
 *    @brief  Sets up the hardware and initializes I2C
 *    @param  wire
 *            The Wire object to be used for I2C connections.
 *    @param  sensor_id
 *            The unique ID to differentiate the sensors from others
 *    @param  i2c_address
 *            The I2C address used to communicate with the sensor
 *    @return True if initialization was successful, otherwise false.
 */
bool Adafruit_AHTX0::begin(TwoWire *wire, int32_t sensor_id,
                           uint8_t i2c_address) {
  delay(20); // 20 ms to power up

  if (i2c_dev) {
    delete i2c_dev; // remove old interface
  }

  i2c_dev = new Adafruit_I2CDevice(i2c_address, wire);

  if (!i2c_dev->begin()) {
    return false;
  }

  uint8_t cmd[3];

  cmd[0] = AHTX0_CMD_SOFTRESET;
  if (!i2c_dev->write(cmd, 1)) {
    return false;
  }
  delay(20);

  while (getStatus() & AHTX0_STATUS_BUSY) {
    delay(10);
  }

  cmd[0] = AHTX0_CMD_CALIBRATE;
  cmd[1] = 0x08;
  cmd[2] = 0x00;
  i2c_dev->write(cmd, 3); // may not 'succeed' on newer AHT20s

  while (getStatus() & AHTX0_STATUS_BUSY) {
    delay(10);
  }
  if (!(getStatus() & AHTX0_STATUS_CALIBRATED)) {
    return false;
  }

  delete humidity_sensor;
  delete temp_sensor;
  humidity_sensor = new Adafruit_AHTX0_Humidity(this);
  temp_sensor = new Adafruit_AHTX0_Temp(this);
  return true;
}

/**
 * @brief  Gets the status (first byte) from AHT10/AHT20
 *
 * @returns 8 bits of status data, or 0xFF if failed
 */
uint8_t Adafruit_AHTX0::getStatus(void) {
  uint8_t ret;
  if (!i2c_dev->read(&ret, 1)) {
    return 0xFF;
  }
  return ret;
}

/**************************************************************************/
/*!
    @brief  Gets the humidity sensor and temperature values as sensor events
    @param  humidity Sensor event object that will be populated with humidity
   data
    @param  temp Sensor event object that will be populated with temp data
    @returns true if the event data was read successfully
*/
/**************************************************************************/
bool Adafruit_AHTX0::getEvent(sensors_event_t *humidity,
                              sensors_event_t *temp) {
  uint32_t t = millis();

  // read the data and store it!
  uint8_t cmd[3] = {AHTX0_CMD_TRIGGER, 0x33, 0};
  if (!i2c_dev->write(cmd, 3)) {
    return false;
  }

  while (getStatus() & AHTX0_STATUS_BUSY) {
    delay(10);
  }

  uint8_t data[6];
  if (!i2c_dev->read(data, 6)) {
    return false;
  }
  uint32_t h = data[1];
  h <<= 8;
  h |= data[2];
  h <<= 4;
  h |= data[3] >> 4;
  _humidity = ((float)h * 100) / 0x100000;

  uint32_t tdata = data[3] & 0x0F;
  tdata <<= 8;
  tdata |= data[4];
  tdata <<= 8;
  tdata |= data[5];
  _temperature = ((float)tdata * 200 / 0x100000) - 50;

  // use helpers to fill in the events
  if (temp)
    fillTempEvent(temp, t);
  if (humidity)
    fillHumidityEvent(humidity, t);
  return true;
}

void Adafruit_AHTX0::fillTempEvent(sensors_event_t *temp, uint32_t timestamp) {
  memset(temp, 0, sizeof(sensors_event_t));
  temp->version = sizeof(sensors_event_t);
  temp->sensor_id = _sensorid_temp;
  temp->type = SENSOR_TYPE_AMBIENT_TEMPERATURE;
  temp->timestamp = timestamp;
  temp->temperature = _temperature;
}

void Adafruit_AHTX0::fillHumidityEvent(sensors_event_t *humidity,
                                       uint32_t timestamp) {
  memset(humidity, 0, sizeof(sensors_event_t));
  humidity->version = sizeof(sensors_event_t);
  humidity->sensor_id = _sensorid_humidity;
  humidity->type = SENSOR_TYPE_AMBIENT_TEMPERATURE;
  humidity->timestamp = timestamp;
  humidity->relative_humidity = _humidity;
}

/**
 * @brief Gets the Adafruit_Sensor object for the AHTx0's humidity sensor
 *
 * @return Adafruit_Sensor*
 */
Adafruit_Sensor *Adafruit_AHTX0::getHumiditySensor(void) {
  return humidity_sensor;
}

/**
 * @brief Gets the Adafruit_Sensor object for the AHTx0's humidity sensor
 *
 * @return Adafruit_Sensor*
 */
Adafruit_Sensor *Adafruit_AHTX0::getTemperatureSensor(void) {
  return temp_sensor;
}
/**
 * @brief  Gets the sensor_t object describing the AHTx0's humidity sensor
 *
 * @param sensor The sensor_t object to be populated
 */
void Adafruit_AHTX0_Humidity::getSensor(sensor_t *sensor) {
  /* Clear the sensor_t object */
  memset(sensor, 0, sizeof(sensor_t));

  /* Insert the sensor name in the fixed length char array */
  strncpy(sensor->name, "AHTx0_H", sizeof(sensor->name) - 1);
  sensor->name[sizeof(sensor->name) - 1] = 0;
  sensor->version = 1;
  sensor->sensor_id = _sensorID;
  sensor->type = SENSOR_TYPE_RELATIVE_HUMIDITY;
  sensor->min_delay = 0;
  sensor->min_value = 0;
  sensor->max_value = 100;
  sensor->resolution = 2;
}
/**
    @brief  Gets the humidity as a standard sensor event
    @param  event Sensor event object that will be populated
    @returns True
 */
bool Adafruit_AHTX0_Humidity::getEvent(sensors_event_t *event) {
  _theAHTX0->getEvent(event, NULL);

  return true;
}
/**
 * @brief  Gets the sensor_t object describing the AHTx0's tenperature sensor
 *
 * @param sensor The sensor_t object to be populated
 */
void Adafruit_AHTX0_Temp::getSensor(sensor_t *sensor) {
  /* Clear the sensor_t object */
  memset(sensor, 0, sizeof(sensor_t));

  /* Insert the sensor name in the fixed length char array */
  strncpy(sensor->name, "AHTx0_T", sizeof(sensor->name) - 1);
  sensor->name[sizeof(sensor->name) - 1] = 0;
  sensor->version = 1;
  sensor->sensor_id = _sensorID;
  sensor->type = SENSOR_TYPE_AMBIENT_TEMPERATURE;
  sensor->min_delay = 0;
  sensor->min_value = -40;
  sensor->max_value = 85;
  sensor->resolution = 0.3; // depends on calibration data?
}
/*!
    @brief  Gets the temperature as a standard sensor event
    @param  event Sensor event object that will be populated
    @returns true
*/
bool Adafruit_AHTX0_Temp::getEvent(sensors_event_t *event) {
  _theAHTX0->getEvent(NULL, event);

  return true;
}
