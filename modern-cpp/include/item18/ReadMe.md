## Item18:对于独占资源使用std::unique_ptr

部分转载于《Boost程序库完全开发指南(第五版)》

* 引言
```
1.std::unique_ptr是轻量级、快速的、只能move的管理专有所有权语义资源的智能指针
2.默认情况，资源销毁通过delete，但是⽀持⾃定义delete函数。有状态的删除器和函数指针会增加
std::unique_ptr的⼤小
3.将std::unique_ptr转化为std::shared_ptr是简单的
```

* 18.1std::unique_ptr类摘要
```
template<class T,class D=default_delete<T>>				//使用删除器
class unique_ptr{
public:
	typedef some_define pointer;						//内部类型定义
	typedef T			element_type;	
	
	constexpr unique_ptr() noexcept;					//构造函数
	explicit unique_ptr(pointer) noexcept;
	
	~unique_ptr();
	unique_ptr& operator=(unique_ptr&& u) noexcept;		//移动赋值函数
	
	element_type& operator*() const;					//操作符重载
	pointer operator->() const noexcept;				//操作符重载
	pointer get() const noexcept;						//获取原始指针
	explicit operator bool() const noexcept;			//bool类型转型
	
	pointer release() noexcept;							//释放指针所有权
	void reset(pointer p) noexcept;						//重置智能指针
	void swap(unique_ptr& u) noexcept;					//交换指针
	
	unique_ptr(const unique_ptr&)			=delete;	//禁用复制构造
	unique_ptr& operator=(const unique_ptr&)=delete;	//禁用赋值拷贝
};
```

* 18.2使用方法

```
1.可以管理单个对象
std::unique_ptr<int> up=std::make_unique<int>(10);
assert(p&&*p==10)

std::unique_ptr<int> low(new int);
*low=10;

low.reset();			//重置指针
assert(!low)			//low不管理任何指针

2.可以管理数组
std::unique_ptr<int[]> ptr1{ new int[5]{1,2,3,4,5} };
auto ptr2 = std::make_unique<std::array<int, 5>>(std::array<int, 5>{1, 2, 3, 4, 5});
```

* 18.3 自定义删除器
```
//类结构
class Investment { 
	...
	virtual ~Investment();
	... 
};
class Sock: public Investment {...};
class Bond: public Investment {...};
class RealEstate: public Investment {...};

template<typename... Ts>
std::unique_ptr<Investment> makeInvestment(Ts&&... params){
	template<typename... Ts>
    makeInvestment(Ts&& params)
    {
    auto delInvmt = [](Investment* pInvestment)		//自定义删除器
    {
    makeLogEntry(pInvestment);
    delete pInvestment;
    };
    std::unique_ptr<Investment, decltype(delInvmt)> pInv(nullptr, delInvmt);	//删除器是unique_ptr模板参数的一部分
    if (/*a Stock object should be created*/)
    {
    pInv.reset(new Stock(std::forward<Ts>(params)...));
    }
    else if ( /* a Bond object should be created */ )
    {
    pInv.reset(new Bond(std::forward<Ts>(params)...));
    }
    else if ( /* a RealEstate object should be created */ )
    {
    pInv.reset(new RealEstate(std::forward<Ts>(params)...));
    }
}

```

* 18.4⽆状态函数对象（⽐如没有捕获的lambda表达式）对unique_ptr⼤小没有影响
```
auto delInvmt = [](Investment* pInvestment)
{
    makeLogEntry(pInvestment);
    delete pInvestment;
};
template<typename... Ts>
std::unique_ptr<Investment, decltype(delInvmt)>
makeInvestment(Ts&& params); //返回Investment*的⼤小

void delInvmt2(Investment* pInvestment)
{
    makeLogEntry(pInvestment);
    delete pInvestment;
}
template<typename... Ts>
std::unique_ptr<Investment, void(*)(Investment*)>
makeInvestment(Ts&&... params); //返回Investment*的指针加⾄少⼀个函数指针的⼤小
```

