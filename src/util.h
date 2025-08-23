#ifndef UTIL_H
#define UTIL_H

#include <QTableView>
#include <QLocale>

struct Util {
    static void selectPreviousTableRow(QTableView *tableView);
    static void selectNextTableRow(QTableView *tableView);

    static QVariant parseNumber(const QLocale &locale, const QString &text);
};

#endif // UTIL_H
