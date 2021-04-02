# 面试题目

* 17.21.直方图的水量
* 给定一个直方图(也称柱状图)，假设有人从上面源源不断地倒水，最后直方图能存多少水量?直方图的宽度为 1。
![](https://assets.leetcode-cn.com/aliyun-lc-upload/uploads/2018/10/22/rainwatertrap.png)
* 上面是由数组 [0,1,0,2,1,0,1,3,2,1,2,1] 表示的直方图，在这种情况下，可以接 6 个单位的水（蓝色部分表示水）。

* 示例
```
输入: [0,1,0,2,1,0,1,3,2,1,2,1]
输出: 6
```

* 解题思路
```
循环遍历直方图vector,依题意可知，直方图左右两外侧的方格不能储水。
找出每个格子左侧最高的数值
for(i=1:n-1)   left[i]=max(left[i-1],height[i])
找出每个格子右侧最高的数值
for(i=n-2;i>0)   right[i]=max(right[i+1],height[i])

每个格子上方能存储水的容量为，min(left[i],right[i])-height[i];
累计相加可以得出结果
```

* 代码
```
class Solution {
public:
    int trap(vector<int>& height) {
        int n=height.size();
        if(n==0) return 0;

        vector<int> left(n,0),right(n,0);
        left[0]=height[0];

        for(int i=1;i<n;++i)
            left[i]= max(left[i-1],height[i]);
        
        right[n-1]=height[n-1];

        for(int i=n-2;i>0;--i)
            right[i]=max(right[i+1],height[i]);
        
        int sum=0;
        for(int i=1;i<n-1;++i)
            sum+=min(left[i],right[i])-height[i];

        return sum;
    }
};
```