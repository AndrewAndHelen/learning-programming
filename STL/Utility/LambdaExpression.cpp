#include<iostream>

int main()
{
	//�����б�[]��ֵ���ݵı�����һ����ʼ��(��int x=0Ϊ��ʼ����������ǳ�ʼ��x��ֵ��
	//����x��ô�仯lambda���ʽ������)�Ͳ���ı�(��Ϊһ������)
	int x = 0;
	int y = 0;
	auto qqq = [x, &y] {
		std::cout << "x" << x << std::endl;
		std::cout << "y" << y << std::endl;
		++y;
	};

	x = y = 77;
	qqq();
	qqq();
	std::cout << "final y:" << y << std::endl;

	//mutable��ʾ����ֵ���ݺͰ����ô��ݵĻ����
	//�������ʼ����idֵ��Ϊ�Լ����ڲ���Ա�������ں������ڸı�
	int id = 0;
	auto f = [id]() mutable {
		std::cout << "id:" << id << std::endl;
		id++;
	};

	id = 42;
	f();
	f();
	f();
	std::cout << id << std::endl;

	//decltype��ʾ�ƶ�һ�����ʽ������
	int ix = 0;
	decltype(ix) iy = 0;

	return 0;
}