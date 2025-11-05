#ifndef WORD_H
#define WORD_H

#include <string>

class Word {
public:
  Word(std::string text);
  bool is_empty() const;
  std::string get_text() const;

  friend std::ostream &operator<<(std::ostream &os, const Word &rhs);

private:
  std::string text;
  bool empty{false};
};

#endif
