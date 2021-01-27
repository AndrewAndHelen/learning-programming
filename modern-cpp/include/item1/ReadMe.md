## Item1 :Understand template type deduction

条款⼀:理解模板类型推导

```
template<typename T>
void f(ParamType param);

f(expr);//使⽤表达式调⽤f

在编译期间，编译器使⽤expr进⾏两个类型推导：⼀个是针对T的，另⼀个是针对ParamType的。这两
个类型通常是不同的，因为ParamType包括了const和引⽤的修饰

T的推导不仅取决于expr的类型，也取决于ParamType的类型:
```

* 引言

```
在模板类型推导时，有引⽤的实参会被视为⽆引⽤，他们的引⽤会被忽略
对于通⽤引⽤的推导，左值实参会被特殊对待
对于传值类型推导，实参如果具有常量性和易变性会被忽略
在模板类型推导时，数组或者函数实参会退化为指针，除⾮它们被⽤于初始化引⽤
```

* 1.1 ParamType是⼀个指针或引⽤但不是通⽤引⽤
```
template<typename T>
void f1(T & param) { } //param是⼀个引⽤

情景1
1.如果expr的类型是⼀个引⽤，忽略引⽤部分 ,
2.然后剩下的部分决定T，然后T与形参匹配得出最终ParamType

int x = 27;
const int cx = x;
const int & rx = cx;
f1(x); //T是int，param的类型是int&
f1(cx); //T是const int，param的类型是const int &
f1(rx); //T是const int，param的类型是const int &
```

* 1.2 ParamType⼀个通⽤引⽤
```
template<typename T>
void f2(T&& param) { } //param现在是⼀个通⽤引⽤类型

情景2
1.如果expr是左值，T和ParamType都会被推导为左值引⽤。这⾮常不寻常，第⼀，这是模板类型推导中唯⼀⼀种T和ParamType都被推导为引⽤的情况。第⼆，虽然ParamType被声明为右值引⽤类型，但是最后推导的结果它是左值引⽤。
2.如果expr是右值，就使⽤情景⼀的推导规则

int x = 27; //如之前⼀样
const int cx = x; //如之前⼀样
const int & rx = cx; //如之前⼀样
f2(x); //x是左值，所以T是int&,param类型也是int&
f2(cx); //cx是左值，所以T是const int &,param类型也是const int&
f2(rx); //rx是左值，所以T是const int &,param类型也是const int&
f2(27); //27是右值，所以T是int,param类型就是int&&
```

* 1.3 ParamType既不是指针也不是引⽤
```
template<typename T>
void f3(T param) { } //以传值的⽅式处理param

情景三
1. 和之前⼀样，如果expr的类型是⼀个引⽤，忽略这个引⽤部分
2. 如果忽略引⽤之后expr是⼀个const，那就再忽略const。如果它是volatile，也会被忽略（volatile不常⻅，它通常⽤于驱动程序的开发中。关于volatile的细节请参⻅Item40)

int x = 27; //如之前⼀样
const int cx = x; //如之前⼀样
const int & rx = cx; //如之前⼀样
f3(x); //T和param都是int
f3(cx); //T和param都是int
f3(rx); //T和param都是int

//，对于形参来说指向const的指针或者指向const的引⽤在类型推导时const都会被保留
const char* const ptr = //ptr是⼀个常量指针，指向常量对象
" Fun with pointers";
f3(ptr); //T和param 为const char*
```

* 1.4 数组实参
```
const char name[] = "J. P. Briggs";
const char * ptrToName = name;
f3(name);//T被推导为const char*
f1(name);//T推导为了数组，param为const char(&)[13]

//有趣的是，对模板函数声明为⼀个指向数组的引⽤使得我们可以在模板函数中推导出数组的⼤小：
//在编译期间返回⼀个数组⼤小的常量值（
//数组形参没有名字，因为我们只关⼼数组
//的⼤小）
template<typename T, std::size_t N>
constexpr std::size_t arraySize(T(&)[N]) noexcept
{
	return N;
}

int Array[10];
std::cout << arraySize(Array);//输出Array的大小
```

* 1.5 函数实参
```
void someFunc(int, double); //someFunc是⼀个函数，类型是void(int,double)
template<typename T>
void f1(T param); //传值
template<typename T>
void f2(T & param); //传引⽤
f1(someFunc); //param被推导为指向函数的指针，类型是void(*)(int, double)
f2(someFunc); //param被推导为指向函数的引⽤，类型为void(&)(int, bouel)
```