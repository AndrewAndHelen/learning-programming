//std::auto_ptr�ѱ���������Ҫ����
//��Ҫ��std::unique_ptr��std::shared_ptr
//���ȼ� ()>[]>*
#include<iostream>
#include<string>
#include<vector>
#include<memory>
using namespace std;

int main()
{
	//�����һ��ӵ��������ʱ�����ӡ��Ϣ
	shared_ptr<string> pNico(new string("nico"),
		[](string* p) {
		cout << "delete " << *p << endl;
		delete p;
	});
	shared_ptr<string> pJutta(new string("jutta"));

	(*pNico)[0] = 'N';
	pJutta->replace(0, 1, "J");

	//��θ��Ƶ�һ��������
	vector<shared_ptr<string>> whoMadeCoffee;
	whoMadeCoffee.push_back(pJutta);
	whoMadeCoffee.push_back(pJutta);
	whoMadeCoffee.push_back(pNico);
	whoMadeCoffee.push_back(pJutta);
	whoMadeCoffee.push_back(pNico);

	//��ӡ����Ԫ��
	for (auto ptr : whoMadeCoffee)
	{
		cout << *ptr << " ";
	}
	cout << endl;

	//��д����
	*pNico = "Nicolai";
	for (auto ptr : whoMadeCoffee)
	{
		cout << *ptr << " ";
	}
	cout << endl;
	//�鿴���ü���Ϊ����
	cout << "use_count:" << whoMadeCoffee[0].use_count() << endl;

	/*���std::shared_ptrָ��һ�����飬Ҫ�Զ���deleter*/
	//��һ��
	//shared_ptr<int> p(new int[10],
	//	[](int *p) {
	//	delete[] p;
	//});
	////�ڶ���
	//shared_ptr<int> p(new int[10],
	//	default_delete<int[]>());

	//ָ��һ������ʱ��std::unique_ptr��std::shared_ptr�÷���ͬ
	std::unique_ptr<int[]> p(new int[10]);
	std::shared_ptr<int> q(new int[10]);
	
	//shared_ptr���ṩoperator [],unique_ptr�ṩoperator []ȡ��operator* ��operator ->
	int a = 1;

}