#include "columnaccessdata.h"

ColumnAccessDataMap::ColumnAccessDataMap(ColumnIds id,
        TableIds tabIdIn,
        std::string nameIn,
        std::initializer_list<std::string> (columns)
    )
    : columnId{id},
      tableID{tabIdIn},
      name{nameIn}
{
    for (auto column: columns)
    {
        columnsInTable.push_back(column);
    }
}

