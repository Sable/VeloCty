#include <loopCollector.hpp>
#include<iostream>
#include<vector>
#define DEBUG
using namespace VRaptor;

void LoopCollector::caseModule(VModule * node){
#ifdef DEBUG
    std::cout<<"Analysing Module"<<node->getName()<<std::endl;
#endif
    std::vector<VFunction*> funcVec = node->getFns();
    for(int i = 0; i < funcVec.size(); i++) {
        caseFunction(funcVec[i]);
    }
}

void LoopCollector::caseFunction(VFunction *node){
    StmtListPtr stmtList = node->getBody();
    caseStmtList(stmtList);

}
void LoopCollector::caseExpr(ExpressionPtr node){
    switch (node->getExprType()) {
    case Expression::CONST_EXPR: // CONST_EXPR
        caseRealConstExpr(static_cast<ConstExprPtr>(node));
        break;
    case Expression::NAME_EXPR: //Name expression
        caseNameExpr(static_cast<NameExprPtr>(node));
        break;
    case Expression::PLUS_EXPR: //Plus expression
        //cout << "plus expression reached" << endl;
        casePlusExpr(static_cast<PlusExprPtr>(node));
        break;
    case Expression::MINUS_EXPR: //minus expression
        caseMinusExpr(static_cast<MinusExprPtr>(node));
        break;
    case Expression::MULT_EXPR: //mult expr
        caseMultExpr(static_cast<MultExprPtr>(node));
        break;
    case Expression::DIV_EXPR: // division expr
        caseDivExpr(static_cast<DivExprPtr>(node));
        break;
    case Expression::INDEX_EXPR: //index expr
        caseIndexExpr(static_cast<IndexExprPtr>(node));
        break;
    case Expression::NEGATE_EXPR: //Negate expr
        /* caseNegateExpr(static_cast<NegateExprPtr>(node)); */
        break;
    case Expression::GT_EXPR:  // greater than
        caseGtExpr(static_cast<GtExprPtr>(node));
        break;
    case Expression::GEQ_EXPR: //greater than equal to
        caseGeqExpr(static_cast<GeqExprPtr>(node));
        break;
    case Expression::LT_EXPR: // less than
        caseLtExpr(static_cast<LtExprPtr>(node));
        break;
    case Expression::LEQ_EXPR: // less than equal to
        caseLeqExpr(static_cast<LeqExprPtr>(node));
        break;
    case Expression::AND_EXPR: // and expression
        caseAndExpr(static_cast<AndExprPtr>(node));
        break;
    case Expression::OR_EXPR: // or expression
        caseOrExpr(static_cast<OrExprPtr>(node));
        break;
    case Expression::NOT_EXPR: // not expression
        /* caseNotExpr(static_cast<NotExprPtr>(node)); */
        break;
    case Expression::EQ_EXPR: //equal expression
        caseEqExpr(static_cast<EqExprPtr>(node));
        break;
    case Expression::FUNCALL_EXPR: // function call expression
        caseFuncallExpr(static_cast<FunCallExprPtr>(node));
        break;
    case Expression::DOMAIN_EXPR: // domain expression
        caseDomainExpr(static_cast<DomainExprPtr>(node));
        break;
    case Expression::DIM_EXPR:  //dim expression
        caseDimExpr(static_cast<DimExprPtr>(node));
        break;
    case Expression::DIMVEC_EXPR: // dim vector
        caseDimVecExpr(static_cast<DimvecExprPtr>(node));
        break;
    case Expression::LIBCALL_EXPR: // library call expression 
        caseLibCallExpr(static_cast<LibCallExprPtr>(node));
        break; 
    case Expression::TUPLE_EXPR:
        caseTupleExpr(static_cast<TupleExprPtr>(node));
        // std::cout<<"Tuple expressions not currently supported. \n Exiting "<<std::endl;
        // exit(0);
        break;
    case Expression::ALLOC_EXPR: // alloc expression
        caseAllocExpr(static_cast<AllocExprPtr>(node));
        break;
    case Expression::IMAG_EXPR : // imaginary expression
        caseImagExpr(static_cast<ImagExprPtr>(node));
        break;
    case Expression::REAL_EXPR: // real expressions
        caseRealExpr(static_cast<RealExprPtr>(node));
        break;
    /* case Expression::REDUCE_EXPR: // reduce expressions */
    /*     break; */
    case Expression::CAST_EXPR: // cast expression
        caseCastExpr(static_cast<CastExprPtr>(node));
        break;
    default:
        break;
    }
}
void LoopCollector::caseRealConstExpr(ConstExprPtr node){
    return;
}
void LoopCollector::caseNameExpr(NameExprPtr node){
    return;
}
void LoopCollector::caseBinaryExpr(BinaryExprPtr node){
    caseExpr(node->getLhs());
    caseExpr(node->getRhs());
}
void LoopCollector::casePlusExpr(PlusExprPtr node){
    caseBinaryExpr(node);
}
void LoopCollector::caseMinusExpr(MinusExprPtr node){
    caseBinaryExpr(node);
}
void LoopCollector::caseMultExpr(MultExprPtr node){
    caseBinaryExpr(node);
}
void LoopCollector::caseDivExpr(DivExprPtr node){
    caseBinaryExpr(node);
}
void LoopCollector::caseGtExpr(GtExprPtr node){
    caseBinaryExpr(node);
}
void LoopCollector::caseGeqExpr(GeqExprPtr node){
    caseBinaryExpr(node);
}
void LoopCollector::caseLtExpr(LtExprPtr node){
    caseBinaryExpr(node);
}
void LoopCollector::caseLeqExpr(LeqExprPtr node){
    caseBinaryExpr(node);
}
void LoopCollector::caseEqExpr(EqExprPtr node){
    caseBinaryExpr(node);
}
void LoopCollector::caseNeqExpr(NeqExprPtr node){
    caseBinaryExpr(node);
}
void LoopCollector::caseAndExpr(AndExprPtr node){
    caseBinaryExpr(node);
}
void LoopCollector::caseOrExpr(OrExprPtr node){
    caseBinaryExpr(node);
}
/* void LoopCollector::caseUnaryExpr(UnaryExprPtr node){ */
/*     caseExpr(node->getBaseExpr()); */
/* } */
/* void LoopCollector::caseNotExpr(NotExprPtr node){ */
/*     caseUnaryExpr(node); */
/* } */
/* void LoopCollector::caseNegateExpr(NegateExprPtr node){ */
/*     caseUnaryExpr(node); */
/* } */
void LoopCollector::caseFuncallExpr(FunCallExprPtr node){
    for(int i = 0; i < node->getNargs(); i++){
        caseExpr(node->getArg(i));
    }
}
void LoopCollector::caseVRange(VRange* node){
    return;
}
void LoopCollector::caseIndexExpr(IndexExprPtr node){
    addToMap(currStmt,node);
    bool hasColon = false;
    IndexVec vec = node->getIndices();
    for(int i =0; i < vec.size(); i++) {
        if(vec[i].m_isExpr){
			if(vec[i].m_val.m_expr->getType()->getBasicType()== VType::ARRAY_TYPE) {
				hasColon = true;
			}
            caseExpr(vec[i].m_val.m_expr);
        }else{
            caseVRange(vec[i].m_val.m_range);
            hasColon = true;

        }
    }
    if(hasColon) {
        addToColonSet(node);
    }
	if(onLhs) {
		addToLhsSet(node);
	}

}

void LoopCollector::caseLibCallExpr(LibCallExprPtr expr){
	for(int i = 0; i < expr->getNargs(); i++) {
		caseExpr(expr->getArg(i));
	}
}

void LoopCollector::caseDomainExpr(DomainExprPtr node){

}
void LoopCollector::caseTupleExpr(TupleExprPtr node){

}
/* void LoopCollector::caseMapExpr(MapExprPtr){ */

/* } */
void LoopCollector::caseDimExpr(DimExprPtr expr){

}
void LoopCollector::caseDimVecExpr(DimvecExprPtr expr){

}
void LoopCollector::caseAllocExpr(AllocExprPtr expr){

}
void LoopCollector::caseImagExpr(ImagExprPtr expr){

}
void LoopCollector::caseRealExpr(RealExprPtr expr){

}
void LoopCollector::caseCastExpr(CastExprPtr expr){

}

void LoopCollector::caseStmt(StmtPtr node){
    currStmt = node;
    switch (node->getStmtType()) {
        case Statement::STMT_ASSIGN: //Assignment Statement
        caseAssignStmt(static_cast<AssignStmtPtr>(node));
        break;
    case Statement::STMT_IF: // IF statement
        caseIfStmt(static_cast<IfStmtPtr>(node));
        break;
    case Statement::STMT_WHILE: // while statement 
        caseWhileStmt(static_cast<WhileStmtPtr>(node));
        break;
    case Statement::STMT_FOR: // for statement
        caseForStmt(static_cast<ForStmtPtr>(node));
        break;
    case Statement::STMT_PFOR: //parallel for
        casePforStmt(static_cast<PforStmtPtr>(node));
        break;
    case Statement::STMT_LIST: // list statement
        caseStmtList(static_cast<StmtListPtr>(node));
        break;
    case Statement::STMT_BREAK: //break statement
        caseBreakStmt(static_cast<BreakStmtPtr>(node));
        break;
    case Statement::STMT_CONTINUE: // continue statement
        caseContinueStmt(static_cast<ContinueStmtPtr>(node));
        break;
        break;
    case Statement::STMT_RETURN : //return
        caseReturnStmt(static_cast<ReturnStmtPtr>(node));
        break;
    default:
        break;
    }

}
void LoopCollector::caseStmtList(StmtListPtr node){
    for(int i = 0; i < node->getNumChildren(); i++) {
        caseStmt(node->getChild(i));
    }
}

void LoopCollector::caseAssignStmt(AssignStmtPtr node){
    ExpressionPtr rhsExpr = node->getRhs();
    caseExpr(rhsExpr);
    onLhs = true ;
    std::vector<ExpressionPtr> lhsVec = node->getLhs();
    for(int i = 0; i < lhsVec.size(); i++) {
        caseExpr(lhsVec[i]);
    }
    onLhs = false;
}
void LoopCollector::caseIfStmt(IfStmtPtr node){
    caseExpr(node->getCond());
    caseStmt(node->getIfBranch());
    if(node->hasElseBranch()){
        caseStmt(node->getElseBranch());
    }
}
void LoopCollector::caseForStmt(ForStmtPtr node){
    caseExpr(node->getDomain());
    caseStmt(node->getBody());
}
void LoopCollector::casePforStmt(PforStmtPtr node){
    caseExpr(node->getDomain());
    caseStmt(node->getBody());
}
void LoopCollector::caseWhileStmt(WhileStmtPtr node){
    caseExpr(node->getCond());
    caseStmt(node->getBody());
}
void LoopCollector::caseBreakStmt(BreakStmtPtr node){
    return;
}
void LoopCollector::caseContinueStmt(ContinueStmtPtr node){
    return ;
}
/* void LoopCollector::caseRefOpStmt(RefOpStmtPtr node){ */
/*     return; */
/* } */
void LoopCollector::caseReturnStmt(ReturnStmtPtr node){
    return ;
}

void LoopCollector::addToMap(StmtPtr currStmt, IndexExprPtr indexExpr){
    if(stmtToIndexExprMap.count(currStmt) > 0 ){
        stmtToIndexExprMap.find(currStmt)->second->insert(indexExpr);
    }else{
        unordered_set<IndexExprPtr> *set = new unordered_set<IndexExprPtr>();
        set->insert(indexExpr);
        stmtToIndexExprMap[currStmt] = set;
    }
}
void LoopCollector::addToColonSet(IndexExprPtr index){
    indexColonSet.insert(index);
}
void LoopCollector::addToLhsSet(IndexExprPtr index) {
	lhsIndexSet.insert(index);		
}

void LoopCollector::analyze(VModule * m){
    onLhs = false;
    caseModule(m);
}
void LoopCollector::prettyPrint() {
    IndexMap::iterator it = stmtToIndexExprMap.begin();
    for(int i = 0; it != stmtToIndexExprMap.end(); it++,i++) {
        IndexSet::iterator it_set = it->second->begin();
        for(; it_set != it->second->end(); it_set++ ){
            std::cout<<(*it_set)->getArrayId()<<"\t";
        }
        std::cout<<std::endl;
    }
}
