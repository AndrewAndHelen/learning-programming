#include "widget.h"

int main()
{
	Widget w1;
	auto w2(std::move(w1)); //�ƶ�����w2
	w1 = std::move(w2); //�ƶ���ֵw1
	return 0;
}
