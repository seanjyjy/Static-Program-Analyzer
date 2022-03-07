#include "SimpleParser/Parser.h"
#include "Common/AstBuilder.h"
#include "SimpleParser/SPUtils.h"
#include "DesignExtractor/EntitiesExtractor.h"
#include "DesignExtractor/CallsExtractor.h"
#include "DesignExtractor/ModifiesExtractor.h"
#include <Exception/SemanticException.h>

using namespace std;

#include <iostream>

int main() {
    string s = "procedure main {\n"
               "call q; call p; a = b + 1; c = d;\n"
               "}\n"
               "procedure q { e = f * g; call p; }\n"
               "procedure p { h = i; call s; }\n"
               "procedure s { j = k + l; }\n"
               "procedure pp { call p; }";
//    cout << SPUtils::highlight(s, 1, 1, 1, 4) << endl;
//
    Parser p;
    TNode* ast = p.parseProgram(s);
    cout << "passing ast" << endl;
    EntitiesExtractor ee = EntitiesExtractor(ast);
    try {
        ee.extractEntities();
    }
    catch (SemanticException e) {
        cout << e.what() << endl;
        return 0;
    }
    cout << "entities extracted" << endl;
    auto stmtMp = ee.getNodeToStmtNumMap();
    auto procSet = ee.getProcSet();
    CallsExtractor ce = CallsExtractor(ast, procSet);
    ce.extractRelationship();

    for (auto [p, st] : ce.getCallsMap()) {
        cout << p << ":" << endl;
        for (auto s : st)
            cout << s << " ";
        cout << endl;
    }
    cout << endl;
    for (auto [p, st] : ce.getCallsTMap()) {
        cout << p << ":" << endl;
        for (auto s : st)
            cout << s << " ";
        cout << endl;
    }
    cout << endl;
    for (auto p : ce.getProcCallOrder())
        cout << p << " ";
    cout << endl << endl;

    auto callsMap = ce.getCallsMap();
    auto lst = ce.getProcCallOrder();
    ModifiesExtractor me = ModifiesExtractor(ast, stmtMp, callsMap, lst);
    me.extractRelationship();

    for (auto [proc, st] : me.getProcModifiesMap()) {
        cout << proc << endl;
        for (auto s : st)
            cout << s << " ";
        cout << endl;
    }
    cout << endl;
    for (auto [stmt, st] : me.getStmtModifiesMap()) {
        cout << stmt << endl;
        for (auto s : st)
            cout << s << " ";
        cout << endl;
    }
    cout << endl;
}