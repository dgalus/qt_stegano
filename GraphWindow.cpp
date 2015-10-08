#include "GraphWindow.h"
#include "ui_GraphWindow.h"

GraphWindow::GraphWindow(int angle, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GraphWindow)
{
    ui->setupUi(this);
    this->angle = angle;
    this->setMinimumHeight(400);
    this->setMinimumWidth(550);
    this->setMaximumHeight(400);
    this->setMaximumWidth(550);
}

GraphWindow::~GraphWindow()
{
    delete ui;
}

void GraphWindow::paintEvent(QPaintEvent *e)
{
    painter = new QPainter(this);

    QPen pen(Qt::blue);
    painter->setPen(pen);
    painter->setBrush(QBrush(Qt::blue));
    painter->drawEllipse(30, 50, 300, 300);

    painter->save();

    QPen pen2(Qt::yellow);
    painter->setPen(pen2);
    painter->setBrush(QBrush(Qt::yellow));
    painter->drawPie(30, 50, 300, 300, 0, angle*16);

    painter->save();


    painter->setPen(Qt::black);
    painter->drawRect(369, 182, 21, 21);
    painter->fillRect(370, 183, 20, 20, Qt::yellow);
    painter->setFont(QFont("Arial", 15));
    painter->drawText(400, 200, textConstants.ConstUsedSpaceGraph);

    painter->setPen(Qt::black);
    painter->drawRect(369, 232, 21, 21);
    painter->fillRect(370, 233, 20, 20, Qt::blue);
    painter->drawText(400, 250, textConstants.ConstFreeSpaceGraph);

    painter->save();
}
