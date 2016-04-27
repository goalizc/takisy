#include <map>
#include <string>
#include <takisy/core/macro.h>
#include <takisy/core/dynamic_linking_loader.h>

#ifdef __os_win__
    #include <Windows.h>
    #define dlopen  LoadLibrary
    #define dlclose FreeLibrary
    #define dlsym   GetProcAddress
#else
    #include <dlfcn.h>
    #define HMODULE void*
    #define dlopen(filename) dlopen(filename, RTLD_LAZY)
#endif

void* dynamic_linking_loader::dlsymbol(const char* filename, const char* symbol)
{
    static class manager
    {
    public:
        ~manager(void)
        {
            for (const decltype(objects_)::value_type& object : objects_)
                dlclose(object.second);
        }

    public:
        void* dlsymbol(const char* filename, const char* symbol)
        {
            if (objects_.find(filename) == objects_.end())
            {
                HMODULE id = dlopen(filename);

                if (!id)
                    return nullptr;

                objects_[filename] = id;
            }

            return (void*)dlsym(objects_[filename], symbol);
        }

    private:
        std::map<std::string, HMODULE> objects_;
    } manager;

    return manager.dlsymbol(filename, symbol);
}
