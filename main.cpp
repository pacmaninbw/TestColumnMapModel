#include <iostream>
#include <string>
#include <vector>
#include "modelcolumntotablecolumnmap.h"

static bool testInitialization()
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

static bool testNoDuplicateColumns()
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

static bool testNoDuplicateColumns2()
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

static bool testSQLQueryGeneration()
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

static bool testSQLQueryGenerationAlternateOrder()
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
        {testSQLQueryGenerationAlternateOrder, "SQL query with alternate order of columns test failed\n"}
    };
    std::size_t testCount = 0;

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
