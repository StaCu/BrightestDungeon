#!/bin/sh
# This copies every file that is tracked by git to the path specified by the first argument.

if [ -z "$1" ]; then
    echo "USAGE: ./sdcard <path>"
    exit
fi

PATH_DST=$1

echo "copy to: " $PATH_DST

git ls-tree --full-tree -r --name-only HEAD | while read f; do
    FILE_NAME="$PATH_DST"/"$f"
    DIR_NAME=$(dirname "$FILE_NAME")
    mkdir -p "$DIR_NAME"
    echo $f " -> " $FILE_NAME
    cp "$f" "$PATH_DST/$f"
done

