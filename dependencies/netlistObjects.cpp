
#include "netlistObjects.h"
#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>

NetlistStatement::NetlistStatement(std::vector<std::string> line) {
  std::cout << "inside construtor" << std::endl;
  parseLine(line);
}

void NetlistStatement::parseLine(std::vector<std::string> line) {
  // TODO:
  std::cout << "parsing line from base" << std::endl;
}

 AnalysisStatement::AnalysisStatement(std::vector<std::string> line) 
   : NetlistStatement(line)
 {
   // parseLine(line);
 }

void AnalysisStatement::parseLine(std::vector<std::string> line) {
  std::cout << "parsing line from child " << std::endl;
  if (line.empty() || "simulator" != line.at(0)) 
    throw std::invalid_argument("AnalysisStatement - invalid line");
}

