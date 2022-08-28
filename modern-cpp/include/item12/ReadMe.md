## Item 12:使⽤override声明重载函数

* 引言
```
1.为重载函数加上override
2.成员函数限定让我们可以区别对待左值对象和右值对象（即*this)
```

* 12.1 override的用法

```
派生类要重写基类的虚函数，显示告诉编译器
class Base {
public:
	virtual void mf1() const;
	virtual void mf2(int x);
	virtual void mf3() &;
	virtual void mf4() const;
};
class Derived: public Base {
public:
	virtual void mf1() const override;
	virtual void mf2(int x) override;
	virtual void mf3() & override;
	void mf4() const override; // 可以添加virtual，但不是必要
};
```

* 12.2 成员函数引⽤限符
```
class Widget {
public:
    using DataType = std::vector<double>;
    …
    DataType& data() & // 对于左值Widgets,
    { return values; } // 返回左值
    DataType data() && // 对于右值Widgets,
    { return std::move(values); } // 返回右值
    …
private:
	DataType values;
};

auto vals1 = w.data(); //调⽤左值重载版本的Widget::data，拷⻉构造vals1
auto vals2 = makeWidget().data(); //调⽤右值重载版本的Widget::data, 移动构造vals2
```
