## Item38:关注不同线程句柄析构行为
转载自：https://blog.csdn.net/zhangyifei216/article/details/70991600
* 引言
```
1. future 的正常析构⾏为就是销毁 future 本⾝的成员数据
2. 最后⼀个引⽤ std::async 创建共享状态的 future 析构函数会在任务结束前block
```
在Item37中解释过一个状态为joinable的std::thread对象其底层对应一个系统线程。std::future对象和std::thread比较类似，一个future对象也对应着一个系统线程。尽管如此这两者在析构的时候其行为也是不一样的。在Item37中已经提到过一个joinable状态的std::thread对象在析构的时候会导致程序terminate，这是因为无论是调用join还是detach都是一个糟糕的行为，这个在Item37中已经进行了解释。而本文所说的std::future对象，它的析构有的时候类似于隐式调用join，有的时候则是类似于隐式调用detach，甚至两者都不是，但是std::future不会导致程序terminate。

既然谈到了std::future，那就来进一步看一下std::future到底是什么东西，这个对象对于在异步编程中有很大的作用，可以异步获取线程的执行结果，相当于是一个channel，用来链接调用者和被调用者，被调用者通常会异步的将结果写入到这个channel中(通常是std::promise)，调用者通过std::future对象来获取写入的结果，整个过程如下图：
![](./image1.png)
到上面这个图，你或许会奇怪，这个结果是保存在哪呢？首先肯定是不能放在std::promise对象的，因为结果可能是在调用future对象获取时早已保存起来了，那么被调用者运行结束会将std::promise析构，如果结果保存在这个对象中那就会导致结果丢失了。那么能否放在std::future对象里面呢? 同样也不行，因为std::future可以用来创建std::shared_future，那么就需要将结果保存在每一个future对象中，会导致这个结果被多次拷贝和复制，或许可以使用引用计数的方式来记录当前有多少个future对象关联到这个结果中，这也不失为一种方法。但是无论是何种方式，都会造成一定的开销，算不上是一个不错的方式。因此C++11将结果保存在一个特殊的位置，这个位置被称为共享状态。可以简单的使用下面这张图来表示它们之间的关系：
![](./image2.png)
​这个共享状态很重要，因为std::future对象的析构行为和这个状态息息相关，这也是本文要讨论的主题。
1. 通过std::async发起的non-deferred策略的task，其返回的std::future，并且是最后一个引用共享状态的，其析构会一直阻塞，直到这个task运行结束，相当于隐式的做了join。
2. 其他形式的std::future对象的析构就是简单的析构，对于异步运行的task来说，这类似于隐式调用detach，对于deferred策略运行的task来说，相当于这个task将不会运行。

让我们把std::future析构会隐式进行join这个行为进一步细化一下，只要满足下列三个条件的情况，std::future的析构才会是隐式的做join。

1. 这个std::future是通过std::async创建的，并且引用了一个共享状态
2. std::async运行任务的时候指定的运行策略是std::launch::async
3. 这个future对象是最后一个引用共享状态的对象

std::future的这种特殊的析构行为让我们的程序行为变得不可预测，特别是我们没有办法知道哪个 future会隐式的join，哪些又是detach，从std::future对象本身来看是无法看出来的。尽管如此，但是我们知道，凡是从std::async创建的std::future都有可能是隐式的join，而其他方式创建的std::future对象则不是，比如通过std::packaged_task创建的std::future，其析构就不会隐式的进行join。