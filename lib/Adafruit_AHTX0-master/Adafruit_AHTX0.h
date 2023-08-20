/*!
 *  @file Adafruit_AHTX0.h
 *
 * 	I2C Driver for the Adafruit AHT10 / AHT20 Humidity and Temperature
 *Sensor library
 *
 * 	This is a library for the Adafruit AHT20 breakout:
 * 	https://www.adafruit.com/products/4566
 *
 * 	Adafruit invests time and resources providing this open source code,
 *  please support Adafruit and open-source hardware by purchasing products from
 * 	Adafruit!
 *
 *
 *	BSD license (see license.txt)
 */

#ifndef _ADAFRUIT_AHTX0_H
#define _ADAFRUIT_AHTX0_H

#include "Arduino.h"
#include <Adafruit_BusIO_Register.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

#define AHTX0_I2CADDR_DEFAULT 0x38   ///< AHT default i2c address
#define AHTX0_I2CADDR_ALTERNATE 0x39 ///< AHT alternate i2c address
#define AHTX0_CMD_CALIBRATE 0xE1     ///< Calibration command
#define AHTX0_CMD_TRIGGER 0xAC       ///< Trigger reading command
#define AHTX0_CMD_SOFTRESET 0xBA     ///< Soft reset command
#define AHTX0_STATUS_BUSY 0x80       ///< Status bit for busy
#define AHTX0_STATUS_CALIBRATED 0x08 ///< Status bit for calibrated

class Adafruit_AHTX0;

/**
 * @brief  Adafruit Unified Sensor interface for the humidity sensor component
 * of AHT10/AHT20
 *
 */
class Adafruit_AHTX0_Humidity : public Adafruit_Sensor {
public:
  /** @brief Create an Adafruit_Sensor compatible object for the humidity sensor
    @param parent A pointer to the AHTX0 class */
  Adafruit_AHTX0_Humidity(Adafruit_AHTX0 *parent) { _theAHTX0 = parent; }
  bool getEvent(sensors_event_t *);
  void getSensor(sensor_t *);

private:
  int _sensorID = 0x1020;
  Adafruit_AHTX0 *_theAHTX0 = NULL;
};

/**
 * @brief Adafruit Unified Sensor interface for the temperature sensor component
 * of AHT10/AHT20
 *
 */
class Adafruit_AHTX0_Temp : public Adafruit_Sensor {
public:
  /** @brief Create an Adafruit_Sensor compatible object for the temp sensor
      @param parent A pointer to the AHTX0 class */
  Adafruit_AHTX0_Temp(Adafruit_AHTX0 *parent) { _theAHTX0 = parent; }

  bool getEvent(sensors_event_t *);
  void getSensor(sensor_t *);

private:
  int _sensorID = 0x1021;
  Adafruit_AHTX0 *_theAHTX0 = NULL;
};

/*!
 *    @brief  Class that stores state and functions for interacting with
 *            the AHT10/AHT20 I2C Temperature/Humidity sensor
 */
class Adafruit_AHTX0 {
public:
  Adafruit_AHTX0();
  ~Adafruit_AHTX0();

  bool begin(TwoWire *wire = &Wire, int32_t sensor_id = 0,
             uint8_t i2c_address = AHTX0_I2CADDR_DEFAULT);

  bool getEvent(sensors_event_t *humidity, sensors_event_t *temp);
  uint8_t getStatus(void);
  Adafruit_Sensor *getTemperatureSensor(void);
  Adafruit_Sensor *getHumiditySensor(void);

protected:
  float _temperature, ///< Last reading's temperature (C)
      _humidity;      ///< Last reading's humidity (percent)

  uint16_t _sensorid_humidity; ///< ID number for humidity
  uint16_t _sensorid_temp;     ///< ID number for temperature

  Adafruit_I2CDevice *i2c_dev = NULL; ///< Pointer to I2C bus interface

  Adafruit_AHTX0_Temp *temp_sensor = NULL; ///< Temp sensor data object
  Adafruit_AHTX0_Humidity *humidity_sensor =
      NULL; ///< Humidity sensor data object

private:
  void _fetchTempCalibrationValues(void);
  void _fetchHumidityCalibrationValues(void);
  friend class Adafruit_AHTX0_Temp;     ///< Gives access to private members to
                                        ///< Temp data object
  friend class Adafruit_AHTX0_Humidity; ///< Gives access to private members to
                                        ///< Humidity data object

  void fillTempEvent(sensors_event_t *temp, uint32_t timestamp);
  void fillHumidityEvent(sensors_event_t *humidity, uint32_t timestamp);
};

#endif
