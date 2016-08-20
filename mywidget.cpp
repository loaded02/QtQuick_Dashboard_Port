#include "mywidget.h"
#include <QtWidgets>
#include <QPixmap>
#include <QPainter>

MyWidget::MyWidget(QWidget *parent) :
    QWidget(parent)
{
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(100);

    setWindowTitle(tr("My Analog Widget"));
    resize(400, 400);
    this->setStyleSheet("background-color:black;");
}

void MyWidget::drawBackground() {
    m_pixmap = new QPixmap(size());
    m_pixmap->fill(QColor(0,0,0,0));
    QPainter painter(m_pixmap);

    QPainter painter2(this);
    painter2.drawPixmap(0,0,*m_pixmap);
}

void MyWidget::paintEvent(QPaintEvent *)
{
    int baseWidth = 4;
    int tipWidth = 2;
    int needleLength = 90;
    static const QPoint needleLighter[4] = {
        QPoint(0, 0),
        QPoint(baseWidth/2, -3),
        QPoint(tipWidth/2, -needleLength),
        QPoint(0, -needleLength)
    };
    static const QPoint needleDarker[4] = {
        QPoint(0, 0),
        QPoint(-baseWidth/2, -3),
        QPoint(-tipWidth/2, -needleLength),
        QPoint(0, -needleLength)
    };

    QColor needleLighterColor(229,0,26);
    QColor needleDarkerColor(187,0,21);
    QColor scaleColor(255,255,255,192);
    int side = qMin(width(),height());

    /*debug animation*/
    static double value = 0;
    double maxValue = 280.0;
    double minValue = 0;
    static bool clockWise = true;
    if (value > maxValue)
        clockWise = false;
    else if (value < minValue)
        clockWise = true;
    if (clockWise)
        value += 1.0;
    else
        value -= 1.0;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(width()/2,height()/2);
    painter.scale(side/200.0,side/200.0);

    QStyleOption opt;
    opt.init(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
    /*fine scale*/
    painter.save();
    painter.setPen(scaleColor);
    painter.rotate(130.0); //280 + 80 = 360
    for(int i=0; i<=28;i++) {
        painter.drawLine(90,0,96,0);
        painter.rotate(10.0);
    }
    painter.restore();
    /*scale*/
    painter.save();
    painter.setPen(scaleColor);
    painter.rotate(130.0);
    for(int i= 0; i<=140; i++) {
        if (i%5)
            painter.drawLine(94,0,96,0);
        painter.rotate(2.0);
    }
    painter.restore();
    /*numbers of scale*/
    painter.setPen(scaleColor);
    QFont font = painter.font();
    font.setPixelSize(10);
    painter.setFont(font);
    for (int i=0; i<=14; i++) {
        int val = i * (280.0) / 14.0;
        qreal x = qCos((i*20.0+130.0)*(M_PI/180.0)) * 75.0;
        qreal y = qSin((i*20.0+130.0)*(M_PI/180.0)) * 75.0;
        painter.save();
        painter.translate(x, y);
        painter.drawText(QRect(-10, -5, 20, 10), Qt::AlignCenter, QString::number(val));
        painter.restore();
    }
    /*value*/
    painter.setPen(scaleColor);
    font.setPixelSize(18);
    font.setBold(true);
    painter.setFont(font);
    QRectF rect(-20.0,10.0,40.0,20.0);
    painter.drawText(rect, Qt::AlignCenter, QString::number(value));
    /*unit*/
    font.setPixelSize(8);
    font.setBold(false);
    painter.setFont(font);
    QRectF rectUnit(-10.0,35.0,20.0,20.0);
    painter.drawText(rectUnit, Qt::AlignCenter, "km/h");
    /*needle*/
    painter.setPen(Qt::NoPen);
    painter.setBrush(needleDarkerColor);
    painter.save();
    painter.rotate(value - 140);
    painter.drawConvexPolygon(needleDarker,4);
    painter.setBrush(needleLighterColor);
    painter.drawConvexPolygon(needleLighter,4);
    painter.restore();
}
