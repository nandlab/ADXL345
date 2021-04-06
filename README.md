# ADXL345
This is a full-featured driver library for the popular
[ADXL345 Accelerometer](https://www.analog.com/en/products/adxl345.html).

It is implemented for STM32 MCUs but could be easily ported to other platforms
like Arduino by just rewriting a few platform-specific functions.

## Structure
The abstract base class ADXL345 represents an interface for changing the settings
of the ADXL345 and reading out its measurement data.
Most methods return type StatusType, it should be checked if it is zero (success) or non-zero (failure).

The derived classes ADXL345_I2C and ADXL345_SPI implement the private register IO methods ```_ReadFrom()``` and ```_WriteTo()``` for the respective protocol. 

For more information on the specific registers you can have a look at the
[datasheet](https://www.analog.com/media/en/technical-documentation/data-sheets/ADXL345.pdf).

## Code Example
```cpp
#include "main.h"
#include <cstring>
#include <cstdio>
#include "adxl345.hpp"

using namespace std;

extern "C" void mymain(void);
extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef huart2;

ADXL345::StatusType ADXL345_Setup(ADXL345 *adxl345) {
  ADXL345::StatusType status;

  status = adxl345->CheckDeviceID();
  if (status) return status;

  status = adxl345->SetRange(ADXL345::VAL_RANGE_16G);
  if (status) return status;

  status = adxl345->SetFullRes(true);
  if (status) return status;

  status = adxl345->SetMeasure(true);
  if (status) return status;

  return ADXL345::StatusType(0);
}

void mymain(void) {
  char str[128];
  size_t len;
  float data[3];
  ADXL345_I2C mpu(&hi2c1);
  ADXL345::StatusType status = ADXL345_Setup(&mpu);
  if (status) {
    sprintf(str, "Adxl345 set up failed: %hu\r\n", (unsigned short) status);
    len = strlen(str);
    HAL_UART_Transmit(&huart2, (uint8_t*)str, len, HAL_MAX_DELAY);
  }
  else {
    sprintf(str, "Adxl345 set up successful.\r\n");
    len = strlen(str);
    HAL_UART_Transmit(&huart2, (uint8_t*)str, len, HAL_MAX_DELAY);
  }
  while (true) {
    status = mpu.GetData(data);
    if (status) {
      sprintf(str, "Adxl345 reading measurement failed: %hu\r\n", (unsigned short) status);
      len = strlen(str);
      HAL_UART_Transmit(&huart2, (uint8_t*)str, len, HAL_MAX_DELAY);
    }
    else {
      sprintf(str, "[ %6.3f | %6.3f | %6.3f ]\r\n", double(data[0]), double(data[1]), double(data[2]));
      len = strlen(str);
      HAL_UART_Transmit(&huart2, (uint8_t*)str, len, HAL_MAX_DELAY);
    }
    HAL_Delay(100);
  }
}

```
