#include <iostream>
#include <string.h>
#pragma warning(disable:4996)


unsigned long long int fact(int num);
unsigned long long int power(int base, unsigned int power);
unsigned long long int combinations(int m, int n);
unsigned long long int combinationsWithRepeats(int m, int n);
unsigned long long int arrangementWithRepeats(int m, int n);


int main() {

	int operNumber = 0;
	int firstNum = 0;
	int secondNum = 0;
	unsigned long long int res = 0;
	printf("1- Сочетания без повторений  C(m n)\n 2- Сочетания с повторениями ~C(m n)\n 3- Размещения с повторениями ~A(m n)\n");
	scanf("%d", &operNumber);
	switch (operNumber) 
	{
		case 1:
			printf("Введите числа m и n,где n>=0; m>=0; m<=n\n");
			scanf("%d%d", &firstNum, &secondNum);
			res = combinations(firstNum, secondNum);
			if (secondNum < 0 || firstNum < 0 || firstNum > secondNum)
			{
				printf("Неправильный ввод данных: ограничения для сочетаний без повторений\nC(m n) : n=>0; m>=0; m<=n ");
				return 0;
			}
			printf("Число сочетаний без повторений из n по m : C(m n) =n! / (m! (n-m)! )");
		break;
		case 2:
			printf("Введите числа m и n,где n>=0; m>=0;\n");
			scanf("%d%d", &firstNum, &secondNum);
			res = combinationsWithRepeats(firstNum, secondNum);
			printf("Число сочетаний c повторениями из n по m : ~C(m n) = C(m n+m-1)");
		break;
		case 3:
			printf("Введите числа m и n,где n>=0; m>=0;\n");
			scanf("%d%d", &firstNum, &secondNum);
			res = arrangementWithRepeats(firstNum, secondNum);
			printf("Число размещений с повторениями из m по n: ~A(m n) = n^m");
		break;
		default:
			printf("Неправильно указан тип операции!");
		break;
	}
	printf("\nРезультат вычисления: %lli\n", res);
	return 0;
}

unsigned long long int combinations(int m, int n)
{
	if (m == 0 || m == n) return 1;

	if (n <= 20)
		return (fact(n) / fact(m)) / fact(n - m);
	else
		return combinations(m - 1, n - 1) +
		combinations(m, n - 1);
}

unsigned long long int combinationsWithRepeats(int m, int n)
{
	if (m == 0 || m == n) return 1;
return combinations(m, n + m - 1);
}
