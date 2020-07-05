CXX := g++
CFLAGS := -std=c++17
OBJS = obj/main.o obj/TSHwithPR.o

main: obj/main.o obj/TSHwithPR.o
	$(CXX) $(CFLAGS) -o run $(OBJS)

obj/main.o: tests/main.cpp
	$(CXX) $(CFLAGS) -c tests/main.cpp -o obj/main.o

obj/TSHwithPR.o: src/TSHwithPR.cpp
	$(CXX) $(CFLAGS) -c src/TSHwithPR.cpp -o obj/TSHwithPR.o

clean:
	rm -f run $(OBJS)
