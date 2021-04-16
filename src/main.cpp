#include "ncurses_display.h"
#include "system.h"

int main() {
  System system;
  //system.MemoryUtilization();
  //system.Cpu();
  //Process process(system.Processes().front());
  //Process process(1651);
  //process.CpuUtilization();
  NCursesDisplay::Display(system);
}