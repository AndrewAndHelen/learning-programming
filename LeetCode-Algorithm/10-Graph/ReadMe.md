# 图

**54** 旋转矩阵

* 给你一个 `m` 行 `n` 列的矩阵 `matrix` ，请按照 **顺时针螺旋顺序** ，返回矩阵中的所有元素。

* 示例
```
输入：matrix = [[1,2,3],[4,5,6],[7,8,9]]
输出：[1,2,3,6,9,8,7,4,5]
```

* 题解
```
可以将矩阵看成若干层，首先输出最外层的元素，其次输出次外层的元素，直到输出最内层的元素。

定义矩阵的第k层是到最近边界距离为k的所有顶点。例如，下图矩阵最外层元素都是第1层，次外层元素都是第2层，剩下的元素都是第3层。

对于每层，从左上方开始以顺时针的顺序遍历所有元素。假设当前层的左上角位于（top,left)，右下角位于 (bottom,right)，按照如下顺序遍历当前层的元素。

从左到右遍历上侧元素，依次为(top,left)到(top,right)。

从上到下遍历右侧元素，依次为(top+1,right)到(bottom,right)。

如果top<bottom，则从右到左遍历下侧元素，依次为(bottom,right−1)到(bottom,left+1)，以及从下到上遍历左侧元素，依次为(bottom,left)到(top+1,left)。

遍历完当前层的元素之后，将left和top分别增加1，将right和bottom 分别减少1，进入下一层继续遍历，直到遍历完所有元素为止。

```

* 代码
```
class Solution {
public:
    vector<int> spiralOrder(vector<vector<int>>& matrix) {
        int rows = matrix.size();
        int cols = matrix[0].size();

        if(rows==0&&cols==0)
            return {};
        
        int top,left,bottom,right;      //top bottom rows. left right cols
        vector<int> order;
        top=left=0;
        bottom=rows-1;
        right =cols-1;

        while(top<=bottom&&left<=right)
        {
            for(int col=left;col<=right;col++)
                order.push_back(matrix[top][col]);
            for(int row=top+1;row<=bottom;row++)
                order.push_back(matrix[row][right]);
            
            if(top<bottom&&left<right)
            {
                for(int col=right-1;col>=left+1;col--)
                    order.push_back(matrix[bottom][col]);
                for(int row=bottom;row>=top+1;row--)
                    order.push_back(matrix[row][left]);
            }
            ++top;
            ++left;
            --bottom;
            --right;
        }

        return order;
    }
};
```

**1135** 最低成本联通所有城市
* 想象一下你是个城市基建规划者，地图上有 N 座城市，它们按以 1 到 N 的次序编号。给你一些可连接的选项 `conections`，其中每个选项 conections[i] = [city1, city2, cost] 表示将城市 `city1` 和城市 `city2` 连接所要的成本。（连接是双向的，也就是说城市 `city1` 和城市 `city2` 相连也同样意味着城市 `city2` 和城市 `city1` 相连）。返回使得每对城市间都存在将它们连接在一起的连通路径（可能长度为 1 的）最小成本。该最小成本应该是所用全部连接代价的综合。如果根据已知条件无法完成该项任务，则请你返回 -1。

* 示例
```
输入：N = 3, conections = [[1,2,5],[1,3,6],[2,3,1]]
输出：6
解释：
选出任意 2 条边都可以连接所有城市，我们从中选取成本最小的 2 条。
```

* 题解
```
最小生成树的Kruskal的求法（一个有N个点的连通图，有N-1条边）：
1） 将连接边按权值（cost）从小到大排序；
2） 依次取出权值最小的边，看是否构成环，如果构成环，则将当前边废弃，否则选用当前边
3） 直到边数已经达到N-1，则可停止循环，此时得出来的cost即是最小的cost
当循环都已经走完了，还是没有选出来N-1条边，则返回-1，说明当前的连接关系不足以构成连通图。
```
* 代码
```
class UnionFindSet
{
public:
    UnionFindSet(int n);
    ~UnionFindSet();
    int findParent(int i);
    void unionSet(int a,int b);
private:
    vector<int> parent;
    vector<int> sz;
};

UnionFindSet::UnionFindSet(int n)
{
    parent.resize(n+1);
    sz.resize(n+1);
    for(int i=1;i<=n;++i)
    {
        parent[i]=i;
        sz[i]=1;
    }
}

UnionFindSet::~UnionFindSet()
{
    parent.erase(parent.begin(), parent.end());
}

int UnionFindSet::findParent(int i)//路径压缩
{
    int root_id=i;
    while(root_id!=parent[root_id])
        root_id=parent[root_id];
    int tmp;
    while(i!=root_id)
    {
        tmp=parent[i];
        parent[i]=root_id;
        i=tmp;
    }

    return root_id;
}

void UnionFindSet::unionSet(int a,int b)//按秩合并（秩为树的结点数）
{
    int pa=findParent(a);
    int pb=findParent(b);
    if(pa==pb)
        return;
    if(sz[pa]>sz[pb])
    {
        sz[pa]+=sz[pb];
        parent[pb]=pa;
    }
    else
    {
        sz[pb]+=sz[pa];
        parent[pa]=pb;
    }
}

class Solution {
public:
    int minimumCost(int N, vector<vector<int>>& connections) {
        sort(connections.begin(),connections.end(),
            [](vector<int>& a,vector<int>& b) {return a[2]<b[2];});
        
        UnionFindSet ufs(N);
        int cost=0;
        int cnt=0;

        for(int i=0;i<connections.size();++i)
        {
            if(ufs.findParent(connections[i][0])!=ufs.findParent(connections[i][1]))
            {
                ufs.unionSet(connections[i][0],connections[i][1]);
                cost+=connections[i][2];
                ++cnt;
            }
            if(cnt==N-1)
                break;
        }

        if(cnt<N-1)
            return -1;
        
        return cost;

    }
};
```

**785** 判断二分图

* 题目描述:存在一个 无向图 ，图中有 n 个节点。其中每个节点都有一个介于 0 到 n - 1 之间的唯一编号。给你一个二维数组 graph ，其中 graph[u] 是一个节点数组，由节点 u 的邻接节点组成。形式上，对于 graph[u] 中的每个 v ，都存在一条位于节点 u 和节点 v 之间的无向边。该无向图同时具有以下属性：
  - 不存在自环（`graph[u]`不包含`u`）
  - 不存在平行边（`graph[u]`不包含重复值）
  - 如果 `v` 在 `graph[u]` 内，那么 `u` 也应该在 `graph[v]` 内（该图是无向图）
  - 这个图可能不是连通图，也就是说两个节点 `u` 和 `v` 之间可能不存在一条连通彼此的路径。



* 二分图定义：如果能将一个图的节点集合分割成两个独立的子集 A 和 B ，并使图中的每一条边的两个节点一个来自 A 集合，一个来自 B 集合，就将这个图称为 二分图 。如果图是二分图，返回 true ；否则，返回 false 。

* 示例

```
输入：graph = [[1,2,3],[0,2],[0,1,3],[0,2]]
输出：false
解释：不能将节点分割成两个独立的子集，以使每条边都连通一个子集中的一个节点与另一个子集中的一个节点。
```

* 题解 

```
对于图中的任意两个节点 uu 和 vv，如果它们之间有一条边直接相连，那么 uu 和 vv 必须属于不同的集合。

如果给定的无向图连通，那么我们就可以任选一个节点开始，给它染成红色。随后我们对整个图进行遍历，将该节点直接相连的所有节点染成绿色，表示这些节点不能与起始节点属于同一个集合。我们再将这些绿色节点直接相连的所有节点染成红色，以此类推，直到无向图中的每个节点均被染色。

如果我们能够成功染色，那么红色和绿色的节点各属于一个集合，这个无向图就是一个二分图；如果我们未能成功染色，即在染色的过程中，某一时刻访问到了一个已经染色的节点，并且它的颜色与我们将要给它染上的颜色不相同，也就说明这个无向图不是一个二分图。

算法的流程如下：

我们任选一个节点开始，将其染成红色，并从该节点开始对整个无向图进行遍历；

在遍历的过程中，如果我们通过节点u遍历到了节点v（即u和v在图中有一条边直接相连），那么会有两种情况：

如果v未被染色，那么我们将其染成与u不同的颜色，并对v直接相连的节点进行遍历；

如果v被染色，并且颜色与u相同，那么说明给定的无向图不是二分图。我们可以直接退出遍历并返回false;

当遍历结束时，说明给定的无向图是二分图，返回true作为答案。

我们可以使用「深度优先搜索」或「广度优先搜索」对无向图进行遍历，下文分别给出了这两种搜索对应的代码。

注意：题目中给定的无向图不一定保证连通，因此我们需要进行多次遍历，直到每一个节点都被染色，或确定答案为false为止。每次遍历开始时，我们任选一个未被染色的节点，将所有与该节点直接或间接相连的节点进行染色。

```

* 代码

```c++
class Solution {
public:
    bool isBipartite(vector<vector<int>>& graph) {
        int num_vertex=graph.size();
        vector<int> color(num_vertex,-1);
        stack<int> color_stack;

        for(int i=0;i<num_vertex;i++)
        {
            if(color[i]==-1)
            {
                color[i]=0;
                color_stack.push(i);
            }

            while(!color_stack.empty())
            {
                int k=color_stack.top();
                color_stack.pop();

                for(int j=0;j<graph[k].size();j++)
                {
                    if(color[graph[k][j]]==-1)
                    {
                        color[graph[k][j]]=color[k]^1;
                        color_stack.push(graph[k][j]);
                    }
                    else
                    {
                        if((color[graph[k][j]]^color[k])!=1)
                            return false;
                    }
                }
            }

        }

        return true;
    }
};
```









## Contack

If you found bugs or have new ideas,please pull requests😄   
If you have trouble compiling or using this software,email to [15313326374@163.com](mailto:15313326374@163.com)  