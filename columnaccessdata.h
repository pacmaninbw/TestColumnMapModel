#ifndef COLUMNACCESSDATA_H
#define COLUMNACCESSDATA_H

#include "columnidenum.h"
#include "tableidenum.h"
#include <initializer_list>
#include <string>
#include <vector>

class ColumnAccessDataMap
{
public:
    ColumnAccessDataMap(
        ColumnIds id,
        TableIds tableIdIn,
        std::string nameIn,
        std::initializer_list<std::string>(columns)
    );
    ColumnIds getColumnId() const noexcept { return columnId; }
    TableIds getTableId() const noexcept { return tableID; }
    const std::string getName() const noexcept { return name; };
    const std::vector<std::string> getColumnsInTable() noexcept { return columnsInTable; };

private:
    ColumnIds columnId;
    TableIds tableID;
    std::string name;
    std::vector<std::string> columnsInTable;
};

#endif // COLUMNACCESSDATA_H

