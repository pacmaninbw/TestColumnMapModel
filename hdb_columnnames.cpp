#include <map>
#include <string>
#include "columnidenum.h"
#include "hdb_columnnames.h"

HDB_ColumnNames::HDB_ColumnNames()
{
    /*
     * The order of the items is not dependent on the enum order, but it
     * might be better to maintain that order.
     *
     * Names are only provided for actual columns, there is no need to provide
     * names for NO_COLUMN or LAST_COLUMN_ID
     */
    addToDictionary(ColumnIds::DATE, "Date");
    addToDictionary(ColumnIds::TEMPERATURE, "Temperature");
    addToDictionary(ColumnIds::PUSLE_RATE, "Pulse Rate");
    addToDictionary(ColumnIds::RESPIRATION_RATE, "Respiration Rate");
    addToDictionary(ColumnIds::BLOOD_PRESSURE, "Blood Pressure");
    addToDictionary(ColumnIds::BLOOD_OXYGEN, "Blood Oxygen");
    addToDictionary(ColumnIds::WEIGHT, "Weight");
    addToDictionary(ColumnIds::BLOOD_SUGAR, "Blood Glucose");
    addToDictionary(ColumnIds::EXERCISE, "Exercise");
    addToDictionary(ColumnIds::SLEEP_HOURS, "Sleep Hours");
    addToDictionary(ColumnIds::SLEEP_INTERRUPTIONS, "Sleep Interruptions");
    addToDictionary(ColumnIds::BOWEL_MOVEMENTS, "Bowel Movements");
    addToDictionary(ColumnIds::NUTRITION_CALORIES, "Calories");
    addToDictionary(ColumnIds::NUTRITION_SATURATED_FAT, "Saturated Fat");
    addToDictionary(ColumnIds::NUTRITION_TRANS_FAT, "Trans Fat");
    addToDictionary(ColumnIds::NUTRITION_CHOLESTEROL, "Cholesterol");
    addToDictionary(ColumnIds::NUTRITION_SODIUM, "Sodium");
    addToDictionary(ColumnIds::NUTRITION_FIBER, "Fiber");
    addToDictionary(ColumnIds::NUTRITION_TOTAL_SUGARS, "Total Sugars");
    addToDictionary(ColumnIds::NUTRITION_PROTIEN, "Protien");
    addToDictionary(ColumnIds::NUTRITION_VITAMIN_D, "Vitamin D");
    addToDictionary(ColumnIds::NUTRITION_CALCIUM, "Calcium");
    addToDictionary(ColumnIds::NUTRITION_IRON, "Iron");
    addToDictionary(ColumnIds::NUTRITION_POTASSIUM, "Potassium");
    addToDictionary(ColumnIds::NOTES, "Notes");
}

std::string_view HDB_ColumnNames::convertToString(ColumnIds id) const noexcept
{
    std::string_view columnName;

    if (auto search = idToName.find(id); search != idToName.end())
    {
        columnName = search->second;
    }
    else
    {
        columnName = "Not found\n";
    }

    return columnName;
}

ColumnIds HDB_ColumnNames::convertToId(std::string_view columnName) const noexcept
{
    ColumnIds id = ColumnIds::NO_COLUMN;

    if (auto search = nameToID.find(columnName); search != nameToID.end())
    {
        id = search->second;
    }

    return id;
}

void HDB_ColumnNames::addToDictionary(ColumnIds id, std::string_view columnName)
{
    idToName.insert({id, columnName});
    nameToID.insert({columnName, id});
}
