#ifndef TABLE_H_
#define TABLE_H_

#include <string>
#include <map>

struct Doc{
  Doc(size_t id, const std::string &doc) : _key(id), _doc(doc)
  {}
  size_t _key;
  std::string _doc;
};

class Table {
 public:
  Table(const std::string &table) ;
  //Table(const Table&) = delete;
  Table& operator=(const Table&) = delete;

  bool insertDoc(const std::string doc);
  void show() const;
  //delete;
  //update;
  //find;

 private:
  size_t genId();

  std::string _name;
  std::map<size_t, Doc> _rows;
};

class DB {
 public:
  static bool createTable(const std::string &tableName);
  static bool destroyTable(const std::string &tableName);
  static bool isTableExist(const std::string &tableName);
  static bool insertTable(const std::string &tableName, 
                          const std::string &doc);

  static void showTable(const std::string &tableName);
 private:
  static Table* getTable(const std::string &tableName);


 private:
  static std::map<std::string, Table> _tables;
};

#endif
