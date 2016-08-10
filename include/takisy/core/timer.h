#ifndef timer_h_20140124
#define timer_h_20140124

#include <memory>
#include <takisy/core/handler.h>

class timer
{
    class implement;
    class core;
    friend class core;

    DECLARE_HANDLER(onTimer);

public:
    timer(void);
    timer(unsigned int interval);
    timer(unsigned int interval, bool repeatable);
   ~timer(void);

private:
    timer(const timer& timer);
    timer& operator=(const timer& timer);

public:
    bool active(void) const;
    bool repeatable(void) const;
    unsigned int interval(void) const;
    unsigned int elapse(void) const;
    unsigned int handle_times(void) const;

public:
    void active(bool active);
    void interval(unsigned int interval);

    void start(void);
    void restart(void);
    void stop(void);

public:
    static unsigned long now();

private:
    class implement* impl_;
};

#endif // timer_h_20140124
