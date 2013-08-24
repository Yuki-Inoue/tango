
#!! c++ (clang) is not working with
#   ptime p;
#   cin >> p;
# this seems to be a bug of clang,
# according to boost web site.

# gcc works fine.
#CPP = c++ -std=c++11 -stdlib=libc++

# variables that consider implicit rules
CXX = c++ -std=c++11 -stdlib=libc++
CXXFLAGS = -I../cpplib -I/opt/local/include
LDLIBS = -L/opt/local/lib -lboost_serialization-mt -lboost_date_time-mt -lreadline


OBJS = card.o cardlist.o commandline_tango.o

tango: $(OBJS)
	$(CXX) -o tango $(OBJS) $(LDLIBS)

card.o cardlist.o commandline_tango.o : card.hh
cardlist.o commandline_tango.o : cardlist.hh orderedList.hpp
commandline_tango.o: ../cpplib/file_manager.hpp ../cpplib/readstring.hpp ../cpplib/commandmap.hpp

.PHONY: clean

clean:
	rm -f *.o tango
