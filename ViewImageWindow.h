#ifndef VIEWIMAGEWINDOW_H
#define VIEWIMAGEWINDOW_H

#include <QMainWindow>

namespace Ui {
class ViewImageWindow;
}

class ViewImageWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ViewImageWindow(QString filename, QString state, QWidget *parent = 0);
    ~ViewImageWindow();

private:
    Ui::ViewImageWindow *ui;
};

#endif // VIEWIMAGEWINDOW_H
