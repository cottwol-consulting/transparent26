# make file for documentation for transparent26

# assumes you have a working pandoc stack on the build machine...

PUBLISHED=\
  testing-transparent-mode.pdf

all:	${PUBLISHED}
	mkdir -p ../opt/transparent26/etc
	cp ${PUBLISHED} ../opt/transparent26/etc

clean:
	rm -f ${PUBLISHED}

testing-transparent-mode.pdf:	doc-src/testing-transparent-mode.md
	pandoc --toc -o testing-transparent-mode.pdf doc-src/testing-transparent-mode.md

