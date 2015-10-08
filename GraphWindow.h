#ifndef GRAPHWINDOW_H
#define GRAPHWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include "TextConstants.h"

namespace Ui {
class GraphWindow;
}

class GraphWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit GraphWindow(int angle, QWidget *parent = 0);
    ~GraphWindow();
    TextConstants textConstants;

protected:
    QPainter *painter = new QPainter(this);
    void paintEvent(QPaintEvent *e);

private:
    Ui::GraphWindow *ui;
    int angle;
};

#endif // GRAPHWINDOW_H
