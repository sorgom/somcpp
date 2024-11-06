#!/bin/bash

cd $(dirname $0)

docopts=../build/docopts
optsTxt=demo_options.txt
tmpSh=../build/tmp.sh

if [ ! -f $docopts ]; then
    echo "$docopts not found. Please build it first."
    exit 1
fi

function help
{
    rm -f $tmpSh
    echo "Usage: $(basename $0) [options] args"
    cat $optsTxt
    exit 0
}

$docopts $optsTxt $@ > $tmpSh || exit 1
source $tmpSh

if $_h; then help; fi

echo options: -c: $_c -t: $_t -o: $_o
echo args: $_args
