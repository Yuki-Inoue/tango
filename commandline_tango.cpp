
#include "cardlist.hh"
#include "file_manager.hpp"

#include <string>
#include <map>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>


using namespace std;
using namespace boost;

typedef FileManager<Cardlist> FiledCardlist;


// query line and update if not empty
// if queried line is empty, then don't update.
void qlineupdateifne(string &target){
  string str;
  getline(cin,str);
  if(!str.empty())
    target = str;
}

static CardTest::Result edit(Card &card){
  cout << "change the question" << endl
       << card.question_ << " to: " << flush;
  qlineupdateifne(card.question_);
  cout << "change the answer" << endl
       << card.answer_ << " to: " << flush;
  qlineupdateifne(card.answer_);
  return CardTest::RETRY;
}


template <class Key, class Value>
void printOptions(ostream &os, const map<Key,Value> &map){
  os << '(';
  typename std::map<Key,Value>::const_iterator it = map.begin(), itend = map.end();

  if( itend == it ) {
    os << ')';
    return;
  }
  
  --itend;
  for(; it!=itend; ++it){
    os << it->first << '/';
  }
  os << it->first << ')';
  return;
}

CardTest::Result info(Card &card){
  std::cout << card << std::endl;
  return CardTest::RETRY;
}

class CardCommandMap {
  typedef map<char, CardTest::Result (*)(Card &)> Map;
  Map map_;
public:
  CardCommandMap(){
    map_['y'] = CardTest::correct;
    map_['n'] = CardTest::wrong;
    map_['x'] = CardTest::deleteCard;
    map_['s'] = CardTest::skip;
    map_['q'] = CardTest::quit;
    map_['e'] = edit;
    map_['i'] = info;
  }

  Map::mapped_type query(istream &is) const {
    char c;
    Map::const_iterator result;
    while(true){
      c = is.get();
      result = map_.find(c);
      if(result != map_.end())
	break;
    }
    while(is.get()!='\n'); // remove tailing \n
    return result->second;
  }

  static const CardCommandMap &instance(){
    static CardCommandMap inst;
    return inst;
  }

  friend ostream &operator<<(ostream &os, const CardCommandMap &cmap){
    printOptions(os, cmap.map_);
    return os;
  }
};






CardTest::Result query(Card &card){

  cout << "Q: " << card.question_ << endl;
  cout << "(press RET to see answer. Other keys ignored, except for 'quit')" << endl;

  string buf;
  getline(cin, buf);
  if(!buf.compare("quit"))
    return CardTest::QUIT;
  if(!buf.compare(card.answer_)){
    cout << "CORRECT!" << endl;
    return CardTest::correct(card);
  }

  cout << "A: " << card.answer_ << endl;
  cout << "You got the right answer?  " << CardCommandMap::instance() << endl;

  return CardCommandMap::instance().query(cin)(card);
}



bool makeCardCommand(FiledCardlist &cl){
  Card card;
  cout << "making a card.\n";

  cout << "\tQuestion: " << flush;
  getline(cin, card.question_);
  if(card.question_.empty())
    goto ERROR;

  cout << "\tAnswer  : ";
  getline(cin, card.answer_);
  if(card.answer_.empty())
    goto ERROR;

  cout << card;
  cl.add(card);
  return true;

 ERROR:
  printf("Failed making card\n");
  return true;
}

bool testCommand(FiledCardlist &cl){
  cl.test();
  return true;
}

bool writeCommand(FiledCardlist &cl){
  cl.write();
  return true;
}

bool quitCommand(FiledCardlist &){
  return false;
}

bool listCommand(FiledCardlist &cl){
  cout << cl;
  return true;
}

bool infoCommand(FiledCardlist &cl){
  cout << "# of cards: " << cl.size() << endl
       << "nexptime  : " << cl.nexptime() << endl;
  return true;
}


class CommandMap {
  typedef map<string, bool (*)(FiledCardlist &)> Map;
  Map map_;

  CommandMap(){
    map_["w"] = writeCommand;
    map_["q"] = quitCommand;
    map_["t"] = testCommand;
    map_["m"] = makeCardCommand;
    map_["l"] = listCommand;
    map_["i"] = infoCommand;
  }

public:
  static CommandMap &instance() {
    static CommandMap inst;
    return inst;
  }

  Map::mapped_type query(){
    string str;
    Map::mapped_type result;
    while(true){
      cout << "> ";
      getline(cin, str);
      Map::iterator it = map_.find(str);
      if(it != map_.end()) {
	result = it->second;
	break;
      }
      cout << "error! valid commands are: " << CommandMap::instance() << endl;
    }
    return result;
  }

  friend ostream &operator<<(ostream &os, const CommandMap &cmap){
    printOptions(os, cmap.map_);
    return os;
  }
};



int main(int argc, char *argv[]){

  tzset();
  srand(time(0));

  if(argc == 1){
    cerr << "usage: " << argv[0] << " <archive>" << endl;
    return 1;
  }

  FiledCardlist cl(argv[1]);


  cl.read();

  while(CommandMap::instance().query()(cl));
  return 0;
}
