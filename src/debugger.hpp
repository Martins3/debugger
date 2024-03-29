#ifndef DEBUGGER_HPP_QP5PRAEH
#define DEBUGGER_HPP_QP5PRAEH
#include "../linenoise/linenoise.h"
#include <iostream>
#include <sstream>
#include <string>
#include <sys/wait.h> // waitpid
#include <sys/ptrace.h> // ptrace
#include <vector>

std::vector<std::string> split(const std::string &s, char delimiter) {
  std::vector<std::string> out{};
  std::stringstream ss{s};
  std::string item;

  while (std::getline(ss, item, delimiter)) {
    out.push_back(item);
  }

  return out;
}

bool is_prefix(const std::string &s, const std::string &of) {
  if (s.size() > of.size())
    return false;
  return std::equal(s.begin(), s.end(), of.begin());
}

class debugger {
public:
  debugger(std::string prog_name, pid_t pid)
      : m_prog_name{std::move(prog_name)}, m_pid{pid} {}

  void run();

private:
  void continue_execution();
  void handle_command(const std::string &line);
  std::string m_prog_name;
  pid_t m_pid;
};

void debugger::run() {
  int wait_status;
  auto options = 0;
  waitpid(m_pid, &wait_status, options);

  char *line = nullptr;
  while ((line = linenoise("minidbg> ")) != nullptr) {
    handle_command(line);
    linenoiseHistoryAdd(line);
    linenoiseFree(line);
  }
}

void debugger::handle_command(const std::string &line) {
  auto args = split(line, ' ');
  auto command = args[0];

  if (is_prefix(command, "continue")) {
    continue_execution();
  } else {
    std::cerr << "Unknown command\n";
  }
}

void debugger::continue_execution() {
  ptrace(PTRACE_CONT, m_pid, nullptr, nullptr);

  int wait_status;
  auto options = 0;
  waitpid(m_pid, &wait_status, options);
}

#endif /* end of include guard: DEBUGGER_HPP_QP5PRAEH */
