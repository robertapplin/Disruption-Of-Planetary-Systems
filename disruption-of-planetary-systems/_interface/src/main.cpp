#include "DPSInterface.h"

#include <QtWidgets/QApplication>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  DPSInterface w;
  w.show();
  return a.exec();
}
