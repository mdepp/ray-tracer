/*
 * File: config.h
 * Authors: Alexander Epp, Rain Epp
 * Project: CMPUT274 Final Project
 * Description: Loads configuration parameters. On the Arduino, this
 *              is done by setting the ARDUINO_ARGS variable at compile
 *              time. On a desktop machine, this is done with command-line
 *              arguments.
 */

#include "util.h" // for cstring functions, forward

namespace config
{
    namespace internal
    {
        #ifdef ARDUINO_BUILD
        /*
         * ARDUINO_ARGS is given not as a string, but as raw text, so stringify it
         */
        #define STRINGIFY(a) XSTRINGIFY(a)
        #define XSTRINGIFY(a) #a
        #ifndef ARDUINO_ARGS
            #define ARDUINO_ARGS_STR ""
        #else
        #define ARDUINO_ARGS_STR STRINGIFY(ARDUINO_ARGS)
        #endif
        #pragma message ("\"" ARDUINO_ARGS_STR "\"")
        
        /*
         * Loads the values from ARDUINO_ARGS_STR into argc and argv. Assumes argv has enough
         * space for MAX_ARC strings that are MAX_ARGL characters long
         */
        void loadArduinoArgs(int& argc, char** argv, const int MAX_ARGC, const int MAX_ARGL)
        {
            util::debugPrint("Argument string: ", ARDUINO_ARGS_STR);
            // Create modifiable version of ARDUINO_ARGS_STR
            char args[sizeof(ARDUINO_ARGS_STR)];
            strcpy(args, ARDUINO_ARGS_STR);
            // argc starts at 1, to match behaviour on desktop machines where
            // the first argument is not a command-line parameter, but rather
            // the executable name
            argc = 1;
            // Split args by whitespace, and load into argv
            char* pch = strtok(args, " \t");
            while (pch != NULL && argc < MAX_ARGC)
            {
                // Using strncat to append to a blank argv[argc] is safer than using strcpy
                // directly, and avoids strncpy's strange behaviour regarding null characters
                strcpy(argv[argc], "");
                strncat(argv[argc], pch, MAX_ARGL);
                ++ argc;
                pch = strtok(NULL, " \t");
            }
        }
    #endif
    
        /*
        * Searches argv for the given flag, and if it is found, call setFnc, passing
        * in the next value in argv
        * 
        * Assumes argv from position 1 onward consists of paired flags and values,
        * starting with a flag. This means that flags must be indexed by odd numbers,
        * and values even numbers.
        */
        template <typename SetFnc>
        void setFromFlags(int argc, char** argv, const char* flag, SetFnc&& setFnc)
        {
            // Start at index 1
            for (int i = 1; i < argc-1; i += 2)
            {
                if (strcmp(argv[i], flag) == 0)
                    setFnc(argv[i+1]);
            }
        }
        /*
        * Calls the previous setFromFlags for each of a variable number of flag and setFnc
        * pairs.
        */
        template <typename SetFnc, typename... Args>
        void setFromFlags(int argc, char* argv[], const char* flag, SetFnc&& setFnc, Args&&... args)
        {
            // Call above setFromFlags function, on the first flag-setFnc pair
            setFromFlags(argc, argv, flag, util::forward<SetFnc>(setFnc));
            // Recursively call this function so that the above statement gets called
            // on all flag-setFnc pairs. If args contains only one pair, this line
            // automatically calls the single-pair version
            setFromFlags(argc, argv, util::forward<Args>(args)...);
        }
    } // end namespace internal


    /*
     * Searches argv for flags, and, if they are found, gives the value to some function.
     * 
     * Assumes 'args' is some number of pairs of flags and functions. e.g.
     * 
     *     setFromFlags(argc, argv, "-echo", [](auto text) { util::debugPrint(text); });
     * 
     * would print 'Hello_World' if run as follows:
     * 
     *     ./appname -echo Hello_World
     * 
     * Values are given to the functions as character arrays, and cannot have spaces.
     * 
     * If this is a desktop build, give setFromFlags argc and argv directly (as
     * the flags are set from command-line parameters). If this is an Arduino build,
     * the given argc and argv values are ignored, and instead the flags are generated
     * by loadArduinoFlags
     */
    template <typename... Args>
    void setFromFlags(int argc, char* argv[], Args&&... args)
    {
        #if defined(ARDUINO_BUILD)
            // Allocate enough memory for 10 50-character arguments
            const int MAX_ARGC = 10, MAX_ARGL = 50;
            char** local_argv = (char**)malloc(MAX_ARGC * sizeof(char*));
            for (int i = 0; i < MAX_ARGC; ++i)
            {
                local_argv[i] = (char*)malloc(MAX_ARGL * sizeof(char));
            }
            // Load data into argc and local_argv
            internal::loadArduinoArgs(argc, local_argv, MAX_ARGC, MAX_ARGL);
            // argc and local_argv are now set equivalently to how they would be in a
            // desktop environment.
            internal::setFromFlags(argc, local_argv, util::forward<Args>(args)...);
            // Free allocated memory.
            for (int i = 0; i < MAX_ARGC; ++i)
            {
                free(local_argv[i]);
            }
            free(local_argv);
        #else
            internal::setFromFlags(argc, argv, util::forward<Args>(args)...);
        #endif
    }
} // end namespace config