#include <utility>
#include <string>
#include <iostream>
#include <type_traits>
using namespace std;

template<typename T>
typename std::enable_if<(sizeof(T) > 4), bool>::type
foo(T a)
{
	std::cout << "memory higher than 4 bytes!" << typeid(a).name() << std::endl;
	return a > 0;
}

template<typename T>
typename std::enable_if<(sizeof(T) < 4), bool>::type
foo(T a)
{
	std::cout << "memory lower than 4 bytes!" << typeid(a).name() << std::endl;
	return a > 0;
}


template <typename T>
typename std::enable_if<std::is_integral<T>::value, bool>::type
gt_zero(T i) {
	cout << "is integral" << endl;
	return i > 0;
}

template <typename T>
typename std::enable_if<!std::is_integral<T>::value, bool>::type
gt_zero(T i) {
	cout << "is NOT integral" << endl;
	return i > 0;
}

// 1. 返回类型（布尔）仅在T为整数类型时有效：
template <class T>
typename std::enable_if<std::is_integral<T>::value, bool>::type
is_odd(T i) {
	return bool(i % 2);
}

// 2. 第二个模板参数仅在T为整数类型时有效：
template < typename T,
	typename = typename std::enable_if<std::is_integral<T>::value>::type> //一种缺省写法
	bool is_even(T i) {
	return !bool(i % 2);
}

template<typename T>
using EnableIfString =
std::enable_if_t<std::is_convertible_v<T, std::string>>;

class Person
{
private:
	std::string name;
public:
	// generic constructor for passed initial name:
	template<typename STR, typename = EnableIfString<STR>>
	explicit Person(STR&& n)
		: name(std::forward<STR>(n)) {
		std::cout << "TMPL-CONSTR for ’" << name << "’\n";
	}// copy and move constructor :
	Person(Person const& p) : name(p.name) {
		std::cout << "COPY-CONSTR Person ’" << name << "’\n";
	}
		Person(Person&& p) : name(std::move(p.name)) {
		std::cout << "MOVE-CONSTR Person ’" << name << "’\n";
	}
};
int main() {
	int i = 1;    // 如果i的类型不是整数，则代码无法编译
	std::cout << "i is odd: " << is_odd(i) << std::endl;
	std::cout << "i is even: " << is_even(i) << std::endl;

	double j = 2;
	foo(j);
	foo(j);

	std::string s = "sname";
	Person p1(s); // init with string object => calls TMPL-CONSTR
	Person p2("tmp"); // init with string literal => calls TMPL-CONSTR
	Person p3(p1); // OK => calls COPY-CONSTR
	Person p4(std::move(p1)); // OK => calls MOVE-CONST
	return 0;
}
