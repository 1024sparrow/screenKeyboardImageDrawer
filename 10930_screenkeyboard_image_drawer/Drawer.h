#pragma once

#include <QPixmap>
#include <QStringList>

class Drawer
{
public:
    struct Source
    {
        int width;// pixels of result pixmap
        int height;
        struct Buttons
        {
            int width; // logic units of layout
            int height;
            QStringList rows;
        } buttons;

        //const char *
    };

    Drawer();
    QPixmap pixmap() const;
    bool draw(const Source &source, const char **error);
private:
    void _drawButton(QPainter &p_painter, int p_x, int p_y, int p_w, int p_h, const QString &p_name);

    QPixmap _pixmap;
};
