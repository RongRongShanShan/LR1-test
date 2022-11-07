#include <iostream>
#include <vector>
#include <iomanip>
using namespace std;

/*���߷����������ӡ*/
string vectorToString(vector<string> vec) {
	string res = "";
	for (string s : vec) {
		res += s;
	}
	return res;
}

/*
����Ԥ�����ķ���������LR��1�����������������봮�����﷨����
�����Ȱ�Ԥ�ȹ����õķ����������ݽṹ�����������ڷ���
�������ڹ�����������ķ�������:
��0��E->S
��1��S->BB
��2��B->aB
��3��B->b
*/

//Ԥ�ȹ����õ�LR��1��������
struct LRAnalyseTable {
	//�ķ����� (��һ���������ķ���չ)
	vector<string> grammer = { "E->S", "S->BB", "B->aB", "B->b" };
	//�ս��  (����#)
	vector<char> ter = { 'a','b','#' };
	//���ս��  (������չ�ķ��е�E)
	vector<char> nonTer = { 'S','B' };
	//����״̬
	vector<string> state = { "S1","S2","S3" ,"S4" ,"S5" ,"S6" ,"S7" ,"S8" ,"S9" };
	//action������ (��ͷ��ter,�����ζ�Ӧÿ��״̬)
	// s��ʾ�ƽ���ת�Ƶ���״̬��r��ʾ���ĸ��ķ���Լ
	vector<vector<string>> actionTable = {
		{"s3","s4",""},{"","","acc"},{"s6","s7",""},{"s3","s4",""},{"r3","r3",""},
		{"","","r1"},{"s6","s7",""},{"","","r3"},{"r2","r2",""},{"","","r2"}
	};
	//goto������ (��ͷ��nonTer,�����ζ�Ӧÿ��״̬)
	// ���ֱ�ʾת�Ƶ���״̬
	vector<vector<string>> gotoTable = {
		{"1","2"},{"",""},{"","5"},{"","8"},{"",""},
		{"",""},{"","9"} ,{"",""},{"",""},{"",""}
	};

	/*��ӡ�ķ�*/
	void printGrammer() {
		cout << "Ԥ�ȹ�����LR(1)��������ķ�:" << endl;
		for (int i = 0; i < grammer.size(); ++i) {
			cout << "  (" << i << ") " << grammer[i] << endl;
		}
		cout << endl;
	}

	/*��ȡ�ս��������*/
	int getTerIdx(char var) {
		for (int i = 0; i < ter.size(); ++i) {
			if (ter[i] == var) {
				return i;
			}
		}
		return -1;
	}

	/*��ȡ���ս��������*/
	int getNonTerIdx(char var) {
		for (int i = 0; i < nonTer.size(); ++i) {
			if (nonTer[i] == var) {
				return i;
			}
		}
		return -1;
	}

	/*�������봮�Ƿ�����ķ�*/
	void analyse(string input) {
		//����
		int step = 1;
		//״̬ջ
		vector<string> stateStack;
		stateStack.push_back("S0");
		//����ջ
		string symbolStack = "#";
		//���봮 (#��β)
		if (input[input.size() - 1] != '#') {
			input += '#';
		}
		//��ͷ
		cout << setw(4) << "����" << setw(10) << "״̬ջ" << setw(10) << "����ջ" << setw(10) << "���봮"
			<< setw(10) << "action" << setw(10) << "goto" << endl;
		while (true) {
			cout << setw(4) << step << setw(10) << vectorToString(stateStack) << setw(10) << symbolStack
				<< setw(10) << input;
			//��ǰ״̬ (��״̬"Sn"�и��"n"��ת������n)
			int s = atoi(stateStack[stateStack.size()-1].substr(1).c_str());
			//��ǰ���봮����
			char c = input.front();
			//��ȡ��ǰ����
			string action = actionTable[s][getTerIdx(c)];
			if (action == "acc") { //������ɣ������ķ�
				cout << setw(10) << "@acc" << setw(10) << " " << endl;
				return;
			}
			else if (action == "") { //������ɣ��������ķ�
				cout << setw(10) << "@error" << setw(10) << " " << endl;
				return;
			}
			else if (action[0] == 's') { //�ƽ�
				//������������ջ
				symbolStack.push_back(c);
				input.erase(input.begin());
				//��״̬��״̬ջ
				action[0] = 'S'; //״̬�ô�д��S��ʾ
				stateStack.push_back(action);
				cout << setw(10) << action << setw(10) << " " << endl;
			}
			else if (action[0] == 'r') {
				//״̬"rn"��ʾ����n���ķ���Լ
				string g = grammer[atoi(action.substr(1).c_str())];
				char left = g[0]; //�ķ���
				string right = g.substr(3); //�ķ��Ҳ�
				//����ջ��״̬ջ��ջ����ջ�������ķ��Ҳ��ַ�����ͬ
				int n = right.length();
				while (n-- > 0) {
					symbolStack.pop_back();
					stateStack.pop_back();
				}
				//���µ�ǰ״̬
				s = atoi(stateStack[stateStack.size() - 1].substr(1).c_str());
				//�ķ��������ջ
				symbolStack.push_back(left);
				//��״̬goto[Sn,left]��״̬ջ
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
	cout << "����������ַ�����";
	cin >> str;
	analyseTable.analyse(str);
}