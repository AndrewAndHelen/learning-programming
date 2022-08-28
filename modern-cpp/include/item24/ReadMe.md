## Item24: 区分通用引用与右值引用
转载自：https://blog.csdn.net/zhangyifei216/article/details/53648228
* 引言
```
1. 如果一个模版函数的参数类型是T&&，并且T的类型是推导出来的，或者一个对象使用auto&&这样的声明，那么这个函数的参数和要声明的对象都是通用引用类型
2. 如果类型的声明不是type&&，或者没有发生类型推导，那么type&&表示的是一个右值引用
3. 如果通用引用初始化的时候传入的是一个右值，那么就和右值引用一样，如果传入的是左值那么就喝左值引用是一样的
```

* 通用引用与右值引用的区别
```
void f(Widget&& param);         //右值引用
Widget&& var1 = Widget();       //右值引用
auto&& var2 = var1;             //通用引用
template<typename T>
void f(std::vector<T>&& param);  //右值引用

template<typename T>            //通用引用
void f(T&& param);
```
 这样看来T&&同一种形式，有了两种不同的含义，一种含义就是右值引用，可以绑定一个右值，表明这个对象是可以移动的。另外一种形式就是通用引用，既可以绑定一个左值，又可以绑定一个右值，通用引用通常有两种形式，第一种就是作为函数模版参数。
 ```
template<typename T>
void f(T&& param);  // param是一个通用引用
 ```
 第二种形式就是实用auto进行类型推导的时候。
 ```
auto&& var2 = var1; // var2 是一个通用引用
 ```
 上面两种形式都有一个共同的特点就是，类型是T&&(auto&& 也是T&&)，且带有类型推导，像下面这两种形式就不是通用引用。
 ```
void f(Widget&& param);     //没有类型推导，是右值引用
Widget&& var1 = Widget();   //没有类型推导，是右值引用
 ```
虽然上面两个形式上是T&&，但是没有类型推导，类型都是固定的，所以都不是通用引用，通用引用是引用，因此它必须初始化，在初始化的时候根据是左值还是右值，就决定了通用引用是右值引用还是左值引用。
```
template<typename T>
void f(T&& param);       // 参数是通用引用

Widget w;               // 传递左值进去，那么参数类型就是左值引用
f(w);

f(std::move(w));        //  传递右值进去，那么参数类型就是右值引用
```
 结合上面我们可以知道作为一个通用引用需要具备T&&(T是一个类型，或是auto)这种形式，其次就是要有类型的推导。只有满足这两者的才是通用引用。
 ```
template<typename T>
void f(std::vector<T>&& param); // 不符合T&& 这种形式

template<typename T>
void f(const T&& param);       // 不符合T&&，多了一个const，是右值引用
 ```
上面两种很容易就可以看出不是通用引用，那么下面这种可能会让你迷惑。
```
template<class T, class Allocator = allocator<T>>
class vector {
  public:
    void push_back(T&& x);
    .....
};
```
上面是vector的源码，其push_back从表面上看是符合通用引用的条件的(T&&形式，又有类型推导)，但是很可惜它不是一个通用引用，因为vector是一个模版，当vector具现化后，push_back的行参就不具备通用引用的条件了。
```
std::vector<Widget> v;
class vector<Widget, allocator<Widget>> {
  public:
    void push_back(Widget&& x); //不具备通用引用的条件
}
```
到了C++11的时候，你会发现多了一个emplace_back，它可以接受一个右值，它是如何做到的呢?
```
template<class T, class Allocator = allocator<T>>
class vector {
  public:
    template<class... Args> // 其参数不再依赖T，而是在调用emplace_back的时候才进行类型推导
    void emplace_back(Args&&... args);
    .....
};
```
上文中列举了很多通用引用的例子，有一个共同的特点就是函数模版参数通常都是通用引用，到了C++14就连lambda的参数也可以是通用引用。
```
auto timeFuncInvocation = [](auto&& func, auto&&... params){
  ....
}
```
C++14中支持在参数中使用auto，上面这个lambda函数中的func就是通用引用。
