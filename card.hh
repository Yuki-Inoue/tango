#ifndef YUKI_INOUE_TANGO_CARD_HH
#define YUKI_INOUE_TANGO_CARD_HH

#include <string>


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




class NexptimeModuleBase : boost::less_than_comparable<NexptimeModuleBase> {
private:
  virtual void setCrct(unsigned crct) = 0;
  virtual unsigned getCrct() const = 0;
  virtual void setNexptime(boost::posix_time::ptime nexptime) = 0;
public:
  virtual boost::posix_time::ptime getNexptime() const = 0;

  void correctUpdate();
  void wrongUpdate();
public:
  bool expired() const {
    using namespace boost::posix_time;
    return getNexptime() < ptime(second_clock::local_time());
  }

  // operators
public:
  class FailedMaking{};
  bool operator<(const NexptimeModuleBase &card) const
  { return getNexptime() < card.getNexptime(); }
  bool operator<(const boost::posix_time::ptime &t) const
  { return getNexptime() < t; }
  friend bool operator<(const boost::posix_time::ptime &t, const NexptimeModuleBase &card)
  { return t < card.getNexptime(); }


private:
  unsigned inchourbycrct() const { return 1U << getCrct(); }
  void anneal();
};




class SimpleQABase {
public:
  virtual void setQuestion(const std::string &) = 0;
  virtual void setQuestion(std::string &&) = 0;
  virtual const std::string &getQuestion() const = 0;
  virtual void setAnswer(const std::string &) = 0;
  virtual void setAnswer(std::string &&) = 0;
  virtual const std::string &getAnswer() const = 0;
};





class SimpleCard :
  public NexptimeModuleBase,
  public SimpleQABase
{
  boost::posix_time::ptime nexptime_;
  unsigned crct_;
  std::string question_;
  std::string answer_;

  void setCrct(unsigned crct) override
  { crct_ = crct; }
  unsigned getCrct() const override
  { return crct_; }
  void setNexptime(boost::posix_time::ptime nexptime) override
  { nexptime_ = nexptime; }

public:
  boost::posix_time::ptime getNexptime() const override
  { return nexptime_; }

  void setQuestion(const std::string &question) override {
    question_ = question;
  }
  void setQuestion(std::string &&question) override {
    question_ = std::move(question);
  }
  const std::string &getQuestion() const override {
    return question_;
  }
  void setAnswer(const std::string &answer) override {
    answer_ = answer;
  }
  void setAnswer(std::string &&answer) override {
    answer_ = std::move(answer);
  }
  const std::string &getAnswer() const override {
    return answer_;
  }



  // Constructor
public:
  SimpleCard() :
    crct_(0), nexptime_(boost::posix_time::second_clock::local_time())
  {}

  explicit SimpleCard(const boost::posix_time::ptime &nexptime) :
    nexptime_(nexptime), crct_(0) {}

  SimpleCard(const std::string &question, const std::string &answer,
       unsigned crct = 0,
       boost::posix_time::ptime nexptime
       = boost::posix_time::second_clock::local_time())
    : nexptime_(nexptime), crct_(crct),
      question_(question), answer_(answer)
  {}


  // stream operators
  friend std::ostream &operator<<(std::ostream &os, const SimpleCard &card);
  friend std::istream &operator>>(std::istream &os, SimpleCard &card);


  // serialization
private:
  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive &ar, const unsigned int){
    ar & nexptime_ & crct_ & question_ & answer_;
  }

};



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




namespace CardTest {
  enum Result { SUCCESS, FAIL, QUIT, DELETE, RETRY };
  inline Result correct(SimpleCard &card) {
    card.correctUpdate();
    return SUCCESS;
  }
  inline Result wrong(SimpleCard &card) {
    card.wrongUpdate();
    return FAIL;
  }
  inline Result deleteCard(SimpleCard &) {
    return DELETE;
  }
  inline Result skip(SimpleCard &) {
    return FAIL;
  }
  inline Result quit(SimpleCard &) {
    return QUIT;
  }
}

// machine specific query
// implement this for each env.
CardTest::Result query(SimpleCard &);


#endif
