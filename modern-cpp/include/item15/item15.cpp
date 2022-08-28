#include <iostream>
#include<array>

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

void case15_1()
{
	constexpr int base = 2;
	constexpr int exp = 4;
	int size = pow2(base, exp);//编译期可知
	int base2, exp2;
	std::cin >> base2;
	std::cin >> exp2;
	auto result = pow2(base2, exp2);//运行期变量，失去const 常量属性
}

void case15_2()
{
	constexpr Point p1(9.4, 27.7); // 没问题，构造函数会在编译期“运⾏”
	constexpr Point p2(28.8, 5.3); // 也没问题
	constexpr auto mid = midpoint(p1, p2);
}
int main()
{
	case15_1();
	case15_2();
	return 0;
}