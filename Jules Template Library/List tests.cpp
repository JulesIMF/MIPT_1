#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include "List.h"
#include "ErrorMessage.h"
#include <cassert>

#define J_ASSERT(expression)                                  \
if(!(expression))                                             \
errorMessage("assertion failed on \"%s\"\n\t", #expression),  \
isFailed = true;

#define J_EXCEPTION(act)                                      \
try                                                           \
{                                                             \
    nFailed++; act;                                           \
}                                                             \
catch (ListException exception)                               \
{                                                             \
        printf("Exception %s caught, OK\n\n",                 \
            LEtoString(exception));                           \
        nFailed--;                                            \
}           

template<typename T>
bool checkList(
               List<T>*             list,
               char const*          content,
               size_t               size,
               bool                 isConsolidated
              )
{
    bool isFailed = false;
    static int testN = 0;
    printf("Test #%lld\n\t", testN++);
    if (size)
    {
        int valueAt0 = 0;
        assert(sscanf(content, "%d", &valueAt0));
        J_ASSERT(list->front() == valueAt0);
    }
    J_ASSERT(list->isConsolidated() == isConsolidated);
    J_ASSERT(list->size() == size);

    char const* string = content;
    int value = 0, procN = 0;
    for (int i = 0; i != size; i++)
    {
        assert(sscanf(string, "%d%n", &value, &procN));
        string += procN;
        if (value != list->searchByIndex(i).value)
            errorMessage("content[%d] != searchByIndex(%d).value\n\t", i, i),
            isFailed = true;
    }

    auto node = list->begin();
    string = content;
    for (int i = 0; i != size; i++, node = list->nextNode(node))
    {
        assert(sscanf(string, "%d%n", &value, &procN));
        string += procN;
        if (value != node.value)
            errorMessage("content[%d] != node.value on step#%d\n\t", i, i),
            isFailed = true;
    }
    if (!isFailed)
        printf("OK");

    printf("\n");
    return isFailed;
}


int main()
{
    int const capacity = 20;
    List<long long>* list = List<long long>::listNew(capacity);

    int nFailed = 0;

    //#e0
    J_EXCEPTION(list->erase(list->begin()));

    //#0
    checkList(list, "", 0, true);

    //#e1
    J_EXCEPTION(list->searchByIndex(3));

    //noexcept
    try
    {
        //#1
        list->pushBack(5);
        nFailed += checkList(list, "5", 1, true);

        //#2
        list->pushBack(55);
        nFailed += checkList(list, "5 55", 2, true);

        //#3
        list->pushFront(60);
        nFailed += checkList(list, "60 5 55", 3, false);

        //#4
        list->insert(list->searchByIndex(2), 30);
        nFailed += checkList(list, "60 5 30 55", 4, false);

        //#5
        list->consolidate();
        nFailed += checkList(list, "60 5 30 55", 4, true);

        //#6
        list->erase(list->searchByIndex(list->size() - 1));
        nFailed += checkList(list, "60 5 30", 3, true);

        //#7
        list->erase(list->searchByIndex(list->size() - 2));
        nFailed += checkList(list, "60 30", 2, false);

        //#8
        //Cпасибо юнит тестам и дампу за починенный clear!
        list->clear();
        nFailed += checkList(list, "", 0, true);
        //list->dump();
        //system("pause");

        //Рекламная пауза
        //throw ListException::LIST_CYCLE;

        //#9
        list->insert(list->begin(), 5);
        nFailed += checkList(list, "5", 1, true);
        
        //#10
        list->insert(list->begin(), 54);
        nFailed += checkList(list, "54 5", 2, false);

        //#11
        list->insert(list->searchByIndex(1), 0);
        nFailed += checkList(list, "54 0 5", 3, false);

        //#12
        list->insert(list->searchByIndex(2), 1);
        nFailed += checkList(list, "54 0 1 5", 4, false);

        //#13
        list->insert(list->searchByIndex(1), 22);
        nFailed += checkList(list, "54 22 0 1 5", 5, false);

        //#14
        list->erase(list->searchByIndex(2));
        nFailed += checkList(list, "54 22 1 5", 4, false);

        //#15
        list->erase(list->searchByIndex(1));
        nFailed += checkList(list, "54 1 5", 3, false);

        //#16
        list->insert(list->searchByIndex(1), 22);
        nFailed += checkList(list, "54 22 1 5", 4, false);

        //#17
        list->consolidate();
        nFailed += checkList(list, "54 22 1 5", 4, true);

        //#18
        list->pushBack(200);
        nFailed += checkList(list, "54 22 1 5 200", 5, true);

        //#19
        list->pushFront(230);
        nFailed += checkList(list, "230 54 22 1 5 200", 6, false);

        //#20
        list->searchByIndex(6);
        nFailed += checkList(list, "230 54 22 1 5 200", 6, false);

        //#e2
        J_EXCEPTION(list->erase(list->searchByIndex(6)));

        //#e3
        J_EXCEPTION(list->searchByIndex(7));

        //#21
        list->clear();
        nFailed += checkList(list, "", 0, true);
    }
    catch (ListException exception)
    {
        errorMessage("unexpected exception \"%s\" was handled. Abort.", LEtoString(exception));
        return -1;
    }

    return nFailed;
}
