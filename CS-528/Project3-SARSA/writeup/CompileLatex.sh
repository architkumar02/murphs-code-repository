#!/bin/bash

# Compiles a latex with bibtext
if [ $# -eq 0 ]
then
	echo "Need to specifiy file name or spellcheck"
else
	if [ "$1" ==  "spellcheck" ]
	then
		# Spell Check
  		for f in *.tex;
		do
			aspell -t check f
		done
	else
		version=$(svnversion .)
        latex --interaction=batchmode $1
		bibtex $1
		latex --interaction=batchmode $1
		latex --interaction=batchmode $1
		latexpdf $1
        cp $1.pdf $1_v$version.pdf
		# Clean up
		rm *.vrb *.out *.bbl *.blg *.dvi *.toc *.nav *.log *.aux *.snm
	fi
fi
