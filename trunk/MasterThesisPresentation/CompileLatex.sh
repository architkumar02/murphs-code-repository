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
			aspell -t f
		done
	else
		latex --interaction=batchmode $1
		bibtex $1
		latex --interaction=batchmode $1
		latex --interaction=batchmode $1
		dvipdf $1

		# Clean up
		rm *.bbl *.blg *.dvi *.toc *.nav *.log *.aux *.snm
	fi
fi
