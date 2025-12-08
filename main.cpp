#include "dependencies/line.h"
#include "dependencies/netlist.h"
#include "dependencies/netlistObjects.h"
#include "dependencies/word.h"
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

std::string FILE_PATH = "files/netlist.cir";

int main() {
  Netlist netlist = Netlist();
  std::unique_ptr<std::fstream> file = netlist.load_file(FILE_PATH);
  netlist.load_netlist_from_file(file);

  std::cout << "PRINTING NETLIST:" << std::endl;
  std::cout << netlist << std::endl;

  // std::shared_ptr<ListOfWords> myWords = std::make_shared<ListOfWords>();
  // std::shared_ptr<Word> word = std::make_shared<Word>("lang=spectre");
  // std::shared_ptr<StatementWord> word0 = word->objectify();
  // word = std::make_shared<Word>("(1 2 3)");
  // std::shared_ptr<StatementWord> word1 = word->objectify();
  // // word0->deactivate();
  // myWords->push_back(word0);
  // myWords->push_back(word1);
  //
  // Statement myLine = Statement(myWords);
  //
  // std::cout << myLine << std::endl;
  // std::cout << myLine.print_line() << std::endl;
  // // myLine.get_word("lang")->deactivate();
  // // myLine.get_word("port")->deactivate();
  // std::cout << myLine.print_line() << std::endl;
  // std::cout << myLine << std::endl;
  return 0;
}
