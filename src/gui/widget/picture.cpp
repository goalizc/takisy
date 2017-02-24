#include <takisy/core/timer.h>
#include <takisy/cgl/basic/canvas.h>
#include <takisy/cgl/tr/filter/scale.h>
#include <takisy/gui/widget/picture.h>

class picture::implement
{
    friend class picture;

public:
    implement(picture& self)
        : self(self), scalable_(false), keep_ratio_(true), frame_index_(0)
    {
        timer_.onTimer(
            [this, &self](timer* timer)
            {
                unsigned int index = image_.seek(timer->elapse());

                if (frame_index_ != index)
                {
                    frame_index_  = index;
                    self.repaint();
                }
            });
    }

public:
    inline void refresh(void)
    {
        frame_index_ = 0;
        timer_.restart();
        self.dynamic(image_.count() > 1);
        self.repaint();
    }

public:
    static void disable_canvas(canvas_bgra8& canvas)
    {
        for (canvas_bgra8::pixel_format& pixel : canvas.pixels())
            pixel.a(pixel.a() >> 1);
    }

private:
    picture& self;
    timer timer_;
    class image image_;
    bool scalable_;
    bool keep_ratio_;
    unsigned int frame_index_;
};

picture::picture(void)
    : impl_(new implement(*this))
{}

picture::picture(const char* uri)
    : picture()
{
    load_uri(uri);
}

picture::picture(const stream& stream)
    : picture()
{
    load_stream(stream);
}

picture::picture(const class image& _image)
    : picture()
{
    image(_image);
}

picture::~picture(void)
{
    delete impl_;
}

bool picture::load_uri(const char* uri)
{
    std::shared_ptr<stream> stream_ptr = stream::from_uri(uri);

    if (!stream_ptr)
        return false;

    return load_stream(*stream_ptr);
}

bool picture::load_stream(const stream& stream)
{
    if (impl_->image_.load_stream(stream))
    {
        impl_->refresh();
        onLoadedHandle();
        return true;
    }

    return false;
}

bool picture::scalable(void) const
{
    return impl_->scalable_;
}

bool picture::keep_ratio(void) const
{
    return impl_->keep_ratio_;
}

bool picture::dynamic(void) const
{
    return impl_->timer_.active();
}

class image& picture::image(void)
{
    return impl_->image_;
}

const class image& picture::image(void) const
{
    return impl_->image_;
}

Size picture::optimal_size(OptimalPolicy policy) const
{
    if (impl_->frame_index_ < impl_->image_.count())
    {
        const canvas_adapter& frame = impl_->image_.frame(impl_->frame_index_);

        if (policy == opUnset || !scalable())
            return Size(frame.width(), frame.height());
        else if (policy == opFixedWidth)
            return Size(width(), frame.height() * width() / frame.width());
        else if (policy == opFixedHeight)
            return Size(frame.width() * height() / frame.height(), height());
    }

    return Size(0, 0);
}

void picture::scalable(bool scalable)
{
    if (impl_->scalable_ == scalable)
        return;

    impl_->scalable_ = scalable;
    repaint();
}

void picture::keep_ratio(bool keep_ratio)
{
    if (impl_->keep_ratio_ == keep_ratio)
        return;

    impl_->keep_ratio_ = keep_ratio;
    repaint();
}

void picture::dynamic(bool dynamic)
{
    impl_->timer_.active(dynamic);
}

void picture::image(const class image& image)
{
    impl_->image_ = image;
    impl_->refresh();
}

void picture::onPaint(graphics graphics, Rect rect)
{
    if (impl_->image_.count() == 0)
        return;

    impl_->frame_index_ = impl_->image_.seek(impl_->timer_.elapse());
    const canvas_adapter& frame = impl_->image_.frame(impl_->frame_index_);
    typedef derived_canvas_adapter<canvas_bgra8&> canvas_adapter;

    if (impl_->scalable_)
    {
        typedef bilinear_interpolation_scale scale;

        if (impl_->keep_ratio_)
        {
            double wratio = static_cast<double>(width()) / height();
            double fratio = static_cast<double>(frame.width()) / frame.height();

            if (width() != frame.width() && wratio < fratio)
            {
                canvas_bgra8 canvas = frame;
                unsigned int height = frame.height() * width() / frame.width();
                scale(width(), height).filter(canvas, canvas);
                if (disabled()) implement::disable_canvas(canvas);
                int y = ((int)this->height() - (int)height) / 2;
                return graphics.draw_image(0, y, canvas_adapter(canvas));
            }
            else
            if (height() != frame.height() && wratio >= fratio)
            {
                canvas_bgra8 canvas = frame;
                unsigned int width = frame.width() * height() / frame.height();
                scale(width, height()).filter(canvas, canvas);
                if (disabled()) implement::disable_canvas(canvas);
                int x = ((int)this->width() - (int)width) / 2;
                return graphics.draw_image(x, 0, canvas_adapter(canvas));
            }
        }
        else
        if (width() != frame.width() && height() != frame.height())
        {
            canvas_bgra8 canvas = frame;
            scale(width(), height()).filter(canvas, canvas);
            if (disabled()) implement::disable_canvas(canvas);
            return graphics.draw_image(canvas_adapter(canvas));
        }
    }

    int x = ((int)width()  - (int)frame.width())  / 2;
    int y = ((int)height() - (int)frame.height()) / 2;

    if (disabled())
    {
        canvas_bgra8 canvas = frame;
        implement::disable_canvas(canvas);
        graphics.draw_image(x, y, canvas_adapter(canvas));
    }
    else
        graphics.draw_image(x, y, frame);
}
