#include "columnidenum.h"
#include "hdb_columnnames.h"
#include <string>
#include "dispalytodbtransferdata.h"

std::string DisplayToDBTransferData::toString()
{
    std::string strValue("Debugging or Testing DisplayToDBTransferData\n");
    HDB_ColumnNames dictionary;

    strValue += "\tColumnId: " + (dictionary.convertToString(columnId))  + "\n";
    strValue += "\tPosition: " + std::to_string(position);
    strValue += "\n";
    strValue += "\tEnabled: ";
    strValue += (enabled ? "True\n" : "False\n");

    return strValue;
}