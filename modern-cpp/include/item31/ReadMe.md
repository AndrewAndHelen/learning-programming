## Item31:避免使用默认捕获模式
转载自：https://blog.csdn.net/zhangyifei216/article/details/55000384
* 引言
```
1. 默认的按引⽤捕获可能会导致悬空引⽤；
2. 默认的按值引⽤对于悬空指针很敏感（尤其是this指针），并且它会误导⼈产⽣lambda是独⽴的想法
```
C++11中Lambda表达式给C++带来了很大的改变，通过Lambda可以很方便的创建一个函数对象，这对于C++的开发来说影响是巨大的，比如可以很方便的结合STL的一些算法，不用再单独创建函数来用，还有可以很好的结合std::unique_ptr和std::shared_ptr，帮助其创建定制删除器等。从字面意义上来看难免让人疑惑Lambda到底是什么，下面则是对Lambda的一个浅显易懂的解释。
* 一个Lambda表达式仅仅是一个表达式，它是源代码的一部分

```
std::find_if(container.begin(), container.end(), [](int val) { return 0 < val && val < 10; });
```
* 闭包是一个由Lambda表达式创建的运行时对象，根据Lambda的捕获模式的不同，闭包会拷贝或者引用捕获的数据， 在上面的代码中闭包作为一个运行时对象传递给了std::find_if。
* 闭包类是一个类，闭包则是它实例化的结果，每一个lambda都会导致编译器生成唯一的闭包类，然后将执行语句放到闭包类的成员函数中。

一个Lambda通常用于创建一个闭包，然后作为函数的参数进行传递，例如上文中的std::find_if，但有的时候会进行拷贝，也就是说对一个Lambda阐述的闭包类型存在多个闭包实例，例如下面这段代码。
```
int x;
auto c1 = [x](int y) { return x * y > 55; }
auto c2 = c1;
auto c3 = c2
```
上面代码中的 c1，c2，c3都是同一个lambda表达式产生的闭包类型的实例。

​总结一下，Lambda，闭包类，闭包，这是三个不同的概念，前两者之存在于编译期，后者是一个运行时的概念，是闭包类的实例化的结果。

说完了Lambda本身，接下来就要进入本文的重点，Lambda捕获，在C++11中有两种捕获模式一种是引用，另外一种则是值，默认的引用传递可能会导致引用悬挂的问题，因为引用的变量其生命周期和闭包本身的生命周期不一致，如果引用的变量其生命周期短于闭包的生命周期，那么就会导致引用悬挂的问题。例如下面这个例子:
```
using FilterContainer = std::vector<std::function<bool(int)>>;
FilterContainer filters;

void addDivisorFilter() {
    .......
    auto divisor = computeDivisor();
    filters.emplace_back(
        [&divisor](int value) { return value % divisor == 0; };
    )
}
```
总结来说这种引用捕获的模式，需要时刻注意引用的变量其生命周期应该要长于闭包的生命周期，只有在这种场景下才比较适合使用引用捕获的这种模式。

对于闭包生命周期比较长的场景可以使用值拷贝这种模式，如下:
```
filters.emplace.back( 
 [=](int value) { return value % divisor == 0; }
)
```

但是这种方式也存在一些问题，比如对于指针的拷贝，尽管可以将指针拷贝到闭包中，但是阻止不了外部指针被delete导致悬挂指针的问题，例如下面这个例子:
```
class Widget {
  public:
    void addFilter() const;
  private:
    int divisor;
}

void Widget::addFilter() const {
    filters.emplace_back(
        [=](int value) { return value % divisor == 0; }
    );
}
```

上面的代码通过默认的值拷贝方式将divisor拷贝到闭包中，看起来上面的闭包很安全。实际上这种说法是错误的。首先来看下面这段代码:
```
void Widget::addFilter() const {
    filters.emplace_back(
        [divisor](int value) { return value % divisor == 0; }
    );
}
```
上面是显示的将divisor值拷贝到闭包中，但是编译出错了，因为对于lambda来说只能捕获在可见作用域内的非静态的局部变量，而divisor是一个成员变量。也就是说值拷贝divisor的这种方式行不通，但是上面的默认值拷贝方式却可以编译通过，这都是编译器帮我做了一些事让我们误认为是divisor值拷贝进去的，下面是还原后的代码:
```
filters.emplace_back(
    [=](int value) { return value % this->divisor == 0; }
);
```

真相就是this指针了，其实拷贝是this指针，对divisor的引用是通过this指针进行的。既然知道了真相，那么这种方式安全吗？ 坦白说同样不安全，因为this指针会失效，存在悬挂指针的风险，例如下面的这段代码:
```
using FilterContainer = std::vector<std::function<bool(int)>>;
FilterContainer filters;

void doSomeWork() {
  auto pw = std::make_unique<Widget>();
  pw->addFilter();
}
```
上面的代码中pw在执行完addFilter就析构了，这导致闭包中拷贝的this指针失效了。这个问题可以通过下面这种方法来解决。
```
void Widget::addFilter() const {
    auto divisorCopy = divisor;
    filters.emplace_back(
        [divisorCopy](int value) { return value % divisorCopy == 0; }
    );
}
```
这不失为一种不错的方法，更幸运的是在C++14中，将上面这种方式直接内置支持了，上面的代码在C++14中可以像下面这样来写。
```
void Widget::addFilter() const {
    filters.emplace_back(
        [divisor = divisor](int value) { return value % divisor == 0; }
    );
}
```
值拷贝也好，引用也好，这些都只是针对非静态的局部变量，对于`静态`的，或者是`全局变量`，那么lambda的捕获将不会起任何作用，和值拷贝不一样的是，lambda不会将静态的或者是全局变量包含到闭包类中，因为这些变量在外部改变后，会影响lambda的行为，而值拷贝不一样，它拷贝的是某一时刻变量的值，此后这个值就被包含到lambda中并且不会因为外部变量的改变而改变。例如下面这个例子:
```
static int static_test = 100;
auto pw = [=](){
    std::cout << static_test << std::endl;
};
++static_test;
pw();
```
上面的代码使用了默认的值拷贝方式，这让人产生了错觉，觉得`static_test`是通过值拷贝的方式传入的，其实并不是，通过上文的介绍，我们知道对于`静态变量是不会捕获的`。因此`static_test`是直接引用外部的，所以起行为收到外部`static_test`的影响。pw()的运行结果是101



