#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <string>

using namespace std;

class Clipper {
   public:
      static void clip() {
         char buffer[128];
         string result = "";
         string c = "./clip.sh";

         // run command
         ::FILE* pipe = popen(c.c_str(), "r");
         if (!pipe) {
            return; //failed
         }

         // read till end of process:
         while (!feof(pipe)) {

            // use buffer to read and add to result
            if (fgets(buffer, 128, pipe) != NULL)
               result += buffer;
         }

         cout << result;

         pclose(pipe);
      }
};
