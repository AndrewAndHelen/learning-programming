## Item 20: 当std::shard_ptr可能悬空时使用std::weak_ptr

* 引言
```
1.像std::shared_ptr使⽤std::weak_ptr可能会悬空。
2.std::weak_ptr的潜在使⽤场景包括：caching、observer lists、打破std::shared_ptr指向循
环。
```
* 20.1 std::weak_ptr类摘要
```
template<class T>
class weak_ptr
{
public:
	weak_ptr();
	template<class Y> weak_ptr(shared_ptr<Y> const& r);
	weak_ptr(weak_ptr const& r);
	~weak_ptr();
	weak_ptr & operator=(weak_ptr const& r);
	
	long use_count() const;
	bool expired() const;						//是否为失效指针
	bool empty() const;
	shared_ptr<T> lock() const;					//获取shared_ptr
	
	void reset();								//重置指针
	void swap(weak_ptr<T>& b);					//交换指针
};
```

* 20.2 用法
```
1.基本用法
std::shared_ptr<int> sp(new int(10));
assert(sp.use_count()==1);

std::weak_ptr<ubr> wp(sp);
assert(wp.use_count()==1);

if(!wp.empty())
{
	std::shared_ptr<int> sp2 = wp.lock();
	*sp2 = 100;
	assert(wp.use_count()==2);
}

assert(wp.use_count()==1);
sp.reset();

assert(wp.expired());
assert(!wp.lock());				//weak_ptr将获得一个空指针
```

* 20.3 打破循环引用
```
一个链表结点
class node
{
public:
	~node()
	{std::cout<<"deleted"<<std::endl;}
	typedef std::shared_ptr<node> ptr_type;
	ptr_type next;
};

int main()
{	
	auto p1 = std::make_shared<node>();
	auto p2 = std::make_shared<node>();
	
	p1->next = p2;
	p2->next = p1;
	assert(p1.use_count()==2);
	assert(p2.use_count()==2);//退出作用域无法正确析构
	return 0;
}

正确做法：
class node
{
public:
	~node()
	{std::cout<<"deleted"<<std::endl;}
	typedef std::weak_ptr<node> ptr_type;
	ptr_type next;
};

int main()
{	
	auto p1 = std::make_shared<node>();
	auto p2 = std::make_shared<node>();
	
	p1->next = p2;
	p2->next = p1;
	assert(p1.use_count()==1);
	assert(p2.use_count()==1);//可以正确析构
	return 0;
}
```