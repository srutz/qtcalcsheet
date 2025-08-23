#include "util.h"

void Util::selectPreviousTableRow(QTableView *tableView) {
    if (!tableView) return;

    QModelIndex current = tableView->currentIndex();
    if (!current.isValid()) return;

    QModelIndex previous = current.sibling(current.row() - 1, current.column());
    if (previous.isValid()) {
        tableView->setCurrentIndex(previous);
        tableView->scrollTo(previous);
    }
}

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

QVariant Util::parseNumber(const QLocale &locale, const QString &text) {
    // parse the text using the locale
    bool ok;
    auto n = locale.toDouble(text, &ok);
    return ok ? QVariant(n) : QVariant();  
}
