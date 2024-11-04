#!/bin/bash

cd $(dirname $0)

docopts=../build/docopts
helpTxt=demo_help.txt
tmpSh=tmp.sh 

function help
{
    rm -f $tmpSh
    echo "Usage: $(basename $0) [options] args"
    cat $helpTxt
    exit 0
}

if [ ! -f $docopts ]; then
    echo "docopts binary not found. Please build it first."
    exit 1
fi

$docopts $helpTxt $@ > $tmpSh || help
source $tmpSh
rm -f $tmpSh

if [[ $_h -eq 1 ]]; then help; fi    

echo options: -c: $_c -h: $_h -t: $_t -o: $_o
echo args: $_args
for arg in $_args; do
    echo "- $arg"
done
