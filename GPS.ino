/*
static void get_local_time( gps_fix &fix ){
  // Display the local time

  if (fix.valid.time && fix.valid.date) {
    NeoGPS::clock_t seconds = fix.dateTime;

    //  USA Daylight Savings Time rule (calculated once per reset and year!)
    static NeoGPS::time_t  changeover;
    static NeoGPS::clock_t springForward, fallBack;

    if ((springForward == 0) || (changeover.year != fix.dateTime.year)) {
      changeover.year    = fix.dateTime.year;
      changeover.month   = 3;
      changeover.date    = 14; // latest 2nd Sunday
      changeover.hours   = 2;
      changeover.minutes = 0;
      changeover.seconds = 0;
      changeover.set_day();
      // Step back to the 2nd Sunday, if day != SUNDAY
      changeover.date -= (changeover.day - NeoGPS::time_t::SUNDAY);
      springForward = (NeoGPS::clock_t) changeover;

      changeover.month   = 11;
      changeover.date    = 7; // latest 1st Sunday
      changeover.hours   = 2 - 1; // to account for the "apparent" DST +1
      changeover.set_day();
      // Step back to the 1st Sunday, if day != SUNDAY
      changeover.date -= (changeover.day - NeoGPS::time_t::SUNDAY);
      fallBack = (NeoGPS::clock_t) changeover;
    }

    // Set these values to the offset of your timezone from GMT
    static const int32_t         zone_hours   = -5L; // EST
    static const int32_t         zone_minutes =  0L; // usually zero
    static const NeoGPS::clock_t zone_offset  =
                      zone_hours   * NeoGPS::SECONDS_PER_HOUR +
                      zone_minutes * NeoGPS::SECONDS_PER_MINUTE;

    seconds += zone_offset;

    if ((springForward <= seconds) && (seconds < fallBack))
      seconds += NeoGPS::SECONDS_PER_HOUR;

    NeoGPS::time_t localTime( seconds );

    gps_local_seconds = seconds;

    Serial << localTime;
  }
  //Serial.println();
}
*/
