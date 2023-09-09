#!/bin/sh

for file in ./designs/**/*.scad; do
    outputFileName=$(basename "$file")
    subDirs=$(dirname "$file" | sed "s/\.\/designs\///")
    mkdir -p generated/$subDirs
    openscad -q -D \$fn=100 $file -o ./generated/$subDirs/$outputFileName.stl
done

