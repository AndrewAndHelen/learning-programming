# singleton-pattern

```
单例模式，即一个类只有一个实例对象。一般的实现方法是将构造函数、拷贝构造函数与赋值操作函数声明为private，该类提供一个静态public方法，通过该方法获取该类唯一一个实例对象。
分类：
饿汉式：即类产生的时候就创建好实例对象，这是一种空间换时间的方式
懒汉式：即在需要的时候，才创建对象，这是一种时间换空间的方式
```

- 1.1 饿汉式实现方法一

```
class CSingleton
{
public:
	static CSingleton* getInstance()
	{
		return &myInstatnce;
	}

private:
	CSingleton() { std::cout << "CSingleton construct\n"; };
	~CSingleton() { std::cout << "CSingleton destruct\n"; };
	CSingleton(const CSingleton& other);
	CSingleton& operator=(const CSingleton&);

	static CSingleton myInstatnce;
};

CSingleton CSingleton::myInstatnce;

int main()
{
	CSingleton *ct1 = CSingleton::getInstance();
	CSingleton *ct2 = CSingleton::getInstance();
	CSingleton *ct3 = CSingleton::getInstance();
	return 0;
}

输出：
CSingleton construct
CSingleton destruct
```

- 1.2 饿汉式实现方法二

```
class CSingleton1
{
public:
	static std::shared_ptr<CSingleton1> getInstance()
	{
		return myInstatnce;
	}

private:
	CSingleton1() { std::cout << "CSingleton1 construct\n"; };
	~CSingleton1() { std::cout << "CSingleton1 destruct\n"; };
	CSingleton1(const CSingleton1& other);
	CSingleton1& operator=(const CSingleton1&);

	static std::shared_ptr<CSingleton1> myInstatnce;

	static void destory(CSingleton1*){ std::cout << "this CSingleton1 destruct\n"; }
};

std::shared_ptr<CSingleton1> CSingleton1::myInstatnce(new CSingleton1(), CSingleton1::destory);

int main()
{
	std::shared_ptr<CSingleton1>  ct1 = CSingleton1::getInstance();
	std::shared_ptr<CSingleton1>  ct2 = CSingleton1::getInstance();
	std::shared_ptr<CSingleton1>  ct3 = CSingleton1::getInstance();
	return 0;
}

/*
删除器声明时（即static void Destory(CSingleton *)），需要传入该对象的指针，否则编译出错。目的是为了让shared_ptr能够正确析构该指针
*/

输出：
CSingleton1 construct
this CSingleton1 destruct
```

- 1.3 总结

```
饿汉式的两种实现方法均为线程安全
```

- 2.1 懒汉式实现方法一

```
class CSingleton2
{
private:
	CSingleton2() { std::cout << "CSingleton2 construct\n" ; };
	CSingleton2(const CSingleton2 &);
	CSingleton2& operator=(const CSingleton2 &);
	~CSingleton2() { std::cout << "CSingleton2 destruct\n"; };

	
public:
	static CSingleton2 * getInstance()
	{
		static CSingleton2 myInstance;
		return &myInstance;
	}
	void print()
	{
		std::cout << "我的实例内存地址是:" << this << std::endl;
	}
};

void printHello()
{
	std::cout << "Hi, 我是线程 ID:[" << std::this_thread::get_id() << "]" << std::endl;
	CSingleton2::getInstance()->print();
}

int main()
{
	int max_num = 4;
	std::vector<std::thread> thread_pool;

	for (int i = 0; i < max_num; ++i)
		thread_pool.emplace_back(printHello);

	for (auto& item : thread_pool)
		item.join();

	return 0;
}

输出：
Hi, 我是线程 ID:[15840]
Hi, 我是线程 ID:[29608]
CSingleton2 construct
我的实例内存地址是:Hi, 我是线程 ID:[30152]
我的实例内存地址是:00007FF706BA74C0
00007FF706BA74C0
Hi, 我是线程 ID:[5016]
我的实例内存地址是:00007FF706BA74C0
我的实例内存地址是:00007FF706BA74C0
CSingleton2 destruct
```

- 懒汉式实现方法二

```
class CSingleton3
{
public:
	static CSingleton3* getInstance()
	{
		if (nullptr == myInstatnce)
		{
			std::unique_lock<std::mutex> lock(myMutex);
			if (nullptr == myInstatnce)
				myInstatnce = new(std::nothrow)CSingleton3;
		}
		return myInstatnce;
	}

	static void deleteInstance()
	{
		std::unique_lock<std::mutex> lock(myMutex);
		if (myInstatnce)
		{
			delete myInstatnce;
			myInstatnce = nullptr;
		}
	}

	void print()
	{
		std::cout << "我的实例内存地址是:" << this << std::endl;
	}
private:
	class Deleter {
	public:
		Deleter() {};
		~Deleter()
		{
			if (nullptr != myInstatnce)
			{
				delete myInstatnce;
				myInstatnce = nullptr;
			}
		}
	};

	static Deleter myDeleter;
private:
	CSingleton3() { std::cout << "CSingleton3 construct\n"; };
	~CSingleton3() { std::cout << "CSingleton3 destruct\n"; };
	CSingleton3(const CSingleton3& other);
	CSingleton3& operator=(const CSingleton3&);

	static CSingleton3* myInstatnce;
	static std::mutex myMutex;
};

CSingleton3* CSingleton3::myInstatnce = nullptr;
std::mutex CSingleton3::myMutex;
CSingleton3::Deleter CSingleton3::myDeleter;

void printHello()
{
	std::cout << "Hi, 我是线程 ID:[" << std::this_thread::get_id() << "]" << std::endl;
	CSingleton3::getInstance()->print();
}

int main()
{
	int max_num = 4;
	std::vector<std::thread> thread_pool;

	for (int i = 0; i < max_num; ++i)
		thread_pool.emplace_back(printHello);

	for (auto& item : thread_pool)
		item.join();

	return 0;
}

输出：
Hi, 我是线程 ID:[5312]
Hi, 我是线程 ID:[27784]
Hi, 我是线程 ID:[30284]
Hi, 我是线程 ID:[29584]
CSingleton3 construct
我的实例内存地址是:我的实例内存地址是:000001F4EC328FF0
000001F4EC328FF0
我的实例内存地址是:000001F4EC328FF0
我的实例内存地址是:000001F4EC328FF0
CSingleton3 destruct
```

- 2.3 总结

```
懒汉式实现方法一是线程安全的
懒汉式实现方法二需要加锁才能保证线程安全，原因在于不加锁多个线程会多次申请堆内存，而析构时只能释放最后一次申请的内存空间，造成内存泄漏。
```

  

