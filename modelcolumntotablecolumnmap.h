#ifndef MODELCOLUMNTOTABLECOLUMNMAP_H
#define MODELCOLUMNTOTABLECOLUMNMAP_H

#include <string>
#include <vector>
#include "columnaccessdata.h"
#include "dispalytodbtransferdata.h"
#define MY_UNIT_TEST 1

/*
 * This class will be a component of a QT Table model either by aggregation
 * or inheritance.
 */
class ModelColumnToTableColumnMap
{
public:
    ModelColumnToTableColumnMap();
    void enableUsedColumns(std::vector<DisplayToDBTransferData> dialogInput) noexcept;
    const std::string buildQueryString() noexcept;
    const std::vector<ColumnAccessDataMap> getEnabledList() const noexcept { return enabledList; };
    void resetEnabledColumns();

#if (DEBUG || MY_UNIT_TEST)
    void debugPrintEnabledList() { debugPrintList(enabledList); };
    void debugPrintBaseTable() { debugPrintList(baseTable); };
    void debugPrintTransferDataList(std::vector<DisplayToDBTransferData> testList);
#endif

private:
    void enableAndAddtoList(DisplayToDBTransferData enabledColumn) noexcept;
    bool duplicateInEnabledList(DisplayToDBTransferData candidateColumn);
    std::vector<ColumnAccessDataMap> baseTable;
    std::vector<ColumnAccessDataMap> enabledList;

#if (DEBUG || MY_UNIT_TEST)
    void debugPrintList(std::vector<ColumnAccessDataMap> listToPrint);
    void debugPrintListMember(ColumnAccessDataMap mapToPrint);
    void debugPrintTransferData(DisplayToDBTransferData transferData);
#endif

};

#endif // MODELCOLUMNTOTABLECOLUMNMAP_H
