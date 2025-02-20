#!/bin/sh
export project_path="$PWD"

export src_path="$project_path/src"

export build_path="$project_path/build"

export bin_path="$project_path/bin"
export tmp_path="$bin_path/tmp"

export TARGET="datastruct"
make -C $src_path/$1 -f zMakefile.mak $2 --no-print-directory