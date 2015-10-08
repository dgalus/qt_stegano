#include "SelectModeWindow.h"
#include "ui_SelectModeWindow.h"

SelectModeWindow::SelectModeWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SelectModeWindow)
{
    ui->setupUi(this);
}

SelectModeWindow::~SelectModeWindow()
{
    delete ui;
}

void SelectModeWindow::on_encodeButton_clicked()
{
    EncodeWindow *ew = new EncodeWindow();
    ew->show();
    hide();
}

void SelectModeWindow::on_decodeButton_clicked()
{
    DecodeWindow *dw = new DecodeWindow();
    dw->show();
    hide();
}
