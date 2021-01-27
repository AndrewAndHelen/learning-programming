## Item37:从各个方面使得std::threads unjoinable
转载自：https://blog.csdn.net/zhangyifei216/article/details/70187289
* 引言
```
1. 在所有路径上保证 thread 最终是unjoinable
2. 析构时 join 会导致难以调试的性能异常问题
3. 析构时 detach 会导致难以调试的未定义⾏为
4. 声明类数据成员时，最后声明 std::thread 类型成员
```
每一个std::thread对象都有两种状态，一个是joinable状态，另外一个则是unjoinable状态。状态是joinable状态的std::thread对象其底层对应的是可以运行的线程。这个线程可以是阻塞的或者是等待被调度运行的，也可以是已经运行的。而一个unjoinable状态的std::thread对象可以是如下几种情况：
* 默认构造的std::thread对象，因为没有指定要运行的函数，所以没有对应要运行的线程。
* std::thread对象已经被move了，也就是其底层对应的线程已经和其他std::thread对象绑定了。
* std::thread对象已经被joined了，已经joined的std::thread其对应的线程已经运行结束了。
* std::thread对象已经被detached了，一个detached的std::thread对象与其对应的线程已经没有任何关系了。

std::thread的joinable和unjoinable的状态很重要，其中一个最重要的原因是当一个状态是`joinable的std::thread对象其析构函数被调用的时候会导致执行的程序terminated`。例如我们有一个过滤功能的函数doWork，这个函数接收一个过滤函数，和一个最大值MaxVal，然后检查在0到MaxVal范围内附合过滤条件的数值。对于这样的一个功能来说我更倾向于使用基于任务的方式(参见Item35)来实现，但是因为需要设置任务的优先级需要使用原生的handle，所以只能使用std::thread的API来运行，基于任务的API不提供获取原生handle的方法，使用基于线程的方式其实现如下：
```
constexpr auto tenmillion = 10000000;
bool doWork(std::function<bool(int)> filter, int maxVal = tenmillion) {
  std::vector<int> goodVals;
  std::thread t([&filter, maxVal, &goodVals] {
    for(auto i = 0; i <= maxVal; ++i) {
      if (filter(i)) goodVals.push_back(i);
    }
  });
  auto nh = t.native_handle();
  //...... 设置任务的优先级
  if (conditioanAreSatisfied()) {
    t.join();
    performComputation(goodVals);
    return true;
  }
  return false;
}
```
在解释上述代码之前，为了让tenmillion的值更易读可以借助于C++14提供的撇号来对其数值进行分割使其更易读，代码如下:
```
constexpr auto tenMillion = 10'000'000;
```
上面的代码在线程运行后，通过conditioanAreSatisfied来判断条件是否满足，如果满足就开始等待线程结束，然后对过滤后的结果进行计算，最后返回true，如果不满足就直接返回false，但是std::thread的状态是joinable，doWork执行结束后会调用std::thread对象的析构函数，这就会导致程序terminated。

你可能会好奇为什么std::thread在析构的时候其行为会是这样的，因为如果是其它的行为的话很明显会更糟糕，其他的行为如下:
* 隐式的join，析构函数被调用，然后隐式的去调用join方法，等待线程完全执行结束，听起来似乎很合理，但是这会导致性能异常，因为根据conditioanAreSatisfied的结果可知是不符合条件的，理所应当不应该继续等待线程运行结束。
* 隐式的detach，这种行为下虽然在不满足条件的情况下doWork可以快速的返回，但是这会导致潜在的bug，还有可能导致程序crash掉，因为doWork执行结束了，其内部的栈变量goodVals其内存会被释放，但是线程仍然在运行，其内部会访问内存被释放掉的goodVals，这就会导致程序crash掉。

由于一个joinable状态的std::thread对象所造成的后果十分可怕，因此标准委员会实际上禁止这种行为的发生(通过终止程序来禁止这种行为)，因此这要求在使用std::thread对象的时候应该在所有的代码执行路径中确保std::thread的状态都是unjoinable的，但是覆盖所有的执行路径并非易事，它包括return、continue、break、goto或异常等能够跳出作用域的情况，这将导致非常多的路径。无论你什么时候想在所有出作用域的地方做某件事，最常见的方法就是将这件事放入到这个局部对象的析构函数中。这些对象被称为RAII对象，他们的对应的类被称为RAII类。RAII表示”资源获取即初始化”，尽管这种技术的关键是析构而非初始化。std::thread并不是一个RAII类，这是因为标准委员会拒绝隐式的join和detach，因为他们也不知道具体该怎么做。

幸运的是实现这样的一个RAII的std::thread类并不困难，下面这段代码就实现了一个RAII的std::thread，并且可以指定其析构的行为。
```
class ThreadRAII {
  public:
     enum class DtorAction { join, detach };
     ThreadRAII(std::thread&& t, DtorAction a) : action(a), t(std::move(t)){}
     ~ThreadRAII() {
       // 可能存在race condition，t.joinable和具体的join/detach之间存在空隙,std::thread可能会被设置成
       // unjoinable
       if (t.joinable()) {
         if (action == DtorAction::join) {
           t.join();
         } else {
           t.detach();
         }
       }
     }
     std::thread& get() { return t;}
 private:
    DtorAction action;
    std::thread t;
}
```
虽然上面的代码几本上很清楚了，但是以下几点解释还是很有必要的：
* 构造函数只接受std::thread的右值引用，因为我们需要将传入的std::thread对象move到ThreadRAII对象中。
* ThreadRAII提供了一个get成员函数来获取它对应的std::thread对象，这模仿了智能指针的get方法。提供get方法可以避免ThreadRAII实现所有的std::thread的接口。
* ThreadRAII析构函数触发前先检查了std::thread是否是joinable，这是必要的，因为在unjoinable的std::thread对象上调用join或detach会导致未定义的行为。

通过将ThreadRAII类放入到doWork中来解决其不满足条件时导致的程序terminated，代码如下:
```
bool doWork(std::function<bool(int)> filter, int maxVal = tenmillion) {
  std::vector<int> goodVals;
  ThreadRII t(std::thread([&filter, maxVal, &goodVals] {
    for(auto i = 0; i <= maxVal; ++i) {
      if (filter(i)) goodVals.push_back(i);
    }
  }), ThreadRAII::DtorAction::join);

  auto nh = t.native_handle();
  //...... 设置任务的优先级
  if (conditioanAreSatisfied()) {
    t.join();
    performComputation(goodVals);
    return true;
  }
  return false;
}
```

在上面的代码中，我们选择了在ThreadRAII中调用join函数等待线程执行结束，因为上文提到使用detach可能会导致程序crash，难以调试。上文中也提到了join会影响到程序的效率。 但是在两者之间做选择，性能下降貌似是最好的选择了。C++11和C++14没有实现`线程中断`的机制，尽管这是可以自己手动实现的。但是这不在本文的讨论范围之内。

根据Item 17的解释，因为ThreadRAII声明了析构函数，那么编译器就不再生成默认的移动构造和移动赋值操作符，而ThreadRAII不应该是一个不可移动的类，为此通过显示的声明移动构造和移动赋值操作符使得ThreadRAII具备移动语义，代码如下：
```
class ThreadRAII {
  public:
     enum class DtorAction { join, detach };
     ThreadRAII(std::thread&& t, DtorAction a) : action(a), t(std::move(t)){}
     ~ThreadRAII() {
       .....
     }
     ThreadRAII(ThreadRAII&&) = default;
     ThreadRAII& operator=(ThreadRAII&&) = default;
     std::thread& get() { return t;}
 private:
    DtorAction action;
    std::thread t;
}
```
