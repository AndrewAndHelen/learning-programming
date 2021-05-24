## default

* 引言
```
C++11允许添加“=default”说明符到函数声明的末尾，以将该函数声明为显示默认构造函数。这就使得编译器为显示默认函数生成了默认实现，它比手动编程函数更加有效。
```

* 使用方法
```
// use of defaulted functions
#include <iostream>
using namespace std;

class A {
public:
    // A user-defined
    A(int x){
        cout << "This is a parameterized constructor";
    }

    // Using the default specifier to instruct
    // the compiler to create the default implementation of the constructor.
    A() = default;
};

int main(){
    A a;          //call A()
    A x(1);       //call A(int x)
    cout<<endl;
    return 0;
} 

```
>*Tips:我们不必指定构造函数A()的主体，因为通过附加说明符’= default’，编译器将创建此函数的默认实现。*

* 限制条件
```
默认函数需要用于特殊的成员函数（默认构造函数，复制构造函数，析构函数等），或者没有默认参数。
```

```
// non-special member functions can't be defaulted(非特殊成员函数不能使用default) 
class B { 
public: 
    // Error, func is not a special member function. 
    int func() = default;  
      
    // Error, constructor B(int, int) is not a special member function. 
    B(int, int) = default;  
  
    // Error, constructor B(int=0) has a default argument. 
    B(int = 0) = default;  
}; 
int main() { 
    return 0; 
} 

```

* 空函数体与default的区别
```
1.给用户定义的构造函数，即使它什么也不做，使得类型不是聚合，也不是微不足道的。如果您希望您的类是聚合类型或普通类型（或通过传递性，POD类型），那么需要使用’=default’。
2.使用’= default’也可以与复制构造函数和析构函数一起使用。例如，空拷贝构造函数与默认拷贝构造函数（将执行其成员的复制副本）不同。对每个特殊成员函数统一使用’=default’语法使代码更容易阅读。
```
