#ifndef DISPALYTODBTRANSFERDATA_H
#define DISPALYTODBTRANSFERDATA_H

#include "columnidenum.h"
//#include <string>

/*
 * The second field in the struct is the unsigned int position value. When
 * initializing this struct the value may appear to be a magic number, however,
 * it indicates the position in the output of the columns in the table.
 */
typedef struct DisplayToDBTransferData
{
    ColumnIds columnId = ColumnIds::NO_COLUMN;
    unsigned int position = 0;
    bool enabled = false;

    std::string toString();
}
DisplayToDBTransferData;

#endif // DISPALYTODBTRANSFERDATA_H
