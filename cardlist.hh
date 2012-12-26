#ifndef YUKI_INOUE_TANGO_CARDLIST_HH
#define YUKI_INOUE_TANGO_CARDLIST_HH

#include "card.hh"
#include "orderedList.hpp"

#include <list>

#include <boost/serialization/list.hpp>


inline bool always_true(const Card &){
  return true;
}

inline bool card_expired(const Card &card){
  return card.expired();
}




// CONSISTENCY: keeping the ordered list.
//              all the cards' question and answer not empty
//              except for the last_created_.
class Cardlist {
  std::list<Card> l_;


  // operators
private:
  friend std::ostream &operator<<(std::ostream &out, const Cardlist &cl);
  friend std::istream &operator>>(std::istream &cin, Cardlist &cl);


  // operations
public:
  void test(){ operate(always_true, card_expired); }
  void search(const std::string &str);
  template <class F1, class F2>
  void operate(F1 do_test, F2 do_continue);
  void add(Card &&card){ mystd::addWithOrder(l_, std::move(card)); }
  void add(const Card &card){ mystd::addWithOrder(l_, card); }



  // conditions
  // nexptime is pos_infin if empty
public:
  std::list<Card>::size_type size() const { return l_.size(); }
  boost::posix_time::ptime nexptime() const {
    using namespace boost::posix_time;
    return l_.empty() ? ptime(pos_infin) : l_.front().getNexptime();
  }
  std::list<Card>::size_type expnum() const;



  // serialization
private:
  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive &ar, const unsigned int){ ar & l_; }

};

inline bool expired(const Cardlist &l){
  using namespace boost::posix_time;
  return l.nexptime() < ptime(second_clock::local_time());
}






template <class F1, class F2>
void Cardlist::operate(F1 do_test, F2 do_continue){
  using namespace std;
  list<Card>::iterator it = l_.begin(), itend = l_.end(), ittemp;
  list<Card> updated;
  bool finish_flag = false;
  while( !finish_flag && it!=itend && do_continue(*it) ){
    if( !do_test(*it) ){
      ++it;
      continue;
    }
    switch(query(*it)) {
    case CardTest::SUCCESS:
      ittemp = it++;
      updated.splice
	(std::lower_bound
	 (updated.begin(), updated.end(), *ittemp), l_, ittemp);
      break;
    case CardTest::FAIL:
      ++it;
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


#endif
