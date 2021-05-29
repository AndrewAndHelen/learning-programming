## cast

* 引言
```
1.static_cast
2.dynamic_cast
3.reinterpret_cast
4.const_cast
```

* 1.static_cast
```
用法： static_cast<type-id>(expression)
说明：
	将expression转换为type-id类型，但没有运行时类型检查保证转换安全性
用法1：
    用于类层次结构中基类和子类之间指针或引用的转换。
    进行上行转换（把子类的指针或引用转换成基类表示）是安全的；
    进行下行转换（把基类指针或引用转换成子类指针或引用）时，由于没有动态类型检查，所以是不安全的。
用法2：
	用于基本数据类型之间的转换，如把int转换成char，把int转换成enum。这种转换的安全性也要开发人员来保证。
用法3：
	把void指针转换成目标类型的指针(不安全!!)
用法4：
	把任何类型的表达式转换成void类型。
Tips:
	static_cast不能转换掉expression的const、volitale、或者__unaligned属性。
```
```
class ANIMAL {
public:
	ANIMAL() :_type("ANIMAL") {};
	virtual void OutPutname() { std::cout << "ANIMAL"; };
private:
	std::string _type;
};
class DOG :public ANIMAL {
public:
	DOG() :_name("大黄"), _type("DOG") {};
	void OutPutname() { std::cout << _name<<std::endl;};
	void OutPuttype() { std::cout << _type<< std::endl; };
private:
	std::string _name;
	std::string _type;
};

//基类与派生类指针或引用类型之间的转换，派生类转基类安全，反之不安全
//基类指针转为派生类指针,且该基类指针指向基类对象。
ANIMAL * ani1 = new ANIMAL;
DOG * dog1 = static_cast<DOG*>(ani1);
//dog1->OutPuttype();//错误，在ANIMAL类型指针不能调用方法OutPutType（）；在运行时出现错误。

//派生类指针转为基类指针，且该派生类指针指向派生类对象
DOG *dog2 = new DOG;
ANIMAL *ani2 = static_cast<DOG*>(dog2);
ani2->OutPutname(); //动态绑定， 基类指针指向派生类，调用虚函数时会调用指向类型的方法（大黄）

//基类指针转为派生类指针，且该基类指针指向派生类对象
ANIMAL * ani3 = new DOG;
DOG* dog3 = static_cast<DOG*>(ani3);
dog3->OutPutname(); //大黄
```
```
//基本数据类型之间的转换
double da = 1.999;
int ia = static_cast<int>(da);

//使用static_cast可以找回存放在void*指针中的值
double a = 1.999;
void * vptr = &a;
double * dptr = static_cast<double*>(vptr);
std::cout << *dptr << std::endl;//输出1.999
```

* 2.dynamic_cast
```
dynamic_cast主要用于类层次间的上行转换和下行转换，还可以用于类之间的交叉转换。
在类层次间进行上行转换时，dynamic_cast和static_cast的效果是一样的；
在进行下行转换时，dynamic_cast具有类型检查的功能，比static_cast更安全。

用法：dynamic_cast < type-id > ( expression )
说明：
    dynamic_cast< type* >(e)
    type必须是一个类类型且必须是一个有效的指针
    dynamic_cast< type& >(e)
    type必须是一个类类型且必须是一个左值
    dynamic_cast< type&& >(e)
    type必须是一个类类型且必须是一个右值

    e的类型必须符合以下三个条件中的任何一个：
    1、e的类型是目标类型type的公有派生类（派生类转基类-安全）
    2、e的类型是目标type的共有基类（基类转为派生类-存在类型检查）
    3、e的类型就是目标type的类型。
	
e能成功转换为type*类型的情况有三种：
	e的类型是目标type的公有派生类：派生类向基类转换一定会成功。
	e的类型是目标type的基类，当e是指针指向派生类对象，或者基类引用引用派生类对象时，类型转换才会成功，当e指向基类对象，试图转换为派生类对象时，转换失败。
	e的类型就是type的类型时，一定会转换成功。
```

```
//基类指针转为派生类指针,且该基类指针指向基类对象。
ANIMAL* ani4 = new ANIMAL;
DOG* dog4 = dynamic_cast<DOG*>(ani4);
//dog4->OutPutname();//dynamic_cast语句的转换目标是指针类型并且转换失败了，会返回一个空指针

//基类指针转为派生类指针，且该基类指针指向派生类对象
ANIMAL * ani5 = new DOG;
DOG* dog5 = static_cast<DOG*>(ani5);
dog5->OutPutname(); //正确
```

* 3.reinterpret_cast
```
用法：reinpreter_cast<type-id> (expression)
说明：
	type-id必须是一个指针、引用、算术类型、函数指针或者成员指针。它可以把一个指针转换成一个整数，也可以把一个整数转换成一个指针（先把一个指针转换成一个整数，在把该整数转换成原类型的指针，还可以得到原先的指针值）。
```

```
class A {
public:
	A(int a = 0, int b = 0) : m_a(a), m_b(b) {}
private:
	int m_a;
	int m_b;
};

//将 char* 转换为 float*
char str[] = "A";
float *p1 = reinterpret_cast<float*>(str);
std::cout << *p1 << std::endl;
char* p2 = reinterpret_cast<char*>(p1);
std::cout << *p2 << std::endl;

//将 int 转换为 int*
int *p = reinterpret_cast<int*>(100);
//将 A* 转换为 int*
p = reinterpret_cast<int*>(new A(25, 96));
std::cout << *p << std::endl;
```

* const_cast
```
用法：const_cast<type_id> (expression)
说明：它用来去掉表达式的 const 修饰或 volatile 修饰
```

```
（1）const_cast只能改变运算对象的底层const，也就是说：
	常量指针转化为非常量的指针，并且仍然指向原来的对象
	常量引用转化为非常量的引用，并且仍然指向原来的对象
	
const int value=12;
int new_value=const_cast<int>(value); 
//错误：const_cast只能改变运算对象的底层const，而对顶层const无能为力（编译信息：[Error] invalid use of const_cast with type 'int', which is not a pointer, reference, nor a pointer-to-data-member type）

const int* value_ptr=&value;
int *ptr=const_cast<int*>(value_ptr);//正确：将常量指针转化为非常量指针，并仍然指向原来的对象

const int& value_re=value;
int& re=const_cast<int&>(value_re);//正确：将常量引用转化为非常量引用，并仍然指向原来的对象
```

```
(2) 不能用const_cast改变表达式的类型
const char* cp;
const_cast<string>(cp); //错误：const_cast不能改变表达式类型，只能改变常量属性
```

```
使用场景：
1.一个函数的参数不是const对象，并且编程者事先已经知道在这个函数中不会对参数进行修改，但需要传递的实参却是已经定义好的const对象。为了成功调用这个函数，就需要利用到const_cast在实参传递前对其进行处理，从而使函数能够成功接收这个实参

#include<iostream>
using namespace std;
void Function(int *val){
    cout<<*val<<endl;
}
int main(){
    const int value=21;
    Function(const_cast<int*>(&value));
    return 0;
}

2.如果我们定义了一个非const的变量，却使用了一个指向const对象的指针来指向它，而在程序的某处希望改变这个变量时发现只有指针可用，此时就可以const_cast进行处理

const int value=21;
const int* const_ptr=&value;
int* ptr=const_cast<int*>(const_ptr); 
*ptr=3;

std::cout << "value:" << value << std::endl;
std::cout << "*const_ptr:" << *const_ptr << std::endl;
std::cout << "*ptr:" << *ptr << std::endl;

value:21
*const_ptr:3
*ptr:3
```