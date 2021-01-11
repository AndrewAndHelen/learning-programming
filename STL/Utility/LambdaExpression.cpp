#include<iostream>

int main()
{
	//捕获列表[]按值传递的变量，一旦初始化(如int x=0为初始化，捕获的是初始化x的值，
	//后面x怎么变化lambda表达式都不动)就不会改变(视为一个常量)
	int x = 0;
	int y = 0;
	auto qqq = [x, &y] {
		std::cout << "x" << x << std::endl;
		std::cout << "y" << y << std::endl;
		++y;
	};

	x = y = 77;
	qqq();
	qqq();
	std::cout << "final y:" << y << std::endl;

	//mutable表示捕获按值传递和按引用传递的混合体
	//即捕获初始化的id值作为自己的内部成员，可以在函数体内改变
	int id = 0;
	auto f = [id]() mutable {
		std::cout << "id:" << id << std::endl;
		id++;
	};

	id = 42;
	f();
	f();
	f();
	std::cout << id << std::endl;

	//decltype表示推断一个表达式的类型
	int ix = 0;
	decltype(ix) iy = 0;

	return 0;
}