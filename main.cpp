#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/icon.png"));
   MainWindow window;
   window.setWindowTitle("🌞 SunAngleCal");
   window.resize(500, 300);
   window.show();
   return app.exec();
}
