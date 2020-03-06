#include <cmath>
#include <iostream>
#include <string>
using namespace std;

typedef enum
{
    Operation,
    Number,
    Variable,
    ParenthesesOpen,
    ParenthesesClose
} State;

typedef struct tStack
{
    char oper;
    struct tStack *next;
    struct tStack *previous;
} Stack;

// string fun = "( (-3)*(22.5 + (-3)) ) * x";
// string fun = "(x - 1.5) * 2"; //17
// string fun = "x - 1.5 * 2"; //7
// string fun = "(6+(x - 3)) * 2"; //26
// string fun = "(6+(2 - 3)) * x"; //50
// string fun = "(-6) - (-1 + (-3))"; //-2
// string fun = "(4) + (-0.2)*(x - (5)) + (-0.533333)*(x - (5))*(x - (10))";
string fun = "(5) + (-0.111111)*(x - (11)) + (-0.0426587)*(x - (11))*(x - (20)) + (-0.00359398)*(x - (11))*(x - (20))*(x - (4))";
float f(float x);
string posFixa(string str);
Stack startStack();
void stackUp(Stack *stack, char data);
char unstack(Stack *stack);
int priority(char c, char t);
int main()
{
    cout << "Result: " << f(0);
}

float f(float x)
{
    float result = 0;
    float op[2] = {0, 0};
    string c = posFixa(fun);
    // cout << c << endl;
    int i = 0;
    bool findOpt = false;
    for (int i = 0; i < c.length(); i++)
    {
        if (c[i] == '+' || c[i] == '-' || c[i] == '*' || c[i] == '/')
        {
            findOpt = true;
        }
    }
    if (!findOpt && (c[i] >= '0' && c[i] <= '9') || c[i] == '.' || c[i] == 'x')
    {
        if (c[i] == 'x')
        {
            op[0] = x;
        }
        else
        {
            op[0] = stof(c);
        }
    }
    while (findOpt)
    {
        if (c[i] == '+' || c[i] == '-' || c[i] == '*' || c[i] == '/') //search operation
        {
            char operation = c[i];
            int k = i - 1;
            int j;
            string auxOp = "";
            for (j = 0; j < 2; j++) //search two numbers
            {
                if (c[k] != 'x')
                {
                    bool find = false;
                    do
                    {
                        if ((c[k] >= '0' && c[k] <= '9') || c[k] == '.')
                        {
                            auxOp = c[k] + auxOp;
                            find = true;
                        }
                        k--;
                    } while (k >= 0 && c[k] != 'x' && c[k] != ' ');
                    if (find)
                    {
                        op[j] = stof(auxOp);
                        if (c[k + 1] == 'N')
                        {
                            op[j] *= -1;
                        }
                    }
                    else
                    {
                        k = -1;
                    }
                }
                else
                {
                    k--;
                    op[j] = x;
                }
                auxOp = "";
            }

            // cout << "1 - C: " << c << " i: " << i << " j: " << j << " k: " << k << endl;
            switch (operation)
            {
            case '+':
                op[0] = op[1] + op[0];
                break;
            case '-':
                op[0] = op[1] - op[0];
                break;
            case '*':
                op[0] = op[1] * op[0];
                break;
            case '/':
                op[0] = op[1] / op[0];
                break;
            default:
                op[0] = op[1] + op[0];
                break;
            }
            // _sleep(500);
            // cout << "C: " << c << " i: " << i << " j: " << j << " k: " << k << " c.length(): " << c.length() << endl;
            string rest = c.substr(i + 1, c.length() - (i + 1));
            if (i + 1 >= c.length())
            {
                break;
            }
            else
            {
                i = 0;
            }
            c = c.substr(0, k + 1);
            string opstr;
            if (op[0] >= 0)
            {
                opstr = to_string(op[0]);
                if (opstr[0] == '-')
                {
                    opstr = opstr.substr(1, opstr.length() - 1);
                }
            }
            else
            {
                opstr = "N" + to_string(-op[0]);
            }
            // cout << "opstr: " << opstr << endl;
            c = c + opstr + " " + rest;
            op[1] = 0;
        }

        i++;
    }
    result = op[0];
    return result;
}
string posFixa(string str)
{
    for (int i = 0; i < str.length() - 1; i++)
    {
        if (str[i] == '(' && (str[i + 1] == '+' || str[i + 1] == '-'))
        {
            string rest = str.substr(i + 1, str.length() - i + 1);
            str = str.substr(0, i + 1) + "0" + rest;
        }
    }

    int i = 0;
    char c, t;
    Stack stack = startStack();
    string result = "";
    stackUp(&stack, '(');
    do
    {
        c = str[i];
        if ((c >= '0' && c <= '9') || c == 'x' || c == '.')
        {
            char number[50];
            number[0] = ' ';
            if (c != 'x')
            {
                for (int j = 0; j < 50; j++) //50 is the most size to find a number
                {
                    // cout << "number[j]" << i << " - " << j << endl;
                    if ((str[i] >= '0' && str[i] <= '9') || str[i] == '.')
                    {
                        number[j] = str[i];
                        i++;
                    }
                    else
                    {
                        number[j] = ' ';
                        number[j + 1] = '\0';
                        break;
                    }
                }
            }
            if (number[0] != ' ')
            {
                result += number;
                i--;
            }
            else
                result += c;
        }
        else if (c == '(')
        {
            stackUp(&stack, '(');
        }
        else if (c == ')' || c == '\0')
        {
            do
            {
                t = unstack(&stack);
                if (t != '(')
                {
                    result += t;
                }
            } while (t != '(');
        }
        else if (c == '+' || c == '-' || c == '*' || c == '/')
        {
            while (true)
            {
                t = unstack(&stack);
                if (priority(c, t))
                {
                    stackUp(&stack, t);
                    stackUp(&stack, c);
                    break;
                }
                else
                {
                    result += t;
                }
            }
        }
        i++;
    } while (c != '\0');

    return result;
}

Stack startStack()
{
    Stack stack;
    stack.next = NULL;
    stack.previous = NULL;
    return stack;
}

void stackUp(Stack *stack, char data)
{
    Stack *newStack = new Stack[1];
    newStack->oper = data;
    newStack->next = NULL;

    Stack *lastStack = stack->previous;
    if (lastStack == NULL)
    {
        stack->next = newStack;
    }
    else
    {
        lastStack->next = newStack;
        newStack->previous = lastStack;
    }
    stack->previous = newStack;
}

char unstack(Stack *stack)
{
    char c = ' ';
    if (stack->next != NULL)
    {
        Stack *elemnt = stack->previous;
        if (elemnt->previous != NULL && stack->previous != stack->next)
        {
            Stack *elemntPrevius = elemnt->previous;
            stack->previous = elemntPrevius;
        }
        else
        {
            stack->previous = NULL;
            stack->next = NULL;
        }
        c = elemnt->oper;
        elemnt->next = NULL;
        elemnt->previous = NULL;
        free(elemnt);
    }
    return c;
}

int priority(char c, char t)
{
    int pc, pt;

    if (c == '*' || c == '/')
        pc = 2;
    else if (c == '+' || c == '-')
        pc = 1;
    else if (c == '(')
        pc = 4;

    if (t == '*' || t == '/')
        pt = 2;
    else if (t == '+' || t == '-')
        pt = 1;
    else if (t == '(')
        pt = 0;

    return (pc > pt);
}

//references
/*
    http://www.vision.ime.usp.br/~pmiranda/mac122_2s14/aulas/aula13/aula13.html
*/