#ifndef stretchy_buffer_h_20140319
#define stretchy_buffer_h_20140319

#include <new>
#include <cstdlib>
#include <cstring>
#include <initializer_list>

template <typename ElementType,
          bool blNeedConstruct = false,
          bool blNeedDestruct  = blNeedConstruct,
          bool blNeedSet0      = true>
class stretchy_buffer
{
    static constexpr unsigned int element_size = sizeof(ElementType);

public:
    typedef ElementType element_type;
    typedef std::initializer_list<element_type> initializer_list;

public:
    inline stretchy_buffer(void)
        : h_(nullptr)
    {}

    inline stretchy_buffer(unsigned int size)
        : stretchy_buffer()
    {
        resize(size);
    }

    inline stretchy_buffer(const element_type* elements, unsigned int size)
        : stretchy_buffer()
    {
        append(elements, size);
    }

    inline stretchy_buffer(initializer_list il)
        : stretchy_buffer()
    {
        append(il.begin(), il.size());
    }

    inline stretchy_buffer(const stretchy_buffer& sb)
        : h_(sb.h_)
    {
        if (h_)
            ++h_->rc;
    }

    inline ~stretchy_buffer(void)
    {
        free();
    }

    inline stretchy_buffer& operator=(const stretchy_buffer& sb)
    {
        if (this != &sb)
        {
            free();

            if ((h_ = sb.h_))
                ++h_->rc;
        }

        return *this;
    }

public:
    inline unsigned int capacity(void) const
    {
        return h_ ? h_->c : 0;
    }

    inline unsigned int size(void) const
    {
        return h_ ? h_->s : 0;
    }

    inline unsigned int reference_count(void) const
    {
        return h_ ? h_->rc : 0;
    }

    inline element_type& front(void)
    {
        return operator[](0);
    }

    inline const element_type& front(void) const
    {
        return operator[](0);
    }

    inline element_type& back(void)
    {
        return operator[](h_->s - 1);
    }

    inline const element_type& back(void) const
    {
        return operator[](h_->s - 1);
    }

    inline element_type* begin(void)
    {
        return h_ ? h_->b : nullptr;
    }

    inline const element_type* begin(void) const
    {
        return h_ ? h_->b : nullptr;
    }

    inline element_type* end(void)
    {
        return h_ ? h_->b + h_->s : nullptr;
    }

    inline const element_type* end(void) const
    {
        return h_ ? h_->b + h_->s : nullptr;
    }

    inline element_type& operator[](unsigned int index)
    {
        return h_->b[index];
    }

    inline const element_type& operator[](unsigned int index) const
    {
        return h_->b[index];
    }

    inline element_type* data(void)
    {
        return h_ ? h_->b : nullptr;
    }

    inline const element_type* data(void) const
    {
        return h_ ? h_->b : nullptr;
    }

public:
    inline bool null(void) const
    {
        return !h_;
    }

    inline bool empty(void) const
    {
        return null() || h_->s == 0;
    }

public:
    inline void recapacity(unsigned int capacity)
    {
        if (!h_)
        {
            h_     = (heart*)::malloc(sizeof(heart));
            h_->c  = capacity < 8 ? 8 : capacity;
            h_->s  = 0;
            h_->rc = 1;
            h_->b  = (element_type*)::malloc(element_size * h_->c);

            if (blNeedConstruct)
                for (unsigned int i = 0; i < h_->c; ++i)
                    new (&h_->b[i]) element_type();
            else
            if (blNeedSet0)
                ::memset(h_->b, 0, h_->c * element_size);
        }
        else if (capacity > h_->c)
        {
            unsigned int nc = h_->c;
            do { nc <<= 1; } while (nc < capacity);
            h_->b = (element_type*)::realloc(h_->b, element_size * nc);

            if (blNeedConstruct)
                for (unsigned int i = h_->c; i < nc; ++i)
                    new (&h_->b[i]) element_type();
            else
            if (blNeedSet0)
                ::memset(h_->b + h_->c, 0, (nc - h_->c) * element_size);

            h_->c = nc;
        }
    }

    inline void resize(unsigned int size)
    {
        recapacity(size);
        h_->s = size;
    }

    inline stretchy_buffer clone(void) const
    {
        stretchy_buffer foobar;

        if (!empty())
            foobar.append(h_->b, h_->s);

        return foobar;
    }

    inline void swap(stretchy_buffer& sb)
    {
        heart* h  =    h_;
               h_ = sb.h_;
            sb.h_ =    h;
    }

    inline void clear(void)
    {
        if (h_)
            h_->s = 0;
    }

    inline void insert(unsigned int index, const element_type& value)
    {
        insert(index, &value, 1);
    }

    inline void insert(unsigned int index, const element_type* values,
                       unsigned int count)
    {
        if (index >= size())
            append(values, count);
        else
        {
            recapacity(h_->s + count);
            move(begin() + index, begin() + index + count, h_->s - index);
            move(values, begin() + index, count);
            h_->s += count;
        }
    }

    inline void insert(unsigned int index, initializer_list il)
    {
        insert(index, il.begin(), il.size());
    }

    inline void append(const element_type& value)
    {
        append(&value, 1);
    }

    inline void append(const element_type* values, unsigned int count)
    {
        recapacity(size() + count);
        move(values, h_->b + h_->s, count);
        h_->s += count;
    }

    inline void append(initializer_list il)
    {
        append(il.begin(), il.size());
    }

    inline void remove(void)
    {
        if (!empty())
            --h_->s;
    }

    inline void remove(unsigned int index)
    {
        if (index < size())
        {
            move(h_->b + index + 1, h_->b + index, h_->s - index - 1);
            --h_->s;
        }
    }

    inline void remove(unsigned int from ,unsigned int to)
    {
        if (from == to)
            return;

        if (from > to)
            from ^= to ^= from ^= to;

        if (from < size())
        {
            if (to > h_->s)
                to = h_->s;

            move(begin() + to, begin() + from, h_->s - to);
            h_->s -= to - from;
        }
    }

private:
    inline void move(const element_type* from,
                           element_type* to, unsigned int count)
    {
        if (from == to)
            return;

        if (blNeedConstruct)
        {
            if (from > to)
                for (unsigned int i = count - 1; i >= 0; --i)
                    to[i] = from[i];
            else
                for (unsigned int i = 0; i < count; ++i)
                    to[i] = from[i];
        }
        else
            ::memmove(to, from, element_size * count);
    }

    inline void free(void)
    {
        if (!h_)
            return;

        if (--h_->rc == 0)
        {
            if (blNeedDestruct)
                for (unsigned int i = 0; i < h_->c; ++i)
                    h_->b[i].~element_type();

            ::free(h_->b);
            ::free(h_);
        }

        h_ = nullptr;
    }

private:
    struct heart
    {
        unsigned int  c, s, rc;
        element_type *b;
    } *h_;
};

#endif // stretchy_buffer_h_20140319
