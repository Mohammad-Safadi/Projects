#ifndef BISON_OBJECTS
#define BISON_OBJECTS

#include <vector>
#include <string>
#include <cassert>
#include <cstring>
#include <iostream>
#include <algorithm>
#include "bp.hpp"

#include "hw3_output.hpp"
using namespace std;

#define NA -208112292
#define UNDEFINED "NULL"
namespace types{
    enum TypeName{
        VOID,
        INT,
        BYTE,
        BOOL,
        STRING
    };
};

extern int yylineno;


static int while_count = 0;




struct Node {
    string quad;
    types::TypeName type;
    vector<pair<int, BranchLabelIndex>> trueList , falseList , nextList , continueList , breakList;
//    vector<pair<int, BranchLabelIndex>> falseList;
//    vector<pair<int, BranchLabelIndex>> nextList;
//    vector<pair<int, BranchLabelIndex>> continueList;
//    vector<pair<int, BranchLabelIndex>> breakList;
    Node() = default;
    Node(types::TypeName type) : type(type), quad(UNDEFINED) {};
    Node(const Node& clone) = default;
    ~Node() = default;
};

struct OverRide : public Node{
    bool is_overRide;
    OverRide(bool is_overRide = false) : is_overRide(is_overRide) {};
    OverRide(const OverRide& annotation) :is_overRide(false){};
    ~OverRide() = default;
};


struct IDType : public Node{
    string name;

    IDType() = default;
    IDType(const string& name) : name(name) {};
    ~IDType() = default;    
};

struct Num : public Node{
    int value;

    Num() = default;
    Num(int value) : value(value){};
    ~Num() = default; 
};


struct FormalDecl : public Node {
    string name;
    OverRide OR;

    FormalDecl() = default;
    FormalDecl(const OverRide& OR, types::TypeName type , const string& name) : Node(type), OR(OR), name(name) {};
    FormalDecl(const FormalDecl& clone) = default;
    ~FormalDecl() = default;
};
struct Exp: public Node{
    string name;
    string val_reg;

    Exp() = default;
    Exp(types::TypeName type, const string& name = "") : Node{type}, name(name), val_reg(UNDEFINED) {};
    ~Exp() = default;
};
struct FormalsList : public Node{
    vector<FormalDecl> params;
    
    FormalsList() = default;
    ~FormalsList() = default;

    void AddParameter(const FormalDecl& param){
        params.push_back(param);
    }
};


struct ExpList : public Node{
    vector<Exp*> params;
    
    ExpList() = default;
    ExpList(const ExpList& clone) = default;
    ~ExpList() = default;

    void AddParameter(Exp* param){
        params.push_back(param);
    }
};

struct FuncDecl : public Node{
    int local_bp;

    FuncDecl() = default;
    FuncDecl(int local_bp): local_bp(local_bp) {}
    FuncDecl(const FuncDecl& clone) = default;
    ~FuncDecl() = default;
};






struct SymTab{
    
    struct SymTabEntry{
        string name;
        bool is_overRide;
        int rnd;
        vector<types::TypeName> types_list;
        int offset;
        string bp_reg;
        string const_value;
        SymTabEntry() = default;
        SymTabEntry(const string& name,
                    bool is_overRide,int rnd,
                    const vector<types::TypeName>& types_list,
                    int offset,string bp_reg = UNDEFINED);
                    
        ~SymTabEntry() = default;
    };
    
    vector<SymTabEntry> symbols;
    SymTab() = default;
    ~SymTab() = default;
    void AddEntry(const string& name, bool is_overRide,int rnd, const vector<types::TypeName>& types_list, int offset);
    bool IsExist(const string& name);
    bool IsExistOv(const string& name);
    string IsExistWithargFunc(const string& name ,  vector<FormalDecl> params);
    SymTabEntry& GetEntry(const string& name);
    bool isvarExistFun(const string& name);
    bool IsExistOVsame(const string &name,vector<types::TypeName> types_list);
    bool isExistArg(vector<FormalDecl> params , string param);
};


static vector<SymTab> tables_stack;
static vector<int> offsets_stack;

bool isvarExistFun(const string& name);
void AddEntry(const string& name, bool is_overRide, const vector<types::TypeName>& types_list, int offset);
SymTab::SymTabEntry& GetCurrentFuncEntry();
bool IsExist(const string& name);
string IsExistWithargFunc(const string& name ,  vector<FormalDecl> params);
bool IsExistOverride(const string& name);
bool IsExistNotOverride(const string& name);
bool IsExistoverrideSame(const string& name ,vector<types::TypeName> types_list);
SymTab::SymTabEntry& GetEntry(const string& name);


types::TypeName GetFuncRetType();
types::TypeName GetCurrentFuncRetType();
string GetCurrentFuncBP();
bool isExistArg(vector<FormalDecl> params , string param);

bool findOverrideFunc(IDType* id ,vector<Exp*> symbols_list);
void AddFunction(OverRide* OR,Node* rettype, IDType* id, FormalsList* params);
void PushParamters(FormalsList* params);
void DeclareVariable( Node* type, IDType* id);
void InitVariable( Node* type, IDType* id, Node* exp);
void AssignVariable(IDType* id, Node* exp);
void ValidateRetTypeVoid();
void ValidateRetTypeNonVoid(types::TypeName type);
void OpenScope();
void CloseScope();
void OpenIfScope(Node* exp);
void OpenWhileScope(Node* exp);
void CloseWhileScope();
void CheckBreak();
void CheckContinue();
Exp* Call(IDType* IDType, const vector<Exp*>& params);
Exp* Binop(Node* left, Node* right);
Exp* ReduceIDToExp(IDType* id);
Exp* ReduceNumBToExp(Num* num);
Exp* LogicalOp(Node* exp);
Exp* LogicalBinop(Node* left, Node* right);
Exp* Relop(Node* left, Node* right);
Exp* Cast(Node* exp, Node* type);
void BeginProgram();
void EndProgram();
void CheckProgram();



#endif