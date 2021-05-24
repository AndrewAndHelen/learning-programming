#include <iostream>

class Widget {
public:
	template<typename T>
	void processPointer(T* ptr)
	{
		std::cout << *ptr;
	}
};
template<>
void Widget::processPointer<int>(int*) = delete; // 还是public，但是已经被删除了

void case11_1()
{
	Widget w;
	int a = 1;
	w.processPointer<int>(&a);
}
