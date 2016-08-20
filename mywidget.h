#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include <QPixmap>

namespace Ui {
class MyWidget;
}

class MyWidget : public QWidget
{
    Q_OBJECT

    void drawBackground();
public:
    MyWidget(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;

private:
    QPixmap *m_pixmap;
};

#endif // MYWIDGET_H
