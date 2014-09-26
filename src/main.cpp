#include <vraptor.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include "vrtimer.hpp"
#include<codegen.hpp>
#include<iostream>
#include<sstream>
#include<string>
#include<fstream>
#include<cstdlib>
#include<prettyPrinter.hpp>
#include<vector>
#include<string.h>
#include <node-collector.hpp>
using namespace std;
using namespace VRaptor;

string readFile(const string& fname) {
        ifstream f(fname.c_str());
        stringstream buf;
        while (f.good()) {
                string line;
                getline(f, line);
                buf << line << endl;
        }
        f.close();
        return buf.str();
}
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
void writeFile(const string& fname , vector<string>& data){
	ofstream f(fname.c_str());
	for(int i=0;i<data.size();i++){
		f<<data[i];
	}
}
string getFileNameNoExt(string fname){
	std::string fCopy =fname;	
	char* tok = strtok(const_cast<char*>(fCopy.c_str()),"/");
	char* tok1=NULL;
	while(tok!=NULL) {
		tok1=tok;
		tok = strtok(NULL,"/");
	}
	if(tok1==NULL){
		tok1=const_cast<char*>(fname.c_str());
	}
	//std::cout<<"value of tok1"<<tok1<<std::endl;
	return string(const_cast<const char*>(strtok(const_cast<char*>(tok1),".")));
}
int main(int argc,char * argv[]){
	if(argc<2){
		std::cout<<"Usage: <filename>"<<std::endl;
		exit(0);
	}
	bool genFile =false;
	std::string fname;
	std::string optionStr="";
    initRaptor();
	for(int i = 1; i < argc ; i++) {
		int optionIndex = -1;
		if(argv[i][0]=='-') {
			if(strlen(argv[i])<=2) {
				optionStr = argv[i++];
				 optionIndex  = i-1;
			}
			else {
				optionStr = argv[i]+2;
				optionIndex = i;
			}
			switch (argv[optionIndex][1]) {
				case 'g': 
					genFile =true;
					break;
				default :
					std::cout<<"Entering default condition"<<std::endl;
					break;
						
			}
		}
		else {
			fname = argv[i];
		}
	}	
	std::string fCopy = fname;
    std::cout<<"file name "<<fname<<std::endl; 
	string s =readDataIntoString(fname);
	VModule *m=NULL;
	m= VModule::readFromString(s);
	if(m!=NULL){
		std::cout<<"Number of functions : "<<m->getFns().size()<<std::endl;
	}
	NodeCollector nc;
	nc.analyze(m);
	//nc.prettyPrint();
	VCompiler vc(getFileNameNoExt(fCopy));
	vc.setCollector(nc);
	fCopy=fname;
	Context cntxt=vc.moduleCodeGen(m);
	std::cout<<cntxt.getAllStmt().size()<<std::endl;
	PrettyPrinter pp;

	vector<string> vec_cpp = pp.prettyPrint(cntxt.getAllStmt());
	for(int i=0;i<vec_cpp.size();i++){
		std::cout<<vec_cpp[i];
	}
	vector<string> vec_hpp = pp.prettyPrint(vc.getHeaderContext().getAllStmt());
	for(int i=0;i<vec_hpp.size();i++){
		std::cout<<vec_hpp[i];
	}
// //#ifdef GEN_CODE_FILE
	if(genFile) {
		std::string fNameNoExt = getFileNameNoExt(fCopy);
		writeFile(fNameNoExt+"Impl.cpp",vec_cpp);	
		writeFile(fNameNoExt+"Impl.hpp",vec_hpp);
	}
//#endif
}
