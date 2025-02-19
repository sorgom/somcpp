#!/bin/bash

callDir=$(pwd)

cd $(dirname $0)
optsTxt=$(pwd)/demo_options.txt

cd ../build
docopts=$(pwd)/docopts
tmpSh=$(pwd)/tmp.sh

cd $callDir

if [ ! -f $docopts ]; then
    echo "$docopts not found. Please build it first."
    exit 1
fi

function help
{
    rm -f $tmpSh
    echo "usage: $(basename $0) [options] args"
    cat $optsTxt
    exit 0
}

$docopts $optsTxt $@ > $tmpSh || exit 1
source $tmpSh

if $_h; then help; fi

echo options: -c: $_c -t: $_t -H: $_H -o: $_o
echo args: $_args
