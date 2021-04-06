/*
adxl345.cpp - ADXL345 library source

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

#include "adxl345.hpp"
#include "main.h"
#include <cmath>

using namespace std;

static uint8_t SquashLongIntoUint(long l) {
	if (l <= 0) {
		l = 0;
	}
	else if (l >= UINT8_MAX) {
		l = UINT8_MAX;
	}
	return uint8_t(l);
}

static int8_t SquashLongIntoInt(long l) {
	if (l <= INT8_MIN) {
		l = INT8_MIN;
	}
	else if (l >= INT8_MAX) {
		l = INT8_MAX;
	}
	return int8_t(l);
}

void ADXL345::SetGain(const float gain[3]) {
	for (uint8_t i=0; i<3; i++) {
		_gain[i] = gain[i];
	}
}

void ADXL345::GetGain(float gain[3]) {
	for (uint8_t i=0; i<3; i++) {
		gain[i] = _gain[i];
	}
}

ADXL345::StatusType ADXL345::GetDeviceID(uint8_t *deviceID) {
	return _ReadFrom(REG_DEVID, deviceID);
}

ADXL345::StatusType ADXL345::CheckDeviceID() {
	StatusType status;
	uint8_t dID;
	status = GetDeviceID(&dID);
	if (!status && dID != VAL_DEVICE_ID)
	{ status = STATUS_INVALID_ID; }
	return status;
}

ADXL345::StatusType ADXL345::SetThreshTapRaw(uint8_t thresh) {
	return _WriteTo(REG_THRESH_TAP, thresh);
}

ADXL345::StatusType ADXL345::GetThreshTapRaw(uint8_t *thresh) {
	return _ReadFrom(REG_THRESH_TAP, thresh);
}

ADXL345::StatusType ADXL345::SetThreshTap(float thresh) {
	// 62.5 mg/LSB
	uint8_t rawThresh = SquashLongIntoUint(lrintf(thresh * 16));
	return SetThreshTapRaw(rawThresh);
}

ADXL345::StatusType ADXL345::GetThreshTap(float *thresh) {
	// 62.5 mg/LSB
	StatusType status;
	uint8_t rawThresh;
	status = GetThreshTapRaw(&rawThresh);
	*thresh = float(rawThresh) / 16;
	return status;
}

ADXL345::StatusType ADXL345::SetOffsetRaw(const int8_t offset[3]) {
	return _WriteTo(REG_OFSX, (const uint8_t*)offset, 3);
}

ADXL345::StatusType ADXL345::GetOffsetRaw(int8_t offset[3]) {
	return _ReadFrom(REG_OFSX, (uint8_t*)offset, 3);
}

ADXL345::StatusType ADXL345::SetOffset(const float offset[3]) {
	int8_t raw[3];
	for (uint8_t i=0; i<3; i++) {
		// Assuming two's complement internal integer representation.
		raw[i] = SquashLongIntoInt(lrintf(64 * offset[i]));
	}
	return SetOffsetRaw(raw);
}

ADXL345::StatusType ADXL345::GetOffset(float offset[3]) {
	StatusType status;
	int8_t raw[3];
	status = GetOffsetRaw(raw);
	for (uint8_t i=0; i<3; i++) {
		offset[i] = float(raw[i]) / 64;
	}
	return status;
}

ADXL345::StatusType ADXL345::SetTapDurRaw(uint8_t dur) {
	return _WriteTo(REG_DUR, dur);
}

ADXL345::StatusType ADXL345::GetTapDurRaw(uint8_t *dur) {
	return _ReadFrom(REG_DUR, dur);
}

ADXL345::StatusType ADXL345::SetTapDur(float dur) {
	// 625 μs/LSB
	uint8_t rawDur = SquashLongIntoUint(lrintf(dur * 8/5));
	return SetTapDurRaw(rawDur);
}

ADXL345::StatusType ADXL345::GetTapDur(float *dur) {
	// 625 μs/LSB
	StatusType status;
	uint8_t rawDur;
	status = GetTapDurRaw(&rawDur);
	*dur = float(rawDur) * 5/8;
	return status;
}

ADXL345::StatusType ADXL345::SetTapLatencyRaw(uint8_t latency) {
	return _WriteTo(REG_LATENT, latency);
}

ADXL345::StatusType ADXL345::GetTapLatencyRaw(uint8_t *latency) {
	return _ReadFrom(REG_LATENT, latency);
}

ADXL345::StatusType ADXL345::SetTapLatency(float latency) {
	// 1.25 ms/LSB
	uint8_t rawLatency = SquashLongIntoUint(lrintf(latency * 4/5));
	return SetTapLatencyRaw(rawLatency);
}

ADXL345::StatusType ADXL345::GetTapLatency(float *latency) {
	// 1.25 ms/LSB
	StatusType status;
	uint8_t rawLatency;
	status = GetTapLatencyRaw(&rawLatency);
	*latency = float(rawLatency) * 5/4;
	return status;
}

ADXL345::StatusType ADXL345::SetTapWindowRaw(uint8_t window) {
	return _WriteTo(REG_WINDOW, window);
}

ADXL345::StatusType ADXL345::GetTapWindowRaw(uint8_t *window) {
	return _ReadFrom(REG_WINDOW, window);
}

ADXL345::StatusType ADXL345::SetTapWindow(float window) {
	// 1.25 ms/LSB
	uint8_t rawWindow = SquashLongIntoUint(lrintf(window * 4/5));
	return SetTapWindowRaw(rawWindow);
}

ADXL345::StatusType ADXL345::GetTapWindow(float *window) {
	// 1.25 ms/LSB
	StatusType status;
	uint8_t rawWindow;
	status = GetTapWindowRaw(&rawWindow);
	*window = float(rawWindow) * 5/4;
	return status;
}


ADXL345::StatusType ADXL345::SetThreshActRaw(uint8_t thresh) {
	return _WriteTo(REG_THRESH_ACT, thresh);
}

ADXL345::StatusType ADXL345::GetThreshActRaw(uint8_t *thresh) {
	return _ReadFrom(REG_THRESH_ACT, thresh);
}

ADXL345::StatusType ADXL345::SetThreshAct(float thresh) {
	// 62.5 mg/LSB
	uint8_t rawThresh = SquashLongIntoUint(lrintf(thresh * 16));
	return SetThreshActRaw(rawThresh);
}

ADXL345::StatusType ADXL345::GetThreshAct(float *thresh) {
	// 62.5 mg/LSB
	StatusType status;
	uint8_t rawThresh;
	status = GetThreshActRaw(&rawThresh);
	*thresh = float(rawThresh) / 16;
	return status;
}

ADXL345::StatusType ADXL345::SetThreshInactRaw(uint8_t thresh) {
	return _WriteTo(REG_THRESH_INACT, thresh);
}

ADXL345::StatusType ADXL345::GetThreshInactRaw(uint8_t *thresh) {
	return _ReadFrom(REG_THRESH_INACT, thresh);
}

ADXL345::StatusType ADXL345::SetThreshInact(float thresh) {
	// 62.5 mg/LSB
	uint8_t rawThresh = SquashLongIntoUint(lrintf(thresh * 16));
	return SetThreshInactRaw(rawThresh);
}

ADXL345::StatusType ADXL345::GetThreshInact(float *thresh) {
	// 62.5 mg/LSB
	StatusType status;
	uint8_t rawThresh;
	status = GetThreshInactRaw(&rawThresh);
	*thresh = float(rawThresh) / 16;
	return status;
}

ADXL345::StatusType ADXL345::SetTimeInact(uint8_t time) {
	return _WriteTo(REG_TIME_INACT, time);
}

ADXL345::StatusType ADXL345::GetTimeInact(uint8_t *time) {
	return _ReadFrom(REG_TIME_INACT, time);
}

ADXL345::StatusType ADXL345::SetActInactCtl(uint8_t bitfield) {
	return _WriteTo(REG_ACT_INACT_CTL, bitfield);
}

ADXL345::StatusType ADXL345::GetActInactCtl(uint8_t *bitfield) {
	return _ReadFrom(REG_ACT_INACT_CTL, bitfield);
}

ADXL345::StatusType ADXL345::SetThreshFFRaw(uint8_t thresh) {
	return _WriteTo(REG_THRESH_FF, thresh);
}

ADXL345::StatusType ADXL345::GetThreshFFRaw(uint8_t *thresh) {
	return _ReadFrom(REG_THRESH_FF, thresh);
}

ADXL345::StatusType ADXL345::SetThreshFF(float thresh) {
	// 62.5 mg/LSB
	uint8_t rawThresh = SquashLongIntoUint(lrintf(thresh * 16));
	return SetThreshFFRaw(rawThresh);
}

ADXL345::StatusType ADXL345::GetThreshFF(float *thresh) {
	// 62.5 mg/LSB
	StatusType status;
	uint8_t rawThresh;
	status = GetThreshFFRaw(&rawThresh);
	*thresh = float(rawThresh) / 16;
	return status;
}

ADXL345::StatusType ADXL345::SetTimeFFRaw(uint8_t time) {
	return _WriteTo(REG_TIME_FF, time);
}

ADXL345::StatusType ADXL345::GetTimeFFRaw(uint8_t *time) {
	return _ReadFrom(REG_TIME_FF, time);
}

ADXL345::StatusType ADXL345::SetTimeFF(unsigned time_ms) {
	const unsigned max_time_ms = 1275;
	uint8_t rawTime;
	if (time_ms >= max_time_ms) {
		rawTime = 0xFF;
	}
	else {
		rawTime = time_ms / 5;
		if (time_ms % 5 >= 3)
		{ rawTime++; }
	}
	return SetTimeFFRaw(rawTime);
}

ADXL345::StatusType ADXL345::GetTimeFF(unsigned *time_ms) {
	StatusType status;
	uint8_t rawTime;
	status = GetTimeFFRaw(&rawTime);
	*time_ms = unsigned(rawTime) * 5;
	return status;
}


ADXL345::StatusType ADXL345::SetTapAxes(uint8_t bitfield) {
	return _WriteTo(REG_TAP_AXES, bitfield);
}

ADXL345::StatusType ADXL345::GetTapAxes(uint8_t *bitfield) {
	return _ReadFrom(REG_TAP_AXES, bitfield);
}

ADXL345::StatusType ADXL345::GetActTapStatus(uint8_t *bitfield) {
	return _ReadFrom(REG_ACT_TAP_STATUS, bitfield);
}

ADXL345::StatusType ADXL345::GetAsleep(bool *asleep) {
	StatusType status;
	uint8_t bitfield;
	status = GetActTapStatus(&bitfield);
	*asleep = (bitfield >> BIT_ACT_TAP_STATUS_ASLEEP) & 1;
	return status;
}

ADXL345::StatusType ADXL345::SetBwRate(uint8_t bitfield) {
	return _WriteTo(REG_BW_RATE, bitfield);
}

ADXL345::StatusType ADXL345::GetBwRate(uint8_t *bitfield) {
	return _ReadFrom(REG_BW_RATE, bitfield);
}

ADXL345::StatusType ADXL345::SetLowPower(bool lowPower) {
	StatusType status;
	uint8_t bwRateVal;
	status = _ReadFrom(REG_BW_RATE, &bwRateVal);
	if (status) { return status; }
	bwRateVal =
	(
		bwRateVal & ~(1 << BIT_BW_RATE_LOW_POWER)
	) | (
		lowPower << BIT_BW_RATE_LOW_POWER
	);
	return _WriteTo(REG_BW_RATE, bwRateVal);
}

ADXL345::StatusType ADXL345::GetLowPower(bool *lowPower) {
	StatusType status;
	uint8_t bwRateVal;
	status = _ReadFrom(REG_BW_RATE, &bwRateVal);
	*lowPower = (bwRateVal >> BIT_BW_RATE_LOW_POWER) & 1;
	return status;
}

ADXL345::StatusType ADXL345::SetRate(uint8_t rate) {
	StatusType status;
	uint8_t bwRateVal;
	status = _ReadFrom(REG_BW_RATE, &bwRateVal);
	if (status) { return status; }
	bwRateVal = (bwRateVal & (1<<BIT_BW_RATE_LOW_POWER)) | rate;
	return _WriteTo(REG_BW_RATE, bwRateVal);
}

ADXL345::StatusType ADXL345::GetRate(uint8_t *rate) {
	const uint8_t rateMask = 0x0F;
	StatusType status;
	uint8_t bwRateVal;
	status = _ReadFrom(REG_BW_RATE, &bwRateVal);
	*rate = bwRateVal & rateMask;
	return status;
}

ADXL345::StatusType ADXL345::SetPowerCtl(uint8_t bitfield) {
	return _WriteTo(REG_POWER_CTL, bitfield);
}

ADXL345::StatusType ADXL345::GetPowerCtl(uint8_t *bitfield) {
	return _ReadFrom(REG_POWER_CTL, bitfield);
}

ADXL345::StatusType ADXL345::SetLink(bool link) {
	StatusType status;
	uint8_t powerCtlVal;
	status = GetPowerCtl(&powerCtlVal);
	if (status) { return status; }
	powerCtlVal =
	(
		powerCtlVal & ~(1 << BIT_POWER_CTL_LINK)
	) | (
		link << BIT_POWER_CTL_LINK
	);
	return SetPowerCtl(powerCtlVal);
}

ADXL345::StatusType ADXL345::GetLink(bool *link) {
	StatusType status;
	uint8_t powerCtlVal;
	status = GetPowerCtl(&powerCtlVal);
	*link = (powerCtlVal >> BIT_POWER_CTL_LINK) & 1;
	return status;
}

ADXL345::StatusType ADXL345::SetAutoSleep(bool autoSleep) {
	StatusType status;
	uint8_t powerCtlVal;
	status = GetPowerCtl(&powerCtlVal);
	if (status) { return status; }
	powerCtlVal =
	(
		powerCtlVal & ~(1 << BIT_POWER_CTL_AUTO_SLEEP)
	) | (
		autoSleep << BIT_POWER_CTL_AUTO_SLEEP
	);
	return SetPowerCtl(powerCtlVal);
}

ADXL345::StatusType ADXL345::GetAutoSleep(bool *autoSleep) {
	StatusType status;
	uint8_t powerCtlVal;
	status = GetPowerCtl(&powerCtlVal);
	*autoSleep = (powerCtlVal >> BIT_POWER_CTL_AUTO_SLEEP) & 1;
	return status;
}

ADXL345::StatusType ADXL345::SetMeasure(bool measure) {
	StatusType status;
	uint8_t powerCtlVal;
	status = GetPowerCtl(&powerCtlVal);
	if (status) { return status; }
	powerCtlVal =
	(
		powerCtlVal & ~(1 << BIT_POWER_CTL_MEASURE)
	) | (
		measure << BIT_POWER_CTL_MEASURE
	);
	return SetPowerCtl(powerCtlVal);
}

ADXL345::StatusType ADXL345::GetMeasure(bool *measure) {
	StatusType status;
	uint8_t powerCtlVal;
	status = GetPowerCtl(&powerCtlVal);
	*measure = (powerCtlVal >> BIT_POWER_CTL_MEASURE) & 1;
	return status;
}

ADXL345::StatusType ADXL345::SetSleep(bool sleep) {
	StatusType status;
	uint8_t powerCtlVal;
	status = GetPowerCtl(&powerCtlVal);
	if (status) { return status; }
	powerCtlVal =
	(
		powerCtlVal & ~(1 << BIT_POWER_CTL_SLEEP)
	) | (
		sleep << BIT_POWER_CTL_SLEEP
	);
	return SetPowerCtl(powerCtlVal);
}

ADXL345::StatusType ADXL345::GetSleep(bool *sleep) {
	StatusType status;
	uint8_t powerCtlVal;
	status = GetPowerCtl(&powerCtlVal);
	*sleep = (powerCtlVal >> BIT_POWER_CTL_SLEEP) & 1;
	return status;
}

ADXL345::StatusType ADXL345::SetWakeup(uint8_t wakeup) {
	const uint8_t wakeupMask = 0x03;
	StatusType status;
	uint8_t powerCtlVal;
	status = GetPowerCtl(&powerCtlVal);
	if (status) { return status; }
	powerCtlVal =
	(
		powerCtlVal & ~wakeupMask
	) | (
		wakeup
	);
	return SetPowerCtl(powerCtlVal);
}

ADXL345::StatusType ADXL345::GetWakeup(uint8_t *wakeup) {
	const uint8_t wakeupMask = 0x03;
	StatusType status;
	uint8_t powerCtlVal;
	status = GetPowerCtl(&powerCtlVal);
	*wakeup = (powerCtlVal & wakeupMask);
	return status;
}

ADXL345::StatusType ADXL345::SetIntEnable(uint8_t bitfield) {
	return _WriteTo(REG_INT_ENABLE, bitfield);
}

ADXL345::StatusType ADXL345::GetIntEnable(uint8_t *bitfield) {
	return _ReadFrom(REG_INT_ENABLE, bitfield);
}

ADXL345::StatusType ADXL345::SetIntMap(uint8_t bitfield) {
	return _WriteTo(REG_INT_MAP, bitfield);
}

ADXL345::StatusType ADXL345::GetIntMap(uint8_t *bitfield) {
	return _ReadFrom(REG_INT_MAP, bitfield);
}

ADXL345::StatusType ADXL345::GetIntSource(uint8_t *bitfield) {
	return _ReadFrom(REG_INT_SOURCE, bitfield);
}

ADXL345::StatusType ADXL345::SetDataFormat(uint8_t bitfield) {
	StatusType status = _WriteTo(REG_DATA_FORMAT, bitfield);
	if (!status) { _dataFormat = bitfield; }
	return status;
}

ADXL345::StatusType ADXL345::RefreshDataFormat() {
	return _ReadFrom(REG_DATA_FORMAT, &_dataFormat);
}

void ADXL345::GetDataFormat(uint8_t *bitfield) {
	*bitfield = _dataFormat;
}

ADXL345::StatusType ADXL345::SetSelfTest(bool selfTest) {
	uint8_t bitfield = (
		_dataFormat & ~(1 << BIT_DATA_FORMAT_SELF_TEST)
	) | (
		(selfTest << BIT_DATA_FORMAT_SELF_TEST)
	);
	return SetDataFormat(bitfield);
}

void ADXL345::GetSelfTest(bool *selfTest) {
	*selfTest = _SelfTest();
}

ADXL345::StatusType ADXL345::SetSPI3Wire(bool spi3wire) {
	uint8_t bitfield = (
		_dataFormat & ~(1 << BIT_DATA_FORMAT_SPI_3WIRE)
	) | (
		(spi3wire << BIT_DATA_FORMAT_SPI_3WIRE)
	);
	return SetDataFormat(bitfield);
}

void ADXL345::GetSPI3Wire(bool *spi3wire) {
	*spi3wire = _SPI3Wire();
}

ADXL345::StatusType ADXL345::SetIntActiveLow(bool intActiveLow) {
	uint8_t bitfield = (
		_dataFormat & ~(1 << BIT_DATA_FORMAT_INT_INVERT)
	) | (
		(intActiveLow << BIT_DATA_FORMAT_INT_INVERT)
	);
	return SetDataFormat(bitfield);
}

void ADXL345::GetIntActiveLow(bool *intActiveLow) {
	*intActiveLow = _IntActiveLow();
}

ADXL345::StatusType ADXL345::SetFullRes(bool fullRes) {
	uint8_t bitfield = (
		_dataFormat & ~(1 << BIT_DATA_FORMAT_FULL_RES)
	) | (
		(fullRes << BIT_DATA_FORMAT_FULL_RES)
	);
	return SetDataFormat(bitfield);
}

void ADXL345::GetFullRes(bool *fullRes) {
	*fullRes = _FullRes();
}

ADXL345::StatusType ADXL345::SetLeftJustify(bool leftJustify) {
	uint8_t bitfield = (
		_dataFormat & ~(1 << BIT_DATA_FORMAT_JUSTIFY_LEFT)
	) | (
		(leftJustify << BIT_DATA_FORMAT_JUSTIFY_LEFT)
	);
	return SetDataFormat(bitfield);
}

void ADXL345::GetLeftJustify(bool *leftJustify) {
	*leftJustify = _LeftJustify();
}

//	Expects VAL_RANGE_x as argument
ADXL345::StatusType ADXL345::SetRange(uint8_t range) {
	const uint8_t rangeMask = 0x03; // Mask two least significant bits
	uint8_t bitfield = (
		_dataFormat & ~rangeMask
	) | (
		range
	);
	return SetDataFormat(bitfield);
}

void ADXL345::GetRange(uint8_t *range) {
	*range = _Range();
}

ADXL345::StatusType ADXL345::GetDataRaw(int16_t data[3]) {
	const bool fullRes		= _FullRes();
	const bool leftJustify	= _LeftJustify();
	const uint8_t range		= _Range();
	StatusType status;
	uint8_t buffer[6];
	status = _ReadFrom(REG_DATAX0, buffer, 6);
	if (status) { return status; }
	for (uint8_t i=0; i<3; i++) {
		data[i] = buffer[2*i] + (int8_t(buffer[2*i+1]) * 256);
		if (leftJustify) {
			uint8_t divisor = 64;
			if (fullRes) {
				divisor >>= range;
			}
			data[i] /= divisor;
		}
	}
	return StatusType(0);
}


ADXL345::StatusType ADXL345::GetData(float data[3]) {
	const bool fullRes	= _FullRes();
	const uint8_t range	= _Range();
	StatusType status;
	int16_t raw[3];
	status = GetDataRaw(raw);
	if (status) { return status; }
	for (uint8_t i=0; i<3; i++) {
		data[i] = _gain[i] * raw[i] / 256;
		if (!fullRes) { data[i] *= (1 << range); }
	}
	return StatusType(0);
}

ADXL345::StatusType ADXL345::SetFifoCtl(uint8_t bitfield) {
	return _WriteTo(REG_FIFO_CTL, bitfield);
}

ADXL345::StatusType ADXL345::GetFifoCtl(uint8_t *bitfield) {
	return _ReadFrom(REG_FIFO_CTL, bitfield);
}

ADXL345::StatusType ADXL345::SetFifoMode(uint8_t mode) {
	const uint8_t modeMask = 0xC0; // bit 7 and 6
	StatusType status;
	uint8_t bitfield;
	status = GetFifoCtl(&bitfield);
	if (status) { return status; }
	bitfield = (
		bitfield & ~modeMask
	) | (
		mode << BIT_FIFO_CTL_MODE_LSB
	);
	return SetFifoCtl(bitfield);
}

ADXL345::StatusType ADXL345::GetFifoMode(uint8_t *mode) {
	StatusType status;
	uint8_t bitfield;
	status = GetFifoCtl(&bitfield);
	*mode = bitfield >> BIT_FIFO_CTL_MODE_LSB;
	return status;
}

ADXL345::StatusType ADXL345::SetFifoTriggerInt2(bool triggerInt2) {
	StatusType status;
	uint8_t bitfield;
	status = GetFifoCtl(&bitfield);
	if (status) { return status; }
	bitfield = (
		bitfield & ~(1 << BIT_FIFO_CTL_TRIGGER_INT2)
	) | (
		triggerInt2 << BIT_FIFO_CTL_TRIGGER_INT2
	);
	return SetFifoCtl(bitfield);
}

ADXL345::StatusType ADXL345::GetFifoTriggerInt2(bool *triggerInt2) {
	StatusType status;
	uint8_t bitfield;
	status = GetFifoCtl(&bitfield);
	*triggerInt2 = (bitfield >> BIT_FIFO_CTL_TRIGGER_INT2) & 1;
	return status;
}

ADXL345::StatusType ADXL345::SetFifoSamples(uint8_t samples) {
	const uint8_t samplesMask = 0x1F;
	StatusType status;
	uint8_t bitfield;
	status = GetFifoCtl(&bitfield);
	if (status) { return status; }
	bitfield = (
		bitfield & ~samplesMask
	) | (
		samples
	);
	return SetFifoCtl(bitfield);
}

ADXL345::StatusType ADXL345::GetFifoSamples(uint8_t *samples) {
	const uint8_t samplesMask = 0x1F;
	StatusType status;
	uint8_t bitfield;
	status = GetFifoCtl(&bitfield);
	*samples = bitfield & samplesMask;
	return status;
}

ADXL345::StatusType ADXL345::GetFifoStatus(uint8_t *fifoStatus) {
	return _ReadFrom(REG_FIFO_STATUS, fifoStatus);
}

ADXL345::StatusType ADXL345::GetFifoTrig(bool *fifoTrig) {
	StatusType status;
	uint8_t bitfield;
	status = GetFifoStatus(&bitfield);
	*fifoTrig = bitfield >> BIT_FIFO_STATUS_FIFO_TRIG;
	return status;
}

ADXL345::StatusType ADXL345::GetFifoEntries(uint8_t *entries) {
	const uint8_t entriesMask = 0x3F;
	StatusType status;
	uint8_t bitfield;
	status = GetFifoStatus(&bitfield);
	*entries = bitfield & entriesMask;
	return status;
}

ADXL345::StatusType ADXL345_I2C::_WriteTo(uint8_t reg, uint8_t val) {
	uint8_t data[2];
	data[0] = reg;
	data[1] = val;
	return HAL_I2C_Master_Transmit(_hi2c, _devAddr, data, 2, COM_TIMEOUT);
}

ADXL345::StatusType ADXL345_I2C::_WriteTo(uint8_t reg, const uint8_t data[], uint8_t n) {
	uint8_t dataWithReg[BUFFER_MAX+1];
	dataWithReg[0] = reg;
	for (uint8_t i=0; i<n; i++) {
		dataWithReg[i+1] = data[i];
	}
	return HAL_I2C_Master_Transmit(_hi2c, _devAddr, dataWithReg, n+1, COM_TIMEOUT);
}

ADXL345::StatusType ADXL345_I2C::_ReadFrom(uint8_t reg, uint8_t *val) {
	StatusType status;
	status = HAL_I2C_Master_Transmit(_hi2c, _devAddr, &reg, 1, COM_TIMEOUT);
	if (!status) {
		status = HAL_I2C_Master_Receive(_hi2c, _devAddr, val, 1, COM_TIMEOUT);
	}
	return status;
}

ADXL345::StatusType ADXL345_I2C::_ReadFrom(uint8_t reg, uint8_t data[], uint8_t n) {
	StatusType status;
	status = HAL_I2C_Master_Transmit(_hi2c, _devAddr, &reg, 1, COM_TIMEOUT);
	if (!status) {
		status = HAL_I2C_Master_Receive(_hi2c, _devAddr, data, n, COM_TIMEOUT);
	}
	return status;
}

ADXL345::StatusType ADXL345_SPI::_WriteTo(uint8_t reg, uint8_t val) {
	StatusType status;
	uint8_t data[2];
	data[0] = reg;
	data[1] = val;
	const bool read	=	false;
	const bool mb	=	false;	// multibyte
	data[0] |= (read << 7) | (mb << 6);
	HAL_GPIO_WritePin(_ssPort, _ssPin, GPIO_PIN_RESET);
	status = HAL_SPI_Transmit(_hspi, data, 2, COM_TIMEOUT);
	HAL_GPIO_WritePin(_ssPort, _ssPin, GPIO_PIN_SET);
	return status;
}

ADXL345::StatusType ADXL345_SPI::_WriteTo(uint8_t reg, const uint8_t data[], uint8_t n) {
	StatusType status;
	uint8_t dataWithReg[BUFFER_MAX+1];
	dataWithReg[0] = reg;
	for (uint8_t i=0; i<n; i++) {
		dataWithReg[i+1] = data[i];
	}
	const bool read	=	false;
	const bool mb	=	true;	// multibyte
	dataWithReg[0] |= (read << 7) | (mb << 6);
	HAL_GPIO_WritePin(_ssPort, _ssPin, GPIO_PIN_RESET);
	status = HAL_SPI_Transmit(_hspi, dataWithReg, n+1, COM_TIMEOUT);
	HAL_GPIO_WritePin(_ssPort, _ssPin, GPIO_PIN_SET);
	return status;
}

ADXL345::StatusType ADXL345_SPI::_ReadFrom(uint8_t reg, uint8_t *val) {
	StatusType status;
	const bool read	=	true;
	const bool mb	=	false;	// multibyte
	reg |= (read << 7) | (mb << 6);
	HAL_GPIO_WritePin(_ssPort, _ssPin, GPIO_PIN_RESET);
	status = HAL_SPI_Transmit(_hspi, &reg, 1, COM_TIMEOUT);
	if (!status) {
		status = HAL_SPI_Receive(_hspi, val, 1, COM_TIMEOUT);
	}
	HAL_GPIO_WritePin(_ssPort, _ssPin, GPIO_PIN_SET);
	return status;
}

ADXL345::StatusType ADXL345_SPI::_ReadFrom(uint8_t reg, uint8_t data[], uint8_t n) {
	StatusType status;
	const bool read	=	true;
	const bool mb	=	true;	// multibyte
	reg |= (read << 7) | (mb << 6);
	HAL_GPIO_WritePin(_ssPort, _ssPin, GPIO_PIN_RESET);
	status = HAL_SPI_Transmit(_hspi, &reg, 1, COM_TIMEOUT);
	if (!status) {
		status = HAL_SPI_Receive(_hspi, data, n, COM_TIMEOUT);
	}
	HAL_GPIO_WritePin(_ssPort, _ssPin, GPIO_PIN_SET);
	return status;
}
