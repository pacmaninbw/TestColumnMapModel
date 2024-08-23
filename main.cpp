#include <vector>
#include <iostream>
#include "modelcolumntotablecolumnmap.h"


int main()
{
    ModelColumnToTableColumnMap testModel;

    std::cout << "Testing initialization of ModelColumnToTableColumnMap\n";
    std::cout << "  Printing Base Table Contents:\n";
    testModel.debugPrintBaseTable();

    std::cout << "\n\n  Printing Enabled Columns, should only be Date Column:\n";
    testModel.debugPrintEnabledList();

    std::cout << "\n\nTesting adding columns to the list, count should be 7 including Date\n";
    std::vector<DisplayToDBTransferData> testInputList =
    {
        {ColumnIds::PUSLE_RATE, 3, true},
        {ColumnIds::BLOOD_SUGAR, 4, true},
        {ColumnIds::NUTRITION_CALORIES, 5, true},
        {ColumnIds::BLOOD_PRESSURE, 1, true},
        {ColumnIds::WEIGHT, 2, true},
        {ColumnIds::SLEEP_HOURS, 6, true}
    };
    testModel.enableUsedColumns(testInputList);
    testModel.debugPrintEnabledList();

    std::cout << "\n\nTesting removal of duplicate columns to the list,"
                 " count should be 7 including Date\n";
    testModel.enableUsedColumns(testInputList);
    testModel.debugPrintEnabledList();

    std::vector<DisplayToDBTransferData> addInputList =
    {
        {ColumnIds::NUTRITION_SODIUM, 7, true},
        {ColumnIds::NUTRITION_FIBER, 8, true},
        {ColumnIds::SLEEP_INTERRUPTIONS, 9, true},
    };
    std::cout << "Testing adding columns to the list,"
                 " count should be 10 including Date\n";
    testModel.enableUsedColumns(addInputList);
    testModel.debugPrintEnabledList();

    std::cout << "\n\nTesting reset enabled list\n";
    testModel.resetEnabledColumns();
    testModel.debugPrintEnabledList();

    return EXIT_SUCCESS;
}
