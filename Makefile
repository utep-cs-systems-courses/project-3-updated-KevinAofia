all:
	(cd timerLib; make install)
	(cd lcdLib; make install)
	(cd shapeLib; make install)
	(cd circleLib; make install)

	(cd p2swLib; make install) #compile/install our switches folder

	(cd shape-motion-demo; make)

	(cd project_main; make) #compile our main folder

	(cd project3 && make)

doc:
	rm -rf doxygen_docs
	doxygen Doxyfile
clean:
	(cd timerLib; make clean)
	(cd lcdLib; make clean)
	(cd shapeLib; make clean)

	(cd p2swLib; make clean)

	(cd shape-motion-demo; make clean)
	(cd circleLib; make clean)

	(cd project_main; make clean)

	(cd project3 && make clean)

	rm -rf lib h
	rm -rf doxygen_docs/*
