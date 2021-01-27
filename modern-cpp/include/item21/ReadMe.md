## Item21:优先考虑使用std::make_unique和std::make_shared而非new

* 引言
```
1.和直接使⽤new相⽐，make函数消除了代码重复，提⾼了异常安全性。对于std::make_shared和std::allocate_shared ,⽣成的代码更小更快。
2.不适合使⽤make函数的情况包括需要指定⾃定义删除器和希望⽤⼤括号初始化。
3.对于std::shared_ptr, make函数可能不被建议的其他情况包括
	(1)有⾃定义内存管理的类。
	(2)特别关注内存的系统，⾮常⼤的对象，以及std::weak_ptr⽐对应的std::shared_ptr活得更久。
```
* 21.1 应该避免重复代码
```
重复写类型和软件⼯程⾥⾯⼀个关键原则相冲突：应该避免重复代码。源代码中的重复增加了编译的时间，会导致⽬标代码冗余，并且通常会让代码库使⽤更加困难。

auto upw1(std::make_unique<Widget>()); // with make func
std::unique_ptr<Widget> upw2(new Widget); // without make func
auto spw1(std::make_shared<Widget>()); // with make func
std::shared_ptr<Widget> spw2(new Widget); // without make func
```
* 21.2 避免内存泄漏
```
例子：
void processWidget(std::shared_ptr<Widget> spw, int priority);

processWidget(std::shared_ptr<Widget>(new Widget), computePriority()); //存在潜在的内存泄漏

1.processWidget执行前操作：
(1)表达式'new Widget'必须计算，例如,⼀个Widget对象必须在堆上被创建
(2)负责管理new出来指针的std::shared_ptr<Widget>构造函数必须被执⾏
(3)computePriority()必须运⾏

2.某种可能的执行顺序：
(1)执⾏new Widget
(2)执⾏computePriority
(3)运⾏std::shared_ptr构造函数

3.存在问题
如果computePriority()发生异常，堆上申请的Widget内存泄漏

4.解决办法
processWidget(std::make_shared<Widget>(), computePriority());

如果是std::make_shared，在computePriority调⽤前，动态分配Widget的原始指针会安全的保存在作为返回值的std::shared_ptr中。
如果computePriority⽣成⼀个异常，那么std::shared_ptr析构函数将确保管理的Widget被销毁。
如果⾸先调⽤computePriority并产⽣⼀个异常，那么std::make_shared将不会被调⽤，因此也就不需要担⼼new Widget(会泄露)。
```
* 21.3 make函数无法使用自定义删除器
```
auto widgetDeleter = [](Widget*){...};//一个删除器
std::unique_ptr<Widget, decltype(widgetDeleter)> upw(new Widget, widgetDeleter);
std::shared_ptr<Widget> spw(new Widget, widgetDeleter);

make函数无法做到
```

* 21.4 {}初始化对make函数的影响
```
当构造函数重载，有std::initializer_list作为参数和不⽤其作为参数时，
⽤{}创建对象更倾向于使⽤std::initializer_list作为参数的构造函数，
⽤()创建对象倾向于不⽤std::initializer_list作为参数的构造函数
在make函数中，完美转发使⽤圆括号，而不是{}。
如果你想⽤{}初始化指向的对象，你必须直接使⽤new。

解决方法：使⽤auto类型推导从⼤括号初始化创建std::initializer_list对象(⻅Item 2)，然后将auto创建的对象传递给make函数。

// create std::initializer_list
auto initList = { 10, 20 };
// create std::vector using std::initializer_list ctor
auto spv = std::make_shared<std::vector<int>>(initList);
```

* 对于make_unique，只存在21.3、21.4的问题，make_shared还存在以下两个问题

* 21.5 make函数不应创建重载了operator new 和 operator delete类的对象
```
例如，Widget类的operator new和operator delete只会处理sizeof(Widget)⼤小的内存块的分配和释放。

这种常识不太适⽤于std::shared_ptr对定制化分配(通过std::allocate_shared)和释放(通过定制化deleters)

因为std::allocate_shared需要的内存总⼤小不等于动态分配的对象⼤小，还需要再加上控制块⼤小。

特别注意：通过查找资料：https://www.zhihu.com/question/372817976
有大佬说明libstdc++、libc++和MSVC STL三家编译器的make_shared的实现都把动态分配的对象和控制块组合到一个对象类型里，然后都是用全局的 ::operator new 分配内存，目前可以说类特定的 T::operator new 和 make_shared 无关。
```

* 21.6 注意对象生存周期
```
通过std::shared_ptr make函数分配的内存，直到最后⼀个std::shared_ptr和最后⼀个指向它的std::weak_ptr已被销毁，才会释放。

如果对象类型⾮常⼤，而且销毁最后⼀个std::shared_ptr和销毁最后⼀个std::weak_ptr之间的时间很⻓，那么在销毁对象和释放它所占⽤的内存之间可能会出现延迟。

使用make：
class ReallyBigType { … };

auto pBigObj = std::make_shared<ReallyBigType>();// 通过std::make_shared创建⼀个⼤对象

… // 创建 std::shared_ptrs 和 std::weak_ptrs指向这个对象，使⽤它们
… // 最后⼀个 std::shared_ptr 在这销毁,但 std::weak_ptrs 还在
… // 在这个阶段，原来分配给⼤对象的内存还分配着
… // 最后⼀个std::weak_ptr在这⾥销毁;
// 控制块和对象的内存被释放

使用new:
class ReallyBigType { … };
//通过new创建特⼤对象
std::shared_ptr<ReallyBigType> pBigObj(new ReallyBigType);
… // 像之前⼀样，创建 std::shared_ptrs 和 std::weak_ptrs指向这个对象，使⽤它们
… // 最后⼀个 std::shared_ptr 在这销毁,但 std::weak_ptrs 还在
// !!!memory for object is deallocated
… // 在这阶段，只有控制块的内存仍然保持分配
… // 最后⼀个std::weak_ptr在这⾥销毁;
// 控制块内存被释放
```