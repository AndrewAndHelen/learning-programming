#include<iostream>
#include<string>

//static_cast
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

//reinpreter_cast
class A {
public:
	A(int a = 0, int b = 0) : m_a(a), m_b(b) {}
private:
	int m_a;
	int m_b;
};
int main()
{
	/*
	static_cast
	用法： static_cast<type-id>(expression)
	说明： 
	*将expression转换为type-id类型，但没有运行时类型检查保证转换安全性
	* 用法1：
	* (1)用于类层次结构中基类和子类之间指针或引用的转换。进行上行转换（把子类的指针或引用转换成基类表示）是安全的；
	*			进行下行转换（把基类指针或引用转换成子类指针或引用）时，由于没有动态类型检查，所以是不安全的。
	* 用法2：
	* 用于基本数据类型之间的转换，如把int转换成char，把int转换成enum。这种转换的安全性也要开发人员来保证。
	* 用法3：
	* 把void指针转换成目标类型的指针(不安全!!)
	* 用法4：
	* 把任何类型的表达式转换成void类型。
	* Tips:static_cast不能转换掉expression的const、volitale、或者__unaligned属性。
	*/
	double da = 1.999;
	int ia = static_cast<int>(da);

	//使用static_cast可以找回存放在void*指针中的值
	double a = 1.999;
	void * vptr = &a;
	double * dptr = static_cast<double*>(vptr);
	std::cout << *dptr << std::endl;//输出1.999

	//基类与派生类指针或引用类型之间的转换，派生类转基类安全，反之不安全
	//基类指针转为派生类指针,且该基类指针指向基类对象。
	ANIMAL * ani1 = new ANIMAL;
	DOG * dog1 = static_cast<DOG*>(ani1);
	//dog1->OutPuttype();//错误，在ANIMAL类型指针不能调用方法OutPutType（）；在运行时出现错误。

	//基类指针转为派生类指针，且该基类指针指向派生类对象
	ANIMAL * ani3 = new DOG;
	DOG* dog3 = static_cast<DOG*>(ani3);
	dog3->OutPutname(); //正确

	//子类指针转为派生类指针
	DOG *dog2 = new DOG;
	ANIMAL *ani2 = static_cast<DOG*>(dog2);
	ani2->OutPutname(); //动态绑定， 基类指针指向派生类，调用虚函数时会调用指向类型的方法

	/*
	dynamic_cast
	用法：dynamic_cast < type-id > ( expression )
	说明：
	* 该运算符把expression转换成type-id类型的对象。Type-id必须是类的指针、类的引用或者void *；
	* 如果type-id是类指针类型，那么expression也必须是一个指针，
	* 如果type-id是一个引用，那么expression也必须是一个引用。

	*dynamic_cast主要用于类层次间的上行转换和下行转换，还可以用于类之间的交叉转换。
	* 在类层次间进行上行转换时，dynamic_cast和static_cast的效果是一样的；
	*在进行下行转换时，dynamic_cast具有类型检查的功能，比static_cast更安全。

	*/

	/*
	reinpreter_cast
	用法：reinpreter_cast<type-id> (expression)
	说明：
					type-id必须是一个指针、引用、算术类型、函数指针或者成员指针。
					它可以把一个指针转换成一个整数，也可以把一个整数转换成一个指针
				（先把一个指针转换成一个整数，在把该整数转换成原类型的指针，还可以得到原先的指针值）。
	*/
	//将 char* 转换为 float*
	char str[] = "http";
	float *p1 = reinterpret_cast<float*>(str);
	std::cout << *p1 << std::endl;
	//将 int 转换为 int*
	int *p = reinterpret_cast<int*>(100);
	//将 A* 转换为 int*
	p = reinterpret_cast<int*>(new A(25, 96));
	std::cout << *p << std::endl;
	/*
	const_cast
	用法：const_cast<type_id> (expression)
	说明：它用来去掉表达式的 const 修饰或 volatile 修饰
					常量指针被转化成非常量指针，并且仍然指向原来的对象；常量引用被转换成非常量引用，并且仍然指向原来的对象；常量对象被转换成非常量对象。
	*/
	const int n = 100;
	int *ip = const_cast<int*>(&n);
	*ip = 234;
	std::cout << "n = " << n << std::endl;
	std::cout << "*ip = " << *ip << std::endl;

}