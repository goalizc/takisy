#include <takisy/core/timer.h>
#include <takisy/cgl/basic/canvas.h>
#include <takisy/cgl/tr/filter/scale.h>
#include <takisy/gui/widget/picture.h>

class picture::implement
{
    friend class picture;

public:
    implement(picture* _this)
        : this_(_this), scalable_(false), keep_ratio_(true), frame_index_(0)
    {
        timer_.onTimer(
            [this](timer& timer)
            {
                unsigned int index = image_.seek(timer.elapse());

                if (frame_index_ != index)
                {
                    frame_index_  = index;
                    this_->repaint();
                }
            });
    }

public:
    inline bool load_image(bool success)
    {
        if (success)
        {
            frame_index_ = 0;
            timer_.restart();
            this_->dynamic(image_.nframes() > 1);
            this_->repaint();
        }

        return success;
    }

private:
    timer timer_;
    class image image_;
    picture* this_;
    bool scalable_;
    bool keep_ratio_;
    unsigned int frame_index_;
};

picture::picture(void)
    : impl_(new implement(this))
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
    buffer_stream bufstream;
    bufstream.plunder(stream);

    static const std::string gif87a_sig = "GIF87a";
    static const std::string gif89a_sig = "GIF89a";
    char gif_sig[7] = {0};
    if (bufstream.read(gif_sig, 6) != 6)
        return false;
    else
        bufstream.seek(0, stream::stBegin);

    if (gif_sig == gif87a_sig || gif_sig == gif89a_sig)
        return impl_->load_image(impl_->image_.load_stream(bufstream, "gif"));
    else
        return impl_->load_image(impl_->image_.load_stream(bufstream));
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

Size picture::optimal_size(void) const
{
    Size optimal(0, 0);

    if (impl_->frame_index_ < impl_->image_.nframes())
    {
        optimal.width  = impl_->image_.frame(impl_->frame_index_).width();
        optimal.height = impl_->image_.frame(impl_->frame_index_).height();
    }

    return optimal;
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
    impl_->load_image(true);
}

void picture::onPaint(graphics graphics, Rect rect)
{
    if (impl_->image_.nframes() == 0)
        return;

    unsigned int index = impl_->image_.seek(impl_->timer_.elapse());
    if (impl_->frame_index_ != index)
        impl_->frame_index_  = index;

    typedef derived_canvas_adapter<canvas_bgra8&> canvas_adapter;
    const auto& frame = impl_->image_.frame(index);

    if (impl_->scalable_)
    {
        typedef bilinear_interpolation_scale scale;

        if (impl_->keep_ratio_)
        {
            double wratio = static_cast<double>(width()) / height();
            double fratio = static_cast<double>(frame.width()) / frame.height();

            if (wratio < fratio && width() != frame.width())
            {
                canvas_bgra8 canvas = frame;
                unsigned int height = frame.height() * width() / frame.width();
                scale(width(), height).filter(canvas, canvas);
                register int y = ((int)this->height() - (int)height) / 2;
                return graphics.draw_image(0, y, canvas_adapter(canvas));
            }
            else
            if (wratio >= fratio && height() != frame.height())
            {
                canvas_bgra8 canvas = frame;
                unsigned int width = frame.width() * height() / frame.height();
                scale(width, height()).filter(canvas, canvas);
                register int x = ((int)this->width() - (int)width) / 2;
                return graphics.draw_image(x, 0, canvas_adapter(canvas));
            }
        }
        else
        if (width() != frame.width() && height() != frame.height())
        {
            canvas_bgra8 canvas = frame;
            scale(width(), height()).filter(canvas, canvas);
            return graphics.draw_image(canvas_adapter(canvas));
        }
    }

    int x = ((int)width()  - (int)frame.width())  / 2;
    int y = ((int)height() - (int)frame.height()) / 2;

    graphics.draw_image(x, y, frame);
}
