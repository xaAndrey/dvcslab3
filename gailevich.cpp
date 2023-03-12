#include "filemanager.h"

/*!
 * \file
 * \brief Файл, содержащий реализацию FileManager
 */

FileManager::FileManager() { }

bool FileManager::setInputFile(QString filename)
{
    ErrorController errorController;

    bool isInputCorrect = errorController.isInputFileCorrect(filename);
    if (isInputCorrect)
        inputFilename = filename;

    return isInputCorrect;
}

bool FileManager::setOutputFile(QString filename)
{
    ErrorController errorController;

    bool isOutputCorrect = errorController.isOutputFileCorrect(filename);
    if (isOutputCorrect)
        outputFilename = filename;

    return isOutputCorrect;
}

QDomNode FileManager::getXRootNode()
{
    QDomDocument doc(inputFilename);
    QFile inputFile(inputFilename);

    inputFile.open(QIODevice::ReadOnly);
    doc.setContent(&inputFile);
    inputFile.close();
    QDomElement docElem = doc.documentElement();
    QDomNode node = docElem.firstChild();

    return node;
}

int TruthTableSystem::insertNode(Node *node)
{
    int isVariable = 0; // считать заданный узел операцией
    if (dynamic_cast<Variable*>(node)) // если заданный узел - переменная
    {
        int index = 0;

        // Найти позицию первого узла не являющегося переменной
        while (index < nodes.length() && dynamic_cast<Variable*>(nodes[index]))
            index++;

        // Вставить узел на найденную позицию
        nodes.insert(index, node);
        isVariable = 1; // считать заданный узел переменной
    }
    else
    {
        // Добавить узел в конец вектора
        nodes.append(node);
    }
    return isVariable;
}

void FileManager::writeOutputFile(QStringList &subExpressions, short *values, int rowAmount, int columnAmount)
{
    QFile outputFile(outputFilename);
    outputFile.open(QIODevice::WriteOnly);
    QTextStream outputStream(&outputFile);

    // Преобразовать строки матрицы значений в строки выходного файла
    char* vals = (char*)malloc(2*rowAmount*columnAmount*sizeof(char)+1);

    int k = 0;
    for(int i = 0; i < rowAmount; i++)
    {
        for (int j = 0; j < columnAmount; j++, k+=2)
        {
            char val = *(values + i*columnAmount + j) + 48;

            *(vals + k) = val;

            if (j < columnAmount - 1)
                *(vals + k+1) = ';';
            else
                *(vals + k+1) = '\n';
        }
    }
    *(vals + k) = '\0';

    outputStream << header;
    outputStream << vals;
    outputFile.close();
    free(values);
    free(vals);
}
