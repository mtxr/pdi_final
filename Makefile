all:
	g++ -g -o pdi_final pdi_final.cpp -I /usr/include/opencv `pkg-config opencv --libs`

run:
	./pdi_final videos/goal3.avi 0 0

test:
	valgrind ./pdi_final videos/goal3.avi 0 0
