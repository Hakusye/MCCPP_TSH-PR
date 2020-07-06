CXX := g++
CFLAGS := -std=c++17
OBJS = obj/main.o obj/TSHwithPR.o obj/MCCPP.o obj/FullSearch.o

main: $(OBJS)
	$(CXX) $(CFLAGS) -o run $(OBJS)

obj/main.o: tests/main.cpp
	$(CXX) $(CFLAGS) -c tests/main.cpp -o obj/main.o

obj/TSHwithPR.o: src/TSHwithPR.cpp
	$(CXX) $(CFLAGS) -c src/TSHwithPR.cpp -o obj/TSHwithPR.o

obj/MCCPP.o: src/MCCPP.cpp
	$(CXX) $(CFLAGS) -c src/MCCPP.cpp -o obj/MCCPP.o

obj/FullSearch.o: src/FullSearch.cpp
	$(CXX) $(CFLAGS) -c src/FullSearch.cpp -o obj/FullSearch.o

clean:
	rm -f run $(OBJS)
