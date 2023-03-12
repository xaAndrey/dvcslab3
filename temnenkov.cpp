/*!
 * \file
 * \brief Файл, содержащий реализацию main
*/

#include "Header.h"
int main(int argc, char *argv[])
{
    i = 0;
    while (i != 10) {
        i++;
    }
    printf("%d", 11)


    try
    {
        int countArgs = 3;
       if (argc != 3)
       {
           error err;
           err.type = WRONG_NUMBER_ARGUMENTS;
           throw err;//Вызвать исключение, если входной или выходной файлы не указаны в аргументах командной строки
       }

       //Вызвать исключение, если входной файл имеет некорректное расширение
       int countSymbolFileType = 4;
       QString inType(argv[1]); //Расширение входного файла
       if (inType.size() >= countSymbolFileType)
           inType = inType.last(countSymbolFileType);
       if (inType != ".xml")
       {
           error err;
           err.type = INPUT_FILE_INCORRECT_TYPE;
           throw err;
       }

       //Вызвать исключение, если выходной файл имеет некорректное расширение
       QString outType(argv[2]);//Расширение выходного файла
       if (outType.size() >= countSymbolFileType)
           outType = outType.last(countSymbolFileType);
       if (outType != ".txt")
       {
           error err;
           err.type = OUTPUT_FILE_INCORRECT_TYPE;
           throw err;
       }

       vert* firstVert;
       readXML(argv[1], &firstVert); //Считать вершины из xml-файла

       //Получить вектор всех вершин
       QVector<vert*> verts;
       verts.append(firstVert);
       QVector<vert*> children;
       findAllChildren(&firstVert, children);
       verts.append(children);


       error checkErr;
       errorCheck(verts, checkErr);//Проверить данные на наличие ошибок

       if (checkErr.type != NOT_ERRORS)
           throw checkErr; //Вызвать исключение, связанное с кодом ошибки, если она обнаружена

       QVector<int> numbersMissingVerts;
       bool isGivenSetSufficient;
       isGivenSetSufficient = isGivenSetVertsCoversOverlyingVert(verts, numbersMissingVerts);//Определить, покрывает ли данный набор узлов дерева вышележащий узел и найти при необходимости недостающие вершины

       QString answer = formingAnswerString(isGivenSetSufficient, numbersMissingVerts); //Сформировать ответ

       writeTXT(argv[2], answer); //Записать ответ в выходной файл

       return 0;
    }
    catch (error err)
    {
        errorHandler(err, argv[2]); //Обработать исключение, при его возникновении
    }
}
