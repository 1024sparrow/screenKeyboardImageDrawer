# 10930_screenkeyboard_image_drawer

## DESCRIPTION

```

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

```

## USAGE EXAMPLE

## LICENCE
Licensed by [MIT license](LICENSE).

