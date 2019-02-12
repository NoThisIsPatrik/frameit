frameprocessor: cFrameprocessor.o mkmedians.o
	g++ mkmedians.o cFrameprocessor.o -lpqxx -ljpeg -oframeprocessor 

cFrameprocessor.o:
	g++ -c cFrameprocessor.cpp -lpqxx

mkmedians.o:
	g++ -c mkmedians.cpp -ljpeg

clean:
	rm -f *.o frameprocessor
