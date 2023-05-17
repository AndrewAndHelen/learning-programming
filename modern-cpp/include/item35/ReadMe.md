## Item35:优先考虑基于任务的编程而非基于线程的编程
转载自：https://blog.csdn.net/zhangyifei216/article/details/68062963
* 引言
```
1. std::thread API不能直接访问异步执⾏的结果，如果执⾏函数有异常抛出，代码会终⽌执⾏
2. 基于线程的编程⽅式关于解决资源超限，负载均衡的⽅案移植性不佳
3. 基于任务的编程⽅式 std::async 会默认解决上⾯两条问题
```
C++11第一次将并发引入到了语言和标准库中，这使得C++程序员们终于可以直接写跨平台的多线程程序了，不用再自己去写不同平台的线程封装了。C++11在引入线程相关的API的同时也引入了一些并发编程的高级组件例如tasks、futures、threads、mutexes、condition、variables、atomic objects等等。

在C++11中如果想异步的去运行一个doAsyncWork函数的话，你有两个选择。第一种方式就是创建一个std::thread然后运行这个函数，这种方式被称为基于线程的方法，代码如下:
```
int doAsyncWork(); 
std::thread t(doAsyncWork);
```
第二种方式可以通过std::async来运行doAsyncWork，这种方式我们称之为基于任务的方法。代码如下:
```
auto fut = std::async(doAsyncWork);
```
基于任务的这种方法要比基于线程的方法更优越，首先从代码上来看只需要更少的代码量，其次当doAsyncWork有返回值的时候基于线程的方法没有提供一个好的机制来获取函数的返回值，但是基于任务的方法可以很方便的获取到doAsyncWork的返回值，因为std::async返回的是一个std::future，这个future提供了一个get方法可以用来获取doAsyncWork的返回值。如果doAsyncWork在运行的过程中发生了异常通过get方法也可以获取到异常的信息，但是基于线程的方法在抛出异常的时候会导致程序最终调用std::terminate。

除了上面提到的几点不同为外，基于任务的这种方式相比基于线程的更抽象，可以不用自己去管理线程，在讲解线程管理这些细节之前，我们需要总结一下C++并发中的关于线程的三层含义:
1. 硬件线程 这是实际执行计算的线程，当代计算机架构下每一个CPU核心都提供了一个或多个硬件线程。
2. 软件线程 也被称为OS线程或是系统线程，操作系统将所有进程中的线程都调度到硬件线程中去执行，一般情况下操作系统会创建比硬件线程数量更多的软件线程，因为如果某个软件线程阻塞了(比如等待磁盘IO、mutex、条件变量等)这个时候通过执行其他的未阻塞的软件线程就可以提高整体的吞吐量。
3. std::thread 是C++中的一个对象作为一个句柄对应于底层的操作系统软件线程，其中一些std::thread对象表示空句柄也就是没有软件线程对应，例如，默认创建的std::thread并切没有指定函数执行、已经joined的std::thread对象、已经detached的std::thread对象、已经被移动的对象等。

软件线程是一个受限的资源，如果试图在系统上创建太多的线程超过了系统可以提供的最大线程数量就会引发std::system_error异常。一个好的软件必须要处理这种错误，但是如何去处理呢?一种办法就是在当前线程中执行doAsyncWork。但是这会导致负载不均衡，如果当前线程是一个GUI线程，那么势必会造成响应不及时的问题。另外一个方法则是等待系统中的其他线程执行结束然后再创建一个新的线程运行。但是很有可能这个线程正在等待doAsyncWork的运行结果。

即使线程没有超过系统的资源，仍然需要担心过载的问题，也就是当前的准备运行的软件线程数量超过了硬件线程，一旦发生这样的状况，线程调度器就会按照时间片将软件线程调度到硬件线程上，当一个线程的时间片用完那么另外一个线程就会被调度过来继续运行。而这个过程中会执行一次上下文切换，这个上下文切换增加了线程管理的开销。尤其是当硬件线程上被调度过来一个另外的软件线程时会导致当前硬件线程上的cache失效。避免这个过载的问题是很难的，因为需要确定软件线程和硬件线程的一个合理的比例，而这个比例取决于线程是如何运行的，并且是会动态变化的。例如当一个程序从一个IO密集型的变成了计算密集型。这个比例同时还和上下文件切换的开销以及软件线程如何使用CPU Cache有关。此外这个比例还与硬件架构有关，所以即使你在一个平台上调优完成了，当换到另外一个平台上并不会保证是优化好的。

综上所述，线程的管理真的很不容易，如果想让自己的生活更容易一些，那么可以将线程管理的任务交给别人来做，std::async就是做了这样的一件事。
```
auto fut = std::async(doAsyncWork); // onus of thread mgmt is 
                                 // on implementer of
                                    // the Standard Library
```
只需要这么简单的调用即可，线程管理的任务都交给了C++标准库来管理了。C++标准库负责处理线程过载，负载不均衡等问题。所以总的来说基于任务方法要比基于线程的方法要更易于使用。但是有几种情况基于线程的方法有其独特的优势，如下所示:

1. 需要访问底层的线程实现的API，这个时候通过std::thread可以拿到底层线程的句柄，然后才可以使用底层的线程API。
2. 需要给你的应用程序来优化线程的使用，这需要根据应用的特点来优化线程的使用，比如特定机器架构的服务器软件，这个时候就需要使用特定平台下的线程实现。
3. 你需要实现一些高级的线程组件，例如线程池，这是C++标准库没有提供的高级组件

除了上述几个特殊情况外，我们应该优先使用基于任务的方法来替换替换线程的方法。

