#ifndef file_h_20140328
#define file_h_20140328

#include <cstdio>

class file
{
public:
    file(FILE* fp) : fp_(fp) {}
    file(const char* filepath, const char* mode) : fp_(fopen(filepath, mode)) {}
    file(const file& f) : fp_(f.fp_) { f.fp_ = nullptr; }
   ~file(void) { if (fp_) fclose(fp_); }

    file& operator=(const file& f) {
        if (this != &f) {
            if (fp_) fclose(fp_);
            fp_ = f.fp_; f.fp_ = nullptr;
        }
        return *this;
    }

public:
    operator FILE*(void) const { return fp_; }

private:
    mutable FILE* fp_ = nullptr;
};

#endif // file_h_20140328
