#ifdef DLL
#include <dlfcn.h>
#include <stdio.h>
    void load(char* path) {
        void* handle = dlopen(path, RTLD_LAZY);
        if(handle == NULL)  {
            printf("[CLI - DLL LOADER] NIE UDALO SIE ZALADOWAC BIBLIOTEKI\n");
            return;
        }

        *(void**) (&collecting_data_init) = dlsym(handle, "collecting_data_init");
        *(void**) (&collecting_data_new) = dlsym(handle, "collecting_data_new");
        *(void**) (&collecting_data_add) = dlsym(handle, "collecting_data_add");
        *(void**) (&collecting_data_get) = dlsym(handle, "collecting_data_get");
        *(void**) (&collecting_data_free_index) = dlsym(handle, "collecting_data_free_index");
        *(void**) (&collecting_data_destruct) = dlsym(handle, "collecting_data_destruct");
    }
#else
    void load(char* path) {}
#endif