#include <iostream>
#include <string>
#include <vector>
#include "modelcolumntotablecolumnmap.h"

static bool testInitialization(void)
{
    bool testPassed = true;

    ModelColumnToTableColumnMap testModel;

    std::cout << "Testing initialization of ModelColumnToTableColumnMap\n";

    std::cout << "\n\n  Printing Enabled Columns, should be empty:\n";
    testModel.debugPrintEnabledList();
    if (testModel.getEnabledCount() != 0)
    {
        std::cerr << "\nTest of testModel initialization failed\n";
        std::cout << "  Printing Base Table Contents:\n";
        testPassed = false;
    }

    return testPassed;
}

static std::vector<DisplayToDBTransferData> testInputList =
{
    {ColumnIds::PUSLE_RATE, 3, true},
    {ColumnIds::BLOOD_SUGAR, 4, true},
    {ColumnIds::NUTRITION_CALORIES, 5, true},
    {ColumnIds::BLOOD_PRESSURE, 1, true},
    {ColumnIds::WEIGHT, 2, true},
    {ColumnIds::SLEEP_HOURS, 6, true}
};

static void setUpTestsForColumnAddition(ModelColumnToTableColumnMap &testModel)
{
    testModel.enableUsedColumns(testInputList);
//    testModel.debugPrintEnabledList();
}

static bool testAddingColumns(void)
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

static bool testNoDuplicateColumns(void)
{
    bool testPassed = true;

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

static bool testNoDuplicateColumns2(void)
{
    bool testPassed = true;
    ModelColumnToTableColumnMap testModel;

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

static bool testResetenableList(void)
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

static bool testAddingColumnsToExistingList(void)
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

static bool testSQLQueryGeneration(void)
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

    ModelColumnToTableColumnMap testModel;
    testModel.enableUsedColumns(sqlTestInput);
    std::string sqlQueryString = testModel.buildQueryString();
    std::cout << "\n" << sqlQueryString << "\n";


    return testPassed;
}

static bool testSQLQueryGenerationAlternateOrder(void)
{
    bool testPassed = true;

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

    ModelColumnToTableColumnMap testModel;
    testModel.enableUsedColumns(sqlTestInput);
    std::string sqlQueryString = testModel.buildQueryString();
    std::cout << "\n" << sqlQueryString << "\n";


    return testPassed;
}

static bool testUnimplementedColumns(void)
{
    bool testPassed = true;

    std::vector<DisplayToDBTransferData> sqlTestInput =
    {
        // Test all vital signs including not implemented ones.
        {ColumnIds::TEMPERATURE, 1, true},
        {ColumnIds::PUSLE_RATE, 2, true},
        {ColumnIds::RESPIRATION_RATE, 3, true},
        {ColumnIds::BLOOD_PRESSURE, 4, true},
        {ColumnIds::BLOOD_OXYGEN, 5, true},
        {ColumnIds::WEIGHT, 6, true},
        {ColumnIds::BLOOD_SUGAR, 7, true},
        {ColumnIds::SLEEP_HOURS, 8, true},
        {ColumnIds::SLEEP_INTERRUPTIONS, 9, true},
        {ColumnIds::NUTRITION_CALORIES, 10, true},
        {ColumnIds::NUTRITION_SATURATED_FAT, 11, true},
        {ColumnIds::NUTRITION_TRANS_FAT, 12, true},
        {ColumnIds::NUTRITION_CHOLESTEROL, 13, true},
        {ColumnIds::NUTRITION_POTASSIUM, 14, true},
        {ColumnIds::NUTRITION_PROTIEN, 15, true},
        {ColumnIds::NUTRITION_SODIUM, 16, true},
        {ColumnIds::LAST_COLUMN_ID, 17, true},
    };

    ModelColumnToTableColumnMap testModel;
    testModel.enableUsedColumns(sqlTestInput);
    std::string sqlQueryString = testModel.buildQueryString();
    std::cout << "\n" << sqlQueryString << "\n";

    return testPassed;
}

static bool testUndefinedEnum(void)
{
    bool testPassed = true;

    ColumnIds badValue0 = ColumnIds(249);
    ColumnIds badValue1 = ColumnIds(250);
    ColumnIds badValue2 = ColumnIds(251);
    ColumnIds badValue3 = ColumnIds(252);

    std::vector<DisplayToDBTransferData> sqlTestInput =
    {
        // Test all vital signs including not implemented ones.
        {badValue0, 1, true},
        {ColumnIds::PUSLE_RATE, 2, true},
        {badValue1, 3, true},
        {ColumnIds::BLOOD_PRESSURE, 4, true},
        {ColumnIds::BLOOD_OXYGEN, 5, true},
        {ColumnIds::WEIGHT, 6, true},
        {ColumnIds::BLOOD_SUGAR, 7, true},
        {ColumnIds::SLEEP_HOURS, 8, true},
        {ColumnIds::SLEEP_INTERRUPTIONS, 9, true},
        {badValue2, 10, true},
        {badValue3, 11, true},
        {ColumnIds::NUTRITION_TRANS_FAT, 12, true},
        {ColumnIds::NUTRITION_CHOLESTEROL, 13, true},
        {ColumnIds::NUTRITION_POTASSIUM, 14, true},
        {ColumnIds::NUTRITION_PROTIEN, 15, true},
        {ColumnIds::NUTRITION_SODIUM, 16, true},
        {ColumnIds::LAST_COLUMN_ID, 17, true},
    };

    ModelColumnToTableColumnMap testModel;
    testModel.enableUsedColumns(sqlTestInput);
    std::string sqlQueryString = testModel.buildQueryString();
    std::cout << "\n" << sqlQueryString << "\n";

    return testPassed;
}

typedef struct oneTest
{
    bool (*theTest)(void);
    std::string errorString;
} OneTest;



int main()
{
    int exitTestStatus = EXIT_SUCCESS;
    std::vector <OneTest> testList =
    {
        {testInitialization, "Initialization test failed.\n"},
        {testAddingColumns, "Adding columns test failed.\n"},
        {testNoDuplicateColumns, "Preventing duplicate columns test 1 failed.\n"},
        {testNoDuplicateColumns2, "Preventing duplicate columns test 2 failed.\n"},
        {testResetenableList, "Reset enable list test failed.\n"},
        {testAddingColumnsToExistingList, "Adding columns to existing enabled list test failed.\n"},
        {testSQLQueryGeneration, "SQL query test failed.\n"},
        {testSQLQueryGenerationAlternateOrder, "SQL query with alternate order of columns test failed\n"},
        // The followi.ng 2 tests are attempts to implement negative testing
        // No errors were detected during run or valgrind.
        // No exceptions were thrown
        {testUnimplementedColumns, "Testing unimiplemented columns"},
        {testUndefinedEnum, "Testing Invalid enum values"}
    };
    std::size_t testCount = 0;

    try {
        for (auto thisTest: testList)
        {
            if (!thisTest.theTest())
            {
                std::cerr << thisTest.errorString;
                exitTestStatus = EXIT_FAILURE;
                break;
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
        std::clog <<(p ? p.__cxa_exception_type()->name() : "null") << "\n";
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
