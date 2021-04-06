/*
adxl345.hpp - ADXL345 library header

Copyright (C) 2021  NANDLAB

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef ADXL345_HPP_
#define ADXL345_HPP_

#include "main.h"

// Abstract ADXL345 Base Class
class ADXL345 {
public:
	// A StatusType of zero means success, non-zero means failure.
	typedef uint8_t StatusType;
	enum {
		/******************* STATUS CODES *********************/
		// inherited from HAL_StatusTypeDef plus following custom ones
		STATUS_INVALID_ID	=	0x10,

		/******************* REGISTER MAP *********************/
		REG_DEVID			=	0x00,		// Device ID
		REG_RESERVED_FIRST	=	0x01,		// First reserved register. Reserved registers should not be accessed.
		REG_RESERVED_LAST	=	0x1C,		// Last reserved register.
		REG_THRESH_TAP		=	0x1D,		// Tap Threshold				// 62.5 mg/LSB (that is, 0xFF = +16g)
		REG_OFSX			=	0x1E,		// X-Axis Offset				// )
		REG_OFSY			=	0x1F,		// Y-Axis Offset				// } 15.6 mg/LSB (that is, 0x7F = +2g)
		REG_OFSZ			=	0x20,		// Z-Axis Offset				// )
		REG_DUR				=	0x21,		// Tap Duration					// 625 μs/LSB.  0 disables tap/double tap.
		REG_LATENT			=	0x22,		// Tap Latency					// 1.25 ms/LSB. 0 disables double tap.
		REG_WINDOW			=	0x23,		// Tap Window					// 1.25 ms/LSB. 0 disables double tap.
		REG_THRESH_ACT		=	0x24,		// Activity Threshold			// 62.5 mg/LSB
		REG_THRESH_INACT	=	0x25,		// Inactivity Threshold			// 62.5 mg/LSB
		REG_TIME_INACT		=	0x26,		// Inactivity Time				// 1 sec/LSB
		REG_ACT_INACT_CTL	=	0x27,		// Axis Enable Control for Activity and Inactivity Detection
		REG_THRESH_FF		=	0x28,		// Free-Fall Threshold			// 62.5 mg/LSB. Between 300 and 600 mg (0x05 to 0x09) recommended.
		REG_TIME_FF			=	0x29,		// Free-Fall Time				// 5 ms/LSB. Between 100 and 350 ms (0x14 to 0x46) recommended.
		REG_TAP_AXES		=	0x2A,		// Axis Control for Tap/Double Tap
		REG_ACT_TAP_STATUS	=	0x2B,		// Source of Tap/Double Tap
		REG_BW_RATE			=	0x2C,		// Data Rate and Power mode Control
		REG_POWER_CTL		=	0x2D,		// Power-Saving Features Control
		REG_INT_ENABLE		=	0x2E,		// Interrupt Enable Control
		REG_INT_MAP			=	0x2F,		// Interrupt Mapping Control
		REG_INT_SOURCE		=	0x30,		// Source of Interrupts
		REG_DATA_FORMAT		=	0x31,		// Data Format Control
		REG_DATAX0			=	0x32,		// X-Axis Data 0
		REG_DATAX1			=	0x33,		// X-Axis Data 1
		REG_DATAY0			=	0x34,		// Y-Axis Data 0
		REG_DATAY1			=	0x35,		// Y-Axis Data 1
		REG_DATAZ0			=	0x36,		// Z-Axis Data 0
		REG_DATAZ1			=	0x37,		// Z-Axis Data 1
		REG_FIFO_CTL		=	0x38,		// FIFO Control
		REG_FIFO_STATUS		=	0x39,		// FIFO Status

		/******************** BIT POSITIONS *****************/
//		Setting BIT_ACT_INACT_CTL_ACT_AC sets AC coupled activity detection.
//		It is DC coupled by default.
//		Setting BIT_ACT_INACT_CTL_ACT_X enables the X-axis for activity detection.
//		Similar for inactivity and other axes.
		BIT_ACT_INACT_CTL_ACT_AC		=	0x7,
		BIT_ACT_INACT_CTL_ACT_X			=	0x6,
		BIT_ACT_INACT_CTL_ACT_Y			=	0x5,
		BIT_ACT_INACT_CTL_ACT_Z			=	0x4,
		BIT_ACT_INACT_CTL_INACT_AC		=	0x3,
		BIT_ACT_INACT_CTL_INACT_X		=	0x2,
		BIT_ACT_INACT_CTL_INACT_Y		=	0x1,
		BIT_ACT_INACT_CTL_INACT_Z		=	0x0,

//		Setting an axis bit enables the corresponding axis' participation in tap detection.
//		Tap detection is always AC-coupled.
//		Setting the suppress bit suppresses double tap detection if
//		acceleration greater than the value in THRESH_TAP is present
//		between taps.
		BIT_TAP_AXES_SUPPRESS	=	0x3,
		BIT_TAP_AXES_TAP_X		=	0x2,
		BIT_TAP_AXES_TAP_Y		=	0x1,
		BIT_TAP_AXES_TAP_Z		=	0x0,

//		The ACT_x and TAP_x bits indicate the first axis
//		involved in a tap or activity event.
//		A setting of 1 in the asleep bit indicates that the part is asleep. (autosleep)
		BIT_ACT_TAP_STATUS_ACT_X	=	0x6,
		BIT_ACT_TAP_STATUS_ACT_Y	=	0x5,
		BIT_ACT_TAP_STATUS_ACT_Z	=	0x4,
		BIT_ACT_TAP_STATUS_ASLEEP	=	0x3,
		BIT_ACT_TAP_STATUS_TAP_X	=	0x2,
		BIT_ACT_TAP_STATUS_TAP_Y	=	0x1,
		BIT_ACT_TAP_STATUS_TAP_Z	=	0x0,

		BIT_BW_RATE_LOW_POWER		=	0x4,
		BIT_BW_RATE_RATE_MSB		=	0x3,
		BIT_BW_RATE_RATE_LSB		=	0x0,

		BIT_POWER_CTL_LINK			=	0x5,
		BIT_POWER_CTL_AUTO_SLEEP	=	0x4,
		BIT_POWER_CTL_MEASURE		=	0x3,
		BIT_POWER_CTL_SLEEP			=	0x2,
		BIT_POWER_CTL_WAKEUP_MSB	=	0x1,
		BIT_POWER_CTL_WAKEUP_LSB	=	0x0,

		BIT_INT_DATA_READY	=	0x7,
		BIT_INT_SINGLE_TAP	=	0x6,
		BIT_INT_DOUBLE_TAP	=	0x5,
		BIT_INT_ACTIVITY	=	0x4,
		BIT_INT_INACTIVITY	=	0x3,
		BIT_INT_FREE_FALL	=	0x2,
		BIT_INT_WATERMARK	=	0x1,
		BIT_INT_OVERRUN		=	0x0,

		BIT_DATA_FORMAT_SELF_TEST		=	0x7,
		BIT_DATA_FORMAT_SPI_3WIRE		=	0x6,
		BIT_DATA_FORMAT_INT_INVERT		=	0x5,	// reset: int pins active high;   set: active low
		BIT_DATA_FORMAT_FULL_RES		=	0x3,
		BIT_DATA_FORMAT_JUSTIFY_LEFT	=	0x2,
		BIT_DATA_FORMAT_RANGE_MSB		=	0x1,
		BIT_DATA_FORMAT_RANGE_LSB		=	0x0,

		BIT_FIFO_CTL_MODE_MSB		=	0x7,
		BIT_FIFO_CTL_MODE_LSB		=	0x6,
		BIT_FIFO_CTL_TRIGGER_INT2	=	0x5,
		BIT_FIFO_CTL_SAMPLES_MSB	=	0x4,
		BIT_FIFO_CTL_SAMPLES_LSB	=	0x0,

		BIT_FIFO_STATUS_FIFO_TRIG	=	0x7,
		BIT_FIFO_STATUS_ENTRIES_MSB	=	0x5,
		BIT_FIFO_STATUS_ENTRIES_LSB	=	0x0,

		/****************** REGISTER VALUES ******************/
		VAL_DEVICE_ID	=	0345,			// Read-only value in REG_DEVID

		VAL_BW_1600_Hz		=	0xF,		// 1111		IDD = 40uA
		VAL_BW_800_Hz		=	0xE,		// 1110		IDD = 90uA
		VAL_BW_400_Hz		=	0xD,		// 1101		IDD = 140uA
		VAL_BW_200_Hz		=	0xC,		// 1100		IDD = 140uA
		VAL_BW_100_Hz		=	0xB,		// 1011		IDD = 140uA
		VAL_BW_50_Hz		=	0xA,		// 1010		IDD = 140uA
		VAL_BW_25_Hz		=	0x9,		// 1001		IDD = 90uA
		VAL_BW_12_5_Hz		=	0x8,		// 1000		IDD = 60uA
		VAL_BW_6_25_Hz		=	0x7,		// 0111		IDD = 50uA
		VAL_BW_3_13_Hz		=	0x6,		// 0110		IDD = 45uA
		VAL_BW_1_56_Hz		=	0x5,		// 0101		IDD = 40uA
		VAL_BW_0_78_Hz		=	0x4,		// 0100		IDD = 34uA
		VAL_BW_0_39_Hz		=	0x3,		// 0011		IDD = 23uA
		VAL_BW_0_20_Hz		=	0x2,		// 0010		IDD = 23uA
		VAL_BW_0_10_Hz		=	0x1,		// 0001		IDD = 23uA
		VAL_BW_0_05_Hz		=	0x0,		// 0000		IDD = 23uA

		VAL_WAKEUP_8_Hz	=	0x0,
		VAL_WAKEUP_4_Hz	=	0x1,
		VAL_WAKEUP_2_Hz	=	0x2,
		VAL_WAKEUP_1_Hz	=	0x3,

		VAL_RANGE_2G	=	0x0,
		VAL_RANGE_4G	=	0x1,
		VAL_RANGE_8G	=	0x2,
		VAL_RANGE_16G	=	0x3,

		VAL_FIFO_MODE_BYPASS	= 0x0,
		VAL_FIFO_MODE_FIFO		= 0x1,
		VAL_FIFO_MODE_STREAM	= 0x2,
		VAL_FIFO_MODE_TRIGGER	= 0x3,

		/******************* INTERRUPT PINS ******************/
		PIN_INT1	=	0x0,
		PIN_INT2	=	0x1,

		/********************* PIN STATES ********************/
		PIN_STATE_HIGH	=	0x1,
		PIN_STATE_LOW	=	0x0,

		/************************ MISC ***********************/
		BUFFER_MAX		=	0x6,
	};
	// Enum with possibly larger constants.
	enum {
		COM_TIMEOUT	=	128, // communication timeout in ms
	};

	ADXL345()
	:	_dataFormat (0x00),
		_gain {1.0f, 1.0f, 1.0f}
	{}
	~ADXL345() {}

//	void methods never fail.
//	If a method returns StatusType, it should be checked
//	if it succeeded (zero) or not (non-zero).
//	float acceleration arguments are in G.
//	float time arguments are in ms.

	void SetGain(const float gain[3]);
	void GetGain(      float gain[3]);

	/**************** DEVID ****************/
	StatusType GetDeviceID(uint8_t *deviceID);
	StatusType CheckDeviceID(); // Returns a non-zero status if it does not read 0345

	/************** THRESH_TAP *************/
	StatusType SetThreshTapRaw(uint8_t  thresh);
	StatusType GetThreshTapRaw(uint8_t *thresh);
	StatusType SetThreshTap(float  thresh);
	StatusType GetThreshTap(float *thresh);

	/*********** OFSX, OFSY, OFSZ **********/
	StatusType SetOffsetRaw(const int8_t offset[3]);
	StatusType GetOffsetRaw(      int8_t offset[3]);
	StatusType SetOffset(const float offset[3]);
	StatusType GetOffset(      float offset[3]);

	/***************** DUR *****************/
	StatusType SetTapDurRaw(uint8_t  dur);
	StatusType GetTapDurRaw(uint8_t *dur);
	StatusType SetTapDur(float  dur);
	StatusType GetTapDur(float *dur);

	/**************** Latent ***************/
	StatusType SetTapLatencyRaw(uint8_t  latency);
	StatusType GetTapLatencyRaw(uint8_t *latency);
	StatusType SetTapLatency(float  latency);
	StatusType GetTapLatency(float *latency);

	/**************** Window ***************/
	StatusType SetTapWindowRaw(uint8_t  window);
	StatusType GetTapWindowRaw(uint8_t *window);
	StatusType SetTapWindow(float  window);
	StatusType GetTapWindow(float *window);

	/************** THRESH_ACT *************/
	StatusType SetThreshActRaw(uint8_t  thresh);
	StatusType GetThreshActRaw(uint8_t *thresh);
	StatusType SetThreshAct(float  thresh);
	StatusType GetThreshAct(float *thresh);

	/************* THRESH_INACT ************/
	StatusType SetThreshInactRaw(uint8_t  thresh);
	StatusType GetThreshInactRaw(uint8_t *thresh);
	StatusType SetThreshInact(float  thresh);
	StatusType GetThreshInact(float *thresh);

	/************** TIME_INACT *************/
	StatusType SetTimeInact(uint8_t  timeSec);
	StatusType GetTimeInact(uint8_t *timeSec);

	/************ ACT_INACT_CTL ************/
	StatusType SetActInactCtl(uint8_t  bitfield);
	StatusType GetActInactCtl(uint8_t *bitfield);

	/************** THRESH_FF **************/
	StatusType SetThreshFFRaw(uint8_t  thresh);
	StatusType GetThreshFFRaw(uint8_t *thresh);
	StatusType SetThreshFF(float  thresh);
	StatusType GetThreshFF(float *thresh);

	/*************** TIME_FF ***************/
	StatusType SetTimeFFRaw(uint8_t  time);
	StatusType GetTimeFFRaw(uint8_t *time);
	StatusType SetTimeFF(unsigned  time_ms);
	StatusType GetTimeFF(unsigned *time_ms);

	/************** TAP_AXES ***************/
	StatusType SetTapAxes(uint8_t  bitfield);
	StatusType GetTapAxes(uint8_t *bitfield);

	/************ ACT_TAP_STATUS ***********/
	StatusType GetActTapStatus(uint8_t *bitfield);

	StatusType GetAsleep(bool *asleep);

	/*************** BW_RATE ***************/
	StatusType SetBwRate(uint8_t  bitfield);
	StatusType GetBwRate(uint8_t *bitfield);

	StatusType SetLowPower(bool  lowPower);
	StatusType GetLowPower(bool *lowPower);

	StatusType SetRate(uint8_t  rate);
	StatusType GetRate(uint8_t *rate);

	/************** POWER_CTL **************/
	StatusType SetPowerCtl(uint8_t  bitfield);
	StatusType GetPowerCtl(uint8_t *bitfield);

	StatusType SetLink(bool  link);
	StatusType GetLink(bool *link);

	StatusType SetAutoSleep(bool  autoSleep);
	StatusType GetAutoSleep(bool *autoSleep);

	StatusType SetMeasure(bool  measure);
	StatusType GetMeasure(bool *measure);

	StatusType SetSleep(bool  sleep);
	StatusType GetSleep(bool *sleep);

	StatusType SetWakeup(uint8_t  wakeup);
	StatusType GetWakeup(uint8_t *wakeup);

	/************** INT_ENABLE *************/
	StatusType SetIntEnable(uint8_t  bitfield);
	StatusType GetIntEnable(uint8_t *bitfield);

	/*************** INT_MAP ***************/
//	0 maps to INT1 pin, 1 maps to INT2 pin.
	StatusType SetIntMap(uint8_t  bitfield);
	StatusType GetIntMap(uint8_t *bitfield);

	/************** INT_SOURCE *************/
//	The DATA_READY, watermark, and overrun bits are always set
//	if the corresponding events occur, regardless of the INT_ENABLE register settings.
//	Other bits, and the corresponding interrupts,
//	are cleared by reading the INT_SOURCE register.
	StatusType GetIntSource(uint8_t *bitfield);

	/************* DATA_FORMAT *************/
//	All the DATA_FORMAT getter methods extract info from _dataFormat
//	so it should be ensured that _dataFormat is up to date.
//	A setter or a RefreshDataFormat() call updates _dataFormat.
	StatusType	SetDataFormat(uint8_t  bitfield);
	StatusType	RefreshDataFormat(); // Reads the DATA_FORMAT register value into _dataFormat
	void		GetDataFormat(uint8_t *bitfield);

	StatusType	SetSelfTest(bool  selfTest);
	void		GetSelfTest(bool *selfTest);

	StatusType	SetSPI3Wire(bool  spi3wire);
	void		GetSPI3Wire(bool *spi3wire);

	StatusType	SetIntActiveLow(bool  intActiveLow);
	void		GetIntActiveLow(bool *intActiveLow);

	StatusType	SetFullRes(bool  fullRes);
	void		GetFullRes(bool *fullRes);

	StatusType	SetLeftJustify(bool  leftJustify);
	void		GetLeftJustify(bool *leftJustify);

//	Expects VAL_RANGE_x as argument
	StatusType	SetRange(uint8_t  range);
	void		GetRange(uint8_t *range);

	/**************** DATAxx ***************/
	StatusType GetDataRaw(int16_t data[3]);
	StatusType GetData(float data[3]);

	/*************** FIFO_CTL **************/
	StatusType SetFifoCtl(uint8_t  bitfield);
	StatusType GetFifoCtl(uint8_t *bitfield);

//	VAL_FIFO_MODE_x expected as argument
	StatusType SetFifoMode(uint8_t  mode);
	StatusType GetFifoMode(uint8_t *mode);

	StatusType SetFifoTriggerInt2(bool  triggerInt2);
	StatusType GetFifoTriggerInt2(bool *triggerInt2);

//	samples can range from 0 to 31
	StatusType SetFifoSamples(uint8_t  samples);
	StatusType GetFifoSamples(uint8_t *samples);

	/************* FIFO_STATUS *************/
	StatusType GetFifoStatus(uint8_t *fifoStatus);

	StatusType GetFifoTrig(bool *fifoTrig);

	StatusType GetFifoEntries(uint8_t *entries);
private:
//	n is the number of bytes in data. It should be at most BUFFER_MAX.
	virtual StatusType _WriteTo(uint8_t reg, uint8_t val) = 0;
	virtual StatusType _WriteTo(uint8_t reg, const uint8_t data[], uint8_t n) = 0;
	virtual StatusType _ReadFrom(uint8_t reg, uint8_t *val) = 0;
	virtual StatusType _ReadFrom(uint8_t reg, uint8_t data[], uint8_t n) = 0;
	bool _SelfTest()		{ return  _dataFormat >> BIT_DATA_FORMAT_SELF_TEST; }
	bool _SPI3Wire()		{ return (_dataFormat >> BIT_DATA_FORMAT_SPI_3WIRE)		& 1; }
	bool _IntActiveLow()	{ return (_dataFormat >> BIT_DATA_FORMAT_INT_INVERT)	& 1; }
	bool _FullRes()			{ return (_dataFormat >> BIT_DATA_FORMAT_FULL_RES)		& 1; }
	bool _LeftJustify()		{ return (_dataFormat >> BIT_DATA_FORMAT_JUSTIFY_LEFT)	& 1; }
	uint8_t _Range() {
		const uint8_t rangeMask = 0x03; // Mask two least significant bits
		return _dataFormat & rangeMask;
	}
	uint8_t _dataFormat; // local backup of the value in the DATA_FORMAT register
	float _gain[3];
};


class ADXL345_I2C : public ADXL345 {
public:
	enum {
		/***************** DEVICE PROPERTIES ******************/
		DEVICE_I2C_ADDR_SDO_LOW		=	0xA6,	// 8-bit I2C address with SDO pulled low
		DEVICE_I2C_ADDR_SDO_HIGH	=	0x3A,	// 8-bit I2C address with SDO pulled high
	};
	ADXL345_I2C(I2C_HandleTypeDef *hi2c, uint8_t sdoState = PIN_STATE_LOW)
	:	ADXL345(),
		_hi2c (hi2c),
		_devAddr (sdoState == PIN_STATE_LOW ? DEVICE_I2C_ADDR_SDO_LOW : DEVICE_I2C_ADDR_SDO_HIGH)
	{}
private:
	virtual StatusType _WriteTo(uint8_t reg, uint8_t val);
	virtual StatusType _WriteTo(uint8_t reg, const uint8_t data[], uint8_t n);
	virtual StatusType _ReadFrom(uint8_t reg, uint8_t *val);
	virtual StatusType _ReadFrom(uint8_t reg, uint8_t data[], uint8_t n);
	I2C_HandleTypeDef *_hi2c;
	uint8_t _devAddr;
};

class ADXL345_SPI : public ADXL345 {
public:
	ADXL345_SPI(SPI_HandleTypeDef *hspi, GPIO_TypeDef *ssPort, uint16_t ssPin)
	:	ADXL345(),
		_hspi (hspi),
		_ssPort (ssPort),
		_ssPin (ssPin)
	{}
private:
	virtual StatusType _WriteTo(uint8_t reg, uint8_t val);
	virtual StatusType _WriteTo(uint8_t reg, const uint8_t data[], uint8_t n);
	virtual StatusType _ReadFrom(uint8_t reg, uint8_t *val);
	virtual StatusType _ReadFrom(uint8_t reg, uint8_t data[], uint8_t n);
	SPI_HandleTypeDef *_hspi;
	GPIO_TypeDef *_ssPort;
	uint16_t _ssPin;
};

#endif /* ADXL345_HPP_ */
