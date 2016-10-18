STDOPTS=-I. --std=c++14

all:
	mkdir -p obj/
	mkdir -p bin/
	$(CXX) ${STDOPTS} -c binutils.cpp -o obj/binutils.o
	$(CXX) ${STDOPTS} -c waypoint.cpp -o obj/waypoint.o
	$(CXX) ${STDOPTS} -c wptfc.cpp -o obj/wptfc.o
	$(CXX) ${STDOPTS} obj/*.o -o bin/wptfc

clean: all
	rm -r obj
	rm -r bin
