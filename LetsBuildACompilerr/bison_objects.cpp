#include "bison_objects.hpp"
#include <iostream>

static inline bool IsValidImplicitCast(types::TypeName from, types::TypeName to);
static inline bool IsNumeric(types::TypeName type);
static vector<string> ReverseEnum(const vector<types::TypeName>& params);
static void PrintScope();
static void PrintEntry(int offset, const vector<types::TypeName>& types_list, const string& name);


SymTab::SymTabEntry::SymTabEntry(const string& name, 
                         bool is_overRide,int rnd,
                         const vector<types::TypeName>& types_list,
                         int offset ,string  bp_reg) :
            name(name),
            is_overRide(is_overRide),rnd(rnd),
            types_list(types_list),
            offset(offset),
            bp_reg(bp_reg)
            {}


void SymTab::AddEntry(const string& name,
                bool is_overRide,int rnd,
                const vector<types::TypeName>& types_list,
                int offset){
    symbols.push_back(SymTabEntry(name, is_overRide, rnd,types_list, offset));
}
bool SymTab::IsExistOv(const string& name){
    for(SymTab::SymTabEntry& entry : symbols){
        if((strcmp(entry.name.c_str(), name.c_str() )== 0) && entry.is_overRide){
            return true;
        }
    }
    return false;
}
SymTab::SymTabEntry& GetCurrentFuncEntry() {
    for (int i = tables_stack.size() - 1; i >= 0; i--) {
        vector <SymTab::SymTabEntry> &symbols = tables_stack[i].symbols;
        if (!symbols.empty() && symbols.back().offset == NA) {
            return symbols.back();
        }
    }
    SymTab::SymTabEntry a;
    return a;
}
string GetCurrentFuncBP(){
    return GetCurrentFuncEntry().bp_reg;
}
types::TypeName GetCurrentFuncRetType(){
    return GetCurrentFuncEntry().types_list[0];
}
bool SymTab::isExistArg(vector<FormalDecl> params , string param){
    int counter = 0;
    for(auto names : params){
        if(strcmp(names.name.c_str(),param.c_str()) == 0){
            counter++;
        }
    }
    return counter>1;
}
bool SymTab::IsExist(const string& name){
    for(SymTab::SymTabEntry& entry : symbols){
        if(strcmp(entry.name.c_str(), name.c_str()) == 0){
            return true;
        }

    }
    return false;
}
string SymTab::IsExistWithargFunc(const string& name ,  vector<FormalDecl> params){
    string matched_name = " ";
    for(SymTab::SymTabEntry& entry : symbols){
       for(int i =0; i< params.size(); i++){
           if(strcmp(params[i].name.c_str() , entry.name.c_str()) ==0  ){
               matched_name = params[i].name;
               return matched_name;
           }

       }
    }
    return matched_name;
}
bool SymTab::isvarExistFun(const string& name){
    for(SymTab::SymTabEntry& entry : symbols){
        if(strcmp(entry.name.c_str(), name.c_str()) == 0){
            return true;
        }
    }
    return false;
}
bool SymTab::IsExistOVsame(const string& name ,vector<types::TypeName> types_list){
    int size = 0;
    for(SymTab::SymTabEntry& entry : symbols){
        if(strcmp(entry.name.c_str(), name.c_str()) == 0 && entry.is_overRide){
            size = entry.types_list.size();
            if(size != types_list.size()){
                return false;
            }
            for(int i = 0; i < entry.types_list.size();i++){
                if(types_list[i] != entry.types_list[i])
                    return false;
            }
        }
    }
    if(size == 0){
        return false;
    }
    return true;
}
SymTab::SymTabEntry& SymTab::GetEntry(const string& name){
    for(SymTab::SymTabEntry& entry : symbols){
        if(strcmp(entry.name.c_str(), name.c_str()) == 0){
            return entry;
        }
    }
    
    // Should not get here
    assert(0 == 1);
    return *(new SymTab::SymTabEntry());
}




void AddEntry(const string& name, bool is_overRide, const vector<types::TypeName>& types_list, int offset){
    int rnd = rand();
    tables_stack.back().AddEntry(name, is_overRide,rnd,types_list, offset);
}
bool isvarExistFun(const string& name){
    for(SymTab table : tables_stack){
        if(table.isvarExistFun(name)){
            return true;
        }
    }
    return false;
}
bool IsExistOverride(const string& name){
    for(SymTab table : tables_stack){
        if(table.IsExist(name) && table.IsExistOv(name)){
            return true;
        }
    }
    return false;
}
bool isExistArg(vector<FormalDecl> params , string param){
    for(SymTab table : tables_stack){
        if(table.isExistArg(params,param)){
            return true;
        }
    }
    return false;
}
bool IsExistNotOverride(const string& name){
    for(SymTab table : tables_stack){
        if(table.IsExist(name) && !table.IsExistOv(name)){
            return true;
        }
    }
    return false;
}

bool IsExist(const string& name ){
    for(SymTab table : tables_stack){
        if(table.IsExist(name)){
            return true;
        }
    }
    return false;
}
string IsExistWithargFunc(const string& name , vector<FormalDecl> params){
    string match_name = " ";
    for(SymTab table : tables_stack){
            match_name = table.IsExistWithargFunc(name , params);
            return match_name;
    }
    return match_name;
}
bool IsExistoverrideSame(const string& name ,vector<types::TypeName> types_list ){
    for(SymTab table : tables_stack){
        if(table.IsExistOVsame(name ,types_list )){
            return true;
        }
    }
    return false;
}

SymTab::SymTabEntry& GetEntry(const string& name){
    for(SymTab& table : tables_stack){
        if(table.IsExist(name)){
            return table.GetEntry(name);
        }
    }
    
    // Should not get here
    assert(0 == 1);
    return *(new SymTab::SymTabEntry());
}


types::TypeName GetFuncRetType(){
    for(int i = tables_stack.size()-1; i>=0; i--){
        vector<SymTab::SymTabEntry>& symbols = tables_stack[i].symbols;
        if(!symbols.empty() && symbols.back().offset == NA){
            return symbols.back().types_list[0];
        }
    }
    
    // Should not get here
    assert(0 == 1);
    return types::VOID;
}


bool findOverrideFunc(IDType* id ,vector<Exp*> symbols_list){
    int param_count = 0, count = 0;
    bool match = false;
    SymTab::SymTabEntry symbols;
    auto functions =  tables_stack[0].symbols;
    for(int i =  functions.size()-1; i>=0; i--){
        if(functions[i].name == id->name &&  (functions[i].offset == NA) && (functions[i].types_list.size()-1 == symbols_list.size())){
           for(int j = 0; j < symbols_list.size(); j++){

               if(IsValidImplicitCast(symbols_list[j]->type,functions[i].types_list[j+1])){
                   param_count++;
               }
           }
            if(param_count == functions[i].types_list.size()-1){
                count++;
            }
            if(count == 1){
                match = true;
            }
            param_count =0;
           if(count >1){
                    output::errorAmbiguousCall(yylineno, id->name);
                    exit(0);
              }

           }
        }

    return  match;
}
void AddFunction(OverRide* OR,Node* rettype, IDType* id, FormalsList* formals_list){
    //  check if override function exist and not override
    if(IsExistOverride(id->name) && !(OR->is_overRide)){
        output::errorOverrideWithoutDeclaration(yylineno, id->name);
        exit(0);
    }

    // check if function exist and override
    if(IsExistNotOverride(id->name) && OR->is_overRide){
        output::errorFuncNoOverride(yylineno, id->name);
        exit(0);
    }
    vector<types::TypeName> types_list(1, rettype->type);
    for(auto param : formals_list->params){
        types_list.push_back(param.type);
    }
    if(IsExistoverrideSame(id->name,types_list)){
        output::errorDef(yylineno, id->name);
        exit(0);
    }
    if(IsExistNotOverride(id->name)){
        output::errorDef(yylineno, id->name);
        exit(0);
    }
    string match_name = IsExistWithargFunc(id->name,formals_list->params);
    if(match_name != " "){
        output::errorDef(yylineno, match_name);
        exit(0);
    }
    if(id->type == types::VOID && id->name == "main" && OR->is_overRide){
        output::errorMainOverride(yylineno);
        exit(0);
    }
    for(auto names : formals_list->params){
        if(isExistArg(formals_list->params,names.name)){
            output::errorDef(yylineno,names.name);
            exit(0);
        }
    }
    AddEntry(id->name, OR->is_overRide, types_list, NA);
}

void PushParamters(FormalsList* params){
    int offset = -1;
    for(int i = params->params.size()-1; i >= 0  ; i--){
        FormalDecl& decl = params->params[i];
        vector<types::TypeName> types_list(1, decl.type);
        AddEntry(decl.name, decl.OR.is_overRide, types_list, offset--);
    }
}



void DeclareVariable(Node* type, IDType* id){
    if(IsExist(id->name)){
        output::errorDef(yylineno, id->name);
        exit(0);
    }

    vector<types::TypeName> types_list(1, type->type);
    AddEntry(id->name, false, types_list, offsets_stack.back()++);
}

void InitVariable( Node* type, IDType* id, Node* exp){
    if(IsExist(id->name)){
        output::errorDef(yylineno, id->name);
        exit(0);
    }

    if(exp->type == types::VOID ||  !IsValidImplicitCast(exp->type, type->type)){
        output::errorMismatch(yylineno);
        exit(0);
    }   
    
    vector<types::TypeName> types_list(1, type->type);
    AddEntry(id->name, false, types_list, offsets_stack.back()++);
}


void AssignVariable(IDType* id, Node* exp){
    if(!IsExist(id->name)){
        output::errorUndef(yylineno, id->name);
        exit(0);
    }

    SymTab::SymTabEntry entry = GetEntry(id->name);

    if(!IsValidImplicitCast(exp->type, entry.types_list[0])){
        if(isvarExistFun(id->name)){
            output::errorUndef(yylineno,id->name);
            exit(0);
        }
        output::errorMismatch(yylineno);
        exit(0);
    }

    if(exp->type == types::VOID){
        output::errorMismatch(yylineno);
        exit(0);
    }

    if(entry.offset == NA){
        //This is a function
        output::errorUndef(yylineno, id->name);
        exit(0);
    }

}


void ValidateRetTypeVoid(){
    if(GetCurrentFuncRetType() != types::VOID){
        output::errorMismatch(yylineno);
        exit(0);        
    }
}


void ValidateRetTypeNonVoid(types::TypeName type){
    if(type == types::VOID || !IsValidImplicitCast(type, GetCurrentFuncRetType())){
        output::errorMismatch(yylineno);
        exit(0);
    } 
}



void OpenScope(){
    tables_stack.push_back(SymTab());
    offsets_stack.push_back(offsets_stack.back());
}

void PrintEntry(int offset, const vector<types::TypeName>& types_list, const string& name){
    if(offset != NA){
        output::printID(name, offset, ReverseEnum(types_list)[0]);
    }else{
        std::vector<std::string> types_str = ReverseEnum(types_list);
        string rettype = types_str[0];
        types_str.erase(types_str.begin());
        std::reverse(types_str.begin(), types_str.end());
        output::printID(name, 0, output::makeFunctionType(rettype,types_str));
    }
}

void PrintScope(){
    SymTab& symtab = tables_stack.back();
    for(SymTab::SymTabEntry& entry : symtab.symbols){
        PrintEntry(entry.offset, entry.types_list, entry.name);
    }
}


void CloseScope(){
    output::endScope();
    PrintScope();
    tables_stack.pop_back();
    offsets_stack.pop_back(); 
}


void OpenIfScope(Node* exp){
    if(exp->type != types::BOOL){
        output::errorMismatch(yylineno);
        exit(0);
    }
    
    OpenScope();
}

void OpenWhileScope(Node* exp){
    if(exp->type != types::BOOL){
        output::errorMismatch(yylineno);
        exit(0);
    }

    while_count++;
    OpenScope();
}

void CloseWhileScope(){
    while_count--;
    CloseScope();
}

void CheckBreak(){
    assert(while_count >= 0);
    
    if(while_count == 0){
        output::errorUnexpectedBreak(yylineno);
        exit(0);
    }
}


void CheckContinue(){
    assert(while_count >= 0);
    
    if(while_count == 0){
        output::errorUnexpectedContinue(yylineno);
        exit(0);
    }
}


vector<string> ReverseEnum(const vector<types::TypeName>& params){
    vector<string> res;
    
    for(auto type : params){
       switch(type){
            case types::VOID:
                res.push_back("VOID");
                break;

            case types::BOOL: 
                res.push_back("BOOL");
                break;

            case types::BYTE: 
                res.push_back("BYTE");
                break;
            
            case types::INT: 
                res.push_back("INT");
                break;
            
            case types::STRING: 
                res.push_back("STRING");
                break;

            default:
                res.push_back("NotUsed");
                break;
        }
    }

    return res;    
}


Exp* Call(IDType* IDType, const vector<Exp*>& params){
    // check if function is override
    if(!IsExist(IDType->name)){
        output::errorUndefFunc(yylineno, IDType->name);
        exit(0);
    }

    SymTab::SymTabEntry func_entry = GetEntry(IDType->name);

    vector<types::TypeName> func_params = func_entry.types_list;
    func_params.erase(func_params.begin());

    vector<string> str_func_params = ReverseEnum(func_params);
    std::reverse(str_func_params.begin(), str_func_params.end());


    if(func_entry.offset != NA) {
        output::errorUndefFunc(yylineno, IDType->name);
        exit(0);
    }
    if(IsExistOverride(IDType->name)){
        bool match =  findOverrideFunc(IDType,params);
        if(!match){
            output::errorPrototypeMismatch(yylineno, IDType->name);
            exit(0);
        }
    }
   else if(params.size() != func_params.size()){
        output::errorPrototypeMismatch(yylineno, IDType->name);
        exit(0);
    }
    for(int i = 0; i < params.size(); i++){
        if(!IsValidImplicitCast(params[i]->type,func_params[i])){
            output::errorPrototypeMismatch(yylineno, IDType->name);
            exit(0);
        }
    }
    return new Exp(func_entry.types_list[0]);
}


Exp* Binop(Node* left, Node* right){
    if(!IsNumeric(left->type) || !IsNumeric(right->type)){
        output::errorMismatch(yylineno);
        exit(0);
    }
    
    if(left->type == types::INT || right->type == types::INT){
        return new Exp(types::INT);
    }

    return new Exp(types::BYTE);
}


Exp* ReduceIDToExp(IDType* id){
    if(!IsExist(id->name) || GetEntry(id->name).offset == NA){
        output::errorUndef(yylineno, id->name);
        exit(0);
    }

    return new Exp(GetEntry(id->name).types_list[0]);
}

Exp* ReduceNumBToExp(Num* num){
    if(num->value > 255){
        output::errorByteTooLarge(yylineno, to_string(num->value));
        exit(0);
    }

    return new Exp(types::BYTE);
}


Exp* LogicalOp(Node* exp){
    if(exp->type != types::BOOL){
        output::errorMismatch(yylineno);
        exit(0);
    }

    return new Exp(types::BOOL);
}

Exp* LogicalBinop(Node* left, Node* right){
    if(left->type != types::BOOL || right->type != types::BOOL){
        output::errorMismatch(yylineno);
        exit(0);
    }

    return new Exp(types::BOOL);
}

Exp* Relop(Node* left, Node* right){
    if(!IsNumeric(left->type) || !IsNumeric(right->type)){
        output::errorMismatch(yylineno);
        exit(0);
    }

    return new Exp(types::BOOL);
}

Exp* Cast(Node* exp, Node* type){
    if(!IsNumeric(exp->type) || !IsNumeric(type->type)){
        output::errorMismatch(yylineno);
        exit(0);
    }

    return new Exp(type->type);
}

void BeginProgram(){

    offsets_stack.push_back(0);
    tables_stack.push_back(SymTab());

    assert(tables_stack.size() == 1);

    vector<types::TypeName> vec_print = {types::VOID, types::STRING};
    vector<types::TypeName> vec_printi = {types::VOID, types::INT};

    tables_stack[0].AddEntry("print", false,0, vec_print, NA);
    tables_stack[0].AddEntry("printi", false,0, vec_printi, NA);

}

void CheckProgram() {
    if(!IsExist("main")){
        output::errorMainMissing();
        exit(0);
    }

    auto main = GetEntry("main");
    if(main.offset != NA || main.types_list.size() != 1 || main.types_list[0] != types::VOID){
        output::errorMainMissing();
        exit(0);
    }

}

void EndProgram(){

    CloseScope();
}


inline bool IsValidImplicitCast(types::TypeName from, types::TypeName to){
    return from == to || (from == types::BYTE && to == types::INT);
}

inline bool IsNumeric(types::TypeName type){
    return type == types::INT || type == types::BYTE;
}