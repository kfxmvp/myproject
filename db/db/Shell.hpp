#ifndef SHELL_H_
#define SHELL_H_


class Shell {
  enum N_OP {
    CREATE_TABLE ,
    INSERT       ,
    DELETE       ,
    SERARCH      ,
    DESTROY_TABLE,
    IMPORT_TABLE ,     
    EXPORT_TABL ,
  };
 public:
  void showHelp();
  void loop();

 private:
  void parseCmd();
  void parseCreateTable();
  void parseCreateTableParams();

  void parseDeleteTable();
  void parseDeleteTableParams();

  void parseInsertTable();
  void parseInsertTableParams();

  void parseShowTable();
  void parseShowTableParams();

  bool isEnd();


 private:
  std::string _line;
  std::string _tableName;
  std::string _file;
};

#endif

