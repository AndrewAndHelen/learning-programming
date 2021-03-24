# 并查集

**200** 岛屿数量

* 给你一个由 `1`（陆地）和 `0`（水）组成的的二维网格，请你计算网格中岛屿的数量。

  岛屿总是被水包围，并且每座岛屿只能由水平方向和/或竖直方向上相邻的陆地连接形成。

  此外，你可以假设该网格的四条边均被水包围。

* 示例
```
输入：grid = [
  ["1","1","1","1","0"],
  ["1","1","0","1","0"],
  ["1","1","0","0","0"],
  ["0","0","0","0","0"]
]
输出：1
```

* 题解
```
使用并查集进行解答
```
* 代码
```
class UnionFindSet{
public:
    UnionFindSet(vector<vector<char>>& grid);
    ~UnionFindSet();
    int getCount(){return count;}
    int findParent(int x);
    void unionSet(int x,int y);
private:
    vector<int> parent;
    vector<int> sz;
    int count;
};

UnionFindSet::UnionFindSet(vector<vector<char>>& grid)
{
    int rows = grid.size();
    int cols = grid[0].size();
    count=0;
    for(int i=0;i<rows;++i)
        for(int j=0;j<cols;++j)
        {
            parent.push_back(i*cols+j);
            sz.push_back(1);
            if(grid[i][j]=='1')
                count++;
        }
   
}

UnionFindSet::~UnionFindSet()
{
    parent.erase(parent.begin(), parent.end());
    sz.erase(sz.begin(), sz.end());
}

int UnionFindSet::findParent(int x)
{
    int root_id = x;
    while(root_id!=parent[root_id])
        root_id=parent[root_id];
    
    int tmp;
    while(x!=root_id)
    {
        tmp = parent[x];
        parent[x] = root_id;
        x = tmp;
    }
    return root_id;
}

void UnionFindSet::unionSet(int x, int y)
{
    int px = findParent(x);
    int py = findParent(y);
    if(px==py)
        return;
    
    if(sz[px]>sz[py])
    {
        sz[px]+=sz[py];
        parent[py]=px;
    }
    else
    {
        sz[py]+=sz[px];
        parent[px]=py;
    }
    count--;
}
class Solution {
public:
    int numIslands(vector<vector<char>>& grid) {
        if(grid.size()==0)
            return 0;
        
        UnionFindSet uf(grid);

        int rows = grid.size();
        int cols = grid[0].size();
        for(int i=0;i<rows;++i)
            for(int j=0;j<cols;++j)
            {
                if(grid[i][j]=='1')
                {
                    if(i-1>=0&&grid[i-1][j]=='1')
                    {
                        if(uf.findParent((i-1)*cols+j)!=uf.findParent(i*cols+j))
                            uf.unionSet((i-1)*cols+j,i*cols+j);
                    }
                    if(i+1<=rows-1&&grid[i+1][j]=='1')
                    {
                        if(uf.findParent((i+1)*cols+j)!=uf.findParent(i*cols+j))
                            uf.unionSet((i+1)*cols+j,i*cols+j);
                    }
                    if(j-1>=0&&grid[i][j-1]=='1')
                    {
                         if(uf.findParent(i*cols+j-1)!=uf.findParent(i*cols+j))
                            uf.unionSet(i*cols+j-1,i*cols+j);
                    }
                    if(j+1<=cols-1&&grid[i][j+1]=='1')
                    {
                         if(uf.findParent(i*cols+j+1)!=uf.findParent(i*cols+j))
                            uf.unionSet(i*cols+j+1,i*cols+j);
                    }
                }
            }
        return uf.getCount();
    }
};
```



## Contack

If you found bugs or have new ideas,please pull requests😄   
If you have trouble compiling or using this software,email to [15313326374@163.com](mailto:15313326374@163.com)  