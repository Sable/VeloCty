/*
 * codegen.hpp
 *
 *  Created on: 2013-03-15
 *      Author: sjagda
 */

#ifndef CODEGEN_HPP_
#define CODEGEN_HPP_

#include <vraptor.hpp>
#include <vector>
#include<iostream>
#include<string>
#include<sstream>
#include<stdlib.h>
#include<algorithm>
#include<set>
#include<map>
#include<tr1/unordered_map>
#include<iterator>
#include<vrloopinfo.hpp>
#include "vrPrelimBCE.hpp"
#include<node-collector.hpp>
#include<loopCollector.hpp>

using std::vector;
using std::string;
using std::set;
namespace VRaptor {
    typedef  unordered_map<ForStmtPtr, IndexSet> ForToIndexMap;

/*! \brief Class to  hold the generated code.
     The generated code is stored as a vector of strings. A context class object may also stored additional data that can be sent back .
*/
class Context {

	vector<string> list; 
	bool arrayFlag;
	

public:
	Context();
	void addStmt(const string stmt);//!<Add generated code to the list 
	void addStmtVec(vector<string> );
	vector<string> getAllStmt();    //<! returns vector of generated code
	void setArrayFlag();
	void resetArrayFlag();
	bool isArray();
};

class BuiltinFunction {
	private :
		const std::string funcName;
		bool arrayOutput;
		bool varArgs;
		bool hasScalOut;
	public :
		BuiltinFunction(std::string funcName,bool arrayOutput,bool varArgs=false,bool hasScalOut = false) : funcName(funcName), 
		arrayOutput(arrayOutput),varArgs(varArgs),hasScalOut(hasScalOut) 
		{
		  
		}
		bool isArrayOutput() const{
			return arrayOutput;
		}
		const std::string getFuncName() const {return funcName;}
		bool hasVarArgs() {
		  return varArgs;
		}
		bool isScalOut() { return hasScalOut;}
}; //end of class BuiltinFunction.

class BuiltinMapper {
	private :
		unordered_map<std::string, BuiltinFunction> builtinMap;
	public :
	void init() {
		builtinMap.insert(std::make_pair<std::string,BuiltinFunction>("rand",BuiltinFunction("rand",true,true)));	
		builtinMap.insert(std::make_pair<std::string,BuiltinFunction>("sqrt",BuiltinFunction("sqrt",true,false,true)));	
		builtinMap.insert(std::make_pair<std::string,BuiltinFunction>("abs",BuiltinFunction("abs",true)));	
		builtinMap.insert(std::make_pair<std::string,BuiltinFunction>("tan",BuiltinFunction("tan",true)));	
		builtinMap.insert(std::make_pair<std::string,BuiltinFunction>("mmult",BuiltinFunction("mmult",true)));
		builtinMap.insert(std::make_pair<std::string,BuiltinFunction>("mean",BuiltinFunction("mean",true)));
		builtinMap.insert(std::make_pair<std::string,BuiltinFunction>("sum",BuiltinFunction("sum",true)));
		builtinMap.insert(std::make_pair<std::string,BuiltinFunction>("min",BuiltinFunction("min",true)));
		builtinMap.insert(std::make_pair<std::string,BuiltinFunction>("max",BuiltinFunction("max",true)));	
		builtinMap.insert(std::make_pair<std::string,BuiltinFunction>("length",BuiltinFunction("length",false)));
		builtinMap.insert(std::make_pair<std::string,BuiltinFunction>("floor",BuiltinFunction("floor",false)));
		builtinMap.insert(std::make_pair<std::string,BuiltinFunction>("size",BuiltinFunction("size",false)));
		builtinMap.insert(std::make_pair<std::string,BuiltinFunction>("colon",BuiltinFunction("colon",false)));
	
	}
	bool contains(const std::string & funcName) {
	  return  builtinMap.count(funcName) > 0;
	}
	
	BuiltinFunction getBuiltin(const std::string & funcName) {
	  if(contains(funcName)) {
	   return builtinMap.find(funcName)->second;
	  }
	  std::cout<<"Entry not found"<<std::endl;
	  return builtinMap.end()->second;
	}
}; 

/*! \class VCompiler
    \brief Main class for the static backend. 

Provides methods which generate code for each node of VRIR. It takes a pointer of type VModule which is the root of the VRIR AST.   
*/
class VCompiler {
    typedef unordered_set<int> IntegerSet;
    typedef unordered_map<IndexExprPtr,IntegerSet> IndexToIterMap;
private:
    IndexToIterMap indxToIterMap; 
	int maxTempVecId;
	int maxTempIterId;
    bool boundsCheckFlag;
	std::string tempVarStr;
	std::string tempIterStr;
    std::map<StmtPtr,LoopInfo*> infoMap;
    std::set<StmtPtr>topLoops; 
	int structVarId;
	VFunction *currFunction;
	Context headerCntxt;
	const std::string moduleName;
	BuiltinMapper mapper;
	NodeCollector collector;
    std::set<LibCallExpr::Code> libCallSet;
    VModule *currModule;
    IndexSet usedIndices;
    LoopCollector lc;
	//! Generates C++ code for a statement node. 
	/*! Calls specialised methods depending on the type of the statement
            \fn stmtTypeCodeGen
	    \param StmtPtr is a pointer to the statement node.
	    \param SymTable is a pointer to the function's symTable
	*/
	Context stmtTypeCodeGen(StmtPtr stmt, SymTable *symTable);  
    void addToIndxToIterMap(IndexExprPtr expr, int id);
	//! Generates C++ code for expression node 
	/*! Calls specialised methods depending on the type of the expression
	    \fn exprTypeCodeGen 
	    \param ExpressionPtr pointer to the expression node 
	    \param SymTable pointer to the function's symbol table 
	*/
	Context exprTypeCodeGen(ExpressionPtr expr, SymTable *symTable,ExpressionPtr lExpr = NULL);
	//! Generates C++ code for the VType . 
	/*! Calls either scalarTypeCodeGen or arrayTypeCodeGen depending on whether whether it is a ArrayType or ScalarType. 
	\param type is a pointer to a Vtype
	\param symTable pointer to the function's symbl table
	\param isPtr is passed to the scalarTypeCodegen to differentiate primitive scalar and scalars of type VrArrayPtr which are passed as parameters to the function. 
		
     */ 
    Context vTypeCodeGen(VTypePtr type, SymTable *symTable,bool isPtr);
    //! Generates C++ for scalar types. 
    /*! The type generated is dependent on the boolean flag passed. 
      The flag is set to false by default which means primitive C++ scalars are generated. VrArrayPtr is generated when the flag is set to true. 
      \param vtype is a pointer to a ScalarType object. 
      \param isPtr is a boolean which differentiates between primitive and VrArray* scalars. 
     */
    Context scalarTypeCodeGen(ScalarTypePtr vtype,bool isPtr );
    //! Generates C++ for Array Types. 
    /*! \fn arrayTypeCodeGen
     * \param type is the ArrayType pointer.
     \param symTable is a pointer to the function's symbol table. 
     \param isPtr differentiates between primitive and VrArray type arrays.   
     */
    Context arrayTypeCodeGen(ArrayTypePtr type, SymTable *symTable,bool);
    //! generates a set of scalars which are wrapped inside VrArrayPtr. 
    /*! These scalars are found in the function parameters.
      \param  func is a pointer to the current VFunction.
     */
    void genScalSet(VFunction* func);
    //! generates a string depending on the VType. For blas libraries
    /*! Blas functions for VRIR differ on depending on the returning type/input parameters.
     */ 
    string getTypeStr(VTypePtr vtype);	
    string getTypeStr(ScalarTypePtr vtype);	
    string getTypeStr(ArrayTypePtr vtype);	
    string genDataStr(VTypePtr vtype,string name);
    string genDimStr(VTypePtr vtype,string name);
    string genDataMacroStr(VTypePtr vtype,string name);
    string genDimMacroStr(VTypePtr vtype,string name);
    std::string genCblasOrder(ArrayTypePtr vtype);
    std::string genCblasTrans();
    std::string getNdims(ArrayTypePtr);
    bool enableOpenMP;
    bool enableSse;
    set<string> *scalSet;
    std::string itoa(int num);
    bool isInt(ExpressionPtr expr);
    bool isInt(ScalarTypePtr type);
    bool isMatMultCall(LibCallExprPtr expr);
    Context getOriginalArrStr(NameExprPtr expr, SymTable * symTable);
    Context handleSpecMatMultCall(LibCallExprPtr expr, SymTable *symTable,ExpressionPtr lhsExpr);
    std::string getIndexDiffStr();
    Context generateArgs(VFunction*func);
    Context generateReturnType(VFunction *func);
    Context generateTypeDec(VFunction *);
    bool requireOriginalArr(AssignStmtPtr stmt);
    bool isArrayCopy(AssignStmtPtr stmt);
    std::string genCopyExpr(std::string name , int ndims);
    std::string genCopyExpr(NameExprPtr expr,SymTable * symTable);
    std::string genIndexStr(IndexExprPtr expr, SymTable *symTable);
    bool isRowMajor(int id, SymTable *symTable);
    bool isSlice(IndexExprPtr expr);
    string genRangeFuncStr(VRange * range,SymTable * symTable);
    std::string genRangeFuncStr(ExpressionPtr expr, SymTable *symTable);
    std::string genSliceStr(IndexExprPtr expr, SymTable *symTable);
    std::string  handleArraySlicing(IndexExprPtr expr, SymTable *symTable,ExpressionPtr lhsExpr=NULL);
    bool scalarArgs(FunCallExprPtr expr);
    bool hasArrayArg(FunCallExprPtr expr);
    std::string genTempVec();
    std::string genIterVar();
    std::string genFuncStructName(VFunction * func);
    std::string genStructData();	
    std::string genConstructor(VFunction *func,SymTable *symTable);
    std::string genRetStruct(VFunction * func);	
    bool hasMultRet(VFunction* func);
    bool hasMultRet(StmtListPtr stmtList);
    std::string getBoundCheckFuncStr();
    Context genBoundCheckStmt(IndexExprPtr expr,SymTable * symTable,bool onLhs = false);
    Context genSpecialisedBoundCheck(IndexExprPtr expr, SymTable *symTable, bool onLhs);
    bool requiresCheck(IndexExprPtr expr);
    std::string genBoundCheckHeader(IndexExprPtr expr, SymTable* symTable);
    std::string genSpecNegativeIndexStr(IndexExprPtr expr, SymTable *symTable);
    bool  canSpecialiseNegativeIndex(IndexVec vec); 
    bool canSpecialiseArraySlice(IndexVec vec);
    void getIndexElimSet(ForStmtPtr stmt, SymTable *symTable,IndexSet& indexSet );
    std::string genSpecArraySliceStr(IndexExprPtr expr, SymTable *symTable,ExpressionPtr lhsExpr = NULL);
    bool isSpecSlice(AssignStmtPtr stmt);
    bool  canSpecBoundCheckStmt(IndexVec vec); 
    bool hasColon(IndexExprPtr expr);
    bool isOnLhs(IndexExprPtr expr);
    Context handleArraySliceSet(IndexExprPtr lhsExpr, ExpressionPtr expr, SymTable* symTable);
    Context handleSpecArraySliceSet(IndexExprPtr lhsExpr, ExpressionPtr expr, SymTable *symTable);
    void getLoopIndices(LoopInfo* info, SymTable *symTable,unordered_set<int> itervarSet, DomainExprPtr domain, unordered_map<IndexStruct, unordered_set<StmtPtr> >& indexToLoopMap,ForStmtPtr stmt, IndexSet& set);
    bool isValidIndex(LoopInfo::IndexInfo indexInfo, unordered_set<int> itervarSet, DomainExprPtr domain, SymTable *symTable, LoopInfo *info);
    bool isIndexAffine(IndexStruct index, LoopInfo *info, unordered_set<int> itervarSet,IndexExprPtr indxExpr);
    bool isNameExprAffine(NameExprPtr nameExpr, LoopInfo *info, unordered_set<int> itervarSet,IndexExprPtr );
    bool isPlusExprAffine(PlusExprPtr expr, LoopInfo *info, unordered_set<int> itervarSet,IndexExprPtr );
    bool isMinusExprAffine(MinusExprPtr expr, LoopInfo *info, unordered_set<int> itervarSet,IndexExprPtr  );
    bool isExprAffine(ExpressionPtr expr, LoopInfo *info, unordered_set<int> itervarSet,IndexExprPtr);
    bool isConstExprAffine(ConstExprPtr expr);
    
    bool areLoopBoundsValid(IndexExprPtr index, LoopInfo *);
    std::vector<ExpressionPtr> getLoopBoundsFromMap(int id);
public:
    void setBoundsCheckFlag(bool val) {
        boundsCheckFlag = val;
    }

    bool getBoundsCheckFlag() {
        return boundsCheckFlag;
    }
    bool isNegativeIndex( IndexExprPtr expr);
    bool isExprInvariant(ExpressionPtr expr, LoopInfo* info);
    bool isNameExprInvariant(NameExprPtr expr, LoopInfo* info);
    bool isPlusExprInvariant(PlusExprPtr expr, LoopInfo* info);
    bool isMinusExprInvariant(MinusExprPtr expr, LoopInfo* info);
    bool isConstExprInvariant(ConstExprPtr expr);
    std::string genIndexPtrFunc() const;
    std::string genIndexPtrStr(IndexExprPtr expr, SymTable *symTable);
    std::vector<string> getIndexTemps( int n );
    VCompiler():tempVarStr("vrTempVec"),tempIterStr("vrTempIter"),moduleName("default"){
        scalSet = new set<string>();
        maxTempVecId = 0;
        maxTempIterId = 0;
        structVarId = 0;
        initLibCallSet();

	}

    VCompiler(std::string moduleName):tempVarStr("vrTempVec"),tempIterStr("vrTempIter"),moduleName(moduleName){
        scalSet = new set<string>();
        maxTempVecId = 0;
        maxTempIterId = 0;
        structVarId = 0;
        initLibCallSet();
    }
    ~VCompiler(){
        delete scalSet;
    }
    typedef enum LoopDirection {
        COUNT_UP,
        COUNT_DOWN, 
        UNKNOWN
    }LoopDirection;
    bool getSseFlag();
    void setSseFlag(bool);
    bool getOpenMpFlag();
    void setOpenMpFlag(bool);
    void setCurrModule(VModule *vm){
        currModule = vm;
    }
    VModule* getCurrModule() const {
        return currModule;
    }
    Context funcCodeGen(VFunction *func);
    Context stmtCodeGen(StmtPtr stmt, SymTable *symTable);
    Context forStmtCodeGen(ForStmtPtr stmt, SymTable *symtable);
    Context loopStmtCodeGen(DomainExprPtr domainPtr, vector<int> iterVar, StmtListPtr bodyStmt, SymTable *symTable);
    Context exprStmtCodeGen(ExprStmtPtr stmt, SymTable *symtable);
    Context assignStmtCodeGen(AssignStmtPtr stmt, SymTable *symTable);
    Context nameExprCodeGen(NameExprPtr expr, SymTable *symTable);
    bool isNegativeConst(IndexStruct index);
    Context plusExprCodeGen(PlusExprPtr expr, SymTable *symTable);
    Context domainExprCodeGen(DomainExprPtr expr, SymTable *symTable);
    Context dimExprCodeGen(DimExprPtr expr, SymTable *symTable); 
    Context constExprCodeGen(ConstExprPtr expr, SymTable *symTable);
    Context stmtListCodeGen(StmtListPtr stmt, SymTable *symTable);
    Context indexExprCodeGen(IndexExprPtr expr, SymTable *symTable,ExpressionPtr lhsExpr = NULL);
    Context replaceIndexWithStart(IndexStruct index, LoopInfo *info, SymTable *table);
    Context replaceIndexWithStop(IndexStruct index, LoopInfo *info, SymTable *table);
    Context genIndexOptimCondition(IndexExprPtr expr, LoopInfo *info, SymTable *table);
    std::string genCheckOptimCondition(IndexSet & indexSet, LoopInfo *info, SymTable *table);
    std::string genCheckOptimStartFunc(IndexExprPtr expr, SymTable *symTable);
    std::string genCheckOptimStopFunc(IndexExprPtr expr,SymTable *symTable);
    Context minusExprCodeGen(MinusExprPtr expr, SymTable *symTable);
    Context matMultCallCodeGen(LibCallExprPtr expr, SymTable *symTable,ExpressionPtr lhsExpr=NULL);
    Context matPlusCallCodeGen(LibCallExprPtr expr, SymTable *symTable,ExpressionPtr lExpr=NULL);
    Context matMinusCallCodeGen(LibCallExprPtr expr, SymTable *symTable,ExpressionPtr lExpr = NULL);
    Context matLDivCallCodeGen(LibCallExprPtr expr, SymTable *symTable, ExpressionPtr lExpr = NULL);
    Context matRDivCallCodeGen(LibCallExprPtr expr,SymTable *symTable,ExpressionPtr lExpr = NULL);
    Context matTransCallCodeGen(LibCallExprPtr expr, SymTable *symTable,ExpressionPtr lExpr = NULL);
    Context elemMultCallCodeGen(LibCallExprPtr expr, SymTable *symTable,ExpressionPtr lExpr = NULL);
    Context elemDivCallCodeGen(LibCallExprPtr expr, SymTable * symTable,ExpressionPtr lExpr = NULL);
    Context returnStmtCodeGen(ReturnStmtPtr stmt, SymTable *symTable);
    Context moduleCodeGen(VModule *vm);
    Context breakStmtCodeGen(BreakStmtPtr stmt);
    Context continueStmtCodeGen(ContinueStmtPtr stmt);
    Context ifStmtCodeGen(IfStmtPtr stmt, SymTable * symTable);
    Context whileStmtCodeGen(WhileStmtPtr stmt, SymTable *symTable);
    //Context refOpStmtCodeGen(RefOpStmtPtr stmt, SymTable *symTable);
    Context pForStmtCodeGen(PforStmtPtr stmt, SymTable *symTable);
    Context allocExprCodeGen(AllocExprPtr expr, SymTable *symTable);
    Context binaryExprCodeGen(BinaryExprPtr expr, SymTable *symTable);
    Context multExprCodeGen(MultExprPtr expr, SymTable *symTable);
    Context divExprCodeGen(DivExprPtr expr, SymTable *symTable);
    Context geqExprCodeGen(GeqExprPtr expr, SymTable *symTable);
    Context eqExprCodeGen(EqExprPtr expr, SymTable *symTable);
    Context gtExprCodeGen(GtExprPtr expr, SymTable *symTable);
    Context ltExprCodeGen(LtExprPtr expr, SymTable *symTable);
    Context leqExprCodeGen(LeqExprPtr expr, SymTable *symTable);
    Context neqExprCodeGen(NeqExprPtr expr, SymTable *symTable);
    Context andExprCodeGen(AndExprPtr expr, SymTable *symTable);
    Context orExprCodeGen(OrExprPtr expr, SymTable *symTable);
    Context notExprCodeGen(NotExprPtr expr, SymTable *symTable); //to be written
    Context funCallExprCodeGen(FunCallExprPtr expr, SymTable *symTable,ExpressionPtr lExpr = NULL);
    Context libCallExprCodeGen(LibCallExprPtr expr, SymTable *symtable,ExpressionPtr lExpr = NULL);
    Context negateExprCodeGen(NegateExprPtr expr, SymTable *symTable);
    std::string getMatTypeStr(VTypePtr vtype);
    std::string generateMatClassStr(LibCallExprPtr expr, SymTable *symTable);
    std::string generateMatClassStr(VTypePtr vtype);
    std::string generateArgs(ExpressionPtrVector args,SymTable *symTable,bool checkIsInt=false, std::string cast="");
    Context catCallCodeGen(FunCallExprPtr expr, SymTable *symTable);
    std::vector<LoopDirection> getLoopDirections(DomainExprPtr expr, SymTable *symTable);
    Context handleMultReturns(ReturnStmtPtr stmt,SymTable *symTable);
    std::string genStructVarStr(VFunction *func);
    bool hasMultRet(FunCallExprPtr expr);
    Context genBoundCheckStmt(StmtPtr stmt,SymTable * symTable, bool onLhs=false);
    std::string genFuncStructName(std::string funcName);
    std::string genFuncStructName(FunCallExprPtr expr);
    std::string genStructVarStr(std::string structType);
    Context handleMultLhs(AssignStmtPtr stmt, SymTable* symtable);
    Context tupleExprCodeGen(TupleExprPtr expr, SymTable* symTable);
    bool isScalarLibCall(AssignStmtPtr stmt, SymTable *symTable);
    bool isScalarFunCall(AssignStmtPtr stmt, SymTable *symTable);
    bool isIterVar(int id);
    void genHeaderFile(VModule *vm);
    Context getHeaderContext() { return headerCntxt;}
    void setHeaderContext(Context cntxt) {headerCntxt = cntxt;}
    std::string genFuncHeader(VFunction *func);
    bool isBuiltin(const std::string & funcName);
    bool isComplex(ExpressionPtr expr);
    std::string genNegativeIndexStr(IndexExprPtr expr, SymTable *symTable);
    bool isSpecLibCall(AssignStmtPtr stmt);
    void initLibCallSet();
    Context castExprCodeGen(CastExprPtr expr,SymTable *symTable);
    Context complexExprCodeGen(ComplexExprPtr expr,SymTable *symTable);
    LoopDirection getLoopDirectionEnum(ExpressionPtr expr);
    void setCollector(NodeCollector& nc) {
        collector = nc;
    }
    NodeCollector getCollector() const {
        return collector;
    }
    bool hasIndexExpr(StmtPtr stmt);
    bool isNegativeIndex( StmtPtr stmt);
    IndexSet* getIndexSet(StmtPtr stmt);
    std::string getSpecBoundCheckFuncStr();
    std::string genSpecBoundCheckHeader(IndexExprPtr expr, SymTable* symTable);
};


}//end of VRaptor namespace 
#endif /* CODEGEN_HPP_ */
