// copy-constructor using delete operator 
#include <iostream> 
using namespace std;

class A {
public:
	A(int x) : m(x) { }

	// Delete the copy constructor 
	A(const A&) = delete;
	// Delete the copy assignment operator 
	A& operator=(const A&) = delete;
	int m;
};

class B {
public:
	B(int) {}

	// Declare the conversion constructor as a  deleted function. Without this step,  
	// even though A(double) isn't defined,  the A(int) would accept any double value
	//  for it's argumentand convert it to an int 
	B(double) = delete;
};

int main() {
	A a1(1), a2(2), a3(3);
	// Error, the usage of the copy assignment operator is disabled 
	a1 = a2;
	// Error, the usage of the copy constructor is disabled 
	a3 = A(a2);

	B B1(1);
	// Error, conversion from  double to class A is disabled. 
	B B2(100.1);
	return 0;
}
