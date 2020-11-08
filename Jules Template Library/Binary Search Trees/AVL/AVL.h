#ifndef JULESIMF_AVL
#define JULESIMF_AVL

#include <cstdlib>
#include <cstdio>
#include <algorithm> //Только для max min


template<typename T>
void mySwap(T* first, T* second)
{
    T copy = *first;
    *first = *second;
    *second = copy;
}

//Надо бы все методы реализовывать снаружи, конечно, но что теперь...
template<typename T>
class AVL
{
public:
    struct Node
    {
    protected:
        Node
            * left,
            * right,
            * parent;

        size_t height;

    public:
        T value;
        friend class AVL;
        friend void swap(Node** first, Node** second);
        Node(Node* left_, Node* right_, Node* parent_, size_t height_, T value_) :
            left(left_), right(right_), parent(parent_),
            height(height_), value(value_) {}

        void updateHeight(void)
        {
            if (left == nullptr && right == nullptr)
            {
                height = 1;
                return;
            }

            if (left == nullptr)
            {
                height = right->height + 1;
                return;
            }

            if (right == nullptr)
            {
                height = left->height + 1;
                return;
            }

            height = max(right->height, left->height) + 1;
        }

        int difference(void)
        {
            int leftHeight  = (left == nullptr)  ? 0 : left->height;
            int rightHeight = (right == nullptr) ? 0 : right->height;
            return leftHeight - rightHeight;
        }
    };
protected:
    Node* root;

    //void swap(Node** first, )

    //Метод исключительно для дебага
    void dfs(FILE* file, Node* node)
    {
        if (node == nullptr)
            return;
    #define NIL "[color = \"black\", fontcolor = \"red\", fillcolor = \"black\", shape = rectangle, style=\"rounded, filled\", label = \"nil\"]	;\n"
        if (node->left)
            fprintf(file, "\"%d, h = %d\"->\"%d, h = %d\"\n", node->value, node->height, node->left->value, node->left->height);
        else
            fprintf(file, "\"%d, h = %d\"->nil_l%d\nnil_l%d" NIL, node->value, node->height, node->value, node->value);

        if (node->right)
            fprintf(file, "\"%d, h = %d\"->\"%d, h = %d\"\n", node->value, node->height, node->right->value, node->right->height);
        else
            fprintf(file, "\"%d, h = %d\"->nil_r%d\nnil_r%d" NIL, node->value, node->height, node->value, node->value);
    #undef NIL
        dfs(file, node->left);
        dfs(file, node->right);
    }

    //Следующие методы крайне очевидны в реализации и не требуют описания
    Node* find_(T value, Node* node)
    {
        while (true)
        {

            if (node == nullptr)
                return nullptr;

            if (node->value == value)
                return node;

            if (node->value > value)
                node = node->left;
            else
                node = node->right;
        }
    }

    void insert_(T value, Node** node, Node* parent)
    {
        Node* thisNode = *node;
        if (*node == nullptr)
        {
            *node = new Node(nullptr, nullptr, parent, 1, value);
            return;
        }

        if (value == thisNode->value)
            return;

        if (value > thisNode->value)
            insert_(value, &(*node)->right, *node);
        else
            insert_(value, &(*node)->left, *node);

        thisNode->updateHeight();
        balance_(*node);
    }


    //Эти методы нужны, чтобы "подниматься, пока мы не левый/правый сын своего родителя" и
    //"спускаться максимально влево/вправо"
    Node* leftParent_(Node* node)
    {
        if (node == nullptr)
            return nullptr;
        if (node->parent == nullptr)
            return nullptr;
        if (node->parent->left == node)
            return node->parent;
        return leftParent_(node->parent);
    }

    Node* rightParent_(Node* node)
    {
        if (node == nullptr)
            return nullptr;
        if (node->parent == nullptr)
            return nullptr;
        if (node->parent->right == node)
            return node->parent;
        return rightParent_(node->parent);
    }

    Node* leftSon_(Node* node)
    {
        if (node == nullptr)
            return nullptr;
        if (node->left == nullptr)
            return node;
        return leftSon_(node->left);
    }

    Node* rightSon_(Node* node)
    {
        if (node == nullptr)
            return nullptr;
        if (node->right == nullptr)
            return node;
        return rightSon_(node->right);
    }

    //Метод нужен только для деструктора
    void _dfs_delete(Node* node)
    {
        if (node == nullptr)
            return;
        _dfs_delete(node->left);
        _dfs_delete(node->right);
        delete node;
    }



    //У меня был выбор - сделать один метод для правого и левого вращений
    //через указатели на поля, либо копипаста. Я выбрал копипасту)
    void smallRotateLeft_(Node* node)
    {
        if (node == nullptr || node->right == nullptr)
            return;

        mySwap(&node->value, &node->right->value);
        auto middle             = node->right->left;
        auto newVirtualBoss     = node->right;
        auto right              = node->right->right;
        auto left               = node->left;

        node->right = right;
        if(right)
            right->parent = node;

        node->left = newVirtualBoss;

        node->left->right = middle;
        if (middle)
            middle->parent = newVirtualBoss;

        node->left->left = left;
        if(left)
            left->parent = newVirtualBoss;

        node->left->updateHeight();
        node->updateHeight();
        //debug();
        //system("PAUSE");
    }

    void smallRotateRight_(Node* node)
    {
        if (node == nullptr || node->left == nullptr)
            return;

        mySwap(&node->value, &node->left->value);
        auto middle = node->left->right;
        auto newVirtualBoss = node->left;
        auto right = node->left->left;
        auto left = node->right;

        node->left = right;
        if (right)
            right->parent = node;

        node->right = newVirtualBoss;

        node->right->left = middle;
        if (middle)
            middle->parent = newVirtualBoss;

        node->right->right = left;
        if (left)
            left->parent = newVirtualBoss;

        node->right->updateHeight();
        node->updateHeight();
        //debug();
        //system("PAUSE");
    }

    void bigRotateLeft_(Node* node)
    {
        smallRotateRight_(node->right);
        smallRotateLeft_(node);
    }

    void bigRotateRight_(Node* node)
    {
        smallRotateLeft_(node->left);
        smallRotateRight_(node);
    }

    void balance_(Node* node)
    {
        if (node == nullptr)
            return;

        node->updateHeight();

        auto difference = node->difference();
        if (-1 <= difference && difference <= 1)
        {
            balance_(node->parent);
            return;
        }

        if (difference == -2 && node->right && node->right->difference() == 1)
        {
            bigRotateLeft_(node);
            balance_(node->parent);
            return;
        }

        if (difference ==  2 && node->left && node->left->difference() == -1)
        {
            bigRotateRight_(node);
            balance_(node->parent);
            return;
        }

        if (difference == -2)
        {
            smallRotateLeft_(node);
            balance_(node->parent);
            return;
        }

        if (difference == 2)
        {
            smallRotateRight_(node);
            balance_(node->parent);
            return;
        }
    }

public:

    AVL() : root(nullptr) {}
    void debug(void)
    {
        FILE* dot = fopen("out.dot", "w");
        fprintf(dot, "digraph G{\ndpi = 400;\n");
        dfs(dot, root);
        fprintf(dot, "}\n");
        fclose(dot);
        system("dot -Tpng out.dot -o tree.png");
        system("start tree.png");
    }

    ~AVL(void)
    {
        _dfs_delete(root);
    }

    void insert(T value)
    {
        insert_(value, &root, nullptr);
    }

    Node* find(T value)
    {
        return find_(value, root);
    }

    void erase(T value)
    {
        auto node = find(value);
        if (node == nullptr)
            return;

        //Место, куда нужно прилепить что-то вместо нашей удаленной вершины
        Node** newNodePlace = nullptr;
        if (node == root)
            newNodePlace = &root;
        else
        {
            if (node->parent->right == node)
                newNodePlace = &node->parent->right;
            else
                newNodePlace = &node->parent->left;
        }

        //Ну если нет у чела правого сына, то подвесим вместо него левого
        if (node->right == nullptr)
        {
            *newNodePlace = node->left;
            if (*newNodePlace)
                (*newNodePlace)->parent = node->parent;
            balance_(*newNodePlace);
            balance_(node->parent);
        }

        //А иначе найдем минимум в правом поддереве, зафигачим его вместо чела и прилепим к нему правые и левые деревья
        else
        {
            Node* minInSubtree = leftSon_(node->right);

            //Надо же будет запустить потом обновление дерева
            Node* nodeToStartBalance = minInSubtree->parent;
            if (minInSubtree == node->right)
                nodeToStartBalance = minInSubtree;

            //Если правый сын сам минимум в своем поддереве, то это в какой-то мере отдельный случай
            if (minInSubtree != node->right)
            {
                minInSubtree->parent->left = minInSubtree->right;
                if (minInSubtree->right != nullptr)
                    minInSubtree->right->parent = minInSubtree->parent;
            }

            //Перевешиваем все подряд
            minInSubtree->parent = (*newNodePlace)->parent;
            *newNodePlace = minInSubtree;
            (*newNodePlace)->left = node->left;
            (*newNodePlace)->right = (node->right != minInSubtree) ?
                node->right : minInSubtree->right;
            if ((*newNodePlace)->left)
                (*newNodePlace)->left->parent = *newNodePlace;
            if ((*newNodePlace)->right)
                (*newNodePlace)->right->parent = *newNodePlace;

            balance_(nodeToStartBalance);
        }

        delete node;
    }


    //ЧТобы глазами не смотреть на больших тестах, является ли дерево АВЛ, сделаем метод
    void checkAvlInvariant_(Node* node)
    {
        if (node == nullptr)
            return;
        
        if (node->difference() > 1 || node->difference() < -1)
        {
            printf("failed: val = %d", node->value);
            debug();
            abort();
        }
        checkAvlInvariant_(node->right);
        checkAvlInvariant_(node->left);
    }

    void checkAvlInvariant(void)
    {
        checkAvlInvariant_(root);
    }


    //Эти методы не то, что просят в задаче. По сути это как ++ и -- итераторов.
    //Я их написал потому что неправильно прочитал задачу. Но вдруг понадобится потом, полезная же вещь.
    Node* next(Node* node)
    {
        if (node == nullptr)
            return nullptr;
        Node* returned = leftSon_(node->right);
        if (returned == nullptr)
            return leftParent_(node);
    }

    Node* prev(Node* node)
    {
        if (node == nullptr)
            return nullptr;
        Node* returned = rightSon_(node->right);
        if (returned == nullptr)
            return rightParent_(node);
    }

    //А это то, что просят
    Node* firstGreater(T value)
    {
        auto node = root;
        Node* candidate = nullptr;
        while (node != nullptr)
        {
            if (node->value <= value)
            {
                node = node->right;
                continue;
            }
            candidate = node;
            node = node->left;
        }

        return candidate;
    }

    Node* firstLess(T value)
    {
        auto node = root;
        Node* candidate = nullptr;
        while (node != nullptr)
        {
            if (node->value >= value)
            {
                node = node->left;
                continue;
            }
            candidate = node;
            node = node->right;
        }

        return candidate;
    }

    //Эти методы нужны были для отлова ошибок с родителями в методе erase
    void dfsValidate_(Node* node)
    {
        if (node == nullptr)
            return;
        if (node->left && node->left->parent != node)
            throw "LEFT!!";
        if (node->right && node->right->parent != node)
            throw "RIGHT!!";
        dfsValidate_(node->left);
        dfsValidate_(node->right);
    }

    void dfsValidate(void)
    {
        dfsValidate_(root);
    }

};

#endif // !JULESIMF_AVL
