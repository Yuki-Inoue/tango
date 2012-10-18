#ifndef YUKI_INOUE_TANGO_CARD_HH
#define YUKI_INOUE_TANGO_CARD_HH

#include <string>


#include <boost/shared_ptr.hpp>
#include <boost/operators.hpp>

#include <boost/serialization/access.hpp>
#include <boost/serialization/string.hpp>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/time_serialize.hpp>

/*
  nexptime is
  if crct == 0, last updated date
  else last updated + (2**crct hours)
  
  nexptime:
	next date this card expires.
  crct:
        # of continued correct answer

  cards are ordered in the nexptime order
 */


/*

  about file:
  file :=
	list of chunks
  chunk :=
	date \n
	count \n
	question \n
	answer \n
	\n
 */



class Card : boost::less_than_comparable<Card> {

  // Constructor
public:
  Card() :
    crct_(0), nexptime_(boost::posix_time::second_clock::local_time())
  {}

  explicit Card(const boost::posix_time::ptime &nexptime) :
    nexptime_(nexptime), crct_(0) {}

  Card(const std::string &question, const std::string &answer,
       int crct = 0,
       boost::posix_time::ptime nexptime
       = boost::posix_time::second_clock::local_time())
    : nexptime_(nexptime), crct_(crct),
      question_(question), answer_(answer)
  {}

  // operators
public:
  class FailedMaking{};
  bool operator<(const Card &card) const { return nexptime_ < card.nexptime_; }
  bool operator<(const boost::posix_time::ptime &t) const { return nexptime_ < t; }
  friend bool operator<(const boost::posix_time::ptime &t, const Card &card) { return t < card.nexptime_; }
  friend std::ostream &operator<<(std::ostream &os, const Card &card);
  friend std::istream &operator>>(std::istream &os, Card &card);


  // operations
public:
  void correctUpdate();
  void wrongUpdate();
  void anneal();



  // attributes
public:
  boost::posix_time::ptime getNexptime() const { return nexptime_; }



  // conditions
public:
  bool expired() const {
    using namespace boost::posix_time;
    return nexptime_ < ptime(second_clock::local_time());
  }

  // members
private:
  boost::posix_time::ptime nexptime_;
  int crct_;
public:
  std::string question_;
  std::string answer_;


  // serialization
private:
  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive &ar, const unsigned int){
    ar & nexptime_ & crct_ & question_ & answer_;
  }

  // internal
private:
  unsigned inchourbycrct() const { return 1U << crct_; }
};



namespace CardTest {
  enum Result { CONTINUE, QUIT, DELETE, RETRY };
  inline Result correct(Card &card) {
    card.correctUpdate();
    return CONTINUE;
  }
  inline Result wrong(Card &card) {
    card.wrongUpdate();
    return CONTINUE;
  }
  inline Result deleteCard(Card &) {
    return DELETE;
  }
  inline Result skip(Card &) {
    return CONTINUE;
  }
  inline Result quit(Card &) {
    return QUIT;
  }
}

// machine specific query
// implement this for each env.
CardTest::Result query(Card &);


#endif
