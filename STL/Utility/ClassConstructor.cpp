#include"ClassConstructor.hpp"

int main()
{
	vector<MyString> vecStr;
	vecStr.reserve(1000); //先分配好1000个空间
	for (int i = 0; i < 1000; i++) {
		MyString tmp("hello");
		vecStr.push_back(tmp); //调用的是拷贝构造函数
	}
	cout << "CCtor = " << MyString::CCtor << endl;
	cout << "MCtor = " << MyString::MCtor << endl;
	cout << "CAsgn = " << MyString::CAsgn << endl;
	cout << "MAsgn = " << MyString::MAsgn << endl;

	cout << endl;
	MyString::CCtor = 0;
	MyString::MCtor = 0;
	MyString::CAsgn = 0;
	MyString::MAsgn = 0;
	vector<MyString> vecStr2;
	vecStr2.reserve(1000); //先分配好1000个空间
	for (int i = 0; i < 1000; i++) {
		MyString tmp("hello");
		vecStr2.push_back(std::move(tmp)); //调用的是移动构造函数
	}
	cout << "CCtor = " << MyString::CCtor << endl;
	cout << "MCtor = " << MyString::MCtor << endl;
	cout << "CAsgn = " << MyString::CAsgn << endl;
	cout << "MAsgn = " << MyString::MAsgn << endl;
	
	std::cin.get();
	return 0;
}