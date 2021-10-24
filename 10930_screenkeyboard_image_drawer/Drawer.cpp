#include "Drawer.h"

#include <QPainter>
#include <QList>

#define E(T) {*p_error=#T"\n";return false;}

QString Drawer::Source::SourceVariant::toString() const
{
    //return "<not implemented>";
    //return "{}";

    QString strLayouts = layouts.join("\", \"");
    QString strRows;
    for (auto oRow : buttons.parsedRows)
    {
        if (!strRows.isEmpty())
        {
            strRows.append(",");
        }
        strRows.append(R"(
                    )" + oRow.toString());
    }

    return QString(R"({
            "layouts": ["%1"],
            "width": %2,
            "height": %3,
            "buttons": {
                "width": %4,
                "height": %5,
                "rows": [ %6
                ]
            }
        })")
        .arg(strLayouts)
        .arg(width)
        .arg(height)
        .arg(buttons.width)
        .arg(buttons.height)
        .arg(strRows)
    ;
}

QString Drawer::Source::SourceVariant::Buttons::Row::toString() const
{
    QString strButtons;
    for (auto oButton : buttons)
    {
        if (!strButtons.isEmpty())
            strButtons.append(",");
        strButtons.append(QString(R"(
                            {
                                "width": %1,
                                "id": "%2"
                            })").arg(oButton.width).arg(oButton.id));
    }
    return QString(R"({
                        "height": %1,
                        "buttons": [ %2
                        ]
                    })")
        .arg(height).arg(strButtons);
}

QString Drawer::Source::toString() const
{
    QString strArrVariants;
    for (auto o : variants)
    {
        if (!strArrVariants.isEmpty())
        {
            strArrVariants.append(R"(,
        )");
        }
        strArrVariants.append(o.toString());
    }
    return QString(R"({
    "variants": [
        %1
    ]
}
)").arg(strArrVariants);
}

Drawer::Drawer()
{
}

QPixmap Drawer::pixmap() const
{
    return _pixmap;
}

bool Drawer::draw(Source &p_source, const char **p_error)
{
    int
        x = 0,
        h = 0
    ;

    QList<QPixmap> fragments;
    for (auto &oVariant : p_source.variants)
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
    _pixmap.fill(Qt::transparent);
    QPainter painter(&_pixmap);
    x = 0;
    for (auto oFragment : fragments)
    {
        painter.drawPixmap(x, 0, oFragment);

        x += oFragment.width();
    }
    return true;
}

bool Drawer::_drawVariant(Source::SourceVariant &p_source, QPixmap &p_result, const char **p_error)
{
    int shift = 0, h = 0, w = 0;
    QList<QPixmap> fragments;
    for (auto oLay : p_source.layouts)
    {
        QPixmap fragment;
        if (!_drawVariantLay(p_source, fragment, oLay, p_error))
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

bool Drawer::_drawVariantLay(Source::SourceVariant &p_source, QPixmap &p_result, const QString &layoutId, const char **p_error)
{
//    if (p_source.width < 256)
//        E(too little width set (minimum 256));
//    if (p_source.height < 256)
//        E(too little height set (minimum 256));

    p_result = QPixmap(p_source.width, p_source.height);
    QColor bgColor(Qt::black);
    if (layoutId == "ru")
    {
        bgColor = QColor(0x66,0,0);
    }
    else if (layoutId == "en")
    {
        bgColor = QColor(0, 0x22, 0x44);
    }
    p_result.fill(bgColor);

    _scaleX = double(p_source.width) / double(p_source.buttons.width);//
    _scaleY = double(p_source.height) / double(p_source.buttons.height);

    QPainter painter(&p_result);
    painter.setPen(Qt::yellow);

    int x{0},y{0};
    for (QString oRow : p_source.buttons.rows)
    {
        Source::SourceVariant::Buttons::Row parsedRow;
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
                if (ch == ' ')
                {
                    if (bnId.length())
                    {
                        parsedRow.height = rowHeight;
                        parsedRow.buttons.append({bnWidth, bnId});
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
                if (ch >= '0' && ch <= '9')
                {
                    bnWidth = 10 * bnWidth + static_cast<int>(ch - '0');
                }
                else if (ch == ' ')
                {
                    parsedRow.height = rowHeight;
                    parsedRow.buttons.append({bnWidth, bnId});
                    _drawButton(painter, x, y, bnWidth, rowHeight, bnId);
                    x += bnWidth;
                    bnId.clear();
                    state = sBnId;
                }
                else
                {
                    E(incorrect button width format);
                }
            }
        }
        if (state == sEmergency)
        {
            E(incorrect p_source data [1]);
        }
        else if (bnWidth)
        {
            parsedRow.height = rowHeight;
            parsedRow.buttons.append({bnWidth, bnId});
            _drawButton(painter, x, y, bnWidth, rowHeight, bnId);
        }
        if (p_source.buttons.width != (x + bnWidth))
        {
            E(Incorrect width for row)
        }
        if (layoutId == p_source.layouts[0])
        {
            p_source.buttons.parsedRows.append(parsedRow);
        }
        y += rowHeight;
    }
    if (p_source.buttons.height != y)
    {
        E(Incorrect height fow rows)
    }

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
    p_painter.drawRect(QRectF(x,y,w,h));

    p_painter.drawText(QRect(x,y,w,h), p_name, QTextOption(Qt::AlignCenter));
}



























