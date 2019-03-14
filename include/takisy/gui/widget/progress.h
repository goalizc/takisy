#ifndef progress_h_20160831
#define progress_h_20160831

#include <takisy/core/handler.h>
#include <takisy/gui/widget/widget.h>

class progress : public widget
{
protected:
    class implement;

public:
    DECLARE_HANDLER(onProgressChanged);

public:
    progress(void);
    progress(double min, double max);
    progress(double min, double max, double value);
   ~progress(void);

public:
    double min(void) const;
    double max(void) const;
    double value(void) const;

public:
    void range(double min, double max);
    void min(double min);
    void max(double max);
    void value(double value);

public:
    void home(void);
    void end(void);

protected:
    class implement* impl_;
};

class vertical_progress : public progress
{
public:
    using progress::progress;

public:
    void onPaint(graphics graphics, Rect rect) override;
};

class horizontal_progress : public progress
{
public:
    using progress::progress;

public:
    void onPaint(graphics graphics, Rect rect) override;
};

#endif // progress_h_20160831
