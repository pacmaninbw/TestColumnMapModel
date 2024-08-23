#ifndef DISPALYTODBTRANSFERDATA_H
#define DISPALYTODBTRANSFERDATA_H

#include "columnidenum.h"

typedef struct DisplayToDBTransferData
{
    ColumnIds columnId = ColumnIds::NO_COLUMN;
    unsigned int position = 0;
    bool enabled = false;
}
DisplayToDBTransferData;

#endif // DISPALYTODBTRANSFERDATA_H
