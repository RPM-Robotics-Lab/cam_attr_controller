#ifndef __IRAP_COMMON_TIMESTAMP_H__
#define __IRAP_COMMON_TIMESTAMP_H__

#include <stdint.h>
#include <inttypes.h>
#include <sys/time.h>
#include <time.h>
#include <glib.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct timestamp_sync_state timestamp_sync_state_t;
struct timestamp_sync_state {
    double  dev_ticks_per_second; // how fast does device clock count? (nominal)
    int64_t dev_ticks_wraparound; // device clock counts modulo what?
    double  max_rate_error;       // how fast do we need to count to ensure we're counting faster than device?

    int64_t sync_host_time;       // when we last synced, what time was it for the host?
    int64_t dev_ticks_since_sync; // how many device ticks have elapsed since the last sync?

    int64_t last_dev_ticks;       // what device time was it when we were last called?

    uint8_t is_valid;             // have we ever synced?
};

/* Returns integer time since the Epoch in microseconds */
int64_t
timestamp_now (void);

/* Returns integer seconds since the Epoch */
int64_t
timestamp_seconds (int64_t utime);

/* Returns number of microseconds since the last integer second */
int64_t
timestamp_useconds (int64_t utime);

/* Returns fractional number of seconds since the Epoch */
double
timestamp_to_double (int64_t utime);

void
timestamp_to_timeval (int64_t utime, struct timeval *tv);

void
timestamp_to_timespec (int64_t utime, struct timespec *ts);

void
timestamp_to_gtimeval (int64_t utime, GTimeVal *result);

/** Create a new time synchronizer.
    @param dev_ticks_per_second  The nominal rate at which the device time increments
    @param dev_ticks_wraparound  Assume that dev_ticks wraps around every wraparound ticks
    @param rate                  An upper bound on the rate error. Should be (1 + eps)
**/
timestamp_sync_state_t *
timestamp_sync_init (double dev_ticks_per_second, int64_t dev_ticks_wraparound, double rate);

void
timestamp_sync_free (timestamp_sync_state_t *s);


/** Estimate the local host's time that corresponds to dev_ticks. (The
    latency model is also updated.) **/
int64_t
timestamp_sync (timestamp_sync_state_t * s, int64_t dev_ticks, int64_t host_utime);



#ifdef __cplusplus
}
#endif

#endif // __IRAP_COMMON_TIMESTAMP_H__
