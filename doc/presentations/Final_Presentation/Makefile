# pdflatex options
PDFLO=-halt-on-error -file-line-error -shell-escape

TEXINPUTS=.::
#TEXINPUTS=.:./linehighlight:./pdfpc-notes::

slides:	init
	TEXINPUTS=$(TEXINPUTS) pdflatex $(PDFLO) -output-directory=out/ finalPresentation.tex
#	cd out && bibtex presentation
#	TEXINPUTS=$(TEXINPUTS) pdflatex $(PDFLO) -output-directory=out/ finalPresentation.tex
	TEXINPUTS=$(TEXINPUTS) pdflatex $(PDFLO) -output-directory=out/ finalPresentation.tex
	sed -i "s/\\\\\\\\/\n/g" out/finalPresentation.pdfpc
	sed -i "s/\\\\par/\n\n/g" out/finalPresentation.pdfpc

init:
	mkdir -p ./out/

clean:
	rm -rf ./out

show:
	evince out/finalPresentation.pdf &

start:
	pdf-presenter-console -b -w out/finalPresentation.pdf

present: slides
	pdf-presenter-console -b -u 75 out/finalPresentation.pdf
