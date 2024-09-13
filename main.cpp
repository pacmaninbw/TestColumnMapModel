#include <algorithm>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include "modelcolumntotablecolumnmap.h"

/*
 * Called by all catch(...) blocks in an attempt to show a reasonable message.
 * Copied from C++ Refference https://en.cppreference.com/
 */
static void handle_unexpected_eptr(std::exception_ptr eptr) // passing by value is OK
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

/*
 * The DisplayToDBTransferData structure is used extensively in the unit tests.
 * The second field in the struct is the unsigned int position value. When
 * initializing this struct the value may appear to be a magic number, however,
 * it indicates the position in the output of the columns in the table.
 */
static std::vector<DisplayToDBTransferData> testInputList =
{
    {ColumnIds::PUSLE_RATE, 3, true},
    {ColumnIds::BLOOD_SUGAR, 4, true},
    {ColumnIds::NUTRITION_CALORIES, 5, true},
    {ColumnIds::BLOOD_PRESSURE, 1, true},
    {ColumnIds::WEIGHT, 2, true},
    {ColumnIds::SLEEP_HOURS, 6, true}
};

static void setUpTestsForColumnAddition(
    ModelColumnToTableColumnMap &testModel, std::size_t debugLevel)
{
    testModel.enableUsedColumns(testInputList);
    if (debugLevel)
    {
        testModel.debugPrintEnabledList();
    }
}

static bool testAddingColumns(std::size_t debugLevel)
{
    bool testPassed = true;

    ModelColumnToTableColumnMap testModel;

    std::cout << "\n\n  Printing Enabled Columns, should be empty:\n";

    setUpTestsForColumnAddition(testModel, debugLevel);

    std::cout << "\n\nTesting adding columns to the list, count should be: "
        << testInputList.size() << "\n";

    if (testModel.getEnabledCount() != testInputList.size())
    {
        std::cerr << "\nIn testAddingColumns enabled list size not equal data added\n";
        testModel.debugPrintEnabledList();
        testPassed = false;
    }

    return testPassed;
}

/*
 * Testing to make sure that there are no duplicate columns. The code attempts
 * to add the same vector of enabled columns twice.
 */
static bool testNoDuplicateColumns(std::size_t debugLevel)
{
    bool testPassed = true;

    ModelColumnToTableColumnMap testModel;

    setUpTestsForColumnAddition(testModel, debugLevel);

    std::cout << "\n\nTesting no duplicates the list, initial count should be: "
        << testInputList.size() << "\n";

    std::cout << "\n\nTesting removal of duplicate column IDs in the list,"
        " count should be still " << testInputList.size() << "\n";;
    testModel.enableUsedColumns(testInputList);

    if (testModel.getEnabledCount() != testInputList.size())
    {
        std::cerr << "\nIn Testing removal of duplicate columns size not equal data added\n";
        testModel.debugPrintEnabledList();
        testPassed = false;
    }

    return testPassed;
}

/*
 * The previous test checks that the ColumnIds value isn't duplicated. This test
 * checks to make sure the positions aren't reused.
 */
static bool testNoDuplicateColumns2(std::size_t debugLevel)
{
    bool testPassed = true;
    ModelColumnToTableColumnMap testModel;

    setUpTestsForColumnAddition(testModel, debugLevel);

    std::vector<DisplayToDBTransferData> addInputList =
    {
        {ColumnIds::NUTRITION_SODIUM, 1, true},
        {ColumnIds::NUTRITION_FIBER, 2, true},
        {ColumnIds::SLEEP_INTERRUPTIONS, 3, true},
    };

    std::cout << "\n\nTesting removal of duplicate column positions in the list,"
        " count should be still " << testInputList.size() << "\n";;
    testModel.enableUsedColumns(addInputList);

    if (testModel.getEnabledCount() != testInputList.size())
    {
        std::cerr << "\nIn Testing removal of duplicate column positions size not equal data added\n";
        testModel.debugPrintEnabledList();
        testPassed = false;
    }

    return testPassed;
}

static bool testResetenableList(std::size_t debugLevel)
{
    bool testPassed = true;
    ModelColumnToTableColumnMap testModel;

    setUpTestsForColumnAddition(testModel, debugLevel);
    std::cout << "\n\nTesting reset enabled list\n";
    testModel.resetEnabledColumns();

    if (testModel.getEnabledCount() != 0)
    {
        std::cerr << "\nIn Testing reset enable list failed\n";
        testModel.debugPrintEnabledList();
        testPassed = false;
    }

    return testPassed;
}

static bool testAddingColumnsToExistingList(std::size_t debugLevel)
{
    bool testPassed = true;
    ModelColumnToTableColumnMap testModel;

    setUpTestsForColumnAddition(testModel, debugLevel);

    std::vector<DisplayToDBTransferData> addInputList =
    {
        {ColumnIds::NUTRITION_SODIUM, 7, true},
        {ColumnIds::NUTRITION_FIBER, 8, true},
        {ColumnIds::SLEEP_INTERRUPTIONS, 9, true},
    };
    std::size_t expectedTotal = testInputList.size() + addInputList.size();

    std::cout << "\n\nTesting added enables to existing list,"
        " count should be " << expectedTotal << "\n";;

    testModel.enableUsedColumns(addInputList);
    if (testModel.getEnabledCount() != expectedTotal)
    {
        std::cerr << "\nIn Testing added enables to existing list failed\n";
        std::cerr << "Expected " << expectedTotal << " Actual Result "
            << testModel.getEnabledCount() << "\n\n";
        testModel.debugPrintEnabledList();
        testPassed = false;
    }
    return testPassed;
}

static bool genericSQLQueryGenerationTest(
        std::vector<DisplayToDBTransferData> sqlTestInput,
        std::string expectedSQLQuery,
        std::string testName,
        std::size_t debugLevel, 
        bool passexpected
    )
{
    bool testPassed = true;

    try
    {
        std::cout << "\n\n" << testName << (passexpected?
             " an SQL Query should be generated.\n" :
             " an SQL Query should NOT be generated.\n");

        ModelColumnToTableColumnMap testModel;
        testModel.enableUsedColumns(sqlTestInput);
    
        std::string sqlQueryString = testModel.buildQueryString();

        if (debugLevel)
        {
            std::cout << "Before string compare\n|" << sqlQueryString << "|\n";
        }

        if (sqlQueryString.compare(expectedSQLQuery))
        {
            std::cerr << testName << 
                " generated SQL Statement not equal Expected SQL Statement\n\n";
            std::cerr << "Expected SQL Statement: " << expectedSQLQuery << "\n";
            std::cerr << "Generated SQL Statement: " << sqlQueryString << "\n";
            testPassed = false;
        }

    }
    catch (std::exception &ex) {
        std::cerr << testName << " Exception Caught: " << ex.what() << "\n";
        testPassed = false;
    }
    catch (std::string &sex)
    {
        std::cerr << testName << " String Exception Caught: " << sex << "\n";
        testPassed = false;
    }
    catch (...)
    {
        std::cerr << testName << " Unknown Unhandled Exception Caught:\n";
        std::exception_ptr p = std::current_exception();
        handle_unexpected_eptr(p);
        testPassed = false;
    }

    return testPassed;
}

/*
 * Basic test to see that the SQL Query is properly generated. The code generated
 * by this test has been inserted into MySQL Workbench and the query runs successfully.
 */
static bool testBasicSQLQueryGeneration(std::size_t debugLevel)
{
    bool testPassed = true;

    std::vector<DisplayToDBTransferData> sqlTestInput =
    {
        {ColumnIds::BLOOD_PRESSURE, 1, true},
        {ColumnIds::PUSLE_RATE, 2, true},
        {ColumnIds::BLOOD_SUGAR, 3, true},
        {ColumnIds::SLEEP_HOURS, 4, true},
        {ColumnIds::SLEEP_INTERRUPTIONS, 5, true},
        {ColumnIds::WEIGHT, 6, true},
        {ColumnIds::NUTRITION_CALORIES, 7, true},
    };

    std::string expectedSQLQuery = "SELECT dpi.Date_of_items,\n"
        "bp.Systolic, bp.Diastolic,\nbp.Pulse_Rate,\nbg.Blood_Sugar_Reading,\n"
        "slp.Hours_of_Sleep,\nslp.Interrupts,\nwt.Weight_in_lbs,\nnut.Calories\n"
        "FROM Date_Primary_Indexing AS dpi\n"
        " LEFT JOIN BloodPressure AS bp ON bp.fk_Date_Index = dpi.idDate_Primary_Indexing\n"
        " LEFT JOIN BloodSugar AS bg ON bg.fk_Date_Index = dpi.idDate_Primary_Indexing\n"
        " LEFT JOIN Sleep AS slp ON slp.fk_Date_Index = dpi.idDate_Primary_Indexing\n"
        " LEFT JOIN Weight AS wt ON wt.fk_Date_Index = dpi.idDate_Primary_Indexing\n"
        " LEFT JOIN Nutrition AS nut ON nut.fk_Date_Index = dpi.idDate_Primary_Indexing\n"
        "ORDER BY dpi.Date_of_items ASC;";

    testPassed = genericSQLQueryGenerationTest(sqlTestInput, expectedSQLQuery,
        "Testing SQL generation for valid values", debugLevel, true);
    
    return testPassed;
}

static bool testSQLQueryGenerationAlternateOrder(std::size_t debugLevel)
{
    bool testPassed = true;
    /*
     * This test is to test the sorting of the columns based on user input.
     * The SQL Query generated should be in the positions indicated the
     * numeric position value rather than the order of the structures in
     * the vector. Please excuse the magic numbers, they are the position
     * value. The DisplayToDBTransferData structures are intentionally out
     * of order.
     * 
     * The code generated by this test has been inserted into 
     * MySQL Workbench and the query runs successfully.
     */

    std::vector<DisplayToDBTransferData> sqlTestInput =
    {
        {ColumnIds::BLOOD_PRESSURE, 6, true},
        {ColumnIds::PUSLE_RATE, 7, true},
        {ColumnIds::BLOOD_SUGAR, 8, true},
        {ColumnIds::SLEEP_HOURS, 9, true},
        {ColumnIds::SLEEP_INTERRUPTIONS, 10, true},
        {ColumnIds::WEIGHT, 1, true},
        {ColumnIds::NUTRITION_CALORIES, 5, true},
        {ColumnIds::NUTRITION_POTASSIUM, 4, true},
        {ColumnIds::NUTRITION_PROTIEN, 3, true},
        {ColumnIds::NUTRITION_SODIUM, 2, true},
    };

    std::string expectedSQLQuery =  "SELECT dpi.Date_of_items,\nwt.Weight_in_lbs,\n"
        "nut.Sodium_mgs,\nnut.Protien_in_Grams,\nnut.Potassium_mgs,\nnut.Calories,\n"
        "bp.Systolic, bp.Diastolic,\nbp.Pulse_Rate,\nbg.Blood_Sugar_Reading,\n"
        "slp.Hours_of_Sleep,\nslp.Interrupts\n"
        "FROM Date_Primary_Indexing AS dpi\n"
        " LEFT JOIN Weight AS wt ON wt.fk_Date_Index = dpi.idDate_Primary_Indexing\n"
        " LEFT JOIN Nutrition AS nut ON nut.fk_Date_Index = dpi.idDate_Primary_Indexing\n"
        " LEFT JOIN BloodPressure AS bp ON bp.fk_Date_Index = dpi.idDate_Primary_Indexing\n"
        " LEFT JOIN BloodSugar AS bg ON bg.fk_Date_Index = dpi.idDate_Primary_Indexing\n"
        " LEFT JOIN Sleep AS slp ON slp.fk_Date_Index = dpi.idDate_Primary_Indexing\n"
        "ORDER BY dpi.Date_of_items ASC;"
    ;

    testPassed = genericSQLQueryGenerationTest(sqlTestInput, expectedSQLQuery,
        "Testing SQL generation for user generated order of positions",
        debugLevel, true);

    return testPassed;
}

/*
 * Negative path testing.
 */
static bool testUnimplementedColumns(std::size_t debugLevel)
{
    bool testPassed = true;

/*
 * This test is intentionally using Column IDs for columns that have not been
 * implemented in the database yet, such as ColumnIds::TEMPERATURE and
 * ColumnIds::RESPIRATION_RATE. Since the have not been implemented yet the
 * SQL Query should not be generated and any exception should be handled in
 * the ModelColumnTableColumnMap code.
 * 
 * This is to check that programming errors can be detected and reported, the
 * user interface will prevent illegal values.
 */
    std::vector<DisplayToDBTransferData> sqlTestInput =
    {
        // Test all vital signs including not implemented ones.
        {ColumnIds::TEMPERATURE, 1, true},          // Not Implemented yet
        {ColumnIds::PUSLE_RATE, 2, true},
        {ColumnIds::RESPIRATION_RATE, 3, true},     // Not Implemented yet
        {ColumnIds::BLOOD_PRESSURE, 4, true},
        {ColumnIds::BLOOD_OXYGEN, 5, true},         // Not Implemented yet
        {ColumnIds::WEIGHT, 6, true},
        {ColumnIds::BLOOD_SUGAR, 7, true},
        {ColumnIds::SLEEP_HOURS, 8, true},
        {ColumnIds::SLEEP_INTERRUPTIONS, 9, true},
        {ColumnIds::NUTRITION_CALORIES, 10, true},
        {ColumnIds::NUTRITION_SATURATED_FAT, 11, true},      // Not Implemented yet
        {ColumnIds::NUTRITION_TRANS_FAT, 12, true},          // Not Implemented yet
        {ColumnIds::NUTRITION_CHOLESTEROL, 13, true},        // Not Implemented yet
        {ColumnIds::NUTRITION_POTASSIUM, 14, true},
        {ColumnIds::NUTRITION_PROTIEN, 15, true},
        {ColumnIds::NUTRITION_SODIUM, 16, true},
        {ColumnIds::LAST_COLUMN_ID, 17, true},
    };

    std::string expectedSQLQuery =  "";
    testPassed = genericSQLQueryGenerationTest(sqlTestInput, expectedSQLQuery,
        "Testing SQL generation for unimplemented ColumnIds",
        debugLevel, false);

    return testPassed;
}

/*
 * Negative path testing.
 */
static bool testUndefinedEnum(std::size_t debugLevel)
{
    bool testPassed = true;

/*
 * This test is intentionally using Column IDs that will never be implemented.
 * The SQL Query should not be generated and any exception should be handled in
 * the ModelColumnTableColumnMap code.
 * 
 * This is to check that programming errors can be detected and reported, the
 * user interface will prevent illegal values.
 */

    std::vector<ColumnIds> badColumnIds = {
        ColumnIds(249), ColumnIds(250), ColumnIds(251), ColumnIds(252)
    };

    std::vector<DisplayToDBTransferData> sqlTestInput =
    {
        {badColumnIds[0], 1, true},
        {ColumnIds::PUSLE_RATE, 2, true},
        {badColumnIds[1], 3, true},
        {ColumnIds::BLOOD_PRESSURE, 4, true},
        {ColumnIds::BLOOD_OXYGEN, 5, true},
        {ColumnIds::WEIGHT, 6, true},
        {ColumnIds::BLOOD_SUGAR, 7, true},
        {ColumnIds::SLEEP_HOURS, 8, true},
        {ColumnIds::SLEEP_INTERRUPTIONS, 9, true},
        {badColumnIds[2], 10, true},
        {badColumnIds[3], 11, true},
        {ColumnIds::NUTRITION_TRANS_FAT, 12, true},
        {ColumnIds::NUTRITION_CHOLESTEROL, 13, true},
        {ColumnIds::NUTRITION_POTASSIUM, 14, true},
        {ColumnIds::NUTRITION_PROTIEN, 15, true},
        {ColumnIds::NUTRITION_SODIUM, 16, true},
        {ColumnIds::LAST_COLUMN_ID, 17, true},
    };

    std::string expectedSQLQuery =  "";
    testPassed = genericSQLQueryGenerationTest(sqlTestInput, expectedSQLQuery,
        "Testing SQL generation for programming error, ColumnIds will never be unimplemented",
        debugLevel, false);

    return testPassed;
}

// Copied from accepted answer https://stackoverflow.com/questions/313970/how-to-convert-an-instance-of-stdstring-to-lower-case
static char asciitolower(char in) {
    if (in <= 'Z' && in >= 'A')
        return in - ('Z' - 'z');
    return in;
}

static bool is_number(const std::string& s)
{
    return !s.empty() && std::find_if(s.begin(), 
        s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
}

static const int MIN_DBG_LEVEL = 0;
static const int MAX_DBG_LEVEL = 5;

static std::size_t getDebugLevel()
{
    std::size_t debugLevel = 0;
    std::string safeInput;
    bool hasNumber = false;

    do {
        std::cout << "Enter the DEBUG level [" << MIN_DBG_LEVEL << "," << MAX_DBG_LEVEL <<
            "] (only 0 and 1 are currently implemented)\n>>";
        std::cin >> safeInput;
        hasNumber = is_number(safeInput);
        if (hasNumber)
        {
            int safeNumber = std::stoi(safeInput);
            if (safeNumber < MIN_DBG_LEVEL || safeNumber > MAX_DBG_LEVEL)
            {
                hasNumber = false;
            }
            else
            {
                debugLevel = safeNumber;
            }
        }
    } while (!hasNumber);

    return debugLevel;
}

static bool getContinueAfterFail()
{
    bool continueAfterFail = false;

    std::string safeInput;
    do {
        std::cout << "Continue testing after a failure? [Y/n]\n>>";
        std::cin >> safeInput;
        std::transform(safeInput.begin(), safeInput.end(),
            safeInput.begin(), asciitolower);
        if (safeInput[0] == 'y')
        {
            continueAfterFail = true;
        }
    } while (safeInput[0] != 'y' && safeInput[0] != 'n');

    return continueAfterFail;
}

static void getTestingParameters(std::size_t &debugLevel, bool &continueAfterFail)
{
    debugLevel = getDebugLevel();
    continueAfterFail = getContinueAfterFail();
}

static void separaterLine()
{
    const std::size_t separaterLength = 80;

    for (std::size_t sCount = 0; sCount < separaterLength; ++sCount)
    {

        std::cout << "_";
    }
    std::cout << "\n";
}

typedef struct oneTest
{
    bool (*theTest)(std::size_t debugLevel);
    std::string errorString;
} OneTest;

static std::vector <OneTest> testList =
{
    {testAddingColumns, "Adding columns test failed.\n"},
    {testNoDuplicateColumns, "Preventing duplicate columns test 1 failed.\n"},
    {testNoDuplicateColumns2, "Preventing duplicate columns test 2 failed.\n"},
    {testResetenableList, "Reset enable list test failed.\n"},
    {testAddingColumnsToExistingList, "Adding columns to existing enabled list test failed.\n"},
    {testBasicSQLQueryGeneration, "SQL query test failed.\n"},
    {testSQLQueryGenerationAlternateOrder, "SQL query with alternate order of columns test failed\n"},
    // The followi.ng 2 tests are attempts to implement negative testing
    // No errors were detected during run or valgrind.
    {testUnimplementedColumns, "Testing unimiplemented columns, SQL Query should not have been generated."},
    {testUndefinedEnum, "Testing Invalid enum values, SQL Query should not have been generated."}
};

int main()
{
    int exitTestStatus = EXIT_SUCCESS;
    std::size_t testCount = 0;
    std::size_t debugLevel = 0;
    bool continueAfterFail = false;
    
    getTestingParameters(debugLevel, continueAfterFail);

    try {
        for (auto thisTest: testList)
        {
            std::cout << "Running Test " << testCount << "\t";
            if (!thisTest.theTest(debugLevel))
            {
                std::cerr << thisTest.errorString;
                std::cerr << "Test " << testCount << " FAILED\n";
                exitTestStatus = EXIT_FAILURE;
                if (!continueAfterFail)
                {
                    break;
                }
            }
            else
            {
                std::cout << "Test " << testCount << " PASSED\n";
                separaterLine();
            }
            testCount++;
        }
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

    if (exitTestStatus != EXIT_SUCCESS)
    {
        std::cerr << "Test Number " << testCount << " FAILED " << (testList.size() - testCount)
            << " tests did not execute\n";
    }
    else
    {
        std::cout << "\n\nAll Tests Passed\n";
    }

    return exitTestStatus;
}
