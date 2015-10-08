#include "ViewImageWindow.h"
#include "ui_ViewImageWindow.h"

ViewImageWindow::ViewImageWindow(QString filename, QString state, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ViewImageWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(state);
    QImage image(filename);
    this->setMaximumHeight(image.height()+20);
    this->setMaximumWidth(image.width()+20);
    this->setMinimumHeight(image.height()+20);
    this->setMinimumWidth(image.width()+20);
    ui->label->setMaximumHeight(image.height());
    ui->label->setMaximumWidth(image.width());
    ui->label->setMinimumHeight(image.height());
    ui->label->setMinimumWidth(image.width());
    ui->label->setPixmap(QPixmap::fromImage(image));
}

ViewImageWindow::~ViewImageWindow()
{
    delete ui;
}
