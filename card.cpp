#include "card.hh"

using namespace std;
using namespace boost::posix_time;





void Card::correctUpdate(){
  crct_++;
  ptime current(second_clock::local_time());
  nexptime_ = current + hours( inchourbycrct() );
  anneal();
}

void Card::wrongUpdate(){
  crct_ = 0;
}

void Card::anneal(){
  unsigned rsec = inchourbycrct() * 60 * 60;
  unsigned range = 2*rsec+1;
  nexptime_ += seconds(rand()%range - rsec);
}

ostream &operator<<(ostream &os, const Card &card){
  os << card.nexptime_ << endl;
  os << card.crct_ << endl;
  os << card.question_ << endl;
  os << card.answer_ << endl;
  os << endl;
  return os;
}

istream &operator>>(istream &is, Card &card){

  is >> card.nexptime_;
  if (card.nexptime_.is_not_a_date_time())
    goto ERROR;

  is >> card.crct_;
  if(!is.good())
    goto ERROR;
  is.ignore();

  getline(is,card.question_);
  getline(is,card.answer_);

  char buf[2];
  is.getline(buf,2);
  if(strlen(buf) != 0)
    goto ERROR;

  return is;

 ERROR:
  throw Card::FailedMaking();
}
