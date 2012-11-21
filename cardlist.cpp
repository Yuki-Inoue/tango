#include "cardlist.hh"

#include <boost/foreach.hpp>
#define foreach BOOST_FOREACH

using namespace std;
using namespace boost::posix_time;




void Cardlist::search(const string &str){
  class finder {
    const string *ptr;
  public:
    finder(const string &str) : ptr(&str) {}
    bool operator()(const Card &card) {
      return card.question_.find(*ptr) != string::npos;
    }
  };
  operate(finder(str), always_true);
}


ostream &operator<<(ostream &out, const Cardlist &cl){
  foreach(const Card &card, cl.l_)
    out << card;
  return out;
}

istream &operator>>(istream &in, Cardlist &cl){
  try{
    while(true){
      Card card;
      in >> card;
      cl.l_.push_back(card);
    }
  }
  catch(Card::FailedMaking &e){
  }
  return in;
}
