## enable_if

* 引言
```
1.如果这个表达式结果为true，它的type 成员会返回一个类型：
-- 如果没有第二个模板参数，返回类型是void。
-- 否则，返回类型是其第二个参数的类型。
2.如果表达式结果false，则其成员类型是未定义的。根据模板的一个叫做SFINAE（substitute failure is not an error，替换失败不是错误）的规则，这会导致包含std::enable_if<>表达式的函数模板被忽略掉。

template<bool Cond, class T = void> 
struct enable_if {
};

template<class T> 
struct enable_if<true, T> { 
	typedef T type; 
};
```

* 用法一：校验函数模板参数类型
```
有时定义的模板函数，只希望特定的类型可以调用
// 1. 返回类型（布尔）仅在T为整数类型时有效：
template <typename T>
typename std::enable_if<std::is_integral<T>::value,bool>::type
is_odd (T i) {
    return bool(i%2);
}

// 2. 第二个模板参数仅在T为整数类型时有效：
template < typename T,
		   typename = typename std::enable_if<std::is_integral<T>::value>::type>
bool is_even (T i) {
    return !bool(i%2);
}

int main() {
  int i = 1;    // 如果i的类型不是整数，则代码无法编译
  std::cout << "i is odd: " << is_odd(i) << std::endl;
  std::cout << "i is even: " << is_even(i) << std::endl;

  return 0;
}
//一个通过返回值，一个通过默认模板参数，都可以实现校验模板参数是整型的功能。
```

* 用法二：类模板的偏特化
```
在使用模板编程时，经常会用到根据模板参数的某些特性进行不同类型的选择，或者在编译时校验模板参数的某些特性。例如：
template <typename T, typename Enable=void>
struct check{
};

template <typename T>
struct check<T, typename std::enable_if<T::value,XXX>::type> {
  //static constexpr bool value = T::value;
};

上述的 check 只希望选择 T::value==true 的 T，否则就报编译时错误，即在特化时进行了条件筛选。
这里需要注意的是模板特化的优先级较高，即使使用了缺省值，依然是特化版本优先！！！
```

* 用法三：控制函数的返回类型
```
std::tuple 的获取第 k 个元素的 get 函数：
template <std::size_t k, class T, class... Ts>
typename std::enable_if<k==0, typename element_type_holder<0, T, Ts...>::type&>::type
get(tuple<T, Ts...> &t) {
  return t.tail; 
}
 
template <std::size_t k, class T, class... Ts>
typename std::enable_if<k!=0, typename element_type_holder<k, T, Ts...>::type&>::type
get(tuple<T, Ts...> &t) {
  tuple<Ts...> &base = t;
  return get<k-1>(base); 
}
```

* 用法四：禁用某些成员函数
```
不能通过使用enable_if<>来禁用copy/move 构造函数以及赋值构造函数
这是因为成员函数模板不会被算作特殊成员函数（依然会生成默认构造函数），而且在需要使用copy 构造函数的地方，相应的成员函数模板会被忽略掉。因此即使像下面这样定义类板：
class C {
public:
    template<typename T>
    C (T const&) {
    	std::cout << "tmpl copy constructor\n";}
		…
};

在需要copy 构造函数的地方依然会使用预定义的copy 构造函数：
C x;
C y{x}; // still uses the predefined copy constructor (not the member template)

删掉copy 构造函数也不行，因为这样在需要copy 构造函数的地方会报错说该函数被删除了。

但是也有一个办法：可以定义一个接受const volatile 的copy 构造函数并将其标示为delete。这样做就不会再隐式声明一个接受const 参数的copy 构造函数。在此基础上，可以定义一个构造函数模板，对于nonvolatile 的类型，它会优选被选择（相较于已删除的copy 构造函数）：
class C
{
public:
    …
    // user-define the predefined copy constructor as deleted
    // (with conversion to volatile to enable better matches)
    C(C const volatile&) = delete;
    // implement copy constructor template with better match:
    template<typename T>
    C (T const&) {
   		std::cout << "tmpl copy constructor\n";
    }
    …
};
这样即使对常规copy，也会调用模板构造函数：
C x;
C y{x}; // uses the member template

于是就可以给这个模板构造函数添加enable_if<>限制。比如可以禁止对通过int 类型参数实例化出来的C<>模板实例进行copy：
template<typename T>
class C
{
public:
…
	// user-define the predefined copy constructor as deleted(with conversion to volatile to enable better matches)
	C(C const volatile&) = delete;
	// if T is no integral type, provide copy constructor template with better match:

    template<typename U,
    typename = std::enable_if_t<!std::is_integral<U>::value>>
   	C (C<U> const&) {
    …
    }
…
};
```