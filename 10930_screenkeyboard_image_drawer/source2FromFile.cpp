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

#define E(T) {*p_error=#T"\n";return false;}

bool Drawer::sourceFromFile(const char *p_filePath, Source &p_retVal, const char **p_error)
{
    QFile file(p_filePath);
    if (!file.open(QIODevice::ReadOnly))
        E(can not open source file with layout description);

    QJsonParseError jsonError;
    auto doc = QJsonDocument::fromJson(file.readAll(), &jsonError);
    if (jsonError.error != QJsonParseError::NoError)
        E(incorrect JSON. Source file is malformed.);

    E(not implemented);
}
