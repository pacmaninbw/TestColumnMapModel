#include "columnaccessdata.h"

ColumnAccessDataMap::ColumnAccessDataMap(
        ColumnIds id,
        std::string nameIn,
        std::string tableNameIn,
        std::initializer_list<std::string> (columns))
    : columnId{id},
      name{nameIn},
      tableName{tableNameIn},
      positionInDisplay{0},
      enabled{false}
{
    for (auto column: columns)
    {
        columnsInTable.push_back(column);
    }
}
