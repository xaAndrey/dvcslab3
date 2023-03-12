#include "constantInFunction.h"




/*
ѕолучить число встреч данной константы внутри своей функции
*/
short constantInFunction::getAppearanceCount() 
{
	return this->countInFunction;
}

/*
* ѕолучить номер строки,в которой константа встречаетс€,из массива строк по индексу
\param[in] index - »ндекс строки,где встречена константа, в массиве
*/
short constantInFunction::getAppearingString(int index) 
{
	if (index >= 0 && index <= countInFunction)
		return this->appearingStrings[index];
	else
		return -1;
}


/*
ƒобавить встречу константы на определЄнной строке
\param[in] string - номер строки,на которой встречена константа
*/
void constantInFunction::addNewAppearance(int string)	
{
	if (string > 0 && string<MAX_STRING_NUMBER)
		this->appearingStrings[countInFunction] = string;	//ƒобавить встречу константы на заданной строке,если она не ошибочна
	countInFunction += 1;	//”величить число встреч этой константы в функции на 1
}

/*
«адать им€ константы
\param[in] name - им€,задаваемое константе
*/
void constantInFunction::setName(std::string _name)
{
	this->name = _name;
}
