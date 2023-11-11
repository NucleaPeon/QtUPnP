#include "mainwindow.hpp"
#include <QApplication>
#ifdef Q_OS_MAC
#include "cocoainitializer.h"
#endif

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
#ifdef Q_OS_MAC
  CocoaInitializer initializer;
  Q_UNUSED(initializer)
#endif
  CMainWindow w;
  w.show();
  return a.exec ();
}
