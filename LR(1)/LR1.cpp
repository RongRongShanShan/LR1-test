#include <iostream>
#include <vector>
#include <iomanip>
using namespace std;

/*工具方法，方便打印*/
string vectorToString(vector<string> vec) {
	string res = "";
	for (string s : vec) {
		res += s;
	}
	return res;
}

/*
根据预先由文法规则建立的LR（1）分析表对任意的输入串进行语法分析
所以先把预先构建好的分析表用数据结构储存起来用于分析
这里用于构建分析表的文法规则是:
（0）E->S
（1）S->BB
（2）B->aB
（3）B->b
*/

//预先构建好的LR（1）分析表
struct LRAnalyseTable {
	//文法规则 (第一个规则是文法拓展)
	vector<string> grammer = { "E->S", "S->BB", "B->aB", "B->b" };
	//终结符  (包含#)
	vector<char> ter = { 'a','b','#' };
	//非终结符  (不算拓展文法中的E)
	vector<char> nonTer = { 'S','B' };
	//所有状态
	vector<string> state = { "S1","S2","S3" ,"S4" ,"S5" ,"S6" ,"S7" ,"S8" ,"S9" };
	//action表内容 (表头是ter,列依次对应每个状态)
	// s表示移进后转移到的状态；r表示按哪个文法规约
	vector<vector<string>> actionTable = {
		{"s3","s4",""},{"","","acc"},{"s6","s7",""},{"s3","s4",""},{"r3","r3",""},
		{"","","r1"},{"s6","s7",""},{"","","r3"},{"r2","r2",""},{"","","r2"}
	};
	//goto表内容 (表头是nonTer,列依次对应每个状态)
	// 数字表示转移到的状态
	vector<vector<string>> gotoTable = {
		{"1","2"},{"",""},{"","5"},{"","8"},{"",""},
		{"",""},{"","9"} ,{"",""},{"",""},{"",""}
	};

	/*打印文法*/
	void printGrammer() {
		cout << "预先构建了LR(1)分析表的文法:" << endl;
		for (int i = 0; i < grammer.size(); ++i) {
			cout << "  (" << i << ") " << grammer[i] << endl;
		}
		cout << endl;
	}

	/*获取终结符的索引*/
	int getTerIdx(char var) {
		for (int i = 0; i < ter.size(); ++i) {
			if (ter[i] == var) {
				return i;
			}
		}
		return -1;
	}

	/*获取非终结符的索引*/
	int getNonTerIdx(char var) {
		for (int i = 0; i < nonTer.size(); ++i) {
			if (nonTer[i] == var) {
				return i;
			}
		}
		return -1;
	}

	/*分析输入串是否符合文法*/
	void analyse(string input) {
		//步骤
		int step = 1;
		//状态栈
		vector<string> stateStack;
		stateStack.push_back("S0");
		//符号栈
		string symbolStack = "#";
		//输入串 (#结尾)
		if (input[input.size() - 1] != '#') {
			input += '#';
		}
		//表头
		cout << setw(4) << "步骤" << setw(10) << "状态栈" << setw(10) << "符号栈" << setw(10) << "输入串"
			<< setw(10) << "action" << setw(10) << "goto" << endl;
		while (true) {
			cout << setw(4) << step << setw(10) << vectorToString(stateStack) << setw(10) << symbolStack
				<< setw(10) << input;
			//当前状态 (将状态"Sn"切割成"n"再转成数字n)
			int s = atoi(stateStack[stateStack.size()-1].substr(1).c_str());
			//当前输入串符号
			char c = input.front();
			//获取当前动作
			string action = actionTable[s][getTerIdx(c)];
			if (action == "acc") { //分析完成，符合文法
				cout << setw(10) << "@acc" << setw(10) << " " << endl;
				return;
			}
			else if (action == "") { //分析完成，不符合文法
				cout << setw(10) << "@error" << setw(10) << " " << endl;
				return;
			}
			else if (action[0] == 's') { //移进
				//输入符号入符号栈
				symbolStack.push_back(c);
				input.erase(input.begin());
				//新状态入状态栈
				action[0] = 'S'; //状态用大写的S表示
				stateStack.push_back(action);
				cout << setw(10) << action << setw(10) << " " << endl;
			}
			else if (action[0] == 'r') {
				//状态"rn"表示按第n条文法规约
				string g = grammer[atoi(action.substr(1).c_str())];
				char left = g[0]; //文法左部
				string right = g.substr(3); //文法右部
				//符号栈和状态栈出栈，出栈个数与文法右部字符数相同
				int n = right.length();
				while (n-- > 0) {
					symbolStack.pop_back();
					stateStack.pop_back();
				}
				//更新当前状态
				s = atoi(stateStack[stateStack.size() - 1].substr(1).c_str());
				//文法左部入符号栈
				symbolStack.push_back(left);
				//新状态goto[Sn,left]入状态栈
				string newState = "S";
				newState += gotoTable[s][getNonTerIdx(left)];
				stateStack.push_back(newState);
				cout << setw(10) << action << setw(10) << gotoTable[s][getNonTerIdx(left)] << endl;
			}
			++step;
		}
	}

} analyseTable;

int main() {
	analyseTable.printGrammer();
	string str;
	cout << "输入分析的字符串：";
	cin >> str;
	analyseTable.analyse(str);
}