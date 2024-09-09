#ifndef DISPALYTODBTRANSFERDATA_H
#define DISPALYTODBTRANSFERDATA_H

#include "columnidenum.h"
#include "hdb_columnnames.h"
#include <iostream>

typedef struct DisplayToDBTransferData
{
    ColumnIds columnId = ColumnIds::NO_COLUMN;
    unsigned int position = 0;
    bool enabled = false;

    void showStruct()
    {
        HDB_ColumnNames dictionary;

        std::cout << "Debugging or Testing DisplayToDBTransferData\n";
        std::cout << "\tColumnId: " << dictionary.convertToString(columnId)  << "\n";
        std::cout << "\tPosition: " << position << "\n";
        std::cout << "\tEnabled: " << (enabled ? "True" : "False") << "\n";
    }
}
DisplayToDBTransferData;

#endif // DISPALYTODBTRANSFERDATA_H
