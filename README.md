# ADXL345
This is a full-featured driver library for the popular
[ADXL345 Accelerometer](https://www.analog.com/en/products/adxl345.html).

It is implemented for STM32 MCUs but could be easily ported to other platforms
like Arduino by just rewriting a few platform-specific functions.

## Structure
The abstract base class ADXL345 represents an interface for changing the settings
of the ADXL345 and reading out its measurement data.

The derived classes ADXL345_I2C and ADXL345_SPI implement the private register IO methods ```_ReadFrom()``` and ```_WriteTo()``` for the respective protocol. 

For more information on the specific registers you can have a look at the
[datasheet](https://www.analog.com/media/en/technical-documentation/data-sheets/ADXL345.pdf).
