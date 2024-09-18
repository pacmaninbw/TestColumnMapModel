#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include "Handle_Unexpected_Exceptions.h"
#include "modelcolumntotablecolumnmap.h"
#include "UnitTesting.h"

static bool continueAfterFail = false;
static std::size_t debugLevel = 0;

typedef struct unitTest
{
    bool (*theTest)();
    std::string errorString;
} UnitTest;

UnitTesting::UnitTesting()
{
}

UnitTesting::~UnitTesting()
{

}

void UnitTesting::getUserTestConfiguration()
{
    getDebugLevel();
    getContinueAfterFail();
}

void UnitTesting::separaterLine(void)
{
    const std::size_t separaterLength = 80;

    for (std::size_t sCount = 0; sCount < separaterLength; ++sCount)
    {

        std::cout << "_";
    }
    std::cout << "\n";

}

void UnitTesting::getDebugLevel(void)
{
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
}

void UnitTesting::getContinueAfterFail(void)
{
    std::string safeInput;
    do {
        std::cout << "Continue testing after a failure? [Y(es)/n(o)]\n>>";
        std::cin >> safeInput;
        std::transform(safeInput.begin(), safeInput.end(),
            safeInput.begin(), asciitolower);
        if (safeInput[0] == 'y')
        {
            continueAfterFail = true;
        }
    } while (safeInput[0] != 'y' && safeInput[0] != 'n');
}

/*
 * The following functions are declared as static functions rather then
 * member functions of the UnitTesting class because there is no way in
 * C++ to create an array of member functions.
 */
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

static void setUpTestsForColumnAddition( ModelColumnToTableColumnMap &testModel)
{
    testModel.enableUsedColumns(testInputList);
    if (debugLevel)
    {
        testModel.debugPrintEnabledList();
    }
}

static bool testAddingColumns()
{
    bool testPassed = true;

    ModelColumnToTableColumnMap testModel;

    std::cout << "\n\n  Printing Enabled Columns, should be empty:\n";

    setUpTestsForColumnAddition(testModel);

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
 * Negative path testing.
 */
/*
 * Testing to make sure that there are no duplicate columns. The code attempts
 * to add the same vector of enabled columns twice.
 */
static bool testNoDuplicateColumns()
{
    bool testPassed = true;
    std::cout << "Error messages are expected during the execution of this testS.\n";

    ModelColumnToTableColumnMap testModel;

    setUpTestsForColumnAddition(testModel);

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
 * Negative path testing.
 */
/*
 * The previous test checks that the ColumnIds value isn't duplicated. This test
 * checks to make sure the positions aren't reused.
 */
static bool testNoDuplicateColumns2()
{
    bool testPassed = true;
    ModelColumnToTableColumnMap testModel;
    std::cout << "Error messages are expected during the execution of this testS.\n";

    setUpTestsForColumnAddition(testModel);

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

static bool testResetenableList()
{
    bool testPassed = true;
    ModelColumnToTableColumnMap testModel;

    setUpTestsForColumnAddition(testModel);
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

static bool testAddingColumnsToExistingList()
{
    bool testPassed = true;
    ModelColumnToTableColumnMap testModel;

    setUpTestsForColumnAddition(testModel);

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
        bool passexpected
    )
{
    bool testPassed = true;

    std::cout << "\n\n" << testName << (passexpected?
            " an SQL Query should be generated.\n" :
            " an SQL Query should NOT be generated.\n");
    if (!passexpected)
    {
        std::cout << "Error messages are expected during the execution of this testS.\n";
    }

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
        std::cerr << "Expected SQL Statement: " << 
            (expectedSQLQuery.empty()? "EMPTY STRING" : expectedSQLQuery) << "\n";
        std::cerr << "Generated SQL Statement: " << sqlQueryString << "\n";
        testPassed = false;
    }

    return testPassed;
}

/*
 * Basic test to see that the SQL Query is properly generated. The code generated
 * by this test has been inserted into MySQL Workbench and the query runs successfully.
 */
static bool testBasicSQLQueryGeneration()
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
        "Testing SQL generation for valid values", true);
    
    return testPassed;
}

static bool testSQLQueryGenerationAlternateOrder()
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
        true);

    return testPassed;
}

/*
 * Negative path testing.
 */
static bool testUnimplementedColumns()
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
        {ColumnIds::NUTRITION_SODIUM, 16, true}
    };

    std::string expectedSQLQuery =  "";
    testPassed = genericSQLQueryGenerationTest(sqlTestInput, expectedSQLQuery,
        "Testing SQL generation for unimplemented ColumnIds",
        false);

    return testPassed;
}

/*
 * Negative path testing.
 */
static bool testUndefinedEnum()
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
        {ColumnIds::WEIGHT, 5, true},
        {ColumnIds::BLOOD_SUGAR, 6, true},
        {ColumnIds::SLEEP_HOURS, 7, true},
        {ColumnIds::SLEEP_INTERRUPTIONS, 8, true},
        {badColumnIds[2], 9, true},
        {badColumnIds[3], 10, true},
        {ColumnIds::NUTRITION_POTASSIUM, 11, true},
        {ColumnIds::NUTRITION_PROTIEN, 12, true},
        {ColumnIds::NUTRITION_SODIUM, 13, true},
        {ColumnIds::LAST_COLUMN_ID, 14, true},
    };

    std::string expectedSQLQuery =  "";
    testPassed = genericSQLQueryGenerationTest(sqlTestInput, expectedSQLQuery,
        "Testing SQL generation for programming error, ColumnIds will never be unimplemented",
        false);

    return testPassed;
}

static std::vector <UnitTest> testList =
{
    {testAddingColumns, "Adding columns test failed.\n"},
    {testResetenableList, "Reset enable list test failed.\n"},
    {testAddingColumnsToExistingList, "Adding columns to existing enabled list test failed.\n"},
    {testBasicSQLQueryGeneration, "SQL query test failed.\n"},
    {testSQLQueryGenerationAlternateOrder, "SQL query with alternate order of columns test failed\n"},
    // Begin Negative Test Path
    // No errors were detected during run or valgrind.
    {testNoDuplicateColumns, "Preventing duplicate columns test 1 failed.\n"},
    {testNoDuplicateColumns2, "Preventing duplicate columns test 2 failed.\n"},
    {testUnimplementedColumns, "Testing unimiplemented columns, SQL Query should not have been generated."},
    {testUndefinedEnum, "Testing Invalid enum values, SQL Query should not have been generated."}
};

int UnitTesting::executeAllUnitTests()
{
    int exitTestStatus = EXIT_SUCCESS;
    std::size_t testCount = 0;

    for (auto thisTest: testList)
    {
        std::cout << "Running Test " << testCount << "\t";
        if (!thisTest.theTest())
        {
            std::cerr << thisTest.errorString;
            std::cerr << "Test " << testCount << " FAILED\n";
            exitTestStatus = EXIT_FAILURE;
            if (debugLevel < MAX_DBG_LEVEL)
            {
                std::size_t tmpDbgLvl = debugLevel;
                debugLevel = MAX_DBG_LEVEL;
                std::cout << "\n\nRerunning Test " << testCount << " " << 
                    thisTest.errorString << " with full debug information";
                thisTest.theTest();
                std::cout << "\n\n";
                debugLevel = tmpDbgLvl;
            }
            if (!continueAfterFail)
            {
                std::cerr << "Test Number " << testCount << " FAILED " << 
                    (testList.size() - testCount -1)
                    << " test(s) did not execute\n";
                return exitTestStatus;
            }
        }
        else
        {
            std::cout << "Test " << testCount << " PASSED\n";
            separaterLine();
        }
        testCount++;
    }

    return exitTestStatus;
}

