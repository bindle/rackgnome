
#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
#include <inttypes.h>

#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#endif


#ifndef CLOCK_MONOTONIC_RAW
#define CLOCK_MONOTONIC_RAW CLOCK_MONOTONIC
#endif


//_RACKGNOME_I void  rackgnome_nsecs(struct timespec *ts)
void  rackgnome_nsecs(struct timespec *ts)
{
#ifdef __MACH__
   clock_serv_t    cclock;
   mach_timespec_t mts;
   host_get_clock_service(mach_host_self(), SYSTEM_CLOCK, &cclock);
   clock_get_time(cclock, &mts);
   mach_port_deallocate(mach_task_self(), cclock);
   ts->tv_sec  = mts.tv_sec;
   ts->tv_nsec = mts.tv_nsec;
#else
  clock_gettime(CLOCK_REALTIME, ts);
#endif
   return;
}

int main(void)
{
   struct timespec ts;
   struct timespec ts1;
   struct timespec ts2;
   uint64_t        c;

   rackgnome_nsecs(&ts);
   rackgnome_nsecs(&ts1);
   printf("tv_sec/tv_nsec:  %li.%09li\n", ts.tv_sec, ts.tv_nsec);
   rackgnome_nsecs(&ts2);
   printf("tv_sec/tv_nsec:  %li.%09li\n", ts1.tv_sec, ts1.tv_nsec);
   printf("tv_sec/tv_nsec:  %li.%09li\n", ts2.tv_sec, ts2.tv_nsec);

   return(0);
}
