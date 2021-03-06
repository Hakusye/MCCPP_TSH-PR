CXX := g++
CFLAGS := -std=c++17
OBJS = obj/main.o obj/TSHwithPR.o obj/MCCPP.o obj/FullSearch.o obj/util.o obj/AnalizerForTSH.o obj/BeamSearch.o obj/DAG.o

main: $(OBJS)
	$(CXX) $(CFLAGS) -o run $(OBJS)

obj/main.o: tests/main.cpp
	$(CXX) $(CFLAGS) -c tests/main.cpp -o obj/main.o

obj/AnalizerForTSH.o: src/AnalizerForTSH.cpp
	$(CXX) $(CFLAGS) -c src/AnalizerForTSH.cpp -o obj/AnalizerForTSH.o

obj/BeamSearch.o: src/BeamSearch.cpp
	$(CXX) $(CFLAGS) -c src/BeamSearch.cpp -o obj/BeamSearch.o

obj/DAG.o : src/DAG.cpp
	$(CXX) $(CFLAGS) -c src/DAG.cpp -o obj/DAG.o

obj/TSHwithPR.o: src/TSHwithPR.cpp
	$(CXX) $(CFLAGS) -c src/TSHwithPR.cpp -o obj/TSHwithPR.o

obj/FullSearch.o: src/FullSearch.cpp
	$(CXX) $(CFLAGS) -c src/FullSearch.cpp -o obj/FullSearch.o

obj/MCCPP.o: src/MCCPP.cpp
	$(CXX) $(CFLAGS) -c src/MCCPP.cpp -o obj/MCCPP.o

obj/util.o: src/util.cpp
	$(CXX) $(CFLAGS) -c src/util.cpp -o obj/util.o


clean:
	rm -f run $(OBJS) 
