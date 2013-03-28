
#include "cardlist.hh"
#include "file_manager.hpp"
#include <commandmap.hpp>
#include <readstring.hpp>

#include <string>
#include <map>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>


using namespace std;
using namespace boost;

typedef FileManager<Cardlist> FiledCardlist;





static bool exitflag = false;



static CardTest::Result edit(SimpleCard &card){

  {

      cout << "change the question" << endl;
      string buff = readstring(card.getQuestion() + " to: ");
      if(!buff.empty())
          card.setQuestion(std::move(buff));
  }

  {
      cout << "change the answer" << endl;
      string buff = readstring(card.getAnswer() + " to: ");
      if(!buff.empty())
          card.setAnswer(std::move(buff));
  }

  return CardTest::RETRY;
}



CardTest::Result info(SimpleCard &card){
  std::cout << card << std::endl;
  return CardTest::RETRY;
}


class CardCommandMap : public CommandMap<CardTest::Result (*)(SimpleCard &)> {

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


CardTest::Result query(SimpleCard &card){

  static const string quit = "quit";
  static const string skip = "skip";

  cout << "Q: " << card.getQuestion() << '\n'
       << "(press RET to see answer. Other keys ignored, except for '" << quit << "' and '" << skip << "')" << endl;

    string buf = readstring("");
  if(!buf.compare(quit))
    return CardTest::quit(card);
  if(!buf.compare(card.getAnswer())){
    cout << "CORRECT!\n" << endl;
    return CardTest::correct(card);
  }
  if(!buf.compare(skip)){
    cout << endl;
    return CardTest::skip(card);
  }

  cout << "A: " << card.getAnswer() << '\n'
       << "You got the right answer?  " << CardCommandMap::instance() << endl;

  return CardCommandMap::instance().query("")(card);
}



bool makeCardCommand(FiledCardlist &cl){
  SimpleCard card;
  string qbuff, abuff;

    cout << "making a card." << endl;
    qbuff = readstring("    Question: ");
  if(qbuff.empty())
    goto ERROR;
  else
    card.setQuestion(std::move(qbuff));

    abuff = readstring("    Answer  : ");
  if(abuff.empty())
    goto ERROR;
  else
    card.setAnswer(std::move(abuff));

  cout << card;
  cl.add(std::move(card));
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
  string query =
    readstring("    query: ");
  cl.search(query);
  return true;
}

bool searchAllCommand(FiledCardlist &cl){
  string query =
    readstring("    query: ");
  cl.search_all(query);
  return true;
}


bool quitAllCommand(FiledCardlist &){
  exitflag = true;
  return false;
}

bool writeQuitAllCommand(FiledCardlist &cl){
  cl.write();
  exitflag = true;
  return false;
}




class CardlistCommandMap : public CommandMap<bool (*)(FiledCardlist &)> {

  CardlistCommandMap(){
    map_["w"] = writeCommand;
    map_["q"] = quitCommand;
    map_["wq"] = writeQuitCommand;
    map_["wqa"] = writeQuitAllCommand;
    map_["t"] = testCommand;
    map_["m"] = makeCardCommand;
    map_["l"] = listCommand;
    map_["i"] = infoCommand;
    map_["s"] = searchCommand;
    map_["sa"] = searchAllCommand;
    map_["qa"] = quitAllCommand;
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
    // consider as test all mode
    if(!expired(cl) && argc > 2){
      cout << "skipping this archive since no expired card\n" << endl;
      continue;
    }
    do cout << "# of expired: " << cl.expnum() << '\n'
	    << "Knowledge   : " << cl.knowledge() << endl;
    while(CardlistCommandMap::instance().query()(cl));
    if(exitflag)
      break;
  }

  return 0;
}
