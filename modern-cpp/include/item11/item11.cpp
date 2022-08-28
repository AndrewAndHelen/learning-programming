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
void Widget::processPointer<int>(int*) = delete; // ����public�������Ѿ���ɾ����

void case11_1()
{
	Widget w;
	int a = 1;
	w.processPointer<int>(&a);
}
