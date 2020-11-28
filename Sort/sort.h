#pragma once
#ifndef SORT_H
#define SORT_H
#include "iostream"

const int MAXSIZE = 10;

struct SqList {
	int length;
	int r[MAXSIZE ];
	
};

void swap(SqList* p, int i, int j);

void BubbleSort(SqList* l);
void BubbleSort2(SqList* l);

void SelectSort(SqList* l);
void InsertSort(SqList* l);
void ShellSort(SqList* l);
void HeapSort(SqList* l);
void MergeSort(SqList *l);
void QuickSort(SqList* l);
void print(SqList* l);


#endif
