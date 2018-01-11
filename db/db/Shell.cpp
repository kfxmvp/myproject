#include <regex>
#include <algorithm>
#include <iterator>
#include <string>
#include <iostream>
#include <stdio.h>
#include <map>
#include "Shell.hpp"
#include "Table.hpp"

//?????? ????? ??????? ????????
//CREATE TABLE test
//CREATE TABLE 
//INSERT table id "fjeosf"
//DELTE TABLE test 3


static bool s_isCmdEnd = false;
void Shell::showHelp() {
  const std::string msg=
      R"(
Usage: command;[comands]
comand:
  CREATE TABLE  <table> -- create a db table
  DESTROY TABLE <table> -- delete a table
  SHOW TABLE  <table> -- list all of table rows
  INSERT        <table> <data> -- insert data to table
  DELETE        <table> <data> -- delete data from table
  SEARCH        <table> <pattern> -- serach data
  IMPORT        <table> [file] -- import file to table
  EXPORT        <file> [table] -- export table to fale
  )";
  std::cout << msg << std::endl;
}

static void badCmd() {
  printf("Bad command\n");
  fflush(stdin);
}


void Shell::loop() {
  while (1) {
    //printf("--debug-- parseCmd\n");
    s_isCmdEnd = false;
    parseCmd();
  }
}

inline static bool isSplitChar(int c) {
  return isblank(c) || c == ';' || c == '\r' || c == '\n';
}

static std::string getToken() {
  std::string token;
  int c;
  while (EOF != (c = getchar())) {
    if (isSplitChar(c)) {
      if (c == ';') {
        s_isCmdEnd = true;
        if (token.empty())
          token += c;
        return token;
      } else {
        if (! token.empty())
          return token;
      }
      continue;
    }
    token += c;
    //printf("--debug %d:%s\n", c, token.c_str());
  }
  return token;
}

bool Shell::isEnd() {
  return s_isCmdEnd || ";" == getToken();
}

void Shell::parseCreateTableParams() {
  std::string token = getToken();
  //printf("parseCreate Table cmd table:%s\n", token.c_str());
  //TODO check is keyword
  //TODO check table is exist
  if (isEnd()) {
    if (DB::createTable(token)) {
      printf("create table %s successed\n", token.c_str());
    } else {
      printf("create table %s failed\n", token.c_str());
    }
    return ;
  }

  badCmd();
}

void Shell::parseInsertTableParams() {
  std::string doc = getToken();
  if (isEnd()) {
    if (DB::insertTable(_tableName, doc)) {
      printf("successed\n");
    } else {
      printf("failed\n");
    }
    return ;
  }
  badCmd();
}


void Shell::parseShowTableParams() {
  std::string token = getToken();
  
  if (!isEnd())
    badCmd();

  DB::showTable(token);
}

void Shell::parseShowTable() {
  std::string token = getToken();
  if (token != "TABLE") {
    badCmd();
    return;
  }
  parseShowTableParams();
}

void Shell::parseInsertTable() {
  _tableName = getToken();

  parseInsertTableParams();
}

void Shell::parseCreateTable() {
  std::string token = getToken();
  if (token != "TABLE") {
    badCmd();
    return;
  }

  parseCreateTableParams();
}

void Shell::parseDeleteTableParams() {
  std::string token = getToken();
  
  if (!isEnd())
    badCmd();

  if (DB::destroyTable(token)) 
    printf("successed\n");
  else
    printf("failed\n");
}



void Shell::parseDeleteTable() {
  std::string token = getToken();
  //printf("--debug getToken:%s\n", token.c_str());
  if (token != "TABLE") {
    badCmd();
    return;
  }
  parseDeleteTableParams();
}


void Shell::parseCmd() {
  printf(">>");
  std::string token = getToken();
  if (token == ";")
    return;

  //printf("parseCmd\n");
  std::map<std::string, void (Shell::*)() > table{
    {"CREATE",  &Shell::parseCreateTable},
    {"DESTROY", &Shell::parseDeleteTable},
    {"SHOW"   , &Shell::parseShowTable},
    {"INSERT",  &Shell::parseInsertTable},
    {"DELETE",  &Shell::parseCreateTable},
    {"SEARCH",  &Shell::parseCreateTable},
    {"IMPORT",  &Shell::parseCreateTable},
    {"EXPORT",  &Shell::parseCreateTable}
  };

  auto it = table.find(token);
  if (it == table.end()) {
    badCmd();
    return;
  }

  auto fn = it->second;
  (this->*fn)();
}

