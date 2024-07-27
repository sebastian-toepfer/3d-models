#!/bin/sh

BASE_OPENSCAD_CMD="openscad --hardwarnings -D \$fn=128"
mkdir -p generated/new_stls

make all
for file in $(find ./designs -type f -name *.scad); do
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
                $BASE_OPENSCAD_CMD -p $parametersFileName -P $configuration $file -o $generatedFileName
                retVal=$?
                if [ $retVal -ne 0 ];
                then
                    exit $retVal
                else
                    cp $generatedFileName generated/new_stls/
                fi
            fi
        done
    fi
done

