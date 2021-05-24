// use of defaulted functions
#include <iostream>
using namespace std;

class A {
public:
	// A user-defined
	A(int x) {
		cout << "This is a parameterized constructor";
	}

	// Using the default specifier to instruct
	// the compiler to create the default implementation of the constructor.
	A() = default;
};

// non-special member functions can't be defaulted(非特殊成员函数不能使用default) 
class B {
public:
	// Error, func is not a special member function. 
	int func() = default;

	// Error, constructor B(int, int) is not a special member function. 
	B(int, int) = default;

	// Error, constructor B(int=0) has a default argument. 
	B(int = 0) = default;
};



int main() {
	A a;          //call A()
	A x(1);       //call A(int x)
	cout << endl;
	return 0;
}

