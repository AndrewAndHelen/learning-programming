# 数学

**42**. 接雨水

* 给定 *n* 个非负整数表示每个宽度为 1 的柱子的高度图，计算按此排列的柱子，下雨之后能接多少雨水。

* 示例
![](https://assets.leetcode-cn.com/aliyun-lc-upload/uploads/2018/10/22/rainwatertrap.png)
```
输入：height = [0,1,0,2,1,0,1,3,2,1,2,1]
输出：6
解释：上面是由数组 [0,1,0,2,1,0,1,3,2,1,2,1] 表示的高度图，在这种情况下，可以接 6 个单位的雨水（蓝色部分表示雨水）。 
```

* 代码
```
class Solution {
public:
    int trap(vector<int>& height) {
        if(height.empty())
            return 0;
        int len = height.size();
        vector<int> left(len,0),right(len,0);
        left[0]=height[0];
        right[len-1]=height[len-1];

        for(int i=1;i<len;++i)
            left[i]=max(left[i-1],height[i]);
        
        for(int i=len-2;i>0;--i)
            right[i]=max(right[i+1],height[i]);

        int sum=0;

        for(int i=1;i<len-1;++i)
            sum+=(min(left[i],right[i])-height[i]);

        return sum;
    }
};
```

