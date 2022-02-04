## Item22:当使用Pimpl惯用法，请在实现文件中定义特殊成员函数
* 引言
```
1.pImpl为了保护我们的头文件，避免在修改类的数据成员的时候导致依赖这个头文件的程序也需要重新编译，常常被人们称为编译防火墙。
2.对于std::unique_ptr类型的pImpl指针，需要在头⽂件的类⾥声明特殊的成员函数，但是在实现⽂件⾥⾯来实现他们。即使是编译器⾃动⽣成的代码可以⼯作，也要这么做。
3.以上的建议只适⽤于std::unique_ptr，不适⽤于std::shared_ptr。
```

* 使用智能指针代替裸指针管理内存分配与释放
```
//声明
class Widget //在"Widget.h"中
{
public:
Widget();
...
private:
struct Impl; //声明⼀个 实现结构体
std::unique_ptr<Impl> pImpl; //使⽤智能指针而不是原始指针
}

//实现
#include "widget.h" //以下代码均在实现⽂件 widget.cpp⾥
#include "gadget.h"
#include <string>
#include <vector>
struct Widget::Impl //跟之前⼀样
{
std::string name;
std::vector<double> data;
Gadget g1,g2,g3;
}
Widget::Widget() //根据Item 21, 通过std::make_shared来创建std::unique_ptr
: pImpl(std::make_unique<Imple>())
{}
```

你会注意到， Widget 的析构函数不存在了。这是因为我们没有代码加在里面了。 `std::unique_ptr` 在 自身析构时，会⾃动销毁它所指向的对象，所以我们不用手动销毁任何东西。这就是智能指针的众 多优点之⼀：它使我们从⼿动资源释放中解放出来。 以上的代码能编译，但是，最普通的 Widget 用法却会导致编译出错：

```
#include "widget.h"
Wdiget w; //编译出错
```

你所看到的错误信息根据编译器不同会有所不同，但是其⽂本⼀般会提到⼀些有关于把 sizeof 和 delete 应用到未完成类型 incomplete type 上的信息。对于未完成类型，使用以上操作是禁止的。

> unique_ptr支持不完全类型(前向声明)，如果在头文件中没有声明默认的析构函数，运行程序会报错。

原因：当实例化对象析构时，析构函数被调用，如果在类的定义中使用了unique_ptr，如果没有声明默认析构函数，C++编译器会替我们声明一个。在这个析构函数中，编译器会插入代码调用Wideget的数据成员Pimpl的析构函数。Pimpl 是⼀个std::unique_ptr<Widget::Impl> ,也就是说，⼀个带有默认销毁器(default deleter)的std::unique_ptr。 默认销毁器(default deleter)是⼀个函数，它使用 delete 来销毁内置于std::unique_ptr 的原始指针。然而，在使⽤delete之前，通常会使默认销毁器使用C++11的特性static_assert来确保原始指针指向的类型不是⼀个未完成类型。 当当编译器为 Widget w 的析构生成代码时，它会遇到 static_assert 检查并且失败，这通常是错误信息的来源。 这些错误信息只在对象w销毁的地⽅出现，因为类 Widget 的析构函数，正如其他的编译器⽣成的特殊成员函数⼀样，是暗含inline 属性的。 错误信息⾃⾝往往指向对象 w 被创建的那⾏，因为这⾏代码明确地构造了这个对象，导致了后⾯潜在的析构。

解决方法：你只需要确保在编译器⽣成销std::unique_ptr<Widget::Imple> 的代码之前，Widget::Impl已经是⼀个完成类型(complete type)。当编译器"看到"它的定义的时候，该类型就成为完成类型了。但是 Widget::Impl的定义在 wideget.cpp 里。成功编译的关键，就是，在widget.cpp文件内，让编译器"看到"Widget的析构函数实现之前（也即编译器自动插⼊销毁std::unique_ptr 的数据成员的位置），先定义Wdiget::Impl 。做出这样的调整很容易。只需要在先在 widget.h 里，只声明(declare)类 Widget 的析构函数，却不要在这里定义(define)它。

做出这样的调整很容易。只需要在先在 widget.h 里，只声明(declare)类 Widget 的析构函数，却不用定义(define)它:

```
class Widget { // as before, in "widget.h"
public:
Widget();
~Widget(); // declaration only
...
private: // as before
struct Impl;
std::unique_ptr<Impl> pImpl;
};
```

在 widget.cpp文件中，在结构体 Widget::Impl 被定义之后，再定义析构函数

```
#include "widget.h" //以下代码均在实现⽂件 widget.cpp⾥
#include "gadget.h"
#include <string>
#include <vector>
struct Widget::Impl //跟之前⼀样,定义Widget::Impl
{
std::string name;
std::vector<double> data;
Gadget g1,g2,g3;
}
Widget::Widget() //根据Item 21, 通过std::make_unique来创建std::unique_ptr
: pImpl(std::make_unique<Imple>())
{}
Widget::~Widget() //析构函数的定义(译者注：这⾥⾼亮)
{}
```

*  支持移动操作

使⽤了 Pimpl 惯⽤法的类⾃然适合⽀持移动操作，因为编译器⾃动⽣成的移动操作正合我们所意： 对隐 藏的 std::unique_ptr 进⾏移动。 正如 Item 17 所解释的那样，声明⼀个类 Widget 的析构函数会阻 ⽌编译器⽣成移动操作，所以如果你想要⽀持移动操作，你必须⾃⼰声明相关的函数。考虑到编译器⾃ 动⽣成的版本能够正常功能，你可能会被诱使着来这样实现:

```
class Widget //在"Widget.h"中
{
public:
Widget();
~Widget();
...
Widget(Widget&& rhs) = default; //思路正确，但代码错误
Widget& operator=(Widget&& rhs) = default;
private:
struct Impl; //如上
std::unique_ptr<Impl> pImpl;
}
```

这样的做法会导致同样的错误，和之前的声明⼀个不带析构函数的类的错误⼀样，并且是因为同样的原 因。 编译器⽣成的移动赋值操作符(move assignment operator)，在重新赋值之前，需要先销毁指针 pImpl 指向的对象。然而在 Widget 的头⽂件⾥， pImpl 指针指向的是⼀个未完成类型。情况和移动构 造函数(move constructor)有所不同。 移动构造函数的问题是编译器⾃动⽣成的代码⾥，包含有抛出异 常的事件，在这个事件⾥会⽣成销毁 pImpl 的代码。然而，销毁 pImpl 需要 Impl 是⼀个完成类型。

因为这个问题同上⾯⼀致，所以解决⽅案也⼀样——把移动操作的定义移动到实现⽂件⾥:

```
class Widget //在"Widget.h"中
{
public:
Widget();
~Widget();
...
Widget(Widget&& rhs); //仅声明
Widget& operator=(Widget&& rhs);
private:
struct Impl; //如上
std::unique_ptr<Impl> pImpl;
}
```

```
#include "widget.h" //以下代码均在实现⽂件 widget.cpp⾥
#include "gadget.h"
#include <string>
#include <vector>
struct Widget::Impl //跟之前⼀样,定义Widget::Impl
{
std::string name;
std::vector<double> data;
Gadget g1,g2,g3;
}
Widget::Widget() //根据Item 21, 通过std::make_unique来创建std::unique_ptr
: pImpl(std::make_unique<Imple>())
{}
Widget::~Widget() = default;
Widget(Widget&& rhs) = default; //在这⾥定义
Widget& operator=(Widget&& rhs) = default;
```

pImpl 惯⽤法是⽤来减少类实现者和类使⽤者之间的编译依赖的⼀种⽅法，但是，从概念而⾔，使⽤这 种惯⽤法并不改变这个类的表现。 原来的类 Widget 包含有 std::string , std::vector 和 Gadget 数 据成员，并且，假设类型 Gadget ，如同 std::string 和 std::vector ⼀样,允许复制操作，所以类 Widget ⽀持复制操作也很合理。 我们必须要⾃⼰来写这些函数，因为第⼀，对包含有只可移动(moveonly)类型，如 std::unique_ptr 的类，编译器不会⽣成复制操作;第⼆，即使编译器帮我们⽣成了，⽣ 成的复制操作也只会复制 std::unique_ptr (也即浅复制(shallow copy)),而实际上我们需要复制指针所 指向的对象(也即深复制(deep copy))。

```
class Widget //在"Widget.h"中
{
public:
Widget();
~Widget();
...
Widget(const Widget& rhs); //仅声明
Widget& operator=(const Widget& rhs);
private:
struct Impl; //如上
std::unique_ptr<Impl> pImpl;
}

```

```
#include "widget.h" //以下代码均在实现⽂件 widget.cpp⾥
#include "gadget.h"
#include <string>
#include <vector>
struct Widget::Impl //跟之前⼀样,定义Widget::Impl
{
...
}
Widget::Widget() //根据Item 21, 通过std::make_shared来创建std::unique_ptr
: pImpl(std::make_unique<Imple>())
{}
Widget::~Widget() = default;
...
Widget::Widget(const Widget& rhs)
:pImpl(std::make_unique<Impl>(*rhs.pImpl))
{}
Widget& Widget::operator=(const Widget& rhs)
{
*pImpl = *rhs.pImpl;
return *this;
}
```

如果我们使⽤ std::shared_ptr 而不是 std::unique_ptr 来做 pImpl 指针， 我们会发 现本节的建议不再适⽤。 我们不需要在类 Widget ⾥声明析构函数，也不⽤⽤⼾定义析构函数，编译器 将会愉快地⽣成移动操作，并且将会如我们所期望般⼯作。代码如下:

```
//在Widget.h中
class Widget{
public:
Widget();
... //没有对移动操作和析构函数的声明
private:
struct Impl;
std::shared_ptr<Impl> pImpl; //使⽤std::shared_ptr而⾮std::unique_ptr
}
```

而类 Widget 的使⽤者，使⽤ #include widget.h ，可以使⽤如下代码

```
Widget w1;
auto w2(std::move(w1)); //移动构造w2
w1 = std::move(w2); //移动赋值w1
```