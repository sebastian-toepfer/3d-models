#!/bin/sh

for file in ./designs/**/*.scad; do
    outputFileName=$(basename "$file")
    subDirs=$(dirname "$file" | sed "s/\.\/designs\///")
    mkdir -p generated/$subDirs
    parametersFileName=designs/$subDirs/$outputFileName.json
    if [ -f $parametersFileName ];
    then
        configurations=$(jq -r ".parameterSets | to_entries[] | .key" $parametersFileName)
        for configuration in $configurations
        do
            generatedFileName=./generated/$subDirs/$outputFileName.$configuration.stl
            if [ ! -f $generatedFileName ] || [ $file -nt $generatedFileName ] || [ $parametersFileName -nt $generatedFileName ];
            then
                echo "generate $generatedFileName"
                openscad -p $parametersFileName -P $configuration -D \$fn=100 $file -o $generatedFileName
            fi
        done
    else
        generatedFileName=./generated/$subDirs/$outputFileName.stl
        if [ ! -f $generatedFileName ] || [ $file -nt $generatedFileName ];
        then
            echo "generate $generatedFileName"
            openscad -D \$fn=100 -o ./generated/$subDirs/$outputFileName.stl $file
        fi
    fi
done

