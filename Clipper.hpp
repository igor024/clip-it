#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <string>

using namespace std;

class Clipper {
   public:
      static void clip() {
         string result = "";
         string c = "./clip.sh";

         // run command
         ::FILE* pipe = popen(c.c_str(), "r");
         if (!pipe) {
            return; //failed
         }

         pclose(pipe);
      }
};
