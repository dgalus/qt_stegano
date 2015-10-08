#ifndef ENCODEWINDOW_H
#define ENCODEWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QFile>
#include <QtGui>
#include "ViewImageWindow.h"
#include "GraphWindow.h"
#include "TextConstants.h"
#include "Enums.h"
#include "Stegano.h"

namespace Ui {
class EncodeWindow;
}

class EncodeWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit EncodeWindow(QWidget *parent = 0);
    Stegano* stegano;
    ~EncodeWindow();

private slots:
    void on_selectTargetButton_clicked();
    void on_selectSourceButton_clicked();
    void on_hideButton_clicked();
    void on_rSlider_valueChanged(int value);
    void on_gSlider_valueChanged(int value);
    void on_bSlider_valueChanged(int value);
    void on_bitNumbersCheckBox_clicked();
    void on_showGraphWindowButton_clicked();
    void on_viewPicturesButton_clicked();

private:
    void showSliders();
    void hideSliders();
    void hideHideButton();
    void showHideButton();
    void updateLabels();
    void setButtonsAfterEncryption();
    int calcMaximumSpaceToUse();

    TextConstants textConstants;
    QByteArray targetFileContent;
    QByteArray sourceFileContent;
    QString targetFileName;
    QString outputFileName;
    unsigned long long int targetFileSize;
    unsigned long long int sourceFileSize;
    Enums::FileFormat fileFormat;
    bool buttonShowed = false;
    Ui::EncodeWindow *ui;
};

#endif
