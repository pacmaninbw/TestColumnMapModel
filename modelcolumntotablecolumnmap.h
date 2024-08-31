#ifndef MODELCOLUMNTOTABLECOLUMNMAP_H
#define MODELCOLUMNTOTABLECOLUMNMAP_H

#include <string>
#include <vector>
#include "columnaccessdata.h"
#include "dispalytodbtransferdata.h"

typedef struct TableIdToTableName
{
    TableIds id = TableIds::NO_TABLE;
    std::string tableName;
    std::string abrevTableName;
}
TableIdToTableName;

/*
 * This class will be a component of a QT Table model either by aggregation
 * or inheritance.
 */
class ModelColumnToTableColumnMap
{
public:
    ModelColumnToTableColumnMap() = default;
    void enableUsedColumns(std::vector<DisplayToDBTransferData> dialogInput) noexcept;
    const std::string buildQueryString() noexcept;
    const std::vector<DisplayToDBTransferData> getEnabledList() const noexcept { return enabledList; };
    std::size_t getEnabledCount() const noexcept { return enabledList.size(); };
    void resetEnabledColumns();

#if (DEBUG || MY_UNIT_TEST)
    void debugPrintEnabledList() { debugPrintTransferDataList(enabledList); };
    void debugPrintTransferDataList(std::vector<DisplayToDBTransferData> testList);
#endif

private:
    void enableAndAddtoList(DisplayToDBTransferData enabledColumn) noexcept;
    bool duplicateInEnabledList(DisplayToDBTransferData candidateColumn);
    std::string addJoin(TableIds tblId) noexcept;
    std::string addColumn(ColumnIds columnId) noexcept;
    void addTableToJoinList(std::vector<TableIds> &tablesToJoin, TableIds newEntry) noexcept;
    std::string buildJoinList(std::vector<TableIds> tablesToJoin) noexcept;
    const ColumnAccessDataMap getColumnData(ColumnIds columnId) const noexcept;
    const ColumnAccessDataMap getColumnData(
            DisplayToDBTransferData transferData) const noexcept
    { return getColumnData(transferData.columnId); }
    ;
    const TableIdToTableName findTableData(TableIds tableId) const noexcept;
    const TableIdToTableName findTableData(ColumnIds columnId) const noexcept;
    const TableIdToTableName findTableData(ColumnAccessDataMap columnData) const noexcept
        { return findTableData(columnData.getTableId()); }
    const TableIdToTableName findTableData(DisplayToDBTransferData transferData) const noexcept
        { return findTableData(transferData.columnId); }
    const std::string getTableName(TableIds tableId) const noexcept
        { return findTableData(tableId).tableName; }
    const std::string getTableName(ColumnIds columnId) const noexcept
        { return findTableData(columnId).tableName; }
    const std::string getTableName(ColumnAccessDataMap columnData) const noexcept
        { return findTableData(columnData).tableName; }
    const std::string getTableName(DisplayToDBTransferData transferData) const noexcept
        { return findTableData(transferData).tableName; }
    const std::string getSqlColumnQueryPrefix(TableIds tableId) const noexcept
        { return findTableData(tableId).abrevTableName; }
    const std::string getSqlColumnQueryPrefix(ColumnIds columnId) const noexcept
        { return findTableData(columnId).abrevTableName; }
    const std::string getSqlColumnQueryPrefix(ColumnAccessDataMap columnData) const noexcept
        { return findTableData(columnData).abrevTableName; }
    const std::string getSqlColumnQueryPrefix(DisplayToDBTransferData transferData) const noexcept
        { return findTableData(transferData).abrevTableName; }

    std::vector<DisplayToDBTransferData> enabledList;

#if (DEBUG || MY_UNIT_TEST)
    void debugPrintList(std::vector<ColumnAccessDataMap> listToPrint);
    void debugPrintListMember(ColumnAccessDataMap mapToPrint);
    void debugPrintTransferData(DisplayToDBTransferData transferData);
#endif

};

#endif // MODELCOLUMNTOTABLECOLUMNMAP_H
