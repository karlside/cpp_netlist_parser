#ifndef LINE_H
#define LINE_H

#include "word.h"
#include <memory>
#include <string>
#include <vector>

class Line {
public:
  Line();
  Line(std::unique_ptr<std::vector<BaseWord>> input);
  void add(BaseWord word);
  BaseWord at(int index) const;
  int length() const;
  std::unique_ptr<Line> objectify(); // Return a 'dynamicly dispateched' line
  // object based in line content

  friend std::ostream &operator<<(std::ostream &os, const Line &rhs);

  virtual ~Line() = default;

private:
protected:
  std::unique_ptr<std::vector<BaseWord>> entries{};
  // virtual void analyse_entries();
};

class SimulatorStatement : public Line {
public:
  SimulatorStatement(std::unique_ptr<std::vector<BaseWord>> input);

private:
protected:
  void analyse_entries();
};

#endif
