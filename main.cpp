#include<iostream>
#include<algorithm>

using namespace std;

constexpr auto MAX_RECORDS_NUM = 500;    // 最大交易记录数
int user_num = 0;   //用户数量

//电费记录
struct transaction {
	double electricity_consumption;           //电量
	double shared_electricity_consumption; //分摊
	double subsidy_electority_consumption; // 补助
	double sum_electority_consumption;    //总电量
	double money;                                   //电费

	transaction() {}
	transaction(transaction* t) {         //构造
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
	transaction pre_transaction;      //上月
	transaction cur_transaction;      //本月
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

record recs[MAX_RECORDS_NUM];   //用于排序

//计算费用
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

//插入
void insert_record(record* head) {
	record* r = nullptr;
	while (r == nullptr) {
		r = (record*)malloc(sizeof(record));
	}
	if (r != nullptr) {
		cout << "请输入用户名(不超过20个字符):" << endl;
		cin >> r->user_name;

		cout << "请输入电量计费号(9位数字):" << endl;
		cin >> r->electricity_billing_number;
		cout << "请输入上月电费信息:" << endl;
		cout << "用电量\t公摊用电量\t补助用电量:" << endl;
		cin >> r->pre_transaction.electricity_consumption >> r->pre_transaction.shared_electricity_consumption >> r->pre_transaction.subsidy_electority_consumption;
		calculate_money(&(r->pre_transaction));

		cout << "请输入本月电费信息:" << endl;
		cout << "用电量\t公摊用电量\t补助用电量:" << endl;
		cin >> r->cur_transaction.electricity_consumption >> r->cur_transaction.shared_electricity_consumption >> r->cur_transaction.subsidy_electority_consumption;
		calculate_money(&(r->cur_transaction));
		cout << "输入结束." << endl;
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

//根据用户名删除交易记录
void delete_record(record* head) {
	record* cur = head->next;
	record* pre = head;
	cout << "输入要删除的用户(用户名,不超过20位):" << endl;
	char username[20];
	cin >> username;
	for (int i = 0; i < user_num; i++) {
		if (strcmp(cur->user_name, username) == 0) {
			cout << "要删除的记录如下:" << endl;
			cout << "电量计费号(9位数字):";
			cout << cur->electricity_billing_number << endl;
			cout << "记录已删除." << endl;
			pre->next = cur->next;
			cur = nullptr;
			user_num -= 1;
			return;
		}
		pre = cur;
		cur = cur->next;  //更新   
	}
	cout << "该用户不存在." << endl;
}

//查找
void  find_record(record* head) {
	record* cur = head->next;
	cout << "输入要查找的用户(用户名,不超过20位):" << endl;
	char username[20];
	cin >> username;
	for (int i = 0; i < user_num; i++) {
		if (strcmp(cur->user_name, username) == 0) {
			cout << "查找到的记录为如下:" << endl;
			cout << "电量计费号(9位数字):";
			cout << cur->electricity_billing_number << endl;;
			cout << "上月电量信息:" << endl;
			cout << "用电量\t公摊用电量\t补助用电量:" << endl;
			cout << cur->pre_transaction.electricity_consumption << "\t" << cur->pre_transaction.shared_electricity_consumption << "\t" << cur->pre_transaction.subsidy_electority_consumption << endl;

			cout << "本月电量信息:" << endl;
			cout << "用电量\t公摊用电量\t补助用电量:" << endl;
			cout << cur->cur_transaction.electricity_consumption << "\t" << cur->cur_transaction.shared_electricity_consumption << "\t" << cur->cur_transaction.subsidy_electority_consumption << endl;

			cout << "查找结束." << endl;

			return;
		}
		cur = cur->next;  //更新   
	}
	cout << "要查找的用户不存在." << endl;
}

// 计算每个户平均用电量和总用电量
void calculate_total_average(record* head) {
	double sum = 0;
	record* cur = head->next;
	cout << "计算每个户平均用电量和总用电量:" << endl;
	cout << "用户\t总用电量" << endl;
	for (int i = 0; i < user_num; i++)
	{
		double total = cur->pre_transaction.sum_electority_consumption + cur->cur_transaction.sum_electority_consumption;
		cout << cur->user_name << "\t" << total << endl;
		sum += total;
		cur = cur->next;
	}
	cout << "平均用电量:" << sum / user_num << endl;
}

//查找每月最大用电用户
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
	cout << "上月用电量最多的用户是:" << pre_user_name << endl;
	cout << "用电量为:" << max_pre_electority_consumption << ",电费为:" << pre_money << endl;;

	cout << "本月用电量最多的用户是:" << cur_user_name << endl;
	cout << "用电量为:" << max_cur_electority_consumption << ",电费为:" << cur_money << endl;
}

//比较函数
bool cmp(const record& r1, const record& r2) {
	return (r1.pre_transaction.money + r1.cur_transaction.money) > (r2.pre_transaction.money + r2.cur_transaction.money);
}

//根据费用排序
void sort_by_total_money(record* head) {
	record* cur = head->next;
	int num = 0;
	for (int i = 0; i < user_num; i++) {
		recs[num++] = record(cur);
		cur = cur->next;
	}

	sort(recs, recs + num, cmp);

	cout << "电费从高到底排序如下:" << endl;
	cout << "排名\t用户\t总电费" << endl;
	for (int i = 0; i < num; i++) {
		cout << i + 1 << "\t" << recs[i].user_name << "\t" << recs[i].pre_transaction.money + recs[i].cur_transaction.money << endl;
	}
	cout << "排序结束." << endl;
}

// 主函数
int main() {
	cout << "======电量计费管理系统======" << endl;
	cout << "请按以下提示操作:" << endl;
	cout << "1.插入记录(i)" << endl
		<< "2.删除记录(d)" << endl
		<< "3.查找记录(f)" << endl
		<< "4.每个户的平均用电量和总用电量(v)" << endl
		<< "5.每月用电量最多的用户(m)" << endl
		<< "6.总电费从高到低排序(s)" << endl
		<< "7.离开此系统(e)" << endl
		<< endl;
	cout << "请输入你的操作:" << endl;
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
			cout << "没有这个选项,请重现输入您的选择:" << endl;
			cin >> opt;
			break;
		}
		}
		cout << "输入您的选择:" << endl;
		cin >> opt;
	}
	free(head);
	return 0;
}

