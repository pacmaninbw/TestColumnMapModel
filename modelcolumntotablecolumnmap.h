#ifndef MODELCOLUMNTOTABLECOLUMNMAP_H
#define MODELCOLUMNTOTABLECOLUMNMAP_H

#include <string>
#include <vector>
#include "columnaccessdata.h"
#include "dispalytodbtransferdata.h"

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
    const std::vector<DisplayToDBTransferData> getEnabledList()
            const noexcept { return enabledList; };
    // The following methods are used primarily in Unit Testing.
    std::size_t getEnabledCount() const noexcept { return enabledList.size(); };
    void resetEnabledColumns();

    // Debug and testing.
    void debugPrintEnabledList() { debugPrintTransferDataList(enabledList); };
    void debugPrintTransferDataList(std::vector<DisplayToDBTransferData> testList);

private:
    bool enableAndAddtoList(DisplayToDBTransferData enabledColumn);
    bool duplicateInEnabledList(DisplayToDBTransferData candidateColumn);

    // SQL Code Generation
    std::string addJoin(TableIds tblId) noexcept;
    std::string addColumn(ColumnIds columnId) noexcept;
    void addTableToJoinList(std::vector<TableIds> &tablesToJoin, TableIds newEntry) noexcept;
    std::string buildJoinList(std::vector<TableIds> tablesToJoin) noexcept;
    bool sqlSanityCheck();
    bool hasUnImplementedColumns() noexcept;
    bool hasInvalidColumnId() noexcept;

    // Utility
    const ColumnAccessDataMap getColumnData(ColumnIds columnId) const noexcept;
    const ColumnAccessDataMap getColumnData(
            DisplayToDBTransferData transferData) const noexcept
            { return getColumnData(transferData.columnId);};
    
    // Debug and testing.
    void debugPrintList(std::vector<ColumnAccessDataMap> listToPrint);
    void reportError(std::string whatErr, std::string whereError);

    std::vector<DisplayToDBTransferData> enabledList;

};

#endif // MODELCOLUMNTOTABLECOLUMNMAP_H
