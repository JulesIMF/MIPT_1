/*
Copyright (c) 2020  MIPT

Module Name:

    List.h

Abstract:

    Описание структуры двусявязного списка на основе
    массива.

Author:

    JulesIMF

Last Edit:

    01.11.2020 18:39

Edit Notes:

*/

#ifndef JULESIMF_LIST
#define JULESIMF_LIST
#define _CRT_SECURE_NO_WARNINGS
#include <cstdlib>
#include "ErrorMessage.h"
#include "Algorithm.h"


size_t const END_NODE_INDEX = 0;

enum class ListException
{
    LIST_OVERFLOW,
    LIST_INVALID_NODE,
    LIST_NULL_NODE,
    LIST_EMPTY,
    LIST_NONEXISTENT_NODE,
    LIST_OUT_OF_BOUNDARIES,
    LIST_CYCLE,
    LIST_LOST_FREE_NODES,
    LIST_LOST_BACK_CONNECTION,
    LIST_INVALID_PARAMETERS,
    LIST_DUMP_ERROR,
};

static char const* LEtoString(ListException exception);

template <typename T>
struct List;

template<typename T>
int cmp_(void const* first, void const* second);

template<typename T>
struct ListNode
{
    T           value;

    friend List<T>;

    template<typename Q>
    friend int cmp_<Q>(void const* first, void const* second);
protected:
    size_t      time;
    size_t      curr,
                next,
                prev;


};


template <typename T>
struct List
{
protected:
    size_t          capacity_;
    size_t          freeNodesTime_;
    size_t          size_;

    ListNode<T>*    data_;
    ListNode<T>     free_;
                    
    bool            deepValidation_;
    bool            isConsolidated_;
    void            deepValidator_(char const* function);
    void            checkNonexistentNode_(ListNode<T> node);
    void            checkFreeNode_(ListNode<T> node);
    void            checkInvalidNode_(ListNode<T> node);
    void            printMainDump_(FILE* file);
    void            makePicture_(FILE* file);
    void            printData_(FILE* file);
    void            printNodeInDotFormat_(FILE* file, ListNode<T> node, size_t index);

public:
    bool            empty           ();//
    size_t          size            ();//
    void            clear           ();//
    void            consolidate     ();//
    bool            isConsolidated  ();

    T               back            ();//
    T               front           ();//
    
    ListNode<T>     begin           ();//
    ListNode<T>     end             ();//
    ListNode<T>     nextNode        (ListNode<T> node);//
    ListNode<T>     prevNode        (ListNode<T> node);//
    ListNode<T>     insert          (ListNode<T> node, T value);//
    ListNode<T>     erase           (ListNode<T> node);//
    ListNode<T>     pushBack        (T value);//
    ListNode<T>     pushFront       (T value);//
    ListNode<T>     advance         (ListNode<T> node, long long difference);
    ListNode<T>     searchByValue   (T value);
    ListNode<T>     searchByIndex   (size_t index);

    ListNode<T>*    at              (ListNode<T> node); //

    void            setValidation   (bool deepValidation);
    void            isValid         ();
    void            dump            ();

    List<T>()           = delete;
    List<T>(List<T>&)   = delete;
    List<T>(List<T>&&)  = delete;

    static List<T>* listNew(size_t capacity);
    static void listDelete(List<T>* list);
    
};


template<typename T>
List<T>* List<T>::listNew(size_t capacity)
{
    List<T>* list = static_cast<List<T>*>(calloc(1, sizeof(List<T>)));
    list->size_ = 0;
    list->capacity_ = capacity;
    list->freeNodesTime_ = capacity + 1;
    list->data_ = static_cast<ListNode<T>*>(calloc(capacity, sizeof(ListNode<T>)));
    list->deepValidation_ = false;
    list->clear();
    list->isConsolidated_ = true;

    return list;
}


template<typename T>
void List<T>::listDelete(List<T>* list)
{
    if (list != nullptr)
        free(list->data_);

    free(list);
}


template<typename T>
ListNode<T>* List<T>::at(ListNode<T> node)
{
    return data_ + node.curr;
}


template<typename T>
bool List<T>::empty()
{
    deepValidator_(__FUNCTION__);

    return !size_;
}


template<typename T>
size_t List<T>::size()
{
    deepValidator_(__FUNCTION__);

    return size_;
}


template<typename T>
T List<T>::back()
{
    deepValidator_(__FUNCTION__);

    if (data_[END_NODE_INDEX].prev == END_NODE_INDEX)
        throw ListException::LIST_EMPTY;

    return data_[data_[END_NODE_INDEX].prev].value;
}


template<typename T>
T List<T>::front()
{
    deepValidator_(__FUNCTION__);

    if (data_[END_NODE_INDEX].next == END_NODE_INDEX)
        throw ListException::LIST_EMPTY;

    return data_[data_[END_NODE_INDEX].next].value;
}


template<typename T>
ListNode<T> List<T>::begin()
{
    deepValidator_(__FUNCTION__);

    return data_[data_[END_NODE_INDEX].next];
}


template<typename T>
ListNode<T> List<T>::end()
{
    deepValidator_(__FUNCTION__);

    return data_[END_NODE_INDEX];
}


template<typename T>
ListNode<T> List<T>::nextNode(ListNode<T> node)
{
    deepValidator_(__FUNCTION__);

    return data_[node.next];
}


template<typename T>
ListNode<T> List<T>::prevNode(ListNode<T> node)
{
    deepValidator_(__FUNCTION__);

    return data_[node.prev];
}


template<typename T>
ListNode<T> List<T>::insert(ListNode<T> node, T value)
{
    deepValidator_(__FUNCTION__);

    if (free_.curr == END_NODE_INDEX)
        throw ListException::LIST_OVERFLOW;

    checkFreeNode_(node);

    ListNode<T>* newNode = data_ + free_.curr;
    free_ = data_[free_.next];
    newNode->value = value;
    newNode->next = node.curr;
    newNode->prev = node.prev;
    newNode->time = data_[node.prev].time + 1;
    data_[node.prev].next = data_[node.curr].prev = newNode->curr;

    size_++;
    if (node.curr != END_NODE_INDEX)
        isConsolidated_ = false;
    return data_[node.prev];
}


template<typename T>
ListNode<T> List<T>::erase(ListNode<T> node)
{
    deepValidator_(__FUNCTION__);

    checkNonexistentNode_(node);

    data_[node.prev].next = node.next;
    data_[node.next].prev = node.prev;
    data_[node.curr].next = free_.curr;
    data_[node.curr].time = freeNodesTime_;
    if (node.next != END_NODE_INDEX)
        isConsolidated_ = false;
    free_ = data_[node.curr];

    size_--;
    return data_[node.next];
}


template<typename T>
void List<T>::clear()
{
    deepValidator_(__FUNCTION__);

    for (size_t i = 1; i != capacity_; i++)
        data_[i].value = (T)0, 
        data_[i].time  = freeNodesTime_, 
        data_[i].curr  = i, 
        data_[i].next  = i + 1,
        data_[i].prev  = 0;

    data_[capacity_ - 1].next = END_NODE_INDEX;
    free_ = *(data_ + 1);
    size_ = 0;
    isConsolidated_ = true;
    data_[END_NODE_INDEX].next = data_[END_NODE_INDEX].prev = 0;

    //Можно на всякий случай делать это, но зачем?
    //
    //data_[0] = { 0, 0, END_NODE_INDEX, END_NODE_INDEX, END_NODE_INDEX };
}



template<typename T>
int cmp_(void const* first, void const* second)
{
    return
        static_cast<ListNode<T> const*>(first)->time -
        static_cast<ListNode<T> const*>(second)->time;
}


template<typename T>
void List<T>::consolidate()
{
    deepValidator_(__FUNCTION__);

    size_t currentIndex = data_[END_NODE_INDEX].next;

    size_t time = 1;
    while (currentIndex != END_NODE_INDEX)
    {
        data_[currentIndex].time = time++;
        currentIndex = data_[currentIndex].next;
    }

    qsort(data_,
        capacity_,
        sizeof(ListNode<T>),
        cmp_<T>); //АААА ЛЯМБДЫ ТВАРИ НЕ РАБОТАЮТ С КУКУСОРТОМ

    for (int i = END_NODE_INDEX; i != size_ + 1; i++)
    {
        data_[i].curr = i,
        data_[i].prev = (i + size_) % (size_ + 1),
        data_[i].next = (i + 1)     % (size_ + 1);
    }

    for(int i = size_ + 1; i != capacity_; i++)
        data_[i].curr = i,
        data_[i].prev = 0,
        data_[i].next = (i + 1);

    free_ = data_[(size_ + 1) % capacity_];
    data_[capacity_ - 1].next = 0;
    isConsolidated_ = true;
}


template<typename T>
ListNode<T> List<T>::searchByValue(T value)
{
    if (!isConsolidated_)
    {
        auto node = begin();
        while (node.curr != END_NODE_INDEX)
        {
            if (node.value == value)
                break;
            node = data_[node.curr];
        }

        return node;
    }

    size_t index = binarySearch(data_ + 1, size_, value);
    if (index == size_)
        return data_[END_NODE_INDEX];
    else
        return data_[index + 1];
}


template<typename T>
ListNode<T> List<T>::searchByIndex(size_t index)
{
    return advance(begin(), index);
}


template<typename T>
ListNode<T> List<T>::pushBack(T value)
{
    deepValidator_(__FUNCTION__);
    return insert(end(), value);
}



template<typename T>
ListNode<T> List<T>::pushFront(T value)
{
    deepValidator_(__FUNCTION__);
    return insert(begin(), value);
}


template<typename T>
ListNode<T> List<T>::advance(ListNode<T> node, long long difference)
{
    deepValidator_(__FUNCTION__);

    size_t index = node.curr;

    if (difference > 0)
        checkNonexistentNode_(node);
    else
        checkFreeNode_(node);

    if (isConsolidated_)
    {
        long long indexReturned = node.curr + difference;
        if(indexReturned <= 0 || indexReturned > size_)
            throw ListException::LIST_OUT_OF_BOUNDARIES;

        return data_[indexReturned];
    }

    bool isPositive = true;
    if (difference < 0)
        isPositive = false,
        difference *= -1;

    for (int i = 0; i != difference; i++)
    {
        if (index == END_NODE_INDEX && isPositive ||
            data_[index].prev == END_NODE_INDEX && !isPositive
           )
            throw ListException::LIST_OUT_OF_BOUNDARIES;

        if (isPositive)
            index = data_[index].next;
        else
            index = data_[index].prev;
    }

    return data_[index];
}


template<typename T>
void List<T>::isValid()
{
    auto node = data_[0];

    if (size_ >= capacity_ ||
        freeNodesTime_ != capacity_ + 1 ||
        data_ == nullptr
        )
        throw ListException::LIST_INVALID_PARAMETERS;

    //Зацикленность обычных вершин
    for (size_t i = 0; i != size_ + 1; i++)
    {
        checkInvalidNode_(node);
        node = data_[node.next];
        if ((i == size_ && node.curr != END_NODE_INDEX) ||
            (i != size_ && node.curr == END_NODE_INDEX) ||
            node.time == freeNodesTime_
           )
            throw ListException::LIST_CYCLE;

        if ((i == size_ && node.curr != END_NODE_INDEX))
            throw ListException::LIST_LOST_BACK_CONNECTION;

        if (node.curr != data_[node.next].prev ||
            node.curr != data_[node.prev].next
           )
            throw ListException::LIST_LOST_BACK_CONNECTION;
    }
    
    //Проблемы с freeNodes
    node = free_;

    for (size_t i = 0; i != capacity_ - 1 - size_; i++)
    {
        checkInvalidNode_(node);
        if (node.time != freeNodesTime_ ||
            node.curr == END_NODE_INDEX
           )
            throw ListException::LIST_LOST_FREE_NODES;
        node = data_[node.next];
    }

    checkInvalidNode_(node);
    if (node.curr != END_NODE_INDEX)
        throw ListException::LIST_CYCLE;
}


template<typename T>
bool List<T>::isConsolidated()
{
    deepValidator_(__FUNCTION__);

    return isConsolidated_;
}


template<typename T>
void List<T>::dump()
{
    FILE* dumpFile = fopen("dumpFile.html", "w");
    if (dumpFile == nullptr)
        throw ListException::LIST_DUMP_ERROR;

    system("echo off");
    fprintf(dumpFile, "<pre style=\"font-size: 20px\">\n");
    printMainDump_(dumpFile);
    makePicture_(dumpFile);
    printData_(dumpFile);
    fclose(dumpFile);
    system("start dumpFile.html\necho off");
}


template<typename T>
void List<T>::setValidation(bool deepValidation)
{
    deepValidator_(__FUNCTION__);

    deepValidation_ = deepValidation;
}


//--------------------------------------------------------------------------------------------------------
//-------------------------------------------Внутренние функции-------------------------------------------
//--------------------------------------------------------------------------------------------------------


static char const* LEtoString(ListException exception)
{
    switch (exception)
    {
    case ListException::LIST_EMPTY:
        return "LIST_EMPTY";
    case ListException::LIST_INVALID_NODE:
        return "LIST_INVALID_NODE";
    case ListException::LIST_NONEXISTENT_NODE:
        return "LIST_NONEXISTENT_NODE";
    case ListException::LIST_NULL_NODE:
        return "LIST_NULL_NODE";
    case ListException::LIST_OUT_OF_BOUNDARIES:
        return "LIST_OUT_OF_BOUNDARIES";
    case ListException::LIST_OVERFLOW:
        return "LIST_OVERFLOW";
    case ListException::LIST_LOST_BACK_CONNECTION:
        return "LIST_LOST_BACK_CONNECTION";
    case ListException::LIST_LOST_FREE_NODES:
        return "LIST_LOST_FREE_NODES";
    case ListException::LIST_CYCLE:
        return "LIST_CYCLE";
    default:
        return "UNKNOWN";
    }
}


template<typename T>
void List<T>::deepValidator_(char const* function)
{
    if (deepValidation_)
    {
        try
        {
            isValid();
        }
        catch (ListException exception)
        {
            errorMessage("in function \"%s\", instance at 0x%016llx, an exception was thrown: "
                         "%s. For more information see documentation.\n", 
                         function, (void*)this, LEtoString(exception)
                        );
        }
    }
}


template<typename T>
void List<T>::checkNonexistentNode_(ListNode<T> node)
{
    if (data_[node.curr].time == freeNodesTime_ ||
        node.curr == END_NODE_INDEX)
        throw ListException::LIST_NONEXISTENT_NODE;
}


template<typename T>
void List<T>::checkFreeNode_(ListNode<T> node)
{
    if (data_[node.curr].time == freeNodesTime_)
        throw ListException::LIST_NONEXISTENT_NODE;
}


template<typename T>
void List<T>::checkInvalidNode_(ListNode<T> node)
{
    if (node.curr >= capacity_ ||
        node.prev >= capacity_ ||
        node.next >= capacity_ ||
        node.time >  capacity_ + 1
        )
        throw ListException::LIST_INVALID_NODE;
}


template<typename T>
void List<T>::printMainDump_(FILE* dumpFile)
{
    fprintf(dumpFile,
        "[List fields]\n"
        "    [capacity]        = %lld\n"
        "    [freeNodesTime]   = %lld\n"
        "    [size]            = %lld\n"
        "    [data]            = 0x%llx\n"
        "    [free]            = %lld\n"
        "    [deepVaildation]  = %d\n"
        "    [isConsolidated]  = %d\n",
        capacity_,
        freeNodesTime_,
        size_,
        data_,
        free_.curr,
        (int)deepValidation_,
        (int)isConsolidated_
    );
}


template<typename T>
void List<T>::printNodeInDotFormat_(FILE* picture, ListNode<T> node, size_t index)
{
    fprintf(picture, "node%lld[color=\"%s\", shape = record, label = "
        "\"{<index> %lld |<val>%lld } | {<prev>%lld |<time>%lld| <next>%lld}\"];\n",
        index, ((node.time == freeNodesTime_) ? "brown" : (index ? "blue" : "black")),
        node.curr, node.value, node.prev, node.time, node.next);
}

template<typename T>
void List<T>::makePicture_(FILE* file)
{
    bool isValidList = true;
    try
    {
        isValid();
    }
    catch (ListException)
    {
        isValidList = false;
    }

    FILE* picture = fopen("graph.dot", "w");
    if (picture == nullptr)
        throw ListException::LIST_DUMP_ERROR;

    fprintf(picture, "digraph G{\ndpi = 400;\nrankdir = LR;\n");

    if (!isValidList)
        for (size_t i = 0; i != capacity_; i++)
            printNodeInDotFormat_(picture, data_[i], i);
    else
    {
        printNodeInDotFormat_(picture, data_[END_NODE_INDEX], END_NODE_INDEX);
        size_t index = data_[END_NODE_INDEX].next;
        while (index)
            printNodeInDotFormat_(picture, data_[index], index),
            index = data_[index].next;

        index = free_.curr;
        while (index)
            printNodeInDotFormat_(picture, data_[index], index),
            index = data_[index].next;
    }



    //Не выводим специальную ноду
    for (size_t i = 1; i != capacity_; i++)
    {
        if (data_[i].next != END_NODE_INDEX)
        {
            if (data_[i].time != freeNodesTime_)
                fprintf(picture, "node%lld:<next>->node%lld:<index> [color = \"red\"];\n", i, data_[i].next);
            else
                fprintf(picture, "node%lld->node%lld [color = \"red\"];\n", i, data_[i].next);
        }

        if (data_[i].time != freeNodesTime_ && data_[i].prev != END_NODE_INDEX)
            fprintf(picture, "node%lld:<prev>->node%lld:<val> [color = \"green\"];\n", i, data_[i].prev);
    }
    fprintf(picture, "}");
    fclose(picture);
    system("dot -Tpng graph.dot -o graph.png");
    //system("dotr -Tpng graph.dot -o graph.png");
    remove("graph.dot");

    fprintf(file, "<img src=\"graph.png\", width=\"95%%\">\n");
}


template<typename T>
void List<T>::printData_(FILE* file)
{
    fprintf(file, "\n\n[data listing]\n\n    [address         ]        = {value    time     curr     next     prev    }\n\n");
    for (size_t i = 0; i != capacity_; i++)
    {
        fprintf(file, "    [%016llx]        = {%-8lld %-8lld %-8lld %-8lld %-8lld}\n",
            i, data_[i].value, data_[i].time, data_[i].curr,
            data_[i].next, data_[i].prev
        );
    }
}


#endif //!JULESIMF_LIST
