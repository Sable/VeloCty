/*
 * codegen.cpp
 *
 *  Created on: 2013-03-15
 *      Author: Sameer Jagdale
 *
 */
///Contains Methods which generate code C++ from  the VRIR
#include <codegen.hpp>
#define MEM_OPTMISE
#ifdef MEM_OPTMISE
bool memOptmise = true;
#else 
bool memOptmise = false;
#endif
#define PRELIM_BOUNDS
#ifdef PRELIM_BOUNDS
bool prelim_bounds = true;
#else 
bool prelim_bounds = true;
#endif

using namespace VRaptor;
using namespace std;

Context::Context() {
	arrayFlag = false;
}

bool Context::isArray() {
	return arrayFlag;
}

void Context::resetArrayFlag() {
	arrayFlag = false;
}

void Context::setArrayFlag() {
	arrayFlag = true;
}
vector<string> Context::getAllStmt() {
	return list;
}

void Context::addStmt(const string stmt) {
	list.push_back(stmt);
}

void Context::addStmtVec(vector<string> vec){

	list.insert(list.end(),vec.begin(),vec.end());
}

void VCompiler::genHeaderFile(VModule *vm) {
	headerCntxt.addStmt("#include\"math.h\" \n");
	headerCntxt.addStmt("#include\"matrix_ops.hpp\"\n");
	headerCntxt.addStmt("#include\"library_ops.hpp\"\n");
	headerCntxt.addStmt("#include\"vrbinding.hpp\"\n");
	vector<VFunction*> funcList = vm->getFns();
	for(int i = 0; i<funcList.size(); i++) {
		if(funcList[i]->getName().compare("pForFunc") == 0) {
			continue;
		}
		if(hasMultRet(funcList[i])) {
			headerCntxt.addStmt(genRetStruct(funcList[i]));
		}
		
	}
	for(int i = 0; i < funcList.size(); i++) {
			if(funcList[i]->getName().compare("pForFunc") == 0) {
					continue;
			}
			headerCntxt.addStmt(genFuncHeader(funcList[i])+ ";\n");
	}
}

void VCompiler::initLibCallSet() {
    libCallSet.insert(LibCallExpr::LIB_MATMULT);
    libCallSet.insert(LibCallExpr::LIB_PLUS);
    libCallSet.insert(LibCallExpr::LIB_MINUS);
    libCallSet.insert(LibCallExpr::LIB_MULT);
    libCallSet.insert(LibCallExpr::LIB_DIV);
    libCallSet.insert(LibCallExpr::LIB_TRANS);
    /* libCallSet.insert(LibCallExpr::LIB_MLDIV); */
    /* libCallSet.insert(LibCallExpr::LIB_MRDIV); */
}
void VCompiler::setOpenMpFlag(bool val){
    enableOpenMP = true;
}
Context VCompiler::moduleCodeGen(VModule *vm) {
    Context cntxt;
    mapper.init();
    currModule = vm;
    setOpenMpFlag(true);
    cntxt.addStmt("#include \""+ moduleName+"Impl.hpp\"\n ");
    vector<VFunction*> funcList = vm->getFns();
#ifdef DEBUG
    std::cout<<"generating header file"<<std::endl;
#endif
    genHeaderFile(vm);
#ifdef DEBUG
    std::cout<<"generating code in module "<<std::endl;
#endif 
    lc.analyze(vm);
    setBoundsCheckFlag(true);
    for (int i = 0; i < funcList.size(); i++) {
        std::cout<<"Generating code for "<<funcList[i]->getName()<<std::endl;
        if(funcList[i]->getName().compare("pForFunc") == 0) {
            continue;
        }
        currFunction = funcList[i];
        LoopInfo::getLoopInfoMap(funcList[i], infoMap, topLoops);
        if(prelim_bounds) {
            VRaptor::prelimBCE(funcList[i], infoMap);
        }
        
        Context tempCntxt = funcCodeGen(funcList[i]);
        for (int j = 0; j < tempCntxt.getAllStmt().size(); j++) {
            cntxt.addStmt(tempCntxt.getAllStmt()[j]);
        }
    }
    return cntxt;
}

std::string VCompiler::genFuncStructName(VFunction * func) {
	return genFuncStructName(func->getName());
}

std::string VCompiler::genFuncStructName(std::string funcName) {
    return std::string("struct_") + funcName + "_ret";
}
std::string VCompiler::genFuncStructName(FunCallExprPtr expr) {
    return genFuncStructName(expr->getName()); 
}
std::string VCompiler::genStructData() {
	return "ret_data";
}

std::string VCompiler::genConstructor(VFunction *func,SymTable *symTable) {
	std::string structName = genFuncStructName(func);
	//generate function arguments
	std::string constructorStr = structName + "(";
	FuncTypePtr funcType = static_cast<FuncTypePtr>(func->getType());
	VTypePtr vtype = funcType->getReturnType(0);
	std::string dataType = vTypeCodeGen(vtype,symTable,false).getAllStmt()[0];
	constructorStr+= dataType + " " +genStructData() + "0";
	for(int i = 1; i < funcType->getNumReturns(); i++) {
		 vtype = funcType->getReturnType(i);
		 dataType = vTypeCodeGen(vtype,symTable,false).getAllStmt()[0];
		constructorStr+= ","+dataType + " "+genStructData() + itoa(i);
	}
	constructorStr+=") :";
	//generate initialisation list.
	for( int i = 0; i < funcType->getNumReturns(); i++) {
		if(i>0){
			constructorStr+=",";
		}
		std::string dataStr = genStructData() + itoa(i);
		constructorStr+= dataStr + "(" + dataStr + ")";
	}
	constructorStr += "\n{\n}\n";
	return constructorStr;
}

std::string VCompiler::genRetStruct(VFunction * func) {
	std::string struct_def = "typedef struct ";
	SymTable *symTable=func->getSymTable();
	std::string structName = genFuncStructName(func);
	struct_def += structName;
	struct_def += " { \n";
	FuncTypePtr funcType = static_cast<FuncTypePtr>(func->getType());
	for(int i = 0; i < funcType->getNumReturns(); i++) {
		VTypePtr vtype = funcType->getReturnType(i);
		std::string dataType = vTypeCodeGen(vtype,symTable,false).getAllStmt()[0];
		struct_def+= dataType + " "+genStructData() + itoa(i) + ";\n";
	} 
	struct_def += "\n";
	struct_def +=genConstructor(func,symTable)+"\n";
	 struct_def +="}"+structName+";\n";
	return struct_def;
}
bool VCompiler::hasMultRet(VFunction* func) {
	return hasMultRet(func->getBody());
}
 
bool VCompiler::hasMultRet(StmtListPtr stmtList) {
	for(int i = 0; i < stmtList->getNumChildren(); i++) {
		if(stmtList->getChild(i)->getStmtType()==Statement::STMT_RETURN) {
			ReturnStmtPtr returnStmt = static_cast<ReturnStmtPtr>(stmtList->getChild(i));
			if(returnStmt->getExprs().size()>1){
				return true;
			}
		}
	}
	return false;
} 

bool VCompiler::hasMultRet(FunCallExprPtr expr) {
    if(expr->getType()->getBasicType() == VType::TUPLE_TYPE) {
        TupleTypePtr tupleType = static_cast<TupleTypePtr>(expr->getType());
        if(tupleType->getNumElements()>1) {
            return true;
        }
    }
    return false;
}

Context VCompiler::scalarTypeCodeGen(ScalarTypePtr vtype,bool isPtr=false) {
	Context cntxt;
	if(!isPtr){
	        if(vtype->getComplexStatus()==ScalarType::REAL) {
		  switch (vtype->getScalarTag()) {
		  case ScalarType::SCALAR_INT32:
			  cntxt.addStmt("int");
			  break;
		  case ScalarType::SCALAR_FLOAT32:
			  cntxt.addStmt("float");
			  break;
		  case ScalarType::SCALAR_BOOL:
			  cntxt.addStmt("bool");
			  break;
		  case ScalarType::SCALAR_FLOAT64:
			  cntxt.addStmt("double");
		  case ScalarType::SCALAR_INT64:
			  cntxt.addStmt("long");
		  default:
			  cntxt.addStmt("void");
		   }
		}
		else {//if(vtype->getComplexStatus()==ScalarType::COMPLEX){
			switch (vtype->getScalarTag()) {
        		case ScalarType::SCALAR_INT32:
				std::cout<<"Complex Integers not currently supported"
				<<std::endl;
                		exit(0);
                		
        		case ScalarType::SCALAR_FLOAT32:
                		cntxt.addStmt("float complex");
                		break;
        		case ScalarType::SCALAR_BOOL:
                		std::cout<<"Complex Integers not currently supported"
				<<std::endl;
                		exit(0);
        		case ScalarType::SCALAR_FLOAT64:
                		cntxt.addStmt("double complex");
				break;
        		case ScalarType::SCALAR_INT64:
               			std::cout<<"Complex Integers not currently supported"
				<<std::endl;
                		exit(0);
       			default:
                		cntxt.addStmt("void*");
        		}
		}
	}
	else{
		if(vtype->getComplexStatus()==ScalarType::REAL){
			switch (vtype->getScalarTag()) {
        		case ScalarType::SCALAR_INT32:
                		cntxt.addStmt("VrArrayPtrI32");
                		break;
        		case ScalarType::SCALAR_FLOAT32:
                		cntxt.addStmt("VrArrayPtrF32");
                		break;
        		case ScalarType::SCALAR_BOOL:
                		cntxt.addStmt("VrArrayPtrB");
                		break;
        		case ScalarType::SCALAR_FLOAT64:
                		cntxt.addStmt("VrArrayPtrF64");
				break;
        		case ScalarType::SCALAR_INT64:
               			cntxt.addStmt("VrArrayPtrI64");
				break;
       			default:
                		cntxt.addStmt("void*");
        		}
		}
		else if(vtype->getComplexStatus()==ScalarType::COMPLEX){
			switch (vtype->getScalarTag()) {
        		case ScalarType::SCALAR_INT32:
                		cntxt.addStmt("VrArrayPtrCI32");
                		break;
        		case ScalarType::SCALAR_FLOAT32:
                		cntxt.addStmt("VrArrayPtrCF32");
                		break;
        		case ScalarType::SCALAR_BOOL:
                		cntxt.addStmt("VrArrayPtrCB");
                		break;
        		case ScalarType::SCALAR_FLOAT64:
                		cntxt.addStmt("VrArrayPtrCF64");
				break;
        		case ScalarType::SCALAR_INT64:
               			cntxt.addStmt("VrArrayPtrCI64");
				break;
       			default:
                		cntxt.addStmt("void*");
        		}
		}
		else{
	
			std::cout<<"may be complex"<<std::endl;
		}
		
	}
	
	return cntxt;
}

Context VCompiler::arrayTypeCodeGen(ArrayTypePtr type, SymTable *symTable,bool isvrPtr=true) {
	Context cntxt;
	if(!isvrPtr){
		switch(type->getElementType()->getScalarTag()){
			case ScalarType::SCALAR_INT32:
				cntxt.addStmt("int*");
				break;
			case ScalarType::SCALAR_INT64:
				cntxt.addStmt("long*");
				break;
			case ScalarType::SCALAR_FLOAT32:
				cntxt.addStmt("float*");
				break;
			case ScalarType::SCALAR_FLOAT64:
				cntxt.addStmt("double*");
				break;
			case ScalarType::SCALAR_BOOL:
				cntxt.addStmt("bool*");
				break;
			
		}
	}
	else{
		if(type->getElementType()->getComplexStatus()==ScalarType::REAL){
			switch(type->getElementType()->getScalarTag()){
				case ScalarType::SCALAR_INT32 : 
					cntxt.addStmt("VrArrayPtrI32");
					break;
				case ScalarType::SCALAR_INT64 :
					cntxt.addStmt("VrArrayPtrI64");
					break;
				case ScalarType::SCALAR_FLOAT32 :
					cntxt.addStmt("VrArrayPtrF32");
					break;
				case ScalarType::SCALAR_FLOAT64:
					cntxt.addStmt("VrArrayPtrF64");	
					break;
				case ScalarType::SCALAR_BOOL: 
					cntxt.addStmt("VrArrayPtrB");
					break;
				default :
#ifdef DEBUG
					std::cout<<"scalar type is not valid"<<std::endl;
#endif	
					break;
			}
		}
		else if(type->getElementType()->getComplexStatus()==ScalarType::COMPLEX){
			switch(type->getElementType()->getScalarTag()){
				case ScalarType::SCALAR_INT32 : 
					cntxt.addStmt("VrArrayPtrCI32");
					break;
				case ScalarType::SCALAR_INT64 :
					cntxt.addStmt("VrArrayPtrCI64");
					break;
				case ScalarType::SCALAR_FLOAT32 :
					cntxt.addStmt("VrArrayPtrCF32");
					break;
				case ScalarType::SCALAR_FLOAT64:
					cntxt.addStmt("VrArrayPtrCF64");	
					break;
				case ScalarType::SCALAR_BOOL: 
					cntxt.addStmt("VrArrayPtrCB");
					break;
				default :
#ifdef DEBUG
					std::cout<<"scalar type is not valid"<<std::endl;
#endif
					break;
			}
		}
		else{
	   		 std::cout<<"may be complex"<<std::endl;
		}
	}
	return cntxt;
}

Context VCompiler::vTypeCodeGen(VTypePtr vType, SymTable *symTable,bool isPtr=false) {
	Context cntxt;
	
	switch (vType->getBasicType()) {
	case VType::SCALAR_TYPE:
		cntxt = scalarTypeCodeGen(static_cast<ScalarTypePtr>(vType),isPtr);
		break;
	case VType::DOMAIN_TYPE:
	#ifdef DEBUG
		cout << "cannot generate code for domain type" << endl;
	#endif	
		break;
	case VType::ARRAY_TYPE:

		cntxt = arrayTypeCodeGen(static_cast<ArrayTypePtr>(vType) , symTable);

		break;
	case VType::UNIT_TYPE:
		cntxt.addStmt("void");

		break;
	default:
	#ifdef DEBUG
		std::cout<<"Entering default case in vTypeCodeGen"<<std::endl;
	#endif	
		cntxt.addStmt("void");
		break;
	}
	//return "";
	return cntxt;
}
void VCompiler::genScalSet(VFunction* func){
	for(int i=0;i<func->m_args.size();i++){
		int id = func->m_args[i].m_id;
		if(func->getSymTable()->getType(id)->getBasicType()==VType::SCALAR_TYPE){
			scalSet->insert(func->getSymTable()->getName(id));
		}
	}
}

Context VCompiler::generateArgs(VFunction *func){
 Context cntxt;
  if (func->m_args.size() > 0) {
		int id = func->m_args[0].m_id;
		std::string argName = func->getSymTable()->getName(id);

		Context argCntxt = vTypeCodeGen(func->getSymTable()->getType(id),
				func->getSymTable(),(func->getSymTable()->getType(id)->getBasicType()==VType::SCALAR_TYPE) && (scalSet->find(argName)==scalSet->end()?false:true));

		if (argCntxt.getAllStmt().size() <= 0) {
#ifdef DEBUG
			cout << "error while fetching arguments " << endl;
#endif
			return cntxt;
		}
		string argType = argCntxt.getAllStmt()[0];
		string str = argType + " " + argName;
		//if argument is an arraycntxt.addStmt(tempCntxt.getAllStmt()[0] + "*");
	for (int i = 1; i < func->m_args.size(); i++) {
		id = func->m_args[i].m_id;
		argName = func->getSymTable()->getName(id);
		argCntxt = vTypeCodeGen(func->getSymTable()->getType(id),
				func->getSymTable(),(func->getSymTable()->getType(id)->getBasicType()==VType::SCALAR_TYPE) && (scalSet->find(argName)==scalSet->end()?false:true));
		argType = argCntxt.getAllStmt()[0];
		str += "," + argType + " " + argName;

	}
	 cntxt.addStmt(str);
  }	
	 return cntxt;
}

Context VCompiler::generateReturnType(VFunction *func){
	Context cntxt;
	FuncTypePtr funcType = static_cast<FuncTypePtr>(func->getType());
	//TODO : Deal with multiple return types
	if(funcType->getNumReturns()>1) {
	  cntxt.addStmt(genFuncStructName(func));
	  return cntxt;
	}
	VTypePtr returnType = funcType->getReturnType(0);
	string ret;
	Context retCntxt = vTypeCodeGen(returnType, func->getSymTable());
	//function signature
	if (retCntxt.getAllStmt().size() > 0) {
		ret = retCntxt.getAllStmt()[0];
	} else {
		ret = "void";
	}
	cntxt.addStmt(ret);
	return cntxt;
}

Context VCompiler::generateTypeDec(VFunction *func){
    Context cntxt;
    SymTable *symTable = func->getSymTable();
	vector<int> idVec = symTable->getAllIds();
	for (int i = 0; i < idVec.size(); i++) {
		int j;
		for (j = 0; j < func->m_args.size(); j++) {
			if (idVec[i] == func->m_args[j].m_id) {
				break;
			}

		}
		if (j == func->m_args.size()) {
			VTypePtr vtype = symTable->getType(idVec[i]);
			Context varCntxt = vTypeCodeGen(vtype, func->getSymTable());
			string varType = varCntxt.getAllStmt()[0];
			string varName = symTable->getName(idVec[i]);
			std::string declStr;
			if(vtype->getBasicType() == VType::ARRAY_TYPE) {
				declStr = varType + " " + varName + ";\n";
			}
			else{
				declStr = varType + " " + varName + ";\n";
			}
			cntxt.addStmt(declStr);
			if(vtype->getBasicType()==VType::SCALAR_TYPE 
			        && !(scalSet->find(varName)==scalSet->end()) ){		  
			  Context tempCntxt = scalarTypeCodeGen(static_cast<ScalarTypePtr>(vtype));
			  string name = symTable->getName(idVec[i]);
			  cntxt.addStmt(
					tempCntxt.getAllStmt()[0] + " " + varName + "_data;\n");
		      }
		}
	}
	//array data
	ostringstream convert;
	for (int i = 0; i < func->getNumArgs(); i++) {
		VTypePtr vtype = symTable->getType(idVec[i]);
		string name = symTable->getName(idVec[i]);
		if(vtype->getBasicType()==VType::SCALAR_TYPE 
		  && !(scalSet->find(name)==scalSet->end()) ){
			  Context tempCntxt = scalarTypeCodeGen(static_cast<ScalarTypePtr>(vtype));
			  string name = symTable->getName(idVec[i]);
			  cntxt.addStmt(
					tempCntxt.getAllStmt()[0] + " " + name + "_data=  *("+genDataMacroStr(vtype,name)+ ");\n");
		}
	}
	return cntxt;
}

std::string VCompiler::genFuncHeader(VFunction *func) {
	string str(generateReturnType(func).getAllStmt()[0] + " "+func->getName() + "(");
	str += generateArgs(func).getAllStmt()[0]+")";
	return str;
}

Context VCompiler::funcCodeGen(VFunction *func) {
	Context cntxt;
	Context argCntxt;
	// generating function header
	std::string str = genFuncHeader(func);
	str +="{ \n";
	cntxt.addStmt(str);
	//variable declarations
	cntxt.addStmtVec(generateTypeDec(func).getAllStmt());
	//function body
	StmtListPtr stmtList = func->getBody();
	for (int i = 0; i < stmtList->getNumChildren(); i++) {
		StmtPtr stmt = stmtList->getChild(i);
		Context tempCntxt = stmtCodeGen(stmt, func->getSymTable());
		vector<string> tempVector = tempCntxt.getAllStmt();
		for (int j = 0; j < tempVector.size(); j++) {
			cntxt.addStmt(tempVector[j]);
		}
	}
	cntxt.addStmt("}\n\n");
	return cntxt;
}

Context VCompiler::stmtCodeGen(StmtPtr stmt, SymTable *symTable) {
	Context cntxt;
	cntxt = stmtTypeCodeGen(stmt, symTable);
	return cntxt;
}

Context VCompiler::pForStmtCodeGen(PforStmtPtr stmt, SymTable *symTable) {
    Context cntxt;
	std::string ompStr = "";
	if(getOpenMpFlag()) {
		ompStr = "#pragma omp parallel for ";
		std::set<int> sharedSet = stmt->getShared();
		if( sharedSet.size() > 0){
				ompStr+= "shared(";
				for(set<int>::iterator it = sharedSet.begin(); it != sharedSet.end(); it++) {
					std::string varName = symTable->getName(*it);
					if(it != sharedSet.begin()){
							ompStr += ", ";
					}
					ompStr += varName;
				}
				ompStr +=")";
		}
		ompStr +="\n";
	}	
	cntxt.addStmt(ompStr);
	StmtPtr sPtr = stmt->getBody();
	StmtListPtr  bodyStmt = static_cast<StmtListPtr> (sPtr);
	ExpressionPtr domainPtr = stmt->getDomain();
	vector<int> iterVar = stmt->getIterVars();
    Context loopCntxt = loopStmtCodeGen(static_cast<DomainExprPtr>(domainPtr),iterVar, bodyStmt, symTable); 
    cntxt.addStmtVec(loopCntxt.getAllStmt());
	return cntxt;
}
std::string VCompiler::genStructVarStr(VFunction *func) {
		return genStructVarStr(genFuncStructName(func));
}

std::string VCompiler::genStructVarStr(std::string structType) {
  return std::string("var_") +structType+itoa(structVarId++);  
}

Context VCompiler::handleMultReturns(ReturnStmtPtr stmt,SymTable *symTable) {
  Context cntxt;
  std::string structName = genFuncStructName(currFunction);
  //std::string structVar = genStructVarStr(currFunction);
  std::string structDecl = structName  + "(";
//  cntxt.addStmt(structDecl);
  ExpressionPtrVector exprVec = stmt->getExprs(); 
  vector<int> rIdVec = stmt->getRids();
  for(int i = 0; i < rIdVec.size(); i++) {
    std::string exprStr = symTable->getName(rIdVec[i]);//exprTypeCodeGen(exprVec[i],symTable).getAllStmt()[0];
	structDecl += exprStr;
	if( (i + 1) != exprVec.size()) {
		structDecl += ",";
	}	
    //string structAssgn = structVar + "." + genStructData() + itoa(i) + " = " 
	//		 + exprStr + ";\n";
    //cntxt.addStmt(structAssgn);
  }
  structDecl +=")";
  cntxt.addStmt("return " + structDecl+";\n");
  return cntxt;
}

Context VCompiler::returnStmtCodeGen(ReturnStmtPtr stmt, SymTable *symTable) {
	Context cntxt;
	ExpressionPtrVector exprVec = stmt->getExprs();
    vector<int> rIdVec = stmt->getRids();
	string retStr = "return ";
	if (rIdVec.size() > 0) {
        if(rIdVec.size() == 1) {
            retStr += symTable->getName(rIdVec[0]);//exprTypeCodeGen(exprVec[0],symTable).getAllStmt()[0];
        }
		/* std::cout<<"Return variable name "<<retStr<<std::endl; */
		if (exprVec.size() > 1) {
			//cout << "compiler does not handle multiple returns" << endl;
			return handleMultReturns(stmt,symTable);
		}
	}
	cntxt.addStmt(retStr + ";\n");
	return cntxt;
}

Context VCompiler::breakStmtCodeGen(BreakStmtPtr stmt) {
	Context cntxt;
	cntxt.addStmt("break;\n");
	return cntxt;
}

Context VCompiler::continueStmtCodeGen(ContinueStmtPtr stmt) {
	Context cntxt;
	cntxt.addStmt("continue;\n");
	return cntxt;
}

Context VCompiler::ifStmtCodeGen(IfStmtPtr stmt, SymTable *symTable) {
	Context cntxt;
	// condition
	ExpressionPtr expr = stmt->getCond();
	Context condCntxt = exprTypeCodeGen(expr, symTable);
	cntxt.addStmt("if(" + condCntxt.getAllStmt()[0] + ")\n");
	cntxt.addStmt("{\n");
	// if block
	StmtPtr ifBlockStmt = stmt->getIfBranch();
	Context ifCntxt = stmtTypeCodeGen(ifBlockStmt, symTable);
    std::cout<<"if statement size "<<ifCntxt.getAllStmt().size()<<std::endl;
	for (int i = 0; i < ifCntxt.getAllStmt().size(); i++) {
		cntxt.addStmt(ifCntxt.getAllStmt()[i]);
	}
	cntxt.addStmt("}\n");
	//else block
	if (stmt->hasElseBranch()) {
		cntxt.addStmt("else\n");
		cntxt.addStmt("{\n");
		StmtPtr elseBlockStmt = stmt->getElseBranch();
		Context elseCntxt = stmtTypeCodeGen(elseBlockStmt, symTable);
		for (int i = 0; i < elseCntxt.getAllStmt().size(); i++) {
			cntxt.addStmt(elseCntxt.getAllStmt()[i]);
		}
		cntxt.addStmt("}\n");
	}
	return cntxt;
}

Context VCompiler::whileStmtCodeGen(WhileStmtPtr stmt, SymTable *symTable) {
	Context cntxt;
	//condition
	ExpressionPtr condExpr = stmt->getCond();
	Context condCntxt = exprTypeCodeGen(condExpr, symTable);
	cntxt.addStmt("while(" + condCntxt.getAllStmt()[0] + ")\n");
	cntxt.addStmt("{\n");
	//body
	StmtPtr bodyStmt = stmt->getBody();
	Context bodyCntxt = stmtTypeCodeGen(bodyStmt, symTable);
	for (int i = 0; i < bodyCntxt.getAllStmt().size(); i++) {
		cntxt.addStmt(bodyCntxt.getAllStmt()[i]);
	}
	cntxt.addStmt("}\n");
	return cntxt;
}
Context VCompiler::stmtTypeCodeGen(StmtPtr stmt, SymTable *symTable) {
	Context cntxt;
	switch (stmt->getStmtType()) {
	case Statement::STMT_ASSIGN: //Assignment Statement
		cntxt = assignStmtCodeGen(static_cast<AssignStmtPtr> (stmt), symTable);
		break;
	case Statement::STMT_IF: // IF statement
		cntxt = ifStmtCodeGen(static_cast<IfStmtPtr> (stmt), symTable);
		break;
	case Statement::STMT_WHILE: // while statement 
		cntxt = whileStmtCodeGen(static_cast<WhileStmtPtr> (stmt), symTable);
		break;
	case Statement::STMT_FOR: // for statement
		cntxt = forStmtCodeGen(static_cast<ForStmtPtr>(stmt), symTable);
		break;
	case Statement::STMT_PFOR: //parallel for
		cntxt = pForStmtCodeGen(static_cast<PforStmtPtr>(stmt), symTable);
		break;
	case Statement::STMT_LIST: // list statement

		cntxt = stmtListCodeGen(static_cast<StmtListPtr>(stmt) , symTable);
		break;
	case Statement::STMT_BREAK: //break statement
		cntxt.addStmt("break; \n");	
		break;
	case Statement::STMT_CONTINUE: // continue statement
		cntxt = continueStmtCodeGen(static_cast<ContinueStmtPtr> (stmt));
		break;
	case Statement::STMT_RETURN : //return
		cntxt = returnStmtCodeGen(static_cast<ReturnStmtPtr> (stmt), symTable);
		break;
    case Statement::STMT_EXPRESSION :
         cntxt = exprStmtCodeGen(static_cast<ExprStmtPtr>(stmt), symTable);
         break;
	default:
		break;
	}
	return cntxt;
}

Context VCompiler::exprStmtCodeGen(ExprStmtPtr stmt, SymTable* symTable) {
    Context cntxt;
    cntxt.addStmt(exprTypeCodeGen(stmt->getExpr(), symTable).getAllStmt()[0] + ";\n");
    return cntxt;
}

Context VCompiler::eqExprCodeGen(EqExprPtr expr, SymTable *symTable){
	Context cntxt;
	Context tempCntxt= binaryExprCodeGen(static_cast<BinaryExprPtr>(expr),symTable);
	string lstr = tempCntxt.getAllStmt()[0];
	string rstr = tempCntxt.getAllStmt()[1];
	cntxt.addStmt(lstr+" == "+rstr);
	return cntxt;
			
}

Context VCompiler::exprTypeCodeGen(ExpressionPtr expr, SymTable *symTable,ExpressionPtr lExpr) {
	Context cntxt;
    if(expr == NULL) {
        std::cout<<"Expression cannot be NULL"<<std::endl;
        exit(0);
    }
	switch (expr->getExprType()) {
	case Expression::CONST_EXPR: // CONST_EXPR
		cntxt = constExprCodeGen(static_cast<ConstExprPtr>(expr), symTable);
		break;
	case Expression::NAME_EXPR: //Name expression
		cntxt = nameExprCodeGen(static_cast<NameExprPtr>(expr), symTable);
		break;
	case Expression::PLUS_EXPR: //Plus expression
		cntxt = plusExprCodeGen(static_cast<PlusExprPtr>(expr), symTable);
		break;
	case Expression::MINUS_EXPR: //minus expression
		cntxt = minusExprCodeGen(static_cast<MinusExprPtr>(expr), symTable);
		break;
	case Expression::MULT_EXPR: //mult expr
		cntxt = multExprCodeGen(static_cast<MultExprPtr>(expr), symTable);
		break;
	case Expression::DIV_EXPR: // division expr
		cntxt = divExprCodeGen(static_cast<DivExprPtr>(expr), symTable);
		break;
	case Expression::INDEX_EXPR: //index expr
		cntxt = indexExprCodeGen(static_cast<IndexExprPtr>(expr), symTable,lExpr);
		break;
	case Expression::NEGATE_EXPR: //Negate expr
		cntxt = negateExprCodeGen(static_cast<NegateExprPtr>(expr), symTable);
		break;
	case Expression::GT_EXPR:  // greater than
		cntxt = gtExprCodeGen(static_cast<GtExprPtr>(expr), symTable);
		break;
	case Expression::GEQ_EXPR: //greater than equal to
		cntxt = geqExprCodeGen(static_cast<GeqExprPtr>(expr), symTable);
		break;
	case Expression::LT_EXPR: // less than
		cntxt = ltExprCodeGen(static_cast<LtExprPtr> (expr), symTable);
		break;
	case Expression::NEQ_EXPR :
		cntxt = neqExprCodeGen(static_cast<NeqExprPtr>(expr), symTable);
		break;
	case Expression::LEQ_EXPR: // less than equal to
		cntxt = leqExprCodeGen(static_cast<LeqExprPtr>(expr), symTable);
		break;
	case Expression::AND_EXPR: // and expression
		cntxt = andExprCodeGen(static_cast<AndExprPtr> (expr), symTable);
		break;
	case Expression::OR_EXPR: // or expression
		cntxt = orExprCodeGen(static_cast<OrExprPtr> (expr), symTable);
		break;
	case Expression::NOT_EXPR: // not expression
		cntxt = notExprCodeGen(static_cast<NotExprPtr>( expr), symTable);
		break;
	case Expression::EQ_EXPR: //equal expression
		cntxt= eqExprCodeGen(static_cast<EqExprPtr>(expr),symTable);
		break;
	case Expression::FUNCALL_EXPR: // function call expression
		cntxt=funCallExprCodeGen(static_cast<FunCallExprPtr>(expr),symTable,lExpr);
		break;
	case Expression::DOMAIN_EXPR: // domain expression
		cntxt = domainExprCodeGen(static_cast<DomainExprPtr>(expr), symTable);
		break;
	case Expression::DIM_EXPR:  //dim expression
        cntxt = dimExprCodeGen(static_cast<DimExprPtr>(expr),symTable);
		break;
	case Expression::DIMVEC_EXPR: // dim vector
		break;
	case Expression::LIBCALL_EXPR: // library call expression 
		cntxt = libCallExprCodeGen(static_cast<LibCallExprPtr> (expr), symTable,lExpr); 
		break; 
	case Expression::TUPLE_EXPR:
		cntxt = tupleExprCodeGen(static_cast<TupleExprPtr>(expr),symTable);
		break;
    case Expression::ALLOC_EXPR: // alloc expression
		cntxt=allocExprCodeGen(static_cast<AllocExprPtr>(expr),symTable);
		break;
	case Expression::IMAG_EXPR : // imaginary expression
        std::cout<<"This is an issue"<<std::endl;
        exit(0);
		break;
	case Expression::REAL_EXPR: // real expressions
		break;
    case Expression::COMPLEX_EXPR :
        cntxt = complexExprCodeGen(static_cast<ComplexExprPtr>(expr), symTable); 
        break;            
	case Expression::CAST_EXPR: // cast expression
        cntxt = castExprCodeGen(static_cast<CastExprPtr>(expr),symTable); 
		break;
	default:
        std::cout<<"Expression not supported "<<expr->getExprType()<<std::endl;
        exit(0);
		break;

	}
	return cntxt;
}

Context VCompiler::complexExprCodeGen(ComplexExprPtr expr, SymTable *symTable) {
   Context cntxt;
    std::string realExprStr = exprTypeCodeGen(expr->getReal(),symTable).getAllStmt()[0]; 
    std::string imagExprStr = exprTypeCodeGen(expr->getImag(),symTable).getAllStmt()[0];
    cntxt.addStmt("(" + realExprStr + " + " + imagExprStr+ " * I )");
    return cntxt;
}

Context VCompiler::castExprCodeGen(CastExprPtr expr, SymTable *symTable) {
    Context cntxt;
    cntxt.addStmt("static_cast<"+vTypeCodeGen(expr->getType(),symTable).getAllStmt()[0] + ">("+exprTypeCodeGen(expr->getBaseExpr(),symTable).getAllStmt()[0] +")");
    return cntxt;
}
Context VCompiler::dimExprCodeGen(DimExprPtr expr, SymTable *symTable) {
    Context cntxt;
     std::string outStr = "size";
     int id  = expr->getArray();
     outStr += "(" + symTable->getName(id)+ ",";
     outStr += itoa(expr->getDimId()) + ")";
     cntxt.addStmt(outStr);
    return cntxt;
}

Context VCompiler::tupleExprCodeGen(TupleExprPtr expr, SymTable* symTable) {
    Context cntxt;
    int ndims = expr->getNdims();
    for(int i = 0; i < ndims; i++) {
        Context tempCntxt = exprTypeCodeGen(expr->getChild(i), symTable);
        std::vector<string> vec = tempCntxt.getAllStmt();
        for(int j = 0; j < vec.size(); j++) {
            cntxt.addStmt(vec[j]);
        }
    }
    return cntxt;
}
Context VCompiler::allocExprCodeGen(AllocExprPtr expr, SymTable* symTable){
    Context cntxt;
    std::string typeStr = "";
    if(expr->getType()->getBasicType() == VType::ARRAY_TYPE) {
        typeStr = scalarTypeCodeGen(static_cast<ArrayTypePtr>(expr->getType())->getElementType()).getAllStmt()[0];
    } 
    switch(expr->getTag()){
        case AllocExpr::ALLOC_ZEROS:
            {
                string args=generateArgs(expr->getArgs(),symTable,true,"int");
                cntxt.addStmt("zeros_"+typeStr+"("+itoa(expr->getNargs())+","+args+")");
                break;
            }
        case AllocExpr::ALLOC_ONES:
            cntxt.addStmt("ones_"+ typeStr +"("+itoa(expr->getNargs())+","+generateArgs(expr->getArgs(),symTable,true,"int")+")");
            break;
        case AllocExpr::ALLOC_EMPTY:
            cntxt.addStmt("empty_"+ typeStr +"("+itoa(expr->getNargs())+","+generateArgs(expr->getArgs(),symTable,true,"int")+")");
            break;
        default:
#ifdef DEBUG
            std::cout<<"default case in allocExprCodeGen"<<std::endl;
#endif	
            break;
    }	
    return cntxt;
}
Context VCompiler::libCallExprCodeGen(LibCallExprPtr expr, SymTable *symTable,ExpressionPtr lExpr) {
    Context cntxt;
    string funcName;
    switch (expr->getLibFunType()) {
        case LibCallExpr::LIB_SQRT:
            funcName = "sqrt";
            break;
        case LibCallExpr::LIB_LOG2:
            funcName = "log2";
            break;
        case LibCallExpr::LIB_LOG10:
            funcName = "log10";
            break;
        case LibCallExpr::LIB_LOGE:
            funcName = "log";
            break;
        case LibCallExpr::LIB_EXPE:
            if(isComplex(expr)){
                funcName = "cexp";
            }
            else{
                funcName = "exp";
            }
            break;
        case LibCallExpr::LIB_EXP10:
            funcName = "exp10";
            break;
        case LibCallExpr::LIB_SIN:
            funcName = "sin";
            break;
        case LibCallExpr::LIB_COS:
            funcName = "cos";
            break;
        case LibCallExpr::LIB_TAN:
            funcName = "tan";
            break;
        case LibCallExpr::LIB_ASIN:
            funcName = "asin";
            break;
        case LibCallExpr::LIB_ACOS:
            funcName = "acos";
            break;
        case LibCallExpr::LIB_ATAN:
            funcName = "atan";
            break;
        case LibCallExpr::LIB_POW:
            funcName="pow";
            break;
        case LibCallExpr::LIB_ABS: 
            {
                VTypePtr type = expr->getArg(0)->getType(); 
                if(type->getBasicType()==VType::SCALAR_TYPE 
                        && (static_cast<ScalarTypePtr>(type)->getScalarTag() == ScalarType::SCALAR_FLOAT64 
                            || static_cast<ScalarTypePtr>(type)->getScalarTag() == ScalarType::SCALAR_FLOAT32)){
                    if(isComplex(expr->getArg(0))) {
                        funcName = "cabs";
                    }else{
                        funcName = "fabs";
                    }
                }
                else {
                    funcName = "abs";
                }
                break;
            }
        case LibCallExpr::LIB_MIN :
            {
                funcName = "min";
                break;
            }
        case LibCallExpr::LIB_MAX :
            {
                funcName = "max";
                break;
            }
        case LibCallExpr::LIB_MATMULT:
            return matMultCallCodeGen(expr,symTable,lExpr);
            break;
        case LibCallExpr::LIB_PLUS:
            return matPlusCallCodeGen(expr,symTable,lExpr);
            break;
        case LibCallExpr::LIB_MINUS:
            return matMinusCallCodeGen(expr,symTable,lExpr);
            break;
        case LibCallExpr::LIB_MULT:
            return elemMultCallCodeGen(expr,symTable,lExpr);
            break;
        case LibCallExpr::LIB_DIV:
            return elemDivCallCodeGen(expr, symTable,lExpr);
            break;
        case LibCallExpr::LIB_TRANS:
            return matTransCallCodeGen(expr,symTable,lExpr);
            break;
        case LibCallExpr::LIB_MLDIV : 
            return matLDivCallCodeGen(expr, symTable,lExpr); 
        case LibCallExpr::LIB_MRDIV : 
            return matRDivCallCodeGen(expr,symTable,lExpr); 
           
        default:
            cout << "error in library call expression \n function not found"<<expr->getLibFunType()
                <<"Exiting"<<std::endl;
            exit(0);
            break;

	}
	Context tempCntxt; 
	std::string fnName = funcName;
	funcName="(";
	if(expr->getNargs()>0){
		tempCntxt = exprTypeCodeGen(expr->getArg(0), symTable);
		if (tempCntxt.getAllStmt().size() > 0) {
                	funcName += tempCntxt.getAllStmt()[0];
        	}
		for (int i = 1; i<expr->getNargs(); i++) {
			tempCntxt = exprTypeCodeGen(expr->getArg(i), symTable);
			if (tempCntxt.getAllStmt().size() > 0) {
			funcName += ("," + tempCntxt.getAllStmt()[0]);
			}
		}
	}
	funcName += ")";
	if(mapper.contains(fnName)) {
	  if (expr->getType()->getBasicType()==VType::SCALAR_TYPE && 
	    mapper.getBuiltin(fnName).isArrayOutput()) {
		  if(mapper.getBuiltin(fnName).isScalOut()){
		  	if(expr->getArg(0)->getType()->getBasicType() == VType::ARRAY_TYPE) {
                fnName += "_scalar";
			}
		  } else { 
            fnName += "_scalar";
		  }
	  }
	}
	cntxt.addStmt(fnName +funcName);	
	return cntxt;
	
}
std::string VCompiler::getMatTypeStr(VTypePtr vtype){
  ScalarTypePtr scalarType;
  if(vtype->getBasicType()==VType::ARRAY_TYPE){
    
      scalarType= static_cast<ArrayTypePtr>(vtype)->getElementType();
    }
    else{
      scalarType=static_cast<ScalarTypePtr>(vtype);
      
    }
    std::string matTypeStr="";
    switch(scalarType->getScalarTag()){
    
      case ScalarType::SCALAR_INT32 :
	    matTypeStr="Int";
        break;
      case ScalarType::SCALAR_INT64:
	    matTypeStr="Long";
        break;
      case ScalarType::SCALAR_FLOAT32:
	    matTypeStr="Single";
        break;
      case ScalarType::SCALAR_FLOAT64:
	    matTypeStr="Double";
        break;
      default:
#ifdef DEBUG
	    std::cout<<"Entering default case in function getMatTypeStr"<<std::endl;
#endif
	    matTypeStr = "Double";
    }
    if(scalarType->getComplexStatus()==ScalarType::COMPLEX) {
    	matTypeStr = "Complex"+ matTypeStr;
    }
     return matTypeStr;
}

bool VCompiler::isComplex(ExpressionPtr expr) {
  VTypePtr vtype = expr->getType();
  ScalarTypePtr scal;
  if(vtype->getBasicType() == VType::SCALAR_TYPE) {
    scal = static_cast<ScalarTypePtr>(vtype);
  }else if(vtype->getBasicType() == VType::ARRAY_TYPE) {
    scal = static_cast<ArrayTypePtr>(vtype)->getElementType();
  }else{
    std::cout<<"other types not supported for isComplex"<<std::endl;
    exit(0);
  }
  ScalarType::VComplexType complexity = scal->getComplexStatus();
  return scal->getComplexStatus() == ScalarType::COMPLEX|| scal->getComplexStatus() 
  == ScalarType::MAYCOMPLEX;
}

std::string VCompiler::generateArgs(ExpressionPtrVector args,SymTable *symTable, bool checkIsInt,string cast){
  std::string str="";
  if(args.size()>0){
     if(checkIsInt){                                                                           
#ifdef DEBUG                                                                                    
        if(cast==""){                                                                         
                std::cout<<"cast string is null"<<std::endl;                                    
        }                                                                                       
#endif                                                                                          
        if(!isInt(args[0])){                                                                    
                str+="("+cast+")";                                                              
        }                                                                                       
      }       
    str+=exprTypeCodeGen(args[0],symTable).getAllStmt()[0];
    for(int i=1;i<args.size();i++){
      Context tempCntxt;
      tempCntxt=exprTypeCodeGen(args[i],symTable);
      str+=",";
      if(checkIsInt){
#ifdef DEBUG
	if(cast==NULL){
		std::cout<<"cast string is null"<<std::endl;
	}
#endif
	if(!isInt(args[i])){
      		str+="("+cast+")";
	}
      }
      str+=tempCntxt.getAllStmt()[0];
    }
  }
  str+="";
  return str;
}
std::string  VCompiler::generateMatClassStr(LibCallExprPtr expr, SymTable *symTable){
    return  generateMatClassStr(expr->getType());
  
}
std::string VCompiler::generateMatClassStr(VTypePtr vtype){
	return "Blas"+getMatTypeStr(vtype);
}
std::string VCompiler::genCblasOrder(ArrayTypePtr vtype){
	switch(vtype->getLayout()){
		case ArrayType::ROW_MAJOR:
			return "CblasRowMojor";
		case ArrayType::COL_MAJOR:
			return "CblasColMajor";
		default :
#ifdef DEBUG
		std::cout<<"Default case in generateCblasOrder"<<std::endl;
#endif	
			return "";
	}
}
std::string VCompiler::genCblasTrans(){
	return "CblasNoTrans";
}
std::string VCompiler::getNdims(ArrayTypePtr vtype){
		
    return itoa(vtype->getNdims());
}

bool VCompiler::isMatMultCall(LibCallExprPtr expr) {
    return expr->getArg(0)->getType()->getBasicType()==VType::ARRAY_TYPE&&expr->getArg(1)->getType()->getBasicType()==VType::ARRAY_TYPE;
}

Context VCompiler::matMultCallCodeGen(LibCallExprPtr expr, SymTable *symTable,ExpressionPtr lhsExpr){
    Context cntxt;
    string outStr="";
    if(isMatMultCall(expr)){
        VTypePtr blasLayoutType = expr->getType();
        if (expr->getType()->getBasicType() == VType::SCALAR_TYPE) {
            blasLayoutType = expr->getArg(0)->getType();
        }
        outStr=generateMatClassStr(expr,symTable)+"::mmult"+"("+genCblasOrder(static_cast<ArrayTypePtr>(blasLayoutType))+","+genCblasTrans()+","+genCblasTrans()+","+generateArgs(expr->getArgs(),symTable)+(lhsExpr!=NULL?", &"+exprTypeCodeGen(lhsExpr,symTable).getAllStmt()[0]:"")+")";
    } else {
        return elemMultCallCodeGen(expr, symTable,lhsExpr);
    }
    cntxt.addStmt(outStr);
    return cntxt;
}

Context VCompiler::matPlusCallCodeGen(LibCallExprPtr expr, SymTable *symTable,ExpressionPtr lExpr) {
	Context cntxt;
	if(expr->getArg(0)->getType()->getBasicType()==VType::ARRAY_TYPE&&expr->getArg(1)->getType()->getBasicType()==VType::ARRAY_TYPE){
	 cntxt.addStmt(generateMatClassStr(expr,symTable)+"::vec_add"+"("+getNdims(static_cast<ArrayTypePtr>(expr->getType()))+","+generateArgs(expr->getArgs(),symTable)+(lExpr != NULL?",&"+exprTypeCodeGen(lExpr,symTable).getAllStmt()[0]:"")+")");
	return cntxt;
	}
	ExpressionPtr arrayExpr, scalExpr;
	if(expr->getArg(0)->getType()->getBasicType()==VType::SCALAR_TYPE && 
	  expr->getArg(1)->getType()->getBasicType()==VType::ARRAY_TYPE ) {
	  arrayExpr = expr->getArg(1);
	  scalExpr = expr->getArg(0); 
	}else if (expr->getArg(1)->getType()->getBasicType()==VType::SCALAR_TYPE &&
	          expr->getArg(0)->getType()->getBasicType()==VType::ARRAY_TYPE) {
	  arrayExpr = expr->getArg(1);
	  scalExpr = expr->getArg(0);
	}
	string arrayStr = exprTypeCodeGen(arrayExpr, symTable).getAllStmt()[0];
	string scalStr = exprTypeCodeGen(scalExpr, symTable).getAllStmt()[0];
	cntxt.addStmt(generateMatClassStr(expr,symTable)+"::scal_add"+"("+getNdims(static_cast<ArrayTypePtr>(expr->getType()))+","+arrayStr +","+scalStr+(lExpr != NULL ?", &"+ exprTypeCodeGen(lExpr,symTable).getAllStmt()[0]:"")+")");
	return cntxt;
}

Context VCompiler::elemMultCallCodeGen(LibCallExprPtr expr, SymTable *symTable,ExpressionPtr lExpr) { 
    Context cntxt; 
    if(expr->getArg(0)->getType()->getBasicType()==VType::ARRAY_TYPE&&expr->getArg(1)->getType()->getBasicType()==VType::ARRAY_TYPE){
        cntxt.addStmt(generateMatClassStr(expr,symTable)+"::elem_mult"+"("+generateArgs(expr->getArgs(),symTable)+(lExpr != NULL?",&"+exprTypeCodeGen(lExpr, symTable).getAllStmt()[0]:"")+")");
        return cntxt;
    }
    ExpressionPtr arrayExpr, scalExpr;
    if(expr->getArg(0)->getType()->getBasicType()==VType::SCALAR_TYPE && 
            expr->getArg(1)->getType()->getBasicType()==VType::ARRAY_TYPE ) {
        arrayExpr = expr->getArg(1);
        scalExpr = expr->getArg(0); 
    }else if (expr->getArg(1)->getType()->getBasicType()==VType::SCALAR_TYPE &&
            expr->getArg(0)->getType()->getBasicType()==VType::ARRAY_TYPE) {
        arrayExpr = expr->getArg(0);
        scalExpr = expr->getArg(1);
    }
    string arrayStr = exprTypeCodeGen(arrayExpr, symTable).getAllStmt()[0];
    string scalStr = exprTypeCodeGen(scalExpr, symTable).getAllStmt()[0];
    cntxt.addStmt(generateMatClassStr(expr,symTable)+"::scal_mult"+"("+getNdims(static_cast<ArrayTypePtr>(expr->getType()))+","+arrayStr +","+scalStr+(lExpr != NULL?",&"+exprTypeCodeGen(lExpr, symTable).getAllStmt()[0]:"")+")");
    return cntxt;
}

Context VCompiler::matMinusCallCodeGen(LibCallExprPtr expr, SymTable *symTable,ExpressionPtr lExpr){
	
	Context cntxt;
	if(expr->getArg(0)->getType()->getBasicType()==VType::ARRAY_TYPE&&expr->getArg(1)->getType()->getBasicType()==VType::ARRAY_TYPE){
		cntxt.addStmt(generateMatClassStr(expr,symTable)+"::vec_sub"+"("+getNdims(static_cast<ArrayTypePtr>(expr->getType()))+","+generateArgs(expr->getArgs(),symTable)+(lExpr != NULL?",&"+exprTypeCodeGen(lExpr, symTable).getAllStmt()[0]:"")+")");
		return cntxt;
	}
	ExpressionPtr arrayExpr, scalExpr;
	if(expr->getArg(0)->getType()->getBasicType()==VType::SCALAR_TYPE && 
	  expr->getArg(1)->getType()->getBasicType()==VType::ARRAY_TYPE ) {
	  arrayExpr = expr->getArg(1);
	  scalExpr = expr->getArg(0); 
	}else if (expr->getArg(1)->getType()->getBasicType()==VType::SCALAR_TYPE &&
	          expr->getArg(0)->getType()->getBasicType()==VType::ARRAY_TYPE) {
	  arrayExpr = expr->getArg(0);
	  scalExpr = expr->getArg(1);
	}
	string arrayStr = exprTypeCodeGen(arrayExpr, symTable).getAllStmt()[0];
	string scalStr = exprTypeCodeGen(scalExpr, symTable).getAllStmt()[0];
	cntxt.addStmt(generateMatClassStr(expr,symTable)+"::scal_minus"+"("+arrayStr +","+scalStr+(lExpr != NULL?",&"+exprTypeCodeGen(lExpr, symTable).getAllStmt()[0]:"")+")");
	return cntxt;
  
}

Context VCompiler::matLDivCallCodeGen(LibCallExprPtr expr, SymTable *symTable,ExpressionPtr lExpr) {
  Context cntxt;
  if(expr->getArg(0)->getType()->getBasicType() == VType::ARRAY_TYPE &&
    expr->getArg(1)->getType()->getBasicType() == VType::ARRAY_TYPE) {
 	cntxt.addStmt(generateMatClassStr(expr,symTable)+"::mat_ldiv"+"("+genCblasOrder(static_cast<ArrayTypePtr>(expr->getType()))+","+generateArgs(expr->getArgs(),symTable)+(lExpr != NULL?",&"+exprTypeCodeGen(lExpr, symTable).getAllStmt()[0]:"")+")");
	return cntxt;
  } else {
  	return elemDivCallCodeGen(expr, symTable,lExpr);
  }
}

Context VCompiler::matRDivCallCodeGen(LibCallExprPtr expr, SymTable *symTable,ExpressionPtr lExpr) {
  Context cntxt;
  if(expr->getArg(0)->getType()->getBasicType() == VType::ARRAY_TYPE &&
    expr->getArg(1)->getType()->getBasicType() == VType::ARRAY_TYPE) {
 	cntxt.addStmt(generateMatClassStr(expr,symTable)+"::mat_rdiv"+"("+genCblasOrder(static_cast<ArrayTypePtr>(expr->getType()))+","+generateArgs(expr->getArgs(),symTable)+(lExpr != NULL?",&"+exprTypeCodeGen(lExpr, symTable).getAllStmt()[0]:"")+")");
	return cntxt;
  } else {
  	return elemDivCallCodeGen(expr, symTable,lExpr);
  }
}
Context VCompiler::elemDivCallCodeGen(LibCallExprPtr expr, SymTable *symTable,ExpressionPtr lExpr) {
Context cntxt;	 
  if(expr->getArg(0)->getType()->getBasicType()==VType::ARRAY_TYPE&&expr->getArg(1)->getType()->getBasicType()==VType::ARRAY_TYPE){
		cntxt.addStmt(generateMatClassStr(expr,symTable)+"::elem_div"+"("+generateArgs(expr->getArgs(),symTable)+(lExpr != NULL?",&"+exprTypeCodeGen(lExpr, symTable).getAllStmt()[0]:"")+")");
		return cntxt;
	}
	ExpressionPtr arrayExpr, scalExpr;
	if(expr->getArg(0)->getType()->getBasicType()==VType::SCALAR_TYPE && 
	  expr->getArg(1)->getType()->getBasicType()==VType::ARRAY_TYPE ) {
	  arrayExpr = expr->getArg(1);
	  scalExpr = expr->getArg(0); 
	}else if (expr->getArg(1)->getType()->getBasicType()==VType::SCALAR_TYPE &&
	          expr->getArg(0)->getType()->getBasicType()==VType::ARRAY_TYPE) {
	  arrayExpr = expr->getArg(0);
	  scalExpr = expr->getArg(1);
	}
	string arrayStr = exprTypeCodeGen(arrayExpr, symTable).getAllStmt()[0];
	string scalStr = exprTypeCodeGen(scalExpr, symTable).getAllStmt()[0];
	cntxt.addStmt(generateMatClassStr(expr,symTable)+"::scal_div"+"("+arrayStr +","+scalStr+(lExpr != NULL?",&"+exprTypeCodeGen(lExpr, symTable).getAllStmt()[0]:"")+")");
	
return cntxt;
}

Context VCompiler::matTransCallCodeGen(LibCallExprPtr expr, SymTable *symTable,ExpressionPtr lExpr){
 Context cntxt;
 std::string argStr;
 if(expr->getType()->getBasicType() == VType::SCALAR_TYPE) {
    if(lExpr != NULL) {
        cntxt.addStmt(exprTypeCodeGen(lExpr,symTable).getAllStmt()[0] + " = " +
            exprTypeCodeGen(expr->getArg(0),symTable).getAllStmt()[0]);
        return cntxt;
    } else {
        return exprTypeCodeGen(expr->getArg(0), symTable);
    }
 }
 std::string exprStr = generateMatClassStr(expr,symTable)+"::transpose"+"("+generateArgs(expr->getArgs(),symTable)
+ (lExpr != NULL?",&"+exprTypeCodeGen(lExpr, symTable).getAllStmt()[0]:"")+")";
 if(expr->getType()->getBasicType() == VType::SCALAR_TYPE) {
 	exprStr = genDataStr(expr->getType(),exprStr);	
 	exprStr = "*(" + exprStr + ")";
 }
 cntxt.addStmt(exprStr); 
 return cntxt; 
}

bool VCompiler::scalarArgs(FunCallExprPtr expr) {
  bool flag=true;
  int nargs= expr->getNargs();
  for(int i = 0; i < nargs; i++) {
    ExpressionPtr argExpr = expr->getArg(i);
    if(argExpr->getType()->getBasicType() != VType::SCALAR_TYPE) {
      flag = false;
    }
  }
  return flag;
}

Context VCompiler::catCallCodeGen(FunCallExprPtr expr, SymTable *symTable){
  Context cntxt;
  string funcStr=expr->getName();
  string typeStr = vTypeCodeGen(expr->getType(),symTable).getAllStmt()[0];
  funcStr += "<" + typeStr + ">";
  int nargs = expr->getNargs();
  /*if (scalarArgs(expr)) {
    if(nargs <= 0) {
      std::cout<<"Arguments cannot be 0. \n Exiting"<<std::endl;
      exit(0);
    }
    string argStr = generateArgs(expr->getArgs(),symTable);
    funcStr += "(" + itoa(nargs) + "," +argStr + ")"  ;
      
  }*/
  //else {
    funcStr += "(" +itoa(nargs);
    /*ExpressionPtr arg = expr->getArg(0);
    string argStr = exprTypeCodeGen(arg,symTable).getAllStmt()[0];
    if (arg->getType()->getBasicType()==VType::SCALAR_TYPE) {
       argStr+= "getVrArray<VrArrayF64,double>("+argStr+")";
    }
    funcStr+=","+argStr;
    */
    for(int i = 0; i < expr->getNargs(); i++) {
      ExpressionPtr arg = expr->getArg(i);
      string argStr = exprTypeCodeGen(arg,symTable).getAllStmt()[0];
      if (arg->getType()->getBasicType()==VType::SCALAR_TYPE) {
       argStr= "getVrArray<VrArrayF64,double>("+argStr+")";
      }
      funcStr+=","+argStr;  
    }
    funcStr+=")";
 // }
  
  cntxt.addStmt(funcStr);
  return cntxt;
}

bool VCompiler::hasArrayArg(FunCallExprPtr expr) {

  for(int i=0;i < expr->getNargs(); i++) {
  
    if(expr->getArg(i)->getType()->getBasicType()==VType::ARRAY_TYPE){
      return true;
    }
  }
  return false;
}

bool VCompiler::isBuiltin(const std::string & funcName) {
  return mapper.contains(funcName);
}

Context VCompiler::funCallExprCodeGen(FunCallExprPtr expr, SymTable *symTable,ExpressionPtr lExpr) {
    Context cntxt;
    Context tempCntxt;
    string fnName =expr->getName();
    string name =fnName;
    if(name.compare("horzcat") == 0 || name.compare("vertcat") == 0) {
        return catCallCodeGen(expr,symTable);
    }
    if(name.compare("true") == 0 || name.compare("false") == 0) {
        cntxt.addStmt(name);
        return cntxt;
    }
    if(name.compare("fix") == 0){
        Context outTypeCntxt = vTypeCodeGen(expr->getType(),symTable);
        Context inTypeCntxt = vTypeCodeGen(expr->getArg(0)->getType(), symTable);
        std::string outStr = outTypeCntxt.getAllStmt()[0];
        std::string inStr = inTypeCntxt.getAllStmt()[0];
        cntxt.addStmt("fix<"+inStr+","+outStr+">(" + exprTypeCodeGen(expr->getArg(0),symTable).getAllStmt()[0]+ ")");
        return cntxt;
    }
    name +="(";
    if(isBuiltin(fnName) && mapper.getBuiltin(fnName).hasVarArgs()) {
        name +=itoa(expr->getNargs())+",";
    }
    if(expr->getNargs()>0){
        tempCntxt = exprTypeCodeGen(expr->getArg(0), symTable);
        if (tempCntxt.getAllStmt().size() > 0) {
            std::string argStr = tempCntxt.getAllStmt()[0];
            if(!isBuiltin(fnName) && 
                    expr->getArg(0)->getType()->getBasicType() != VType::SCALAR_TYPE
                    && (expr->getArg(0)->getExprType() != Expression::FUNCALL_EXPR && expr->getArg(0)->getExprType() != Expression::LIBCALL_EXPR)) {
                Context typeCntxt = vTypeCodeGen(expr->getArg(0)->getType(),symTable);
                std::string typeStr = typeCntxt.getAllStmt()[0]; 
                argStr = typeStr + "(&" + argStr + ")";				
            }	
            name += argStr;
        }
        for (int i = 1; i<expr->getNargs(); i++) {
            tempCntxt = exprTypeCodeGen(expr->getArg(i), symTable);
            if (tempCntxt.getAllStmt().size() > 0) {
                std::string argStr = tempCntxt.getAllStmt()[0];
                if(!isBuiltin(fnName) && 
                        expr->getArg(i)->getType()->getBasicType() != VType::SCALAR_TYPE 
                        &&( expr->getArg(i)->getExprType() != Expression::FUNCALL_EXPR ||    
                            expr->getArg(i)->getExprType() != Expression::LIBCALL_EXPR)) {
                    Context typeCntxt = vTypeCodeGen(expr->getArg(i)->getType(),symTable);
                    std::string typeStr = typeCntxt.getAllStmt()[0]; 
                    argStr = typeStr + "(&" + argStr + ")";
                }	
                name += "," + argStr;
            }
        }
    }
    if(lExpr != NULL) {
        name += ", &" + exprTypeCodeGen(lExpr,symTable).getAllStmt()[0] +")";
        cntxt.addStmt(name);
        return cntxt;
    }
    name += ")";
    if(mapper.contains(fnName)) {
        if (expr->getType()->getBasicType()==VType::SCALAR_TYPE && 
                mapper.getBuiltin(fnName).isArrayOutput()) {
            if(fnName.compare("rand") == 0) {
                name = fnName +"()";
            } else{
                name="(*"+genDataMacroStr(expr->getType(),name)+")";
            }
        }
    }
    cntxt.addStmt(name);
    return cntxt;
}



Context VCompiler::notExprCodeGen(NotExprPtr expr, SymTable *symTable) {
    Context cntxt;
    Context baseCntxt = exprTypeCodeGen(expr->getBaseExpr(), symTable);
    string baseStr = baseCntxt.getAllStmt()[0];
    cntxt.addStmt("(!" + baseStr + "");
    return cntxt;
}

Context VCompiler::negateExprCodeGen(NegateExprPtr expr, SymTable *symTable) {
    Context cntxt;
    std::string exprStr = "";
    Context baseCntxt = exprTypeCodeGen(expr->getBaseExpr(), symTable);
    if(expr->getBaseExpr()->getType()->getBasicType() == VType::ARRAY_TYPE) {
        exprStr = generateMatClassStr(expr->getType()) + "::scal_mult" +"("+
            getNdims(static_cast<ArrayTypePtr>(expr->getBaseExpr()->getType()))+
            ","+baseCntxt.getAllStmt()[0] +",-1)";
    } else {
        string baseStr = baseCntxt.getAllStmt()[0];
        exprStr = "(-" + baseStr + ")";
    }
    cntxt.addStmt(exprStr);
    return cntxt;
}
Context VCompiler::constExprCodeGen(ConstExprPtr expr, SymTable *symTable) {
    Context cntxt;
    ostringstream convert;
    ScalarTypePtr sc = static_cast<ScalarTypePtr> (expr->getType());
    convert.str("");
    int intVal;
    double float64Val;
    float float32Val;
    std::string constStr;
    switch (sc->getScalarTag()) {
        case ScalarType::SCALAR_INT32:
            intVal = expr->getIntVal();
            convert << intVal;
            constStr = convert.str();
            break;
        case ScalarType::SCALAR_FLOAT64:
            float64Val = expr->getDoubleVal();
            convert << float64Val;
            constStr = convert.str();
            if(constStr.find(".") == std::string::npos && constStr.find("E") == std::string::npos &&
                    constStr.find("e") == std::string::npos) {
                constStr += ".0f";	
            }
            break;
        case ScalarType::SCALAR_FLOAT32:
            float32Val=expr->getFloatVal();
            convert<<float32Val;
            constStr = convert.str();
        case ScalarType::SCALAR_INT64:
            long longVal=expr->getLongVal();
            convert<<longVal;
            constStr = convert.str();
    }
    cntxt.addStmt(constStr);
    return cntxt;
}

VCompiler::LoopDirection VCompiler::getLoopDirectionEnum(ExpressionPtr expr) {
        ExpressionPtr  stepExpr = NULL;
        if(expr == NULL) {
            return VCompiler::COUNT_UP;
        }
       if(expr->getExprType() == Expression::CAST_EXPR) {
            stepExpr = static_cast<CastExprPtr>(expr)->getBaseExpr();
        } else {
            stepExpr = expr;
        }
        if(stepExpr->getExprType()==Expression::CONST_EXPR) {
            ConstExprPtr constExpr = static_cast<ConstExprPtr>(stepExpr);
            if(constExpr->getDoubleVal()>0) {
                return (VCompiler::COUNT_UP);
            }else {
                return (VCompiler::COUNT_DOWN);
            }
        }
		else if(stepExpr->getExprType()==Expression::NEGATE_EXPR) {
			NegateExprPtr nExpr = static_cast<NegateExprPtr>(stepExpr);
			if(nExpr->getBaseExpr()->getExprType()==Expression::CONST_EXPR) {
				ConstExprPtr constExpr = static_cast<ConstExprPtr>(stepExpr);
				if(constExpr->getDoubleVal()<0) {
					return (VCompiler::COUNT_UP);
				}else{
					return (VCompiler::COUNT_DOWN);
				}
			}
     }
    return VCompiler::UNKNOWN;
}

std::vector<VCompiler::LoopDirection> VCompiler::getLoopDirections(DomainExprPtr expr, SymTable *symTable){
    std::vector<VCompiler::LoopDirection> loopVec;
    for(int i=0;i<expr->getNdims(); i++) {
        ExpressionPtr stepExpr = expr->getStepExpr(i);
        loopVec.push_back(getLoopDirectionEnum(stepExpr));
	}
	return loopVec;
}

Context VCompiler::domainExprCodeGen(DomainExprPtr expr, SymTable *symTable) {
	Context cntxt, strtCntxt, endCntxt, stepCntxt;
	vector<string> vec;
	for (int j = 0; j < expr->getNdims(); j++) {
		Context tempCntxt;
		ExpressionPtr startExpr = expr->getStartExpr(j);
        if(startExpr == NULL) {
            startExpr = ConstExprBuilder::getLongConstExpr(0);
        }
		tempCntxt = exprTypeCodeGen(startExpr, symTable);
		vec = tempCntxt.getAllStmt();
		for (int i = 0; i < vec.size(); i++) {
			strtCntxt.addStmt(vec[i]);
		}
		ExpressionPtr endExpr = expr->getStopExpr(j);
        if (endExpr == NULL) {
            std::cout<<"End Expression cannot be NULL"<<std::endl;
            exit(0);
        }
		tempCntxt = exprTypeCodeGen(endExpr, symTable);
		vec = tempCntxt.getAllStmt();
		for (int i = 0; i < vec.size(); i++) {
			endCntxt.addStmt(vec[i]);
		}
		ExpressionPtr stepExpr = expr->getStepExpr(j);
        if(stepExpr == NULL) {
            stepExpr = ConstExprBuilder::getLongConstExpr(1);
        }
		tempCntxt = exprTypeCodeGen(stepExpr, symTable);
		vec = tempCntxt.getAllStmt();
		for (int i = 0; i < vec.size(); i++) {
			stepCntxt.addStmt(vec[i]);
		}
	}
	vec = strtCntxt.getAllStmt();
	for (int i = 0; i < vec.size(); i++) {
		cntxt.addStmt(vec[i]);
	}
	vec = endCntxt.getAllStmt();
	for (int i = 0; i < vec.size(); i++) {
		cntxt.addStmt(vec[i]);
	}
	vec = stepCntxt.getAllStmt();
	for (int i = 0; i < vec.size(); i++) {
		cntxt.addStmt(vec[i]);
	}
	return cntxt;
}
string VCompiler::getTypeStr(VTypePtr vtype){
	switch(vtype->getBasicType()){
		case VType::SCALAR_TYPE: 
			return getTypeStr((ScalarTypePtr)vtype);
			break;
		case VType::ARRAY_TYPE:
			return getTypeStr((ArrayTypePtr)vtype);
		default :
			std::cout<<"Other types not supported"<<vtype->getBasicType()<<std::endl;
	}
}
string VCompiler::genDataStr(VTypePtr vtype,string name ){
	return "VR_GET_DATA_"+getTypeStr(vtype)+"("+name+")";
}
string VCompiler::genDimStr(VTypePtr vtype,string name ){
	return "VR_GET_DIMS_"+getTypeStr(vtype)+"("+name+")";
}

string VCompiler::genDataMacroStr(VTypePtr vtype,string name){
  return "VR_GET_DATA_"+getTypeStr(vtype)+"("+ name +")";
    
} 

string VCompiler::genDimMacroStr(VTypePtr vtype,string name){
  
  return "VR_GET_DIMS_"+getTypeStr(vtype)+"("+name+")";
}

string VCompiler::getTypeStr(ScalarTypePtr vtype){
	switch (vtype->getScalarTag()) {
		case ScalarType::SCALAR_FLOAT64 :
			return "F64";
		case ScalarType::SCALAR_FLOAT32:
			return "F32";
		case ScalarType::SCALAR_INT32:
			return "I32";
		case ScalarType::SCALAR_INT64:
			return "I64";
		case ScalarType::SCALAR_BOOL :
			return "B";
	}
}

string VCompiler::getTypeStr(ArrayTypePtr vtype) {
	return getTypeStr((ScalarTypePtr)vtype->getElementType());
}

Context VCompiler::nameExprCodeGen(NameExprPtr expr, SymTable *symTable) {
	Context cntxt;
	string name = symTable->getName(expr->getId());
	VTypePtr vtype = symTable->getType(expr->getId());
	if(vtype->getBasicType()==VType::SCALAR_TYPE){
		//if it is a scalar pointer(which would then be a  VrArray ) then return a pointer otherwise just return the variable name
		// itself. Check set for pointer variable. 
		if (scalSet->find(name)!=scalSet->end()) {
			cntxt.addStmt(name+"_data");
		}else {
		  cntxt.addStmt(name);
		}	
	}
	else if (vtype->getBasicType()==VType::ARRAY_TYPE) {
		 cntxt.addStmt(name);	
	}else {
		std::cout<<"Basic Type "<<vtype->getBasicType()<<" not supported"<<std::endl;
	}
	return cntxt;
}

Context VCompiler::binaryExprCodeGen(BinaryExprPtr expr, SymTable *symTable) {
	Context cntxt;
	ExpressionPtr lPtr = expr->getLhs();
	ExpressionPtr rPtr = expr->getRhs();
	Context lCntxt = exprTypeCodeGen(lPtr, symTable);
	Context rCntxt = exprTypeCodeGen(rPtr, symTable);
	vector<string> lVec = lCntxt.getAllStmt();
	vector<string> rVec = rCntxt.getAllStmt();
	string lStr, rStr;
	if (lVec.size() > 0) {
		lStr = lVec[0];
		for (int i = 1; i < lVec.size(); i++) {
			lStr += " " + lVec[i];
		}
	}
	if (rVec.size()) {
		rStr = rVec[0];
		for (int i = 1; i < rVec.size(); i++) {
			rStr += " " + rVec[i];
		}

	}
	lStr = "(" + lStr;
 	rStr += ")";
	cntxt.addStmt(lStr);
	cntxt.addStmt(rStr);
	return cntxt;
}
Context VCompiler::neqExprCodeGen(NeqExprPtr expr, SymTable *symTable) {
	Context cntxt;
	Context tempCntxt = binaryExprCodeGen(expr, symTable);
	string lStr = tempCntxt.getAllStmt()[0];
	string rStr = tempCntxt.getAllStmt()[1];
	cntxt.addStmt(lStr + " != " + rStr);
	return cntxt;
}
Context VCompiler::leqExprCodeGen(LeqExprPtr expr, SymTable *symTable) {
	Context cntxt;
	Context tempCntxt = binaryExprCodeGen(expr, symTable);
	string lStr = tempCntxt.getAllStmt()[0];
	string rStr = tempCntxt.getAllStmt()[1];
	cntxt.addStmt(lStr + " <= " + rStr);
	return cntxt;
}

Context VCompiler::andExprCodeGen(AndExprPtr expr, SymTable *symTable) {
	Context cntxt;
	Context tempCntxt = binaryExprCodeGen(expr, symTable);
	string lStr = tempCntxt.getAllStmt()[0];
	string rStr = tempCntxt.getAllStmt()[1];
	cntxt.addStmt(lStr + " && " + rStr);
	return cntxt;
}

Context VCompiler::orExprCodeGen(OrExprPtr expr, SymTable *symTable) {
	Context cntxt;
	Context tempCntxt = binaryExprCodeGen(expr, symTable);
	string lStr = tempCntxt.getAllStmt()[0];
	string rStr = tempCntxt.getAllStmt()[1];
	cntxt.addStmt(lStr + " || " + rStr);
	return cntxt;
}

Context VCompiler::minusExprCodeGen(MinusExprPtr expr, SymTable *symTable) {
	Context cntxt;
	Context tempCntxt = binaryExprCodeGen(expr, symTable);
	string lStr = tempCntxt.getAllStmt()[0];
	string rStr = tempCntxt.getAllStmt()[1];
	cntxt.addStmt(lStr + " - " + rStr);
	return cntxt;
}

Context VCompiler::ltExprCodeGen(LtExprPtr expr, SymTable *symTable) {
	Context cntxt;
	Context tempCntxt = binaryExprCodeGen(expr, symTable);
	string lStr = tempCntxt.getAllStmt()[0];
	string rStr = tempCntxt.getAllStmt()[1];
	cntxt.addStmt(lStr + " < " + rStr);
	return cntxt;
}

Context VCompiler::gtExprCodeGen(GtExprPtr expr, SymTable *symTable) {
	Context cntxt;
	Context tempCntxt = binaryExprCodeGen(expr, symTable);
	string lStr = tempCntxt.getAllStmt()[0];
	string rStr = tempCntxt.getAllStmt()[1];
	cntxt.addStmt(lStr + " > " + rStr);
	return cntxt;
}

Context VCompiler::geqExprCodeGen(GeqExprPtr expr, SymTable *symTable) {
	Context cntxt;
	Context tempCntxt = binaryExprCodeGen(expr, symTable);
	string lStr = tempCntxt.getAllStmt()[0];
	string rStr = tempCntxt.getAllStmt()[1];
	cntxt.addStmt(lStr + " >= " + rStr);
	return cntxt;
}

Context VCompiler::multExprCodeGen(MultExprPtr expr, SymTable *symTable) {
	Context cntxt;
	Context tempCntxt = binaryExprCodeGen(expr, symTable);
	string lStr = tempCntxt.getAllStmt()[0];
	string rStr = tempCntxt.getAllStmt()[1];
	cntxt.addStmt(lStr + " * " + rStr);
	return cntxt;
}

Context VCompiler::divExprCodeGen(DivExprPtr expr, SymTable *symTable) {
	Context cntxt;
	Context tempCntxt = binaryExprCodeGen(expr, symTable);
	string lStr = tempCntxt.getAllStmt()[0];
	string rStr = tempCntxt.getAllStmt()[1];
	cntxt.addStmt(lStr + " / " + rStr);
	return cntxt;
}

Context VCompiler::plusExprCodeGen(PlusExprPtr expr, SymTable *symTable) {
	Context cntxt;
	Context tempCntxt = binaryExprCodeGen(expr, symTable);
	string lStr = tempCntxt.getAllStmt()[0];
	string rStr = tempCntxt.getAllStmt()[1];
	cntxt.addStmt(lStr + " + " + rStr);
	return cntxt;
}

string VCompiler::itoa(int num){
	stringstream convert;
	convert<<num;
	return convert.str();
}

bool VCompiler::isInt(ScalarTypePtr type){
       if (type->getScalarTag()==ScalarType::SCALAR_INT64||type->getScalarTag()==ScalarType::SCALAR_INT32) {
             return true;
       }
       else {
             return false;
       }
}

bool VCompiler::isInt(ExpressionPtr expr) {
	switch (expr->getType()->getBasicType()) {
		case VType::SCALAR_TYPE :
			return isInt(static_cast<ScalarTypePtr>(expr->getType()));
		case VType::ARRAY_TYPE :
			return isInt(static_cast<ArrayTypePtr>(expr->getType())->getElementType());
	}
}

std::string VCompiler::getIndexDiffStr(){
	return "1";
}

bool VCompiler::isRowMajor(int id ,SymTable *symTable) {
    VTypePtr type= symTable->getType(id); 
    if (type->getBasicType()!=VType::ARRAY_TYPE) {
	  std::cout<<"Type is not array type . cannot generate index expression \n. Exiting."<<std::endl;
	  exit(0);
    }
    return static_cast<ArrayTypePtr>(type)->getLayout()==ArrayType::ROW_MAJOR;
}

bool VCompiler::isNegativeConst(IndexStruct index) {
    if(index.m_isNegativeMode && index.m_isExpr && 
        index.m_val.m_expr->getExprType() == Expression::MINUS_EXPR) 
    {
        MinusExprPtr expr = static_cast<MinusExprPtr>(index.m_val.m_expr);
        if(expr->getLhs()->getExprType() == Expression::CONST_EXPR &&
            expr->getRhs()->getExprType() == Expression::CONST_EXPR) {
            return true;
        }
    }
    return false;
}

bool VCompiler::isNegativeIndex( IndexExprPtr expr) {
    IndexVec vec = expr->getIndices();
    for( int i = 0; i < vec.size(); i++ ) {
        if(vec[i].m_isNegativeMode) return true;
    }
    return false;
}

bool VCompiler::canSpecialiseNegativeIndex(IndexVec vec) {
    return vec.size() <= 2;
}

bool VCompiler::canSpecialiseArraySlice(IndexVec vec) {
    return vec.size() <= 3;
}

std::string VCompiler::genSpecArraySliceStr(IndexExprPtr expr, SymTable *symTable,ExpressionPtr lhsExpr) {
    std::string arrayName = symTable->getName(expr->getArrayId());
    return "(" + arrayName + ".sliceArraySpec(" + (lhsExpr != NULL? "&"+exprTypeCodeGen(lhsExpr, symTable).getAllStmt()[0]+",":"") + genSliceStr(expr,symTable) + "))";
}

std::string VCompiler::genSpecNegativeIndexStr(IndexExprPtr expr, SymTable *symTable) {
    int id=expr->getArrayId();
    std::string arrayName=symTable->getName(id); 
    IndexVec vec=expr->getIndices();
    Context typeCntxt = vTypeCodeGen(symTable->getType(id), symTable);
    std::string typeStr = typeCntxt.getAllStmt()[0];
    std::string exprStr = "getIndexVal_spec<"+typeStr + ">("+ arrayName + ", "  + exprTypeCodeGen(vec[0].m_val.m_expr,symTable).getAllStmt()[0]; 
    for( int i = 1; i < vec.size(); i++ ) {
        exprStr += "," + exprTypeCodeGen(vec[i].m_val.m_expr,symTable).getAllStmt()[0];
    }
    if( vec.size() >1) {
        exprStr += ","+itoa(!isRowMajor(id,symTable)); 
    }
    exprStr += ")";
    return exprStr;
}

std::string VCompiler::genNegativeIndexStr(IndexExprPtr expr , SymTable *symTable) {
    if(canSpecialiseNegativeIndex(expr->getIndices())) {
        return genSpecNegativeIndexStr(expr, symTable);
    } 
    int id=expr->getArrayId();
    std::string arrayName=symTable->getName(id); 
    IndexVec vec=expr->getIndices();
    Context typeCntxt = vTypeCodeGen(symTable->getType(id), symTable);
    std::string typeStr = typeCntxt.getAllStmt()[0];
    std::string exprStr = "getIndexVal<" + typeStr + ">("+ arrayName + ", " + itoa(vec.size()) + ", VrIndex(" + exprTypeCodeGen(vec[0].m_val.m_expr,symTable).getAllStmt()[0] + ")"; 
    for( int i = 1; i < vec.size(); i++ ) {
        exprStr += ", VrIndex(" + exprTypeCodeGen(vec[i].m_val.m_expr,symTable).getAllStmt()[0] + ")";
    }
    exprStr += ")"; 
    return exprStr;
}

std::string VCompiler::genIndexPtrFunc() const {
    return "getIndexPtr"; 
}

std::string VCompiler::genIndexPtrStr(IndexExprPtr expr, SymTable *symTable) {
    std::string funcStr = genIndexPtrFunc(); 
    funcStr += "<" + vTypeCodeGen(expr->getType(), symTable).getAllStmt()[0] + ">";
    funcStr += "(";
    IndexVec vec = expr->getIndices();
    for( int i = 0; i < vec.size(); i++) {
        if(vec[i].m_isExpr) {
            funcStr += exprTypeCodeGen(vec[i].m_val.m_expr, symTable).getAllStmt()[0];
        }
        else {
            std::cout<<"Slicing not supported in indexPtrStr"<<std::endl;
            exit(0);
        }
        if( i != vec.size() - 1) {
            funcStr += ", ";
        }
    }
    funcStr +=")";
    return funcStr;
}

std::string VCompiler::genIndexStr(IndexExprPtr expr, SymTable *symTable) {
    std::string declStr ="";
    int id=expr->getArrayId();
    std::string arrayName=symTable->getName(id); 
    IndexVec vec=expr->getIndices();
    string indexStr="";
    std::string exprStr;	
    if(expr->getType()->getBasicType() == VType::ARRAY_TYPE) {
        return genIndexPtrStr(expr, symTable);
    }
    bool rowMajor = isRowMajor(id, symTable);
    if (rowMajor && !isNegativeIndex(expr)) {
        std::reverse(vec.begin(),vec.end());
    }
    if (vec.size() <= 0) {
        std::cout<<"Index vector is of size 0"<<std::endl<<"Exiting";
        exit(0);
    }
    if(isNegativeIndex(expr)) {
        return genDataStr(symTable->getType(id),arrayName)+"["+genNegativeIndexStr(expr, symTable)+"]";
    } 
    for (int i = 0;i < vec.size();i++) {
        if (vec[i].m_isExpr) {		
            exprStr=exprTypeCodeGen(vec[i].m_val.m_expr,symTable).getAllStmt()[0];				
            if(!getCurrModule()->m_zeroIndex){
                exprStr="(" + exprStr + " - "+getIndexDiffStr()+ ")";
            }
            if (!isInt(vec[i].m_val.m_expr)) {
                exprStr="static_cast<long>("+exprStr+")";
            }
            indexStr+=exprStr;
            if (i < (vec.size() - 1)) {
                if(rowMajor) {
                    indexStr+=" + "+genDimStr(symTable->getType(id),arrayName)+"["+itoa(vec.size() - i - 1)+"]"+"*(";\
                } else { 
                    indexStr+=" + "+genDimStr(symTable->getType(id),arrayName)+"["+itoa(i)+"]"+"*(";\
                }
            }
        }
        else {
            std::cout<<"range expression for indices not supported"<<"Array Name "<<arrayName<<std::endl;
            exit(0);	
        }	
    }	
    for (int i=0;i<vec.size()-1;i++) {
        indexStr+=")";	 
    }
    return genDataStr(symTable->getType(id),arrayName)+"["+indexStr+"]";
}

bool VCompiler::isSlice(IndexExprPtr expr) {
  IndexVec vec = expr->getIndices();
  for (int i = 0;i < vec.size();i++) {
    if (!vec[i].m_isExpr) {
	return true;
    }
    if(vec[i].m_val.m_expr->getExprType()==Expression::FUNCALL_EXPR){   
      if (static_cast<FunCallExprPtr>(vec[i].m_val.m_expr)->getName().compare("colon") == 0) {
	return true;
      }
    }
  }
  return false;
}

std::string VCompiler::genRangeFuncStr(VRange * range,SymTable * symTable) {
		Context tempCntxt;
		string rangeStr="VrIndex(";
		tempCntxt= exprTypeCodeGen(range->m_start, symTable);
		rangeStr += tempCntxt.getAllStmt()[0]+",";
		tempCntxt = exprTypeCodeGen(range->m_stop, symTable);
		rangeStr += tempCntxt.getAllStmt()[0]+",";
		if(range->m_step != NULL) {
				tempCntxt = exprTypeCodeGen(range->m_step, symTable);
				rangeStr += tempCntxt.getAllStmt()[0];
		} else {
			rangeStr += "1";

		}
			rangeStr += ")";
		return rangeStr;
}

std::string VCompiler::genRangeFuncStr(ExpressionPtr expr, SymTable *symTable) {
  return "VrIndex(" + exprTypeCodeGen(expr, symTable).getAllStmt()[0] +")";
}

std::string VCompiler::genSliceStr(IndexExprPtr expr, SymTable *symTable) {
  std::string sliceStr;
  IndexVec vec = expr->getIndices();
  std::string arrayName = symTable->getName(expr->getArrayId());
  for (int i=0;i<vec.size();i++) {
      if(i!=0){
          sliceStr +=",";
      }
      if (vec[i].m_isExpr) {
          sliceStr+=genRangeFuncStr(vec[i].m_val.m_expr,symTable);
      }else {
          sliceStr+=genRangeFuncStr(vec[i].m_val.m_range,symTable); 
      }
  }
  return sliceStr;
}

std::string  VCompiler::handleArraySlicing(IndexExprPtr expr, SymTable *symTable,ExpressionPtr lhsExpr){
    IndexVec vec = expr->getIndices(); 
    if( canSpecialiseArraySlice(vec)) {
        return genSpecArraySliceStr(expr,symTable,lhsExpr);
    }
    std::string arrayName = symTable->getName(expr->getArrayId());
    return "(" + arrayName + ".sliceArray("+(lhsExpr != NULL? "&"+exprTypeCodeGen(lhsExpr,symTable).getAllStmt()[0]:"" ) + ", "+  itoa(vec.size())+ ", "+genSliceStr(expr,symTable) + "))";
}

Context VCompiler::indexExprCodeGen(IndexExprPtr expr , SymTable *symTable,ExpressionPtr lhsExpr) {
    Context cntxt;
    int id=expr->getArrayId();
    std::string arrayName=symTable->getName(id); 
    if(!isSlice(expr)){
        string indexStr=genIndexStr(expr,symTable);
        cntxt.addStmt(indexStr);
    } else {
        cntxt.addStmt(handleArraySlicing(expr,symTable,lhsExpr));
    }
    return cntxt;
}

std::string VCompiler::genTempVec() {
	return tempVarStr + itoa(maxTempVecId++);
}

std::string VCompiler::genIterVar() {
	return tempIterStr + itoa(maxTempIterId++);
}

Context VCompiler::loopStmtCodeGen(DomainExprPtr domainPtr, vector<int> iterVar, StmtListPtr bodyStmt, SymTable *symTable) {
    Context cntxt;
	Context domainCntxt = exprTypeCodeGen(domainPtr, symTable);
	std::vector<LoopDirection> loopVec;
	if(domainPtr->getExprType()==Expression::DOMAIN_EXPR) {
		loopVec = getLoopDirections(domainPtr,symTable); 
	}
	else {
		std::cout<<"getDomain does not return a domain expression"<<std::endl
		<<"Exiting"<<std::endl;
		exit(0);
	}
    
	string initStmt, compStmt, iterStmt;
	vector<string> domainVec = domainCntxt.getAllStmt();
	string var = symTable->getName(iterVar[0]);
	int count = iterVar.size();
	for (int i = 0; i < iterVar.size(); i++) {
		var = symTable->getName(iterVar[i]);
		if(loopVec[i] == VCompiler::COUNT_UP 
		   || loopVec[i] == VCompiler::COUNT_DOWN){
			initStmt = var + "=" + domainVec[i];
			if(loopVec[i]==VCompiler::COUNT_UP) {
				compStmt = var + "<" ;
			}else {
		        	compStmt = var + ">";
			}
            if(!static_cast<DomainExprPtr>(domainPtr)->getExclude(i)){
                compStmt += "=";
            }
            compStmt +=  " "+domainVec[i + count];
        
            iterStmt = var + "=" + var + "+" + domainVec[i + 2 * count];
            cntxt.addStmt(
                    "for(" + initStmt + ";" + compStmt + ";" + iterStmt + ")\n");
            cntxt.addStmt("{\n");
        }
        else {
            std::string vecStr = genTempVec();
            std::string iterStr=genIterVar();
            VTypePtr type = symTable->getType(iterVar[i]);
            Context typeCntxt;
            if(type->getBasicType() == VType::SCALAR_TYPE){
                typeCntxt = scalarTypeCodeGen(static_cast<ScalarTypePtr>(type));
            }
            else if(type->getBasicType() == VType::ARRAY_TYPE){
                typeCntxt = scalarTypeCodeGen(static_cast<ArrayTypePtr>(type)->getElementType());
            }
            std::string typeStr = typeCntxt.getAllStmt()[0];
            cntxt.addStmt("std::vector<"+typeStr+"> "+ vecStr + " = getIterArr<"+typeStr+">("+
                    domainVec[i]+","+domainVec[i+count]+","
                    + domainVec[i+2*count]+");\n");
            cntxt.addStmt(
                    "for( long " + iterStr+" = 0 "  + "; " + iterStr + " < "+vecStr+".size(); "+ iterStr + "++ )\n");
            cntxt.addStmt("{\n");
            cntxt.addStmt(var + "=" + vecStr+"["+iterStr+"];\n");
        }
    }
    Context bodyCntxt = stmtTypeCodeGen(bodyStmt, symTable);
    vector<string> bodyVec = bodyCntxt.getAllStmt();
    for (int i = 0; i < bodyVec.size(); i++) {
        cntxt.addStmt(bodyVec[i]);
    }
    for (int i = 0; i < iterVar.size(); i++) {
        cntxt.addStmt("}\n");
    }
    return cntxt;
}

Context VCompiler::replaceIndexWithStop(IndexStruct index, LoopInfo *info, SymTable *symTable){
    Context cntxt;
    if(!index.m_isExpr) {
        std::cout<<"Index has to be an expression"<<std::endl;
    }
    cntxt = replaceExprWithExpr(index.m_val.m_expr, info, symTable, false);
    return cntxt;
}

Context VCompiler::replaceNameExprWithExpr(NameExprPtr nameExpr, LoopInfo *info, SymTable *symTable,bool isStart) {
    Context cntxt;
    unordered_set<int> defSet = info->m_udmgInfo->m_defs;
    if(isIterVar(nameExpr->getId()) && defSet.find(nameExpr->getId()) != defSet.end()) {
        ExpressionPtrVector exprVec = getLoopBoundsFromMap(nameExpr->getId());
        VCompiler::LoopDirection dir  = getLoopDirectionEnum(exprVec[2]);
        if(isStart) {
            if(dir == COUNT_UP) {
                cntxt = exprTypeCodeGen(exprVec[0], symTable);
            } else if(dir == COUNT_DOWN) {
                cntxt = exprTypeCodeGen(exprVec[1], symTable);
            } else  {
                std::cout<<"loop direction is not known. Can not generate optimisation. String. Exiting"<<std::endl;
            }
        }
        else  {
            if(dir == COUNT_UP) {
                cntxt = exprTypeCodeGen(exprVec[1], symTable);
            } else if(dir == COUNT_DOWN) {
                cntxt = exprTypeCodeGen(exprVec[0], symTable);
            } else  {
                std::cout<<"loop direction is not known. Can not generate optimisation. String. Exiting"<<std::endl;
            }
        }
    } else  {
        std::cout<<"NameExpression in replace"<<symTable->getName(nameExpr->getId())<<std::endl;
        return nameExprCodeGen(nameExpr, symTable);
    }
    return cntxt;
}

Context VCompiler::replaceBinaryExprWithExpr(BinaryExprPtr expr, LoopInfo* info, SymTable *table, bool isStart, std::string op) {
    Context  cntxt; 
    Context lCntxt = replaceExprWithExpr(expr->getLhs(),info, table,isStart);
    Context rCntxt = replaceExprWithExpr(expr->getRhs(),info, table,isStart);
    cntxt.addStmt("( " +lCntxt.getAllStmt()[0] +" "+ op  + " " + rCntxt.getAllStmt()[0] + ")");
    return cntxt;
}

Context VCompiler::replacePlusExprWithExpr(PlusExprPtr expr, LoopInfo *info, SymTable *table,bool isStart) {
    return replaceBinaryExprWithExpr(expr, info, table, isStart, "+");
} 

Context VCompiler::replaceMinusExprWithExpr(MinusExprPtr expr, LoopInfo *info, SymTable *table,bool isStart) {
    return replaceBinaryExprWithExpr(expr, info, table, isStart, "-");
}

Context VCompiler::replaceConstExprWithExpr(ConstExprPtr expr,SymTable *symTable) {
    return constExprCodeGen(expr,symTable);
}

Context VCompiler::replaceExprWithExpr(ExpressionPtr expr, LoopInfo *info, SymTable *symTable,bool isStart) {
    if(expr == NULL) {
        return Context();
    }
    switch(expr->getExprType()) {
        case Expression::NAME_EXPR : 
            return replaceNameExprWithExpr(static_cast<NameExprPtr>(expr),info, symTable, isStart);
        case Expression::CONST_EXPR :
            return replaceConstExprWithExpr(static_cast<ConstExprPtr>(expr),symTable);
        case Expression::PLUS_EXPR :
            return replacePlusExprWithExpr(static_cast<PlusExprPtr>( expr),info, symTable, isStart);
        case Expression::MINUS_EXPR :
            return replaceMinusExprWithExpr(static_cast<MinusExprPtr>(expr), info, symTable, isStart);
        default :
            return Context();
    }  
}

Context VCompiler::replaceIndexWithStart(IndexStruct index, LoopInfo *info, SymTable *symTable){
    Context cntxt;
    if(!index.m_isExpr) {
        std::cout<<"Index has to be an expression"<<std::endl;
    }
    ExpressionPtr expr = index.m_val.m_expr;
    cntxt = replaceExprWithExpr(expr, info, symTable, true);
    return cntxt;
}

Context VCompiler::genIndexOptimCondition(IndexExprPtr expr, LoopInfo *info, SymTable *symTable) {
    IndexVec vec = expr->getIndices();
    Context cntxt;
    std::string startFuncCall = genCheckOptimStartFunc(expr, symTable) +"(";
    std::string stopFuncCall = genCheckOptimStopFunc(expr, symTable)+"(";
    startFuncCall += symTable->getName(expr->getArrayId()); 
    stopFuncCall += symTable->getName(expr->getArrayId()); 
    for(int i = 0; i < vec.size(); i++) {
        Context startCntxt = replaceIndexWithStart(vec[i], info, symTable);
        Context stopCntxt = replaceIndexWithStop(vec[i], info, symTable);
        startFuncCall += ",";
        stopFuncCall += ",";
        startFuncCall += startCntxt.getAllStmt()[0];
        stopFuncCall += stopCntxt.getAllStmt()[0];
         
    }
    startFuncCall += ")";
    stopFuncCall += ")";
    cntxt.addStmt(startFuncCall);
    cntxt.addStmt(stopFuncCall);
    return cntxt;
}

std::string VCompiler::genCheckOptimStartFunc(IndexExprPtr expr,SymTable *symTable) {
    if(expr->getIndices().size() <=3) {
        return "checkDimStart_spec<" + vTypeCodeGen(symTable->getType(expr->getArrayId()),symTable).getAllStmt()[0]+">";
    }
        return "checkDimStart<" + vTypeCodeGen(symTable->getType(expr->getArrayId()),symTable).getAllStmt()[0]+">";
}

std::string VCompiler::genCheckOptimStopFunc(IndexExprPtr expr,SymTable *symTable) {
    if(expr->getIndices().size() <=3) {
        return "checkDimStop_spec<" + vTypeCodeGen(symTable->getType(expr->getArrayId()),symTable).getAllStmt()[0]+">";
    }
        return "checkDimStop<" + vTypeCodeGen(symTable->getType(expr->getArrayId()),symTable).getAllStmt()[0]+">";
}

std::string VCompiler::genCheckOptimCondition(IndexSet & indexSet, LoopInfo *info, SymTable *symTable) {
    Context cntxt;
    std::string condString="";
    IndexSet::iterator it = indexSet.begin();
    bool flag = false;
    for(; it != indexSet.end(); it++) {
        cntxt = genIndexOptimCondition(*it, info, symTable);
        if(flag) {
            condString += " && ";
        } else{
            flag = true;
        }
        condString +=cntxt.getAllStmt()[0] + " && " + cntxt.getAllStmt()[1]; 
    }
    return condString;
}

Context VCompiler::forStmtCodeGen(ForStmtPtr stmt, SymTable *symTable) { 
    Context cntxt;
    IndexSet indexSet;
    std::string optimString;
    indxToIterMap.clear();
    getIndexElimSet(stmt, symTable, indexSet);
    LoopInfo::LoopInfoMap::iterator it = infoMap.find(stmt); 
    if(it != infoMap.end() && indexSet.size() > 0) {
        LoopInfo *info = it->second;
        optimString = genCheckOptimCondition(indexSet, info, symTable);
    }
    StmtPtr sPtr = stmt->getBody();
    StmtListPtr bodyStmt;
    if(sPtr->getStmtType() == Statement::STMT_LIST) {
        bodyStmt = static_cast<StmtListPtr> (sPtr);
    } else {
        std::cout<<"Body of the loop has to be a statement list"<<std::endl;
        exit(0);
    }
    ExpressionPtr domainPtr = stmt->getDomain();
    if(indexSet.size() > 0) {
    cntxt.addStmt("if(" + optimString + ") { \n");
    setBoundsCheckFlag(false); 
    }
    cntxt.addStmtVec(loopStmtCodeGen(static_cast<DomainExprPtr>(domainPtr),stmt->getIterVars(), bodyStmt, symTable).getAllStmt());
    if(indexSet.size() > 0) {
    setBoundsCheckFlag(true); 
        cntxt.addStmt("} else {\n");
        cntxt.addStmtVec(loopStmtCodeGen(static_cast<DomainExprPtr>(domainPtr),stmt->getIterVars(), bodyStmt, symTable).getAllStmt());
        cntxt.addStmt("}\n");
    }
    return cntxt;
}

void VCompiler::getIndexElimSet(ForStmtPtr stmt, SymTable *symTable,IndexSet& indexSet) {
    if(infoMap.find(stmt) != infoMap.end()) {
        LoopInfo::LoopInfoMap::iterator it = infoMap.find(stmt); 
        std::vector<int> itervar = stmt->getIterVars();
        unordered_set<int> itervarSet(itervar.begin(), itervar.end());
        unordered_map<IndexStruct, unordered_set<StmtPtr> > indexToLoopMap;
        if(infoMap.find(stmt) != infoMap.end()) {
            getLoopIndices(infoMap.find(stmt)->second, symTable, itervarSet, static_cast<DomainExprPtr>(stmt->getDomain()), indexToLoopMap,stmt, indexSet);
        } else {
            std::cout<<"Warning: LoopInfo for the current for loop not found. skipping"<<std::endl;
            return;
        } 
        std::cout<<"set size"<<indexSet.size()<<std::endl;
    }
}

void VCompiler::getLoopIndices(LoopInfo * info, SymTable *symTable,unordered_set<int> itervarSet, DomainExprPtr domain,unordered_map<IndexStruct, unordered_set<StmtPtr> > & indexToLoopMap, ForStmtPtr currStmt, IndexSet & set) {
    std::vector<LoopInfo::IndexInfo> indices;
    if(infoMap.find(currStmt) != infoMap.end()) {
        indices = infoMap.find(currStmt)->second->m_indexes; 
    }
    for(int i = 0; i < indices.size(); i++ ) {
        if(usedIndices.find(indices[i].m_iexpr) != usedIndices.end())  {
            continue;
        }
        if(!requiresCheck(indices[i].m_iexpr)) {
            continue;
        }
        if(isValidIndex(indices[i],itervarSet, domain, symTable, info)) {
            set.insert(indices[i].m_iexpr);
            usedIndices.insert(indices[i].m_iexpr);
        }
    }
    std::vector<StmtPtr> childLoops ; 
    if(infoMap.find(currStmt) != infoMap.end()) {
         childLoops = infoMap.find(currStmt)->second->m_childLoops;
    }
    for (int  i = 0; i < childLoops.size(); i++ ) {
        if(childLoops[i]->getStmtType() == Statement::STMT_FOR) {
            ForStmtPtr forStmt = static_cast<ForStmtPtr>(childLoops[i]);
            std::vector<int> itervar = forStmt->getIterVars();
            itervarSet.insert(itervar.begin(), itervar.end());
            getLoopIndices(info, symTable,itervarSet, static_cast<DomainExprPtr>(forStmt->getDomain()), indexToLoopMap, forStmt, set); 
        }  
    }   
}

bool VCompiler::isExprAffine(ExpressionPtr expr, LoopInfo *info, unordered_set<int> itervarSet,IndexExprPtr index) {
    if(expr == NULL) {
        std::cout<<"Expression is NULL"<<std::endl;
        return false;
    }
    switch(expr->getExprType()) {
        case Expression::NAME_EXPR : 
            return isNameExprAffine(static_cast<NameExprPtr>(expr),info,itervarSet,index);
        case Expression::CONST_EXPR :
            return isConstExprAffine(static_cast<ConstExprPtr>(expr));
        case Expression::PLUS_EXPR :
            return isPlusExprAffine(static_cast<PlusExprPtr>(expr), info,itervarSet,index);
        case Expression::MINUS_EXPR :
            return isMinusExprAffine(static_cast<MinusExprPtr>(expr), info, itervarSet,index);
        default:
            return false;
    }
}

bool VCompiler::isMinusExprAffine(MinusExprPtr expr, LoopInfo *info, unordered_set<int> itervarSet,IndexExprPtr index) {
    return isExprAffine(expr->getLhs(),info,itervarSet,index) && isExprAffine(expr->getRhs(), info, itervarSet,index); 
}

bool VCompiler::isPlusExprAffine(PlusExprPtr expr, LoopInfo *info, unordered_set<int> itervarSet,IndexExprPtr index) {
    return isExprAffine(expr->getLhs(),info,itervarSet,index) && isExprAffine(expr->getRhs(), info, itervarSet,index); 
}

bool VCompiler::isConstExprAffine(ConstExprPtr expr) {
    return true;
}

void VCompiler::addToIndxToIterMap(IndexExprPtr index, int id) {
    if(indxToIterMap.find(index) != indxToIterMap.end()) {
        IntegerSet set = indxToIterMap.find(index)->second;
        set.insert(id);
        indxToIterMap.erase(index);
        indxToIterMap.insert(std::pair<IndexExprPtr,IntegerSet>(index, set));
    } else {
        IntegerSet set;
        set.insert(id);
        indxToIterMap.insert(std::pair<IndexExprPtr,IntegerSet>(index, set));
    }
}

bool VCompiler::isNameExprAffine(NameExprPtr nameExpr, LoopInfo *info, unordered_set<int> itervarSet,IndexExprPtr index) {
    if(itervarSet.find(nameExpr->getId()) == itervarSet.end() && 
            info->m_udmgInfo->m_defs.find(nameExpr->getId()) != info->m_udmgInfo->m_defs.end()) {
        return false;
    } else {
        if(itervarSet.find(nameExpr->getId()) != itervarSet.end()) {
            addToIndxToIterMap(index, nameExpr->getId()); 
            if(indxToIterMap.find(index) != indxToIterMap.end()) {
                IntegerSet set = indxToIterMap.find(index)->second; 
                IntegerSet::iterator it = set.begin();    
            }
        }
        return true;
    }

}

bool VCompiler::isIndexAffine(IndexStruct index, LoopInfo *info, unordered_set<int> itervarSet, IndexExprPtr indxExpr) {
    if(!index.m_isExpr) return false;
    return isExprAffine((index.m_val.m_expr),info, itervarSet,indxExpr);
}

std::vector<ExpressionPtr> VCompiler::getLoopBoundsFromMap(int id) {
    return lc.getLoopExpr(id);
}

bool VCompiler::isIterVar(int id) {
    return lc.isIterVar(id);
}

bool VCompiler::isNameExprInvariant(NameExprPtr expr,LoopInfo *info) {
    std::cout<<"name expression "<<expr->getId()<<std::endl;
    std::cout<<"In isNameExprInvariant"<<(info->m_udmgInfo->m_defs.find(expr->getId()) == 
        info->m_udmgInfo->m_defs.end())<<std::endl;

    return info->m_udmgInfo->m_defs.find(expr->getId()) == 
        info->m_udmgInfo->m_defs.end();
}

bool VCompiler::isConstExprInvariant(ConstExprPtr expr) {
    return true;
}

bool VCompiler::isPlusExprInvariant(PlusExprPtr expr,LoopInfo *info) {
    return isExprInvariant(expr->getLhs(),info) &&
        isExprInvariant(expr->getRhs(),info);
}

bool VCompiler::isMinusExprInvariant(MinusExprPtr expr, LoopInfo *info) {
    return isExprInvariant(expr->getLhs(),info) &&
        isExprInvariant(expr->getRhs(),info);
}

bool VCompiler::isExprInvariant(ExpressionPtr expr,LoopInfo *info) {
    if(expr == NULL) {
        return false;
    }
    switch(expr->getExprType()) {
        case Expression::NAME_EXPR : 
            return isNameExprInvariant(static_cast<NameExprPtr>(expr), info);
        case Expression::CONST_EXPR :
            return isConstExprInvariant(static_cast<ConstExprPtr>(expr));
        case Expression::MINUS_EXPR :
            return isMinusExprInvariant(static_cast<MinusExprPtr>(expr),info);
        case Expression::PLUS_EXPR :
            return isPlusExprInvariant(static_cast<PlusExprPtr>(expr),info);
        default :
            return false;
    }
    return false;
}


bool VCompiler::areLoopBoundsValid(IndexExprPtr expr, LoopInfo *info) {
    IndexVec indices = expr->getIndices();
    if(indxToIterMap.find(expr) != indxToIterMap.end()) {
       IntegerSet set = indxToIterMap.find(expr)->second; 
        IntegerSet::iterator it = set.begin();    
        for(; it != set.end(); it++) {
            int id = *it;
            ExpressionPtrVector exprVec = getLoopBoundsFromMap(id);
            if(exprVec.size() == 0) {
                return false;
            }
            if(getLoopDirectionEnum(exprVec[2]) == UNKNOWN) {
                std::cout<<"Loop Direction can not be determined."<<std::endl;
                return false;
            }
            if(!isExprInvariant(exprVec[0],info) || !isExprInvariant(exprVec[1],info)) {
                std::cout<<"Loop bounds are not invariant"<<std::endl;
                return false;
            }
        }
    }
    
    return true;
}

bool VCompiler::isValidIndex(LoopInfo::IndexInfo indexInfo, unordered_set<int> itervarSet, DomainExprPtr domain, SymTable *symTable, LoopInfo *info) {
    IndexExprPtr indexExpr = indexInfo.m_iexpr;  
    IndexVec vec = indexExpr->getIndices();
    for( int i = 0; i < vec.size(); i++) {
        if(!vec[i].m_isExpr) {
            std::cout<<"Not an expr index"<<std::endl;
            return false;
        }
        if(!isIndexAffine(vec[i], info, itervarSet,indexExpr)) {
            std::cout<<"Index is not affine"<<std::endl;
            return false;
        }
    }
    if(!areLoopBoundsValid(indexExpr,info)) {
        std::cout<<"Loop range variables not loop invariant"<<std::endl;
        return false; 
    }
    return true;
}

Context VCompiler::getOriginalArrStr(NameExprPtr expr, SymTable * symTable){
	string name = symTable->getName(expr->getId());
	Context cntxt;
	cntxt.addStmt(name);
	return cntxt;
}

bool VCompiler::requireOriginalArr(AssignStmtPtr stmt){
      ExpressionPtr rExpr= stmt->getRhs();
      ExpressionPtr lExpr=stmt->getLhs()[0];
      if (lExpr->getExprType()==Expression::NAME_EXPR) {   
	if (rExpr->getExprType()==Expression::ALLOC_EXPR) {	  
	    return true;
	}
	else if (rExpr->getExprType()==Expression::LIBCALL_EXPR) {
	  LibCallExprPtr libExpr= static_cast<LibCallExprPtr>(rExpr);
	  return libExpr->getLibFunType()==LibCallExpr::LIB_MATMULT||libExpr->getLibFunType()==LibCallExpr::LIB_MINUS||libExpr->getLibFunType()==LibCallExpr::LIB_MULT || libExpr->getLibFunType()==LibCallExpr::LIB_DIV||libExpr->getLibFunType()==LibCallExpr::LIB_PLUS ||libExpr->getLibFunType()==LibCallExpr::LIB_TRANS;
	}
      }
      return false;
  
}
bool VCompiler::isArrayCopy(AssignStmtPtr stmt ) {
	ExpressionPtr rhsExpr=stmt->getRhs();
	if (stmt->getLhs().size()>1) {
		return false;
	}
	ExpressionPtr lhsExpr=stmt->getLhs()[0];
	if (lhsExpr->getExprType()!=Expression::NAME_EXPR) {
		return false;
	}
	if (lhsExpr->getType()->getBasicType()!=VType::ARRAY_TYPE) {
		return false;
	}
	if (rhsExpr->getExprType()!=Expression::NAME_EXPR) { 
		return false;
	}
	if (rhsExpr->getType()->getBasicType()!=VType::ARRAY_TYPE) {
		return false;
	}
	return true;
}

std::string VCompiler::genCopyExpr(std::string name,int ndims) {	
	return "vec_copy("+itoa(ndims)+","+name+")";
}

std::string VCompiler::genCopyExpr(NameExprPtr expr, SymTable * symTable){
	
	string name =symTable->getName(expr->getId());
	ArrayTypePtr type =static_cast<ArrayTypePtr>(symTable->getType(expr->getId()));
	string classStr= generateMatClassStr(type);
	return classStr+"::"+genCopyExpr(name,type->getNdims());	
}

Context VCompiler::handleMultLhs(AssignStmtPtr stmt, SymTable* symTable) {
    Context cntxt;
    ExpressionPtr rExpr = stmt->getRhs();
    FunCallExprPtr fExpr=NULL;
    if(rExpr->getExprType() == Expression::FUNCALL_EXPR) {
         fExpr = static_cast<FunCallExprPtr>(rExpr);
    }else {
        std::cout<<"Error in handleMultLhs : RHS not of type FunCallExpr. \n Exiting"
        <<std::endl;
        exit(0);
    }
    std::string structVar = genStructVarStr(fExpr->getName());
    std::string structType = genFuncStructName(fExpr->getName());
    std::string rStr = exprTypeCodeGen(rExpr,symTable).getAllStmt()[0];
    cntxt.addStmt(structType + " " + structVar + " = "+ rStr + ";\n");
    ExpressionPtrVector lExprVec = stmt->getLhs();
    if(lExprVec.size()==0) {
    	std::cout<<"Error in handleMultLhs : Lhs is empty"<<std::endl;
    }
    int count =0;
    for(int i = 0; i < lExprVec.size(); i++) {
        Context tempCntxt = exprTypeCodeGen(lExprVec[i],symTable);
        std::string lStr;
        if(lExprVec[i]->getType()->getBasicType() == VType::TUPLE_TYPE) {
        	std::vector<std::string> vec = tempCntxt.getAllStmt();
        	
        	for(int j = 0; j < vec.size(); j++) {
        		lStr = vec[j];
        		std::string structData = structVar + "." + genStructData() + itoa(count++);
        		cntxt.addStmt(lStr + " = " + structData + ";\n");	
        	}
        	continue;
        }
        lStr = tempCntxt.getAllStmt()[0];
        std::string structData = structVar + "." + genStructData() + itoa(count++);
        cntxt.addStmt(lStr + " = " + structData + ";\n");
    }
    return cntxt;
}

bool VCompiler::hasIndexExpr(StmtPtr stmt) {
	return collector.containsStmt(stmt);
}

std::string VCompiler::getBoundCheckFuncStr() {
	return std::string("checkBounds");
}

std::string VCompiler::getSpecBoundCheckFuncStr() {
	return std::string("checkBounds_spec");
}
std::string VCompiler::genBoundCheckHeader(IndexExprPtr expr, SymTable* symTable) {
	std::string boundFuncStr = getBoundCheckFuncStr();	
	VTypePtr vt = symTable->getType(expr->getArrayId());	
	std::string dataType = "";
	if(vt->getBasicType() == VType::ARRAY_TYPE) {
		ArrayTypePtr arrType  = static_cast<ArrayTypePtr>(vt);
		dataType  = vTypeCodeGen(arrType->getElementType(),symTable).getAllStmt()[0];	
	} else {
		std::cout<<"Error in genBoundCheckHeader : types apart from array types not supported."<<std::endl;
	}
	boundFuncStr += "<" + vTypeCodeGen(symTable->getType(expr->getArrayId()),symTable).getAllStmt()[0] + ","+ dataType+">";
	return boundFuncStr;
}

std::string VCompiler::genSpecBoundCheckHeader(IndexExprPtr expr, SymTable* symTable) {
	std::string boundFuncStr = getSpecBoundCheckFuncStr();	
	VTypePtr vt = symTable->getType(expr->getArrayId());	
	std::string dataType = "";
	if(vt->getBasicType() == VType::ARRAY_TYPE) {
		ArrayTypePtr arrType  = static_cast<ArrayTypePtr>(vt);
		dataType  = vTypeCodeGen(arrType->getElementType(),symTable).getAllStmt()[0];	
	} else {
		std::cout<<"Error in genBoundCheckHeader : types apart from array types not supported."<<std::endl;
	}
	boundFuncStr += "<" + vTypeCodeGen(symTable->getType(expr->getArrayId()),symTable).getAllStmt()[0] + ","+ dataType+">";
	return boundFuncStr;
}

Context VCompiler::genSpecialisedBoundCheck(IndexExprPtr expr, SymTable *symTable, bool onLhs) {
    Context cntxt;
    IndexVec vec = expr->getIndices();
    std::string boundFuncStr = genSpecBoundCheckHeader(expr,symTable);
    boundFuncStr += "(";
    boundFuncStr += "&"+symTable->getName(expr->getArrayId());
    boundFuncStr += ",";
    boundFuncStr +=  onLhs ? "true" : "false";
    for (int i = 0; i < vec.size(); i++) {
        std::string argStr = exprTypeCodeGen(vec[i].m_val.m_expr, symTable).getAllStmt()[0];
        if(vec[i].m_val.m_expr->getType()->getBasicType() == VType::SCALAR_TYPE ) {
            if ((static_cast<ScalarTypePtr>(vec[i].m_val.m_expr->getType()))->getScalarTag() != ScalarType::SCALAR_INT64 || 
                    (static_cast<ScalarTypePtr>(vec[i].m_val.m_expr->getType()))->getScalarTag() != ScalarType::SCALAR_INT32) {
                argStr = "static_cast<dim_type>(" + argStr + ")";
            } 
        } else if (vec[i].m_val.m_expr->getType()->getBasicType() == VType::ARRAY_TYPE) {
            ArrayTypePtr arrayType = static_cast<ArrayTypePtr>(vec[i].m_val.m_expr->getType());
            if(arrayType->getElementType()->getScalarTag() != ScalarType::SCALAR_INT32 || 
                    arrayType->getElementType()->getScalarTag() != ScalarType::SCALAR_INT64) {
                argStr = "static_cast<dim_type>(" + argStr + ")";
            } 
        }
        boundFuncStr += "," + argStr; 
    } 
    cntxt.addStmt(boundFuncStr + ");\n");
    return cntxt;
}
bool VCompiler::canSpecBoundCheckStmt(IndexVec vec) {
    for( int i = 0; i < vec.size(); i++) {
        if(!vec[i].m_isExpr) return false;
    }
    return vec.size() <=2;
}
Context VCompiler::genBoundCheckStmt(IndexExprPtr expr,SymTable * symTable,bool onLhs) {
    Context cntxt; 
    IndexVec vec = expr->getIndices();
    if (canSpecBoundCheckStmt(expr->getIndices())) {
        return genSpecialisedBoundCheck(expr, symTable, onLhs);	
    }
    std::string boundFuncStr = genBoundCheckHeader(expr,symTable);
    boundFuncStr += "(";
    boundFuncStr += "&"+symTable->getName(expr->getArrayId());
    boundFuncStr += ","+itoa(onLhs);
    boundFuncStr += ","+ itoa(vec.size());
    for(int i = 0; i < vec.size(); i++) {
        std::string rangeFunc;
        if(!vec[i].m_isExpr) {
            rangeFunc = genRangeFuncStr(vec[i].m_val.m_range,symTable); 
        }
        else{
            rangeFunc = genRangeFuncStr(vec[i].m_val.m_expr,symTable);
        }
        boundFuncStr += ","+rangeFunc;
        /*if(i < vec.size()-1) {
          boundFuncStr +=",";
          } */
    }	
    boundFuncStr +=");\n";
    cntxt.addStmt(boundFuncStr);
    return cntxt;
}
bool VCompiler::requiresCheck(IndexExprPtr expr){
    IndexVec vec = expr->getIndices();
    for( int i = 0; i < vec.size(); i++) {
        if(vec[i].m_isMinBoundsChecked || vec[i].m_isMaxBoundsChecked) {
            return true;
        }
    }
    return false;
}
Context VCompiler::genBoundCheckStmt(StmtPtr stmt,SymTable * symTable,bool onLhs) {
    Context cntxt;
    unordered_set<IndexExprPtr> *indexSet = collector.getIndexSet(stmt);
    if(indexSet == NULL) {
        return cntxt;
    }
    else {
        Context tmpCntxt;
        unordered_set<IndexExprPtr>::iterator it = indexSet->begin();
        for(; it !=indexSet->end(); it++) {
            if(!getBoundsCheckFlag() && 
                    usedIndices.find(*it) != usedIndices.end()) {
                continue;
            }
            if(isOnLhs(static_cast<IndexExprPtr>(*it))) {
                onLhs = true;	
            } else {
                onLhs = false;
            }
            if(requiresCheck(*it)){
                tmpCntxt.addStmtVec(genBoundCheckStmt(*it,symTable,onLhs).getAllStmt());
            }
        }
            cntxt.addStmt("#ifdef BOUND_CHECK\n");
        if(tmpCntxt.getAllStmt().size() > 0 ) {
            cntxt.addStmtVec(tmpCntxt.getAllStmt());
        }
            cntxt.addStmt("#endif\n");
    }
    return cntxt;
}

bool VCompiler::hasColon(IndexExprPtr expr) {
	return collector.hasColon(expr);	
}
bool VCompiler::isOnLhs(IndexExprPtr expr) {
	return	collector.isOnLhs(expr); 
}
Context VCompiler::handleSpecArraySliceSet(IndexExprPtr lhsExpr, ExpressionPtr expr, SymTable *symTable) {
    Context cntxt;
    std::string arrayName = symTable->getName(lhsExpr->getArrayId());
    std::string rhsExprStr  = exprTypeCodeGen(expr,symTable).getAllStmt()[0];
    std::string sliceStr = genSliceStr(lhsExpr,symTable);
    cntxt.addStmt(arrayName + ".setArraySliceSpec("+rhsExprStr+", " +sliceStr + ");\n");
    return cntxt;
}
Context VCompiler::handleArraySliceSet(IndexExprPtr lhsExpr, ExpressionPtr expr, SymTable* symTable) {
    Context cntxt;
    IndexVec vec = lhsExpr->getIndices();
    if( canSpecialiseArraySlice(vec)) {
        return handleSpecArraySliceSet(lhsExpr,expr,symTable);
    }
    std::string arrayName = symTable->getName(lhsExpr->getArrayId());
    std::string rhsExprStr  = exprTypeCodeGen(expr,symTable).getAllStmt()[0];
    std::string sliceStr = genSliceStr(lhsExpr,symTable);
    cntxt.addStmt(arrayName + ".setArraySlice("+rhsExprStr+", "+itoa(vec.size())+"," +sliceStr + ");\n");
    return cntxt;	
}

bool VCompiler::isScalarLibCall(AssignStmtPtr stmt, SymTable *symTable) {
    //TODO : The check for the matmult statement is temporary . Replace with a set
    return stmt->getRhs()->getExprType() == Expression::LIBCALL_EXPR && stmt->getRhs()->getType()->getBasicType() == VType::SCALAR_TYPE
            && (static_cast<LibCallExprPtr>(stmt->getRhs())->getLibFunType() == LibCallExpr::LIB_MATMULT || static_cast<LibCallExprPtr>(stmt->getRhs())->getLibFunType()  == LibCallExpr::LIB_TRANS)
            && stmt->getLhs()[0]->getExprType() == Expression::NAME_EXPR;
}
bool VCompiler::isScalarFunCall(AssignStmtPtr stmt, SymTable *symTable) {
    //TODO : The check for the sum and mean   is temporary . Replace with a set
    return stmt->getRhs()->getExprType() == Expression::FUNCALL_EXPR && stmt->getRhs()->getType()->getBasicType() == VType::SCALAR_TYPE
            &&( static_cast<FunCallExprPtr>(stmt->getRhs())->getName().compare("mean")  == 0 || 
            static_cast<FunCallExprPtr>(stmt->getRhs())->getName().compare("sum")  == 0) 
            && stmt->getLhs()[0]->getExprType() == Expression::NAME_EXPR;
}
bool VCompiler::isSpecLibCall(AssignStmtPtr stmt) {
    if(stmt->getLhs().size() != 1 || stmt->getLhs()[0]->getExprType() != Expression::NAME_EXPR) {
       return false; 
    }
    if(stmt->getRhs()->getExprType() == Expression::LIBCALL_EXPR && 
        libCallSet.count(static_cast<LibCallExprPtr>(stmt->getRhs())->getLibFunType()) > 0) {
        return true; 
    }
    return false;
}

bool VCompiler::isSpecSlice(AssignStmtPtr stmt) {
    if(stmt->getLhs().size() !=1 || stmt->getLhs()[0]->getExprType() != Expression::NAME_EXPR) {
        return false;
    }
    if (stmt->getRhs()->getExprType() == Expression::INDEX_EXPR  && isSlice(static_cast<IndexExprPtr>(stmt->getRhs()))) {
    return true; 
    } 
}
Context VCompiler::assignStmtCodeGen(AssignStmtPtr stmt, SymTable *symTable) {
    Context cntxt;
    ExpressionPtr rExpr = stmt->getRhs();
    string rStr;
    if(memOptmise) {
        if(isScalarLibCall( stmt,  symTable)) {
            Context tmpcntxt = exprTypeCodeGen(static_cast<LibCallExprPtr>(stmt->getRhs()), symTable, stmt->getLhs()[0]); 
            cntxt.addStmt(tmpcntxt.getAllStmt()[0] + ";\n"); 
            return cntxt;
        }
        if(isSpecLibCall(stmt)) {
            Context tempCntxt =  exprTypeCodeGen(stmt->getRhs(),symTable,stmt->getLhs()[0]);
            cntxt.addStmt(tempCntxt.getAllStmt()[0] + ";\n");
            return cntxt;
        }
        if(isScalarFunCall(stmt,symTable)) {
            Context tempCntxt = exprTypeCodeGen(stmt->getRhs(),symTable,stmt->getLhs()[0]);
            cntxt.addStmt(tempCntxt.getAllStmt()[0] + ";\n");
            return cntxt;
        }
        if(isSpecSlice(stmt)) {
            Context tempCntxt = exprTypeCodeGen(stmt->getRhs(),symTable,stmt->getLhs()[0]);
            cntxt.addStmt(tempCntxt.getAllStmt()[0] + ";\n");
            return cntxt;
        }
    }
    if(rExpr->getExprType() == Expression::FUNCALL_EXPR) {
        FunCallExprPtr fExpr = static_cast<FunCallExprPtr>(rExpr);
        if(hasMultRet(fExpr))
        {
            return handleMultLhs(stmt, symTable);
        }

    }
	if(isArrayCopy(stmt)){
		rStr=genCopyExpr(static_cast<NameExprPtr>(rExpr),symTable);
	}
	else{
		Context rCntxt = exprTypeCodeGen(rExpr, symTable);
		vector<string> rExprVec = rCntxt.getAllStmt();
		for (int i = 0; i < rExprVec.size(); i++) {
			rStr += rExprVec[i];
		}
	}
	vector<ExpressionPtr> lExprVec = stmt->getLhs();
	string lStr;
	if(lExprVec.size() > 1){
	  cout<<"Multiple expressions on the LHS not currently supported. \n Exiting"<<std::endl;
	  exit(0);
	}
	ExpressionPtr lExpr = lExprVec[0];
	Context lCntxt;
	if(lExpr->getExprType() == Expression::INDEX_EXPR && 
		isOnLhs(static_cast<IndexExprPtr>(lExpr)) && 
		hasColon(static_cast<IndexExprPtr>(lExpr))) {
		bool onLhs;
		Context tempCntxt;
		cntxt.addStmtVec(genBoundCheckStmt(stmt,symTable,onLhs).getAllStmt());
		cntxt.addStmtVec(handleArraySliceSet(static_cast<IndexExprPtr>(lExpr),rExpr,symTable).getAllStmt());	
		return cntxt;
	}
	lCntxt= exprTypeCodeGen(lExpr, symTable);
	vector<string> lVec = lCntxt.getAllStmt();
	for (int j = 0; j < lVec.size(); j++) {
		lStr += lVec[j];
	}
	if(hasIndexExpr(stmt)){
		bool onLhs; 
		cntxt.addStmtVec(genBoundCheckStmt(stmt, symTable,onLhs).getAllStmt());
	}
	cntxt.addStmt(lStr + " = " + rStr + ";\n");
	return cntxt;
}

IndexSet* VCompiler::getIndexSet(StmtPtr stmt) {
    return collector.getIndexSet(stmt);
}

Context VCompiler::stmtListCodeGen(StmtListPtr stmt, SymTable *symTable) {
	Context cntxt;
	for (int i = 0; i < stmt->getNumChildren(); i++) {

		StmtPtr childStmt = stmt->getChild(i);
		Context tempCntxt = stmtTypeCodeGen(childStmt, symTable);
		for (int j = 0; j < tempCntxt.getAllStmt().size(); j++) {
			cntxt.addStmt(tempCntxt.getAllStmt()[j]);
		}
	}
	return cntxt;
}

void VCompiler::setSseFlag(bool val) {
	enableSse = val;
}

bool VCompiler::getOpenMpFlag() {
	return enableOpenMP;
}

bool VCompiler::getSseFlag() {
	return enableSse;
}
