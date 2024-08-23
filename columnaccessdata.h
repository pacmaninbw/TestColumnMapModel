#ifndef COLUMNACCESSDATA_H
#define COLUMNACCESSDATA_H

#include "columnidenum.h"
#include <initializer_list>
#include <string>
#include <vector>

class ColumnAccessDataMap
{
public:
    ColumnAccessDataMap(
            ColumnIds id,
            std::string nameIn,
            std::string tableNameIn,
            std::initializer_list<std::string>(columns)
    );
    void setPosition(unsigned int position) noexcept { positionInDisplay = position; }
    void setEnabled(bool enabler) noexcept {enabled = enabler; };
    unsigned int getPosition() const noexcept { return positionInDisplay; }
    const ColumnIds getColumnId() const noexcept { return columnId; }
    const bool getEnabled() const noexcept { return enabled; };
    const std::string getName() const noexcept { return name; };
    const std::string getTableName() const noexcept { return tableName; };
    const std::vector<std::string> getColumnsInTable() noexcept { return columnsInTable; };

private:
    unsigned int positionInDisplay;
    std::string name;
    std::string tableName;
    std::vector<std::string> columnsInTable;
    ColumnIds columnId;
    bool enabled;
};

#endif // COLUMNACCESSDATA_H

