#ifndef CONTAINERS_H
#define CONTAINERS_H

#include <cassert>
#include <iostream>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

// --------------------------
// --- StatementContainer ---
// --------------------------

template <typename StatementType> struct Item {
  Item(std::pair<std::string, std::shared_ptr<StatementType>> input)
      : key{input.first}, item{input.second} {}
  std::string const key;
  std::shared_ptr<StatementType> item;
  std::string print_list() { return "TODO:"; }
};

template <typename StatementType> struct ListOfTypes {
public:
  void push_back(std::shared_ptr<StatementType> input);
  std::shared_ptr<StatementType> pop_back();
  std::shared_ptr<StatementType> get_item(std::string key) const;
  std::shared_ptr<StatementType> at(int index) const {
    return items.at(index).item;
  }
  int size() const { return items.size(); }

  auto begin() { return items.begin(); }
  auto end() { return items.end(); }
  auto begin() const { return items.begin(); }
  auto end() const { return items.end(); }

private:
  std::vector<Item<StatementType>> items;
  std::unordered_map<std::string, int> index;
  std::string create_key(std::string key, int iterator = 0);
};

template <typename StatementType>
std::string ListOfTypes<StatementType>::create_key(std::string key,
                                                   int iterator) {
  if (index.find(key) == index.end())
    return key;
  iterator += 1;
  key = key + "_" + std::to_string(iterator);
  return create_key(key, iterator);
}

template <typename StatementType>
std::shared_ptr<StatementType>
ListOfTypes<StatementType>::get_item(std::string key) const {
  if (index.find(key) == index.end())
    throw std::runtime_error("Key not in List");
  // TODO: Create new errors
  return items.at(index.at(key)).item;
}

template <typename StatementType>
void ListOfTypes<StatementType>::push_back(
    std::shared_ptr<StatementType> input) {
  std::string key = create_key(input->produce_id());
  items.push_back(Item(std::make_pair(key, input)));
  index[key] = index.size();
};

template <typename StatementType>
std::shared_ptr<StatementType> ListOfTypes<StatementType>::pop_back() {
  Item ret_item = items.back();
  items.pop_back();
  index.erase(ret_item.item->get_text());
  return ret_item.item;
}

template <typename StatementType>
std::ostream &operator<<(std::ostream &os,
                         const ListOfTypes<StatementType> &rhs) {
  for (Item<StatementType> item : rhs.items)
    os << item.item->get_text() << " ";
  return os;
}

#endif
