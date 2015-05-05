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
#include<getopt.h>
using namespace std;
using namespace VRaptor;

extern bool memOptimise;
extern bool prelim_bounds;
extern bool phase2Optimise;
extern bool enableOpenMp;

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
    return string(const_cast<const char*>(strtok(const_cast<char*>(tok1),".")));
}
void setOptFlags(string optArg) {
    if(optArg.compare("mem") == 0) {
        memOptimise = true;
    }
    if(optArg.compare("bounds") == 0) {
        prelim_bounds=true;
        phase2Optimise = true;
    } 
    if(optArg.compare("par") == 0) {
        enableOpenMp = true;
    }
    if(optArg.compare("all") == 0) {
        enableOpenMp = true;
    }
}
     
int main(int argc,char * argv[]){
    if(argc<2){
        std::cout<<"Usage: <filename>"<<std::endl;
        exit(0);
    }
    static struct option long_options[] =
    {
        /* These options don’t set a flag.
           We distinguish them by their indices. */
        {"opt", required_argument,0, 'O'},
        {"outFile",required_argument,0, 'f'},
        {0, 0, 0, 0}
    };
    std::string fname;
    std::string outFilePath = "";
    bool genFile =false;
    while(1) {
        int option_index = 0;
        int c = getopt_long (argc, argv, "O:f:",
                 long_options, &option_index);
        if(c==-1)  {
            break;
        }
        switch(c) {
            case 0:    
                printf("probably should not be here\n");
                break;
            case 'O':
                setOptFlags(optarg);
                break;
            case 'f' :
                outFilePath = strdup(optarg);
                genFile = true;
                break;
            default: 
                break;
        }
    
    }
    if(optind < argc) {
        fname  = argv[optind];      
    } else  {
        printf("atleast one input file required\n");
    }
    std::string optionStr="";
    initRaptor();
    std::string fCopy = fname;
    string s =readDataIntoString(fname);
    VModule *m=NULL;
    m= VModule::readFromString(s);
    NodeCollector nc;
    nc.analyze(m);
    std::string str = getFileNameNoExt(fCopy);
    VCompiler vc(str);
    vc.setCollector(nc);
    Context cntxt=vc.moduleCodeGen(m);
    PrettyPrinter pp;

    vector<string> vec_cpp = pp.prettyPrint(cntxt.getAllStmt());
    vector<string> vec_hpp = pp.prettyPrint(vc.getHeaderContext().getAllStmt());
    if(!genFile) {
        for(int i=0;i<vec_cpp.size();i++){
            std::cout<<vec_cpp[i];
        }
        for(int i=0;i<vec_hpp.size();i++){
            std::cout<<vec_hpp[i];
        }
    } else  {
        writeFile(str+"Impl.cpp",vec_cpp);	
        writeFile(str+"Impl.hpp",vec_hpp);
    }
}
