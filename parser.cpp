#include<iostream>
#include<cmath>
#include<stack>
#include<climits>
#define Q 10
using namespace std;

float scanNum(char ch){
    int value;
    value = ch;
    return float(value-'0');//return float from character
}

int isOperator(char ch){
    if(ch == '+'|| ch == '-'|| ch == '*'|| ch == '/' || ch == '^')
        return 1;//character is an operator
    return -1;//not an operator
}

int isOperand(char ch){
    if(ch == 'x' || (ch >= '0' && ch <= '9'))
        return 1;//character is an operand
    return -1;//not an operand
}

float operation(float a, float b, char op){
    //Perform operation
    if(op == '+')
        return b+a;
    else if(op == '-')
        return b-a;
    else if(op == '*')
        return b*a;
    else if(op == '/')
        return b/a;
    else if(op == '^')
        return pow(b,a); //find b^a
    else
        return INT_MIN; //return negative infinity
}

float postfixEval(string postfix, float number){
    float a, b;
    stack<float> stk;
    string::iterator it;
    for(it=postfix.begin(); it!=postfix.end(); it++){
        //read elements and perform postfix evaluation
        if(isOperator(*it) != -1){
            a = stk.top();
            if (a == Q)
                a = number;
            stk.pop();
            b = stk.top();
            if (b == Q)
                b = number;
            stk.pop();
            stk.push(operation(a, b, *it));
        }
        else if(isOperand(*it) > 0){
            if (*it == 'x')
                stk.push(Q);
            else
                stk.push(scanNum(*it));
        }
    }
    return stk.top();
}

int operatorOrNot(char ch)
{
    if (ch == '^' || ch == '/' || ch == '*' || ch == '+' || ch == '-' || ch == '(')
        return 1;
    return 0;
}
// Priority of operators.
int Priority(char op)
{
    if (op == '^')
        return 3;
    else if (op == '/' || op == '*')
        return 2;
    else if (op == '+' || op == '-')
        return 1;
    // When char is '(', While loop should not be executed.
    return INT8_MAX;
}
string InfixToPostfix(string expression)
{
    string output;
    stack<char> Stack;
    for (int i = 0; i < expression.length(); i++)
    {
        char character = expression[i];
        // if character is operand.
        if (isalnum(character))
        {
            output.push_back(character);
        }
            // if character is an operator or '('.
        else if (operatorOrNot(character))
        {
            // Pop and append operators greater than scanned operator.
            while (!Stack.empty() && Stack.top() != '(' && Priority(Stack.top()) >= Priority(character))
            {
                output.push_back(Stack.top());
                Stack.pop();
            }
            Stack.push(character);
        }
            // if character is ')'
            //   then pop until '('.
        else if (character == ')')
        {
            while (Stack.top() != '(')
            {
                output.push_back(Stack.top());
                Stack.pop();
            }
            Stack.pop();
        }
    }
    // Pop all remaining elements.
    while (!Stack.empty())
    {
        output.push_back(Stack.top());
        Stack.pop();
    }
    return output;
}

// int main(){
//     string str;
//     cin >> str;
//     string str1 = InfixToPostfix(str);
//     float point;
//     cin >> point;
//     cout << str1 << endl;
//     cout << "The result is: " << postfixEval(str1, point);
//     return 0;
// }
