#include <QApplication>
#include <QLabel>

int main(int argc, char* argv[]) {
  QApplication a(argc, argv);
  QLabel* label = new QLabel{"This is application b"};
  label->show();
  return a.exec();
}