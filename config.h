
#include "util.h"

namespace config
{

#ifdef ARDUINO_BUILD
    #define STRINGIFY(a) XSTRINGIFY(a)
    #define XSTRINGIFY(a) #a
    #ifndef ARDUINO_ARGS
        #define ARDUINO_ARGS_STR ""
    #else
    #define ARDUINO_ARGS_STR STRINGIFY(ARDUINO_ARGS)
    #endif
    
    void loadArduinoArgs(int& argc, char** argv)
    {
        util::debugPrint("Argument string: ", ARDUINO_ARGS_STR);
        char args[sizeof(ARDUINO_ARGS_STR)];
        strcpy(args, ARDUINO_ARGS_STR);
        argc = 1;
        char* pch = strtok(args, " \t");
        while (pch != NULL)
        {
            strcpy(argv[argc], pch);
            ++ argc;
            pch = strtok(NULL, " \t");
        }
    }
#endif

    template <typename SetFnc>
    void setFromFlags(int argc, char** argv, const char* flag, SetFnc&& setFnc)
    {
        for (int i = 1; i < argc-1; i += 2)
        {
            if (strcmp(argv[i], flag) == 0)
                setFnc(argv[i+1]);
        }
    }
    template <typename SetFnc, typename... Args>
    void setFromFlags(int argc, char* argv[], const char* flag, SetFnc&& setFnc, Args&&... args)
    {
        #if defined(ARDUINO_BUILD) && !defined(LOADED_ARDUINO_ARGS)
            char** local_argv = (char**)malloc(10 * sizeof(char*));
            for (int i = 0; i < 10; ++i)
            {
                local_argv[i] = (char*)malloc(50 * sizeof(char));
            }
            loadArduinoArgs(argc, local_argv);
            setFromFlags(argc, local_argv, flag, util::forward<SetFnc>(setFnc));
            setFromFlags(argc, local_argv, util::forward<Args>(args)...);
            free(local_argv);
            #define LOADED_ARDUINO_ARGS
        #else
            setFromFlags(argc, argv, flag, util::forward<SetFnc>(setFnc));
            setFromFlags(argc, argv, util::forward<Args>(args)...);
        #endif
    }
}