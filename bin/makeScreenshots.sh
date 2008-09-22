#!/bin/sh

for img in *.bmp 
do
    filename=`basename $img .bmp`
    echo "Make screenshot files for $img." 
    convert $img -rotate -90 $filename.png
    convert $filename.png -scale 500 help-$filename.png
    convert $filename.png -scale 100 thumb-$filename.png
done
