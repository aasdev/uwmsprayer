/*
 *
 *  This program is meant to be called remotely from a web-based script -- it does not
 *  do sufficient input / sanity checking for interactive use in the current version.
 *
 *  Uses libitl from the arabeyes project (http://www.arabeyes.org)
 */


#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <time.h>
#include <string.h>
#include <itl/prayer.h>
#include <itl/hijri.h>
#include "uwmsprayer.h"


const char optionstring[] = "d:m:y:hjscp";

int	uwmsday, uwmsmonth, uwmsyear;
int	fullmonth = 0;
int	singleday = 0;
int	printcsv = 0;
int	hijri = 0;

void printUsage (void);

/**** usage ***
  uwmsprayer -d <dd> -m <mm> -y <yyyy>
  -j -- print hijri: day,month,year,monthname
  -s -- print full month schedule (either hijri or gregorian)
  -c -- print full month schedule in CSV (comma separated) format
  -p -- print prayer times: fajr,time,sunrise,time,dhuhr,time,asr,time,maghrib,time,isha,time
  -h -- print usage

*/



/******************************************************************/
int
main (int argc, char **argv)
{
  int	c, errflg = 0;

  while ((c = getopt(argc, argv, optionstring)) != -1) {

    switch (c) {
    case 'd':
      uwmsday = (int) strtol(optarg, NULL, 10);
      break;
    case 'm':
      uwmsmonth = (int) strtol(optarg, NULL, 10);
      break;
    case 'y':
      uwmsyear = (int) strtol(optarg, NULL, 10);
      break;
    case 'h':
      printUsage();
      return (0);
      break;
    case 'j':
      hijri =1;
      break;      
    case 's':
      fullmonth = 1;
      singleday = 0;
      break;
    case 'c':
      printcsv = 1;
      fullmonth = 1;
      singleday = 0;
      break;
    case 'p':
      fullmonth = 0;
      singleday = 1;
      break;
    case '?':
      errflg++;
    }
  }

  if (errflg || argc == 1) {
    printUsage ();
    return (-1);
  }

    /* decide what to do */
  if (fullmonth) {  /* print full month schedule */
    if (hijri) {
      getHijriPrayerTimesMonth ();
    }
    else {
      getUWMSPrayerTimesMonth ();
    }
  }
  else { /* print single day prayer times or hijri data */
    if (!singleday) {
      if (hijri) { /* just print hijri date */
	getUWMSHijriDate ();
      }
      else {
	/* not implemented */
      }
    }
    else {  /* print single day prayer timess */
      getUWMSPrayerTimes ();
    }
  }

  
  return (0);
}

/******************************************************************/
void
printUsage (void)
{
  printf ("uwmsprayer -d <dd> -m <mm> -y <yyyy>\n");
  printf ("\t-j print hijri: day,month,year,monthname (month and year specified in hijri)\n");
  printf ("\t-s print full month schedule (combine with -j for hijri\n");
  printf ("\t-c print full month schedule in CSV (comma separated) format\n");
  printf ("\t-p print prayer times: fajr,<time>,sunrise,<time>,dhuhr,<time>,asr,<time>,maghrib,<time>,isha,<time>\n");
  printf ("\t-h print this usage message\n");
}

/******************************************************************/
  
void
getUWMSPrayerTimes ()
{
  Location  loc;
  Method    conf;
  Date	    date;
  sDate	    hdate;
  Prayer    prayers[6];
  struct tm *tm, asktm;
  time_t    asktime;
  int	    i;



  /* initialize calculation method */
  getMethod (0, &conf); /* reset */ 
  getMethod (UWMS_CALCMETHOD, &conf);
  conf.offset = 1;
  for (i = 0; i < 6; i++) {
    conf.offList[i] = OFFSETS[i];
  }

  /* init location struct */
  loc.degreeLong = UWMS_LONGITUDE;
  loc.degreeLat = UWMS_LATITUDE;
  loc.gmtDiff = UWMS_GMTDIFF;
  loc.seaLevel = UWMS_SEALEVEL;
  loc.pressure = 1010;	/* astro std value */
  loc.temperature = 10; /* astro std value */


  if (hijri) {
    /* convert hijri date to gregorian */
    memset ((void *) &hdate, 0, sizeof (hdate));
    H2G (&hdate, uwmsday, uwmsmonth, uwmsyear);
    date.day = hdate.day;
    date.month = hdate.month;
    date.year = hdate.year;
  }
  else {
    date.day = uwmsday;
    date.month = uwmsmonth;;
    date.year = uwmsyear;
  }


  /* convert requested time into time since epoch -- fills in DST */
  /* make sure time is past the DST conversion time */
  asktm.tm_year = date.year - 1900;
  asktm.tm_mon = date.month -1;
  asktm.tm_mday = date.day;
  asktm.tm_hour = 2;
  asktm.tm_min = 0;
  asktm.tm_sec = 0;
  asktm.tm_isdst = -1;
  asktime = mktime (&asktm);
  tm = localtime(&asktime);

  /* update loc dst */
  loc.dst = tm->tm_isdst;

    
  getPrayerTimes (&loc, &conf, &date, prayers);
  

  for (i = 0; i < 5; i++) {
    printf("%s,%d:%02d,", UWMSPrayers[i], prayers[i].hour, prayers[i].minute);
  }
  printf("%s,%d:%02d\n", UWMSPrayers[i], prayers[i].hour, prayers[i].minute);
  
  
}

/******************************************************************/
void
getUWMSPrayerTimesMonth ()
{
  Location  loc;
  Method    conf;
  Date	    date;
  sDate	    hdate;
  Prayer    prayers[6];
  struct tm *tm, asktm;
  time_t    result, asktime;
  int	    i, dayindex, daysinmon;;

  date.month = uwmsmonth;
  date.year = uwmsyear;


  getMethod (UWMS_CALCMETHOD, &conf);
  conf.offset = 1;
  for (i = 0; i < 6; i++) {
    conf.offList[i] = OFFSETS[i];
  }


  /* init location struct */
  memset ((void *) &loc, 0, sizeof (loc));
  loc.degreeLong = UWMS_LONGITUDE;
  loc.degreeLat = UWMS_LATITUDE;
  loc.gmtDiff = UWMS_GMTDIFF;
  loc.seaLevel = UWMS_SEALEVEL;
  loc.pressure = 1010;	/* astro std value */
  loc.temperature = 10; /* astro std value */

  /* figure out days in this month */
  daysinmon = DAYSINMONTH[uwmsmonth];
  if (uwmsmonth == 2 && isLeapYear(uwmsyear)) {
    daysinmon = DAYSINMONTH[uwmsmonth] + 1;
  }

  if (printcsv) {
    printf ("date,day,hijri,");
    for (i = 0; i < 5; i++) {
      printf ("%s,", UWMSPrayers[i]);
    }
    printf ("%s\n", UWMSPrayers[5]);
  }
  else {
    printf ("date\tday\thirji\t");
    for (i = 0; i < 6; i++) {
      printf ("%s\t", UWMSPrayers[i]);
    }
    printf ("\n");
  }
  
  for (dayindex = 1; dayindex <= daysinmon; dayindex++)
    {
      date.day = dayindex;
      G2H (&hdate, dayindex, date.month, date.year);
      /* printf ("%d,%d,%d,%s\n", hdate.day, hdate.month, hdate.year, hdate.to_mname); */

      /* convert requested time into time since epoch -- fills in DST */
      /* make sure time is past the DST conversion time */      
      memset ((void *) &asktm, 0, sizeof(asktm));
      asktm.tm_year = date.year - 1900;
      asktm.tm_mon = date.month -1;
      asktm.tm_mday = date.day;
      asktm.tm_hour = 2;
      asktm.tm_min = 0;
      asktm.tm_sec = 0;
      asktm.tm_isdst = -1;
      asktime = mktime (&asktm);
      tm = localtime(&asktime);

      /* update loc dst */
      loc.dst = tm->tm_isdst;

      getPrayerTimes (&loc, &conf, &date, prayers);

      if (printcsv) {
	/* print csv output suitable for printed prayer schedule */
	if (hdate.day == 1 || dayindex == 1) {
	  printf ("%d,%s,%d/%s,", dayindex, DAYSOFWEEK[asktm.tm_wday],hdate.day,hdate.to_mname);
	}
	else {
	  printf ("%d,%s,%d/%d,", dayindex, DAYSOFWEEK[asktm.tm_wday],hdate.day,hdate.month);
	}
	for (i = 0; i < 5; i++) {
	  printf("%d:%02d,", (prayers[i].hour>12 ? (prayers[i].hour-12):(prayers[i].hour)), prayers[i].minute);
	}
	printf("%d:%02d\n", (prayers[5].hour>12 ? (prayers[5].hour-12):(prayers[5].hour)), prayers[5].minute);	
      }
      else {
	if (hdate.day == 1 || dayindex == 1) {
	  printf ("%d\t%s\t%d/%s\t", dayindex, DAYSOFWEEK[asktm.tm_wday],hdate.day,hdate.to_mname);
	}
	else {
	  printf ("%d\t%s\t%d/%d\t", dayindex, DAYSOFWEEK[asktm.tm_wday],hdate.day,hdate.month);
	}
	for (i = 0; i < 6; i++) {
	  printf("%d:%02d\t", prayers[i].hour, prayers[i].minute);
	}
	printf ("\n");
      }
    }
  
}

/******************************************************************/

void
getUWMSHijriDate ()
{
  sDate	hdate;
  int	found;

  /* Convert using hijri code from gregorian to umm_alqura hijri */
  found = G2H (&hdate, uwmsday, uwmsmonth, uwmsyear);
  
  if (!found) {
    /*      printf("Got an error from the library (code = %d)", error_code); */
    return ;
  }
  
  printf ("%d,%d,%d,%s\n", hdate.day, hdate.month, hdate.year, hdate.to_mname);
  
}

/******************************************************************/

int
isLeapYear (int year)
{
  int	result = 0;

  if (year % 4 == 0)
    result = 1;    /* leap_year = 1; */

   if (year % 100 == 0)
   {
      /* years=100,200,300,500,... are not leap years */
      result = 0;

      /* years=400,800,1200,1600,2000,2400 are leap years */
      if (year % 400 == 0)
	result = 1;
   }

   return (result);
}

/******************************************************************/
void
getHijriPrayerTimesMonth ()
{
  Location  loc;
  Method    conf;
  Date	    date;
  
  sDate	    hdate;
  Prayer    prayers[6];
  struct tm *tm, asktm;
  time_t    asktime;
  int	    i, dayindex, daysinmon;

  /* set the prayer method structure */
  getMethod (UWMS_CALCMETHOD, &conf);
  conf.offset = 1;
  for (i = 0; i < 6; i++) {
    conf.offList[i] = OFFSETS[i];
  }

  /* initialize the location struct */
  memset ((void *) &loc, 0, sizeof (loc));
  loc.degreeLong = UWMS_LONGITUDE;
  loc.degreeLat = UWMS_LATITUDE;
  loc.gmtDiff = UWMS_GMTDIFF;
  loc.seaLevel = UWMS_SEALEVEL;
  loc.pressure = 1010;	/* astro std value */
  loc.temperature = 10; /* astro std value */

  /* uwmsmonth and uwmsyear are in hijri -- figure out the corresponding gregorian dates */
  memset ((void *) &hdate, 0, sizeof (hdate));
  H2G (&hdate, 1, uwmsmonth, uwmsyear);
  /* printf ("%d %s, %d -> %d %s, %d\n", 1, hdate.frm_mname, uwmsyear, hdate.day, hdate.to_mname, hdate.year); */

  /* figure out days in this hijri month */
  daysinmon = HMonthLength (uwmsyear, uwmsmonth);

  printf ("hijri,day,date,");
  for (i = 0; i < 5; i++) {
    printf ("%s,", UWMSPrayers[i]);
  }
  printf ("%s\n", UWMSPrayers[5]);

  
  for (dayindex = 1; dayindex <= daysinmon; dayindex++)
    {
      H2G (&hdate, dayindex, uwmsmonth, uwmsyear);
      /* printf ("%d,%d,%d,%s\n", hdate.day, hdate.month, hdate.year, hdate.to_mname); */
      date.day = hdate.day;
      date.month = hdate.month;
      date.year = hdate.year;
      /* convert requested time into time since epoch -- fills in DST */
      /* make sure time is past the DST conversion time */      
      memset ((void *) &asktm, 0, sizeof(asktm));
      asktm.tm_year = date.year - 1900;
      asktm.tm_mon = date.month -1;
      asktm.tm_mday = date.day;
      asktm.tm_hour = 2;
      asktm.tm_min = 0;
      asktm.tm_sec = 0;
      asktm.tm_isdst = -1;
      asktime = mktime (&asktm);
      tm = localtime(&asktime);

      /* update loc dst */
      loc.dst = tm->tm_isdst;

      getPrayerTimes (&loc, &conf, &date, prayers);

      if (printcsv) {
	/* print csv output suitable for printed prayer schedule */
	if (hdate.day == 1 || dayindex == 1) {
	  printf ("%d,%s,%d/%s,", dayindex, DAYSOFWEEK[asktm.tm_wday],hdate.day,hdate.to_mname_sh);
	}
	else {
	  printf ("%d,%s,%d/%d,", dayindex, DAYSOFWEEK[asktm.tm_wday],hdate.day,hdate.month);
	}
	for (i = 0; i < 5; i++) {
	  printf("%d:%02d,", (prayers[i].hour>12 ? (prayers[i].hour-12):(prayers[i].hour)), prayers[i].minute);
	}
	printf("%d:%02d\n", (prayers[5].hour>12 ? (prayers[5].hour-12):(prayers[5].hour)), prayers[5].minute);	
      }
      else {
	if (hdate.day == 1 || dayindex == 1) {
	  printf ("%d\t%s\t%d/%s\t", dayindex, DAYSOFWEEK[asktm.tm_wday],hdate.day,hdate.to_mname);
	}
	else {
	  printf ("%d\t%s\t%d/%d\t", dayindex, DAYSOFWEEK[asktm.tm_wday],hdate.day,hdate.month);
	}
	for (i = 0; i < 6; i++) {
	  printf("%d:%02d\t", prayers[i].hour, prayers[i].minute);
	}
	printf ("\n");
      }
    }
}


  
