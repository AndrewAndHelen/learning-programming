#ifndef WIDGET_H
#define WIDGET_H

#include <memory>
#include <iostream>
class Widget //仍然在"Widget.h"中
{
public:
	Widget();
	~Widget(); 
	Widget(const Widget& rhs); //仅声明
	Widget& operator=(const Widget& rhs);
	Widget(Widget&& rhs); //仅声明
	Widget& operator=(Widget&& rhs);

private:
	struct Impl; //声明一个 实现结构体
	std::unique_ptr<Impl> pImpl; //使用智能指针而不是原始指针
};

#endif
