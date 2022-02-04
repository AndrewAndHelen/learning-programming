## Effective-modern-c++

A summary for Effective-modern-c++

### catalog

1.类型推导
* [Item1:理解模板类型推导](./include/item1/ReadMe.md)
* [Item2:理解auto类型推导](./include/item2/ReadMe.md)
* [Item 3:理解decltype](./include/item3/ReadMe.md)
* [Item 4:学会查看类型推导结果](./include/item4/ReadMe.md)

2.auto

* [Item 5:优先考虑auto而非显式类型转换](./include/item5/ReadMe.md)
* [Item 6:auto推导若非己愿，使用显示类型初始化惯用法](./include/item6/ReadMe.md)

3.移步现代C++
* [Item 7:区分使用()和{}创建对象](./include/item7/ReadMe.md)
* [Item 8: 优先考虑nullptr而非0和NULL](./include/item8/ReadMe.md)
* [Item 9:优先考虑别名声明而非typedef](./include/item9/ReadMe.md)
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

* [Item23: 理解std::move和std::forward](./include/item23/ReadMe.md)
* [Item24: 区分通用引用与右值引用](./include/item24/ReadMe.md)
* [Item25: 对右值引用使用std::move，对通用引用使用std::forward](./include/item25/ReadMe.md)
* [Item26: 避免在通用引用上重载](./include/item26/ReadMe.md)
* [Item27:熟悉通用引用重载的替代方法](./include/item27/ReadMe.md)
* [Item28:理解引用折叠](./include/item28/ReadMe.md)
* [Item29:认识移动操作的缺点](./include/item29/ReadMe.md)
* [Item30:熟悉完美转发的失败情况](./include/item30/ReadMe.md)

6.Lambda表达式
* [Item31:避免使用默认捕获模式](./include/item31/ReadMe.md)
* [Item32:使用初始化捕获来移动对象到闭包中](./include/item32/ReadMe.md)
* [Item33:对于std::forward的auto&&形参使用decltype](./include/item33/ReadMe.md)
* [Item34:考虑lambda表达式而非std::bind](./include/item34/ReadMe.md)

7.并发API
* [Item35:优先考虑基于任务的编程而非基于线程的编程](./include/item35/ReadMe.md)
* [Item36:如果有异步的必要请指定std::launch::async](./include/item36/ReadMe.md)
* [Item37:从各个方面使得std::threads unjoinable](./include/item37/ReadMe.md)
* [Item38:关注不同线程句柄析构行为](./include/item38/ReadMe.md)
* [Item39:考虑对于单次事件通信使用void](./include/item39/ReadMe.md)
* [Item40:对于并发使用std::atomic，volatile用于特殊内存区](./include/item40/ReadMe.md)

8.微调

* [Item41:对于那些可移动总是被拷贝的形参使用传值方式](./include/item41/ReadMe.md)
* [Item42:考虑就地创建而非插入](./include/item42/ReadMe.md)

