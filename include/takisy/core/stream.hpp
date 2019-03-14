#ifndef stream_hpp_20180717
#define stream_hpp_20180717

#include <cstring>
#include <stdexcept>
#include <vector>

class istream {
public:
    istream(void)
        : i_(0)
    {}

    virtual ~istream(void) {}

protected:
    virtual bool nostock(void) const = 0;
    virtual unsigned long read(void* buffer, unsigned long length) const = 0;

public:
    unsigned char peek(void) const {
        if (i_ >= buffer_.size()) {
            i_ = 0;
            buffer_.resize(64 * 1024);
            buffer_.resize(read(buffer_.data(), buffer_.size()));
            if (buffer_.empty())
                throw std::runtime_error("end of stream.");
        }

        return buffer_[i_];
    }

    unsigned char get(void) const {
        unsigned char ch = peek();
        ++i_;
        return ch;
    }

    unsigned long get(void* buffer, unsigned long length) const {
        unsigned long l = buffer_.size() - i_;

        if (l < length) {
            if (l) {
                memcpy(buffer, buffer_.data() + i_, l);
                i_ += l;
                return l + read((unsigned char*)buffer + l, length - l);
            }
            return read(buffer, length);
        } else if (length) {
            memcpy(buffer, buffer_.data() + i_, length);
            i_ += length;
            return  length;
        }

        return 0;
    }

private:
    mutable unsigned long i_;
    mutable std::vector<unsigned char> buffer_;
};

class ostream {
public:
    virtual ~ostream(void) {}
    virtual unsigned long write(const void* buffer, unsigned long length) = 0;

public:
    void safe_write(const void* buffer, unsigned long length) {
        unsigned char *ucbuffer = buffer;
        unsigned long offset = 0;

        while (offset < length) {
            unsigned long wl = write(ucbuffer + offset, length - offset);

            if (wl)
                offset += wl;
            else
                throw std::runtime_error("write error.");
        }
    }

    unsigned long obtain(const istream& is, unsigned long szbuff = 64 * 1024) {
        std::vector<char> buffer(szbuff);
        unsigned long length = 0;

        while (unsigned long rl = is.get(buffer.data(), buffer.size())) {
            length += rl;
            safe_write(buffer.data(), rl);
        }

        return length;
    }
};

struct buffer_stream : public istream, public ostream {
    mutable unsigned long tell = 0;
    mutable std::vector<unsigned char> buffer;

public:
    bool nostock(void) const override {
        return tell >= buffer.size();
    }

    unsigned long read(void* buf, unsigned long len) const override {
        unsigned long left = buffer.size() - tell;
        if (left < len) len = left;
        memcpy(buf, buffer.data() + tell, len);
        tell += len;
        return len;
    }

    unsigned long write(const void* buf, unsigned long len) override {
        unsigned long offset = buffer.size();
        buffer.resize(buffer.size() + len);
        memcpy(buffer.data() + offset, buf, len);
        return len;
    }
};

class ifile_stream : public istream {
public:
    ifile_stream(const char* filepath)
        : file_(filepath, "rb")
    {}

public:
    bool nostock(void) const override {
        return !file_ || feof(file_);
    }

    unsigned long read(void* buffer, unsigned long length) const override {
        return fread(buffer, 1, length, file_);
    }

private:
    file file_;
};

class ofile_stream : public ostream {
public:
    ofile_stream(const char* filepath, bool app = false)
        : file_(filepath, app ? "ab" : "wb")
    {}

public:
    unsigned long write(const void* buffer, unsigned long length) override {
        return fwrite(buffer, 1, length, file_);
    }

private:
    file file_;
};

#endif // stream_hpp_20180717
