
CXXFLAGS = -I ./inc
VPATH = ./src

obj = main.o dcel.o
exec = assignment1

$(exec): $(obj)
	$(CXX) $(obj) -o $@

$(obj): %.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $(CPPFLAGS) $< -o $@

clean:
	rm -f $(obj) $(exec)