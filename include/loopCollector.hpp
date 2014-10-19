#ifndef __LOOP_COLLECTOR__H
#define __LOOP_COLLECTOR__H 
#include <vraptor.hpp>
#include<iostream>
#include <tr1/unordered_map>
#include <tr1/unordered_set>

// using namespace std;
using namespace VRaptor;
using std::tr1::unordered_map;
using std::tr1::unordered_set;
namespace VRaptor{
    typedef unordered_map<int, ExpressionPtrVector> IterToExprMap;
    typedef unordered_map<int, bool> IterToExcludeMap;
    class LoopCollector {
        private :
            IterToExprMap iterMap;
            IterToExcludeMap excludeMap;
            void addToExcludeMap(int iterVar, bool excludeVal);
            void addToIterMap(int iterVar, ExpressionPtr start, ExpressionPtr stop, ExpressionPtr step);
        public :
            void caseModule(VModule * node);
            void caseFunction(VFunction *node);
            void caseExpr(ExpressionPtr node);
            void caseRealConstExpr(ConstExprPtr node);
            void caseNameExpr(NameExprPtr node);
            void caseBinaryExpr(BinaryExprPtr node);
            void casePlusExpr(PlusExprPtr node);
            void caseMinusExpr(MinusExprPtr node);
            void caseMultExpr(MultExprPtr node);
            void caseDivExpr(DivExprPtr node);
            void caseGtExpr(GtExprPtr node);
            void caseGeqExpr(GeqExprPtr node);
            void caseLtExpr(LtExprPtr node);
            void caseLeqExpr(LeqExprPtr node);
            void caseEqExpr(EqExprPtr node);
            void caseNeqExpr(NeqExprPtr node);
            void caseAndExpr(AndExprPtr node);
            void caseOrExpr(OrExprPtr node);
            void caseNotExpr(NotExprPtr node);
            void caseNegateExpr(NegateExprPtr node);
            void caseFuncallExpr(FunCallExprPtr node);
            void caseVRange(VRange* node);
            void caseIndexExpr(IndexExprPtr node);
            void caseDomainExpr(DomainExprPtr node);
            void caseTupleExpr(TupleExprPtr node);
            void caseLibCallExpr(LibCallExprPtr expr);
            void caseDimExpr(DimExprPtr expr);
            void caseDimVecExpr(DimvecExprPtr expr);
            void caseAllocExpr(AllocExprPtr expr);
            void caseImagExpr(ImagExprPtr expr);
            void caseRealExpr(RealExprPtr expr);
            void caseCastExpr(CastExprPtr expr);
            void caseStmt(StmtPtr node);
            void caseStmtList(StmtListPtr node);
            void caseAssignStmt(AssignStmtPtr node);
            void caseIfStmt(IfStmtPtr node);
            void caseForStmt(ForStmtPtr node);
            void casePforStmt(PforStmtPtr node);
            void caseWhileStmt(WhileStmtPtr node);
            void caseBreakStmt(BreakStmtPtr node);
            void caseContinueStmt(ContinueStmtPtr node);
            void caseReturnStmt(ReturnStmtPtr node);
            void prettyPrint();
            void analyze(VModule *);
            ~LoopCollector(){
                iterMap.clear();
            }
        IterToExprMap getIterMap() const {
            return iterMap;
        }
        
        IterToExcludeMap getExcludeMap() const {
            return excludeMap;
        } 
        
        ExpressionPtrVector getLoopExpr(int itervar) const {
            if(iterMap.find(itervar) != iterMap.end()) {
                return iterMap.find(itervar)->second;
            }
            return ExpressionPtrVector();
        }
        
        bool getLoopExclude(int itervar) const {
            if(excludeMap.find(itervar) != excludeMap.end()) {
                return excludeMap.find(itervar)->second;
            }
            return false;
        }
        bool isIterVar(int id) const {
            return iterMap.find(id) != iterMap.end();
        }
    };

}
#endif
