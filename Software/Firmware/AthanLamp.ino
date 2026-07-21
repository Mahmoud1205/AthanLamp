/**
 * This is the first working version of the Athan Lamp firmware.
 *
 * References:
 * 1. https://praytimes.org/docs/calculation
 * 2. https://github.com/zarrabi/praytime/blob/main/src/praytime.js
 * 3. https://bu.edu.eg/portal/uploads/Engineering,%20Shoubra/Surveying%20Engineering/3666/publications/Saad%20Zaki%20Bolbol_pdf.pdf
 * 4. https://quasar.as.utexas.edu/BillInfo/JulianDatesG.html
 */

/**
 * TODO:
 * fix LED switch delay
 * 	replace alarm sound with athan sound
 *		add EN25Q32 code
 * clean the code, organize it
 */

#include <Wire.h>
#include <RTClib.h>
#include <stdio.h>
#include <math.h>

typedef int32_t I32;
typedef uint32_t U32;
typedef float F32;

struct AstroInfo
{
	F32 sunDeclination;
	F32 eqt;
};

struct GregorianDate
{
	I32 year;
	I32 month;
	I32 day;
	I32 hour;
};

typedef F32 JulianDate;

/**
 * @brief Note: All values are hours into the day until the athan respecting
 * the current timezone.
 */
struct AthanTimes
{
	F32 duhr;
	F32 asr;
	F32 maghrib;
	F32 isha;
	F32 fajr;
};

// cairo approx coordinates
const F32 lat = 30.033333f;
const F32 lon = 31.233334f;

const F32 timezone = 3.0f; // GMT+3

/// @brief Sun degrees below the horizon to be considered sunrise or sunset.
#define SUNSET_CONST (0.833f)

/**
 * @brief Egyptian General Authority of Survey sun angle below the horizon
 * until it is considerde fajr.
 */
#define EGAS_FAJR_ANGLE (19.5f)
 /**
  * @brief Egyptian General Authority of Survey sun angle below the horizon
  * until it is considered ishaa.
  */
#define EGAS_ISHA_ANGLE (17.5f)

#define DEG2RAD(inX) ((inX) * (M_PI / 180.0f))
#define RAD2DEG(inX) ((inX) * (180.0f / M_PI))

// trig functions in degrees. they are needed because the algorithms in
// ref 1 and 2 expect degrees but trig functions in math.h expect radians.
inline F32 sind(F32 inX)			{ return sinf(DEG2RAD(inX));		}
inline F32 cosd(F32 inX)			{ return cosf(DEG2RAD(inX));		}
inline F32 tand(F32 inX)			{ return tanf(DEG2RAD(inX));		}
inline F32 asind(F32 inX)			{ return RAD2DEG(asinf(inX));		}
inline F32 acosd(F32 inX)			{ return RAD2DEG(acosf(inX));		}
inline F32 atand(F32 inX)			{ return RAD2DEG(atanf(inX));		}
inline F32 atan2d(F32 inY, F32 inX)	{ return RAD2DEG(atan2f(inY, inX));	}
inline F32 acotd(F32 inX)			{ return atan2d(1.0f, inX);			}

/// @brief Acts the same as JS mod(). See ref 2.
inline F32 mod(F32 inX, F32 inY)
{
	F32 r = fmodf(inX, inY);
	return (r < 0) ? r + inY : r;
}

void getAstroInfo(AstroInfo* ioInfo, F32 inJulianDate)
{
	// see ref 1 and 2

	const F32 d = inJulianDate - 2451545.0f;

	const F32 g = mod(357.529f + 0.98560028f * d, 360.0f);
	const F32 q = mod(280.459f + 0.98564736f * d, 360.0f);
	const F32 L = mod(q + 1.915f * sind(g) + 0.020f * sind(2.0f * g), 360.0f);
	const F32 e = 23.439f - 0.00000036f * d;
		  F32 RA = mod(atan2d(cosd(e) * sind(L), cosd(L)) / 15.0f, 24.0f);

	ioInfo->sunDeclination = asind(sind(e) * sind(L));
	ioInfo->eqt = q / 15.0f - RA;
	
	/**
	 * HACK: as the year progresses: RA increases by around 1 per day until it reaches
	 * its maximum of around 24 then drops to around 0 and keeps repeating the pattern.
	 * but in the few days near which RA reaches zero and starts to repeat its pattern,
	 * the EqT increases by exactly 24 hours. so to fix this we just check if the EqT
	 * became higher than 23 and if it does we subtract 24 from it to bring it back to
	 * the normal range. i don't know why this happens and it's probably floating point
	 * inaccuracy but i just keep this hack because it works. i tested this fix with
	 * athan times over 100 years and they looked stable so i will keep it.
	 */
	if (ioInfo->eqt > 23.0f)
		ioInfo->eqt -= 24.0f;
}

/// @brief The time taken from mid day until the sun reaches an angle in degrees below the horizon.
F32 getTimeFromMidDayToAngle(const AstroInfo* ioInfo, F32 inAngle)
{
	// see ref 1

	F32 D = ioInfo->sunDeclination;

	F32 numerator = -sind(inAngle) - (sind(lat) * sind(D));
	F32 denominator = cosd(lat) * cosd(D);
	F32 t = (1.0f / 15.0f) * acosd(numerator / denominator);

	return t;
}

/// @brief The time taken from solar noon until an object's shadow is the same as its length.
F32 getTimeFromNoonToShadowLength(const AstroInfo* ioInfo)
{
	// see ref 1

	F32 D = ioInfo->sunDeclination;

	F32 numerator = sind(acotd(1.0f + tand(fabsf(lat - D)))) - sind(lat) * sind(D);
	F32 denominator = cosd(lat) * cosd(D);
	F32 t = (1.0f / 15.0f) * acosd(numerator / denominator);

	return t;
}

char* timeToStr(char ioStr[12], F32 inT)
{
	I32 totalMinutes = (I32)(inT * 60.0f + 0.5f);
	I32 hour24 = totalMinutes / 60;
	I32 minutes = totalMinutes % 60;

	I32 hour12 = hour24 % 12;
	if (hour12 == 0)
		hour12 = 12;

	char* amOrPm = (hour24 >= 12) ? "PM" : "AM";

	I32 i = 0;
	if (hour12 >= 10)
				ioStr[i++] = '0' + (hour12 / 10);

	ioStr[i++] = '0' + (hour12 % 10);
	ioStr[i++] = ':';
	ioStr[i++] = '0' + (minutes / 10);
	ioStr[i++] = '0' + (minutes % 10);
	ioStr[i++] = amOrPm[0];
	ioStr[i++] = amOrPm[1];
	ioStr[i]   = '\0';

	return ioStr;
}

/// @param inHour In UTC.
JulianDate getJulianDate(const GregorianDate* inTime)
{
	// see ref 4 for detailed conversion explaination

	F32 y = (F32)inTime->year;
	F32 m = (F32)inTime->month;
	F32 d = (F32)inTime->day;
	F32 h = (F32)inTime->hour;

	d += (h / 24.0f);

	if (m <= 2)
	{
		y -= 1.0f;
		m += 12.0f;
	}

	// ints are used in this part of the code to discard the fraction parts
	// of all calculations
	I32 a = (I32)y / 100;
	I32 b = a / 4;
	I32 c = 2 - a + b;
	I32 e = (I32)(365.25f * (y + 4716.0f));
	I32 f = (I32)(30.6001f * (m + 1.0f));

	JulianDate jd = (F32)c + (F32)d + (F32)e + (F32)f - 1524.5f;

	return jd;
}

void getAthanTimes(AthanTimes* ioTimes, JulianDate inDate)
{
	AstroInfo ai = { 0 };
	getAstroInfo(&ai, inDate);
	F32 EqT = ai.eqt;

	ioTimes->duhr = 12.0f + timezone - (lon / 15.0f) - EqT;
	ioTimes->asr = ioTimes->duhr + getTimeFromNoonToShadowLength(&ai);
	// 1m is added to maghrib and isha for safety
	// TODO: consider 15s or 30s instead of 60s
	ioTimes->maghrib = ioTimes->duhr + getTimeFromMidDayToAngle(&ai, SUNSET_CONST) + (1.0f / 60.0f);
	ioTimes->isha = ioTimes->duhr + getTimeFromMidDayToAngle(&ai, EGAS_ISHA_ANGLE) + (1.0f / 60.0f);
	ioTimes->fajr = ioTimes->duhr - getTimeFromMidDayToAngle(&ai, EGAS_FAJR_ANGLE);
}

#define LED_STRIP A1
#define LIGHT_SWITCH A2
#define FW_LED A3
#define SPEAKER 9

RTC_DS3231 gRTC;
AthanTimes gTimes = { 0 };

void setup()
{
	Serial.begin(9600);
	Wire.begin();

	pinMode(LED_STRIP, OUTPUT);
	pinMode(LIGHT_SWITCH, INPUT_PULLUP);
	pinMode(FW_LED, OUTPUT);
	pinMode(SPEAKER, OUTPUT);

	if (!gRTC.begin())
	{
		while (1)
		{
			Serial.println("Error: RTC not found!");
			digitalWrite(FW_LED, HIGH);
			delay(100);
			digitalWrite(FW_LED, LOW);
			delay(100);
			digitalWrite(FW_LED, HIGH);
			delay(100);
			digitalWrite(FW_LED, LOW);
			delay(600);
		}
	}

	Serial.println("RTC initialized.");

	digitalWrite(FW_LED, HIGH);

	// NOTE: uncomment this once to calibrate the RTC
	// using unix time (epoch) in seconds
	// gRTC.adjust(1782655335);
}

bool fajrPlayed = false;
bool duhrPlayed = false;
bool asrPlayed = false;
bool maghribPlayed = false;
bool ishaPlayed = false;

void loop()
{
	if (digitalRead(LIGHT_SWITCH) == LOW)
	{
		Serial.println("LED ON");
		digitalWrite(LED_STRIP, HIGH);
	} else
	{
		digitalWrite(LED_STRIP, LOW);
	}

	DateTime now = gRTC.now() + TimeSpan(0, (int8_t)timezone, 0, 0);

	// the hardcoding of 12PM as the hour is not a mistake, it is intentionally
	// like this because astronomical calculations are the most accurate around
	// solar noon. this doesnt effect the athan times because they will be accurate
	// regardless of whether the gregorian date hour is exactly the current
	// hour or not.
	GregorianDate curTimeGregorian = {
		.year = (I32)now.year(),
		.month = (I32)now.month(),
		.day = (I32)now.day(),
		.hour = 12
	};
	const JulianDate curJD = getJulianDate(&curTimeGregorian);

	AstroInfo ai = { 0 };
	const JulianDate jd = curJD;
	getAstroInfo(&ai, jd);
	getAthanTimes(&gTimes, jd);

	F32 curHoursIntoDay = (F32)now.hour() + (((F32)now.minute()) / 60.0f) + (((F32)now.second()) / 3600.0f);

	// TODO: clean this part; duplicate code

	if (!fajrPlayed && fabsf(gTimes.fajr - curHoursIntoDay) < 5.0f / 3600.0f)
	{
		fajrPlayed = true;

		// play 7 beeps
		for (int i = 0; i < 7; i++)
		{
			tone(SPEAKER, 440);
			delay(1000);
			noTone(SPEAKER);
			delay(1000);
		}
	}

	if (!duhrPlayed && fabsf(gTimes.duhr - curHoursIntoDay) < 5.0f / 3600.0f)
	{
		duhrPlayed = true;

		// play 7 beeps
		for (int i = 0; i < 7; i++)
		{
			tone(SPEAKER, 440);
			delay(1000);
			noTone(SPEAKER);
			delay(1000);
		}
	}

	if (!asrPlayed && fabsf(gTimes.asr - curHoursIntoDay) < 5.0f / 3600.0f)
	{
		asrPlayed = true;

		// play 7 beeps
		for (int i = 0; i < 7; i++)
		{
			tone(SPEAKER, 440);
			delay(1000);
			noTone(SPEAKER);
			delay(1000);
		}
	}

	if (!maghribPlayed && fabsf(gTimes.maghrib - curHoursIntoDay) < 5.0f / 3600.0f)
	{
		maghribPlayed = true;

		// play 7 beeps
		for (int i = 0; i < 7; i++)
		{
			tone(SPEAKER, 440);
			delay(1000);
			noTone(SPEAKER);
			delay(1000);
		}
	}

	if (!ishaPlayed && fabsf(gTimes.isha - curHoursIntoDay) < 5.0f / 3600.0f)
	{
		ishaPlayed = true;

		// play 7 beeps
		for (int i = 0; i < 7; i++)
		{
			tone(SPEAKER, 440);
			delay(1000);
			noTone(SPEAKER);
			delay(1000);
		}
	}

	// TODO: review or clean this part
	// reset flags at astronomical noon
	if (now.hour() == 12 && fajrPlayed)
		fajrPlayed = false;
	// duhr especially resets at night because duhr is at noon time
	if (now.hour() == 3 && duhrPlayed)
		duhrPlayed = false;
	if (now.hour() == 12 && asrPlayed)
		asrPlayed = false;
	if (now.hour() == 12 && maghribPlayed)
		maghribPlayed = false;
	if (now.hour() == 12 && ishaPlayed)
		ishaPlayed = false;

	// wait 1s before next update and blink the blue firmware
	// LED so you can see the code is working and updating
	delay(500);
	digitalWrite(FW_LED, LOW);
	delay(500);
	digitalWrite(FW_LED, HIGH);
}
