#!/bin/bash

# Compiles a latex with bibtext
if [ $# -eq 0 ]
then
	echo "Need to specifiy file name"
else
	latex --interaction=batchmode $1
	bibtex $1
	latex --interaction=batchmode $1
	latex --interaction=batchmode $1
	dvipdf $1

	# Clean up
	rm *.toc *.nav *.log *.aux *.snm
fi
