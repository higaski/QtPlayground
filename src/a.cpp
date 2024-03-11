#include <QApplication>
#include <QLabel>

int main(int argc, char* argv[]) {
  QApplication a(argc, argv);
  QLabel* label = new QLabel{"This is application a"};
  label->show();
  return a.exec();
}