#ifndef __NODE_COLLECTOR__H
#define __NODE_COLLECTOR__H 
#include <vraptor.hpp>
#include <tr1/unordered_map>
#include <tr1/unordered_set>

// using namespace std;
using namespace VRaptor;
using std::tr1::unordered_map;
using std::tr1::unordered_set;
namespace VRaptor{
      typedef unordered_set<IndexExprPtr> IndexSet;
      typedef unordered_map<StmtPtr,IndexSet*> IndexMap;
      typedef unordered_map<IndexExprPtr, StmtPtr> StmtMap;
  class NodeCollector {
    private :
      IndexMap stmtToIndexExprMap;
      StmtMap stmtMap;
      StmtPtr currStmt;
      StmtPtr currStmtList;
      bool onLhs;
      IndexSet lhsIndexSet;
	  IndexSet indexColonSet; 
      void addToMap(StmtPtr currStmt,IndexExprPtr indexExpr);
      void addToColonSet(IndexExprPtr index);
	  void addToLhsSet(IndexExprPtr index);
      void addToStmtMap(IndexExprPtr expr, StmtPtr stmt) {
         stmtMap.insert(std::pair<IndexExprPtr,StmtPtr>(expr,stmt));
      }
    public :
      /* void caseNode(Node* node); */
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
      /* void caseUnaryExpr(UnaryExprPtr node); */
      void caseNotExpr(NotExprPtr node);
      void caseNegateExpr(NegateExprPtr node);
      void caseFuncallExpr(FunCallExprPtr node);
      void caseVRange(VRange* node);
      void caseIndexExpr(IndexExprPtr node);
      void caseDomainExpr(DomainExprPtr node);
      void caseTupleExpr(TupleExprPtr node);
      /* void caseMapExpr(MapExprPtr); */
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
      /* void caseRefOpStmt(RefOpStmtPtr node); */
      void caseReturnStmt(ReturnStmtPtr node);
      void prettyPrint();
      bool containsStmt( StmtPtr stmt ) {
            return stmtToIndexExprMap.count(stmt) > 0;
      }
      IndexSet* getIndexSet(StmtPtr stmt){
            if(containsStmt(stmt)) {
                  return stmtToIndexExprMap.find(stmt)->second;
            }
            return NULL;
      }
	  bool isOnLhs(IndexExprPtr expr) {
			return lhsIndexSet.count(expr) > 0;
	  }
	  bool hasColon(IndexExprPtr expr) {
			return indexColonSet.find(expr) != indexColonSet.end(); 
	  } 
      StmtPtr getStmt(IndexExprPtr expr) {
        StmtMap::iterator it = stmtMap.find(expr);
        if(it != stmtMap.end()) {
            return it->second; 
        }
        return NULL;
      }
      void analyze(VModule *);
      ~NodeCollector(){
        stmtToIndexExprMap.clear();
      }
      IndexMap getMap()const { return stmtToIndexExprMap;}

  };

}
#endif
