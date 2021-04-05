# 数学

**50**. Pow(x,n)
* 实现 pow(x, n) ，即计算 x 的 n 次幂函数（即，xn）。

* 示例
```
输入：x = 2.00000, n = 10
输出：1024.00000
```

* 代码
```
class Solution {
public:
    double quickMul(double x,long long n)
    {
        if(n==0)
            return 1;
        double y = quickMul(x, n/2);
        return n%2?y*y*x:y*y;
    }
    double myPow(double x, int n) {
        double y=quickMul(x, abs(n));
        if(n>=0)
            return y;
        else
            return 1/y;
    }
};
```

