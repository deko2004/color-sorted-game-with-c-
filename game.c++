#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

// عدد الزجاجات وعدد الألوان في اللعبة
const int NUM_BOTTLES = 5;
const int MAX_COLORS = 4;

struct stack
{
    char color;
    stack *next;
};

void push(stack *&head, char color)
{
    stack *newnode = new stack{color, head};
    head = newnode;
}

char pop(stack *&head)
{
    if (head == nullptr)
    {
        cout << "Bottle is empty!" << endl;
        return '\0';
    }
    char topcolor = head->color;
    stack *tmp = head;
    head = head->next;
    delete tmp;
    return topcolor;
}
// show bottles
void showBottles(stack *head, const string &bottleName)
{
    cout << bottleName << ": ";
    if (head == nullptr)
    {
        cout << "Empty";
    }
    else
    {
        stack *temp = head;
        stack *reverseStack = nullptr;

        while (temp)
        {
            push(reverseStack, temp->color);
            temp = temp->next;
        }

        // عرض العناصر
        while (reverseStack != nullptr)
        {
            cout << pop(reverseStack) << " ";
        }
    }
    cout << endl;
}

// التحقق من إمكانية النقل
bool canMove(stack *from, stack *to)
{
    if (from == nullptr || from->color == '\0') // لا يمكن النقل من زجاجة فارغة
        return false;

    int toCount = 0;
    stack *temp = to;
    while (temp != nullptr)
    {
        toCount++;
        temp = temp->next;
    }
    if (toCount >= MAX_COLORS) // لا يمكن تجاوز الحد الأقصى
        return false;

    if (to == nullptr) // يمكن دائمًا النقل إلى زجاجة فارغة
        return true;

    return to->color == from->color; // يجب أن تتطابق الألوان
}

bool checkWin(stack *head[NUM_BOTTLES])
{
    for (int i = 0; i < NUM_BOTTLES; ++i)
    {
        if (head[i] != nullptr)
        {
            char color = head[i]->color;
            stack *temp = head[i];
            int count = 1;

            while (temp != nullptr)
            {
                if (temp->color != color)
                    return false;
                temp = temp->next;
                count++;
            }

            if (count != MAX_COLORS)
                return false;
        }
    }
    return true;
}

void randomizeBottles(stack *bottles[NUM_BOTTLES])
{
    const char colors[] = {'R', 'G', 'B', 'Y'};
    const int totalColors = (NUM_BOTTLES - 2) * MAX_COLORS;
    char allColors[totalColors];
    int index = 0;

    for (int i = 0; i < MAX_COLORS; ++i)
    {
        for (int j = 0; j < NUM_BOTTLES - 2; ++j)
        {
            allColors[index++] = colors[i];
        }
    }

    srand(time(0));
    for (int i = totalColors - 2; i > 0; --i)
    {
        int j = rand() % (i + 1);
        swap(allColors[i], allColors[j]);
    }

    index = 0;
    for (int i = 0; i < NUM_BOTTLES - 2; ++i)
    {
        for (int j = 0; j < MAX_COLORS; ++j)
        {
            push(bottles[i], allColors[index++]);
        }
    }
}
int main()
{
    stack *bottles[NUM_BOTTLES] = {nullptr};

    randomizeBottles(bottles);

    while (true)
    {
        for (int i = 0; i < NUM_BOTTLES; ++i)
        {
            showBottles(bottles[i], "Bottle " + to_string(i + 1));
        }

        if (checkWin(bottles))
        {
            cout << "Congratulations! You won the game!\n";
            break;
        }

        int from, to;
        cout << "Enter the bottle to move from (1-" << NUM_BOTTLES << "): ";
        cin >> from;
        cout << "Enter the bottle to move to (1-" << NUM_BOTTLES << "): ";
        cin >> to;

        if (from < 1 || from > NUM_BOTTLES || to < 1 || to > NUM_BOTTLES)
        {
            cout << "Invalid input. Try again.\n";
            continue;
        }

        --from;
        --to;

        if (canMove(bottles[from], bottles[to]))
        {
            char movedColor = pop(bottles[from]);
            if (movedColor != '\0')
            {
                push(bottles[to], movedColor);
            }
        }
        else
        {
            cout << "Move not possible. Try again.\n";
        }
    }

    return 0;
}
