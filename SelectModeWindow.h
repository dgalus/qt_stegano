#ifndef SELECTMODEWINDOW_H
#define SELECTMODEWINDOW_H

#include <QMainWindow>
#include "EncodeWindow.h"
#include "DecodeWindow.h"

namespace Ui {
class SelectModeWindow;
}

class SelectModeWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SelectModeWindow(QWidget *parent = 0);
    ~SelectModeWindow();

private slots:
    void on_encodeButton_clicked();
    void on_decodeButton_clicked();

private:
    Ui::SelectModeWindow *ui;
};

#endif // SELECTMODEWINDOW_H
