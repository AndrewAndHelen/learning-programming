## Item 14:如果函数不抛出异常请使用noexcept

* 引言
```
1.noexcept是函数接口的⼀部分，这意味着调⽤者会依赖它、
2.noexcept函数较之于⾮noexcept函数更容易优化
3.noexcept对于移动语义,swap，内存释放函数和析构函数⾮常有⽤
4.⼤多数函数是异常中⽴的(译注：可能抛也可能不抛异常）而不是noexcept
```

* 14.1 注意事项
```
1.noexcept出现在函数的所有声明语句和定义之中，尾置返回类型之前。
2.在typedef或类型别名中不能出现noexcept。
3.在成员函数中，noexcept需要在const及引用限定符之后，在final、override或虚函数的=0之前。

注意事项：如果一个函数声明了noexcept，同时又含有throw语句或者调用了可能抛出异常的其他函数，编译器可以顺利通过，一旦抛出了异常，程序会调用std::terminate()防止异常传播。
```

* 14.2 使用方法
```
1.在函数声明后加上noexcept关键字
void excpt_func() noexcept;

2.可以接受一个常量表达式作为参数
void excpt_func() c (常量表达式);

注意事项：常量表达式的结果会被转换成一个bool类型的值。该值为true，表示函数不会抛出异常，反之，则有可能抛出异常。这里， 不带常量表达式的noexcept相当于声明noexcept(true)，即不会抛出异常。

void recoup(int) noexcept(true);	//recoup不会抛出异常
void alloc(int) noexcept(false);	//alloc可能抛出异常
```

* 14.3 noexcept运算符
```
1.noexcept()返回一个bool类型的右值常量表达式
noexcept(recoup(i))	//recoup不抛出异常则结果为true,否则为false。

void f() noexcept(noexcept(g()));//f和g的异常说明一致
如果g承诺不会抛出异常，f也不会抛出异常；如果g没有异常说明符，或者g有异常说明符但是运行抛出异常，则f也可能抛出异常。
```

* 14.4 异常说明与指针、虚函数和拷贝控制
```
1.函数指针及该指针所指函数必须具有一致的异常说明
如果指针做出不抛出异常的声明，该指针只能指向不抛出异常的函数。
如果显式或者隐式说明指针可能抛出异常，该指针可以指向任何函数。

void (*pf1)(int) noexcept = recoup;//recoup和pf1都承诺不抛出异常
void (*pf2)(int) = recoup;//recoup不会抛出异常，pf2可能抛出，互不干扰

pf1 = alloc;//错误, alloc可能抛出异常，pf1不会抛出异常，冲突
pf2 = alloc;//正确

2.基类虚函数承诺不抛出异常，派生类的虚函数也必须做出相同声明；如果基类虚函数允许抛出异常，派生类虚函数既可以运行抛出异常，也可以不允许抛出异常。

class Base
{
public:
	virtual double f1(double) noexcept;//不会抛出异常
	virtual int f2() noexcept(false);	//可能抛出异常
	virtual void f3();					//可能抛出异常
};

class Derived:public Base
{
public:
	double f1(double);				//错误，Base::f1承诺不抛出异常
	int f2() noexcept(false);		//正确
	void f3() noexcept;				//正确
};

3.一个类析构函数不应该抛出异常
C++11标准中让类的析构函数默认也是noexcept(true)的。当然，如果程序员显式地为析构函数指定了noexcept，或者类的基类或成员有noexcept(false)的析构函数，析构函数就不会再保持默认值。

#include <iostream>
using namespace std;

struct A {
	~A() { throw 1; }
};

struct B {
	~B() noexcept(false) { throw 2; }
};

struct C {
	B b;
};

void funA() { A a; }
void funB() { B b; }
void funC() { C c; }

int main() {
	try {
		funB();
	}
	catch (...) {
		cout << "caught funB." << endl; // caught funB.
	}

	try {
		funC();
	}
	catch (...) {
		cout << "caught funC." << endl; // caught funC.
	}

	try {
		funA(); // terminate called after throwing an instance of 'int'
	}
	catch (...) {
		cout << "caught funA." << endl;
	}
}

在代码中，无论是析构函数声明为noexcept(false)的类B，还是包含了B类型成员的类C，其析构函数都是可以抛出异常的。只有什么都没有声明的类A，其析构函数被默认为noexcept(true)，从而阻止了异常的扩散。
```
* 14.5 noexcept函数较之于⾮noexcept函数更容易优化

```
RetType function(params) noexcept; // 极尽所能优化
RetType function(params) throw(); // 较少优化
RetType function(params); // 较少优化
```