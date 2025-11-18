#ifndef LINE_H
#define LINE_H

#include "word.h"
#include <memory>
#include <string>
#include <vector>

class Line {
public:
  Line();
  Line(std::unique_ptr<std::vector<std::shared_ptr<Word>>> input);
  void add(std::shared_ptr<Word> word);
  std::shared_ptr<Word> *at(int index) const;
  std::string get_text() const;
  int length() const;
  std::unique_ptr<Line> objectify(); // Return a 'dynamicly dispateched' line
  // object based in line content

  friend std::ostream &operator<<(std::ostream &os, const Line &rhs);

  virtual ~Line() = default;

private:
protected:
  std::unique_ptr<std::vector<std::shared_ptr<Word>>> entries{};
};

class SimulatorStatement : public Line {
public:
  SimulatorStatement(std::unique_ptr<std::vector<std::shared_ptr<Word>>> input);

private:
protected:
  void analyse_entries();
};

#endif
