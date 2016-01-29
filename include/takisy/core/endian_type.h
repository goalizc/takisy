#ifndef endian_type_h_20131104
#define endian_type_h_20131104

enum EndianType
{
    etBigEndian    = 0,
    etLittleEndian = 1,
};

// ValueType must be integer type.
template <typename ValueType, EndianType endianType = etBigEndian>
class endian_type
{
    typedef endian_type<ValueType, endianType> self_type;
    typedef unsigned int uint;

public:
    endian_type(void)
        : value_(0)
    {}

    endian_type(ValueType value)
        : value_(transform(value))
    {}

    template <typename ValueType2, EndianType endianType2>
    endian_type(const endian_type<ValueType2, endianType2>& et)
        : value_(transform(et.value()))
    {}

    ~endian_type(void) {}

    template <typename ValueType2, EndianType endianType2>
    endian_type& operator=(const endian_type<ValueType2, endianType2>& et)
    {
        value(et.value());
    }

public:
    inline self_type& value(ValueType value)
    {
        value_ = transform(value);

        return *this;
    }

    inline ValueType value() const
    {
        return transform(value_);
    }

    inline self_type& endian_value(ValueType value)
    {
        value_ = value;

        return *this;
    }

    inline ValueType endian_value(void) const
    {
        return value_;
    }

    inline endian_type operator=(ValueType _value)
    {
        value(_value);

        return *this;
    }

public:
    inline self_type& operator+=(ValueType other)
    {
        return value(value() + other);
    }

    inline self_type& operator-=(ValueType other)
    {
        return value(value() - other);
    }

    inline self_type& operator*=(ValueType other)
    {
        return value(value() * other);
    }

    inline self_type& operator/=(ValueType other)
    {
        return value(value() / other);
    }

    inline self_type& operator%=(ValueType other)
    {
        return value(value() % other);
    }

    inline self_type& operator&=(ValueType other)
    {
        return value(value() & other);
    }

    inline self_type& operator^=(ValueType other)
    {
        return value(value() ^ other);
    }

    inline self_type& operator|=(ValueType other)
    {
        return value(value() | other);
    }

    inline self_type& operator>>=(uint shift)
    {
        return value(value() >> shift);
    }

    inline self_type& operator<<=(uint shift)
    {
        return value(value() << shift);
    }

    inline self_type& operator++(void)
    {
        return value(value() + 1);
    }

    inline self_type& operator--(void)
    {
        return value(value() - 1);
    }

    inline ValueType operator++(int)
    {
        return value(value() + 1) - 1;
    }

    inline ValueType operator--(int)
    {
        return value(value() - 1) + 1;
    }

    inline operator ValueType(void) const
    {
        return value();
    }

private:
    static inline ValueType transform(ValueType value)
    {
        static const unsigned short foobar = 0xff00;
        static const EndianType local_endian_type =
            *reinterpret_cast<const unsigned char*>(&foobar) == 0xff
                                            ? etBigEndian : etLittleEndian;

        if (local_endian_type != endianType)
        {
            unsigned char* first = reinterpret_cast<unsigned char*>(&value);
            unsigned char* last  = first + sizeof(value) - 1;

            while (first < last)
                *first ^= *last ^= *first ^= *last, ++first, --last;
        }

        return value;
    }

private:
    ValueType value_;
};

// etlc: location endian type
typedef             char                                etlc_char;
typedef             signed char                         etlc_sint8;
typedef             signed short                        etlc_sint16;
typedef             signed int                          etlc_sint32;
typedef             signed long                         etlc_slong;
typedef             signed long long                    etlc_sint64;
typedef             unsigned char                       etlc_uint8;
typedef             unsigned short                      etlc_uint16;
typedef             unsigned int                        etlc_uint32;
typedef             unsigned long                       etlc_ulong;
typedef             unsigned long long                  etlc_uint64;

typedef             char                                etbe_char;
typedef             signed char                         etbe_sint8;
typedef endian_type<signed short,       etBigEndian>    etbe_sint16;
typedef endian_type<signed int,         etBigEndian>    etbe_sint32;
typedef endian_type<signed long,        etBigEndian>    etbe_slong;
typedef endian_type<signed long long,   etBigEndian>    etbe_sint64;
typedef             unsigned char                       etbe_uint8;
typedef endian_type<unsigned short,     etBigEndian>    etbe_uint16;
typedef endian_type<unsigned int,       etBigEndian>    etbe_uint32;
typedef endian_type<unsigned long,      etBigEndian>    etbe_ulong;
typedef endian_type<unsigned long long, etBigEndian>    etbe_uint64;

typedef             char                                etle_char;
typedef             signed char                         etle_sint8;
typedef endian_type<signed short,       etLittleEndian> etle_sint16;
typedef endian_type<signed int,         etLittleEndian> etle_sint32;
typedef endian_type<signed long,        etLittleEndian> etle_slong;
typedef endian_type<signed long long,   etLittleEndian> etle_sint64;
typedef             unsigned char                       etle_uint8;
typedef endian_type<unsigned short,     etLittleEndian> etle_uint16;
typedef endian_type<unsigned int,       etLittleEndian> etle_uint32;
typedef endian_type<unsigned long,      etLittleEndian> etle_ulong;
typedef endian_type<unsigned long long, etLittleEndian> etle_uint64;

#endif //endian_type_h_20131104
