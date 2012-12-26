
#!! c++ (clang) is not working with
#   ptime p;
#   cin >> p;
# this seems to be a bug of clang,
# according to boost web site.

# gcc works fine.
#CPP = c++ -std=c++11 -stdlib=libc++
CPP = g++-mp-4.7 -std=c++11
INCLUDES = -I../cpplib -I/opt/local/includes
LIBS = -L/opt/local/lib -lboost_serialization -lboost_date_time

OBJS = card.o cardlist.o commandline_tango.o

tango: $(OBJS)
	$(CPP) -o tango $(LIBS) $(OBJS)

card.o: card.cpp card.hh
cardlist.o: cardlist.cpp card.hh cardlist.hh orderedList.hpp
commandline_tango.o: commandline_tango.cpp cardlist.hh ../cpplib/file_manager.hpp

.cpp.o:
	$(CPP) $(INCLUDES) -c $<
