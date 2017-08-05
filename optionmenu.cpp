#include "optionmenu.h"

OptionMenu::OptionMenu(QWidget *parent) :
    QMenu(parent)
{
    setStyleSheet("background-color:rgb(240, 240, 240);");
    addAction("About",parent,SLOT(about()));
    addAction("About Qt",parent,SLOT(aboutQt()));
    addSeparator();
    addAction("Quit",parent,SLOT(close()));
}
