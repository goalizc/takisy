#ifndef grayscale_h_20140926
#define grayscale_h_20140926

class grayscale
{
public:
    template <typename Canvas>
    void filter(const Canvas& src, Canvas& dst) const
    {
        dst.resize(src.width(), src.height());

        const typename Canvas::pixels_type& src_pixels = src.pixels();
              typename Canvas::pixels_type& dst_pixels = dst.pixels();

        for (unsigned int i = 0; i < src_pixels.size(); ++i)
            dst_pixels[i].grayscale(src_pixels[i].grayscale());
    }
};

#endif // grayscale_h_20140926
