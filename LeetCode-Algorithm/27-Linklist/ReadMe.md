# 堆

**82**. 删除排序链表中的重复元素2
* 存在一个按升序排列的链表，给你这个链表的头节点 head ，请你删除链表中所有存在数字重复情况的节点，只保留原始链表中 没有重复出现 的数字。返回同样按升序排列的结果链表。

* 示例
```
输入：head = [1,2,3,3,4,4,5]
输出：[1,2,5]
```

* 代码
```
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
class Solution {
public:
    ListNode* deleteDuplicates(ListNode* head) {
        ListNode* dumpy = new ListNode();
        ListNode* tail = dumpy;

        while(head)
        {
            if(head->next==nullptr || head->val!=head->next->val)
            {
                tail->next=head;
                tail=head;
            }
            while(head->next && head->val==head->next->val)
                head=head->next;
            head=head->next;
        }
        tail->next=nullptr;
        return dumpy->next;
    }
};
```
# 链表排序
## **148. 排序链表**
* 给你链表的头结点 head ，请将其按 升序 排列并返回 排序后的链表 。
* 示例
```
输入：head = [4,2,1,3]
输出：[1,2,3,4]
```
* Code
```cpp
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
class Solution {
private:
    //将链表head切掉前n个节点，并返回后半部分的链表头。
    ListNode* cut(ListNode* head,int n){
        auto temp=head;
        while(--n&&temp){
            temp=temp->next;
        }
        if(temp==nullptr) return nullptr;
        auto result=temp->next;
        temp->next=nullptr;
        return result;
    }
    //合并操作
    ListNode * merge(ListNode* Left,ListNode* Right){
        ListNode* dummyHead = new ListNode(0);
        auto temp=dummyHead;

        while(Left&&Right){
            if(Left->val<Right->val){
                temp->next=Left;
                temp=temp->next;
                Left=Left->next;
            }
            else{
                temp->next=Right;
                temp=temp->next;
                Right=Right->next;
            }
        }

        temp->next=Left!=nullptr?Left:Right;
        return dummyHead->next;
    }
public:
    ListNode* sortList(ListNode* head) {
        //方法一：将元素从链表之中取出后，存入有限队列之中，然后将其重新放入链表
        //空间复杂度：O(n) 时间复杂度：O(nlogn)
        //不符合题意
        /*
        if(head==nullptr) return head;
        priority_queue<int,vector<int>,greater<int>> queueNumbers;
        auto currentNode=head;
        while(currentNode){
            queueNumbers.push(currentNode->val);
            currentNode=currentNode->next;
        }
        currentNode=head;
        while(!queueNumbers.empty()){
            currentNode->val=queueNumbers.top();
            currentNode=currentNode->next;
            queueNumbers.pop();
        }
        return head;
        */
        //方法二：归并排序
        //空间复杂度：O(1) 时间复杂度：O(nlogn)
        //设计技巧 断链操作cut(ListNode*,int)
        //         合并操作merge(ListNode*,ListNode*)
        if(head==nullptr) return head;
        
        //统计节点数量
        auto numsPtr=head;
        int nodeSize=0;
        while(numsPtr){
            nodeSize++;
            numsPtr=numsPtr->next;
        }

        ListNode* dummyHead = new ListNode(0, head);

        for(int i=1;i<nodeSize;i*=2){//依次将链表分成1块，2块，4块...
        //每次变换步长，tail指针和currentNode指针都初始化在链表头
            auto currentNode=dummyHead->next;
            auto tail=dummyHead;

            while(currentNode){
                auto left=currentNode;
                auto right=cut(left,i);
                currentNode=cut(right,i);

                tail->next=merge(left,right);
                while(tail->next) tail=tail->next;
            }
        }

        return dummyHead->next;
    }
};
```
## 19. 删除链表的倒数第 N 个结点
* 给你一个链表，删除链表的倒数第 n 个结点，并且返回链表的头结点。
* 示例
```
输入：head = [1,2,3,4,5], n = 2
输出：[1,2,3,5]
```
* Code
```cpp
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
class Solution {
public:
    ListNode* removeNthFromEnd(ListNode* head, int n) {
        //方法一：采取分情况讨论的方法
        /*
        int nodeNums=0;
        auto temp=head;
        while(temp->next&&++nodeNums) temp=temp->next;//统计节点数量

        temp=head;
        //情况一：删除头结点
        if(n==nodeNums){
            return head->next;
        }
        else if(n==1){//删除尾结点
            nodeNums-=1;
            while(nodeNums--!=1) temp=temp->next;
            temp->next=temp->next->next;
        }
        else{//普通情况
            int key=nodeNums-n;
            while(key--!=1) temp=temp->next;
            temp->next=temp->next->next;
        }
        return head;
        */
        //方法二：快慢指针
        if(head->next==nullptr) return nullptr;
        auto slow=head,fast=head;
        while(n--) fast=fast->next;
        if(!fast) return head->next;
        while(fast->next){
            slow=slow->next;
            fast=fast->next;
        }
        slow->next=slow->next->next;
        return head;
    }
};
```
