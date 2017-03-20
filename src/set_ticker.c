/*set_ticker.c
 * Written by Bruce Molay for "Understanding Unix/Linux Programming"
 * Used by Matthew Schalk/Jay Siberski for Backanoid
 * 09 March 2017
*/ 

#include <stdio.h>
#include <sys/time.h>
#include <signal.h>

int set_ticker(int millisecs)
{
	struct itimerval new_timeset;
	long n_sec, n_usecs;
	
	n_sec = millisecs / 1000;//int part
	n_usecs = (millisecs %1000) * 1000L;//remainder
	
	new_timeset.it_interval.tv_sec = n_sec;//set reload
	new_timeset.it_interval.tv_usec = n_usecs;//new ticker value
	new_timeset.it_value.tv_sec = n_sec;//stored value
	new_timeset.it_value.tv_usec = n_usecs;//stored value
	
	return setitimer(ITIMER_REAL, &new_timeset, NULL);//return everything
}
