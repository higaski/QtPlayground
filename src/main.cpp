#include <QApplication>
#include <QLabel>

int main(int argc, char* argv[]) {
  QApplication a(argc, argv);
  QLabel* label = new QLabel{"Some fucking test"};
  label->show();
  return a.exec();
}