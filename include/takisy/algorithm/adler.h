#ifndef adler_h_20140324
#define adler_h_20140324

class adler32
{
    class implement;

public:
    adler32(void);
    adler32(const void* data, unsigned int length);
    adler32(const adler32& adler);
   ~adler32(void);

    adler32& operator=(const adler32& adler);

public:
    void         update(const void* data, unsigned int length);
    unsigned int digest(void) const;

private:
    class implement* impl_;
};

#endif // adler_h_20140324
