#include "Handle_Unexpected_Exceptions.h"
#include <iostream>


/*
 * Called by all catch(...) blocks in an attempt to show a reasonable message.
 * Copied from C++ Refference https://en.cppreference.com/
 */
void handle_unexpected_eptr(std::exception_ptr eptr) // passing by value is OK
{
    try
    {
        if (eptr)
        {
            std::rethrow_exception(eptr);
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << "Caught unexpected exception: '" << e.what() << "'\n";
    }
}
