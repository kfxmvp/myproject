#include <iostream>
#include <stdio.h>
//#include <unistd.h>
#include "Shell.hpp"

int main(int argc, char *argv[]) {
  Shell shell;
  shell.showHelp();

  shell.loop();
  printf("all passs\n");
}
