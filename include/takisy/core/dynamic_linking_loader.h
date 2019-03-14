#ifndef dynamic_linking_loader_h_20141024
#define dynamic_linking_loader_h_20141024

typedef class dynamic_linking_loader
{
public:
    static void* dlsymbol(const char* filename, const char* symbol);

    template <typename ReturnType>
    static inline ReturnType dlsymbol(const char* filename, const char* symbol)
    {
        return reinterpret_cast<ReturnType>(dlsymbol(filename, symbol));
    }
} dll;

#endif // dynamic_linking_loader_h_20141024
