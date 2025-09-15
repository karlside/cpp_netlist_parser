#ifndef NETLISTOBJECT_H
#define NETLISTOBJECT_H

#include <stdexcept>
#include <vector>
#include <string>

class NetlistStatement{
public:
  NetlistStatement(std::vector<std::string> line);
  // NetlistStatement(NetlistStatement &&) = default;
  // NetlistStatement(const NetlistStatement &) = default;
  // NetlistStatement &operator=(NetlistStatement &&) = default;
  // NetlistStatement &operator=(const NetlistStatement &) = default;
  // ~NetlistStatement();
  //
protected:
  virtual void parseLine(std::vector<std::string> line);
  
};

class AnalysisStatement : public NetlistStatement {
public:
  AnalysisStatement(std::vector<std::string> line);
protected:
  void parseLine(std::vector<std::string> line) override;
};

#endif
