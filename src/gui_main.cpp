#include "main_window.hpp"

#include <QApplication>
#include <QDir>
#include <QPixmap>
#include <QTimer>

int main(int argc, char** argv)
{
    QApplication application{argc, argv};

    main_window window;
    window.show();

    const bool capture_screenshot = application.arguments().contains(
        "--capture-screenshot");

    if (capture_screenshot)
    {
        QTimer::singleShot(600, [&application, &window]()
        {
            QDir{}.mkpath("docs");
            window.grab().save("docs/pixelwave_gui.png");
            application.quit();
        });
    }

    return application.exec();
}
