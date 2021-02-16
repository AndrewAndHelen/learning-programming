#pragma once
#ifndef SORT_H
#define SORT_H
#include<iostream>
#include<vector>
#include<assert.h>
#include <type_traits>

template<typename Object>
static void swap(std::vector<Object>& SqList, int i, int j);

template<typename Object>
void print(const std::vector<Object>& SqList);

/*�����㷨*/
//����������
template<typename Object>
void BubbleSort(std::vector<Object>& SqList);

template<typename Object>
void QuickSort(std::vector<Object>& SqList);

//ѡ��������
template<typename Object>
void SelectSort(std::vector<Object>& SqList);

template<typename Object>
void HeapSort(std::vector<Object>& SqList);

//��������
template<typename Object>
void InsertSort(std::vector<Object>& SqList);

template<typename Object>
void ShellSort(std::vector<Object>& SqList);

//�ϲ�����
template<typename Object>
void MergeSort(std::vector<Object>& SqList);

//�ǱȽ�������
template<typename Object, typename = std::enable_if_t<std::is_convertible_v<Object, int>>>
void CountSort(std::vector<Object>& SqList);

template<typename Object, typename = std::enable_if_t<std::is_convertible_v<Object, int>>>
void BucketSort(std::vector<Object>& SqList);

template<typename Object>
void swap(std::vector<Object>& SqList, int i, int j)
{
	assert(i >= 0 && i < SqList.size());
	assert(j >= 0 && j < SqList.size());

	std::swap(SqList[i], SqList[j]);
}

template<typename Object>
void print(const std::vector<Object>& SqList)
{
	for (auto i = SqList.begin(); i != SqList.end(); i++)
		std::cout << *i<<"\t";
}

template<typename Object>
static void quicksort(std::vector<Object>& SqList, int low, int high)
{
	if (low >= high)
		return;
	int i = low, j = high;
	float index = SqList[i];

	while (i < j)
	{
		while (i < j&& SqList[j] >= index) { // ����Ѱ�ҵ�һ��С��index����
			j--;
		}

		if (i < j)
			SqList[i++] = SqList[j];// ��array[j]����array[i]������i�����ƶ�

		while (i < j&&SqList[i] < index) {// ����Ѱ�ҵ�һ������index����
			i++;
		}
		if (i < j)
			SqList[j--] = SqList[i];// ��array[i]����array[j]������j�����ƶ�
	}

	SqList[i] = index;
	quicksort(SqList, low, i - 1);
	quicksort(SqList, i + 1, high);
}

template<typename Object>
static void HeapAdjust(std::vector<Object>& SqList, int s, int m)
{
	int tmp, j;
	tmp = SqList[s];
	for (j = 2 * s + 1; j < m; j = 2 * s + 1)//�Ӹ��ڵ�����ӽڵ㿪ʼ
	{
		if (j < m&&SqList[j] < SqList[j + 1])
			j++;
		if (tmp >= SqList[j])
			break;
		SqList[s] = SqList[j];
		s = j;
	}
	SqList[s] = tmp;
}

//�������SR[i..m]��SR[m+1..n]�鲢Ϊ�����TR[i..n]
template<typename Object>
static void merge(std::vector<Object>& arr, int start, int mid, int end) {
	std::vector<Object>  temp(end - start + 1);    //�����������ݵ���ʱ����
	int k = 0;        //��ʱ�����ָ��
	int i = start;    //��һ����������ָ��
	int j = mid + 1;    //�ڶ�����������ָ��

	while (i <= mid && j <= end) {
		if (arr[i] <= arr[j])
			temp[k++] = arr[i++];
		else
			temp[k++] = arr[j++];
	}
	while (i <= mid)
		temp[k++] = arr[i++];
	while (j <= end)
		temp[k++] = arr[j++];

	for (k = 0; k <= end - start; k++)
		arr[k + start] = temp[k];

}

template<typename Object>
static void mSort(std::vector<Object>& arr, int start, int end) {
	if (start == end)
		return;
	int mid = (start + end) / 2;
	mSort(arr, start, mid);
	mSort(arr, mid + 1, end);
	merge(arr, start, mid, end);
}

template<typename Object>
void BubbleSort(std::vector<Object>& SqList)
{
	int length = SqList.size();
	for (int i = 0; i < length; ++i)
	{
		for (int j = length - 1; j >= i + 1;--j)
		{
			if (SqList[j] < SqList[j - 1])
				swap(SqList, j, j - 1);
		}
	}
}

template<typename Object>
void QuickSort(std::vector<Object>& SqList)
{
	quicksort(SqList, 0, SqList.size() - 1);
}

template<typename Object>
void SelectSort(std::vector<Object>& SqList)
{
	int length = SqList.size();
	int min = 0;

	for (int i = 0; i < length; i++)
	{
		min = i;
		for (int j = i + 1; j < length; j++)
		{
			if (SqList[j] < SqList[min])
				min = j;
		}

		if (i != min)
			swap(SqList, i, min);
	}
}

template<typename Object>
void HeapSort(std::vector<Object>& SqList)
{
	for (int i = SqList.size() / 2 - 1; i >= 0; i--)
		HeapAdjust(SqList, i, SqList.size() - 1);

	for (int i = SqList.size() - 1; i > 1; i--)
	{
		swap(SqList, 0, i);
		HeapAdjust(SqList, 0, i - 1);
	}
}
template<typename Object>
void InsertSort(std::vector<Object>& SqList)
{
	int current_index;
	Object current;

	for (int i = 1; i < SqList.size(); i++)
	{
		current_index = i;
		current = SqList[current_index];

		while (current_index-1 >=0&& SqList[current_index-1]>current)
		{
			SqList[current_index] = SqList[current_index-1];
			current_index--;
		}
		SqList[current_index ] = current;
	}
}

template<typename Object>
void ShellSort(std::vector<Object>& SqList)
{
	for (int gap = SqList.size() / 2; gap > 0; gap /= 2)
	{
		int current_index;
		Object current;

		for (int i = gap; i < SqList.size(); i++)
		{
			current_index = i;
			current = SqList[current_index];
			while (current_index-gap>=0&&SqList[current_index - gap]> current)
			{
				SqList[current_index] = SqList[current_index - gap];
				current_index -= gap;
			}

			SqList[current_index] = current;
		}
	}
}

template<typename Object>
void MergeSort(std::vector<Object>& SqList)
{
	mSort(SqList, 0, SqList.size()-1);
}

template<typename Object, typename = std::enable_if_t<std::is_convertible_v<Object, int>>>
void CountSort(std::vector<Object>& SqList)
{
	Object max = SqList[0];

	for (int i = 1; i < SqList.size(); i++)
	{
		if (SqList[i] > max)
			max = SqList[i];
	}

	std::vector<Object> count_list(max+1);

	for (int i = 0; i < SqList.size(); i++)
		count_list[SqList[i]]++;

	int index = 0;
	for (int i = 0; i < count_list.size(); i++)
		for (int j = 0; j < count_list[i]; j++)
			SqList[index++] = i;
}

template<typename Object, typename = std::enable_if_t<std::is_convertible_v<Object, int>>>
void BucketSort(std::vector<Object>& SqList)
{
	Object max = std::numeric_limits<Object>::min();
	Object min = std::numeric_limits<Object>::max();

	// �������ֵ����Сֵ
	for (int i = 0; i < SqList.size(); i++)
	{
		max = max > SqList[i] ? max : SqList[i];
		min = min < SqList[i] ? min : SqList[i];
	}

	// ����Ͱ������
	int bucket_num = static_cast<int>((max - min) / SqList.size() + 1);
	std::vector<std::vector<Object>> bucket_list(bucket_num);

	// ��ÿ��Ԫ�ط���Ͱ
	for (int i = 0; i < SqList.size(); i++)
	{
		int num = static_cast<int>((SqList[i] - min) / SqList.size() );
		bucket_list[num].push_back(SqList[i]);
	}

	// ��ÿ��Ͱ��������
	for (int i = 0; i < bucket_list.size(); i++)
		CountSort(bucket_list[i]);

	// ��Ͱ�е�Ԫ�ظ�ֵ��ԭ����
	int index = 0;
	for (int i = 0; i < bucket_list.size(); i++)
		for (int j = 0; j < bucket_list[i].size(); j++)
			SqList[index++] = bucket_list[i].at(j);

}


#endif // !SORT_H

