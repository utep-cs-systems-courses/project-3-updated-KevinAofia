all:
	(cd timerLib; make install)  #compile/install our timerLib folder
	(cd lcdLib; make install)    #compile/install our lcdLib folder
	(cd shapeLib; make install)  #compile/install our shapeLib folder
	(cd circleLib; make install) #compile/install our circleLib folder
	(cd p2swLib; make install)   #compile/install our switches folder
	(cd shape-motion-demo; make)
	(cd project_main; make)      #compile our main folder
	(cd lab2 && make)

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
	(cd lab2 && make clean)

	rm -rf lib h
	rm -rf doxygen_docs/*
