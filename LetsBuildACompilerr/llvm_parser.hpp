#ifndef LLVM_PARSER
#define LLVM_PARSER

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include "bison_objects.hpp"
#include "bp.hpp"
#include <ctype.h>
#include <map>
#include <stdlib.h>

class LLVM;

using namespace std;


const std::map<string, string> op_map = {
    { "+", "add" },
    { "-", "sub" },
    { "*", "mul" },
    { "/", "sdiv"},
    { "==", "eq" },
    { "!=", "ne"},
    { "<", "slt" },
    { "<=", "sle"},
    { ">", "sgt" },
    { ">=", "sge"}
};

const std::map<types::TypeName, string> type_map = {
    { types::INT, "i32" },
    { types::BYTE, "i8" },
    { types::BOOL, "i1" },
    { types::VOID, "void" },
    { types::STRING, "i8*"}
};


class LLVM {
    private:
        unsigned long regs_num;
        int reg_counter;
        int str_counter;

        LLVM();
	    LLVM(LLVM const&);

    public:
	    static LLVM &instance();

        void parseBinop(Node* left, string op, Node* right);


        void handleID(Exp* res, IDType* id);

        void handleAssign(IDType* id, Exp* exp);
        void handleBoolExp(Exp* exp);

        void handleFuncDecl(OverRide* OV,Node* ret_type, IDType* id, FormalsList* formals);
        void handleClosingFuncDecl();

        void handleBinop(Exp* res, Exp* exp1, char op,  Exp* exp2);

        void handleIf(Node* S, Exp* B, Node* M, Node* S1);
        void handleIfElse(Node* S, Exp* B, Node* M1, Node* S1, Node* N, Node* M2, Node* S2);
        void handleWhile(Node* S, Node* M1, Exp* B, Node* M2, Node* S1);
        void handleRelop(Exp* B, const string& id1, const string& op, const string& id2);
        void handleAnd(Exp* B, Exp* B1, Node* M, Exp* B2);
        void handleOr(Exp* B, Exp* B1, Node* M, Exp* B2);
        void handleInit(Node* type, IDType* id, Exp* exp);
        void handleCall(Exp* exp, IDType* id, const vector<Exp*>& params);
        void handleReturn(Exp* ret_exp);
        void handleReturnVoid();
        void handleReduceStringToExp(Exp* exp, char* yytext);

        void handleCast(Exp* left, types::TypeName type, Exp* exp);

        void handleBreak(Node* statement);
        void handleContinue(Node* statement);

        void handleProgramInit();

    	string genReg();
        string genStrName();
        vector<pair<int,BranchLabelIndex>> genBrToLabel();

        void genAllocaForFunctionBlock(string res_reg);

        void genTrunc32To8(const string& trunc_res, const string& in_reg);
        void genZext8To32(const string& zext_res, const string& in_reg);

        void genBinop(string res_reg, const string& in_reg1, char op ,const string& in_reg2);
        void genDivByZeroCheck(string in_reg2);

        void genPrintDivError(); 
        void genExit();

        void genStore(IDType* id, const string& val_reg);

        void genReturn(types::TypeName ret_type, const string& val_reg);

        string genFunDecl( bool is_override,string ret_type, string& id, vector<FormalDecl>& params);

        void genCompare(Exp* exp);

        void genGlobalStr(const string& str, const string& str_name);
        string genStringReg(int length, const string& name);


};



#endif