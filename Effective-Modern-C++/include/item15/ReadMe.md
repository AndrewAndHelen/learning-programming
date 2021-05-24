## Item 15:尽可能的使用constexpr

* 引言
```
1.constexpr对象是cosnt，它的值在编译期可知
2.当传递编译期可知的值时，cosntexpr函数可以产出编译期可知的结果
```

* 15.1 constexpr对象编译器可知

```
int sz; // ⾮constexpr变量
…
constexpr auto arraySize1 = sz; // 错误! sz的值在编译期不可知
std::array<int, sz> data1; // 错误!⼀样的问题
constexpr auto arraySize2 = 10; // 没问题，10是编译期可知常量 
std::array<int, arraySize2> data2; // 没问题, arraySize2是constexpr

注意事项：
使⽤场景涉及函数，带有constexpr的函数的实参是编译期常量，它们将产出编译期值；如果是运⾏时值，它们就将产出运⾏时值。
```

* 15.2 当传递编译期可知的值时，cosntexpr函数可以产出编译期可知的结果
```
1.constexpr函数可以⽤于需求编译期常量的上下⽂。如果你传给constexpr函数的实参在编译期可
知，那么结果将在编译期计算。如果实参的值在编译期不知道，你的代码就会被拒绝。
2.当⼀个constexpr函数被⼀个或者多个编译期不可知值调⽤时，它就像普通函数⼀样，运⾏时计算
它的结果。这意味着你不需要两个函数，⼀个⽤于编译期计算，⼀个⽤于运⾏时计算。constexpr全做了。

//c++ 11用法，constexpr函数体只能1行
constexpr int pow1(int base, int exp) noexcept
{
	return exp == 0 ? 1 : base * pow1(base, exp);
}

//c++ 14用法，constexpr函数体可以多行
constexpr int pow2(int base, int exp) noexcept
{
	if (exp == 0)
		return 1;

	int half_result = pow2(base, exp / 2);
	return exp % 2 == 0 ? half_result* half_result : half_result* half_result*base;
}

constexpr int base = 2;
constexpr int exp = 4;
int size = pow2(base, exp);//编译期可知
int base2, exp2;
std::cin >> base2;
std::cin >> exp2;
auto result = pow2(base2, exp2);//运行期变量，失去const 常量属性
```

* 15.3 构造函数与成员函数也可以为constexpr
```
class Point {
public:
	constexpr Point(double xVal = 0, double yVal = 0) noexcept : x(xVal), y(yVal)
	{}
	constexpr double xValue() const noexcept { return x; }
	constexpr double yValue() const noexcept { return y; }
	void setX(double newX) noexcept { x = newX; }//c++11不能声明为constexpr,c++14可以
	void setY(double newY) noexcept { y = newY; }
private:
	double x, y;
};

constexpr
Point midpoint(const Point& p1, const Point& p2) noexcept
{
	return { (p1.xValue() + p2.xValue()) / 2,
	(p1.yValue() + p2.yValue()) / 2 };
}


constexpr Point p1(9.4, 27.7); // 没问题，构造函数会在编译期“运⾏”
constexpr Point p2(28.8, 5.3); // 也没问题
constexpr auto mid = midpoint(p1, p2);
```