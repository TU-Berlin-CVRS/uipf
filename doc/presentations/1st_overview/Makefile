# pdflatex options
PDFLO=-halt-on-error -file-line-error -shell-escape

TEXINPUTS=.::
#TEXINPUTS=.:./linehighlight:./pdfpc-notes::

slides:	init
	TEXINPUTS=$(TEXINPUTS) pdflatex $(PDFLO) -output-directory=out/ presentation.tex
#	cd out && bibtex presentation
#	TEXINPUTS=$(TEXINPUTS) pdflatex $(PDFLO) -output-directory=out/ presentation.tex
	TEXINPUTS=$(TEXINPUTS) pdflatex $(PDFLO) -output-directory=out/ presentation.tex
	sed -i "s/\\\\\\\\/\n/g" out/presentation.pdfpc
	sed -i "s/\\\\par/\n\n/g" out/presentation.pdfpc

init:
	mkdir -p ./out/

clean:
	rm -rf ./out

show:
	evince out/presentation.pdf &

start:
	pdf-presenter-console -b -w out/presentation.pdf

present: slides
	pdf-presenter-console -b -u 75 out/presentation.pdf
