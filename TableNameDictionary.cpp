#include "TableNameDictionary.h"

TableNameDictionary::TableNameDictionary()
{
    addDefinition(TableIds::TEMPURATURE, "Tempurature", "tmp");
    addDefinition(TableIds::REPIRATION_RATE, "RepirationRate", "resp");
    addDefinition(TableIds::BLOOD_PRESSURE, "BloodPressure", "bp");;
    addDefinition(TableIds::BLOOD_OXYGEN, "BloodOxygen", "bo");
    addDefinition(TableIds::WEIGHT, "Weight", "wt");
    addDefinition(TableIds::BLOOD_SUGAR, "BloodSugar", "bg");
    addDefinition(TableIds::EXERCISE, "Exercise", "exer");
    addDefinition(TableIds::SLEEP, "Sleep", "slp");
    addDefinition(TableIds::BOWEL_MOVEMENTS, "BowlMovements", "bm");
    addDefinition(TableIds::NUTRITION, "Nutrition", "nut");
    addDefinition(TableIds::NOTES, "Notes", "nts");

}

TableNameDictionary::~TableNameDictionary()
{

}

std::pair<std::string_view, std::string_view>
    TableNameDictionary::getTableNames(TableIds id)
{
    std::pair<std::string_view, std::string_view> returnPair = 
        {"No Table Name", "No Short Table Name"};

    if (auto search = idToTabNames.find(id); search != idToTabNames.end())
    {
        returnPair = search->second;
    }


    return returnPair;
}

std::string_view TableNameDictionary::getTableName(TableIds id)
{
    std::pair<std::string_view, std::string_view> returnPair = getTableNames(id);
    return returnPair.first;
}

std::string_view TableNameDictionary::getShortName(TableIds id)
{
    std::pair<std::string_view, std::string_view> returnPair = getTableNames(id);
    return returnPair.second;
}

/*
 * Searches both Table Names and Short Table Names to find ID
 */
TableIds TableNameDictionary::getId(std::string_view tabName) const noexcept
{
    TableIds id = TableIds::NO_TABLE;

    if (auto search = TableToId.find(tabName); search != TableToId.end())
    {
        id = search->second;
    }

    if (id == TableIds::NO_TABLE)
    {
        if (auto search = ShortNameToId.find(tabName); search != ShortNameToId.end())
        {
            id = search->second;
        }
    }

    return id;
}

void TableNameDictionary::addDefinition(
        TableIds id, std::string_view tableName, std::string_view shortName
    )
{
    idToTabNames.insert({id, {tableName, shortName}});
    TableToId.insert({tableName, id});
    ShortNameToId.insert({shortName, id});
}