#ifndef PERFORMANCECHECKER_H
#define PERFORMANCECHECKER_H

#include <ctime>

class TimeCheck {
public:
  TimeCheck();
  ~TimeCheck();

  void report() const;

  double timeElapsed() const;

private:
  std::clock_t m_start;
};

#endif /* PERFORMANCECHECKER_H */
