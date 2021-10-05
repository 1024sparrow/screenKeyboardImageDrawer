/*
 * SOURCE FILE FORMAT:
    Source file is a JSON-file with content like following:
    {
        "width": 1024,
        "height": 600,
        "buttons": {
            "width": 24,
            "height": 10,
            "rows": [
                "2: Ctrl:4 Win:16 Ctrl:4",
                "2: Ctrl:4 Win:20",
                "3: Ctrl:4 Win:8 Space:8 Ctrl:4",
                "3: Ctrl:4 Win Alt:4 Space:8 Ctrl:4"
            ]
        }
    }

    or like wollowing:

    {
        variants: [
            {
                "orient": "v",
                "layouts": ["ru","en"],
                "width": 541,
                "height": 143,
                "buttons": {
                    "width": 24,
                    "height": 10,
                    "rows": [
                        "2: Ctrl:4 Win:16 Ctrl:4",
                        "2: Ctrl:4 Win:20",
                        "3: Ctrl:4 Win:8 Space:8 Ctrl:4",
                        "3: Ctrl:4 Win Alt:4 Space:8 Ctrl:4"
                    ]
                }
            },
            {
                "orient": "h",
                "layouts": ["ru", "en"],
                "width": 541,
                "height": 541,
                "buttons": {
                    "width": 2,
                    "height": 2,
                    "rows": [
                        "1: 1:1 2",
                        "1: 3:1"
                    ]
                }
            }
        ]
    }

 */

#include "Drawer.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>//

#define E(T) {*p_error=#T"\n";return false;}

bool Drawer::sourceFromFile(const char *p_filePath, Source &p_retVal, const char **p_error)
{
//    p_retVal = Drawer::Source{
//        {
//            {
//                Drawer::Source::SourceVariant::orientVertical,
//                {"ru","en"},
//                512,
//                300,
//                {
//                    24,
//                    10,
//                    QStringList{
//                        "2: Ctrl:4 Win:16 Ctrl:4",
//                        "2: Ctrl:4 Win:20",
//                        "3: Ctrl:4 Win:8 Space:8 Ctrl:4",
//                        "3: Ctrl:4 Win Alt:4 Space:8 Ctrl:4"
//                    }
//                }
//            },
//            {
//                Drawer::Source::SourceVariant::orientVertical,
//                {"ru","en"},
//                256,
//                300,
//                {
//                    24,
//                    10,
//                    QStringList{
//                        "2: Ctrl:4 Win:16 Ctrl:4",
//                        "2: Ctrl:4 Win:20",
//                        "3: Ctrl:4 Win:8 Space:8 Ctrl:4",
//                        "3: Ctrl:4 Win Alt:4 Space:8 Ctrl:4"
//                    }
//                }
//            }
//        }
//    };
//    return true;

    QFile file(p_filePath);
    if (!file.open(QIODevice::ReadOnly))
        E(can not open source file with layout description);

    QJsonParseError jsonError;
    auto doc = QJsonDocument::fromJson(file.readAll(), &jsonError);
    if (jsonError.error != QJsonParseError::NoError)
    {
        qDebug() << jsonError.errorString();
        E(incorrect JSON. Source file is malformed.);
    }

    if (!doc.isObject())
        E(E1);
    auto eRoot = doc.object();
    auto eVariants = eRoot["variants"];
    if (!eVariants.isArray())
        E(E2);
    for (auto eVariant : eVariants.toArray())
    {
        if (!eVariant.isObject())
            E(E3);
        Source::SourceVariant targetVariant;
        auto var = eVariant.toObject();
        {
            auto eOrientation = var["orientation"];
            if (eOrientation == "v")
            {
                targetVariant.orientation = Source::SourceVariant::orientVertical;
            }
            else if (eOrientation == "h")
            {
                targetVariant.orientation = Source::SourceVariant::orientHorizontal;
            }
            else
            {
                E(E4);
            }
        }
        {
            auto eLayouts = var["layouts"];
            if (!eLayouts.isArray())
                E(E5);
            auto layouts = eLayouts.toArray();
            if (layouts.size() == 0)
                E(layouts: empty list is not accepted)
            for (auto oLay : layouts)
            {
                if (!oLay.isString())
                    E(E6);
                targetVariant.layouts.append(oLay.toString());
            }
        }
        {
            if (!(targetVariant.width = var["width"].toInt(0)))
            {
                E(width: must be set and be greater then zero);
            }
            if (!(targetVariant.height = var["height"].toInt(0)))
            {
                E(height: must be set and be greater then zero);
            }
        }
        {
            auto eButtons = var["buttons"];
            if (!eButtons.isObject())
            {
                E(buttons: must be set and set to no empty);
            }
            auto oButtons = eButtons.toObject();
            if (!(targetVariant.buttons.width = oButtons["width"].toInt(0)))
            {
                E(buttons: width: must be set and be greater then zero);
            }
            if (!(targetVariant.buttons.height = oButtons["height"].toInt(0)))
            {
                E(buttons: height: must be set and be greater then zero);
            }
            auto eRows = oButtons["rows"];
            if (!eRows.isArray())
                E(E7);
            for (auto oRow : eRows.toArray())
            {
                if (!oRow.isString())
                    E(E8);
                targetVariant.buttons.rows.append(oRow.toString());
            }
        }
        p_retVal.variants.append(targetVariant);
    }

    //E(not implemented);
    return true;
}









































