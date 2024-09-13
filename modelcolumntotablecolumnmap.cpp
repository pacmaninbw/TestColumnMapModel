#include <algorithm>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "columnidenum.h"
#include "tableidenum.h"
#include "columnaccessdata.h"
#include "dispalytodbtransferdata.h"
#include "modelcolumntotablecolumnmap.h"
#include "hdb_columnnames.h"
#include "TableNameDictionary.h"

/*
 * Static basis for display column to database table and column mapping.
 * Used for initialization of the model. All of the information to display
 * one column of data is in an element in the staticModelMap.
 */
static std::vector<ColumnAccessDataMap> staticModelMap =
{
    // Vital Signs
    {
        ColumnIds::TEMPERATURE,
        TableIds::TEMPURATURE,
        "Tempurature",
        {"No Columns", "Not Implemented"}
    },
    {
        ColumnIds::PUSLE_RATE,
        TableIds::BLOOD_PRESSURE,
        "Pulse Rate",
        {"Pulse_Rate"}
    },
    {
        ColumnIds::RESPIRATION_RATE,
        TableIds::REPIRATION_RATE,
        "Respiration Rate",
        {"No Columns", "Not Implemented"}
    },
    {
        ColumnIds::BLOOD_PRESSURE,
        TableIds::BLOOD_PRESSURE,
        "Blood Pressure",
        {"Systolic","Diastolic"}
    },
    {
        ColumnIds::BLOOD_OXYGEN,
        TableIds::BLOOD_OXYGEN,
        "Blood Oxygen",
        {"No Columns", "Not Implemented"},
    },
    {
        ColumnIds::WEIGHT,
        TableIds::WEIGHT,
        "Weight",
        {"Weight_in_lbs"}
    },
    {
        ColumnIds::BLOOD_SUGAR,
        TableIds::BLOOD_SUGAR,
        "Blood Glucose",
        {"Blood_Sugar_Reading"}
    },

    // External Factors or Measurements
    {
        ColumnIds::EXERCISE,
        TableIds::EXERCISE,
        "Exercise",
        {"Exercise_Minutes"}
    },
    {
        ColumnIds::SLEEP_HOURS,
        TableIds::SLEEP,
        "Sleep Hours",
        {"Hours_of_Sleep",}
    },
    {
        ColumnIds::SLEEP_INTERRUPTIONS,
        TableIds::SLEEP,
        "Sleep Interruptions",
        {"Interrupts",}
    },
    {
        ColumnIds::BOWEL_MOVEMENTS,
        TableIds::BOWEL_MOVEMENTS,
        "Bowl Movements",
        {"BM_Count"}
    },
    // Nutrition
    {
        ColumnIds::NUTRITION_CALORIES,
        TableIds::NUTRITION,
        "Calories",
        {"Calories"}
    },
    {
        ColumnIds::NUTRITION_SATURATED_FAT,
        TableIds::NUTRITION,
        "Saturated Fat",
        {"No Column", "Not Implemented"}
    },
    {
        ColumnIds::NUTRITION_TRANS_FAT,
        TableIds::NUTRITION,
        "Trans Fat",
        {"No Column", "Not Implemented"}
    },
    {
        ColumnIds::NUTRITION_CHOLESTEROL,
        TableIds::NUTRITION,
        "Cholesterol",
        {"No Column", "Not Implemented"}
    },
    {
        ColumnIds::NUTRITION_SODIUM,
        TableIds::NUTRITION,
        "Sodium",
        {"Sodium_mgs"}
    },
    {
        ColumnIds::NUTRITION_FIBER,
        TableIds::NUTRITION,
        "Fiber",
        {"Fiber_Grams"}
    },
    {
        ColumnIds::NUTRITION_TOTAL_SUGARS,
        TableIds::NUTRITION,
        "Total Sugars",
        {"No Column", "Not Implemented"}
    },
    {
        ColumnIds::NUTRITION_PROTIEN,
        TableIds::NUTRITION,
        "Protien",
        {"Protien_in_Grams"}
    },
    {
        ColumnIds::NUTRITION_VITAMIN_D,
        TableIds::NUTRITION,
        "Vitamin D",
        {"No Column", "Not Implemented"}
    },
    {
        ColumnIds::NUTRITION_CALCIUM,
        TableIds::NUTRITION,
        "Calcium",
        {"No Column", "Not Implemented"}
    },
    {
        ColumnIds::NUTRITION_IRON,
        TableIds::NUTRITION,
        "Iron",
        {"No Column", "Not Implemented"}
    },
    {
        ColumnIds::NUTRITION_POTASSIUM,
        TableIds::NUTRITION,
        "Potassium",
        {"Potassium_mgs"}
    },

    // Other
    {
        ColumnIds::NOTES,
        TableIds::NOTES,
        "Notes",
        {"Note_Content"},
    }

};

const std::string ModelColumnToTableColumnMap::buildQueryString() noexcept
{
    /*
     * Sort the list to the order requested by the user.
     */
    std::sort(enabledList.begin(), enabledList.end(),
        [](DisplayToDBTransferData a, DisplayToDBTransferData b)
            {return  a.position < b.position;});

    /*
     * Generate an SQL query from the selected columns.
     */
    std::string selectedColumns("dpi.Date_of_items");
    std::vector<TableIds> tablesToJoin;

    for (auto columnData: enabledList)
    {
        selectedColumns += ",\n";
        selectedColumns += addColumn(columnData.columnId);
        addTableToJoinList(tablesToJoin, getColumnData(columnData.columnId).getTableId());
    }

    std::string sqlQuery("SELECT " + selectedColumns +
        +"\nFROM Date_Primary_Indexing AS dpi\n" + buildJoinList(tablesToJoin) +
        "ORDER BY dpi.Date_of_items ASC;");

    return sqlQuery;
}

void ModelColumnToTableColumnMap::resetEnabledColumns()
{
    enabledList.clear();
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

    // Don't assume the input data is correct, only add legitamate columns.
    auto baseSource = std::find_if(staticModelMap.begin(), staticModelMap.end(),
          [&columnToFind](ColumnAccessDataMap &cdm) {return (cdm.getColumnId() == columnToFind);});
    if (baseSource == staticModelMap.end())
    {
        return;
    }

    // No duplicate columns in the enabled list of columns.
    // Date is the zero element and will always be there.
    if (enabledList.size() > 0)
    {      
        if (duplicateInEnabledList(enabledColumn))
        {
            return;
        }
    }

    enabledList.push_back(enabledColumn);
}

bool ModelColumnToTableColumnMap::duplicateInEnabledList(DisplayToDBTransferData candidateColumn)
{
    ColumnIds id = candidateColumn.columnId;
    unsigned int pos = candidateColumn.position;

    auto duplicate = std::find_if(enabledList.begin(), enabledList.end(),
        [&id, &pos](DisplayToDBTransferData &cdm)
            {return ((cdm.columnId == id) || (cdm.position == pos));});

    return (duplicate != enabledList.end());
}

/*
 * While some tables will have an entry for every day and could use INNER JOIN,
 * use LEFT JOIN for all so that there are no order dependent issues.
 */
std::string ModelColumnToTableColumnMap::addJoin(TableIds tblId) noexcept
{
    TableNameDictionary tabDef;
    std::pair<std::string_view, std::string_view> tabNamePair = tabDef.getTableNames(tblId);

    std::string tableName(tabNamePair.first);
    std::string tablePrefix(tabNamePair.second);
    std::string sqlJoinClause(" LEFT JOIN " + tableName + " AS " + tablePrefix);

    sqlJoinClause += " ON " + tablePrefix + "." +
        "fk_Date_Index = dpi.idDate_Primary_Indexing\n";

    return sqlJoinClause;
}

/*
 * In most cases the columns displayed by the contents of a ColumnAccessDataMap
 * object will be a single column, but there are special cases where it will be
 * more than on column.
 */
std::string ModelColumnToTableColumnMap::addColumn(ColumnIds columnId) noexcept
{
    ColumnAccessDataMap columnData = getColumnData(columnId);
    TableNameDictionary tabDef;
    std::string addedColumnsWithTableName;
    std::string tablePrefix(tabDef.getShortName(columnData.getTableId()));
    tablePrefix += ".";

    bool needComma = false;

    for (auto column: columnData.getColumnsInTable())
    {
        if (needComma)
        {
           addedColumnsWithTableName += ", ";
        }
        std::string columnToAdd(tablePrefix + column);
        addedColumnsWithTableName += columnToAdd;
        needComma = true;
    }

    return addedColumnsWithTableName;
}

void ModelColumnToTableColumnMap::addTableToJoinList(std::vector<TableIds> &tablesToJoin, TableIds newEntry) noexcept
{
    auto tabIdIter = std::find(tablesToJoin.begin(), tablesToJoin.end(), newEntry);
    if (tabIdIter == tablesToJoin.end())
    {
        tablesToJoin.push_back(newEntry);
    }
}

std::string ModelColumnToTableColumnMap::buildJoinList(std::vector<TableIds> tablesToJoin) noexcept
{
    std::string joinSqlStr;

    for (auto aTable: tablesToJoin)
    {
        joinSqlStr += addJoin(aTable);
    }

    return joinSqlStr;
}

const ColumnAccessDataMap ModelColumnToTableColumnMap::getColumnData(ColumnIds columnId) const noexcept
{
    auto columnSource = std::find_if(staticModelMap.begin(), staticModelMap.end(),
          [&columnId](ColumnAccessDataMap &cdm) {return (cdm.getColumnId() == columnId);});

    if (columnSource == staticModelMap.end())
    {
        ColumnAccessDataMap badData(ColumnIds::NO_COLUMN, TableIds::NO_TABLE, "Bad Data", {"Bad Data"});
        return badData;
    }

    return *columnSource;
}

void ModelColumnToTableColumnMap::debugPrintTransferDataList(std::vector<DisplayToDBTransferData> testList)
{
    for (auto testMember: testList)
    {
        std::cout << testMember.toString();
    }
}

void ModelColumnToTableColumnMap::debugPrintList(std::vector<ColumnAccessDataMap> listToPrint)
{
    unsigned int testCount = 0;

    for (auto testMember: listToPrint)
    {
        std::cout << "Column " << testCount << "\n";
        testMember.showContents();
        testCount++;
    }

}

