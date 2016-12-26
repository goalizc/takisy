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
    timer(unsigned long interval);
    timer(unsigned long interval, bool repeatable);
   ~timer(void);

private:
    timer(const timer& timer);
    timer& operator=(const timer& timer);

public:
    bool active(void) const;
    bool repeatable(void) const;
    unsigned long interval(void) const;
    unsigned long elapse(void) const;
    unsigned long handle_times(void) const;

public:
    void active(bool active);
    void interval(unsigned long interval);

    void start(void);
    void restart(void);
    void stop(void);

public:
    static unsigned long now();

private:
    class implement* impl_;
};

#endif // timer_h_20140124
