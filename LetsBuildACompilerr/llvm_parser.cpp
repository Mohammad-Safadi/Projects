#include "llvm_parser.hpp"
#include <cstdlib>

/* Static Functions */




static inline int GetIDOffset(const string& id_name){
	return GetEntry(id_name).offset;
}
static inline vector<pair<int,BranchLabelIndex>> makePairList(int address , BranchLabelIndex num ){
    return CodeBuffer::instance().makelist({address, num});;
}

static inline int emitNew(string toStack ){
    return  CodeBuffer::instance().emit(toStack);
}
LLVM::LLVM() : regs_num(0), str_counter(0){} //regcounter = 0

LLVM& LLVM::instance(){
	static LLVM inst;//only instance
	return inst;
}

string LLVM::genReg(){
    string ret = to_string(reg_counter);
    reg_counter++;
	return "%t" + ret;
}

vector<pair<int,BranchLabelIndex>> LLVM::genBrToLabel() {
	int address = emitNew("br label @");
	auto List = makePairList(address,FIRST);
	return List;
}


void LLVM::handleBoolExp(Exp* exp){
	string trueListAddress = CodeBuffer::instance().genLabel();
	int labelTrue = emitNew("br label @");
	string falseListAddres = CodeBuffer::instance().genLabel();
	int labelFalse = emitNew("br label @");
	string phiAddress = CodeBuffer::instance().genLabel();

	string val_reg = genReg();
	string toStack = val_reg + " = phi i32 [1, %" + trueListAddress + "] , [0, %" + falseListAddres + "]";
	emitNew(toStack);
	exp->val_reg = val_reg;

	CodeBuffer::instance().bpatch(exp->trueList, trueListAddress);
	CodeBuffer::instance().bpatch(exp->falseList, falseListAddres);
	CodeBuffer::instance().bpatch(makePairList(labelTrue, FIRST), phiAddress);
	CodeBuffer::instance().bpatch(makePairList(labelFalse, FIRST), phiAddress);
}



void LLVM::handleAssign(IDType* id, Exp* exp){	
	if(exp->type == types::BOOL){
		handleBoolExp(exp);
	}
	genStore(id, exp->val_reg);
}


void LLVM::handleFuncDecl(OverRide* OV,Node* ret_type, IDType* id, FormalsList* formals){
	GetCurrentFuncEntry().bp_reg = genFunDecl(OV->is_overRide,type_map.at(ret_type->type), id->name, formals->params);
}

void LLVM::handleClosingFuncDecl(){
    emitNew("}");
}

void LLVM::handleCall(Exp* exp, IDType* id, const vector<Exp*>& params){
	types::TypeName ret_type = GetEntry(id->name).types_list[0];
	string name = id->name;
	bool is_override =GetEntry(id->name).is_overRide;
	if(is_override){
	    name = name + to_string(GetEntry(id->name).rnd);
	}
	string ret_type_str = ret_type == types::VOID ? "void" : "i32";
	string code = "";

	if(ret_type != types::VOID){
		string val_reg = genReg();
		code += val_reg + " = ";
		exp->val_reg = val_reg;
	}

	code += "call " + ret_type_str + " @" + name + "(";


    vector<Exp*> r_params = params;
    std::reverse(r_params.begin(), r_params.end());

	for(auto param : r_params){
		string param_type = !id->name.compare("print") ? type_map.at(param->type) + " " : "i32 ";
		code += param_type + param->val_reg + ", ";
	}


	if(params.size() > 0){
		code.erase(code.size() - 2);
	}

	code += ")";

	emitNew(code);
		
}

void LLVM::genAllocaForFunctionBlock(string res_reg){
	emitNew(res_reg + " = alloca [50 x i32]");
}


void LLVM::handleOr(Exp* B, Exp* B1, Node* M, Exp* B2){
	CodeBuffer::instance().bpatch(B1->falseList, M->quad);
	B->trueList = CodeBuffer::instance().merge(B1->trueList, B2->trueList);
	B->falseList = B2->falseList;
}

void LLVM::handleAnd(Exp* B, Exp* B1, Node* M, Exp* B2){
	CodeBuffer::instance().bpatch(B1->trueList, M->quad);
	B->trueList = B2->trueList;
	B->falseList = CodeBuffer::instance().merge(B1->falseList, B2->falseList);
}

void LLVM::handleInit(Node* type, IDType* id, Exp* exp){

	if(type->type == types::BOOL){
		handleBoolExp(exp);
	}
	genStore(id, exp->val_reg);
}

void LLVM::handleReduceStringToExp(Exp* exp, char* yytext){
	string str(yytext);
	str = str.substr(1,str.length()-2);
	string str_name = genStrName();

	genGlobalStr(str, str_name);

	exp->val_reg = genStringReg(str.length(), str_name);
}

void LLVM::handleCast(Exp* left, types::TypeName type, Exp* right){
	/* int->byte or byte->int*/
	if(type != types::BYTE || right->type != types::INT){
		return;
	}	

	string trunc_reg = genReg();
	string res_reg = genReg();

	genTrunc32To8(trunc_reg, right->val_reg);
	genZext8To32(res_reg, trunc_reg);

	left->val_reg = res_reg;
}

void LLVM::handleProgramInit(){
	emitNew("declare i32 @printf(i8*, ...)");
	emitNew("declare void @exit(i32)");
  
	CodeBuffer::instance().emitGlobal("@.int_specifier = constant [4 x i8] c\"%d\\0A\\00\"");
	CodeBuffer::instance().emitGlobal("@.str_specifier = constant [4 x i8] c\"%s\\0A\\00\"");
    CodeBuffer::instance().emitGlobal("@.divError = constant [23 x i8] c\"Error division by zero\\00\"");
	
    stringstream code_printi;
	code_printi <<"define void @printi(i32) {" << endl
		 << "%spec_ptr = getelementptr [4 x i8], [4 x i8]* @.int_specifier, i32 0, i32 0" << endl
		 << "call i32 (i8*, ...) @printf(i8* %spec_ptr, i32 %0)" << endl
		 << "ret void" << endl 
		 << "}" << endl;
	emitNew(code_printi.str());
	
	stringstream code_print;
    code_print << "define void @print(i8*) {" << endl
		 << "%spec_ptr = getelementptr [4 x i8], [4 x i8]* @.str_specifier, i32 0, i32 0" << endl
		 << "call i32 (i8*, ...) @printf(i8* %spec_ptr, i8* %0)" << endl
		 << "ret void" << endl 
		 << "}" << endl;
    emitNew(code_print.str());
}


void LLVM::genBinop(string res_reg, const string& in_reg1, char op, const string& in_reg2){
	string strOp(1,op);
	const string& opAdd = op_map.at(strOp);
	emitNew(res_reg + " = " + opAdd  + " i32 " + in_reg1 + ", " + in_reg2);
}

void LLVM::genDivByZeroCheck(string in_reg2){
	string cond_res = genReg();
	emitNew(cond_res + " = icmp eq i32 " + in_reg2 + ", 0");
	int br_cmd = emitNew(" br i1 " + cond_res + ", label @, label @");
	string error_label = CodeBuffer::instance().genLabel();
	genPrintDivError();
	genExit();
	vector<pair<int, BranchLabelIndex>> address_list = LLVM::instance().genBrToLabel();
    string legal_label = CodeBuffer::instance().genLabel();
    
    CodeBuffer::instance().bpatch(makePairList(br_cmd, FIRST), error_label);
    CodeBuffer::instance().bpatch(makePairList(br_cmd, SECOND), legal_label);
	CodeBuffer::instance().bpatch(address_list, legal_label);   
}

void LLVM::genStore(IDType* id, const string& val_reg){
	int offset = GetEntry(id->name).offset;

	string bp_reg = GetCurrentFuncBP();
	string ptr_reg = genReg();
	string code;
	
	code = ptr_reg + " = getelementptr [50 x i32], [50 x i32]* " + bp_reg 
		+ ", i32 0, i32 " + to_string(offset) + "\n";
	code += "store i32 " + val_reg + ", i32* " + ptr_reg;

	emitNew(code);
}

void LLVM::genZext8To32(const string& zext_res, const string& in_reg){
    string statement = zext_res +  " = zext i8 " ;
    statement += in_reg + " to i32" ;
	emitNew(statement);
}

void LLVM::genTrunc32To8(const string& trunc_res, const string& in_reg){
    string statement = trunc_res + " = trunc i32 ";
    statement += in_reg + " to i8";
	emitNew(statement);
}

void LLVM::genPrintDivError(){
    string statement = "call void @print(i8* ";
    statement += genStringReg(22, "@.divError");
    statement += ")";

	emitNew(statement);
}

void LLVM::genExit(){
    string statement = "call void @exit(i32 0)" ;
   emitNew(statement);
}

void LLVM::genReturn(types::TypeName ret_type, const string& val_reg){
    string type = "void";
    if(ret_type != types::VOID){
        type = "i32";
    }
	emitNew("ret " + type + " " + val_reg);
}

string LLVM::genFunDecl( bool is_override,string ret_type, string& id, vector<FormalDecl>& params){
    int sizeCheck = 0;
    if(is_override){
        int rnd = GetEntry(id).rnd;
        id = id + to_string(rnd);
    }
    if(ret_type != "void"){
        ret_type = "i32";
    } else{
        ret_type = "void";
    }

	string d_line = "define " + ret_type + " @" + id + " (";
	for(auto param : params){
	    sizeCheck++;
            d_line += "i32, ";
	}
    int dSize = d_line.size();
	if(params.size() > 0) {
        d_line.erase(dSize - 2);
    }
    d_line += ") {";
	emitNew(d_line);
	
	string r_reg = genReg();
	genAllocaForFunctionBlock(r_reg);
	
	return r_reg;
}

void LLVM::genCompare(Exp* exp){
	string cmp_reg = genReg();

	string code = cmp_reg + " = " + "icmp eq i32 1, " + exp->val_reg + "\n";
	emitNew(code);

	code = "br i1 " + cmp_reg + ", label @, label @";
	int address = emitNew(code);

	exp->trueList = makePairList(address, FIRST);
	exp->falseList = makePairList(address, SECOND);
}

string LLVM::genStrName(){
	string res = "@.string" + to_string(str_counter++);
	return res;
}

void LLVM::genGlobalStr(const string& str, const string& str_name){
    string lenStr = to_string(str.length() + 1);
    string toEmit = str_name + " = internal constant [" + lenStr + " x i8] c\""+ str;
	CodeBuffer::instance().emitGlobal( toEmit+ "\\00\"");
}


string LLVM::genStringReg(int length, const string& name){
	string address_reg = genReg();
    string lenStr = to_string(length + 1);
	string type = "[" + lenStr + " x i8]";
	string code = address_reg + " = getelementptr " + type + ", " + type + "* "
			+ name + ", i32 0, i32 0";

	
	emitNew(code);

	return address_reg;
}