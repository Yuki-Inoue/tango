#include "cardlist.hh"

#include <boost/foreach.hpp>
#define foreach BOOST_FOREACH

using namespace std;
using namespace boost::posix_time;



list<Card>::size_type Cardlist::expnum() const{
  const list<Card>::const_iterator itend = l_.end();
  list<Card>::const_iterator it;
  const ptime current = second_clock::local_time();
  list<Card>::size_type n = 0;
  for(it=l_.begin(); it->getNexptime() < current && it!=itend; ++it)
    ++n;
  return n;
}

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
