#define  STB_IMAGE_IMPLEMENTATION
#include <third_party/stb/stb_image.h>
#include <takisy/core/stretchy_buffer.h>
#include <takisy/cgl/image/format.h>

bool stb::load(stream& stream, frames& frames) const
{
    canvas_adapter::pointer ca = nullptr;
    stbi_io_callbacks callback;
    int width, height, comp;

    callback.read = [](void* user, char* data, int size) -> int
        { return reinterpret_cast<class stream*>(user)->read(data, size); };
    callback.skip = [](void* user, int n) -> void
        { stretchy_buffer<char> skip_data(n);
          reinterpret_cast<class stream*>(user)->read(skip_data.data(), n); };
    callback.eof  = [](void* user) -> int
        { return reinterpret_cast<class stream*>(user)->readable(); };

    stbi_uc* data = stbi_load_from_callbacks(&callback,
                                             &const_cast<class stream&>(stream),
                                             &width, &height, &comp, 0);
    if (!data)
        return false;

    switch (comp)
    {
    case 1: ca = canvas_adapter::make<canvas_G8>(width, height); break;
    case 2: ca = canvas_adapter::make<canvas_Ga8>(width, height); break;
    case 3: ca = canvas_adapter::make<canvas_rgb8>(width, height); break;
    case 4: ca = canvas_adapter::make<canvas_rgba8>(width, height); break;
    default: return false;
    }

    memcpy(ca->buffer(), data, ca->bytes());
    frames.append(frame {.canvas = ca, .interval = 0});
    stbi_image_free(data);

    return true;
}

bool stb::dump(const frames& frames, stream& stream) const
{
    return false;
}

