#include <algorithm>
#include <vector>

#include "modelcolumntotablecolumnmap.h"
#include "hdb_columnnames.h"
#if (DEBUG || MY_UNIT_TEST)
#include <iostream>
#endif

/*
 * Static basis for display column to database table and columnmapping.
 * Used for initialization of the model. All of the information to display
 * one column of data is in an element in the staticModelMap.
 */
static std::vector<ColumnAccessDataMap> staticModelMap =
{
    // All columns are order by date.
    {
        ColumnIds::DATE,
        "Date",
        "Date_Primary_Indexing",
        {"idDate_Primary_Indexing","Date_of_items"}
    },
    // Vital Signs
    {
        ColumnIds::TEMPERATURE,
        "Tempurature",
        "No Table",
        {"No Columns", "Not Implemented"}
    },
    {
        ColumnIds::PUSLE_RATE,
        "Pulse Rate",
        "BloodPressure",
        {"Pulse_Rate" "Not Implemented"}
    },
    {
        ColumnIds::RESPIRATION_RATE,
        "Respiration Rate",
        "No Table",
        {"No Columns", "Not Implemented"}
    },
    {
        ColumnIds::BLOOD_PRESSURE,
        "Blood Pressure",
        "BloodPressure",
        {"Systolic","Diastolic"}
    },
    {
        ColumnIds::BLOOD_OXYGEN,
        "Blood Oxygen",
        "No Table",
        {"No Columns", "Not Implemented"}
    },
    {
        ColumnIds::WEIGHT,
        "Weight",
        "Weight",
        {"Weight_in_lbs"}
    },
    {
        ColumnIds::BLOOD_SUGAR,
        "Blood Glucose",
        "Blood_Sugar",
        {"Blood_Sugar_Reading"}
    },

    // External Factors or Measurements
    {
        ColumnIds::EXERCISE,
        "Exercise",
        "Exercise",
        {"Exercise_Minutes"}
    },
    {
        ColumnIds::SLEEP_HOURS,
        "Sleep Hours",
        "Sleep",
        {"Hours_of_Sleep",}
    },
    {
        ColumnIds::SLEEP_INTERRUPTIONS,
        "Sleep Interruptions",
        "Sleep",
        {"Interrupts",}
    },
    {
        ColumnIds::BOWEL_MOVEMENTS,
        "Bowel Movements",
        "BowlMovements",
        {"BM_Count"}
    },
    // Nutrition
    {
        ColumnIds::NUTRITION_CALORIES,
        "Calories",
        "Nutrition",
        {"Calories"}
    },
    {
        ColumnIds::NUTRITION_SATURATED_FAT,
        "Saturated Fat",
        "Nutrition",
        {"No Column", "Not Implemented"}
    },
    {
        ColumnIds::NUTRITION_TRANS_FAT,
        "Saturated Fat",
        "Nutrition",
        {"No Column", "Not Implemented"}
    },
    {
        ColumnIds::NUTRITION_CHOLESTEROL,
        "Saturated Fat",
        "Nutrition",
        {"No Column", "Not Implemented"}
    },
    {
        ColumnIds::NUTRITION_SODIUM,
        "Sodium",
        "Nutrition",
        {"Sodium_mgs"}
    },
    {
        ColumnIds::NUTRITION_FIBER,
        "Fiber",
        "Nutrition",
        {"Fiber_Grams"}
    },
    {
        ColumnIds::NUTRITION_TOTAL_SUGARS,
        "Total Sugars",
        "Nutrition",
        {"No Column", "Not Implemented"}
    },
    {
        ColumnIds::NUTRITION_PROTIEN,
        "Protien",
        "Nutrition",
        {"Protien_in_Grams"}
    },
        {
        ColumnIds::NUTRITION_VITAMIN_D,
        "Vitamin D",
        "Nutrition",
        {"No Column", "Not Implemented"}
    },
    {
        ColumnIds::NUTRITION_CALCIUM,
        "Calcium",
        "Nutrition",
        {"No Column", "Not Implemented"}
    },
        {
        ColumnIds::NUTRITION_IRON,
        "Iron",
        "Nutrition",
        {"No Column", "Not Implemented"}
    },
        {
        ColumnIds::NUTRITION_POTASSIUM,
        "Potassium",
        "Nutrition",
        {"Potassium_mgs"}
    },

    // Other
    {
        ColumnIds::NOTES,
        "Notes",
        "Notes",
        {"Note_Content"}
    }

};

ModelColumnToTableColumnMap::ModelColumnToTableColumnMap()
{
    for (auto newTableEntry: staticModelMap)
    {
        baseTable.push_back(newTableEntry);
    }

    DisplayToDBTransferData DateCol = {ColumnIds::DATE, 0 , true};
    enableAndAddtoList(DateCol);
}

const std::string ModelColumnToTableColumnMap::buildQueryString() noexcept
{
    std::string sqlQuery("select ");

    return sqlQuery;
}

void ModelColumnToTableColumnMap::resetEnabledColumns()
{
    enabledList.clear();
    DisplayToDBTransferData DateCol = {ColumnIds::DATE, 0 , true};
    enableAndAddtoList(DateCol);
}

void ModelColumnToTableColumnMap::enableUsedColumns(std::vector<DisplayToDBTransferData> dialogInput) noexcept
{
    for (auto inputItem: dialogInput)
    {
        // Position 0 is reserved for the Date.
        if (inputItem.columnId != ColumnIds::NO_COLUMN && inputItem.position !=0)
        {
            enableAndAddtoList(inputItem);
        }
    }

}

void ModelColumnToTableColumnMap::enableAndAddtoList(DisplayToDBTransferData enabledColumn) noexcept
{
    ColumnIds columnToFind = enabledColumn.columnId;

    // No duplicate columns in the enabled list of columns.
    // Date is the zero element and will always be there.
    if (enabledList.size() > 0)
    {      
        if (duplicateInEnabledList(enabledColumn))
        {
            return;
        }
    }

    // Create a copy of the object in the base table, add the additional data from the
    // columns dialog and add the copy to the enabled columns list.
    auto baseSource = std::find_if(baseTable.begin(), baseTable.end(),
          [&columnToFind](ColumnAccessDataMap &cdm) {return (cdm.getColumnId() == columnToFind);});

    ColumnAccessDataMap columnToBeAdded = *baseSource;
    columnToBeAdded.setPosition(enabledColumn.position);
    columnToBeAdded.setEnabled(enabledColumn.enabled);

    enabledList.push_back(columnToBeAdded);
}

bool ModelColumnToTableColumnMap::duplicateInEnabledList(DisplayToDBTransferData candidateColumn)
{
    ColumnIds id = candidateColumn.columnId;
    unsigned int pos = candidateColumn.position;

    auto duplicate = std::find_if(enabledList.begin(), enabledList.end(),
        [&id, &pos](ColumnAccessDataMap &cdm)
            {return ((cdm.getColumnId() == id) || (cdm.getPosition() == pos));});

    return (duplicate != enabledList.end());
}

#if (DEBUG || MY_UNIT_TEST)
void ModelColumnToTableColumnMap::debugPrintTransferDataList(std::vector<DisplayToDBTransferData> testList)
{
    for (auto testMember: testList)
    {
        debugPrintTransferData(testMember);
    }
}

void ModelColumnToTableColumnMap::debugPrintList(std::vector<ColumnAccessDataMap> listToPrint)
{
    unsigned int testCount = 0;

    for (auto testMember: listToPrint)
    {
        std::cout << "Column " << testCount << "\n";
        debugPrintListMember(testMember);
        testCount++;
    }

}

void ModelColumnToTableColumnMap::debugPrintListMember(ColumnAccessDataMap mapToPrint)
{
    HDB_ColumnNames dictionary;

    std::cout << "\tColumn ID converted to string " <<
        dictionary.convertToString(mapToPrint.getColumnId()) << "\n";
    std::cout << "\tColumn Name " << mapToPrint.getName() << "\n";
    std::cout << "\tColumn Position " << mapToPrint.getPosition() << "\n";
    std::cout << "\t" << ((mapToPrint.getEnabled()) ? "Enabled\n" :"Disabled\n");
    std::cout << "\tTable Name: " << mapToPrint.getTableName() << "\n";

     const std::vector<std::string> columnsInTable = mapToPrint.getColumnsInTable();
     std::cout << "\tColumns to access in specified table:\n";
     for (auto columnName: columnsInTable)
     {
         std::cout << "\t\t" << columnName << "\n";
     }
     // Provide vertical spacing to make the test output more readable.
     std::cout << "\n\n";
}

void ModelColumnToTableColumnMap::debugPrintTransferData(DisplayToDBTransferData transferData)
{
    HDB_ColumnNames dictionary;

    std::cout << "Debugging or Testing DisplayToDBTransferData\n";
    std::cout << "\tColumnId: " << dictionary.convertToString(transferData.columnId)  << "\n";
    std::cout << "\tPosition: " << transferData.position << "\n";
    std::cout << "\Enabled: " << (transferData.enabled ? "True" : "False") << "\n";
}
#endif

