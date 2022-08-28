#include "widget.h"

int main()
{
	Widget w1;
	auto w2(std::move(w1)); //移动构造w2
	w1 = std::move(w2); //移动赋值w1
	return 0;
}
