## Item 6:Use the explicitly typed initializer idiom when auto deduces undesired types.
条款六:auto推导若⾮⼰愿，使⽤显式类型初始化惯⽤法

* 引言
```
不可⻅的代理类可能会使auto从表达式中推导出“错误的”类型
显式类型初始器惯⽤法强制auto推导出你想要的结果
```

* 6.1 使用显式类型初始化
```
std::vector<bool> features(const Widget& w);
case1：
bool highPriority = features(w)[5];
...
processWidget(w,highPriority);//没问题
case2：
auto highPriority = features(w)[5];
...
processWidget(w,highPriority); //未定义⾏为！

解释：
std::vector<bool>的operator[]不会返回容器中元素的引⽤，取而代之它返回⼀个
std::vector<bool>::reference的代理类对象

正确使用：
auto highPriority = static_cast<bool>(features(w)[5]);
```