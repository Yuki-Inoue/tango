#include "cardlist.hh"

#include <boost/foreach.hpp>
#define foreach BOOST_FOREACH

using namespace std;
using namespace boost::posix_time;


void Cardlist::test(){
  list<Card>::iterator it = l_.begin(), itend = l_.end(), ittemp;
  list<Card> updated;
  bool finish_flag = false;
  while( !finish_flag && it!=itend && it->expired() ){
    switch(query(*it)) {
    case CardTest::CONTINUE:
      ittemp = it++;
      updated.splice
	(std::lower_bound
	 (updated.begin(), updated.end(), *ittemp), l_, ittemp);
      break;
    case CardTest::DELETE:
      l_.erase(it++);
      break;
    case CardTest::QUIT:
      finish_flag = true;
      break;
    case CardTest::RETRY:
      break;
    }
  }
  l_.merge(updated);
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
