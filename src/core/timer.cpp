#include <list>
#include <algorithm>
#include <stdexcept>
#include <takisy/core/macro.h>
#include <takisy/core/timer.h>

#ifdef __os_win__
#include <Windows.h>
#else
#include <signal.h>
#include <sys/time.h>
#endif

class timer::implement
{
    friend class timer;
    friend class core;

public:
    implement(unsigned long interval, bool repeatable)
        : repeatable_(repeatable), interval_(interval), handle_times_(0)
    {}

private:
    bool  repeatable_;
    unsigned long interval_;
    unsigned long start_timestamp_, handle_timestamp_;
    unsigned long handle_times_;
};

class timer::core
{
    static constexpr unsigned long minimal_interval = 1;

private:
    core(void)
    #ifdef __os_win__
        : eventid_(SetTimer(nullptr, 0, minimal_interval, callback))
    #endif
    {
    #ifndef __os_win__
        signal(SIGALRM, callback);

        struct itimerval itimerval;
        itimerval.it_interval.tv_sec  =
        itimerval.it_value.tv_sec     = minimal_interval / 1000;
        itimerval.it_interval.tv_usec =
        itimerval.it_value.tv_usec    = (minimal_interval % 1000) * 1000;

        setitimer(ITIMER_REAL, &itimerval, nullptr);
    #endif
    }

    ~core(void)
    {
    #ifdef __os_win__
        KillTimer(nullptr, eventid_);
    #endif
    }

public:
    static core& get(void)
    {
        static core single_instance;
        return single_instance;
    }

public:
    bool exists(const timer* timer)
    {
        return std::find(timers_.begin(),
                         timers_.end(), timer) != timers_.end();
    }

public:
    void add(timer* timer)
    {
        if (!exists(timer))
        {
            timers_.push_back(timer);
            timer->restart();
        }
    }

    void remove(timer* timer)
    {
        timers_.remove(timer);
    }

private:
#ifdef __os_win__
    static VOID CALLBACK callback(HWND, UINT, UINT_PTR, DWORD timestamp)
#else
    static void callback(int sig)
#endif
    {
    #ifndef __os_win__
        unsigned long timestamp = timer::now();
    #endif

        for (timer* timer : get().timers_)
        {
            if (  timer->impl_->handle_timestamp_
                + timer->impl_->interval_ < timestamp)
            {
                timer->onTimerHandle();
                timer->impl_->handle_timestamp_ = timestamp;
                timer->impl_->handle_times_++;

                if (!timer->impl_->repeatable_)
                    timer->stop();
            }
        }
    }

private:
#ifdef __os_win__
    UINT_PTR eventid_;
#endif
    std::list<timer*> timers_;
};

timer::timer(void)
    : timer(1)
{}

timer::timer(unsigned long interval)
    : timer(interval, true)
{}

timer::timer(unsigned long interval, bool repeatable)
    : impl_(new implement(interval, repeatable))
{}

timer::~timer(void)
{
    stop();
}

timer::timer(const timer& timer)
{
    operator=(timer);
}

timer& timer::operator=(const timer& timer)
{
    throw std::logic_error("class timer is noncopyable class.");
}

bool timer::active(void) const
{
    return core::get().exists(this);
}

bool timer::repeatable(void) const
{
    return impl_->repeatable_;
}

unsigned long timer::elapse(void) const
{
    return active() ? now() - impl_->start_timestamp_ : 0;
}

unsigned long timer::handle_times(void) const
{
    return active() ? impl_->handle_times_ : 0;
}

void timer::active(bool active)
{
    active ? start() : stop();
}

void timer::interval(unsigned long interval)
{
    impl_->interval_ = interval;
}

void timer::start(void)
{
    core::get().add(this);
}

void timer::restart(void)
{
    impl_->start_timestamp_ = impl_->handle_timestamp_ = now();
    impl_->handle_times_    = 0;

    start();
}

void timer::stop(void)
{
    core::get().remove(this);
}

unsigned long timer::now()
{
#ifdef __os_win__
    return GetTickCount();
#else
    struct timeval timeval;

    if (gettimeofday(&timeval, nullptr) == 0)
        return timeval.tv_sec * 1000 + timeval.tv_usec / 1000;
    else
        return 0;
#endif
}
