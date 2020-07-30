#include "dlist.h"
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

template<class T>
class Stack {
    Dlist<T> dlist;

public:
    bool empty();

    T *top();

    void push(T *op);

    void pop();
};

template<class T>
bool Stack<T>::empty() {
    return dlist.isEmpty();
}

template<class T>
T *Stack<T>::top() {
    T *op = dlist.removeFront();
    dlist.insertFront(op);
    return op;
}

template<class T>
void Stack<T>::push(T *op) {
    dlist.insertFront(op);
}

template<class T>
void Stack<T>::pop() {
    dlist.removeFront();
}

int main() {
    istringstream Istream;
    string input[1000];
    int count = 0;
    string str;
    getline(cin, str);
    Istream.str(str);

    string parenthesisMismatch = "ERROR: Parenthesis mismatch";
    string notEnoughOper = "ERROR: Not enough operands";
    string divideByZero = "ERROR: Divide by zero";
    string tooManyOper = "ERROR: Too many operands";

    while (Istream) {
        Istream >> input[count];
        count++;
    }
    if (input[count - 1].empty()) {
        count--;
    }

    Stack<char> stack;
    string output;

    try {
        for (int i = 0; i < count; i++) {
            if (input[i] == "+" || input[i] == "-" || input[i] == "*" || input[i] == "/") {
                while ((!stack.empty()) &&
                       ((*stack.top() == '*' || *stack.top() == '/') || (input[i] == "+" || input[i] == "-")) &&
                       (*stack.top() != '(')) {
                    output += *stack.top();
                    output += " ";
                    stack.pop();
                }
                stack.push(const_cast<char *>(input[i].c_str()));
            } else if (input[i] == "(") {
                stack.push(const_cast<char *>(input[i].c_str()));
            } else if (input[i] == ")") {
                if (stack.empty()) {
                    throw parenthesisMismatch;
                }
                while (*stack.top() != '(') {
                    output += stack.top();
                    output += " ";
                    stack.pop();
                    if (stack.empty()) {
                        throw parenthesisMismatch;
                    }
                }
                stack.pop();
            } else {
                output += input[i];
                output += " ";
            }
        }
        while (!stack.empty()) {
            if (*stack.top() == '(' || *stack.top() == ')') {
                throw parenthesisMismatch;
            }
            output += *stack.top();
            output += " ";
            stack.pop();
        }
        cout << output << endl;

        Istream.clear();
        Istream.str(output);
        Stack<long long int> cal;

        while (Istream) {
            string oper;
            Istream >> oper;
            if (oper.empty()) {
                break;
            }
            if (oper == "+" || oper == "-" || oper == "*" || oper == "/") {
                if (oper.empty()) {
                    continue;
                }
                if (cal.empty()) {
                    throw notEnoughOper;
                }
                long long int *back = cal.top();
                cal.pop();
                if (cal.empty()) {
                    delete back;
                    throw notEnoughOper;
                }
                long long int *front = cal.top();
                cal.pop();
                if (oper == "+") {
                    cal.push(new long long int(*front + *back));
                } else if (oper == "-") {
                    cal.push(new long long int(*front - *back));
                } else if (oper == "*") {
                    cal.push(new long long int(*front * *back));
                } else if (oper == "/") {
                    if (*back == 0) {
                        delete front;
                        delete back;
                        throw divideByZero;
                    }
                    cal.push(new long long int(*front / *back));
                }
                delete front;
                delete back;
            } else {
                cal.push(new long long int(stoi(oper)));
            }
        }
        long long int *res = cal.top();
        cal.pop();
        if (!cal.empty()) {
            delete res;
            throw tooManyOper;
        }
        cout << *res << endl;
        delete res;
    }
    catch (string exception) {
        cout << exception << endl;
    }

    return 0;
}