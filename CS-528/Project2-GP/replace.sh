#!/bin/bash
cat $1 | sed -e 's#//\(.*\)#/*\1 */#' >$1.tmp
mv $1.tmp $1
