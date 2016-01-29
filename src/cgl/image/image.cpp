#include <ctime>
#include <cctype>
#include <string>
#include <map>
#include <takisy/algorithm/stralgo.h>
#include <takisy/core/stretchy_buffer.h>
#include <third_party/stb/stb_image.h>
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
            static bmp bmp; formats_["bmp"] = &bmp;
            static gif gif; formats_["gif"] = &gif;
            static png png; formats_["png"] = &png;

            is_default_formats_registered = true;
        }
    }

public:
    static inline std::string suffix(const std::string& file_path)
    {
        std::string::size_type pos = file_path.find_last_of('.');

        if (pos == std::string::npos)
            return "";
        else
            return stralgo::lowerc(file_path.substr(pos + 1));
    }

private:
    format::frames frames_;
    clock_t load_timestamp_;
    static std::map<std::string, format*> formats_;
};

std::map<std::string, format*> image::implement::formats_;

image::image(void)
    : impl_(new implement)
{}

image::image(const char* uri)
    : image()
{
    load_stream(*stream::from_uri(uri));
}

image::image(const char* uri, const char* suffix)
    : image()
{
    load_stream(*stream::from_uri(uri), suffix);
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
        impl_->frames_ = image.impl_->frames_.clone();

    return *this;
}

format* image::register_format(const char* suffix, format* format)
{
    if (implement::formats_.find(suffix) != implement::formats_.end())
    {
        class format* old_format = implement::formats_[suffix];
        implement::formats_[suffix] = format;
        return old_format;
    }
    else
    {
        implement::formats_[suffix] = format;
        return nullptr;
    }
}

format* image::deregister_format(const char* suffix)
{
    if (implement::formats_.find(suffix) != implement::formats_.end())
    {
        format* format = implement::formats_[suffix];
        implement::formats_.erase(suffix);
        return format;
    }

    return nullptr;
}

bool image::load_uri(const char* uri)
{
    return load_stream(*stream::from_uri(uri));
}

bool image::load_uri(const char* uri, const char* suffix)
{
    return load_stream(*stream::from_uri(uri), suffix);
}

bool image::load_file(const char* file_path)
{
    return load_file(file_path, implement::suffix(file_path).c_str());
}

bool image::load_file(const char* file_path, const char* suffix)
{
    return load_stream(file_stream(file_path, "rb"), suffix);
}

bool image::load_stream(const stream& stream)
{
    typedef struct frame frame_t;
    canvas_adapter::pointer canvas = nullptr;
    stbi_io_callbacks callback;
    int width, height, comp;

    callback.read = [](void* user, char* data, int size) -> int
        { return reinterpret_cast<class stream*>(user)->read(data, size); };
    callback.skip = [](void* user, int n) -> void
        { stretchy_buffer<char> skip_data(n);
          reinterpret_cast<class stream*>(user)->read(skip_data.data(), n); };
    callback.eof  = [](void* user) -> int
        { return reinterpret_cast<class stream*>(user)->working(); };

    stbi_uc* data = stbi_load_from_callbacks(&callback,
                                             &const_cast<class stream&>(stream),
                                             &width, &height, &comp, 0);
    if (!data)
        return false;

    switch (comp)
    {
    case 1: canvas = canvas_adapter::make<canvas_G8>(width, height); break;
    case 2: canvas = canvas_adapter::make<canvas_Ga8>(width, height); break;
    case 3: canvas = canvas_adapter::make<canvas_rgb8>(width, height); break;
    case 4: canvas = canvas_adapter::make<canvas_rgba8>(width, height); break;
    default: return false;
    }

    clear();
    impl_->load_timestamp_ = clock();
    memcpy(canvas->buffer(), data, canvas->bytes());
    impl_->frames_.append(frame_t{.canvas = canvas, .interval = 0});
    stbi_image_free(data);

    return true;
}

bool image::load_stream(const stream& stream, const char* suffix)
{
    if (implement::formats_.find(suffix) != implement::formats_.end())
    {
        clear();
        impl_->load_timestamp_ = clock();
        return implement::formats_[suffix]->load(stream, impl_->frames_);
    }

    return load_stream(stream);
}

bool image::dump_file(const char* file_path) const
{
    file_stream fstream(file_path, "wb");

    return dump_stream(fstream, implement::suffix(file_path).c_str());
}

bool image::dump_stream(stream& stream, const char* suffix) const
{
    if (implement::formats_.find(suffix) != implement::formats_.end())
        return implement::formats_[suffix]->dump(impl_->frames_, stream);

    return dump_stream(stream, "png");
}

canvas_adapter& image::new_frame(unsigned int width, unsigned int height)
{
    return new_frame(width, height, implement::default_interval);
}

canvas_adapter& image::new_frame(unsigned int w, unsigned int h,
                                 unsigned int interval)
{
    return new_frame(canvas_adapter::make<canvas_bgra8>(w, h), interval);
}

canvas_adapter& image::new_frame(const canvas_adapter::pointer& canvas)
{
    return new_frame(canvas, implement::default_interval);
}

canvas_adapter& image::new_frame(const canvas_adapter::pointer& canvas,
                                 unsigned int interval)
{
    typedef struct frame frame_t;

    if (!impl_->frames_.empty() && interval == 0)
        impl_->frames_.back().canvas = canvas;
    else
        impl_->frames_.append(frame_t{.canvas = canvas, .interval = interval});

    return *impl_->frames_.back().canvas;
}

unsigned int image::size(void) const
{
    return impl_->frames_.size();
}

unsigned int image::duration(void) const
{
    unsigned int duration = 1;

    for (const class frame& frame : impl_->frames_)
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
