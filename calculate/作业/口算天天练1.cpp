#include <iostream>
using namespace std;
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>
#include <cstring>
#include <unordered_map>

#define random() (rand()%100000)

unordered_map< char, unordered_map < char, char > > Priorities;
void InitPriorities();

class problem {
public:
	string combine(string str1, string str2, char k) { //算式转化为字符
		string combination;
		combination = str1 + k + str2;
		return combination;
	}
	string int_string(int number) { //数字转换为字符型
		int temp = abs(number);
		char str[200];
		itoa(temp, str, 10);
		return str;
	}
	int scan(float answer) {//该函数用于与用户交互，获取输入数据
		float user;
		cin >> user;
		answer = ((int)(((float)answer) * 100 + 0.5)) / 100.0;
		cout << answer << endl;
		if (user == answer)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	int end() { //用于判断程序是否需要结束
		cout << _whether_to_exit << " ";
		char choose;
		cin >> choose;
		if (choose == 'Y' || choose == 'y')
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	char randomOperation() { //用于随机生成运算符
		int j;
		char symbol[1];
		j = random() % 4;
		switch (j) {
		case 0:
			symbol[0] = '+';
			break;
		case 1:
			symbol[0] = '-';
			break;
		case 2:
			symbol[0] = '*';
			break;
		case 3:
			symbol[0] = '/';
			break;
		}
		return symbol[0];
	}
	void print(int right) { //将统计结果输出给用户
		cout << _correctNum << right << endl;
		cout << _thanks << endl;
	}
	string generateExpression() { //用于生成运算式
		int num1, num2, change, count;
		char symbol;
		string str_num1, str_num2, Equation, t;
		num1 = random() % 10 + 1;
		num2 = random() % 10 + 1;
		count = random() % 6 + 2;
		symbol = randomOperation();
		str_num1 = int_string(num1);
		str_num2 = int_string(num2);
		Equation = combine(str_num1, str_num2, symbol);          //随机数与随机的符号结合 
		for (count; count>2; count--) {
			symbol = randomOperation();
			str_num1 = Equation;
			change = random() % 2;
			if (change == 0) {
				str_num1 = '(' + str_num1 + ')';
			}
			num2 = random() % 10 + 1;
			str_num2 = int_string(num2);
			change = random() % 2;
			if (change == 0) {
				t = str_num1;
				str_num1 = str_num2;
				str_num2 = t;
			}
			Equation = combine(str_num1, str_num2, symbol);
		}
		//		cout << Equation << "=" << endl;
		return Equation;                                       //返回生成的算式 
	}
	float calculateResult(string str) {                        //用于计算计算生成算式的值 
		InitPriorities();                                      //声明优先级比较表 
		vector< float > Operands;                              //运算数栈 
		vector< char > Operators;                              //运算符栈 
		float OperandTemp = 0;
		char LastOperator = 0;                                 //记录上一次所遇到的符号 
		for (int i = 0; i < str.size(); i++) {                 //此循环用于去括号 
			char ch = str[i];
			if ('0' <= ch && ch <= '9') {
				OperandTemp = OperandTemp * 10 + ch - '0';
			}
			else if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '(' || ch == ')') {
				if (ch != '(' && LastOperator != ')') {         //结合本次和上次所遇见的符号来判断是否需要将当前存储的运算数压入栈 
					Operands.push_back(OperandTemp);
					OperandTemp = 0;
				}
				char Opt2 = ch;                                 
				for (; Operators.size() > 0;) {
					char Opt1 = Operators.back();               
					char CompareRet = Priorities[Opt1][Opt2];   //用当前符号与栈顶符号来对算式简化 
					if (CompareRet == '>') {                    //当前的符号的优先级小于栈顶符号时就可以将栈顶符号计算掉并将结果压入栈  
						float Operand2 = Operands.back();
						Operands.pop_back();
						float Operand1 = Operands.back();
						Operands.pop_back();
						Operators.pop_back();
						float Ret = caculate(Operand1, Operand2, Opt1);
						Operands.push_back(Ret);
					}
					else if (CompareRet == '<') {                //当前的符号优先级大于栈顶符号不能进行运算所以跳出循环来存储当前符号 
						break;
					}
					else if (CompareRet == '=') {                //这个是“（”，“） ”结合的情况 所以移除“（”并退出循环 
						Operators.pop_back();
						break;
					}
				}
				if (Opt2 != ')') {
					Operators.push_back(Opt2);
				}
				LastOperator = Opt2;
			}
		}
		if (LastOperator != ')') {                               //接下来就是计算一个不含括号的算式了 
			Operands.push_back(OperandTemp);
		}
		for (; Operators.size() > 0;) {
			float Operand2 = Operands.back();
			Operands.pop_back();
			float Operand1 = Operands.back();
			Operands.pop_back();
			char Opt = Operators.back();
			Operators.pop_back();
			float Ret = caculate(Operand1, Operand2, Opt);
			Operands.push_back(Ret);
		}
		return Operands.back();                                                 //返回结果 
	}
	float caculate(float Operand1, float Operand2, char Operator) {        //计算函数 
		float result = 0;
		if (Operator == '+') {
			result = Operand1 + Operand2;
		}
		if (Operator == '-') {
			result = Operand1 - Operand2;
		}
		if (Operator == '*') {
			result = Operand1*Operand2;
		}
		if (Operator == '/') {
			result = Operand1 / Operand2;
		}
		return result;
	}
	void showTip()
	{
		cout << _tip << endl;
	}
	void SelectLanguage()                              //选择语言 
	{
		char str[1000][55] = { 0 };
		ifstream is("LanguageList.txt");
		int fnum = 0;
		char buf[49];
		for (int i = 0; !is.eof(); i++)
		{
			is.getline(buf, 49);
			if (strlen(buf) > 1)
			{
				strcpy(str[fnum], buf);
				fnum++;
			}
		}
		cout << "请选择语言：" << endl;
		for (int i = 0; i < fnum; i++)
		{
			cout << i + 1 << "\t";
			cout << str[i] << endl;
		}
		cout << "我选择：";
		is.close();

		int choose;
		cin >> choose;
		choose--;
		strcat(str[choose], ".txt");
		is.open(str[choose]);
		if (is.is_open())
		{
			is.getline(_tip, 100);
			is.getline(_whether_to_exit, 100);
			is.getline(_correctNum, 100);
			is.getline(_thanks, 100);
		}
		is.close();
		system("cls");
	}
private:
	char _tip[102];
	char _whether_to_exit[102];
	char _correctNum[102];
	char _thanks[102];
};

int main() {
	srand(time(0));
	int sum = 0;
	class problem p;
	p.SelectLanguage();
	p.showTip();
	while (true)
	{
		string question = p.generateExpression();		//用于生成题目 
		cout << question << " = ";
		float answer = p.calculateResult(question);		//用于计算结果 
		//	cout << answer << endl;
		sum += p.scan(answer);							//用于用户输入并计算做对题目的数目 
		//	cout << sum << endl;
		if (p.end() == 1)                               //用于结束应用 
		{
			break;
		}
	}
	p.print(sum);                                //用于输出结果 
	return 0;
}

void InitPriorities()
{
	Priorities['+']['-'] = '>';
	Priorities['+']['+'] = '>';
	Priorities['+']['*'] = '<';
	Priorities['+']['/'] = '<';
	Priorities['+']['('] = '<';
	Priorities['+'][')'] = '>';

	Priorities['-']['-'] = '>';
	Priorities['-']['+'] = '>';
	Priorities['-']['*'] = '<';
	Priorities['-']['/'] = '<';
	Priorities['-']['('] = '<';
	Priorities['-'][')'] = '>';

	Priorities['*']['-'] = '>';
	Priorities['*']['+'] = '>';
	Priorities['*']['*'] = '>';
	Priorities['*']['/'] = '>';
	Priorities['*']['('] = '<';
	Priorities['*'][')'] = '>';

	Priorities['/']['-'] = '>';
	Priorities['/']['+'] = '>';
	Priorities['/']['*'] = '>';
	Priorities['/']['/'] = '>';
	Priorities['/']['('] = '<';
	Priorities['/'][')'] = '>';

	Priorities['(']['+'] = '<';
	Priorities['(']['-'] = '<';
	Priorities['(']['*'] = '<';
	Priorities['(']['/'] = '<';
	Priorities['(']['('] = '<';
	Priorities['('][')'] = '=';

	Priorities[')']['+'] = '?';
	Priorities[')']['-'] = '?';
	Priorities[')']['*'] = '?';
	Priorities[')']['/'] = '?';
	Priorities[')']['('] = '?';
	Priorities[')'][')'] = '?';
}

