# docopts

## the problem
-  in Win cmd there is no options handling att all
-  in linux there is a native handling but could be easier

## the principle
-  parse options text file
-  evaluate CLI arguments
-  generate temporary cmd / sh content

## rapid development
- write options text which can also be used as help output
- add options just with a new line in text

### regexes
value option
```
^ *-[a-zA-Z] +<\w+
```
simple switch
```
^ *-[a-zA-Z] +
```

## samples
### sample options.txt
```
options:
    -c  generate coverage
    -o  <file> output to file
    -t  write todo
    -H  create HTML
    -h  this help
```
### sample call windows
```
SETLOCAL
docopts.exe options.txt %* > tmp.cmd
call tmp.cmd
```
with arguments _-Hto wumpel.dat file1 file2_

tmp.cmd
```
set _Hu=1==1
set _c=0==1
set _h=0==1
set _t=1==1
set _o=wumpel.dat
set _args=file1 file2
```
Since win cmd variables are not case sensitive upper case options are indicated by **u**.

### sample call linux
```
docopts options.txt $* > tmp.sh
source tmp.sh
```
with arguments _-Hto wumpel.dat file1 file2_

tmp.sh
```
export _H=true
export _c=false
export _h=false
export _t=true
export _o=wumpel.dat
export _args="file1 file2 "
```

### sample scripts
for sample scripts see [shell folder](shell)
