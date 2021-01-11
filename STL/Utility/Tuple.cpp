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


// ƫ���ԣ��������Ϊ�ݹ����ֹ
template<typename Tuple>
struct tuple_show < Tuple, 1>
{
	static void show(const Tuple &t, std::ostream &os)
	{
		os << std::get<0>(t);
	}
};



// �Լ�д����������������ĵݹ�չ����
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
	/* tuple��ʼ��*/
	std::tuple<int, double, std::string> t(64, 128.0, "Caroline");
	std::tuple<std::string, std::string, int> t2 =
		std::make_tuple("Caroline", "Wendy", 1992);

	/*����Ԫ�ظ��� */ 
	size_t num = std::tuple_size<decltype(t)>::value;
	std::cout << "num = " << num << std::endl;

	/*��ȡ��1��ֵ��Ԫ������*/  
	std::tuple_element<1, decltype(t)>::type cnt;
	cnt = std::get<1>(t);
	std::cout << "cnt = " << cnt << std::endl;

	/*�Ƚϴ�С*/  
	std::tuple<int, int> ti(24, 48);
	std::tuple<double, double> td(28.0, 56.0);
	bool b = (ti < td);
	std::cout << "b = " << b << std::endl;

	/*tuple��Ϊ����ֵ*/  
	auto a = giveName();
	std::cout << "name: " << std::get<0>(a)
		<< " years: " << std::get<1>(a) << std::endl;

	/*std::tie���*/
	std::tuple<char, int, long, std::string> fourth('A', 2, 3, "4");

	// ������������������
	char tuple_0 = '0';
	int tuple_1 = 0;
	long tuple_2 = 0;
	std::string tuple_3("");

	// ʹ��std::tie, ���δ����Ӧ�Ľ������
	std::tie(tuple_0, tuple_1, tuple_2, tuple_3) = fourth;
	// ʹ��ռλ��
	// std::tie(tuple_0, std::ignore, tuple_2, std::ignore) = fourth;
	// ���������
	std::cout << "tuple_0 = " << tuple_0 << "\n";
	std::cout << "tuple_1 = " << tuple_1 << "\n";
	std::cout << "tuple_2 = " << tuple_2 << "\n";
	std::cout << "tuple_3 = " << tuple_3.c_str() << "\n";

	/*std::tuple_cat ִ��ƴ��*/
	std::tuple<char, int, double> first('A', 1, 2.2f);

	// ��ϵ�һ��, ʹ��auto�� �Զ��Ƶ�
	auto second = std::tuple_cat(first, std::make_tuple('B', std::string("-=+")));
	// ��ϵ�һ�𣬿���֪��ÿһ��Ԫ�ص���������ʱʲô �� auto�Ƶ�Ч��һ��
	std::tuple<char, int, double, char, std::string> third = std::tuple_cat(first, std::make_tuple('B', std::string("-=+")));

	// ����ϲ����Ԫ������
	int index = 0;
	std::cout << index++ << " = " << std::get<0>(second) << "\n";
	std::cout << index++ << " = " << std::get<1>(second) << "\n";
	std::cout << index++ << " = " << std::get<2>(second) << "\n";

	std::cout << index++ << " = " << std::get<3>(second) << "\n";
	std::cout << index++ << " = " << std::get<4>(second).c_str() << "\n";

	/* �ݹ����*/
	auto t1 = std::make_tuple(1, 'A', "-=+", 2);
	std::cout << t1;
	return 0;
}