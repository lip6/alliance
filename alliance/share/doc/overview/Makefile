
MYFILE=overview

ps   : $(MYFILE).tex
	latex $(MYFILE).tex
	dvips $(MYFILE).dvi -o $(MYFILE).ps
	cp -f $(MYFILE).ps ..

view :        
	gv $(MYFILE).ps

clean :
	rm -f $(MYFILE).ps *.log *.dvi *.aux
