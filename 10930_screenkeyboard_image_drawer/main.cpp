#include <QApplication>
#include <string.h>
#include <stdio.h>

#include <QLabel>

#include "Drawer.h"

int main(int argc, char **argv)
{
	for (int iArg = 0 ; iArg < argc ; ++iArg)
	{
		char *arg = argv[iArg];
		if (!strcmp(arg, "--help"))
		{
			puts(R"(
Program 10930_screenkeyboard_image_drawer

SYNOPSIS:
    10930_screenkeyboard_image_drawer [--gui] [--generate-sprite <path-to-file>] [--generate-sprite-builder <path-to-directory>] <sourceFile> [--help]

OPTIONS:

--help
    print this message and close immediatelly

--gui
    if set then result in window will be shown

--generate-sprite <path-to-file>
    result sprite will be written as sprite

--generate-sprite-builder
)");
			return 0;
		}
        else if (!strcmp(arg,"--generate-sprite")||!strcmp(arg,"--generate-sprite-builder")||!strcmp(arg,"--gui"))
            ;
		else if (arg[0] == '-')
		{
			printf("Unknown key \"%s\". See help.\n", arg);
			return 1;
		}
	}

    bool argGui = false;
    enum
    {
        sInitial,
        sSprite,
        sSpriteBuilder
    } state {sInitial};
    for (int iArg = 0 ; iArg < argc ; ++iArg)
    {
        char *arg = argv[iArg];
        if (state == sInitial)
        {
            if (!strcmp(arg, "--gui"))
            {
                argGui = true;
            }
            else if (!strcmp(arg, "--generate-sprite"))
            {
                state = sSprite;
            }
            else if (!strcmp(arg, "--generate-sprite-builder"))
            {
                state = sSpriteBuilder;
            }
        }
    }

    Drawer::Source drawerSource{
        1024,
        600,
        {
            24,
            10,
            QStringList{
                "2: Ctrl:4 Win:16 Ctrl:4",
                "2: Ctrl:4 Win:20",
                "3: Ctrl:4 Win:8 Space:8 Ctrl:4",
                "3: Ctrl:4 Win Alt:4 Space:8 Ctrl:4"
            }
        }
    };

    QApplication app(argc, argv);
    Drawer drawer;
    const char *error = 0;
    if (!drawer.draw(drawerSource, &error))
    {
        fputs(error, stderr);
        return 1;
    }

    //return 0;//
    if (argGui)
    {
        QLabel label;
        label.setPixmap(drawer.pixmap());
        label.setFixedSize(label.sizeHint());
        label.show();
        return app.exec();
    }

	return 0;
}
























