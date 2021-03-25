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
