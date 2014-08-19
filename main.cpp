#include "drop_window.h"
#include "util.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DropWindow w(Util::getExePath() + "\\key");
    w.show();
    return a.exec();
}
