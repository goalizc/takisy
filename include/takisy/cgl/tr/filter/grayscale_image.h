#ifndef grayscale_image_h_20140926
#define grayscale_image_h_20140926

class grayscale_image
{
public:
    template <typename Canvas>
    void filter(const Canvas& src, Canvas& dst) const
    {
        dst.resize(src.width(), src.height());

        const typename Canvas::stretchy_buffer& src_pixels = src.pixels();
              typename Canvas::stretchy_buffer& dst_pixels = dst.pixels();

        for (unsigned int i = 0; i < src_pixels.size(); ++i)
            dst_pixels[i] = src_pixels[i].grayscale_color();
    }
};

#endif // grayscale_image_h_20140926
