#include "editor.h"
#include "calcsheet.h"
#include <QClipboard>
#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QScreen>
#include <QGuiApplication>
#include <QMenu>
#include <QMenuBar>
#include <QIcon>
#include <QDialog>
#include <QMessageBox>
#include <QSystemTrayIcon>
#include <QCoreApplication>

// Then use:
QString version = QT_VERSION_STR;
QString runtimeVersion = qVersion();

Editor::Editor() : QMainWindow()
{
    auto centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    setWindowTitle(tr("Qt-Calcsheet"));
    adjustSize();
    //setMinimumHeight(500);
    resize(800, 600);
    centralWidget->setContentsMargins(0, 0, 0, 0);

    auto menuBar = new QMenuBar(this);
    setMenuBar(menuBar);
    
    auto actionsMenu = new QMenu("&Actions", this);
    menuBar->addAction(actionsMenu->menuAction());
    auto helpMenu = new QMenu("&Help", this);
    menuBar->addAction(helpMenu->menuAction());

    auto exitAction = new QAction("E&xit", this);
    connect(exitAction, &QAction::triggered, this, [this]() {
        QApplication::quit();
    });
    actionsMenu->addAction(exitAction);

    auto dumpModelAction = new QAction("&Dump Model", this);
    connect(dumpModelAction, &QAction::triggered, this, [this]() {
        auto calcSheet = findChild<CalcSheet *>();
        if (calcSheet) {
            calcSheet->model()->dump();
        }   
    });
    actionsMenu->addAction(dumpModelAction);

    auto aboutAction = new QAction("&About", this);
    connect(aboutAction, &QAction::triggered, this, [this]() {
        auto msg = QString::fromUtf8(u8"🌟 Qt Calcsheet\n\n"
            u8"🖼️ A simple screenshot tool.\n\n"
            u8"🖥️ Compile-Version: %1\n"
            u8"🕒 Runtime-Version: %2\n\n"
            u8"👨‍💻 Author: Stepan Rutz / stepan.rutz AT stepanrutz.com\n"
            u8"📦 Version: 1.0.0")
            .arg(QString::fromUtf8(QT_VERSION_STR))
            .arg(QString::fromUtf8(qVersion()))
            ;
        QString appName = QApplication::applicationName();
        QString appVersion = QApplication::applicationVersion();
        QMessageBox aboutBox(this);
        aboutBox.setWindowTitle("About " + appName);
        //aboutBox.setTextFormat(Qt::RichText);
        aboutBox.setText(msg);
        aboutBox.addButton(QMessageBox::Ok);
        aboutBox.exec();    
    });
    helpMenu->addAction(aboutAction);

    auto calcSheet = new CalcSheet(centralWidget);
    auto layout = new QVBoxLayout(centralWidget);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(calcSheet);

}

Editor::~Editor()
{
}
