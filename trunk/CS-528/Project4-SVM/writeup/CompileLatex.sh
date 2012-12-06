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
			aspell -t check $f
		done
	else
		version=$(svnversion .)
        mkdir tmp								# For EPS to PDF files
		#options="--interaction=batchmode"
		options="--shell-escape"
		postProcess="grep Warning\|Overful"
		pdflatex $options $1 | $postProcess 
		bibtex  $1 | $postProcess
		pdflatex $options $1 | $postProcess
		echo "All refrences should now be resolved"
		echo
		pdflatex $options $1 | $postProcess
        cp $1.pdf $1_v$version.pdf
		# Clean up
		rm -f *.vrb *.tdo *.out *.bbl *.blg *.dvi *.toc *.nav *.log *.aux *.snm
		rm -rf tmp
	fi
fi
