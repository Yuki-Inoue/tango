
#include "cardlist.hh"
#include "file_manager.hpp"
#include <commandmap.hpp>

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
  cout << "change the question" << '\n'
       << card.question_ << " to: " << flush;
  qlineupdateifne(card.question_);
  cout << "change the answer" << '\n'
       << card.answer_ << " to: " << flush;
  qlineupdateifne(card.answer_);
  return CardTest::RETRY;
}



CardTest::Result info(Card &card){
  std::cout << card << std::endl;
  return CardTest::RETRY;
}


class CardCommandMap : public CommandMap<CardTest::Result (*)(Card &)> {

  CardCommandMap(){
    map_["y"] = CardTest::correct;
    map_["n"] = CardTest::wrong;
    map_["x"] = CardTest::deleteCard;
    map_["s"] = CardTest::skip;
    map_["q"] = CardTest::quit;
    map_["e"] = edit;
    map_["i"] = info;
  }

public:

  static const CardCommandMap &instance(){
    static CardCommandMap inst;
    return inst;
  }

};


CardTest::Result query(Card &card){

  static const string quit = "quit";
  static const string skip = "skip";

  cout << "Q: " << card.question_ << '\n'
       << "(press RET to see answer. Other keys ignored, except for '" << quit << "' and '" << skip << "')" << endl;

  string buf;
  getline(cin, buf);
  if(!buf.compare(quit))
    return CardTest::quit(card);
  if(!buf.compare(card.answer_)){
    cout << "CORRECT!\n" << endl;
    return CardTest::correct(card);
  }
  if(!buf.compare(skip)){
    cout << endl;
    return CardTest::skip(card);
  }

  cout << "A: " << card.answer_ << '\n'
       << "You got the right answer?  " << CardCommandMap::instance() << endl;

  return CardCommandMap::instance().query("")(card);
}



bool makeCardCommand(FiledCardlist &cl){
  Card card;

  cout << "making a card.\n"
       << "\tQuestion: " << flush;
  getline(cin, card.question_);
  if(card.question_.empty())
    goto ERROR;

  cout << "\tAnswer  : " << flush;
  getline(cin, card.answer_);
  if(card.answer_.empty())
    goto ERROR;

  cout << card;
  cl.add(card);
  return true;

 ERROR:
  cout << "Failed making card" << endl;
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

bool writeQuitCommand(FiledCardlist &cl){
  cl.write();
  return false;
}

bool listCommand(FiledCardlist &cl){
  cout << cl;
  return true;
}

bool infoCommand(FiledCardlist &cl){
  cout << "# of cards: " << cl.size() << '\n'
       << "nexptime  : " << cl.nexptime() << endl;
  return true;
}

bool searchCommand(FiledCardlist &cl){
  string query;
  cout << "\tquery: " << flush;
  getline(cin, query);
  cl.search(query);
  return true;
}


class CardlistCommandMap : public CommandMap<bool (*)(FiledCardlist &)> {

  CardlistCommandMap(){
    map_["w"] = writeCommand;
    map_["q"] = quitCommand;
    map_["wq"] = writeQuitCommand;
    map_["t"] = testCommand;
    map_["m"] = makeCardCommand;
    map_["l"] = listCommand;
    map_["i"] = infoCommand;
    map_["s"] = searchCommand;
  }

public:
  static CardlistCommandMap &instance() {
    static CardlistCommandMap inst;
    return inst;
  }
};



int main(int argc, char *argv[]){

  tzset();
  srand(time(0));

  if(argc == 1){
    cerr << "usage: " << argv[0] << " <archive(s)>" << endl;
    return 1;
  }

  for(int i=1; i<argc; ++i){
    cout << "archive of " << argv[i] << endl;
    FiledCardlist cl(argv[i]);
    cl.read();
    do cout << "# of expired: " << cl.expnum() << endl;
    while(CardlistCommandMap::instance().query()(cl));
  }

  return 0;
}
