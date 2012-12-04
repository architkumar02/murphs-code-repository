#!/usr/local/bin/bash
WORKDIR="$(cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
LIBSVMDIR=$WORKDIR/libsvm-3.14
DATADIR=$WORKDIR/data
echo "Work Directory   : " $WORKDIR
echo "LIBSVM Directory : " $LIBSVMDIR
echo "DATA Directory   : " $DATADIR
export DATADIR=$DATADIR
export LIBSVMDIR=$LIBSVMDIR
# The following line will go add all subdirectories
# PATHAPPEND=$(find $WORKDIR/libsvm-3.14 -maxdepth 1 -type d | ( while read f ; do echo -n "${f}:" ; done))
echo $PATHAPPEND
#export PATH=$PATH:$PATHAPPEND
