# 动态规划

**64**. 最小路径和

* 给定一个包含非负整数的 m x n 网格 grid ，请找出一条从左上角到右下角的路径，使得路径上的数字总和为最小。

* 说明：每次只能向下或者向右移动一步。

* 示例
```
输入：grid = [[1,3,1],[1,5,1],[4,2,1]]
输出：7
解释：因为路径 1→3→1→1→1 的总和最小。
```

* 题解

```
初始状态：f[0][0]=grid[0][0];
转移方程：
1. 当前位置只能通过 往下 移动而来，即有 f[i][j] = f[i-1][j] + grid[i][j]
2. 当前位置只能通过 往右 移动而来，即有 f[i][j] = f[i][j-1] + grid[i][j]
3. 当前位置既能通过 往下 也能 往右 移动，即有 f[i][j] = min(f[i][j-1],f[i-1][j]) + grid[i][j]

返回值：f[rows-1][cols-1]
```

* 代码
```
class Solution {
public:
    int minPathSum(vector<vector<int>>& grid) {
        int rows = grid.size();
        int cols = grid[0].size();

        vector<vector<int>> f(rows,vector<int>(cols,0));
        f[0][0]=grid[0][0];

        for(int i=1;i<rows;++i)
            f[i][0]=f[i-1][0]+grid[i][0];
        
        for(int j=1;j<cols;++j)
            f[0][j]=f[0][j-1]+grid[0][j];
        
        for(int i=1;i<rows;++i)
            for(int j=1;j<cols;++j)
            {
                f[i][j]=min(f[i-1][j]+grid[i][j],f[i][j-1]+grid[i][j]);
            }
       
        return f[rows-1][cols-1];
    }
};

```
