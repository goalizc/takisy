#ifndef crc_h_20140313
#define crc_h_20140313

class crc32
{
    class implement;

public:
    crc32(void);
    crc32(const void* data, unsigned int length);
    crc32(const crc32& crc);
   ~crc32(void);

    crc32& operator=(const crc32& crc);

public:
    void         update(const void* data, unsigned int length);
    unsigned int digest(void) const;

private:
    class implement* impl_;
};

#endif // crc_h_20140313
