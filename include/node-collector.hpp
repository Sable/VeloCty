#ifndef __NODE_COLLECTOR__H
#define __NODE_COLLECTOR__H 
#include <vraptor.hpp>
#include <unordered_map>
#include <unordered_set>

using namespace std;
using namespace VRaptor;
namespace VRaptor{
      typedef std::unordered_set<IndexExprPtr> IndexSet;
      typedef std::unordered_map<StmtPtr,IndexSet*> IndexMap;
  class NodeCollector {
    private :
      IndexMap stmtToIndexExprMap;
      StmtPtr currStmt;
      bool onLhs;
      IndexSet lhsIndexSet;
	  IndexSet indexColonSet; 
      void addToMap(StmtPtr currStmt,IndexExprPtr indexExpr);
      void addToColonSet(IndexExprPtr index);
	  void addToLhsSet(IndexExprPtr index);
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
      void caseBoundCheckStmt(BoundCheckStmtPtr node);
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
      void analyze(VModule *);
      ~NodeCollector(){
        stmtToIndexExprMap.clear();
      }
      IndexMap getMap()const { return stmtToIndexExprMap;}

  };

}
#endif
