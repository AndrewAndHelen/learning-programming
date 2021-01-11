#include<iostream>
#include<string>

//static_cast
class ANIMAL {
public:
	ANIMAL() :_type("ANIMAL") {};
	virtual void OutPutname() { std::cout << "ANIMAL"; };
private:
	std::string _type;
};
class DOG :public ANIMAL {
public:
	DOG() :_name("���"), _type("DOG") {};
	void OutPutname() { std::cout << _name<<std::endl;};
	void OutPuttype() { std::cout << _type<< std::endl; };
private:
	std::string _name;
	std::string _type;
};

//reinpreter_cast
class A {
public:
	A(int a = 0, int b = 0) : m_a(a), m_b(b) {}
private:
	int m_a;
	int m_b;
};
int main()
{
	/*
	static_cast
	�÷��� static_cast<type-id>(expression)
	˵���� 
	*��expressionת��Ϊtype-id���ͣ���û������ʱ���ͼ�鱣֤ת����ȫ��
	* �÷�1��
	* (1)�������νṹ�л��������֮��ָ������õ�ת������������ת�����������ָ�������ת���ɻ����ʾ���ǰ�ȫ�ģ�
	*			��������ת�����ѻ���ָ�������ת��������ָ������ã�ʱ������û�ж�̬���ͼ�飬�����ǲ���ȫ�ġ�
	* �÷�2��
	* ���ڻ�����������֮���ת�������intת����char����intת����enum������ת���İ�ȫ��ҲҪ������Ա����֤��
	* �÷�3��
	* ��voidָ��ת����Ŀ�����͵�ָ��(����ȫ!!)
	* �÷�4��
	* ���κ����͵ı��ʽת����void���͡�
	* Tips:static_cast����ת����expression��const��volitale������__unaligned���ԡ�
	*/
	double da = 1.999;
	int ia = static_cast<int>(da);

	//ʹ��static_cast�����һش����void*ָ���е�ֵ
	double a = 1.999;
	void * vptr = &a;
	double * dptr = static_cast<double*>(vptr);
	std::cout << *dptr << std::endl;//���1.999

	//������������ָ�����������֮���ת����������ת���లȫ����֮����ȫ
	//����ָ��תΪ������ָ��,�Ҹû���ָ��ָ��������
	ANIMAL * ani1 = new ANIMAL;
	DOG * dog1 = static_cast<DOG*>(ani1);
	//dog1->OutPuttype();//������ANIMAL����ָ�벻�ܵ��÷���OutPutType������������ʱ���ִ���

	//����ָ��תΪ������ָ�룬�Ҹû���ָ��ָ�����������
	ANIMAL * ani3 = new DOG;
	DOG* dog3 = static_cast<DOG*>(ani3);
	dog3->OutPutname(); //��ȷ

	//����ָ��תΪ������ָ��
	DOG *dog2 = new DOG;
	ANIMAL *ani2 = static_cast<DOG*>(dog2);
	ani2->OutPutname(); //��̬�󶨣� ����ָ��ָ�������࣬�����麯��ʱ�����ָ�����͵ķ���

	/*
	dynamic_cast
	�÷���dynamic_cast < type-id > ( expression )
	˵����
	* ���������expressionת����type-id���͵Ķ���Type-id���������ָ�롢������û���void *��
	* ���type-id����ָ�����ͣ���ôexpressionҲ������һ��ָ�룬
	* ���type-id��һ�����ã���ôexpressionҲ������һ�����á�

	*dynamic_cast��Ҫ�������μ������ת��������ת����������������֮��Ľ���ת����
	* �����μ��������ת��ʱ��dynamic_cast��static_cast��Ч����һ���ģ�
	*�ڽ�������ת��ʱ��dynamic_cast�������ͼ��Ĺ��ܣ���static_cast����ȫ��

	*/

	/*
	reinpreter_cast
	�÷���reinpreter_cast<type-id> (expression)
	˵����
					type-id������һ��ָ�롢���á��������͡�����ָ����߳�Աָ�롣
					�����԰�һ��ָ��ת����һ��������Ҳ���԰�һ������ת����һ��ָ��
				���Ȱ�һ��ָ��ת����һ���������ڰѸ�����ת����ԭ���͵�ָ�룬�����Եõ�ԭ�ȵ�ָ��ֵ����
	*/
	//�� char* ת��Ϊ float*
	char str[] = "http";
	float *p1 = reinterpret_cast<float*>(str);
	std::cout << *p1 << std::endl;
	//�� int ת��Ϊ int*
	int *p = reinterpret_cast<int*>(100);
	//�� A* ת��Ϊ int*
	p = reinterpret_cast<int*>(new A(25, 96));
	std::cout << *p << std::endl;
	/*
	const_cast
	�÷���const_cast<type_id> (expression)
	˵����������ȥ�����ʽ�� const ���λ� volatile ����
					����ָ�뱻ת���ɷǳ���ָ�룬������Ȼָ��ԭ���Ķ��󣻳������ñ�ת���ɷǳ������ã�������Ȼָ��ԭ���Ķ��󣻳�������ת���ɷǳ�������
	*/
	const int n = 100;
	int *ip = const_cast<int*>(&n);
	*ip = 234;
	std::cout << "n = " << n << std::endl;
	std::cout << "*ip = " << *ip << std::endl;

}