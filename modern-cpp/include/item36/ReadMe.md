## Item36:如果有异步的必要请指定std::launch::async
* 引言
```
1. std::async 的默认启动策略是异步或者同步的
2. 灵活性导致访问thread_locals的不确定性，隐含了task可能不会被执⾏的意思，会影响程序基于wait的超时逻辑
3. 只有确实异步时才指定 std::launch::async
```

当你调用std::async执行一个函数或者是可调用对象的时候，你通常会试图让这个函数异步的执行，但是std::async并不是必须要这样做。只有通过给std::async指定运行的策略为异步的情况下函数才能确保一定是异步执行的。std::async有个标准的策略，每一个策略通过一个枚举值来表示，这些枚举值的作用域在std::launch中，如下所示:
* async策略意味这函数必须是在其他的线程上异步运行
* deferred策略意味着函数仅仅对std::async返回的std::future调用get或者wait的时候才会同步在当前线程执行，然后等待执行结束后返回执行后的结果。
std::async的默认策略是上面两个的组合，效果如下:
```
auto fut1 = std::async(f);
// 默认策略等同于下面这种调用
auto fut2 = std::async(std::launch::async | 
                     std::launch::deferred,
                     f);
```
通过这种组合的方式使得std::async可以灵活的对线程进行管理在Item35提到过std::async会处理线程过载和线程耗尽的问题。这种默认策略就可以很好的处理线程耗尽的问题，当线程耗尽的时候就会在当前线程中同步执行。 但是使用这种默认策略也会带来一些问题：
* 无法预测函数f是否是并发运行
* 当函数f运行然后调用get或wait获取结果的时候，无法预测f是否是运行在其他线程中
* 无法保障函数f是否已经运行，因为没办法保障一定会调用get或wait获取结果，如果函数f在当前线程中执行，那么就必须要主动调用get或wait才会同步执行。

默认的策略固然很灵活，但是这仍然存在一些问题，当运行的函数要访问线程本地存储的时候，没有办法知道访问的是哪个线程的存储。有可能是当前线程的，也有可能是新创建的线程的。默认策略还会影响到wait_for的调用，可能或造成潜在的bug，wait_for的一般用法如下:
```
using namespace std::literals;
void f() {
  std::this_thread::sleep_for(1s);
}
auto fut = std::async(f);
while (fut.wait_for(100ms) != std::future_status::ready) {
    ...
}
```
面的代码中通过wait_for循环的等待线程运行结束，但是这存在一个问题，当这个线程是同步在当前线程执行的时候，永远都不会返回std::future_status::ready，而是会返回std::future_status::deferred。所以上面的代码在线程耗尽时会导致潜在的bug出现。修复这个问题也是很容易的，通过设置wait_for的超时时间为0来检测std::async是异步执行还是同步执行。
```
auto fut = std::async(f);
if (fut.wait_for(0s) == std::future_status::deferred) { // 检测是否是异步执行
} else { 
while (fut.wait_for(100ms) !=  std::future_status::ready) { // 是异步执行，开始循环等待
//......
}
```

综上所述，std::async的默认策略固然很灵活，但是仍然存在一些潜在的问题，需要根据不同的使用场景来看，如果你的使用场景不属于下面几种几种场景的话，请酌情考虑是否要替换默认的策略。
* 当运行get或wait的时候线程不需要并发执行
* 不在意是对哪个线程的线程局部存储的读和写
* 如果代码中并没有对**std::asyn**c返回的future调用get或wait，这种情况是可以接受任务不被执行
* 代码中有对wait_for或wait_until的调用时需要考虑会出现deferred的状态

如果不适合上述的场景，那么只好使用async策略，也就是真正去创建一个线程来执行，或者可以使用下面这个封装好的函数来实现真正的异步执行。
```
template<typename F, typename... Ts>
inline
std::future<typename std::result_of<F(Ts...)>::type> 
reallyAsync(F&& f, Ts&&... params) 
{
 return std::async(std::launch::async, // call to f(params...)
                       std::forward<F>(f),
                       std::forward<Ts>(params)...);
}
```
通过使用reallyAsync可以使得代码的语意更加明确，使得用户不用去理解async和defered两种执行策略的含义。
