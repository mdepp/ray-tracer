#include "util.h"
#include "vec.h"

namespace util
{
#ifdef ARDUINO_BUILD
		int32_t random(int32_t max)
		{
				return ::random(max);
		}
		void randomSeed()
		{
				::randomSeed(getSeed<int32_t, sizeof(int32_t)>(0, 50));
		}

		uint8_t* getStackPointer()
    {
        return (uint8_t *)(SP);
    }
		uint32_t getAvailableMemory()
    {
        static uint8_t* stackptr, *heapptr;
        stackptr = (uint8_t *)malloc(4); // use stackptr temporarily
        heapptr = stackptr; // save value of heap pointer
        free(stackptr); // free up the memory again (sets stackptr to 0)
        stackptr = (uint8_t *)(SP); // save value of stack pointer
        return (stackptr-heapptr);
    }
	#else
		int32_t random(int32_t maxval)
		{
				return std::rand()%maxval;
		}
		void randomSeed()
		{
				std::srand(std::time(0));
		}

		uint8_t* getStackPointer()
    {
        return nullptr;
    }
		uint32_t getAvailableMemory()
    {
        return 0;
    }
#endif
}
