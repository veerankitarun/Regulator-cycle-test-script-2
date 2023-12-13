/**
  ******************************************************************************
  * @file    tdk_ctrl.h
  * @author  Eugene Mwangi
  * @date    07/11/2023
  * @brief   Provides communication with TDK pressure sensor
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2023 PayGo Energy, Inc.</center></h2>
  ******************************************************************************
  */
 #include "tdk_ctrl.h"


 /**
* @brief Reads the temperature and pressure from the pressure sensor(TDK)
* @param buffer: pointer to array variable storing the pressure and temperature values
* @param length: length of the array variable
* @retval true if data is available, false if data is not available
*/
bool readSensor(uint8_t* buffer, uint8_t length) {
  Wire.beginTransmission(SENSOR_ADDRESS);
  Wire.requestFrom(SENSOR_ADDRESS, length);
  if (Wire.available() < length) {
    return false; // Not enough data available
  }
  for (uint8_t i = 0; i < length; i++) {
    buffer[i] = Wire.read();
  }
  return true;
}


/**
* @brief Reads the Sensor and prints out the values
*/
void sendSensorReadings(){
  uint8_t buffer[4]; // variable used to store the pressure and temperature readings
  if (!readSensor(buffer, 4)) {
    Serial.println("+TDK:-1,-1");
    return;
  }
  float result[2];
  parseSensorData(buffer,result, 2);
  float temp_f = result[0];
  float millibar = result[1];
  #if DEBUG_EN
    Serial.print("Pressure (mb): ");
    Serial.print(millibar);
    Serial.println(" mb");

    Serial.print("Temperature: ");
    Serial.print(temp_f);
    Serial.println(" C");
  #endif //DEBUG_EN
  Serial.print("+TDK:");Serial.print(millibar);Serial.print(",");Serial.println(temp_f);
}

/**
* @brief Parse the sensor data to get the temperature and pressure values
* @param buffer: pointer to raw sensor readings
* @param result pointer to array variable where the pressure and temperature readings will be saved
* @param length length of the result array varaible
*/
void parseSensorData(uint8_t* buffer,float* result, uint8_t length){
  uint8_t status = buffer[0] >> 6;
  uint16_t pressure = 0;
  pressure = buffer[0] & 0x3F;
  pressure <<= 8;
  pressure |= buffer[1];

  uint16_t temp = buffer[2];
  temp <<= 8;
  temp |= buffer[3];
  temp >>= 5;

  float temp_f = static_cast<float>(temp);
  temp_f = (temp_f / 2047.0) * 200.0 - 50;

  float pres_f = static_cast<float>(pressure);

  // Convert hectopascals to millibars
  float millibars = ((pres_f - 1635) / 13107) * 100;

  result[0] = temp_f;
  result[1] = millibars;
}
