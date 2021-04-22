## 广度优先搜索

### 837. 访问所有节点的最短路径
* 给出 graph 为有 N 个节点（编号为 0, 1, 2, ..., N-1）的无向连通图。 graph.length = N，且只有节点 i 和 j 连通时，j != i 在列表 graph[i] 中恰好出现一次。返回能够访问所有节点的最短路径的长度。你可以在任一节点开始和停止，也可以多次重访节点，并且可以重用边

* 示例
```
输入：[[1,2,3],[0],[0],[0]]
输出：4
解释：一个可能的路径为 [1,0,2,0,3]
```

* 题解
```
使用BFS+剪枝的方法进行求解
利用二进制位表示节点访问状态，举例，当共有8个节点时，0000 0001表示已访问0号节点。
当所有节点都访问过时，二进制数的大小为2^n-1。
BFS的思想就是将图的遍历作为一种穷举，遍历从每个节点出发的情况，找到最快符合条件的一种。
剪枝方法就是建立一个二维数组booked，行数为节点的个数，列数为2^n-1（所有节点都遍历的二进制数+1）。
也就是说如果booked[i][state]==1时，表示存在一条分支已经是本分支将要走的路径，那么就不要加入队列重复操作了。
具体可看代码
```

* 代码
```
class Solution {
public:
    int shortestPathLength(vector<vector<int>>& graph) {
        int graphSize = graph.size(), minSteps = 0;
        int allVisited = (1 << graphSize) - 1;
        vector<vector<int>> booked(graphSize, vector<int>(allVisited + 1, 0));
        queue<pair<int, int>> myQue;
        for (int i = 0; i < graphSize; ++i){
            myQue.push({i, 1 << i});
        }
        while (!myQue.empty())
        {
            for (int i = myQue.size(); i > 0; --i)
            {
                pair<int, int> front = myQue.front();
                myQue.pop();
                if (allVisited == front.second)
                    return minSteps;
                
                for (auto &next : graph[front.first])
                {
                    int next_state = front.second | (1 << next);
                    if (!booked[next][next_state])
                    {
                        booked[next][next_state] = 1;
                        myQue.push({next, next_state});
                    }
                }
            }
            ++minSteps;//路边的条数自增
        }
        return minSteps;
    }
};
```

