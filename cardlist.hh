#ifndef YUKI_INOUE_TANGO_CARDLIST_HH
#define YUKI_INOUE_TANGO_CARDLIST_HH

#include "card.hh"
#include "orderedList.hpp"

#include <list>

#include <boost/serialization/list.hpp>


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
  void test();
  void add(const Card &card){ mystd::addWithOrder(l_, card); }



  // conditions
public:
  std::list<Card>::size_type size() const { return l_.size(); }
  boost::posix_time::ptime nexptime() const { return l_.front().getNexptime(); }



  // serialization
private:
  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive &ar, const unsigned int){ ar & l_; }

};


#endif
