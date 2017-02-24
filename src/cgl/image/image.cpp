#include <ctime>
#include <vector>
#include <algorithm>
#include <takisy/cgl/image/image.h>

class image::implement
{
    friend class image;

public:
    static const unsigned int default_interval = 1000 / 25;

public:
    implement(void)
        : load_timestamp_(0)
    {
        static bool is_default_formats_registered = false;

        if (!is_default_formats_registered)
        {
         // static png png; formats_.push_back(&png);
            static gif gif; formats_.push_back(&gif);
         // static bmp bmp; formats_.push_back(&bmp);
            static stb stb; formats_.push_back(&stb);

            is_default_formats_registered = true;
        }
    }

private:
    format::frames frames_;
    clock_t load_timestamp_;
    static std::vector<const format*> formats_;
};

std::vector<const format*> image::implement::formats_;

image::image(void)
    : impl_(new implement)
{}

image::image(const char* uri)
    : image()
{
    load_uri(uri);
}

image::image(const char* uri, const format& format)
    : image()
{
    load_uri(uri, format);
}

image::image(const stream& istream)
    : image()
{
    load_stream(istream);
}

image::image(const stream& istream, const format& format)
    : image()
{
    load_stream(istream, format);
}

image::image(const image& _image)
    : image()
{
    operator=(_image);
}

image::~image(void)
{
    delete impl_;
}

image& image::operator=(const image& image)
{
    if (this != &image)
    {
        impl_->load_timestamp_ = image.impl_->load_timestamp_;
        impl_->frames_         = image.impl_->frames_.copy();
    }

    return *this;
}

const format* image::register_format(const format* format)
{
    std::vector<const class format*>::iterator iter =
            std::find(implement::formats_.begin(),
                      implement::formats_.end(), format);

    if (iter != implement::formats_.end())
    {
        std::swap(*iter, format);
        return format;
    }
    else
    {
        implement::formats_.push_back(format);
        return nullptr;
    }
}

void image::deregister_format(const format* format)
{
    std::vector<const class format*>::iterator iter =
            std::find(implement::formats_.begin(),
                      implement::formats_.end(), format);

    if (iter != implement::formats_.end())
        implement::formats_.erase(iter);
}

bool image::load_uri(const char* uri)
{
    std::shared_ptr<stream> streamptr = stream::from_uri(uri);

    if (!streamptr)
        return false;

    return load_stream(*streamptr);
}

bool image::load_uri(const char* uri, const format& format)
{
    std::shared_ptr<stream> streamptr = stream::from_uri(uri);

    if (!streamptr)
        return false;

    return load_stream(*streamptr, format);
}

bool image::load_stream(const stream& istream)
{
    seek_stream skstream(istream);
    const stream* stptr;

    if (istream.seekable())
        stptr = &istream;
    else
        stptr = &skstream;

    for (const format* format : implement::formats_)
    {
        stptr->seek(0, stream::stBegin);

        if (load_stream(*stptr, *format))
            return true;
    }

    return false;
}

bool image::load_stream(const stream& istream, const format& format)
{
    format::frames frames;
    bool loaded = format.load(istream, frames);

    if (loaded || impl_->frames_.empty())
    {
        impl_->frames_         = frames;
        impl_->load_timestamp_ = clock();
    }

    return loaded;
}

bool image::dump_uri(const char* uri) const
{
    return dump_uri(uri, png());
}

bool image::dump_uri(const char* uri, const format& format) const
{
    std::shared_ptr<stream> streamptr = stream::from_uri(uri);

    if (!streamptr)
        return false;

    return dump_stream(*streamptr, format);
}

bool image::dump_stream(stream& ostream) const
{
    return dump_stream(ostream, png());
}

bool image::dump_stream(stream& ostream, const format& format) const
{
    return format.dump(impl_->frames_, ostream);
}

canvas_adapter& image::new_frame(unsigned int width, unsigned int height)
{
    return new_frame(width, height, implement::default_interval);
}

canvas_adapter& image::new_frame(unsigned int width, unsigned int height,
                                 unsigned int interval)
{
    return new_frame(canvas_adapter::make<canvas_bgra8>(width, height),
                     interval);
}

canvas_adapter& image::new_frame(const canvas_adapter::pointer& canvas)
{
    return new_frame(canvas, implement::default_interval);
}

canvas_adapter& image::new_frame(const canvas_adapter::pointer& canvas,
                                 unsigned int interval)
{
    if (!impl_->frames_.empty() && interval == 0)
        impl_->frames_.back().canvas = canvas;
    else
        impl_->frames_.append({.canvas = canvas, .interval = interval});

    return *impl_->frames_.back().canvas;
}

unsigned int image::count(void) const
{
    return impl_->frames_.size();
}

unsigned int image::duration(void) const
{
    unsigned int duration = 1;

    for (const format::frame& frame : impl_->frames_)
        duration += frame.interval;

    return duration;
}

unsigned int image::seek(unsigned int timestamp) const
{
    unsigned int frame_index = 0;

    timestamp %= duration();

    while (frame_index < impl_->frames_.size()
           && timestamp > impl_->frames_[frame_index].interval)
        timestamp -= impl_->frames_[frame_index++].interval;

    return frame_index;
}

canvas_adapter& image::first_frame(void)
{
    return *impl_->frames_.front().canvas;
}

const canvas_adapter& image::first_frame(void) const
{
    return *impl_->frames_.front().canvas;
}

canvas_adapter& image::frame(void)
{
    return *impl_->frames_[seek(clock() - impl_->load_timestamp_)].canvas;
}

const canvas_adapter& image::frame(void) const
{
    return *impl_->frames_[seek(clock() - impl_->load_timestamp_)].canvas;
}

canvas_adapter& image::frame(unsigned int frame_index)
{
    return *impl_->frames_[frame_index].canvas;
}

const canvas_adapter& image::frame(unsigned int frame_index) const
{
    return *impl_->frames_[frame_index].canvas;
}

canvas_adapter& image::last_frame(void)
{
    return *impl_->frames_.back().canvas;
}

const canvas_adapter& image::last_frame(void) const
{
    return *impl_->frames_.back().canvas;
}

unsigned int image::width(void) const
{
    return first_frame().width();
}

unsigned int image::height(void) const
{
    return first_frame().height();
}

color image::pixel(unsigned int x, unsigned int y) const
{
    return first_frame().pixel(x, y);
}

void  image::pixel(unsigned int x, unsigned int y, const color& color)
{
    first_frame().pixel(x, y, color);
}

void image::clear(void)
{
    impl_->frames_.clear();
}
