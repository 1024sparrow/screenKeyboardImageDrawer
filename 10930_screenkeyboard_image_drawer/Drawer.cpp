#include "Drawer.h"

#include <QPainter>
#include <QDebug>//boris debug

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

    int x{0},y{0};
    for (QString oRow : source.buttons.rows)
    {
        int rowHeight = 0;
        QString bnId;
        int bnWidth = 0;

        enum {
            sRowHeight,
            sBnId,
            sBnWidth,
            sEmergency
        } state {sRowHeight};

        for (char ch : oRow.toStdString())
        {
            //if (ch == ' ')
            //    continue;
            if (state == sRowHeight)
            {
                if (ch >= '0' && ch <= '9')
                {
                    rowHeight = 10 * rowHeight + static_cast<int>(ch - '0');
                }
                else if (ch == ':')
                {
                    bnId.clear();
                    state = sBnId;
                }
                else
                {
                    state = sEmergency;
                }
            }
            else if (state == sBnId)
            {
                qDebug() << "** " << ch;
                if (ch == ' ')
                {
                    if (bnId.length())
                    {
                        // TODO: переходим к следующей кнопке
                        //qDebug() << "  button " << bnId.c_str();
                        qDebug() << "1 button " << bnId << "with width " << bnWidth;

                        bnId.clear();
                        state = sBnId;
                    }
                    else
                    {
                        continue;
                    }
                }
                else if (ch == ':')
                {
                    bnWidth = 0;
                    state = sBnWidth;
                }
                else
                {
                    bnId += ch;
                }
            }
            else if (state == sBnWidth)
            {
                qDebug() << "====button " << bnId << bnWidth << ":" << ch;
                if (ch >= '0' && ch <= '9')
                {
                    bnWidth = 10 * bnWidth + static_cast<int>(ch - '0');
                }
                else if (ch == ' ')
                {
                    qDebug() << "2 button " << bnId << "with width " << bnWidth;
                    bnId.clear();
                    state = sBnId;
                }
                else
                {
                    qDebug() << "+++++++++" << ch;
                }
            }
            //if (state)
        }
        if (state == sEmergency)
        {
            E(incorrect source data [1]);
        }
        qDebug() << "rowHeight:" << rowHeight;

        y += rowHeight;
    }

    //E(not implemented);
    return true;
}



























