#include <list>
#include <algorithm>
#include <stdexcept>
#include <Windows.h>
#include <takisy/core/timer.h>

class timer::implement
{
    friend class timer;
    friend class core;

public:
    implement(DWORD interval, bool repeatable)
        : repeatable_(repeatable), interval_(interval), handle_times_(0)
    {}

private:
    bool  repeatable_;
    DWORD interval_;
    DWORD start_timestamp_, handle_timestamp_;
    unsigned int handle_times_;
};

class timer::core
{
private:
    core(void)
        : eventid_(SetTimer(nullptr, 0, 10, callback))
    {}

   ~core(void)
    {
        KillTimer(nullptr, eventid_);
    }

public:
    bool exists(const timer* timer)
    {
        return std::find(timers_.begin(),
                         timers_.end(), timer) != timers_.end();
    }

public:
    void push(timer* timer)
    {
        if (!exists(timer))
        {
            timer->restart();
            timers_.push_back(timer);
        }
    }

    void pop(timer* timer)
    {
        timers_.remove(timer);
    }

public:
    static core& get(void)
    {
        static core single_instance;
        return single_instance;
    }

private:
    static VOID CALLBACK callback(HWND hwnd, UINT msg,
                                  UINT_PTR eventid, DWORD timestamp)
    {
        for (timer* timer : get().timers_)
        {
            if (  timer->impl_->handle_timestamp_
                + timer->impl_->interval_ < timestamp)
            {
                timer->onTimer();

                if (timer->impl_->repeatable_)
                    timer->impl_->handle_timestamp_ = timestamp;
                else
                    get().pop(timer);
            }
        }
    }

private:
    UINT_PTR eventid_;
    std::list<timer*> timers_;
};

timer::timer(void)
    : timer(1)
{}

timer::timer(unsigned int interval)
    : timer(interval, true)
{}

timer::timer(unsigned int interval, bool repeatable)
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

unsigned int timer::elapse(void) const
{
    return active() ? now() - impl_->start_timestamp_ : 0;
}

unsigned int timer::handle_times(void) const
{
    return active() ? impl_->handle_times_ : 0;
}

void timer::active(bool active)
{
    active ? start() : stop();
}

void timer::interval(unsigned int interval)
{
    impl_->interval_ = interval;
}

void timer::start(void)
{
    core::get().push(this);
}

void timer::restart(void)
{
    impl_->handle_times_     = 0;
    impl_->handle_timestamp_ =
    impl_->start_timestamp_  = now();
}

void timer::stop(void)
{
    core::get().pop(this);
}

unsigned long timer::now()
{
    return GetTickCount();
}
