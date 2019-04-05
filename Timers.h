#ifndef TIMERS_H
#define TIMERS_H

class Timers {
    public:
        double physicsRate;
        double oobillion;
        struct timespec timeStart, timeEnd, timeCurrent;
        struct timespec walkTime;
        Timers();
        double timeDiff(timespec * start, timespec * end);
        void timeCopy(timespec * dest, timespec * source);
        void recordTime(struct timespec* t);
        // double timeDiff(struct timespec *start, struct timespec *end) {
        //     return (double)(end->tv_sec - start->tv_sec ) +
        //         (double)(end->tv_nsec - start->tv_nsec) * oobillion;
        // }
        // void timeCopy(struct timespec *dest, struct timespec *source) {
        //     memcpy(dest, source, sizeof(struct timespec));
        // }
        // void recordTime(struct timespec *t) {
        //     clock_gettime(CLOCK_REALTIME, t);
        // }
};

#endif