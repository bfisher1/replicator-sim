#ifndef TIMER_H
#define TIMER_H

#include <time.h>
#include <map>
#include "world.hpp"

class World;

/*
Represents a timer that is finished after a certain amount of time.

- last_updated lets us know when the timer started
- the length is how long the timer waits until it rings (length is in seconds)
*/
class Timer {
  public:
    clock_t last_updated;
    double length;
    Timer(double length);
    bool ready();
    void reset();
};


class IntervalExecutor {
  public:
    Timer *timer;
    void (*execute)(World *);
    IntervalExecutor(Timer *timer, void (*)(World *world));
};

/*
  Handles a collection of timers. Whenever one is ready, a function associated with
  the timer is executed and the timer is reset.

  Functions are executed at every frequency seconds.
  Index of an interval executor is returned upon creation so that it can be removed if needed.
*/
class IntervalExecutorBus {
  public:
    World *world;
    int count;
    map<int, IntervalExecutor*> *intervalExecutors;
    IntervalExecutorBus();
    int addIntervalExecutor(double frequency, void (*)(World *world));
    void removeIntervalExecutor(int id);
    void checkAndRunExecutors();
};

#endif /* TIMER_H */
