all:
	(cd src; make);
	(cd src-mqtt; make)
	(cd documentation; make)

package:	all
	(cd package; make package)

clean:
	(cd src; make clean)
	(cd src-mqtt; make clean)
	(cd documentation; make clean)
	(cd package; make clean)
	rm -rf *.deb opt

