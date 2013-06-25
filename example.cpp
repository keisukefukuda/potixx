/*
    This file is part of Poti

    Poti is free software: you can redistribute it and/or modify
    it under the terms of the GNU Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Poti is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Public License for more details.

    You should have received a copy of the GNU Public License
    along with Poti. If not, see <http://www.gnu.org/licenses/>.
*/

#include <iostream>

#include "potixx.hpp"

int main (int argc, char **argv)
{
  if (argc > 1) {
    poti_open(argv[1]);
  }

  //first, dump the pajeheader
  //first parameter is a boolean indicating a basic header or not
  //second parameter is a boolean indicating an old header or not
  poti_header (0, 0);

  //Defining my types
  poti::ContainerType root_type("ROOT", NULL);
  poti::ContainerType program_type("PROGRAM", &root_type);
  poti::ContainerType thread_type("THREAD", &program_type);
  poti::LinkType      link_type("LINK", &program_type, &thread_type, &thread_type);
  
  poti::StateType state("STATE", &thread_type);
  
  const char *COLOR_GREEN = "0.0 0.6 0.4";
  const char *COLOR_RED   = "0.9 0.1 0.0";
  //define values and color for the STATE type
  poti::EntityValue<poti::StateType> r("running", &state, 0.0, 0.6, 0.4);
  poti::EntityValue<poti::StateType> i("idle", &state, 0.9, 0.1, 0.0);

  //define values and color for the LINK type
  //poti_DefineEntityValue("c", "LINK", "communication", "1 0 0");
  poti::EntityValue<poti::LinkType> c("communication", &link_type, 1,0,0);

  //Create my root container and containers for two threads
  poti::Container root(0.00, "root", &root_type, NULL);
  poti::Container program(0.01, "program", &program_type, &root);
  poti::Container thread_0(0.23, "thread-0", &thread_type, &program);
  poti::Container thread_1(0.23, "thread-1", &thread_type, &program);
  
  // put a link from thread-0 to thread-1
  poti::Link link(&link_type, &program, &thread_0, 0.24, &thread_1, 1.02, &c, "myKey-001");

  //Push and Pop two states for each thread
  thread_0.pushState(0.34, &r);
  thread_0.popState(0.56);
  thread_0.pushState(0.67, &i);
  thread_0.popState(0.90);

  thread_1.pushState(0.45, &r);
  thread_1.pushState(0.89, &i);
  thread_1.popState(1.01);
  thread_1.popState(1.11);

  //Closing containers
  thread_0.destroy(1.12);
  thread_1.destroy(1.23);
  program.destroy(1.34);
  root.destroy(1.34);

  if (argc > 1) {
    poti_close();
  }

  return 0;
}
