#ifndef stretchy_buffer_h_20140319
#define stretchy_buffer_h_20140319

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <initializer_list>
#include <new>
#include <string>

template <typename T,
          bool bc = false, bool b0 = false,
          bool bd = bc>
struct stretchy_buffer {
    typedef T value_type;
public:
    stretchy_buffer(void)
        : c_(nullptr) {}
    stretchy_buffer(unsigned size)
        : stretchy_buffer()
        { resize(size); }
    stretchy_buffer(const T* values, unsigned size)
        : stretchy_buffer()
        { append(values, size); }
    stretchy_buffer(std::initializer_list<T> initlist)
        : stretchy_buffer(initlist.begin(), initlist.size()) {}
    stretchy_buffer(const std::string& string)
        : stretchy_buffer((const T*)string.data(), string.size()) {}
    stretchy_buffer(const stretchy_buffer& stbuf)
        : c_(stbuf.c_)
        { if (c_) ++c_->rc; }
    stretchy_buffer(stretchy_buffer&& stbuf)
        : c_(stbuf.c_)
        { stbuf.c_ = nullptr; }
   ~stretchy_buffer(void)
        { release(); }

    stretchy_buffer& operator=(const stretchy_buffer& stbuf) {
        if (this != &stbuf) {
            release();
            if ((c_ = stbuf.c_))
                ++c_->rc;
        }
        return *this;
    }

    stretchy_buffer& operator=(stretchy_buffer&& stbuf) {
        return swap(stbuf), *this;
    }

public:
    operator std::string(void) const {
        if (empty())
            return std::string();
        else
            return std::string((const char*)data(), size() * sizeof(T));
    }

public:
    bool null(void)  const { return !c_; }
    bool empty(void) const { return null() || c_->s == 0; }

public:
    unsigned capacity(void) const { return c_ ? c_->c  : 0; }
    unsigned size(void)     const { return c_ ? c_->s  : 0; }
    unsigned refcount(void) const { return c_ ? c_->rc : 0; }

    T& operator[](unsigned index) { return c_->b[index]; }
    T& front(void) { return operator[](0); }
    T& back(void)  { return operator[](c_->s - 1); }
    T* begin(void) { return c_ ? c_->b : nullptr; }
    T* end(void)   { return c_ ? c_->b + c_->s : nullptr; }
    T* data(void)  { return c_ ? c_->b : nullptr; }

    const T& operator[](unsigned index) const { return c_->b[index]; }
    const T& front(void) const { return operator[](0); }
    const T& back(void)  const { return operator[](c_->s - 1); }
    const T* begin(void) const { return c_ ? c_->b : nullptr; }
    const T* end(void)   const { return c_ ? c_->b + c_->s : nullptr; }
    const T* data(void)  const { return c_ ? c_->b : nullptr; }

public:
    void recapacity(unsigned capacity) {
        if (!c_) {
            c_     = (core*)malloc(sizeof(core));
            c_->c  = capacity < 8 ? 8 : capacity;
            c_->s  = 0;
            c_->rc = 1;
            c_->b  = (T*)malloc(sizeof(T) * c_->c);
            if (bc) new (c_->b) T [c_->c];
            else if (b0) memset(c_->b, 0, c_->c * sizeof(T));
        } else if (capacity > c_->c) {
            unsigned nc = c_->c;
            do { nc <<= 1; } while (nc < capacity);
            c_->b = (T*)realloc(c_->b, sizeof(T) * nc);
            if (bc) new (c_->b + c_->c) T [nc - c_->c];
            else if (b0) memset(c_->b + c_->c, 0, (nc - c_->c) * sizeof(T));
            c_->c = nc;
        }
    }

    void resize(unsigned size) {
        recapacity(size); c_->s = size;
    }

    stretchy_buffer copy(void) const {
        stretchy_buffer stbuf;
        if (!empty()) stbuf.append(c_->b, c_->s);
        return stbuf;
    }

    void swap(stretchy_buffer& stbuf) {
        std::swap(c_, stbuf.c_);
    }
    void clear(void) {
        if (c_) c_->s = 0;
    }

    void insert(unsigned index, const T& value) {
        insert(index, &value, 1);
    }

    void insert(unsigned index, const T* values, unsigned count) {
        if (index >= size())
            append(values, count);
        else {
            recapacity(c_->s + count);
            move(begin() + index, begin() + index + count, c_->s - index);
            move(values, begin() + index, count);
            c_->s += count;
        }
    }

    void insert(unsigned index, std::initializer_list<T> initlist) {
        insert(index, initlist.begin(), initlist.size());
    }

    void append(const T& value) {
        append(&value, 1);
    }

    void append(const T* values, unsigned count) {
        recapacity(size() + count);
        move(values, c_->b + c_->s, count);
        c_->s += count;
    }

    void append(std::initializer_list<T> initlist) {
        append(initlist.begin(), initlist.size());
    }

    void remove(void) {
        if (!empty()) --c_->s;
    }

    void remove(unsigned index) {
        if (index < size()) {
            move(c_->b + index + 1, c_->b + index, c_->s - index - 1);
            --c_->s;
        }
    }

    void remove(unsigned from ,unsigned to) {
        if (from == to)
            return;
        else if (from > to)
            std::swap(from, to);
        if (from < size()) {
            if (to > c_->s) to = c_->s;
            move(begin() + to, begin() + from, c_->s - to);
            c_->s -= to - from;
        }
    }

private:
    void move(const T* from, T* to, int count) {
        if (from == to) return;
        if (bc) {
            if (from > to) for (int i = count - 1; i >= 0; --i)
                to[i] = from[i];
            else for (int i = 0; i < count; ++i)
                to[i] = from[i];
        } else memmove(to, from, sizeof(T) * count);
    }

    void release(void) {
        if (!c_) return;
        if (--c_->rc == 0) {
            if (bd) for (unsigned i = 0; i < c_->c; ++i)
                c_->b[i].~T();
            free(c_->b);
            free(c_);
        }
        c_ = nullptr;
    }

private:
    struct core { unsigned c, s, rc; T *b; } *c_;
};

#endif // stretchy_buffer_h_20140319
