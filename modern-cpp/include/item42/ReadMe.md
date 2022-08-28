## Item42:考虑就地创建而非插入
转载自：https://blog.csdn.net/zhangyifei216/article/details/72866977
* 引言
```
1. 原则上，emplacement函数有时会⽐insertion函数⾼效，并且不会更差
2. 实际上，当执⾏如下操作时，emplacement函数更快
值被构造到容器中，而不是直接赋值 
传⼊的类型与容器类型不⼀致 
容器不拒绝已经存在的重复值
3. emplacement函数可能执⾏insertion函数拒绝的显式构造
```

如果你有一个容器，存放的是std::string类型，当你通过插入函数(如:insert、push_front、push_back或者是std::forward_list的insert_after)传递一个std::string类型的元素到容器中，逻辑上容器中就会有这个元素了。尽管这可能只是逻辑上的，但并不总是真的，考虑下面这段代码：
```
std::vector<std::string> vs;
vs.push_back("xyzzy");
```

上面的代码中，包含了一个存放std::string类型的容器，但是实际上插入的却是一个字符串常量，并不是std::string类型的字符串，也就是意味着插入的并不是容器期望的类型。

std::vector的push_back函数对于左值和右值都进行了重载，代码如下:
```
template<class T,
        class Allocator = allocator<T>>
class vector {
 public:
    void push_back(const T& x);
    void push_back(T&& x);
};
```
当插入一个字符串常量的时候，编译器就会去寻找合适的重载函数，但是push_back并没有对字符串常量的重载，因此编译器会创建一个临时的std::string变量，并将传入的字符串常量对其进行构造，最后将创建的临时变量传入到push_back中，换句话说等同与下面这段代码：
```
vs.push_back(std::string("xyzzy"));
```

上面的代码正常编译和运行，每个人都很高兴，除了那些对性能要求苛刻的人，因为这段代码存在性能问题，并不是很高效。为了插入字符串常量而构造一个std::string的临时变量这是没有问题的，但是实际上并不仅仅是这些，编译器还会对这个临时变量进行拷贝，拷贝完成后，这个临时变量就会析构了。整个过程如下:
1. ⼀个 std::string 的临时对象从字⾯量"xyzzy"被创建。这个对象没有名字，我们可以称为temp，temp通过 std::string 构造器⽣成，因为是临时变量，所以temp是右值。
2. temp被传递给 push_back 的右值x重载函数。在std::vector 的内存中⼀个x的副本被创建。这次构造器是第⼆次调⽤，在 std::vector 内部重新创建⼀个对象。（将x副本复制到 std::vector内部的构造器是移动构造器，因为x传⼊的是右值，有关将右值引⽤强制转换为右值的信息，请参⻅Item25）。
3. 在 push_back 返回之后，temp被销毁，调⽤了⼀次 std::string 的析构器。

从上面分析的过程来看如果传入的字符串常量构造的临时temp对象能在上面的第二步中直接放到容器中，那么就避免了拷贝构造和析构的开销。要想达到这样的效果就不能使用push_back函数，这个函数是存在问题的，应该使用emplace_back。这个函数的行为和我们预期是一样的，无论传递什么样的参数都会直接构造成std::string直接传递到容器中，没有临时对象的参与。
```
template <class... Args>
  void emplace_back (Args&&... args);
```

通过上面的代码可见emplace_back还带有一个可变参数，可以接受任意参数，通过完美转发会将传入的参数放入到容器中直接构造出元素。例如下面这段代码:
```
vs.emplace_back(50, 'x');
```

50和x会被传递给std::string的构造函数，用来在容器中构造一个std::string对象，该对象包含了50个x字符。通过emplace_back的可变参数使得emplace_back可以支持std::string所有的构造函数。

emplace_back对于支持push_back的标准容器来说都是可用的，对于支持push_font的容器C++11提供了emplace_front，对于支持insert函数的标准容器来说(除了std::forward_list和std::array)提供了emplace，对于关联容器则提供了emplace_hint替换其insert函数，对于std::forward_list使用emplace_after代替其insert_after。

emplace系列函数能够替换传统的插入函数，最重要的原因是因为，emplace系列函数拥有更灵活的接口，传统的插入函数是拿到对象进行插入，而emplace系列函数可以拿着构造函数进行对象的构造，然后插入到容器中。两者的不同就是避免了临时对象的构造和析构开销。如果直接插入的就是目标类型，也就没有临时对象的产生，那么emplace系列函数和传统的插入函数本质上就是一致的了。
```
vs.push_back(queenOfDisco);
vs.emplace_back(queenOfDisco);
```

综合来说，传统的插入函数能做的，emplace系列函数都可以做，并且在某些情况下更高效。理论上emplace可以完全替代传统的插入函数，那么为什么我们不总是这样用呢? 理论上这是对的，两者在行为上的表现一致，但是实际上并不是这样的，目前标准库在实现的时候在某些方面和我们的预期还不太一样，虽然大多数场景下emplace系列函数的确是优于传统的插入函数，但是可悲的是，存在某些场景下传统的插入函数要比emplace快。这样的场景又不太好归类，因为这取决于传递的参数类型、使用的容器、插入容器的位置、容器中保存的对象其构造函数的异常安全性、容器是否允许插入重复的值、要插入的元素是否已经在容器中了等等。按照通常的调优建议应该对两者进行压力测试。

这当然不是很令人满意，所以你会很高兴地得知，有一个可以帮助你识别何种方式插入是高效的方法。 如果以下所有情况都是真实的，则emplaces几乎肯定会比传统的插入要高效：

要插入的值是通过构造函数插入到容器中的，不是通过赋值。上文中提到的字符串常量就是这种情况，传入的值需要先构造为一个临时的对象，然后再保存到容器中，此时emplace系列函数就发挥了它的作用，将这个临时对象直接move到容器中，但是如果要插入的位置已经有元素存在的时候，情况又不一样了。考虑下面这段代码:
```
vs.emplace(vs.begin(), "xyzzy");
```
上面的代码中，很少有编译器会将上面的代码实现为构造一个std::string对象然后放到vs[0]位置上，大多数编译器的实现是进行移动赋值，将传入的参数直接移动赋值到目标位置。但是移动赋值需要传入的是一个对象，这就需要构建一个临时对象，然后移动过去，等操作完成后再将这个对象 析构。上文中其实我们已经分析了emplace系列函数的优势在于省去临时对象的构造和析构成本，很不幸在移动赋值的这种场景下这些开销并没有被省去。
1. 传递的参数类型和容器中的类型不同。emplace系列函数的优点就是在于避免临时对象的构造和析构，如果传递的参数类型和容器中的类型相同就不会产生临时对象了，那么很自然emplace系列函数的优势也就无法体现了。
2. 容器不太可能插入重复的值。容器允许有重复的值但是大多数情况下插入的值都是唯一的，这是因为emplace系列函数首先会将传入的值构造为一个新的元素，然后拿着这个新元素的值和容器中已经存在的元素进行比较，如果不在容器中那么就添加进去，否则就丢弃刚创建的这个值，这就意味着多了一次额外的构造和析构成本开销。

下面这些调用满足上述三个条件，因此使用emplace系列函数要比使用push_back快。
```
vs.emplace_back("xyzzy");
vs.emplace_back(50. 'x');
```
当你想往这个容器中添加一个新的std::shared_ptr<Widget>元素，并且给这个元素指定自定义删除器(Item19)，你应该使用在Item21中提到过的std::make_shared来创建一个std::shared_ptr元素，但是在这里行不通，因为需要指定自定义删除器，所以只能使用裸指针来构造std::shared_ptr元素，代码如下:
```
void killWidget(Widget* pWidget);
ptrs.push_back(std::shared_ptr<Widget>(new Widget, killWidget));
// 或者是如下形式
ptrs.push_back({new Widget, killWidget});
```
1. 进行如上的调用，对传入的裸指针进行构造，产生了一个临时的std::shared_ptr<Widget>对象，这里称为temp对象。
2. push_back函数持有对这个temp对象的引用，开始在容器中分配一个节点，用来保存对temp对象的拷贝，在分配内存的时候发生了out-of-memory异常
3. 异常发生后，temp对象析构，其管理的Widget对象也会自动调用自定义删除函数killWidget进行析构。

通过上面的分析可知，即使push_back中发生了异常，也不会造成内存的泄漏，这都是因为std::shared_ptr可以负责管理资源，现在考虑一下如果使用emplace_back，情况又是如何:
```
ptrs.emplace_back(new widget, killWidget);
```
1. 通过new Widget产生的裸指针，经过完美转发后进入push_back函数内部，然后容器开始分配内存用于构造新元素，分配内存的时候发生了out-of-memory异常。
2. emplace_back发生异常后指向Widget的指针变量析构，所指向的内存还没有释放，已经没有办法引用这段内存了，产生了内存泄漏。

通过上面的分析可知，使用emplace_back的情况使得程序有可能会产生内存泄漏，对于std::unique_ptr需要定制删除器的场景来说也是同样存在这个问题的。这也从另一个方面反映出std::make_shared和std::make_unique这类构造资源管理类对象的重要性。

综上所述，对于保存资源管理类的容器(例如: std::list<std::shared_ptr<Widget>>)来说，传统的插入函数其参数就保证了其内存资源的分配和管理是安全的，而emplace系列函数其完美转发延迟了资源管理对象的创建，直到容器的内存分配成功后才会进行构造，这是的在异常发生的情况下会出现内存泄漏的问题。所有的标准容器都存在这类问题，因此当你选择使用emplace系列函数插入一个资源管理类的时候，你需要考虑性能和异常安全性谁更重要。

坦白说，你不应该在代码中出现将new Widget这样的表达式作为参数传递给emplace_back、push_back，或是其他任何函数，因为在Item21中曾经提到过这样做会导致异常安全性的问题。正确的做法应该是将new Widget和构造资源管理类这两个动作放在一个独立的语句中，等资源管理类构造完成后再将其作为右值传递给目标函数，代码如下:
```
std::shared_ptr<Widget> spw(new Widget, killWidget);
ptrs.push_back(std::move(spw));

std::shared_ptr<Widget> spw(new Widget, killWidget);
std::emplace_back(std::move(spw));
```
不管怎么样上面的这个方法都存在spw对象的构造和析构的开销，而我们使用emplace_back 替换push_back的目的就是为了避免临时对象的构造和析构开销，而spw概念上就是这个临时对象。但是当使用emplace_back往容器中插入资源管理对象的时候，必须要使用适当的方式来保证资源的获取和资源管理对象的构建之间不能被干预，这也就是spw对象存在的意义。

第二个因素是和explicit构造函数交互相关。在C++11中开始支持正则表达式，下面这段代码试图创建一组正则表达式对象，并存到容器中。
```
std::vector<std::regex> regexes;
```

有一天你不小心写了下面这段看似意义的代码。
```
regexes.emplace_back(nullptr);
```

你并没有注意到你输入的这段错误的代码，编译器页没有提示你编译问题，最终你花费大量时间来debug这个问题，在debug这个问题的时候，你发现容器中存在nullptr指针，但是正则表达式不可能是nullptr的，例如下面这段代码:
```
std::regex r = nullptr; //编译出错
```

通过上面的代码可知，std::regex没有办法是nullptr空指针的，有趣的是如果通过push_back替换emplace_back的话，编译器同样会报错。
```
regexes.push_back(nullptr);
```

造成这样奇怪的行为的根本原因是std::regex的构造成本太高了，std::regex构造的时候会对传入的正则字符串进行预编译，是一个很耗时的动作，为此std::regex禁止隐式构造，避免不必要的性能损失。所以上面代码中通过赋值操作符进行构造是被禁止的，而push_back本质上就是先将nullptr进行隐式构造然后拷贝构造到容器内部，因此也是被禁止的，改成下面的代码后即可编译通过：
```
std::regex r1(nullptr);     // 显式构造
```
上文中emplace_back可以插入nullptr的原因是因为emplace_back是将nullptr参数通过完美转发后传入到容器中直接显式构造正则对象。

综上所述，当使用emplace系列函数的时候要特别注意确保传递正确的参数，因为显式构造函数被认为是编译器试图找了一种正确的方式来让你的代码有效。
