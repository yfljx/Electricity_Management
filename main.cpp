#include<iostream>
#include<algorithm>

using namespace std;

constexpr auto MAX_RECORDS_NUM = 500;    // ����׼�¼��
int user_num = 0;   //�û�����

//��Ѽ�¼
struct transaction {
	double electricity_consumption;           //����
	double shared_electricity_consumption; //��̯
	double subsidy_electority_consumption; // ����
	double sum_electority_consumption;    //�ܵ���
	double money;                                   //���

	transaction() {}
	transaction(transaction* t) {         //����
		electricity_consumption = t->electricity_consumption;
		shared_electricity_consumption = t->shared_electricity_consumption;
		subsidy_electority_consumption = t->subsidy_electority_consumption;
		sum_electority_consumption = t->sum_electority_consumption;
		money = t->money;
	}
};

struct record {
	char user_name[20];
	char electricity_billing_number[20];
	transaction pre_transaction;      //����
	transaction cur_transaction;      //����
	record* next = nullptr;

	record() {}
	record(record* r) {
		strcpy_s(user_name, r->user_name);
		strcpy_s(electricity_billing_number, r->electricity_billing_number);
		pre_transaction = transaction(&(r->pre_transaction));
		cur_transaction = transaction(&(r->cur_transaction));
		next = r->next;
	}
};

record recs[MAX_RECORDS_NUM];   //��������

//�������
void calculate_money(transaction* t) {
	t->sum_electority_consumption = t->electricity_consumption + t->shared_electricity_consumption;
	double pay_electority_consumption = t->sum_electority_consumption - t->subsidy_electority_consumption;
	double sum = 0;
	if (pay_electority_consumption < 180) {
		sum += 0.47 * pay_electority_consumption;
	}
	else {
		if (pay_electority_consumption < 280) {
			sum += 0.47 * 180 + (pay_electority_consumption - 180) * 0.52;
		}
		else {
			sum += 0.47 * 180 + 100 * 0.52 + (pay_electority_consumption - 280) * 0.82;
		}
	}
	t->money = sum;
}

//����
void insert_record(record* head) {
	record* r = nullptr;
	while (r == nullptr) {
		r = (record*)malloc(sizeof(record));
	}
	if (r != nullptr) {
		cout << "�������û���(������20���ַ�):" << endl;
		cin >> r->user_name;

		cout << "����������ƷѺ�(9λ����):" << endl;
		cin >> r->electricity_billing_number;
		cout << "���������µ����Ϣ:" << endl;
		cout << "�õ���\t��̯�õ���\t�����õ���:" << endl;
		cin >> r->pre_transaction.electricity_consumption >> r->pre_transaction.shared_electricity_consumption >> r->pre_transaction.subsidy_electority_consumption;
		calculate_money(&(r->pre_transaction));

		cout << "�����뱾�µ����Ϣ:" << endl;
		cout << "�õ���\t��̯�õ���\t�����õ���:" << endl;
		cin >> r->cur_transaction.electricity_consumption >> r->cur_transaction.shared_electricity_consumption >> r->cur_transaction.subsidy_electority_consumption;
		calculate_money(&(r->cur_transaction));
		cout << "�������." << endl;
	}
	else {
		return;
	}
	record* p = head;
	for (int i = 0; i < user_num; i++)p = p->next;
	p->next = r;
	user_num += 1;
	return;
}

//�����û���ɾ�����׼�¼
void delete_record(record* head) {
	record* cur = head->next;
	record* pre = head;
	cout << "����Ҫɾ�����û�(�û���,������20λ):" << endl;
	char username[20];
	cin >> username;
	for (int i = 0; i < user_num; i++) {
		if (strcmp(cur->user_name, username) == 0) {
			cout << "Ҫɾ���ļ�¼����:" << endl;
			cout << "�����ƷѺ�(9λ����):";
			cout << cur->electricity_billing_number << endl;
			cout << "��¼��ɾ��." << endl;
			pre->next = cur->next;
			cur = nullptr;
			user_num -= 1;
			return;
		}
		pre = cur;
		cur = cur->next;  //����   
	}
	cout << "���û�������." << endl;
}

//����
void  find_record(record* head) {
	record* cur = head->next;
	cout << "����Ҫ���ҵ��û�(�û���,������20λ):" << endl;
	char username[20];
	cin >> username;
	for (int i = 0; i < user_num; i++) {
		if (strcmp(cur->user_name, username) == 0) {
			cout << "���ҵ��ļ�¼Ϊ����:" << endl;
			cout << "�����ƷѺ�(9λ����):";
			cout << cur->electricity_billing_number << endl;;
			cout << "���µ�����Ϣ:" << endl;
			cout << "�õ���\t��̯�õ���\t�����õ���:" << endl;
			cout << cur->pre_transaction.electricity_consumption << "\t" << cur->pre_transaction.shared_electricity_consumption << "\t" << cur->pre_transaction.subsidy_electority_consumption << endl;

			cout << "���µ�����Ϣ:" << endl;
			cout << "�õ���\t��̯�õ���\t�����õ���:" << endl;
			cout << cur->cur_transaction.electricity_consumption << "\t" << cur->cur_transaction.shared_electricity_consumption << "\t" << cur->cur_transaction.subsidy_electority_consumption << endl;

			cout << "���ҽ���." << endl;

			return;
		}
		cur = cur->next;  //����   
	}
	cout << "Ҫ���ҵ��û�������." << endl;
}

// ����ÿ����ƽ���õ��������õ���
void calculate_total_average(record* head) {
	double sum = 0;
	record* cur = head->next;
	cout << "����ÿ����ƽ���õ��������õ���:" << endl;
	cout << "�û�\t���õ���" << endl;
	for (int i = 0; i < user_num; i++)
	{
		double total = cur->pre_transaction.sum_electority_consumption + cur->cur_transaction.sum_electority_consumption;
		cout << cur->user_name << "\t" << total << endl;
		sum += total;
		cur = cur->next;
	}
	cout << "ƽ���õ���:" << sum / user_num << endl;
}

//����ÿ������õ��û�
void sort_by_month(record* head) {
	record* cur = head->next;

	char* pre_user_name = nullptr;
	char* cur_user_name = nullptr;
	double max_pre_electority_consumption = 0;
	double max_cur_electority_consumption = 0;
	double pre_money = 0;
	double cur_money = 0;
	for (int i = 0; i < user_num; i++) {
		if (cur->pre_transaction.sum_electority_consumption > max_pre_electority_consumption) {
			max_pre_electority_consumption = cur->pre_transaction.sum_electority_consumption;
			pre_user_name = cur->user_name;
			pre_money = cur->pre_transaction.money;
		}
		if (cur->cur_transaction.sum_electority_consumption > max_cur_electority_consumption) {
			max_cur_electority_consumption = cur->cur_transaction.sum_electority_consumption;
			cur_user_name = cur->user_name;
			cur_money = cur->cur_transaction.money;
		}
		cur = cur->next;
	}
	cout << "�����õ��������û���:" << pre_user_name << endl;
	cout << "�õ���Ϊ:" << max_pre_electority_consumption << ",���Ϊ:" << pre_money << endl;;

	cout << "�����õ��������û���:" << cur_user_name << endl;
	cout << "�õ���Ϊ:" << max_cur_electority_consumption << ",���Ϊ:" << cur_money << endl;
}

//�ȽϺ���
bool cmp(const record& r1, const record& r2) {
	return (r1.pre_transaction.money + r1.cur_transaction.money) > (r2.pre_transaction.money + r2.cur_transaction.money);
}

//���ݷ�������
void sort_by_total_money(record* head) {
	record* cur = head->next;
	int num = 0;
	for (int i = 0; i < user_num; i++) {
		recs[num++] = record(cur);
		cur = cur->next;
	}

	sort(recs, recs + num, cmp);

	cout << "��ѴӸߵ�����������:" << endl;
	cout << "����\t�û�\t�ܵ��" << endl;
	for (int i = 0; i < num; i++) {
		cout << i + 1 << "\t" << recs[i].user_name << "\t" << recs[i].pre_transaction.money + recs[i].cur_transaction.money << endl;
	}
	cout << "�������." << endl;
}

// ������
int main() {
	cout << "======�����Ʒѹ���ϵͳ======" << endl;
	cout << "�밴������ʾ����:" << endl;
	cout << "1.�����¼(i)" << endl
		<< "2.ɾ����¼(d)" << endl
		<< "3.���Ҽ�¼(f)" << endl
		<< "4.ÿ������ƽ���õ��������õ���(v)" << endl
		<< "5.ÿ���õ��������û�(m)" << endl
		<< "6.�ܵ�ѴӸߵ�������(s)" << endl
		<< "7.�뿪��ϵͳ(e)" << endl
		<< endl;
	cout << "��������Ĳ���:" << endl;
	char opt;
	cin >> opt;
	record* head = (record*)malloc(sizeof(record));
	while (opt != 'e')
	{
		switch (opt)
		{
		case 'i': {
			insert_record(head);
			break;
		}
		case 'd': {
			delete_record(head);
			break;
		}
		case 'f': {
			find_record(head);
			break;
		}
		case 'v': {
			calculate_total_average(head);
			break;
		}
		case 'm': {
			sort_by_month(head);
			break;
		}
		case 's': {
			sort_by_total_money(head);
			break;
		}
		default: {
			cout << "û�����ѡ��,��������������ѡ��:" << endl;
			cin >> opt;
			break;
		}
		}
		cout << "��������ѡ��:" << endl;
		cin >> opt;
	}
	free(head);
	return 0;
}

