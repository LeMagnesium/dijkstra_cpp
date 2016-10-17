STDOPTS=--std=c++14

all: wptfc
	mkdir -p bin/
	mkdir -p obj/

	$(CXX) ${STDOPTS} -c main.cpp -o ./obj/main.o
	$(CXX) ${STDOPTS} -c wptfc/waypoint.cpp -o obj/waypoint.o
	$(CXX) ${STDOPTS} -c wptfc/binutils.cpp -o obj/binutils.o
	$(CXX) ${STDOPTS} -c dijkstra.cpp -o obj/dijkstra.o
	$(CXX) ${STDOPTS} obj/*.o -o bin/dijkstra

wptfc:
	make -C wptfc

clean: all
	rm -r bin/
	rm -r obj/ $(find -type f -name '*.wptc')
	make -C wptfc clean
