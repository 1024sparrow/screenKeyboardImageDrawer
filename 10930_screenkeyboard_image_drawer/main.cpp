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
    10930_screenkeyboard_image_drawer --generateBareDescriptor [--help]
    10930_screenkeyboard_image_drawer [--gui] [--generate-sprite <path-to-file>] [--generate-sprite-builder <path-to-directory>] <sourceFile> [--help]

OPTIONS:

--help
    print this message and close immediatelly

--gui
    if set then result in window will be shown

--generate-sprite <path-to-file>
    result sprite will be written as sprite

SOURCE FILE FORMAT:
Source file is a JSON-file with content like following:
 {
     "variants": [
         {
             "orientation": "v",
             "layouts": ["ru","en"],
             "width": 512,
             "height": 300,
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
             "orientation": "v",
             "layouts": ["ru","en"],
             "width": 256,
             "height": 300,
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
     ]
 }
Variant button rows description:
Example: "3: Ctrl:4 Win Alt:4 Space:8 Ctrl:4".
          ┬  ──┬─ ┬ ─┬─
          └ row height (3)
               └ Button identifier (Ctrl)
                  └ Button width (4)
                     └ Button identifier (Win). The button width not set, so it's width will be taken as for previous button, i.e. equal 4.
Buttons separated with ' '(space symbol(s))
For each button may be set button width - after ':'-symbol. For first button in row it is required to be set button width.
So we have row with height 3 and 5 buttons with the following widths: 4 for Ctrl, 4 for Win (as previous by default), 4 for Alt, 8 for Space, 4 for Ctrl

Variant width - width in pixels
Variant.buttons width - width in internal units. The same units used for row heights and buttons widths.
)");
			return 0;
		}
        else if (!strcmp(arg,"--generate-sprite")||!strcmp(arg,"--gui")||!strcmp(arg,"--generateBareDescriptor"))
            ;
		else if (arg[0] == '-')
		{
			printf("Unknown key \"%s\". See help.\n", arg);
			return 1;
		}
    }

    bool argGui = false;
    const char *argSourceFilePath = nullptr;
    const char *argResultFilePath = nullptr;
    enum
    {
        sInitial,
        sSprite // result file path
    } state {sInitial};
    for (int iArg = 1 ; iArg < argc ; ++iArg)
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
            else if (arg[0] == '-')
            {
                fprintf(stderr, "unknown key \"%s\" passed\n", arg);
                return 1;
            }
            else
            {
                if (argSourceFilePath)
                {
                    fputs("only one non-key argument expected\n", stderr);
                    return 1;
                }
                argSourceFilePath = arg;
            }
        }
        else if (state == sSprite)
        {
            argResultFilePath = arg;
            state = sInitial;
        }
    }
    if (state != sInitial)
    {
        fputs("argument set incomplete. See help for details.\n", stderr);
        return 1;
    }

    if (!argSourceFilePath)
    {
        fputs("source file path not set\n", stderr);
        return 1;
    }
    const char *error = 0;
    Drawer::Source drawerSource;
    if (!Drawer::sourceFromFile(argSourceFilePath, drawerSource, &error))
    {
        fputs(error, stderr);
        return 1;
    }

    QApplication app(argc, argv);
    Drawer drawer;
    if (!drawer.draw(drawerSource, &error))
    {
        fputs(error, stderr);
        return 1;
    }

    if (argResultFilePath)
    {
        if (!drawer.pixmap().save(argResultFilePath))
        {
            fputs("can not save result sprite\n", stderr);
            return 1;
        }
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
























