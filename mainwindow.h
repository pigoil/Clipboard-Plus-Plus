#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QPropertyAnimation>
#include "optionmenu.h"

int triggerCheck(QRect wndRec,QRect scrRec,int threshold);

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    enum AnimeType {
        Show,
        Hide
    };

    Ui::MainWindow *ui;
    QPoint ref_point;
    Qt::Edge current_edge;
    bool triggered;
    OptionMenu* optMenu;
    QPropertyAnimation* anime;

    const int TRIGGER_DISTANCE;
    const int TRIGGER_ORIENTATION;
    const int DOCK_WIDTH;

    QRect target_geometry(Qt::Edge edge,AnimeType type,int width);
    void slide(Qt::Edge edge,AnimeType type);
protected:
    void mouseMoveEvent(QMouseEvent* );
    void mousePressEvent(QMouseEvent* );
    void mouseReleaseEvent(QMouseEvent* );
    void leaveEvent(QEvent* );
    void enterEvent(QEvent*);
public slots:
    void about();
    void aboutQt();
private slots:
    void on_pushButton_clicked();
};

#endif // MAINWINDOW_H
