## Effective-modern-c++

A summary for Effective-modern-c++

### catalog

1.类型推导
* [Item1 :Understand template type deduction](./include/item1/ReadMe.md)
* [Item2:Understand auto type deduction](./include/item2/ReadMe.md)
* [Item 3: Understand decltype](./include/item3/ReadMe.md)
* [Item 4:Know how to view deduced types](./include/item4/ReadMe.md)

2.auto
* [Item 5:Prefer auto to explicit type declarations](./include/item5/ReadMe.md)
* [Item 6:Use the explicitly typed initializer idiom when auto deduces undesired types](./include/item6/ReadMe.md)

3.移步现代C++
* [Item 7:Distinguish between () and {} when creatingobjects](./include/item7/ReadMe.md)
* [Item 8: Prefer nullptr to 0 and NULL](./include/item8/ReadMe.md)
* [Item 9:Prefer alias declarations to typedefs](./include/item9/ReadMe.md)
* [Item 10:优先考虑限域枚举而⾮未限域枚举](./include/item10/ReadMe.md)
* [Item 11:优先考虑使⽤deleted函数而⾮使⽤未定义的私有声明](./include/item11/ReadMe.md)
* [Item 12:使⽤override声明重载函数](./include/item12/ReadMe.md)
* [Item 13:优先考虑const_iterator而⾮iterator](./include/item13/ReadMe.md)
* [Item 14:如果函数不抛出异常请使⽤noexcept条款](./include/item14/ReadMe.md)
* [Item 15:尽可能的使⽤constexpr](./include/item15/ReadMe.md)
* [Item16:让const成员函数线程安全](./include/item16/ReadMe.md)
* [Item17:理解特殊成员函数的⽣成](./include/item17/ReadMe.md)

4.智能指针

* [Item18:对于独占资源使⽤std::unique_ptr](./include/item18/ReadMe.md)
* [Item19:对于共享资源使⽤std::shared_ptr](./include/item19/ReadMe.md)
* [Item20: 当std::shard_ptr可能悬空时使⽤std::weak_ptr](./include/item20/ReadMe.md)
* [Item21:优先考虑使⽤std::make_unique和std::make_shared而⾮new](./include/item21/ReadMe.md)
* [Item22:当使⽤Pimpl惯⽤法，请在实现⽂件中定义特殊成员函数](./include/item22/ReadMe.md)

5.右值引用，移动语义，完美转发

* [Item23: 理解std::move和std::forward]
* [Item24: 区分通⽤引⽤与右值引⽤]
* [Item25: 对右值引⽤使⽤std::move，对通⽤引⽤使⽤std::forward]
* [Item26: 避免在通⽤引⽤上重载]
* [Item27:熟悉通⽤引⽤重载的替代⽅法]
* [Item28:理解引⽤折叠]
* [Item29:认识移动操作的缺点]
* [Item30:熟悉完美转发的失败情况]

6.Lambda表达式
* [Item31:避免使⽤默认捕获模式]
* [Item32:使⽤初始化捕获来移动对象到闭包中]
* [Item33:对于std::forward的auto&&形参使⽤decltype]
* [Item34:考虑lambda表达式而⾮std::bind]

7.并发API
* [Item35:优先考虑基于任务的编程而⾮基于线程的编程]
* [Item36:如果有异步的必要请指定std::launch::threads]
* [Item37:从各个⽅⾯使得std::threads unjoinable]
* [Item38:关注不同线程句柄析构⾏为]
* [Item39:考虑对于单次事件通信使⽤void]
* [Item40:对于并发使⽤std::atomic，volatile⽤于特殊内存区]

8.微调

* [Item41:对于那些可移动总是被拷⻉的形参使⽤传值⽅式]
* [Item42:考虑就地创建而⾮插⼊]

