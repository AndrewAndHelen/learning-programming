//std::auto_ptr已被废弃，不要用它
//主要用std::unique_ptr、std::shared_ptr
//优先级 ()>[]>*
#include<iostream>
#include<string>
#include<vector>
#include<memory>
using namespace std;

int main()
{
	//当最后一个拥有者销毁时，会打印信息
	shared_ptr<string> pNico(new string("nico"),
		[](string* p) {
		cout << "delete " << *p << endl;
		delete p;
	});
	shared_ptr<string> pJutta(new string("jutta"));

	(*pNico)[0] = 'N';
	pJutta->replace(0, 1, "J");

	//多次复制到一个容器里
	vector<shared_ptr<string>> whoMadeCoffee;
	whoMadeCoffee.push_back(pJutta);
	whoMadeCoffee.push_back(pJutta);
	whoMadeCoffee.push_back(pNico);
	whoMadeCoffee.push_back(pJutta);
	whoMadeCoffee.push_back(pNico);

	//打印所有元素
	for (auto ptr : whoMadeCoffee)
	{
		cout << *ptr << " ";
	}
	cout << endl;

	//重写名字
	*pNico = "Nicolai";
	for (auto ptr : whoMadeCoffee)
	{
		cout << *ptr << " ";
	}
	cout << endl;
	//查看引用计数为多少
	cout << "use_count:" << whoMadeCoffee[0].use_count() << endl;

	/*如果std::shared_ptr指向一个数组，要自定义deleter*/
	//第一种
	//shared_ptr<int> p(new int[10],
	//	[](int *p) {
	//	delete[] p;
	//});
	////第二种
	//shared_ptr<int> p(new int[10],
	//	default_delete<int[]>());

	//指向一个数组时，std::unique_ptr和std::shared_ptr用法不同
	std::unique_ptr<int[]> p(new int[10]);
	std::shared_ptr<int> q(new int[10]);
	
	//shared_ptr不提供operator [],unique_ptr提供operator []取代operator* 和operator ->
	int a = 1;

}