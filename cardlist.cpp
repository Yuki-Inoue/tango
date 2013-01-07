#include "cardlist.hh"

using namespace std;
using namespace boost::posix_time;



list<SimpleCard>::size_type Cardlist::expnum() const{
  const ptime current = second_clock::local_time();
  list<SimpleCard>::size_type n = 0;
  for(auto it=l_.begin(); it!=l_.end() && it->getNexptime() < current; ++it)
    ++n;
  return n;
}

void Cardlist::search(const string &str){
  function<bool(const SimpleCard &)> contain_str =
    [&str](const SimpleCard &card) {
    return card.getQuestion().find(str) != string::npos;
  };
  operate(contain_str, always_true);
}


ostream &operator<<(ostream &out, const Cardlist &cl){
  for(const SimpleCard &card : cl.l_)
    out << card;
  return out;
}

istream &operator>>(istream &in, Cardlist &cl){
  try{
    while(true){
      SimpleCard card;
      in >> card;
      cl.l_.push_back(card);
    }
  }
  catch(SimpleCard::FailedMaking &e){
  }
  return in;
}


static void success_operation(list<SimpleCard> &updated, list<SimpleCard> &on_operation, list<SimpleCard>::iterator &it){
  auto ittemp = it++;
  updated.splice
    (std::lower_bound
     (updated.begin(), updated.end(), *ittemp), on_operation, ittemp);
}


void Cardlist::operate(const function<bool(const SimpleCard &)> &do_test, const function<bool(const SimpleCard &)> &do_continue){
  auto it = l_.begin();
  list<SimpleCard> updated;
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
