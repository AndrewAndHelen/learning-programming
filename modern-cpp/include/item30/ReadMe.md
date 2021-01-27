## Item30:熟悉完美转发的失败情况
转载自：https://blog.csdn.net/zhangyifei216/article/details/54897406
* 引言
```
1. 完美转发会失败当模板类型推导失败或者推导类型错误
2. 导致完美转发失败的类型有braced initializers；作为空指针的0或者NULL；只声明的整型static const数据成员；模板和重载的函数名；位域
```
在C++11中最为显著的一个新特性，当属完美转发了，是的它很完美，和它的名字一样，转发两字意味着一个函数将其参数传递给另外一个函数，第二个函数的目标则是接收来自于第一个函数传递过来的对象，转发二字体现出第二个函数接收到的对象应该和第一个参数传递过来的是相同的，因此如果采用值传递的方式就没有办法达到效果了，因为拷贝后的对象是原来的对象是两个不同的对象，如果想在第二个函数中操作传递过来的对象也达不到效果，因为操作的是复制后的对象，并不是原对象。如果采用指针传递的话，的确是可以达到转发的效果。但是这要求用户必须传递指针，因此算不上完美。

完美转发意味着不仅仅要转发对象本身，还要转发它附带的属性，它的类型，左值或者是右值，是否是`const`或`volatile`，根据Item24中的介绍，这里只有通用引用可以做到。下面是一个完美转发的例子。
```
template<typename T>
void fwd(T&& param) {       // 这里是通用引用
  f(std::forward<T>(param));
}
```
不仅仅可以转发一个参数，还可以结合可变参数转发多个参数，如下:
```
template<typename... _Args>
void fwd(_Args&&... params) {
  f(std::forward<_Args>(params)...);
}
```
这个和STL中的make_shared和make_unique是一样的，make_shared源码如下:
```
template<typename _Tp, typename... _Args>
  inline shared_ptr<_Tp>
  make_shared(_Args&&... __args)
{   
  typedef typename std::remove_const<_Tp>::type _Tp_nc;
  return std::allocate_shared<_Tp>(std::allocator<_Tp_nc>(),
                                   std::forward<_Args>(__args)...);
}   
```
然而完美转发并不完美，在一些场合下会因为类型推导错误导致转发了错误的类型，最后导致执行失败，本文则是通过讲解这些失败的场景，让我们知道哪些场景下完美转发无法正常工作。
* 支撑初始化
```
void f(const std::vector<int>& v);
f({1, 2, 3});   // {1, 2, 3}会隐式转换成std::vector<int>，因为vector带有列表初始化的构造函数
```
因为std::vector<int>带有列表初始化的构造函数，所以{1, 2, 3}可以隐式构造成std::vector<int> 
```
vector(initializer_list<value_type> __l,            // 初始化列表
       const allocator_type& __a = allocator_type())
  : _Base(__a)
  {    
    _M_range_initialize(__l.begin(), __l.end(),
                        random_access_iterator_tag());
  }  
```
但是如果将{1, 2, 3}通过完美转发给f的话则导致编译失败。
```
fwd({1, 2, 3});
```
上面的调用编译失败了，其原因是因为{1, 2, 3}进行模版推导失败了，所以找不到匹配的fwd函数，编译出错的信息如下:
```
note: candidate template ignored: couldn't infer template argument 'T'
```
这就是完美转发遇到的第一个失败的例子，解决这个问题的方法也很简单，在Item2中介绍过，尽管模版推导无法推导出初始化列表的正确类型，但是auto可以，所以上面的代码改成如下的形式就可以顺利运行。
```
auto il = {1, 2, 3};
fwd(il);
```

* 0或者NULL 作为空指针

在Item8中提到过，当0或者是NULL作为指针类型传递给一个模版函数的时候，会推导出错误的类型，把0或者NULL作为int类型，很显然0或者NULL无法被当作指针类型进行完美转发，解决这个问题也很简单，使用C++11中的nullptr替换即可。

* 仅仅声明整型的静态const数据成员
```
class Widget {
public:
    static const std::size_t MinVals = 28;
};
```
上面的MinVals只是声明了，但是还没有在类外进行定义，任何一个学过C++的人都知道静态成员变量需要在类外进行初始化，但是static const是一个例外，它可以不用在类外进行定义，但是它在内存中不会分配实际的存储。所以下面的代码运行一切正常。
```
void f2(std::size_t val) {
    ....
}
f2(Widget::MinVals);
```

编译期就会直接把`Widget::MinVals`替换成28了，然后进行实际的调用了。一旦对`Widget::MinVals`进行取地址的操作，会导致MinVals去寻找定义，但是上面的代码没有在类外定义MinVals，这就导致在链接阶段报错，找不到`Widget::MinVals`的定义。
```
const std::size_t *p = &(Widget::MinVals);
```

静态const数据成员的这个特点在完美转发的场景下也会带来同样的问题，完美转发是通过通用引用来转发参数，通用引用本质上是指针，因此在这个场景下如果没有在类外定义MinVals也会在链接的时候报错。
```
template<typename T>
void fwd(T&& param) {       // 这里是通用引用
  f(std::forward<T>(param));
}
fwd(Widget::MinVals);
```
* 函数重载和函数模版
```
void f(int (*pf)(int)); //函数f接收一个函数作为参数

int processVal(int value);
int processVal(int value, int priority);

fwd(processVal);    // 编译失败
```
上面的fwd通过完美转发processVal但是编译失败，因为模版的类型推导无法推导出processVal的类型。这就是在函数重载的场景下模版类型推导失败的例子。同样如果使用函数模版替换模版重载的话，模版类型推导依然失败，无法推导出函数模版的类型。
```
template<typename T>
T workOnVal(T param) {
  .....
}

fwd(workOnVal);
```
解决上面这两个问题也很简单，就是主动赋予函数和函数模版的类型，代码如下:
```
using ProcessFuncType = int(*)(int);
ProcessFuncType processValPtr = processVal;
fwd(processValPtr);

fwd(static_cast<ProcessFuncType>(workOnVal));
```

* 位域
最后一种完美转发失效的情况是位域，代码如下:
```
struct IPv4Header {
  std::uint32_t version:4,
               IHL:4,
               DSCP:6,
               ECN:2,
               totalLength:16;
  .....
};

void f(std::size_t sz);
IPv4Header h;
f(h.totolLength);
fwd(h.totolLength);     //编译失败
```
上面的代码中位域h.totolLength被传递给f的时候可以正常工作，当传递给fwd完美转发的时候编译失败，因为fwd是一个模版，模版的参数是通用引用，本质上是一个引用，C++标准规定一个非const的引用无法引用一个位域字段。这个规定也是有原因的，因为位域可能只是一个int的部分字节，没有一个确定的地址，没办法通过指针指向位域，引用本质上就是指针，自然没办法引用位域了。既然没办法对一个位域进行引用那么可以通过拷贝位域的值后然后再进行完美转发，代码如下:
```
auto length = static_cast<std::uint16_t>(h.totolLength);
fwd(length);
```

