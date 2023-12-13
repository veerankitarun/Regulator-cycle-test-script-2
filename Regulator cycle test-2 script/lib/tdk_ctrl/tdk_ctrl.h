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
 #ifndef TDK_CTRL_h
 #define TDK_CTRL_h

#include <Arduino.h>
#include "Wire.h"
#include "../../include/config.h"

// Function Declaration
bool readSensor(uint8_t* , uint8_t);
void sendSensorReadings();
void parseSensorData(uint8_t* buffer, float* result, uint8_t length);

#endif //TDK_CTRL_h