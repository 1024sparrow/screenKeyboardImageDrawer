#include "Drawer.h"

#include <QPainter>
#include <QList>
#include <QDebug>//boris debug

#define E(T) {*p_error=#T"\n";return false;}

Drawer::Drawer()
{
}

QPixmap Drawer::pixmap() const
{
    return _pixmap;
}

bool Drawer::draw(const Source &p_source, const char **p_error)
{
    int
        x = 0,
        h = 0
    ;

    QList<QPixmap> fragments;
    for (auto oVariant : p_source.variants)
    {
        QPixmap fragment;
        if (!_drawVariant(oVariant, fragment, p_error))
            return false;
        if (fragment.height() > h)
            h = fragment.height();
        x += fragment.width();
        fragments.append(fragment);
    }

    _pixmap = QPixmap(x, h);
    _pixmap.fill(Qt::red);
    QPainter painter(&_pixmap);
    x = 0;
    for (auto oFragment : fragments)
    {
        painter.drawPixmap(x, 0, oFragment);

        x += oFragment.width();
    }
    return true;
}

bool Drawer::_drawVariant(const Source::SourceVariant &p_source, QPixmap &p_result, const char **p_error)
{
    int shift = 0, h = 0, w = 0;
    QList<QPixmap> fragments;
    for (auto oLay : p_source.layouts)
    {
        QPixmap fragment;
        if (!_drawVariantLay(p_source, fragment, p_error))
            return false;
        if (p_source.orientation == Source::SourceVariant::orientVertical)
        {
            w = fragment.width();
            h += fragment.height();
        }
        else // orientHorizontal
        {
            w += fragment.width();
            h = fragment.height();
        }
        fragments.append(fragment);
    }
    p_result = QPixmap(w,h);
    shift = 0;
    QPainter painter(&p_result);
    for (int iFragment = 0 ; iFragment < p_source.layouts.length() ; ++iFragment)
    {
        auto &oLay = p_source.layouts[iFragment];
        auto &fragment = fragments[iFragment];
        if (p_source.orientation == Source::SourceVariant::orientVertical)
        {
            painter.drawPixmap(0, shift, fragment);
            shift += fragment.height();
        }
        else
        {
            painter.drawPixmap(shift, 0, fragment);
            shift += fragment.width();
        }
    }
    return true;
}

bool Drawer::_drawVariantLay(const Source::SourceVariant &p_source, QPixmap &p_result, const char **p_error)
{
//    if (p_source.width < 256)
//        E(too little width set (minimum 256));
//    if (p_source.height < 256)
//        E(too little height set (minimum 256));

    p_result = QPixmap(p_source.width, p_source.height);
    p_result.fill(QColor(0x44,0,0));

    _scaleX = double(p_source.width+1) / double(p_source.buttons.width);
    _scaleY = double(p_source.height) / double(p_source.buttons.height);

    QPainter painter(&p_result);
    painter.setPen(Qt::yellow);
    //painter.drawRect(5,5,100,50);

    int x{0},y{0};
    for (QString oRow : p_source.buttons.rows)
    {
        int rowHeight = 0;
        QString bnId;
        int bnWidth = 0;
        x = 0;

        enum {
            sRowHeight = 0,
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
                //qDebug() << "** " << ch;
                if (ch == ' ')
                {
                    if (bnId.length())
                    {
                        // TODO: переходим к следующей кнопке
                        //qDebug() << "  button " << bnId.c_str();
                        //qDebug() << "1 button " << bnId << "with width " << bnWidth;
                        _drawButton(painter, x, y, bnWidth, rowHeight, bnId);
                        x += bnWidth;

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
                //qDebug() << "====button " << bnId << bnWidth << ":" << ch;
                if (ch >= '0' && ch <= '9')
                {
                    bnWidth = 10 * bnWidth + static_cast<int>(ch - '0');
                }
                else if (ch == ' ')
                {
                    //qDebug() << "2 button " << bnId << "with width " << bnWidth;
                    _drawButton(painter, x, y, bnWidth, rowHeight, bnId);
                    x += bnWidth;
                    bnId.clear();
                    state = sBnId;
                }
                else
                {
                    qDebug() << "+++++++++" << ch;
                }
            }
        }
        if (state == sEmergency)
        {
            E(incorrect p_source data [1]);
        }
        else if (bnWidth)
        {
            //qDebug() << "3 button " << bnId << "with width " << bnWidth;
            _drawButton(painter, x, y, bnWidth, rowHeight, bnId);
        }
//        qDebug() << "##" << source.width << x << bnWidth;
        if (p_source.buttons.width != (x + bnWidth))
        {
            E(Incorrect width for row)
        }
        //qDebug() << "rowHeight:" << rowHeight;
        y += rowHeight;
    }
    if (p_source.buttons.height != y)
    {
        E(Incorrect height fow rows)
    }

    //E(not implemented);
    return true;
}

void Drawer::_drawButton(QPainter &p_painter, int p_x, int p_y, int p_w, int p_h, const QString &p_name)
{
    double
        x = (double)_scaleX * (double)p_x,
        y = (double)_scaleY * (double)p_y,
        w = (double)_scaleX * (double)p_w,
        h = (double)_scaleY * (double)p_h
    ;
    //p_painter.drawRect(x,y,w,h);
    //p_painter.drawRect(QRectF(x,y,w-1,h-1));
    p_painter.drawRect(QRectF(x,y,w,h));

    p_painter.drawText(QRect(x,y,w,h), p_name, QTextOption(Qt::AlignCenter));
}



























