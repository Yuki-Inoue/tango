#ifndef YUKI_INOUE_COMMAND_MAP_HPP
#define YUKI_INOUE_COMMAND_MAP_HPP

#include <map>
#include <string>
#include <iostream>

#include "readstring.hpp"

template <class CommandPtr>
class CommandMap {
  typedef std::map<std::string, CommandPtr> Map;

protected:
  Map map_;

public:
  CommandPtr query(const std::string prompt = "> ") const {
    using namespace std;
    CommandPtr result;
    while(true){
      string str = readstring(prompt);
      typename Map::const_iterator it = map_.find(str);
      if(it != map_.end()) {
	result = it->second;
	break;
      }
      cout << "error! valid commands are: " << *this << endl;
    }
    return result;
  }

  friend std::ostream &operator<<(std::ostream &os, const CommandMap &cmap){
    os << '(';
    typename Map::const_iterator it = cmap.map_.begin(), itend = cmap.map_.end();
    if( itend == it ) {
      os << ')';
      return os;
    }
    --itend;
    for(; it!=itend; ++it){
      os << it->first << '/';
    }
    os << it->first << ')';
    return os;
  }
};


#endif
