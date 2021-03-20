# 堆

**215. 数组中的第K个最大元素**

* 在未排序的数组中找到第 k 个最大的元素。请注意，你需要找的是数组排序后的第 k 个最大的元素，而不是第 k 个不同的元素。

* 示例
```
输入: [3,2,3,1,2,4,5,5,6] 和 k = 4
输出: 4
```

* 题解
```cpp
//方法一：使用STL内置的排序算法
class Solution {
public:
    int findKthLargest(vector<int>& nums, int k) {
        sort(nums.rbegin(),nums.rend());
        return nums[k-1];
    }
};
//方法二：使用priority_queue
class Solution {
public:
    int findKthLargest(vector<int>& nums, int k) {
        priority_queue<int,vector<int>,greater<int>> my_queue;// // 创建小顶堆（堆顶为最小元素，升序排列）
        // 先填充前 k 个元素入堆
        for(int i=0;i<k;i++){
            my_queue.push(nums[i]);
        }
        for(int i=k;i<nums.size();i++){
            if(nums[i]>my_queue.top()){
            // 若 nums[i] 大于堆顶元素，弹出堆顶元素并将 nums[i] 入堆
                my_queue.pop();
                my_queue.push(nums[i]);
            }
        }
        return my_queue.top();
    }
};
```

