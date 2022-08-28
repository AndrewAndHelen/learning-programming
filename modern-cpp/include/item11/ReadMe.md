## Item 11:优先考虑使用deleted函数而非使用未定义的私有声明

* 引言
```
1.⽐起声明函数为private但不定义，使⽤delete函数更好
2.任何函数都能delete，包括⾮成员函数和模板实例
```

* 11.1 禁用拷贝构造函数
```
// copy-constructor using delete operator 
#include <iostream> 
using namespace std; 
  
class A { 
public: 
    A(int x): m(x) { } 
      
    // Delete the copy constructor 
    A(const A&) = delete;      
    // Delete the copy assignment operator 
    A& operator=(const A&) = delete;  
    int m; 
}; 
  
int main() { 
    A a1(1), a2(2), a3(3); 
    // Error, the usage of the copy assignment operator is disabled 
    a1 = a2;   
    // Error, the usage of the copy constructor is disabled 
    a3 = A(a2);  
    return 0; 
} 

```

* 11.2 禁用不需要的参数转换
```
class Widget {
public:
…
template<typename T>
void processPointer(T* ptr)
{ … }
…
};
template<>
void Widget::processPointer<void>(void*) = delete; // 还是public，但是已经被删除了
void case11_1()
{
	Widget w;
	int a = 1;
	//error C2280: “void Widget::processPointer<int>(int *)”: 尝试引用已删除的函数
	w.processPointer<int>(&a);
}
```
