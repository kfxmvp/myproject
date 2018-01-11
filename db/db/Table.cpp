#include <algorithm>
#include "Table.hpp"


std::map<std::string, Table> DB::_tables;


bool DB::isTableExist(const std::string &tableName) {
  return _tables.count(tableName) == 1;
}


bool DB::createTable(const std::string &tableName) {
  if (isTableExist(tableName))
    return false;

  Table t(tableName);
  //_tables[tableName] = t;
  _tables.insert({tableName, t});
  return true;
}


bool DB::destroyTable(const std::string &tableName) {
  return _tables.erase(tableName) == 1;
}

Table* DB::getTable(const std::string &tableName) {
  auto it= _tables.find(tableName);
  if (it == _tables.end()) 
    return nullptr;

  return &(it->second);
}

bool DB::insertTable(const std::string &tableName, const std::string &doc) {
  Table* t = getTable(tableName);
  if (t == nullptr)
    return false;

  return t->insertDoc(doc);
}

void DB::showTable(const std::string &tableName) {
  Table* t = getTable(tableName);
  if (t == nullptr)
    return;

  return t->show();
}

Table::Table(const std::string &tableName) 
  : _name(tableName)
{}

//生成的id保证不和当前所有id重复
size_t Table::genId() {
  if (_rows.empty())
    return 1;
  return _rows.rbegin()->first + 1;
}

bool Table::insertDoc(const std::string doc) {
  size_t id = genId();

  _rows.insert({id, Doc(id, doc)});

  return true;
}

void Table::show() const {
  printf("+---id---+-----doc-----+\n");
  std::for_each(_rows.cbegin(),
                _rows.cend(),
                [](const std::pair<size_t, Doc>&p) {
                  printf("|%5lu | %s |\n", p.first, p.second._doc.c_str());}
                );
  printf("+--------+-------------+\n");
}

