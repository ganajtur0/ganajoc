#!/bin/bash

source $(dirname "$0")/aoc_cookie.sh

dir=$(ls | grep '^d' | sort -V | tail -1)

tmp=${dir:3:2}

tmp=$((tmp+1))

newdir="day$tmp"

mkdir $newdir

cd $newdir

mkdir exe

touch test

touch "$newdir.c"

printf "#include <stdio.h>\n#include <stdlib.h>\n\nint\nmain(int argc, char *argv[]){\n\n\treturn 0;\n}" > "$newdir.c"

touch Makefile

printf "main: $newdir.c\n\tcc -o ./exe/main $newdir.c && ./exe/main\ntest: $newdir.c\n\tcc -DDEBUG -o ./exe/test $newdir.c && ./exe/test\ndebug: $newdir.c\n\tcc -ggdb -DDEBUG -o ./exe/debug && gdb ./exe/debug" > Makefile

touch i
curl --cookie "session=$COOKIE" "$(echo `date +https://adventofcode.com/%Y/day/%d/input` | sed 's/\/0/\//g')" > i

