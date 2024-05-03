#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <string>

using namespace std;

class Clipper {
   public:
      static void clip() {
         string c = "./clip.sh";

         // run command
         ::FILE* pipe = popen(c.c_str(), "r");
         if (!pipe) {
            return; //failed
         }

         /* to log output 
         string result = "";
         char buffer[128];
         while (!feof(pipe)) {

            // use buffer to read and add to result
            if (fgets(buffer, 128, pipe) != NULL)
               result += buffer;
         } 

         cout << result;
         */
        
         pclose(pipe);
      }
};
