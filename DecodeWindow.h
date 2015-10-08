#ifndef DECODEWINDOW_H
#define DECODEWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include "TextConstants.h"
#include "Stegano.h"

namespace Ui {
class DecodeWindow;
}

class DecodeWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit DecodeWindow(QWidget *parent = 0);
    ~DecodeWindow();

private slots:
    void on_selectFileButton_clicked();

private:
    TextConstants textConstants;
    QByteArray fileContent;
    Stegano stegano;
    Ui::DecodeWindow *ui;
};

#endif // DECODEWINDOW_H
