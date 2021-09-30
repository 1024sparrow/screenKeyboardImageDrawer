#include "Drawer.h"

#include <QPainter>

#define E(T) {*error=#T"\n";return false;}

Drawer::Drawer()
{
}

QPixmap Drawer::pixmap() const
{
    return _pixmap;
}

bool Drawer::draw(const Source &source, const char **error)
{
//    *error = "not iplemented\n";
//    return false;
    //E(not implemented)

    if (source.width < 256)
        E(too little width set (minimum 256));
    if (source.height < 256)
        E(too little height set (minimum 256));
    _pixmap = QPixmap(source.width, source.height);
    _pixmap.fill(QColor(0x44,0,0));

    QPainter painter(&_pixmap);
    painter.setPen(Qt::yellow);
    painter.drawRect(5,5,100,50);

    //E(not implemented);
    return true;
}
