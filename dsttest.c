 #include <stdio.h>
#include <time.h>

struct tm time_str;

char daybuf[128];

int main(void)
{
  time_str.tm_year = 2009 - 1900;
  time_str.tm_mon = 3 - 1;
  time_str.tm_mday = 9;
  time_str.tm_hour = 5;
  time_str.tm_min = 0;
  time_str.tm_sec = 1;
  time_str.tm_isdst = -1;
  if (mktime(&time_str) == -1)
    (void)puts("-unknown-");
  else {
    (void)strftime(daybuf, sizeof(daybuf), "%x %Z", &time_str);
    (void)puts(daybuf);
  }

  return 0;

}
