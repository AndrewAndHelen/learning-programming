## 回溯
**78** 子集
* 给你一个整数数组 nums ，数组中的元素 互不相同 。返回该数组所有可能的子集（幂集）。解集不能包含重复的子集。你可以按任意顺序返回解集。

* 示例
```
输入：nums = [1,2,3]
输出：[[ ],[1],[2],[1,2],[3],[1,3],[2,3],[1,2,3]]
```
* 代码

```cpp
class Solution {
private:
    vector<vector<int>> result;
    vector<int> temp;
    void backtracking(vector<int> &nums,int startindex){
        result.push_back(temp);
        //递归终止条件
        if(startindex>=nums.size()){
            return;
        }
        //单层搜索逻辑
        for(int i=startindex;i<nums.size();i++){//本层集合之中的元素（树中节点孩子的数量就是集合大小）
            temp.push_back(nums[i]);//处理节点，子集收集元素
            backtracking(nums,i+1);//递归，从i+1开始，元素不可以重复
            temp.pop_back();//回溯，撤销处理结果
        }
    }
public:
    vector<vector<int>> subsets(vector<int>& nums) {
        result.clear();
        temp.clear();
        backtracking(nums,0);
        return result;
    }
};
```
**90** 子集 II

 >给你一个整数数组 nums ，其中可能包含重复元素，请你返回该数组所有可能的子集（幂集）。
 >解集 不能 包含重复的子集。返回的解集中，子集可以按 任意顺序 排列。
 >
 ![](https://img-blog.csdnimg.cn/img_convert/087abd6a71816a97cdd253407cfe0b3f.png)
* 示例

```
输入：nums = [1,2,2]
输出：[[],[1],[1,2],[1,2,2],[2],[2,2]]
```
* 代码

```cpp
class Solution {
private:
    //回溯思想
    vector<vector<int>> result;
    vector<int> temp;
    void backtracking(vector<int> &nums,int startIndex){
        result.push_back(temp);
        if(startIndex>=nums.size()) return;
        for(int i=startIndex;i<nums.size();i++){
            //跳过树层已经被使用过的元素
            if(i>startIndex&&nums[i]==nums[i-1]) continue;
            temp.push_back(nums[i]);
            backtracking(nums,i+1);
            temp.pop_back();
        }
    }
public:
    vector<vector<int>> subsetsWithDup(vector<int>& nums) {
        result.clear();
        temp.clear();
        //去重需要进行预先排序
        sort(nums.begin(),nums.end());
        backtracking(nums,0);
        return result;
    }
};
```
**46** 全排列

 >给定一个 没有重复 数字的序列，返回其所有可能的全排列。
 ### 示例一
 ```
 输入: [1,2,3]
输出:
[
  [1,2,3],
  [1,3,2],
  [2,1,3],
  [2,3,1],
  [3,1,2],
  [3,2,1]
]
 ```
### Code
```cpp
//第一种解法
class Solution {
private:
    vector<vector<int>> result;
    vector<int> temp;
    void backtracking(vector<int>& nums,vector<bool>& used){
        //存储结果
        if(temp.size()==nums.size()){
            result.push_back(temp);
            return;
        }    

        for(int i=0;i<nums.size();i++){
            if(used[i]==true) continue;//元素被使用，跳过
            temp.push_back(nums[i]);
            used[i]=true;//使用之后，改变其状态为使用
            backtracking(nums,used);
            temp.pop_back();
            used[i]=false;//在撤销之后，改变其状态为未使用
        }
    }

public:
    vector<vector<int>> permute(vector<int>& nums) {
        result.clear();
        temp.clear();
        //保留每一个元素是否被使用的状态
        vector<bool> used(nums.size(),false);
        backtracking(nums,used);
        return result;

    }
};

//第二种解法
class Solution {
public:
    vector<vector<int>> permute(vector<int>& nums) {
        int len = nums.size();

        vector<vector<int>> result;
        permuteAll(nums,0,len-1,result);
        return result;
    }
    void permuteAll(vector<int>& nums,int left,int right,vector<vector<int>>& result)
    {
        if(left==right)
            result.push_back(nums);
        
        for(int i=left;i<=right;++i)
        {
            swap(nums[left],nums[i]);
            permuteAll(nums,left+1,right,result);
            swap(nums[left],nums[i]);
        }
    }
};
```
