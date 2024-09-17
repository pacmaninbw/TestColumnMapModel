#include <algorithm>
#include "Handle_Unexpected_Exceptions.h"
#include <iostream>
#include <string>
#include <vector>
#include "modelcolumntotablecolumnmap.h"
#include "UnitTesting.h"

int main()
{
    int exitTestStatus = EXIT_SUCCESS;
    UnitTesting unitTests;
    
    unitTests.getUserTestConfiguration();

    try {
        exitTestStatus = unitTests.executeAllUnitTests();
    }
    catch (std::exception &ex) {
        std::cerr << "Exception Caught: " << ex.what() << "\n";
        exitTestStatus = EXIT_FAILURE;
    }
    catch (std::string &sex)
    {
        std::cerr << "String Exception Caught: " << sex << "\n";
        exitTestStatus = EXIT_FAILURE;
    }
    catch (...)
    {
        std::exception_ptr p = std::current_exception();
        handle_unexpected_eptr(p);
        exitTestStatus = EXIT_FAILURE;
    }

    std::cout << ((exitTestStatus != EXIT_SUCCESS) ? "\n\nSome Tests FAILED\n" : "\n\nAll Tests Passed\n");

    return exitTestStatus;
}
