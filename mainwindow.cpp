#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDebug>
#include <QDesktopWidget>
#include <QMessageBox>

//构造函数
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    //变量
    ui(new Ui::MainWindow),
    triggered(false),
    optMenu(nullptr),
    //常量
    TRIGGER_DISTANCE(10),
    TRIGGER_ORIENTATION(Qt::Vertical|Qt::Horizontal),
    DOCK_WIDTH(5)

{
    ui->setupUi(this);
    optMenu = new OptionMenu(this);
    anime = new QPropertyAnimation(this,"geometry");

    setWindowFlags(Qt::FramelessWindowHint|\
                   Qt::WindowStaysOnTopHint|\
                   Qt::Tool);//隐藏窗口标题
    setAttribute(Qt::WA_TranslucentBackground, true);//透明
    setAttribute(Qt::WA_AlwaysStackOnTop,true);
    setAttribute(Qt::WA_QuitOnClose,true);
    windowFlags();
}
//析构函数
MainWindow::~MainWindow()
{
    delete optMenu;
    delete ui;
}
/*计算目标位置
 * edge：窗体所紧贴的边缘
 * type：动画的类型，Show/Hide
 * width：缩起后剩下的宽度
*/
QRect MainWindow::target_geometry(Qt::Edge edge,AnimeType type,int width)
{
    QDesktopWidget *desktop = QApplication::desktop();
    QRect wndRect = geometry();
    QRect scrRect = desktop->screen(desktop->screenNumber(this))->rect();
    QRect res = wndRect;//当前位置

    if(type == Hide)
    {
        switch (edge) {
        case Qt::LeftEdge:
            res.moveRight(scrRect.left() + width);
            break;
        case Qt::RightEdge:
            res.moveLeft(scrRect.right() - width);
            break;
        case Qt::TopEdge:
            res.moveBottom(scrRect.top() + width);
            break;
        case Qt::BottomEdge:
            //向下缩就不弄了，很奇怪
            break;
        default:
            break;
        }
    }
    else
    {
        switch (edge) {
        case Qt::LeftEdge:
            res.moveLeft(scrRect.left());
            break;
        case Qt::RightEdge:
            res.moveRight(scrRect.right());
            break;
        case Qt::TopEdge:
            res.moveTop(scrRect.top());
            break;
        case Qt::BottomEdge:
            //do nothing
            break;
        default:
            break;
        }
    }
    return res;
}
/*滑动动画函数
 * edge：目标边缘
 * type：动画类型，Show/Hide
*/
void MainWindow::slide(Qt::Edge edge, MainWindow::AnimeType type)
{
    anime->setEasingCurve(QEasingCurve::InOutQuart);
    anime->setDuration(400);
    anime->setStartValue(geometry());
    anime->setEndValue(target_geometry(edge,type,DOCK_WIDTH));
    anime->start(QPropertyAnimation::KeepWhenStopped);
    triggered = true;
    current_edge = edge;
}
/**          事 件 重 载           **/
//鼠标按下事件，记录按下鼠标时指针在窗体中的坐标
void MainWindow::mousePressEvent(QMouseEvent* e)
{
    ref_point = e->pos();
}
//鼠标释放事件
void MainWindow::mouseReleaseEvent(QMouseEvent* e)
{
    Q_UNUSED(e);
    QDesktopWidget *desktop = QApplication::desktop();
    /*检测边缘触发*/
    int res = triggerCheck(geometry(),desktop->screen(desktop->screenNumber(this))->rect(),TRIGGER_DISTANCE);

    if(res)//如果触发到边缘
    {
        //触发的边缘可能有多条，最终进入的优先级为:左>右>上>下
        if(TRIGGER_ORIENTATION == (Qt::Horizontal|Qt::Vertical) )//两个方向都允许收缩
        {
            if(res & Qt::LeftEdge)slide(Qt::LeftEdge,Hide);
            else if(res & Qt::RightEdge)slide(Qt::RightEdge,Hide);
            else if(res & Qt::TopEdge)slide(Qt::TopEdge,Hide);
            else if(res & Qt::BottomEdge)slide(Qt::BottomEdge,Hide);
            else triggered = false;

        }
        else if(TRIGGER_ORIENTATION == Qt::Horizontal)
        {
            if(res & Qt::LeftEdge)slide(Qt::LeftEdge,Hide);
            else if(res & Qt::RightEdge)slide(Qt::RightEdge,Hide);
            else triggered = false;//如果没有合法的触发，复位触发标志
        }
        else if(TRIGGER_ORIENTATION == Qt::Vertical)
        {
            if(res & Qt::TopEdge)slide(Qt::TopEdge,Hide);
            else if(res & Qt::BottomEdge)slide(Qt::BottomEdge,Hide);
            else triggered = false;
        }
    }
    else triggered = false;//如果没有触发，复位触发标志
}
//鼠标移动事件，实现窗口拖拽移动
void MainWindow::mouseMoveEvent(QMouseEvent* e)
{
    move(e->globalPos() - ref_point);
}
//鼠标离开事件
void MainWindow::leaveEvent(QEvent *)
{
    if(triggered)slide(current_edge,Hide);
}
//鼠标进入事件
void MainWindow::enterEvent(QEvent *)
{
    if(triggered)slide(current_edge,Show);
}
/**    槽 函 数    **/
//关于
void MainWindow::about()
{
    QMessageBox::about(this,"About","This is a clipboard enhancer.");
}
//关于Qt
void MainWindow::aboutQt()
{
    QMessageBox::aboutQt(this);
}
//按钮槽函数
void MainWindow::on_pushButton_clicked()
{
    QRect menuRect = optMenu->geometry();
    menuRect.moveTopRight(geometry().topLeft()+ui->pushButton->geometry().bottomRight());
    optMenu->popup(menuRect.topLeft());
}
/**    类 外 函 数    **/
//判断窗体是否满足边缘触发
#define DISTANCE_OF(pa,pb) qAbs(pa-pb)
int triggerCheck(QRect wndRec, QRect scrRec, int threshold)
{
    int res = 0;

    if(DISTANCE_OF(wndRec.left(),scrRec.left()) < threshold)res|=Qt::LeftEdge;
    if(DISTANCE_OF(wndRec.right(),scrRec.right()) < threshold)res|=Qt::RightEdge;
    if(DISTANCE_OF(wndRec.top(),scrRec.top()) < threshold)res|=Qt::TopEdge;
    if(DISTANCE_OF(wndRec.bottom(),scrRec.bottom()) < threshold)res|=Qt::BottomEdge;

    return res;
}


