# 数组

**485.**最大连续1的个数（一）
* 给定一个二进制数组， 计算其中最大连续 1 的个数。

* 示例
```
输入：[1,1,0,1,1,1]
输出：3
解释：开头的两位和最后的三位都是连续 1 ，所以最大连续 1 的个数是 3.
```

* 代码
```
class Solution {
public:
    int findMaxConsecutiveOnes(vector<int>& nums) {
        int maxCount=0;
        int count=0;

        for(int num:nums)
        {
           if(num==1)
                count++;
            if(num==0)
                count=0;
            
            maxCount = max(count,maxCount);
        }
        return maxCount;
    }
};
```

**487.**最大连续1的个数（二）
* 给定一个二进制数组，你可以最多将 1 个 0 翻转为 1，找出其中最大连续 1 的个数。
* 示例
```
输入：[1,0,1,1,0]
输出：4
解释：翻转第一个 0 可以得到最长的连续 1。
     当翻转以后，最大连续 1 的个数为 4。
```

* 题解
```
该题为找最大滑动窗口问题，限制条件为窗口内有K个0，此题K=1
初始化最大窗口长度len=0，窗口左侧索引left=0，窗口右侧索引right=0，窗口内最大0的个数maxZero=0;

依次增大right，循环遍历数组nums
    如果nums[right]==0,maxZero++;
    如果maxZero>K
        如果nums[left]==0,maxZero--;
        left++;
	len=max(len,right-left+1);

返回len;
```

* 代码
```
class Solution {
public:
    int findMaxConsecutiveOnes(vector<int>& nums) {
        int len=0,left=0,right=0;
        int maxZero=0;
        for(right;right<nums.size();++right)
        {
            if(nums[right]==0)
                ++maxZero;
            if(maxZero>1)
            {
                if(nums[left++]==0)
                    --maxZero;
            }
            len = max(len,right-left+1);
        }
        return len;
    }
};
```

**1004.**最大连续1的个数（三）
* 给定一个由若干 0 和 1 组成的数组 A，我们最多可以将 K 个值从 0 变成 1 。返回仅包含 1 的最长（连续）子数组的长度。

* 示例
```
输入：A = [1,1,1,0,0,0,1,1,1,1,0], K = 2
输出：6
解释： 
[1,1,1,0,0,1,1,1,1,1,1]
粗体数字从 0 翻转到 1，最长的子数组长度为 6。
```

* 题解
```
如487题解
```

* 代码
```
class Solution {
public:
    int longestOnes(vector<int>& A, int K) {
        int res = 0, zeros = 0, left = 0;
        for (int right = 0; right < A.size(); ++right) {
            if (A[right] == 0) ++zeros;
            if (zeros > K) {
                if (A[left++] == 0) --zeros;
            }
            res = max(res, right - left + 1);
        }
        return res;
    }
};
```
 