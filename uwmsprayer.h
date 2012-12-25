#ifndef _uwmsprayer_h_
#define _uwmsprayer_h_

#define	UWMS_LATITUDE	    41.12333;	/* Thornwood lat */
#define UWMS_LONGITUDE	    -73.77944;	/* Thornwood long */
#define UWMS_SEALEVEL	    95		/* Thornwood height above sea level in m */
#define UWMS_GMTDIFF	    -5;		/* US Eastern */
#define UWMS_CALCMETHOD	    4		/* ISNA */

static const double OFFSETS[6] = {1, 2.5, 0, 0, 1, 0};
static const char   UWMSPrayers[6][10] = {"fajr", "sunrise", "dhuhr", "asr", "maghrib", "isha"};
static const int DAYSINMONTH[13] = {0 ,31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
static const char DAYSOFWEEK[7][4] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

typedef struct {
  char   ptimes[6][6];
} uwmsprayertimes_t;


typedef struct {
  int	day;
  int	year;
  int	month;
  char	monthname[20];
} uwmshijri_t;




/******************************************************************/

void getUWMSPrayerTimes ();

void getUWMSPrayerTimesMonth ();

void getHijriPrayerTimesMonth ();

void getUWMSHijriDate ();

int isLeapYear (int year);



#endif
