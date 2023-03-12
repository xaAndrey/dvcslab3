#include <iostream>
#include "09_Marenkov.h"
#include "string.h"

int main()
{
	positionOfSymbol positionCurlyBracket;
	ProgramText code;
	Operator foundOperator;


	printf("Число: %d", 8)


	for (int i = 1; i <= 10; i++)
	{
		printf("%d\n", i)
	}

	//-------Ввод данных-------------
	if (input(&positionCurlyBracket, &code))
	{
		printf_s("invalid input data");
	}
	else
	{
		//-----Вызов главной вычислительной функции---------
		successWork_findControlOperator result = findControlOperator(&code, &positionCurlyBracket, &foundOperator);

		//-------Вывод результатов--------
		output(&result, &foundOperator);
	}
}

successWork_findControlOperator findControlOperator(const ProgramText* code, const positionOfSymbol* positionOfCurlyBracket, Operator* operatorInformation)
{
	//Если переданная позиция не является скобкой...
	char gettenSymbol = code->code[positionOfCurlyBracket->stringIndex][positionOfCurlyBracket->symbolInStringIndex];
	if (gettenSymbol != '{' && gettenSymbol != '}')
		return NO_BRACKET; //Считать, что передана не скобка
	else
	{
	
		positionOfSymbol posParidBracket = findPairedCurlyBracket(code, positionOfCurlyBracket); //найти позицию скобки, парной к заданной

		//Найти первую непустую строку перед открывающейся скобкой
		int indexNotEmptyString;
		if (gettenSymbol == '{')
		{
			indexNotEmptyString = findFirstNotEmptyString(code, positionOfCurlyBracket->stringIndex, OVER_STRING);
		}
		else
		{
			indexNotEmptyString = findFirstNotEmptyString(code, posParidBracket.stringIndex, OVER_STRING);
		}

		//Найти управляющий опреатор в строке
		Operator findOperator;
		int resultContains = isContainsOperator(code->code[indexNotEmptyString], &findOperator);

		if (resultContains!=0 && !isEndsInSemicolon(code->code[indexNotEmptyString]))//оператор найден и в конце строки отсутствует точка с запятой
		{
			if (findOperator.type == ELSE) //найден else
			{
				//Найти if
				positionOfSymbol positionNextBracket;
				positionNextBracket.stringIndex = findFirstNotEmptyString(code, indexNotEmptyString, OVER_STRING);
				positionNextBracket.symbolInStringIndex = strchr(code->code[positionNextBracket.stringIndex], '}') - code->code[positionNextBracket.stringIndex];

				findControlOperator(code, &positionNextBracket, operatorInformation);
			}
			else
			{
					//Считать оператор найденным
					findOperator.startPosition.stringIndex = indexNotEmptyString;
					*(operatorInformation) = findOperator;
					return DONE;
			}
		}
		else
		{
			return NO_OPERATOR; //Считать, что скобка относиться не к оператору
		}
	}
	return DONE;
}

positionOfSymbol findPairedCurlyBracket(const ProgramText* code, const positionOfSymbol* positionOfCurlyBracket)
{
	int isBracketFound = 0; //Считать, что парная фигурная скобка не найдена
	int countPairs = 0; //Считать количеством пар фигурных скобок между переданной и искомой нулём
	int currentRow = positionOfCurlyBracket->stringIndex; //Считать текущей строкой индекс строки переданной скобки
	DirectionFind findDir; // Направление поиска
	char givenBracket = code->code[positionOfCurlyBracket->stringIndex][positionOfCurlyBracket->symbolInStringIndex]; // Переданная скобка
	positionOfSymbol findPosition; // Позиция искомой скобки

	if (givenBracket == '{') //передана открывающаяся фигурная скобка
	{
		findDir = UNDER_STRING; //Считать направлением поиска "под строкой"
	}
	else
	{
		findDir = OVER_STRING; //Считать направлением поиска "над строкой"
	}

	while (isBracketFound == 0) //Пока не найдена парная скобка
	{
		int findRowIndex = findFirstNotEmptyString(code, currentRow, findDir);//Найти первую непустую строку в направлении поиска
		int indexBracketInRow = strcspn(code->code[findRowIndex], "{}");
		if (indexBracketInRow != strlen(code->code[findRowIndex])) //строка содержит фигурную скобку
		{ 
			if (code->code[findRowIndex][indexBracketInRow] == givenBracket) //скобка совпадает с переданной
			{
				countPairs++; //Увеличить количество пар фигурных скобок на 1
			}
			else if(countPairs == 0) //количество пар фигурных скобок равно 0
			{
				findPosition = { findRowIndex, indexBracketInRow };//Считать позицию найденной скобки позицией искомой
				isBracketFound = 1; //Считать парную скобку найденной
			}
			else
			{
				countPairs--; //Уменьшить количество парных скобок на 1
			}
		}
			currentRow = findRowIndex;//Считать текущей строкой индекс найденной строки
	}

	return findPosition;
}

int isContainsOperator(const char* str, Operator* operaorInformation)
{	
	int indexMinOperator = -1; //Считать индекс самого раннего названия предполагаемого оператора  не найденным
	char operatorsName[6][7] = {"if", "switch",	"for", "while", "do", "else"}; // Имена операторов
	const char* pointers[6]; //указатели на каждый оператор
	int countSupposedOperators = 0; //Считать количество предполагаемых операторов равным нулю

	for (int i = 0; i < 6; i++)//Для каждого управляющего оператора
	{
		pointers[i] = strstr(str, operatorsName[i]); //Найти его вхождение в строку

		if (pointers[i] != NULL) //оператор найден
		{
			countSupposedOperators++; //Увеличить количество предполагаемых операторов на 1 

			if( indexMinOperator == -1 || pointers[i] < pointers[indexMinOperator])
				indexMinOperator = i;//Считать индексом самого раннего названия предполагаемого оператора равным индексу текущего оператора,если индекс не найден или оператор располагается раньше минимального на данный момент
		}
	}

	if (countSupposedOperators == 0) //в строке не найден ни один предполагаемый оператор
	{
		return 0; //Считать, что оператор в строке отсутсвует
	}
	else
	{
		//Если найденный предполагаемый оператор не является частью идентификатора
		int isNotExtraSymbolsLeft = pointers[indexMinOperator] == str || isspace(*(pointers[indexMinOperator] - 1));
		int isNotExtraSymbolsRight = *(pointers[indexMinOperator] + strlen(operatorsName[indexMinOperator])) == '(' || *(pointers[indexMinOperator] + strlen(operatorsName[indexMinOperator])) == NULL 
			||isspace(*(pointers[indexMinOperator] + strlen(operatorsName[indexMinOperator])));

		if (isNotExtraSymbolsLeft && isNotExtraSymbolsRight)
		{
			operaorInformation->type = (operatorType)indexMinOperator;//Считать типом оператора, найденный оператор
			operaorInformation->startPosition.symbolInStringIndex = pointers[indexMinOperator] - str;//Считать позицией оператора в строке позицию найденного оператора
			return 1; //Считать, что оператор в строке присутствует
		}	
		else
		{
			return 0;//Считать, что оператор в строке отсутсвует
		}
	}

}

int findFirstNotEmptyString(const ProgramText* code, int strIndex, DirectionFind direction)
{
	int indexNotEmptyString = -1; //Считать, что непустая строка не найдена
	
	int deltaI = 0; // Шаг изменения индекса строки
	int textBorder; //Граница текста
	char whiteSeps[] = " \t\n\v\f\r";
	if (direction == OVER_STRING)//искомая строка над заданной
	{
		deltaI = -1; //Считать шагом изменения индекса строки - 1
		textBorder = -1; //Считать границей текста его верхнюю границу
	}
	else	
	{
		deltaI = 1;//Считать шагом изменения индекса строки + 1
		textBorder = code->stringCount; //Считать границей текста его нижнюю границу
	}

	int i = strIndex + deltaI; //Считать начальным значением индекса текущей строки индекс следующей (предшествующей) передаванной строке
	while (indexNotEmptyString == -1 && i!=textBorder)	//Пока не найдена непустая строка и не достигнута граница текста
	{
		if (strspn(code->code[i], whiteSeps) < strlen(code->code[i])) //текущая строка содержит символы кроме белых разделителей
		{
			indexNotEmptyString = i; //Считать индекс текущей строки позицией искомой строки
		}
		else
		{
			i += deltaI; //Перейти к следующей строке
		}
	}

	return indexNotEmptyString;
}

int isEndsInSemicolon(const char* str)
{
	char strCpy[MAX_ROW_LENGTH];
	strcpy_s(strCpy, str);//Скопировать содержимое переданной строки

	//Удалить все белые разделители из скопированной строки
	char whiteSeps[] = " \t\n\v\f\r";
	deleteAllSeparatorsFromString(strCpy, whiteSeps);

	if (strCpy[strlen(strCpy) - 1] == ';') //последний символ в скопированной строке является точкой с запятой
	{
		return 1; //	Считать, что строка оканчивается точкой с запятой
	}
	else
	{
		return 0; //	Считать, что строка не оканчивается точкой с запятой
	}
}

int input(positionOfSymbol* positionCurlyBracket, ProgramText* code)
{
	int isError = 0; //Флаг наличия ошибки (0 - ошибок нет, 1 - ошибки есть)
	scanf_s("%d %d", &positionCurlyBracket->stringIndex, &positionCurlyBracket->symbolInStringIndex);
	if (positionCurlyBracket->stringIndex > 39 || positionCurlyBracket->stringIndex < 0 || positionCurlyBracket->symbolInStringIndex>79 || positionCurlyBracket->symbolInStringIndex < 0)
	{
		isError = 1;
	}
	else
	{
		scanf_s("%d", &code->stringCount);
		if (code->stringCount < 1 || code->stringCount>40)
		{
			isError = 1;
		}
		else
		{
			getchar();

			for (int i = 0; i < code->stringCount; i++)
			{
				gets_s(code->code[i]);
			}
		}
	}

	return isError;
}

void output(const successWork_findControlOperator* result, const Operator* foundOperator)
{
	if (*result == DONE)
	{
		printf_s("%d %d\n", foundOperator->startPosition.stringIndex, foundOperator->startPosition.symbolInStringIndex);
		char operatorsName[5][9] = { "if", "switch", "for", "while", "do while" }; // Имена операторов
		puts(operatorsName[foundOperator->type]);
	}
	else if (*result == NO_BRACKET)
	{
		printf_s("no bracket");
	}
	else
	{
		printf_s("no operator");
	}
}

void deleteAllSeparatorsFromString(char str[], const char* seps)
{
	int i = 0; //Считать индекс текущего элемента равным нулю
	while (i < strlen(str))//пока не пройдены все элементы строки
	{
		if (strchr(seps, str[i]))//текущий элемент является разделителем
		{
			//Сдвинуть все элементы начиная с текущего влево
			for (int j = i; j < strlen(str) - 1; j++)
			{
				str[j] = str[j + 1];
			}
			str[strlen(str) - 1] = NULL; //Считать последний символ в строке концом строки
		}
		else
		{
			i++; //Перейти к следующему элементу
		}
	}
}