#include <map>
#include <set>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>

using namespace std;

struct sym { int kind; int type; string value; int addr; };
struct midcode { string result; string left; int op; string right; set<string> out; };
enum classnum {
	IDENFR, INTCON, CHARCON, STRCON, CONSTTK, INTTK, CHARTK, VOIDTK, MAINTK, IFTK, ELSETK, DOTK, WHILETK, FORTK, SCANFTK, PRINTFTK,
	RETURNTK, PLUS, MINU, MULT, DIV, LSS, LEQ, GRE, GEQ, EQL, NEQ, ASSIGN, SEMICN, COMMA, LPARENT, RPARENT, LBRACK, RBRACK, LBRACE, RBRACE
};
enum midop {
	NEG, ADD, SUB, MUL, DIVI, ASS, GETARR, PUTARR, SETLABEL, GOTO, BNZ, BZ, LESS, LE, GREAT,
	GE, EQUAL, UNEQUAL, READ, WSTRING, WEXP, WSE, RET, RETX, PARA, FUNCALL, FUN, PAR, ASSRET
};

ifstream in;
ofstream out;
ofstream err;
ofstream mid;

char c;
int line_num;
string level;
int tmpnum;
int labelnum;
int stringnum;
int enter;
int symnum;
bool haveReturn;
bool haveReturnInt;
bool haveReturnChar;
string word;
string classCode[36];

map<string, map<string, struct sym> > symList;
map<string, map<string, struct sym> > tmpList;
map<string, int> reserved;
map<string, int> fun_idenfr;
map<string, vector<string> > fun_para;
vector <struct midcode> midCodes;
map<string, vector<int> > fun_mids;

int getsym();
int isReserved(string s);
int isExpression(string& var);
int isTerm(string& var);
int isFactor(string& var);

string isIdenfr();
string isInteger();
string isString();
string getLabel();
string isFun_call(string idenfr);
string getTmp();
string getString(string s);

bool isAlpha(char ch);

void outputMidcodes();
void error(char errNum);
void init_code(string classCode[36]);
void init_reserved();
void isProgram();
void isConst();
void isVar(int type, string idenfr);
void isFun_return(int type, string idenfr);
void isParaList(string name);
void isFun_void();
void isMain();
void outputsym();
void symInsert(string name, struct sym sym);
void isStatements();
void isStatement();
void isConditional();
void isLoop();
void isAssign(string idenfr);
void isScanf();
void isCompound();
void addMidCode(string result, string left, int op, string right);
void isPrintf();
void isReturn();
void isCondition(string& con);

map<string, string> isValueList(string name);

int main() {
	map<string, struct sym> all;
	map<string, struct sym> all_tmp;
	line_num = 1;
	tmpnum = 0;
	labelnum = 1;
	level = "$all";
	symList[level] = all;
	tmpList[level] = all_tmp;
	init_code(classCode);
	init_reserved();

	in.open("testfile.txt", ios_base::in);
	out.open("output.txt", ios_base::out | ios_base::trunc);
	err.open("err.txt", ios_base::out | ios_base::trunc);

	c = in.get();
	word = "";
	symnum = getsym();
	cout << "|||||" << symnum << endl;
	isProgram();
	out.close();

	if (in.get() != EOF) { error('0'); }
	in.close();

	return 0;
}

void init_code(string classCode[36]) {
	classCode[0] = "IDENFR";
	classCode[1] = "INTCON";
	classCode[2] = "CHARCON";
	classCode[3] = "STRCON";
	classCode[4] = "CONSTTK";
	classCode[5] = "INTTK";
	classCode[6] = "CHARTK";
	classCode[7] = "VOIDTK";
	classCode[8] = "MAINTK";
	classCode[9] = "IFTK";
	classCode[10] = "ELSETK";
	classCode[11] = "DOTK";
	classCode[12] = "WHILETK";
	classCode[13] = "FORTK";
	classCode[14] = "SCANFTK";
	classCode[15] = "PRINTFTK";
	classCode[16] = "RETURNTK";
	classCode[17] = "PLUS";
	classCode[18] = "MINU";
	classCode[19] = "MULT";
	classCode[20] = "DIV";
	classCode[21] = "LSS";
	classCode[22] = "LEQ";
	classCode[23] = "GRE";
	classCode[24] = "GEQ";
	classCode[25] = "EQL";
	classCode[26] = "NEQ";
	classCode[27] = "ASSIGN";
	classCode[28] = "SEMICN";
	classCode[29] = "COMMA";
	classCode[30] = "LPARENT";
	classCode[31] = "RPARENT";
	classCode[32] = "LBRACK";
	classCode[33] = "RBRACK";
	classCode[34] = "LBRACE";
	classCode[35] = "RBRACE";
}

void outputsym() {
	if (symnum >= 0 && symnum < 36)
	{
		cout << classCode[symnum] << " " << word << endl;
		out << classCode[symnum] << " " << word << endl;
	}

	if (symnum == 1)
	{
		cout << "<无符号整数>" << endl;
		out << "<无符号整数>" << endl;
	}

}

void init_reserved() {
	reserved["const"] = 4;
	reserved["int"] = 5;
	reserved["char"] = 6;
	reserved["void"] = 7;
	reserved["main"] = 8;
	reserved["if"] = 9;
	reserved["else"] = 10;
	reserved["do"] = 11;
	reserved["while"] = 12;
	reserved["for"] = 13;
	reserved["scanf"] = 14;
	reserved["printf"] = 15;
	reserved["return"] = 16;
}

void error(char errNum) {
	if (errNum == 'k') err << line_num - enter;
	else err << line_num;
	err << " " << errNum << endl;
	return;
}

int getsym() {
	word = "";
	enter = 0;
	while (isspace(c)) {
		if (c == '\n') {
			line_num++;
			enter++;
		}
		c = in.get();
	}
	// 从第一个不为空白符的字符c起，分析一个单词
	if (isAlpha(c)) {
		word = isIdenfr();
		return isReserved(word);
	}
	else if (isdigit(c)) {
		word = isInteger();
		return 1;
	}
	else if (c == '\'') {
		c = in.get();
		if (c == '+' || c == '-' || c == '*' || c == '/' || isdigit(c) || isAlpha(c)) {
			word = word + c;
		}
		else {
			error('a');
		}
		c = in.get();
		if (c != '\'') {
			error('a');
		}
		else {
			c = in.get();
		}
		return 2;
	}
	else if (c == '\"') {
		c = in.get();
		word = isString();
		if (c != '\"') {
			error('a');
		}
		else {
			c = in.get();
		}
		return 3;
	}
	else if (c == '+') {
		word = word + c;
		c = in.get();
		return 17;
	}
	else if (c == '-') {
		word = word + c;
		c = in.get();
		return 18;
	}
	else if (c == '*') {
		word = word + c;
		c = in.get();
		return 19;
	}
	else if (c == '/') {
		word = word + c;
		c = in.get();
		return 20;
	}
	else if (c == '<') {
		word = word + c;
		c = in.get();
		if (c != '=') {
			return 21;
		}
		else {
			word = word + c;
			c = in.get();
			return 22;
		}
	}
	else if (c == '>') {
		word = word + c;
		c = in.get();
		if (c != '=') {
			return 23;
		}
		else {
			word = word + c;
			c = in.get();
			return 24;
		}
	}
	else if (c == '=') {
		word = word + c;
		c = in.get();
		if (c != '=') {
			return 27;
		}
		else {
			word = word + c;
			c = in.get();
			return 25;
		}
	}
	else if (c == '!') {
		word = word + c;
		c = in.get();
		if (c != '=') {
			error('a');
			return 26;
		}
		else {
			word = word + c;
			c = in.get();
			return 26;
		}
	}
	else if (c == ';') {
		word = word + c;
		c = in.get();
		return 28;
	}
	else if (c == ',') {
		word = word + c;
		c = in.get();
		return 29;
	}
	else if (c == '(') {
		word = word + c;
		c = in.get();
		return 30;
	}
	else if (c == ')') {
		word = word + c;
		c = in.get();
		return 31;
	}
	else if (c == '[') {
		word = word + c;
		c = in.get();
		return 32;
	}
	else if (c == ']') {
		word = word + c;
		c = in.get();
		return 33;
	}
	else if (c == '{') {
		word = word + c;
		c = in.get();
		return 34;
	}
	else if (c == '}') {
		word = word + c;
		c = in.get();
		return 35;
	}
	else {
		error('a');
		return -1;
	}
}

bool isAlpha(char ch) {
	if (isalpha(ch) || ch == '_')
		return true;
	else
		return false;
}

string isIdenfr() {
	string s = "";
	s = s + c;
	c = in.get();
	while (isAlpha(c) || isdigit(c)) {
		s = s + c;
		c = in.get();
	}
	return s;
}

string isInteger() {
	//int n = c - '0';
	string s = "";
	s = s + c;
	c = in.get();
	if (s == "0")
		return s;
	while (isdigit(c)) {
		//n = n * 10 + (c - '0');
		s = s + c;
		c = in.get();
	}
	return s;
}

string isString() {
	string s = "";
	while ((int)c == 32 || (int)c == 33 || ((int)c >= 35 && (int)c <= 126)) {
		if (c == '\\')
		{
			s = s + '\\';
		}
		else
			s = s + c;
		c = in.get();
	}
	return s;
}

int isReserved(string s) {
	if (reserved.find(s) == reserved.end())
		return 0;
	else
		return reserved[s];
}

void isProgram() {
	int type;
	string idenfr;
	bool isval = false;
	// 处理常量说明
	if (symnum == CONSTTK) {
		isConst();
	}
	// 处理变量说明和第一个有返回值函数定义
	if (symnum == INTTK || symnum == CHARTK) {
		type = symnum;
		symnum = getsym();
		if (symnum == IDENFR) {
			idenfr = word;
			symnum = getsym();
			// 进入变量说明分支
			while (symnum == COMMA || symnum == SEMICN || symnum == LBRACK) {
				isval = true;
				isVar(type, idenfr);
				if (symnum == INTTK || symnum == CHARTK) {
					type = symnum;
					symnum = getsym();
					if (symnum == IDENFR) {
						idenfr = word;
						symnum = getsym();
					}
					else { error('0'); }
				}
				else {
					// 碰到void或错误，跳出，错误在最后处理
					if (isval) {
						out << "<变量说明>" << endl; cout << "<变量说明>" << endl;
					}
					break;
				}
			}
			// 对于正常结束的while循环，查看是否为有返回值函数定义
			if (symnum == LPARENT) {
				if (isval) { out << "<变量说明>" << endl; cout << "<变量说明>" << endl; }
				isFun_return(type, idenfr);
			}
		}
		else { error('0'); }
	}
	// 处理有、无返回值的函数定义和主函数
	while (symnum == VOIDTK || symnum == INTTK || symnum == CHARTK) {
		if (symnum == VOIDTK) {
			outputsym();
			symnum = getsym();
			// 主函数处理完直接返回
			if (symnum == MAINTK) {
				isMain();
				{out << "<程序>" << endl; cout << "<程序>" << endl; }
				return;
				// 无返回值函数定义处理完继续循环
			}
			else if (symnum == IDENFR) {
				isFun_void();
			}
			else { error('0'); }
			// 有返回值函数定义的处理
		}
		else {
			type = symnum;
			symnum = getsym();
			if (symnum == IDENFR) {
				idenfr = word;
				symnum = getsym();
				isFun_return(type, idenfr);
			}
			else { error('0'); }
		}
	}
	// 没有主函数的错误
	error('0');
	return;
}

void isConst() {
	int type;
	string name;
	struct sym sym = { 1, 5, "" };
	if (symnum != CONSTTK) { error('0'); }
	outputsym();

	symnum = getsym();
	if (symnum != INTTK && symnum != CHARTK) { error('0'); }
	type = symnum;
	sym.type = symnum;
	outputsym();

	symnum = getsym();
	if (symnum != IDENFR) { error('0'); }
	while (symnum == IDENFR) {
		sym.value = "";
		name = word;
		outputsym();

		symnum = getsym();
		if (symnum != ASSIGN) { error('0'); }
		outputsym();

		symnum = getsym();
		if (symnum == PLUS || symnum == MINU) {
			if (symnum == MINU) sym.value = word;
			outputsym();
			symnum = getsym();
		}
		if (!(symnum == INTCON && type == INTTK) &&
			!(symnum == CHARCON && type == CHARTK)) {
			error('o');
		}
		if (symnum == INTCON) sym.value = sym.value + word;
		else sym.value = (int)(word[0]) + '0';//sym.value = to_string((int)(word[0]));
		outputsym();
		if (symnum == INTCON)
		{
			out << "<整数>" << endl;
			cout << "<整数>" << endl;
		}


		symInsert(name, sym);
		symnum = getsym();
		if (symnum == COMMA) {
			// 逗号在常量定义内部循环
			outputsym();
			symnum = getsym();
		}
		else if (symnum == SEMICN) {
			break;
		}
		else { error('k');  break; }
	}
	// 分号结束本次常量定义循环
	out << "<常量定义>" << endl;
	cout << "<常量定义>" << endl;
	outputsym();
	symnum = getsym();
	if (symnum == CONSTTK) {
		isConst();
	}
	else {
		// 分号后不再是常量定义循环，常量说明结束.
		out << "<常量说明>" << endl;
		cout << "<常量说明>" << endl;
		return;
	}
}

void isVar(int type, string idenfr) {
	string name = idenfr;
	struct sym sym = { 0, type };
	if (type == INTTK) {
		out << "INTTK int" << endl;
		cout << "INTTK int" << endl;
	}
	else if (type == CHARTK) { out << "CHARTK char" << endl; cout << "CHARTK char" << endl; }

	out << "IDENFR" << " " << idenfr << endl;
	cout << "IDENFR" << " " << idenfr << endl;
	while (symnum == SEMICN || symnum == COMMA || symnum == LBRACK) {
		if (symnum == SEMICN) {
			// 一遍变量定义结束
			symInsert(name, sym);
			out << "<变量定义>" << endl;
			cout << "<变量定义>" << endl;
			outputsym();
			symnum = getsym();
			return;
		}
		else if (symnum == COMMA) {
			symInsert(name, sym);
			sym.type = type;
			outputsym();
			symnum = getsym();
			if (symnum == IDENFR) {
				name = word;
				outputsym();
				symnum = getsym();
				// 变量定义大括号循环
			}
			else { error('0'); }
		}
		else if (symnum == LBRACK) {
			if (level != "$all") symList["$all"][level].value = "f";
			sym.type = type - 2;
			outputsym();
			symnum = getsym();
			if (symnum != INTCON || word == "0") { error('i'); }
			sym.value = word;
			outputsym();
			symnum = getsym();
			if (symnum == RBRACK) {
				outputsym();
				symnum = getsym();
				// 变量定义大括号循环
			}
			else { error('m'); }
		}
	}
	error('k');
}

void isFun_return(int type, string idenfr) {
	extern bool haveReturn;
	extern bool haveReturnInt;
	extern bool haveReturnChar;
	map<string, struct sym> map1;
	map<string, struct sym> map_tmp;
	if (type == INTTK) {
		out << "INTTK int" << endl;
		cout << "INTTK int" << endl;
		addMidCode("", "int", FUN, idenfr);
	}
	else if (type == CHARTK) {
		out << "CHARTK char" << endl;
		cout << "CHARTK char" << endl;
		addMidCode("", "char", FUN, idenfr);
	}
	out << "IDENFR" << " " << idenfr << endl;
	out << "<声明头部>" << endl;
	cout << "IDENFR" << " " << idenfr << endl;
	cout << "<声明头部>" << endl;
	fun_idenfr[idenfr] = 1;
	sym dad = { 2, type, "t" };
	symInsert(idenfr, dad);

	level = idenfr;
	symList[level] = map1;
	tmpList[level] = map_tmp;

	if (symnum != LPARENT) { error('0'); }
	outputsym();
	symnum = getsym();

	isParaList(idenfr);

	if (symnum != RPARENT) { error('l'); }
	else {
		outputsym();
		symnum = getsym();
	}

	if (symnum != LBRACE) { error('0'); }
	outputsym();
	symnum = getsym();

	haveReturn = false;
	haveReturnInt = false;
	haveReturnChar = false;
	isCompound();

	if (symnum != RBRACE) { error('0'); }
	outputsym();
	out << "<有返回值函数定义>" << endl;
	cout << "<有返回值函数定义>" << endl;
	symnum = getsym();
	level = "$all";
}

void isParaList(string name) {
	vector<string> para;
	struct sym sym = { 3, 0 };
	string type;
	while (symnum == INTTK || symnum == CHARTK) {
		type = word;
		sym.type = symnum;
		outputsym();

		symnum = getsym();
		if (symnum != IDENFR) { error('0'); }
		symInsert(word, sym);
		para.push_back(word);
		addMidCode("", type, PAR, word);
		outputsym();

		symnum = getsym();
		if (symnum != COMMA) {
			fun_para[name] = para;
			out << "<参数表>" << endl;
			cout << "<参数表>" << endl;
			return;
		}
		outputsym();

		symnum = getsym();
	}
	if (symnum == RPARENT) {
		fun_para[name] = para;
		out << "<参数表>" << endl;
		cout << "<参数表>" << endl;
	}
	else
		error('0');
}

void isFun_void() {
	extern bool haveReturn;
	extern bool haveReturnInt;
	extern bool haveReturnChar;
	string name = word;
	map<string, struct sym> map1;
	map<string, struct sym> map_tmp;
	if (symnum != IDENFR) { error('0'); }
	outputsym();
	fun_idenfr[word] = 0;
	sym dadad = { 2, 0, "t" };
	symInsert(word, dadad);
	addMidCode("", "void", FUN, word);
	level = word;
	symList[level] = map1;
	tmpList[level] = map_tmp;
	symnum = getsym();

	if (symnum != LPARENT) { error('0'); }
	outputsym();
	symnum = getsym();

	isParaList(name);

	if (symnum != RPARENT) { error('l'); }
	else {
		outputsym();
		symnum = getsym();
	}

	if (symnum != LBRACE) { error('0'); }
	outputsym();
	symnum = getsym();

	haveReturn = false;
	haveReturnInt = false;
	haveReturnChar = false;
	isCompound();

	addMidCode("", "", RET, "");
	if (symnum != RBRACE) { error('0'); }
	outputsym();
	out << "<无返回值函数定义>" << endl;
	cout << "<无返回值函数定义>" << endl;
	symnum = getsym();
	level = "$all";
}

void isMain() {
	extern bool haveReturn;
	extern bool haveReturnInt;
	extern bool haveReturnChar;
	map<string, struct sym> map1;
	map<string, struct sym> map_tmp;
	sym dad = { 2, 0, "t" };
	symInsert("main", dad);
	addMidCode("", "void", FUN, "main");
	level = "main";
	symList[level] = map1;
	tmpList[level] = map_tmp;
	if (symnum != MAINTK) { error('0'); }
	outputsym();
	symnum = getsym();

	if (symnum != LPARENT) { error('0'); }
	outputsym();
	symnum = getsym();

	if (symnum != RPARENT) { error('l'); }
	else {
		outputsym();
		symnum = getsym();
	}

	if (symnum != LBRACE) { error('0'); }
	outputsym();
	symnum = getsym();

	haveReturn = false;
	haveReturnInt = false;
	haveReturnChar = false;
	isCompound();

	if (symnum != RBRACE) { error('0'); }
	outputsym();
	out << "<主函数>" << endl;
	cout << "<主函数>" << endl;
	level = "$all";
}

void symInsert(string name, struct sym sym) {
	if (symList[level].find(name) == symList[level].end()) {
		symList[level][name] = sym;
	}
	else { error('b'); }
}

void isCompound() {
	bool isval = false;
	int type;
	string idenfr;
	// 常量说明
	if (symnum == CONSTTK) {
		isConst();
	}
	// 变量定义
	while (symnum == INTTK || symnum == CHARTK) {
		isval = true;
		type = symnum;
		symnum = getsym();

		if (symnum != IDENFR) { error('0'); }
		idenfr = word;
		symnum = getsym();

		isVar(type, idenfr);
	}
	if (isval) { cout << "<变量说明>" << endl; out << "<变量说明>" << endl; }
	// 语句列
	isStatements();
	cout << "<复合语句>" << endl;
	out << "<复合语句>" << endl;
}

void isStatements() {
	while (symnum == IFTK || symnum == WHILETK || symnum == DOTK ||
		symnum == FORTK || symnum == LBRACE || symnum == IDENFR ||
		symnum == SCANFTK || symnum == PRINTFTK ||
		symnum == RETURNTK || symnum == SEMICN) {
		isStatement();
	}
	cout << "<语句列>" << endl;
	out << "<语句列>" << endl;
}

void isStatement() {
	string idenfr;
	if (symnum == IFTK) {
		// 条件语句
		isConditional();
		cout << "<语句>" << endl;
		out << "<语句>" << endl;
		return;
	}
	else if (symnum == WHILETK || symnum == DOTK || symnum == FORTK) {
		// 循环语句
		isLoop();
		cout << "<语句>" << endl;
		out << "<语句>" << endl;
		return;
	}
	else if (symnum == LBRACE) {
		outputsym();
		//语句列
		symnum = getsym();
		isStatements();
		if (symnum != RBRACE) { error('0'); }
		outputsym();
		symnum = getsym();
		cout << "<语句>" << endl;
		out << "<语句>" << endl;
		return;
	}
	else if (symnum == IDENFR) {
		idenfr = word;
		outputsym();
		symnum = getsym();
		if (symnum == LPARENT) {
			// 有/无返回值函数调用语句
			if (symList[level].find(idenfr) == symList[level].end()
				&& symList["$all"].find(idenfr) == symList["$all"].end()) {
				error('c');
			}
			isFun_call(idenfr);
		}
		else if (symnum == ASSIGN || symnum == LBRACK) {
			// 赋值语句
			if (symList[level].find(idenfr) == symList[level].end()) {
				if (symList["$all"].find(idenfr) == symList["$all"].end()) { error('c'); }
				else if (symList["$all"][idenfr].kind == 1) { error('j'); }
			}
			else if (symList[level][idenfr].kind == 1) { error('j'); }
			isAssign(idenfr);
		}
		else { error('0'); }
	}
	else if (symnum == SCANFTK) {
		// 读语句
		isScanf();
	}
	else if (symnum == PRINTFTK) {
		// 写语句
		isPrintf();
	}
	else if (symnum == RETURNTK) {
		// 返回语句
		isReturn();
	}
	if (symnum != SEMICN) { error('k'); }
	else {
		outputsym();
		symnum = getsym();
	}
	cout << "<语句>" << endl;
	out << "<语句>" << endl;
}

string getLabel() {
	stringstream ss;
	string s;
	ss << "label$" << labelnum++;
	s = ss.str();
	return s;
}

string getTmp() {
	stringstream ss;
	string s;
	struct sym dead = { 0,5 };
	ss << "t$" << tmpnum++;
	s = ss.str();
	map<string, struct sym> k = tmpList[level];
	k[s] = dead;
	//tmpList[level][s] = { 0, 5};
	return s;
}

void isCondition(string& con) {
	string tmp1, tmp2;
	int type = isExpression(tmp1);
	int opcode;
	if (type != 5) { error('f'); }
	if (symnum >= LSS && symnum <= NEQ) {
		opcode = symnum - LSS + LESS;
		outputsym();
		symnum = getsym();
		type = isExpression(tmp2);
		con = getTmp();
		addMidCode(con, tmp1, opcode, tmp2);
		if (type != 5) { error('f'); }
	}
	else {
		con = tmp1;
	}
	cout << "<条件>" << endl;
	out << "<条件>" << endl;
}

int isExpression(string& var) {
	int type = -1;
	int opcode = -1;
	string tmp1, tmp2, tmp3;
	if (symnum == PLUS || symnum == MINU) {
		if (symnum == MINU) opcode = NEG;
		outputsym();
		symnum = getsym();
	}
	type = isTerm(tmp1);
	if (opcode == 0) {
		tmp3 = getTmp();
		addMidCode(tmp3, tmp1, opcode, "");
		tmp1 = tmp3;
	}
	while (symnum == PLUS || symnum == MINU) {
		if (symnum == PLUS) opcode = ADD;
		else if (symnum == MINU) opcode = SUB;
		outputsym();
		symnum = getsym();
		isTerm(tmp2);
		tmp3 = getTmp();
		addMidCode(tmp3, tmp1, opcode, tmp2);
		tmp1 = tmp3;
		type = 5;
	}
	var = tmp1;
	out << "<表达式>" << endl;
	////cout << "<表达式>" << endl;
	return type;
}

int isTerm(string& var) {
	string tmp1, tmp2, tmp3;
	int type = isFactor(tmp1);
	int opcode = -1;
	while (symnum == MULT || symnum == DIV) {
		if (symnum == MULT) opcode = MUL;
		else if (symnum == DIV) opcode = DIVI;
		outputsym();
		symnum = getsym();
		isFactor(tmp2);
		tmp3 = getTmp();
		addMidCode(tmp3, tmp1, opcode, tmp2);
		tmp1 = tmp3;
		type = 5;
	}
	var = tmp1;
	out << "<项>" << endl;
	//cout << "<项>" << endl;
	return type;
}

int isFactor(string& var) {
	extern map<string, int> fun_idenfr;
	string idenfr;
	int type = -1;
	int opcode = -1;
	string tmp;
	if (symnum == LPARENT) {
		// （表达式）
		outputsym();
		symnum = getsym();
		type = isExpression(var);
		
		outputsym();
		symnum = getsym();
	}
	else if (symnum == PLUS || symnum == MINU) {
		// 带+-号的整数
		type = 5;
		if (symnum == MINU) opcode = NEG;
		outputsym();
		symnum = getsym();
		
		outputsym();
		if (opcode == NEG) var = "-" + word;
		else var = word;
		out << "<整数>" << endl;
		////cout << "<整数>" << endl;
		symnum = getsym();
	}
	else if (symnum == INTCON) {
		// 无符号整数
		type = 5;
		outputsym();
		var = word;
		out << "<整数>" << endl;
		////cout << "<整数>" << endl;
		symnum = getsym();
	}
	else if (symnum == CHARCON) {
		// 字符
		type = 6;
		outputsym();
		var = (int)(word[0]) + '0';//to_string((int)(word[0]));
		symnum = getsym();
	}
	else if (symnum == IDENFR) {
		if (symList[level].find(word) == symList[level].end()) {
			if (symList["$all"].find(word) == symList["$all"].end()) {
				type = 5;
			}
			else { type = symList["$all"][word].type; }
		}
		else { type = symList[level][word].type; }
		idenfr = word;
		outputsym();
		symnum = getsym();
		if (symnum == LPARENT) {
			// 有返回值函数调用语句
			
			var = isFun_call(idenfr);
			if (symList["$all"][idenfr].value == "f") {
				var = getTmp();
				addMidCode(var, "", ASSRET, "");
			}
		}
		else if (symnum == LBRACK) {
			// 数组 标识符[表达式]
			symList["$all"][level].value = "f";
			outputsym();
			symnum = getsym();
			
			var = getTmp();
			addMidCode(var, idenfr, GETARR, tmp);
			type = type + 2;
			outputsym();
			symnum = getsym();
		}
		else {
			// 单纯只是标识符
			if (symList[level].find(idenfr) != symList[level].end()) {
				if (symList[level][idenfr].kind == 1)
					var = symList[level][idenfr].value;
				else
					var = idenfr;
			}
			else if (symList["$all"].find(idenfr) != symList["$all"].end() && symList["$all"][idenfr].kind == 1)
				var = symList["$all"][idenfr].value;
			else {
				if (symList["$all"].find(idenfr) != symList["$all"].end() && symList["$all"][idenfr].kind == 0)
					symList["$all"][level].value = "f";
				var = idenfr;
			}
		}
	}
	
	out << "<因子>" << endl;
	////cout << "<因子>" << endl;
	return type;
}

void isConditional() {
	string con;
	string label1 = getLabel(), label2;
	symList["$all"][level].value = "f";
	
	outputsym();
	symnum = getsym();
	
	outputsym();
	symnum = getsym();
	
	isCondition(con);
	addMidCode(label1, con, BZ, "");
	
	outputsym();
	symnum = getsym();
	
	isStatement();
	
	if (symnum == ELSETK) {
		label2 = getLabel();
		addMidCode(label2, "", GOTO, "");
		addMidCode(label1, "", SETLABEL, "");
		outputsym();
		symnum = getsym();
		isStatement();
		addMidCode(label2, "", SETLABEL, "");
	}
	else {
		addMidCode(label1, "", SETLABEL, "");
	}
	////cout << "<条件语句>" << endl;
	out << "<条件语句>" << endl;
}

void isLoop() {
	int opcode = -1, i, j;
	string idenfr, idenfr2, step, con;
	string label1 = getLabel(), label2;
	vector<int> midTmpIndex;
	vector<struct midcode> midTmp;
	extern vector <struct midcode> midCodes;
	
	symList["$all"][level].value = "f";
	if (symnum == WHILETK) {
		label2 = getLabel();
		midTmpIndex.emplace_back(midCodes.size());
		addMidCode(label1, "", SETLABEL, "");
		outputsym();
		symnum = getsym();
		
		
		outputsym();
		symnum = getsym();
		
		midTmpIndex.emplace_back(midCodes.size());
		isCondition(con);
		
		midTmpIndex.emplace_back(midCodes.size());
		addMidCode(label2, con, BZ, "");
		
		
		
		outputsym();
		symnum = getsym();
		
		
		midTmpIndex.emplace_back(midCodes.size());
		isStatement();
		
		midTmpIndex.emplace_back(midCodes.size());
		addMidCode(label1, "", GOTO, "");
		midTmpIndex.emplace_back(midCodes.size());
		addMidCode(label2, "", SETLABEL, "");
		midTmpIndex.emplace_back(midCodes.size());
		// 调整中间代码顺序
		for (i = midTmpIndex[4]; i < midTmpIndex[6]; i++)
			midTmp.emplace_back(midCodes[i]);
		for (i = midTmpIndex[3]; i < midTmpIndex[4]; i++)
			midTmp.emplace_back(midCodes[i]);
		for (i = midTmpIndex[0]; i < midTmpIndex[2]; i++)
			midTmp.emplace_back(midCodes[i]);
		struct midcode m = midCodes[midTmpIndex[2]];
		m.op = BNZ;
		midTmp.emplace_back(m);
		for (i = midTmpIndex[0], j = 0; i < midTmpIndex[6]; i++, j++)
			midCodes[i] = midTmp[j];
	}
	else if (symnum == DOTK) {
		addMidCode(label1, "", SETLABEL, "");
		outputsym();
		symnum = getsym();
		
		isStatement();
		
		isCondition(con);
		addMidCode(label1, con, BNZ, "");
		
		
	}
	else if (symnum == FORTK) {
		label2 = getLabel();
		outputsym();
		symnum = getsym();
		
		
		outputsym();
		symnum = getsym();
		
		idenfr = word;
		outputsym();
		symnum = getsym();
		
		outputsym();
		symnum = getsym();
		
		isExpression(idenfr2);
		midTmpIndex.emplace_back(midCodes.size());
		addMidCode(idenfr, idenfr2, ASS, "");
		
		outputsym();
		symnum = getsym();
		
		midTmpIndex.emplace_back(midCodes.size());
		addMidCode(label1, "", SETLABEL, "");
		
		midTmpIndex.emplace_back(midCodes.size());
		isCondition(con);
		
		midTmpIndex.emplace_back(midCodes.size());
		addMidCode(label2, con, BZ, "");
		
		outputsym();
		symnum = getsym();
		
		idenfr = word;
		outputsym();
		symnum = getsym();
		
		outputsym();
		symnum = getsym();
		
		idenfr2 = word;
		outputsym();
		symnum = getsym();
		
		if (symnum == PLUS) opcode = ADD;
		else if (symnum == MINU) opcode = SUB;
		outputsym();
		symnum = getsym();
		
		step = word;
		outputsym();
		////cout << "<步长>" << endl;
		out << "<步长>" << endl;
		symnum = getsym();
		
		outputsym();
		symnum = getsym();
		
		midTmpIndex.emplace_back(midCodes.size());
		isStatement();
		
		midTmpIndex.emplace_back(midCodes.size());
		addMidCode(idenfr, idenfr2, opcode, step);
		midTmpIndex.emplace_back(midCodes.size());
		addMidCode(label1, "", 9, "");
		midTmpIndex.emplace_back(midCodes.size());
		addMidCode(label2, "", 8, "");
		midTmpIndex.emplace_back(midCodes.size());
		// 调整中间代码顺序
		for (i = midTmpIndex[0]; i < midTmpIndex[1]; i++)
			midTmp.emplace_back(midCodes[i]);
		for (i = midTmpIndex[6]; i < midTmpIndex[8]; i++)
			midTmp.emplace_back(midCodes[i]);
		for (i = midTmpIndex[4]; i < midTmpIndex[6]; i++)
			midTmp.emplace_back(midCodes[i]);
		for (i = midTmpIndex[1]; i < midTmpIndex[3]; i++)
			midTmp.emplace_back(midCodes[i]);
		struct midcode m = midCodes[midTmpIndex[3]];
		m.op = BNZ;
		midTmp.emplace_back(m);
		for (i = midTmpIndex[0], j = 0; i < midTmpIndex[8]; i++, j++)
			midCodes[i] = midTmp[j];
	}
	
	//cout << "<循环语句>" << endl;
	out << "<循环语句>" << endl;
}

map<string, string> isValueList(string name) {
	extern map<string, vector<string> > fun_para;
	extern vector <struct midcode> midCodes;
	vector<int> valueList;
	vector <struct midcode> midTmp;
	map<string, string> funPara;
	int i = 0, type = 0, j = 0;
	string tmp;
	if (symnum != RPARENT) {
		valueList.emplace_back(int(midCodes.size()) - 1);
		type = isExpression(tmp);
		if (i >= fun_para[name].size()) {
			while (getsym() != RPARENT)
				return funPara;
		}
		
		valueList.emplace_back(int(midCodes.size()));
		addMidCode(name, tmp, PARA, fun_para[name][i++]);
		while (symnum == COMMA) {
			outputsym();
			symnum = getsym();
			type = isExpression(tmp);
			if (i >= fun_para[name].size()) {
				
				while (getsym() != RPARENT);
				return funPara;
			}
			
			valueList.emplace_back(int(midCodes.size()));
			addMidCode(name, tmp, PARA, fun_para[name][i++]);
		}
		if (i < fun_para[name].size()) {  return funPara; }
		// 调整中间代码参数计算顺序
		for (i = int(valueList.size()) - 1; i > 0; i--) {
			for (j = valueList[i - 1] + 1; j < valueList[i]; j++)
				midTmp.emplace_back(midCodes[j]);
		}
		// 函数不可内联输出push，可内联建立paraList，输出tmp赋值
		for (i = int(valueList.size()) - 1, j = (int(fun_para[name].size())) - 1; i > 0 && j >= 0; i--, j--) {
			if (symList["$all"][name].value == "f")
				midTmp.emplace_back(midCodes[valueList[i]]);
			else {
				tmp = getTmp();
				midcode dead = { tmp, midCodes[valueList[i]].left, ASS, "" };
				midTmp.emplace_back(dead);
				funPara[fun_para[name][j]] = tmp;
			}
		}
		for (i = valueList[0] + 1, j = 0; i < midCodes.size() && j < midTmp.size(); i++, j++) {
			midCodes[i] = midTmp[j];
			// outputMidcode(midTmp[j], cout);
		}
		while (i != midCodes.size())
			midCodes.pop_back();
		// cout << endl;
	}
	//cout << "<值参数表>" << endl;
	out << "<值参数表>" << endl;
	return funPara;
}


string isFun_call(string idenfr) {
	string ret = "";
	extern map<string, int> fun_idenfr;
	extern vector <struct midcode> midCodes;
	extern map<string, vector<int> > fun_mids;
	if (symList["$all"][idenfr].value == "f") symList["$all"][level].value = "f";
	
	outputsym();
	symnum = getsym();
	
	map<string, string> funVar = isValueList(idenfr);
	if (symList["$all"][idenfr].value == "f") addMidCode("", idenfr, FUNCALL, "");
	// 函数内联
	else {
		for (auto i : fun_mids[idenfr]) {
			struct midcode m = midCodes[i];
			// 对应result，局部新分配tmp，加入tmp符号表，全局不变
			if (funVar.find(m.result) != funVar.end());
			else if (symList[idenfr].find(m.result) != symList[idenfr].end() ||
				tmpList[idenfr].find(m.result) != tmpList[idenfr].end())
				funVar[m.result] = getTmp();
			else
				funVar[m.result] = m.result;
			// 对应left，常量改成值，局部变量新分配tmp，其余不变
			if (funVar.find(m.left) != funVar.end());
			else if (symList[idenfr].find(m.left) != symList[idenfr].end()) {
				if (symList[idenfr][m.left].kind == 1) funVar[m.left] = symList[idenfr][m.left].value;
				else if (symList[idenfr][m.left].kind == 0) funVar[m.left] = getTmp();
			}
			else if (tmpList[idenfr].find(m.left) != tmpList[idenfr].end()) {
				funVar[m.left] = getTmp();
			}
			else
				funVar[m.left] = m.left;
			// 对应right，常量改成值，局部变量新分配tmp，其余不变
			if (funVar.find(m.right) != funVar.end());
			else if (symList[idenfr].find(m.right) != symList[idenfr].end()) {
				if (symList[idenfr][m.right].kind == 1) funVar[m.right] = symList[idenfr][m.right].value;
				else if (symList[idenfr][m.right].kind == 0) funVar[m.right] = getTmp();
			}
			else if (tmpList[idenfr].find(m.right) != tmpList[idenfr].end()) {
				funVar[m.right] = getTmp();
			}
			else
				funVar[m.right] = m.right;
			// 加入中间代码
			if (m.op == RET || m.op == PAR);
			else if (m.op == RETX) ret = funVar[m.left];
			else addMidCode(funVar[m.result], funVar[m.left], m.op, funVar[m.right]);
		}
	}
	outputsym();
	symnum = getsym();
	
	if (fun_idenfr[idenfr]) {
		//cout << "<有返回值函数调用语句>" << endl;
		out << "<有返回值函数调用语句>" << endl;
	}
	else {
		//cout << "<无返回值函数调用语句>" << endl;
		out << "<无返回值函数调用语句>" << endl;
	}
	return ret;
}

void isAssign(string idenfr) {
	string tmp1, tmp2;
	if (symnum == ASSIGN) {
		outputsym();
		symnum = getsym();
		isExpression(tmp1);
		addMidCode(idenfr, tmp1, ASS, "");
	}
	else if (symnum == LBRACK) {
		outputsym();
		symnum = getsym();
		
		if (symnum != RBRACK) {  }
		else {
			outputsym();
			symnum = getsym();
		}
		
		outputsym();
		symnum = getsym();
		isExpression(tmp2);
		addMidCode(idenfr, tmp1, PUTARR, tmp2);
	}
	
	//cout << "<赋值语句>" << endl;
	out << "<赋值语句>" << endl;
}

void isScanf() {
	// symList["$all"][level].value = "f";
	outputsym();
	symnum = getsym();
	outputsym();
	symnum = getsym();
	
	addMidCode("", word, READ, "");
	outputsym();
	symnum = getsym();
	
	while (symnum == COMMA) {
		outputsym();
		symnum = getsym();
		
		addMidCode("", word, READ, "");
		outputsym();
		symnum = getsym();
	}
	outputsym();
	symnum = getsym();
	
	//cout << "<读语句>" << endl;
	out << "<读语句>" << endl;
}

void addMidCode(string result, string left, int op, string right) {
	struct midcode midcode = { result, left, op, right };
	vector<int> p;
	if (fun_mids.find(level) == fun_mids.end())
		fun_mids[level] = p;
	fun_mids[level].emplace_back(midCodes.size());
	midCodes.push_back(midcode);
}

string getString(string s) {
	extern map<string, map<string, struct sym> > tmpList;
	stringstream ss;
	string str;
	ss << "string$" << stringnum++;
	str = ss.str();
	sym dead = { 1, 4, s };
	tmpList["$all"][str] = dead;
	return str;
}

void isPrintf() {
	string str, tmp, type;
	
	outputsym();
	symnum = getsym();
	
	outputsym();
	symnum = getsym();
	
	if (symnum == STRCON) {
		str = getString(word);
		outputsym();
		//cout << "<字符串>" << endl;
		out << "<字符串>" << endl;
		symnum = getsym();
		
		if (symnum == COMMA) {
			outputsym();
			symnum = getsym();
			if (isExpression(tmp) == 5) type = "int";
			else type = "char";
			addMidCode(type, str, WSE, tmp);
		}
		else {
			addMidCode("", str, WSTRING, "");
		}
	}
	else {
		if (isExpression(tmp) == 5) type = "int";
		else type = "char";
		addMidCode("", tmp, WEXP, type);
	}
	
	if (symnum != RPARENT) {  }
	else {
		outputsym();
		symnum = getsym();
	}
	//cout << "<写语句>" << endl;
	out << "<写语句>" << endl;
}

void isReturn() {
	int type;
	string tmp;
	if (symnum != RETURNTK) { }
	outputsym();
	symnum = getsym();
	
	if (symnum == LPARENT) {
		outputsym();
		symnum = getsym();
		type = isExpression(tmp);
		if (type == 5 && !haveReturnInt) {
			haveReturnInt = true;
		}
		else if (type == 6 && !haveReturnChar) {
			haveReturnChar = true;
		}
		outputsym();
		symnum = getsym();
		addMidCode("", tmp, RETX, "");
	}
	else {
		if (!haveReturn) {
			haveReturn = true;
		}
		if (level != "main") addMidCode("", "", RET, "");
	}
	//cout << "<返回语句>" << endl;
	out << "<返回语句>" << endl;
}

void outputMidcodes() {
	for (auto m : midCodes){
		string result = m.result;
		string left = m.left;
		int op = m.op;
		string right = m.right;
		enum midop {
			NEG, ADD, SUB, MUL, DIVI, ASS, GETARR, PUTARR, SETLABEL, GOTO, BNZ, BZ, LESS, LE, GREAT,
			GE, EQUAL, UNEQUAL, READ, WSTRING, WEXP, WSE, RET, RETX, PARA, FUNCALL, FUN, PAR, ASSRET
		};
		switch (op) {
		case NEG:
			mid << result << " = -" << left << endl;
			break;
		case ADD:
			mid << result << " = " << left << " + " << right << endl;
			break;
		case SUB:
			mid << result << " = " << left << " - " << right << endl;
			break;
		case MUL:
			mid << result << " = " << left << " * " << right << endl;
			break;
		case DIVI:
			mid << result << " = " << left << " / " << right << endl;
			break;
		case ASS:
			mid << result << " = " << left << endl;
			break;
		case GETARR:
			mid << result << " = " << left << "[" << right << "]" << endl;
			break;
		case PUTARR:
			mid << result << "[" << left << "] = " << right << endl;
			break;
		case SETLABEL:
			mid << result << ":" << endl;
			break;
		case GOTO:
			mid << "GOTO " << result << endl;
			break;
		case BNZ:
			mid << "BNZ " << result << endl;
			break;
		case BZ:
			mid << "BZ " << result << endl;
			break;
		case LESS:
			mid << left << " < " << right << endl;
			break;
		case LE:
			mid << left << " <= " << right << endl;
			break;
		case GREAT:
			mid << left << " > " << right << endl;
			break;
		case GE:
			mid << left << " >= " << right << endl;
			break;
		case EQUAL:
			mid << left << " == " << right << endl;
			break;
		case UNEQUAL:
			mid << left << " != " << right << endl;
			break;
		case READ:
			mid << "scanf " << left << endl;
			break;
		case WSTRING:
			mid << "printf \"" << tmpList["$all"][left].value << "\"" << endl;
			break;
		case WEXP:
			mid << "printf " << left << endl;
			break;
		case WSE:
			mid << "printf \"" << tmpList["$all"][left].value << "\"" << right << endl;
			break;
		case RET:
			mid << "ret" << endl;
			break;
		case RETX:
			mid << "ret " << left << endl;
			break;
		case PARA:
			mid << "push " << left << endl;
			break;
		case FUNCALL:
			mid << "call " << left << endl;
			break;
		case FUN:
			mid << left << " " << right << "()" << endl;
			break;
		case PAR:
			mid << "para " << left << " " << right << endl;
			break;
		case ASSRET:
			mid << result << " = RET" << endl;
			break;
			default:
				break;
		}
		
	}
}

