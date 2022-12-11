#!/bin/bash

dir=$(ls | grep '^d' | sort -V | tail -1)

tmp=${dir:3:2}

tmp=$((tmp+1))

newdir="day$tmp"

mkdir $newdir

cd $newdir

mkdir exe

touch i

touch test

touch "$newdir.c"

printf "#include<stdio.h>\nint\nmain(){\n\treturn 0;\n}" > "$newdir.c"

touch Makefile

printf "main: $newdir.c\n\tcc -o ./exe/main $newdir.c && ./exe/main" > Makefile
