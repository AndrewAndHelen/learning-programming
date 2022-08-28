#include <iostream>
#include <memory>

class shared {
private:
	std::shared_ptr<int> p;
public:
	shared(std::shared_ptr<int>& _p):p(_p){ }
	void print()
	{
		std::cout << "count: " << p.use_count() << "v= " << *p << std::endl;
	}

};
void print_func(std::shared_ptr<int> p)
{
	std::cout << "count: " << p.use_count() << "v= " << *p << std::endl;
}

void case19_1()
{
	std::shared_ptr<int> sp(new int(10));
	std::cout << *sp<<std::endl;

	std::shared_ptr<int> sp2 = sp;
	std::cout << sp.use_count() << std::endl;

	*sp2 = 100;
	sp.reset();																	//sp不在持有任何指针
	std::cout << sp2.use_count() << std::endl;
}

void case19_2()
{
	std::shared_ptr<int> p(new int(100));
	shared s1(p), s2(p);

	s1.print();
	s2.print();

	*p = 20;
	print_func(p);

	s1.print();

	/*输出
	count: 3v = 100
	count : 3v = 100
	count : 4v = 20
	count : 3v = 20*/
}

void case19_3()
{
	auto pw = new int; // pw是原始指针
	std::shared_ptr<int> spw1(pw); // 为*pw创建控制块
	std::shared_ptr<int> spw2(pw); // 为*pw创建第⼆个控制块,出现异常
}
int main()
{
	case19_3();
	return 0;
}