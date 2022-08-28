## Item 2:Understand auto type deduction

条款⼆:理解auto类型推导

* 引言

```
1.auto类型推导通常和模板类型推导相同，但是auto类型推导假定花括号初始化代表
std::initializer_list而模板类型推导不这样做
2.在C++14中auto允许出现在函数返回值或者lambda函数形参中，但是它的⼯作机制是模板类型推
导那⼀套⽅案。
```

* 2.1 与模板类型推导相同之处

```
auto与模板类型推导类似，auto扮演了模板的角色。
变量的类型说明符扮演了ParamType的角色

auto x = 27;//类型说明符	auto
const auto cx = x;//类型说明符	const auto
const auto & rx=cx;//类型说明符	const auto &

等价于：
template<typename T> //理想化的模板⽤来推导x的类型
void func_for_x(T param);
func_for_x(27);

template<typename T> //理想化的模板⽤来推导cx 的类型
void func_for_cx(const T param);
func_for_cx(x);

template<typename T> //理想化的模板⽤来推导rx的类型
void func_for_rx(const T & param);
func_for_rx(x);
```

* 2.2 与模板类型推导不同之处

```
变量初始化方法：
auto x1=27; //类型是int，值是27
auto x2(27); //同上
auto x3={27}; //类型是std::initializer_list<int>,值是{27}
auto x4{27}; //同上

case1:
auto x5={1,2,3.0}; //错误！auto类型推导不能⼯作，发⽣了两种类型推导！

case2:
template<typename T>
void f(T param);
f({11,23,9}); //错误！不能推导出T

case3:
template<typename T>
void f(std::initializer_list<T> initList);
f({11,23,9}); //T被推导为int，initList的类型被推导为std::initializer_list<int>

case4:
C++14允许auto⽤于函数返回值并会被推导（参⻅Item3）
虽然表⾯上使⽤的是auto但是实际上是模板类型推导的那⼀套规则在⼯作
auto createInitList()
{return {1,2,3}; //错误！推导失败}
```

  