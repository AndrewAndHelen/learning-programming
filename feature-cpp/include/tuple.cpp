#include <iostream>  
#include <vector>  
#include <string>  
#include <tuple>  

template<typename Tuple, size_t N>
struct tuple_show
{
	static void show(const Tuple &t, std::ostream& os)
	{
		tuple_show<Tuple, N - 1>::show(t, os);
		os << ", " << std::get<N - 1>(t);
	}
};


// 偏特性，可以理解为递归的终止
template<typename Tuple>
struct tuple_show < Tuple, 1>
{
	static void show(const Tuple &t, std::ostream &os)
	{
		os << std::get<0>(t);
	}
};



// 自己写个函数，调用上面的递归展开，
template<typename... Args>
std::ostream& operator << (std::ostream &os, const std::tuple<Args...>& t)
{
	os << "[";
	tuple_show<decltype(t), sizeof...(Args)>::show(t, os);
	os << "]";

	return os;
}

std::tuple<std::string, int> giveName(void)
{
	std::string cw("Caroline");
	int a(2013);
	std::tuple<std::string, int> t = std::make_tuple(cw, a);
	return t;
}

int main()
{
	/* tuple初始化*/
	std::tuple<int, double, std::string> t(64, 128.0, "Caroline");
	std::tuple<std::string, std::string, int> t2 =
		std::make_tuple("Caroline", "Wendy", 1992);

	/*返回元素个数 */ 
	size_t num = std::tuple_size<decltype(t)>::value;
	std::cout << "num = " << num << std::endl;

	/*获取第1个值的元素类型*/  
	std::tuple_element<1, decltype(t)>::type cnt;
	cnt = std::get<1>(t);
	std::cout << "cnt = " << cnt << std::endl;

	/*比较大小*/  
	std::tuple<int, int> ti(24, 48);
	std::tuple<double, double> td(28.0, 56.0);
	bool b = (ti < td);
	std::cout << "b = " << b << std::endl;

	/*tuple作为返回值*/  
	auto a = giveName();
	std::cout << "name: " << std::get<0>(a)
		<< " years: " << std::get<1>(a) << std::endl;

	/*std::tie解包*/
	std::tuple<char, int, long, std::string> fourth('A', 2, 3, "4");

	// 定义变量，保存解包结果
	char tuple_0 = '0';
	int tuple_1 = 0;
	long tuple_2 = 0;
	std::string tuple_3("");

	// 使用std::tie, 依次传入对应的解包变量
	std::tie(tuple_0, tuple_1, tuple_2, tuple_3) = fourth;
	// 使用占位符
	// std::tie(tuple_0, std::ignore, tuple_2, std::ignore) = fourth;
	// 输出解包结果
	std::cout << "tuple_0 = " << tuple_0 << "\n";
	std::cout << "tuple_1 = " << tuple_1 << "\n";
	std::cout << "tuple_2 = " << tuple_2 << "\n";
	std::cout << "tuple_3 = " << tuple_3.c_str() << "\n";

	/*std::tuple_cat 执行拼接*/
	std::tuple<char, int, double> first('A', 1, 2.2f);

	// 组合到一起, 使用auto， 自动推导
	auto second = std::tuple_cat(first, std::make_tuple('B', std::string("-=+")));
	// 组合到一起，可以知道每一个元素的数据类型时什么 与 auto推导效果一样
	std::tuple<char, int, double, char, std::string> third = std::tuple_cat(first, std::make_tuple('B', std::string("-=+")));

	// 输出合并后的元组内容
	int index = 0;
	std::cout << index++ << " = " << std::get<0>(second) << "\n";
	std::cout << index++ << " = " << std::get<1>(second) << "\n";
	std::cout << index++ << " = " << std::get<2>(second) << "\n";

	std::cout << index++ << " = " << std::get<3>(second) << "\n";
	std::cout << index++ << " = " << std::get<4>(second).c_str() << "\n";

	/* 递归遍历*/
	auto t1 = std::make_tuple(1, 'A', "-=+", 2);
	std::cout << t1;
	return 0;
}