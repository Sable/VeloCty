/**Copyright 2014, Rahul Garg and McGill University
   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
 */

#include <vraptor.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
/* #include "vrantlr.hpp" */
#include "vrtimer.hpp"
using namespace std;
using namespace VRaptor;


string readDataIntoString(const string& fname){
    ifstream ifile(fname.c_str());
    stringstream ss;
    string line;
    while(!ifile.eof()){
        getline(ifile,line);
        ss<<line<<endl;
    }
    return ss.str();
}

void doParseStuff(const std::string& data){
    VModule *mod = VModule::readFromString(data);
    if(mod!=NULL) delete mod;
}

int main(int argc,char **argv){
    if(argc!=3){
        cout<<"Usage: vrirfront OPTION FILE"<<endl;
    }
    initRaptor();

    std::string option(argv[1]);
    std::string fname(argv[2]);
    std::string data(readDataIntoString(fname));
     if(option.compare("-p")==0) doParseStuff(data);

}
