CC = g++
ODIR = obj
PROG = main
CXXFLAGS = -std=c++11

OBJS = $(ODIR)/main.o $(ODIR)/matrix.o
$(PROG) : $(ODIR) $(OBJS)
	$(CC) -o $@ $(OBJS) $(CXXFLAGS)

$(ODIR)/main.o : ./src/main.cpp ./include/matrix.h ./include/environment.h
	$(CC) -c ./src/main.cpp -o $@ $(CXXFLAGS)

$(ODIR)/matrix.o : ./src/matrix.cpp ./include/matrix.h
	$(CC) -c ./src/matrix.cpp -o $@ $(CXXFLAGS)

$(ODIR) :
	if [ ! -d $(ODIR) ]; then mkdir $(ODIR); fi

.PHONY : clean
clean :
	if [ -d $(ODIR) ]; then rm $(ODIR) -r; fi
	if [ -f $(PROG) ]; then rm $(PROG); fi
