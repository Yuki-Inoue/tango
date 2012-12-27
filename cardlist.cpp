#include "cardlist.hh"

using namespace std;
using namespace boost::posix_time;



list<Card>::size_type Cardlist::expnum() const{
  const ptime current = second_clock::local_time();
  list<Card>::size_type n = 0;
  for(auto it=l_.begin(); it->getNexptime() < current && it!=l_.end(); ++it)
    ++n;
  return n;
}

void Cardlist::search(const string &str){
  function<bool(const Card &)> contain_str =
    [&str](const Card &card) {
    return card.question_.find(str) != string::npos;
  };
  operate(contain_str, always_true);
}


ostream &operator<<(ostream &out, const Cardlist &cl){
  for(const Card &card : cl.l_)
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


static void success_operation(list<Card> &updated, list<Card> &on_operation, list<Card>::iterator &it){
  auto ittemp = it++;
  updated.splice
    (std::lower_bound
     (updated.begin(), updated.end(), *ittemp), on_operation, ittemp);
}


void Cardlist::operate(const function<bool(const Card &)> &do_test, const function<bool(const Card &)> &do_continue){
  auto it = l_.begin();
  list<Card> updated;
  bool finish_flag = false;
  while( !finish_flag && it!=l_.end() && do_continue(*it) ){
    if( !do_test(*it) ){
      ++it;
      continue;
    }
    switch(query(*it)) {
    case CardTest::SUCCESS:
      success_operation(updated, l_, it);
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
