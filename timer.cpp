#include "timer.hpp"

Timer::Timer(double len) {
  length = len;
  reset();
}

void Timer::reset() {
  last_updated = clock();
}

bool Timer::ready() {
  double time_diff = (clock() - last_updated) / CLOCKS_PER_SEC;
  return time_diff >= length;
}


IntervalExecutor::IntervalExecutor(Timer *t, void (*exec)(World *world)) {
  timer = t;
  execute = exec;
}


IntervalExecutorBus::IntervalExecutorBus(World *w) {
  intervalExecutors = new map<int, IntervalExecutor *>();
  world = w;
}

int IntervalExecutorBus::addIntervalExecutor(double frequency, void (*exec)(World *world)) {
  (*intervalExecutors)[count] = new IntervalExecutor(new Timer(frequency), exec);
  count++;
  return count - 1;
}

void IntervalExecutorBus::removeIntervalExecutor(int id) {
  intervalExecutors->erase(id);
}

void IntervalExecutorBus::checkAndRunExecutors() {
  for(auto it = intervalExecutors->begin(); it != intervalExecutors->end(); it++) {
    // check if the timer is ready
    if(it->second->timer->ready()) {
      it->second->execute(world);
      it->second->timer->reset();
    }
  }
}