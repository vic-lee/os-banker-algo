CXX = g++
CXXFLAGS = -Wall -g -std=c++11

src = $(wildcard src/*.cc) \
		$(wildcard src/ds/*.cc) \
		$(wildcard src/io/*.cc) \
		$(wildcard src/manager/*.cc) 

obj = $(src:.cc=.o)

exec: $(obj)
	$(CXX) -o $@ $^ $(CXXFLAGS)

.PHONY: clean
clean:
	rm -f $(obj) exec