#include "convertTreeToTEX.h"
#include <boost/algorithm/string.hpp>

// Добавил какие-то изменения


ExpressionTree* convertReversePolishEntryToTree(vector<string>& reversePolishEntryElements)
{
	int lastElementNumber = reversePolishEntryElements.size() - 1; // ����� ���������� �������� � �������� �������� ������


	if (lastElementNumber < 0) { // ��������� ������ ���
		throw LACK_OF_OPERANDS_EXCEPTION; // ��������� ����������
	}

	string value = reversePolishEntryElements[lastElementNumber];  // ��������� ������� �������� �������� �����
	reversePolishEntryElements.pop_back(); // ������� ������� � ����� �������
	ExpressionTree* current;

	if (isNumber(value, 20) && value[0] == '-') // ������� �������� �������� ������ �������� ������������� ������
	{
		current = new ExpressionTree("--"); // ������� ����� ������� � ���� ������ �� ��������� ���������� ������
		value.erase(0,1); // ������� ����� �� ��������
		reversePolishEntryElements.push_back(value); // ��������� �������� � ������ ��������� �������� �������� ������
	}
	else {
		current = new ExpressionTree(value); // ������� ����� ������� � ���� ������
	}

	if (isNumber(value)  && !isNumber(value, 20) ) { // ���������� �������� ���� ������ 20
		throw INCORRECT_DIAPOSON_EXCEPTION; // ��������� ����������
	}


	if (current->getExpressionElementType() == OPERATOR && reversePolishEntryElements.size() >= current->getOperandsCount()) { // ����� ������� �������� ���������� � ���������� ��������� � ������� ����������, ����� ��������� �� ���������� ��� ������ ��������

		vector<ExpressionTree*> operands(current->getOperandsCount()); // ���������� ���������� ��������� ���������
		for (int i = current->getOperandsCount() - 1; i >= 0; i--) { // ��� ���� ��������� ���������
			auto p = convertReversePolishEntryToTree(reversePolishEntryElements); // ������� ������� ���������
			operands[i] = p; 
		}

		for (int i = 0; i < operands.size(); i++) { // ��������� ������� ���������
			current->addChild(operands[i]);
		}
	}
	else if (current->getExpressionElementType() == OPERATOR && reversePolishEntryElements.size() < current->getOperandsCount()) // ���������� ��������� ������ ������������ ��� ������� ���������
	{
		throw LACK_OF_OPERANDS_EXCEPTION; // ��������� ����������
	}
	else if (current->getExpressionElementType() == UNDEFINED) // ������� ������ �������� �������������� ��������� �������� �������� ������
	{
		throw INCORRECT_VAL_FORMAT_EXCEPTION; // ��������� ����������
	}

	return current;
}


string convertSubFormulaToTex(ExpressionTree* current, int& curPriority)
{

	string subFormula; // �������, ��� ��������� �� �������

	if (current->getExpressionElementType() == OPERATOR) { // ���� ����������� ������� �������� ����������

		curPriority = current->getOperatorPriority(); // ���������� ���������
		string operands[MAX_OPERAND_COUNT] = {}; // ������ ���������
		int priority[MAX_OPERAND_COUNT] = {}; // ������  ���������� ���������

		for (int i = current->getOperandsCount() - 1; i >= 0; i--) { // ��� ���� ��������� ���������
			operands[i] = convertSubFormulaToTex(current->getChild(i), priority[i]); // ������� ������� � ��� ���������
		}


		int currentOperandsCount = current->getOperandsCount(); // ���������� ��������� � ���������
		string operatorTex = current->getTexFormat(); // ��������� �������� � tex-������

		if (currentOperandsCount == 1 && current-> getValue() != "--") { // ���������� ��������� �����  1 � �������� �� �������� ��������� �������
			subFormula = operatorTex + " {" + operands[0] + "}"; // ��������� ��������� � tex-������
		}

		if (currentOperandsCount == 1 && current->getValue() == "--") { // ���������� ��������� �����  1 � ��������  �������� ��������� �������
			subFormula = operatorTex + operands[0]; // ��������� ��������� � tex-������
		}

		if (currentOperandsCount == 2) // ���������� ��������� ����� 2
		{
			string value = current->getValue(); // �������� �������

			if (value == "sqrt()") { // �������� �������� ������

				if (operands[0] == "2") // ������ ������ �������
				{
					subFormula = operatorTex + " { " + operands[1] + " }"; // �������� ������ �������
				}
				else {
					subFormula = operatorTex + " [ " + operands[0] + " ]" + " { " + operands[1] + " }"; // ��������� ��������� � tex-������'

				}
			}
			else if (value == "frac()") { // �������� �������� ������
				subFormula = operatorTex + " { " + operands[0] + " }" + " { " + operands[1] + " }"; // ��������� ��������� � tex-������
			}
			else {

				if (curPriority < priority[0] && (operands[0][0] != '-' || value == "pow()")) operands[0] = "(" + operands[0] + ")"; // ���� ��������� ������� �������� ���� ���������� �������� ������ �������� � ����������� ��������� ����� � ������ �������� ��� ������������ ���������� � �������, ����� ����� ������� � ������
				
				bool is_number = isNumber(operands[0]) ||  isNumber(operands[1]); // ���� �� ��������� �����

				bool is_var = current->getChild(0)->getExpressionElementType() == VAR || current->getChild(1)->getExpressionElementType() == VAR; // ���� �� ��������� ����������

				bool is_greek_letter = current->getChild(0)->getExpressionElementType() == GREEKLETTER  || current->getChild(1)->getExpressionElementType() == GREEKLETTER; // ���� �� ��������� ��������� �����
				

				if (value == "*"  && (is_var || is_greek_letter)  && is_number ) { // ���������� ���������  ����� �� ����������/�����

					if (isNumber(operands[1])) { // ������ ������� �����
						swap(operands[0], operands[1]); // �������� ������� ��������
					}

					subFormula = operands[0] + operands[1]; // ��������� ����� ���������� ������ �������� � ����� ���������
				}
				else if (value == "pow()") // �������� �������� ����������� � �������
				{
					subFormula = operands[0] + " " + operatorTex + " {" + operands[1] + "}";
				}
				else 
				{
					if (curPriority < priority[1]) operands[1] = "(" + operands[1] + ")"; // ���� ��������� ������� �������� ���� ���������� �������� ������� ��������, ����� ������ ������� � ������
					if (operands[1][0] == '-') { // ������ ������� ���������� � ������, �� ����������� ��������� ������� � ������ 
						if (operands[1].find(' ') != -1) { // ������� �������� �������
							int position = operands[1].find(" "); // ����� ������� �����������
							operands[1].insert(position, ")");  // �������� ������ ����� ���������
							operands[1] = "(" + operands[1]; // �������� ������ ����� ���������
						}
						else // �����
						{
							operands[1] = "(" + operands[1] + ")"; // �������� ������� ��������
						}
					}
					subFormula = operands[0] + " " + operatorTex + " " + operands[1]; // ������� ����������
				}
			}

		}

		if (currentOperandsCount == 3) { // ���������� ��������� ����� 3
			subFormula = operatorTex + "_ {" + operands[0] + "} ^ {" + operands[1] + "}" + " {" + operands[2] + "}"; // ��������� ��������� � tex-������
		}

	}

	else if (current->getExpressionElementType() == GREEKLETTER) { // ������ �������� ��������� ������
		subFormula = current->getTexFormat(); // ��������� � TEX-������
	}
	else
	{
		subFormula = current->getValue(); // ����� �������, ��� ���������� ����� �������
	}

	return subFormula;
}


string convertFormulaToTex(const string& reversePolishEntry)
{
	string seps = " \t"; // �����������
	vector <string> reversePolishEntryElements; // ������ ��������� �������� �������� ������
	
	string workingReversePolishEntry = reversePolishEntry; // ������� ����� �������� �������� ������, � ������� � ����� ��������
	

	while (seps.find(workingReversePolishEntry[workingReversePolishEntry.size() - 1]) != -1) // ���� � ����� ������ �������� �������� ������ ������������ �����������
	{
		workingReversePolishEntry.pop_back(); // ������� �����������
	}

	
	boost::split(reversePolishEntryElements, workingReversePolishEntry,boost::is_any_of(seps), boost ::token_compress_on); // ����������� ������ ��������  �������� ������ � ������ ��������� �������� �������� ������

	ExpressionTree* tree = NULL; // ������� ������� ������


	if (reversePolishEntryElements.size() > 400) { // ���������� ��������� � �������� �������� ������ ������ �����������
		throw VERY_LONG_STRING_EXCEPTION; // ��������� ����������
	}

	try {
		tree = convertReversePolishEntryToTree(reversePolishEntryElements); // ����������� �������� �������� ������ � ������ ���������
	}
	catch (Exception ex) {
		throw ex;
	}

	if (reversePolishEntryElements.size() != 0) { // � ������� �������� ��������  �������� �������� ����������������� ��������
		throw EXCESS_OF_OPERANDS_EXCEPTION; // ������ ����������
	}


	int maxPriority = 0; // ���������
	string texFormula = "$ " + convertSubFormulaToTex(tree, maxPriority) + " $"; // ����������� ������ tex-�������. ������� � ����� ���������� ���� $.

	tree->deleteTree(); // ������� ������

	return texFormula;
}
