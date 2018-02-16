#include <iostream>
#include <string>

#include "AbstractTester.hpp"
#include "SymbolTableTester.hpp"
#include "SymbolTableTimer.hpp"
#include "BinaryTreeSymbolTable.hpp"

void run(const std::string& test, AbstractTester& tester)
{
    std::cout << test << std::endl;
    tester.runAll();
    tester.report();
}

// Visual Studio Leak Detection (Begin)
#define _CRTDBG_MAP_ALLOC      
#include <stdlib.h>
#include <crtdbg.h>
// Visual Studio Leak Detection (End)

const bool RUN_TEST = true;
const bool RUN_TIMER = true;

int main()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    std::string test = "";
    SymbolTable<int, int>* table = nullptr;
    SymbolTableTimer<int, int>* timer = nullptr;

    if (RUN_TEST)
    {
        test = "Binary Search Tree Symbol Table";
        table = new BinaryTreeSymbolTable<int, int>{};
        run(test, SymbolTableTester<int, int>(table, test, std::cout));
        delete table;
    }

    if (RUN_TIMER)
    {
        table = new BinaryTreeSymbolTable<int, int>{};
        timer = new SymbolTableTimer<int, int>{ table, test, std::cout };
        timer->timeAll();
        timer->report();
        delete timer;
        delete table;
    }

    system("pause");

    return 0;
}