#!/bin/bash

TARGET=build/10930_screenkeyboard_image_drawer/10930_screenkeyboard_image_drawer

pushd $(dirname $0)

    echo "# $(basename $TARGET)

## DESCRIPTION
" > README.md
    echo '```' >> README.md
    $TARGET --help >> README.md
    echo '```

## USAGE EXAMPLE
For input file
```json' >> README.md
cat test.json >> README.md
echo '```
we taken the following result:
![test_result.png](test_result.png)
' >> README.md

echo '
## LICENCE
Licensed by [MIT license](LICENSE).
' >> README.md

popd
