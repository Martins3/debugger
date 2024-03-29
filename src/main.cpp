#include <iostream>
#include <unistd.h> // fork
#include <sys/ptrace.h> // ptrace
#include "debugger.hpp"

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "Program name not specified";
    return -1;
  }

  auto prog = argv[1];

  auto pid = fork();
  if (pid == 0) {
    // we're in the child process
    // execute debugee
    ptrace(PTRACE_TRACEME, 0, nullptr, nullptr);
    execl(prog, prog, nullptr);

  } else if (pid >= 1) {
    // we're in the parent process
    // execute debugger
    debugger dbg{prog, pid};
    dbg.run();
  }
}
