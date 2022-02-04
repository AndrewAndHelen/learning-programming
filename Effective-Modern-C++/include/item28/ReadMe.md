## Item28:理解引用折叠
转载自：https://blog.csdn.net/zhangyifei216/article/details/54237603
* 引言
```
1. 引⽤折叠发⽣在四种情况：模板实例化；auto类型推导；typedef的创建和别名声明；decltype
2. 当编译器⽣成了引⽤的引⽤时，结果通过引⽤折叠就是单个引⽤。有左值引⽤就是左值引⽤，否则就是右值引⽤
3. 通⽤引⽤就是通过类型推导区分左值还是右值，并且引⽤折叠出现的右值引⽤
```
回顾一下通用引用的类型推导，请看下面这个例子:
```
template<typename T>
void func(T&& param);
```

面是一个通用引用模版，根据传入参数类型的不同，T被推导后的类型也不同，但整个推导的机制比较简单，当传入的是一个左值的时候，T被推导为左值引用类型，当传入是一个右值的时候，T和传入的参数类型相同，可以使用下面的方法进行测试(也可以参考Item1)。

```
template <typename T>
void func1(T&& data) {
    if (std::is_lvalue_reference<T>::value) {
        std::cout << "data is lvalue reference" << "\n";
    } else if (std::is_same<T, int>::value) {
        std::cout << "data is int" << "\n";
    }
}

int data = 100;
func1(data);    // 会输出 data is lvalue reference  ，T被推导成左值引用
func1(100);     // 会输出 data is int  T被推导成和100的类型相同也就是int
```
上文在介绍通用引用类型推导的时候介绍到，当传入的类型是一个左值的时候，最终会被推导到一个左值引用类型，然后结合&&最终还是一个左值引用类型。整个过程如下:
```
template <typename T>
void func1(T&& data);
Setp1: 传入一个int型的左值，那么T被推导成int&
Setp2: 实例化后变成 void func1(int& && data);
Setp3: 经过某种机制转换后变成 void func1(int& data);
```
上面说到的某种机制其实就是本文要探讨的，它被称为引用折叠。这种机制就是用来支撑通用引用的类型推导和std::forward的核心。引用折叠规定了两种类型折叠，一种就是左值引用和&&发生折叠后就是左值引用，除此之外折叠后的结果都是右值引用。介绍完引用折叠后，我相信接下来解释std::forward的工作原理将会变的越发轻松，下面是std::forward的典型实现。
```
template<typename T>
T&& forward<typename remove_reference<T>::type& param)
{
  return static_cast<T&&>(param);
}
```
当传入的类型是一个左值的时候，比如int，按照模版的推导规则T会被推导成T&(参考上文或者Item1)，然后整个模版就变成了如下这种形式:
```

int& && forward<typename remove_reference<int&>::type& param)
{
  return static_cast<int& &&>(param);
}
```
上面是模版实例化后的状态，这个时候引用折叠机制开始派上用场了，经过引用折叠机制后，变成了如下这种形式，并最终返回一个左值引用类型(左值引用本质上就是左值)。这完全符合std::forward的预期，传入左值那么std::forward则返回左值，实际上并没有做任何类型cast。
```
int& forward<int& param)
{
  return static_cast<int&>(param);
}
```

接着测试另外一种场景，当用户传入的是一个右值的情况下，按照模版的推导规则T会被推导成传入的参数的类型本身，然后整个模版实例化成如下这种形式:
```
int&& forward<int& param)
{
  return static_cast<int&&>(param);
}
```
最终返回的是一个右值引用类型，符合std::forward的预期，和前文中提到的std::forward本质上是一个有条件的类型cast，只有当传入的是右值时才会进行类型的cast这一理念不谋而合。

到此为此本文所要描述的核心内容已经结束，但是总的来说上文只总结了引用折叠在模版类型推导这一场景下的应用，其实引用折叠除了这个场景外，还广泛应用在其他地方，总结来说有四个应用场景。

* 模版类型推导
* 生成auto变量
```
Widget w;
auto&& w1 = w;    // w是一个左值，按照模版类型推导的规则，会被推导成Widget&
Widget& && w1 = w; 经过引用折叠后变成了Widget& w1 = w;
auto&& w2 = Widget(); // 传入的是一个右值，按照模版类型推导的规则，会被推导成右值Widget
widget&& w2 = Widget();
```
* typedef类型别名
```
template<typename T>
class Widget {
public:
    typedef T&& RvalueRefToT;
    ....
};

Widget<int&> w;   //T在这里是直接被实例化成int&，没有经过推导
typedef int& && RvalueRefToT; 发生引用折叠后变成了typedef int& RvalueRefToT;
```

* decltyoe类型推导
请看Item3
