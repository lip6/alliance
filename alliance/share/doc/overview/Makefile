
MYFILE=overview

all : $(MYFILE).tex
	latex $(MYFILE).tex
	dvips $(MYFILE).dvi -o $(MYFILE).ps
view :        
	gs $(MYFILE).ps

clean :
	rm -f $(MYFILE).ps *.log *.dvi *.aux
