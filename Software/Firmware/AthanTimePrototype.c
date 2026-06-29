/**
 * This is the first working prototype of the offline athan time algorithm according to the Egyptian method.
 * It can be easily edited for other country's method.
 * This will be the foundation of the firmware that will be written for the ATmega328P MCU when I start to write it.
 *
 * References:
 * 1. https://praytimes.org/docs/calculation
 * 2. https://github.com/zarrabi/praytime/blob/main/src/praytime.js
 * 3. https://bu.edu.eg/portal/uploads/Engineering,%20Shoubra/Surveying%20Engineering/3666/publications/Saad%20Zaki%20Bolbol_pdf.pdf
 * 4. https://quasar.as.utexas.edu/BillInfo/JulianDatesG.html
 */

#include <stdio.h>
#include <math.h>

typedef int I32;
typedef unsigned int U32;
typedef float F32;

// cairo approx coordinates
const F32 lat = 30.033333f;
const F32 lon = 31.233334f;

// TODO: account for DST
const F32 timezone = 2.0f; // GMT+2

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

typedef struct
{
	F32 sunDeclination;
	F32 eqt;

	// TODO: the below members are for debugging only, remove in prod
	F32 g;
	F32 q;
	F32 L;
	F32 e;
	F32 RA;
} AstroInfo;

typedef struct
{
	I32 year;
	I32 month;
	I32 day;
	I32 hour;
} GregorianDate;

typedef F32 JulianDate;

/**
 * @brief Note: All values are hours into the day until the athan respecting
 * the current timezone.
 */
typedef struct
{
	F32 duhr;
	F32 asr;
	F32 maghrib;
	F32 isha;
	F32 fajr;
} AthanTimes;

U32 aiCount = 0;
void getAstroInfo(AstroInfo* ioInfo, F32 inJulianDate)
{
	// see ref 1 and 2

	const F32 d = inJulianDate - 2451545.0f;

	const F32 g = mod(357.529f + 0.98560028f * d, 360.0f);
	const F32 q = mod(280.459f + 0.98564736f * d, 360.0f);
	const F32 L = mod(q + 1.915f * sind(g) + 0.020f * sind(2.0f * g), 360.0f);
	const F32 e = 23.439f - 0.00000036 * d;
	      F32 RA = mod(atan2d(cosd(e) * sind(L), cosd(L)) / 15.0f, 24.0f);

	ioInfo->sunDeclination = asind(sind(e) * sind(L));
	ioInfo->eqt = q / 15.0f - RA;

	ioInfo->g = g;
	ioInfo->q = q;
	ioInfo->L = L;
	ioInfo->e = e;
	ioInfo->RA = RA;
	
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

	//if (ioInfo->eqt > 23.0f)
	//	puts("\033[41mBAD ASTRO INFO:");
	//else
	//	puts("\033[42mGOOD ASTRO INFO:");

	//printf("i:   %u\n", aiCount++);
	//printf("d:   %.0f\n", d);
	//printf("g:   %.2f\n", g);
	//printf("q:   %.2f\n", q);
	//printf("L:   %.2f\n", L);
	//printf("e:   %.2f\n", e);
	//printf("RA:  %.2f\n", RA);
	//printf("D:   %.2f\n", ioInfo->sunDeclination);
	//printf("EqT: %.2f\n", ioInfo->eqt);

	//puts("\033[0m");
	//puts("");
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

char* timeToStr(char ioStr[8], F32 inT)
{
	I32 hour24 = (I32)inT;
	I32 minutes = (I32)roundf((inT - hour24) * 60.0f);

	if (minutes == 60)
	{
		minutes = 0;
		hour24 += 1;
	}

	I32 hour12 = hour24 % 12;
	if (hour12 == 0)
		hour12 = 12;

	char* amOrPm = (hour24 >= 12) ? "PM" : "AM";

	snprintf(ioStr, 8, "%d:%02d%s", hour12, minutes, amOrPm);
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

	// 1m is added to maghrib and isha for safety
	ioTimes->duhr = 12.0f + timezone - (lon / 15.0) - EqT;
	ioTimes->asr = ioTimes->duhr + getTimeFromNoonToShadowLength(&ai);
	ioTimes->maghrib = ioTimes->duhr + getTimeFromMidDayToAngle(&ai, SUNSET_CONST) + (1.0f / 60.0f);
	ioTimes->isha = ioTimes->duhr + getTimeFromMidDayToAngle(&ai, EGAS_ISHA_ANGLE) + (1.0f / 60.0f);
	ioTimes->fajr = ioTimes->duhr - getTimeFromMidDayToAngle(&ai, EGAS_FAJR_ANGLE);
}

I32 main(void)
{
	// TODO: should the julian date be updated by the day at noon or by the hour ?
	// if by the day, hardcode .hour to noon (12)
	GregorianDate curTimeGregorian = {
		.year = 2026,
		.month = 4,
		.day = 11,
		.hour = 12
	};
	const JulianDate curJD = getJulianDate(&curTimeGregorian);

	FILE* csvFD = fopen("times.csv", "wb");
	fprintf(csvFD, "JD,EqT,D,fajr_f,duhr_f,asr_f,maghrib_f,isha_f,fajr,duhr,asr,maghrib,isha\n");
	//fprintf(csvFD, "JD,EqT,D,g,q,L,e,RA\n");

	AstroInfo ai = { 0 };
	AthanTimes times = { 0 };
	char buf[8 * 5] = { 0 };
	for (U32 i = 0; i < 365 * 100; i++)
	{
		const JulianDate jd = curJD + (JulianDate)i;
		getAstroInfo(&ai, jd);
		getAthanTimes(&times, jd);

		const char* fajrStr = timeToStr(buf + 8 * 0, times.fajr);
		const char* duhrStr = timeToStr(buf + 8 * 1, times.duhr);
		const char* asrStr = timeToStr(buf + 8 * 2, times.asr);
		const char* maghribStr = timeToStr(buf + 8 * 3, times.maghrib);
		const char* ishaStr = timeToStr(buf + 8 * 4, times.isha);

		fprintf(csvFD, "%.0f,%.4f,%.4f,%s,%s,%s,%s,%s,%.2f,%.2f,%.2f,%.2f,%.2f\n",
			jd,
			ai.eqt,
			ai.sunDeclination,
			fajrStr,
			duhrStr,
			asrStr,
			maghribStr,
			ishaStr,
			times.fajr,
			times.duhr,
			times.asr,
			times.maghrib,
			times.isha
		);

		//fprintf(csvFD, "%.0f,%f,%f,%f,%f,%f,%f,%f\n",
		//	jd,
		//	ai.eqt,
		//	ai.sunDeclination,
		//	ai.g,
		//	ai.q,
		//	ai.L,
		//	ai.e,
		//	ai.RA
		//);
	}

	fclose(csvFD);

	return 0;
}
