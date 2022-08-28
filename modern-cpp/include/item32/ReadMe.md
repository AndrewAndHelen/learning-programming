## Item32:使用初始化捕获来移动对象到闭包中
转载自：https://blog.csdn.net/zhangyifei216/article/details/55001329
* 引言
```
1. 使⽤C ++14的初始化捕获将对象移动到闭包中。
2. 在C ++11中，通过⼿写类或 std::bind 的⽅式来模拟初始化捕获。
```
一些时候Lambda的值拷贝和引用两种捕获模式都没有办法满足你的需求，比如当你有一个可移动的对象，你希望可以将这个对象移动到闭包中，因为值拷贝的效率不高，引用捕获的话会导致生命周期的问题。很可惜C++11并没有提供将对象移动到闭包中的方法，但是C++14可以，C++14对对象移动到闭包中提供了直接支持，即使你不打算升级你的编译器，本文也适合你，因为本文会介绍在C++11中通过另外一种方式间接的对对象移动提供支持。

C++14支持将对象移动到闭包中，你可能会以为这和之前的值拷贝方式(=)，引用方式(&)的使用类似，提供一个&&符号就表明是移动捕获，但是这次C++标准委员会没有这样做，他们使用了一种更灵活的方式来支持移动捕获。他们通过提供一种叫做Init capture的机制，而移动捕获就是通过这个机制来实现，在Item31中提到的C++14捕获类的数据成员的方式就是利用Init capture这种机制。下面来看看Init capture机制的语法。
* 类的成员名称(也就是lambda生成闭包类后，要给闭包类添加的数据成员)
* 一个表达式，用来初始化闭包类的成员

下面是一个使用Init capture进行移动捕获的例子:
```
class Widget {
  public:
   ...
   bool isValdated() const;
   bool isProcessed() const;
   bool isArchived() const;
  private:
   ...
};

auto pw = std::make_unique<Widget>();
// 这里的pw就是闭包类中的数据成员名称，std::move(pw) 则是一个表达式
// 用来初始化闭包类的数据成员pw
auto func = [pw = std::move(pw)] {
  return pw->isValidated()
        && pw->isArchived();
};
```
C++14的这个Init capture的机制是如此强大，在没有语言层面支持的情况下C++11该如何实现C++14的这种Init capture机制呢?
* 将要捕获的对象移动到一个std::bind产生的函数对象中
* 让lambda的参数包含一个对要捕获对象的引用
```
std::vector<double> data;
auto func = std::bind([](const std::vector<double>& data) {
  ....
}, std::move(data));
```
我们都知道std::bind默认会将参数拷贝到它产生的函数对象中保存，但是前提是参数是左值，如果是右值的话则是移动到函数对象中，使用一个左值来保存移动进去的参数。待函数对象被调用的时候将这个左值传递进去，因此这里你可以看到函数对象的形参是一个引用，这就是为什么这里不是右值引用的原因。至于形参为什么是const，这是因为默认的std::bind产生的函数对象，也称为仿函数，就是通过operator()重载来实现普通函数的行为，默认这个重载方法是const的，所以它是不能对对象成员做修改的，所以这里加上了const，如果希望可以对传入的参数进行修改，可以利用mutable关键字，如下:
```
std::vector<double> data;
auto func = std::bind([](std::vector<double>& data) mutable {
  ....
}, std::move(data));
```
