#include <iostream>
#include <cstring>

//Note:: bigthree函数,拷贝构造函数，拷贝赋值运算符,析构函数
/*
"Big Three" 是指在 C++ 类中，通常需要定义的三个特殊成员函数：
拷贝构造函数（Copy Constructor）
拷贝赋值运算符（Copy Assignment Operator）
析构函数（Destructor）

这三个函数在涉及到动态内存分配、资源管理以及对象拷贝时非常重要。当一个类拥有指针成员时，这些函数尤为关键。

拷贝构造函数：
    拷贝构造函数用于在创建一个新对象并将其初始化为已存在对象的副本时调用。对于类含有指针成员，这意味着需要深度拷贝
    （而不是浅拷贝）指针指向的资源，以确保不会共享相同的资源。这是为了防止多个对象共享相同的资源，导致释放资源时出现问题。
拷贝赋值运算符：
    拷贝赋值运算符用于将一个已存在的对象赋值给另一个对象。与拷贝构造函数类似，如果类包含指针成员，也需要确保资源的正确管
    理。这意味着在赋值时需要释放目标对象原有的资源，并深度拷贝源对象的资源。
析构函数：
    析构函数用于在对象被销毁时释放对象占用的资源。对于包含指针成员的类，析构函数非常重要，因为它需要负责释放动态分配的内存
    以避免内存泄漏。在析构函数中，您应该释放对象所拥有的任何资源，包括指针指向的内存。

这三个函数一起确保了对象的正确复制、赋值和销毁。在包含指针成员的类中，正确实现它们可以帮助您避免内存泄漏和资源管理问题。这
些函数的正确实现需要小心谨慎，通常需要手动编写，以确保资源被正确管理。
*/

class MyString {
private:
    char* str;  // 指向字符数组的指针

public:
    // 构造函数
    MyString(const char* s = nullptr) {
        if (s) {
            str = new char[strlen(s) + 1];
            strcpy(str, s);
        } else {
            str = nullptr;
        }
    }

    // 拷贝构造函数
    MyString(const MyString& other) {
        if (other.str) {
            str = new char[strlen(other.str) + 1];
            strcpy(str, other.str);
        } else {
            str = nullptr;
        }
    }

    // 拷贝赋值运算符
    MyString& operator=(const MyString& other) {
        // Note:: 重新赋值
        if (this != &other) {  // 避免自赋值
            delete[] str;       // 释放原有资源
            if (other.str) {
                str = new char[strlen(other.str) + 1];
                strcpy(str, other.str);
            } else {
                str = nullptr;
            }
        }
        return *this;
    }

    // 析构函数
    ~MyString() {
        delete[] str;  // 释放动态分配的内存
    }

    // 打印字符串
    void print() {
        if (str) {
            std::cout << str << std::endl;
        } else {
            std::cout << "Empty String" << std::endl;
        }
    }
};

int main() {
    MyString str1("Hello");
    MyString str2 = str1;  // 调用拷贝构造函数
    MyString str3;
    str3 = str1;           // 调用拷贝赋值运算符

    str1.print();
    str2.print();
    str3.print();

    return 0;
}
