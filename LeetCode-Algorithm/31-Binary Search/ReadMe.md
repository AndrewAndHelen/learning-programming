# 堆

 **74.搜索二维矩阵**

* 编写一个高效的算法来判断 m x n 矩阵中，是否存在一个目标值。该矩阵具有如下特性：
	* 每行中的整数从左到右按升序排列。
	* 每行的第一个整数大于前一行的最后一个整数。

* 示例
```
输入：matrix = [[1,3,5,7],[10,11,16,20],[23,30,34,60]], target = 3
输出：true
```

* 题解
```
首先找出target在二维数组中的上界（即首先比target大的第一列所在行数），然后row--，
在row行对target进行二分查找，如果找到，则表明存在。
```

* 代码
```
class Solution {
public:
    bool searchMatrix(vector<vector<int>>& matrix, int target) {
        auto upper_row = upper_bound(matrix.begin(),matrix.end(),target,[](int a,vector<int>b){return a<b[0];});
        
        if(upper_row==matrix.begin())
            return false;

        upper_row--;

        return binary_search(upper_row->begin() , upper_row->end(), target);
    }
};
```
