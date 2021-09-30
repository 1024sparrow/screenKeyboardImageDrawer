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
			puts("Program 10930_screenkeyboard_image_drawer");
			return 0;
		}
		else if (arg[0] == '-')
		{
			printf("Unknown key \"%s\". See help.\n", arg);
			return 1;
		}
	}

    Drawer::Source drawerSource{
        256,
        256,
        {
            20,
            7,
            QStringList{
                "2:",
                "2:",
                "3: Ctrl:4 Win:4 Alt:4 Space:8"
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

    QLabel label;
    label.setPixmap(drawer.pixmap());
    label.setFixedSize(label.sizeHint());
    label.show();

	return app.exec();
}
























