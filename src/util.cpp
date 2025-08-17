#include "util.h"


void Util::selectNextTableRow(QTableView *tableView) {
    if (!tableView) return;

    QModelIndex current = tableView->currentIndex();
    if (!current.isValid()) return;

    QModelIndex next = current.sibling(current.row() + 1, current.column());
    if (next.isValid()) {
        tableView->setCurrentIndex(next);
        tableView->scrollTo(next);
    }
}
