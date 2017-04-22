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
	string combine(string str1, string str2, char k) { //��ʽת��Ϊ�ַ�
		string combination;
		combination = str1 + k + str2;
		return combination;
	}
	string int_string(int number) { //����ת��Ϊ�ַ���
		int temp = abs(number);
		char str[200];
		itoa(temp, str, 10);
		return str;
	}
	int scan(float answer) {//�ú����������û���������ȡ��������
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
	int end() { //�����жϳ����Ƿ���Ҫ����
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
	char randomOperation() { //����������������
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
	void print(int right) { //��ͳ�ƽ��������û�
		cout << _correctNum << right << endl;
		cout << _thanks << endl;
	}
	string generateExpression() { //������������ʽ
		int num1, num2, change, count;
		char symbol;
		string str_num1, str_num2, Equation, t;
		num1 = random() % 10 + 1;
		num2 = random() % 10 + 1;
		count = random() % 6 + 2;
		symbol = randomOperation();
		str_num1 = int_string(num1);
		str_num2 = int_string(num2);
		Equation = combine(str_num1, str_num2, symbol);          //�����������ķ��Ž�� 
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
		return Equation;                                       //�������ɵ���ʽ 
	}
	float calculateResult(string str) {                        //���ڼ������������ʽ��ֵ 
		InitPriorities();                                      //�������ȼ��Ƚϱ� 
		vector< float > Operands;                              //������ջ 
		vector< char > Operators;                              //�����ջ 
		float OperandTemp = 0;
		char LastOperator = 0;                                 //��¼��һ���������ķ��� 
		for (int i = 0; i < str.size(); i++) {                 //��ѭ������ȥ���� 
			char ch = str[i];
			if ('0' <= ch && ch <= '9') {
				OperandTemp = OperandTemp * 10 + ch - '0';
			}
			else if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '(' || ch == ')') {
				if (ch != '(' && LastOperator != ')') {         //��ϱ��κ��ϴ��������ķ������ж��Ƿ���Ҫ����ǰ�洢��������ѹ��ջ 
					Operands.push_back(OperandTemp);
					OperandTemp = 0;
				}
				char Opt2 = ch;                                 
				for (; Operators.size() > 0;) {
					char Opt1 = Operators.back();               
					char CompareRet = Priorities[Opt1][Opt2];   //�õ�ǰ������ջ������������ʽ�� 
					if (CompareRet == '>') {                    //��ǰ�ķ��ŵ����ȼ�С��ջ������ʱ�Ϳ��Խ�ջ�����ż�����������ѹ��ջ  
						float Operand2 = Operands.back();
						Operands.pop_back();
						float Operand1 = Operands.back();
						Operands.pop_back();
						Operators.pop_back();
						float Ret = caculate(Operand1, Operand2, Opt1);
						Operands.push_back(Ret);
					}
					else if (CompareRet == '<') {                //��ǰ�ķ������ȼ�����ջ�����Ų��ܽ���������������ѭ�����洢��ǰ���� 
						break;
					}
					else if (CompareRet == '=') {                //����ǡ����������� ����ϵ���� �����Ƴ����������˳�ѭ�� 
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
		if (LastOperator != ')') {                               //���������Ǽ���һ���������ŵ���ʽ�� 
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
		return Operands.back();                                                 //���ؽ�� 
	}
	float caculate(float Operand1, float Operand2, char Operator) {        //���㺯�� 
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
	void SelectLanguage()                              //ѡ������ 
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
		cout << "��ѡ�����ԣ�" << endl;
		for (int i = 0; i < fnum; i++)
		{
			cout << i + 1 << "\t";
			cout << str[i] << endl;
		}
		cout << "��ѡ��";
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
		string question = p.generateExpression();		//����������Ŀ 
		cout << question << " = ";
		float answer = p.calculateResult(question);		//���ڼ����� 
		//	cout << answer << endl;
		sum += p.scan(answer);							//�����û����벢����������Ŀ����Ŀ 
		//	cout << sum << endl;
		if (p.end() == 1)                               //���ڽ���Ӧ�� 
		{
			break;
		}
	}
	p.print(sum);                                //���������� 
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

