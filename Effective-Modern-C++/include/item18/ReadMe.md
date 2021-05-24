## Item18:对于独占资源使用std::unique_ptr

* 引言
```
1.std::unique_ptr是轻量级、快速的、只能move的管理专有所有权语义资源的智能指针
2.默认情况，资源销毁通过delete，但是⽀持⾃定义delete函数。有状态的删除器和函数指针会增加
std::unique_ptr的⼤小
3.将std::unique_ptr转化为std::shared_ptr是简单的
```

* 
```

```