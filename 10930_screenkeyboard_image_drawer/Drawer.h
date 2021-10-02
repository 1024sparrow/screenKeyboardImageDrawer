#pragma once

#include <QPixmap>
#include <QStringList>

class Drawer final
{
public:
    struct Source
    {
        struct SourceVariant
        {
            enum {
                orientVertical,
                orientHorizontal
            } orientation;
            QStringList layouts;
            int width; // pixels of result pixmap
            int height;
            struct Buttons
            {
                int width; // logic units of layout
                int height;
                QStringList rows;
            } buttons;

            //const char *
        };
        QList<SourceVariant> variants;
    };
    static bool sourceFromFile(const char *p_filePath, Source &p_retVal, const char **p_error);

    Drawer();
    QPixmap pixmap() const;
    bool draw(const Source &source, const char **error);
private:
    bool _drawVariant(const Source::SourceVariant &source, const char **error);
    void _drawButton(QPainter &p_painter, int p_x, int p_y, int p_w, int p_h, const QString &p_name);

    QPixmap _pixmap;
    double _scaleX=10, _scaleY=10;
};
