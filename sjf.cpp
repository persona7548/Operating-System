#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Process {
	char name[10];
	int cpu_data[100]; //���μ����� ���ڵ����͵��� ��Ƴ��� �迭 0 = Arrvial / Ȧ�� = cpu / ¦�� = I/O
	int now = 1; //���� ��ġ
	int data_num,waiting_time=0,total_time=-1,aging=0;
}Process;
int getIntNum(char *str) { //�ؽ�Ʈ���Ͽ��� ���ڸ� �����ϴ� �Լ�
	int len,num;
	len = strlen(str);
	for (int i = 0; i<len; i++)
	{
		if ('0' <= str[i] && str[i] <= '9')
		{num = (int)str[i]-'0';}
	}
	return num;
}
void read_p(Process *P, char* arr) {
	char *check;
	int j = 0;
	strcpy(P->name,strtok(arr, "\t"));
	check = strtok(NULL, "\t");
	do
	{
		P->cpu_data[j] = getIntNum(check);
		P->total_time += P->cpu_data[j];
		j++;
	} while (check =strtok(NULL, "\t\n"));
	P->data_num = j;
}
int run(int time,Process *P,int end_p_num,bool *run_check) {
	P->cpu_data[P->now]--;
	//printf("run : %s	%d\n",P->name, P->cpu_data[P->now]);

	if (P->cpu_data[P->now] == 0)
	{
		P->now++;
		*run_check = true;
	}
	if (P->now == P->data_num) {
		end_p_num++;
		P->waiting_time = time;
		P->now = -1;
		P->cpu_data[0] = 1000;
	}

	return end_p_num;
}
int short_cpu(int p_num,int time, Process P[]) { //burst�ð��� ���� ª�� ���μ����� ã�� �Լ�
	int short_burst=500;
	int short_p = -1;
	int short_aging = 0;
	for (int i = 0; i < p_num; i++) {
		if (P[i].now != -1 && P[i].cpu_data[0] <= time && P[i].now % 2 != 0) {
			if (P[i].cpu_data[P[i].now] < short_burst) {
				short_burst = P[i].cpu_data[P[i].now];
				short_p = i;
				short_aging = P[i].aging;
			}
			else if (P[i].cpu_data[P[i].now] == short_burst)	//����Ʈ���� ������ Readyť�� �� �����ִ� ���μ����� ����
				if (P[i].aging > short_aging) {
					short_burst = P[i].cpu_data[P[i].now];
					short_p = i;
					short_aging = P[i].aging;
				}
		}
	}
	return short_p;
}
void SJF(Process *P, int p_num) {
	int run_p; //���� Run������ ���μ���, ���μ����� ����
	int end_p_num = 0; //��������� ���μ��� ����
	int time = 0;
	bool run_check = true; //true�ϋ� run �ڸ��� �������

	while (p_num != end_p_num)
	{
		//printf("%d��\n ", time);
		for (int i = 0; i < p_num; i++) {

			if (P[i].cpu_data[0] == time) {							//��Ȳ 1: Ready�� ��������
				if (run_check) {
					run_p = i;
					run_check = false;
				}
				else {
					run_p = P[i].cpu_data[P[i].now] >= P[run_p].cpu_data[P[run_p].now] ? run_p : i;
				}
			}
			if (run_check)											//��Ȳ 2: Run�� ���������
			{
				run_p = short_cpu(p_num, time, P);
				run_check = false;
				if (run_p == -1)
					run_check = true;
			}

			if (P[i].now % 2 == 0 && P[i].cpu_data[P[i].now] == 0) { //��Ȳ 3: I/O �����
				P[i].now++;
				P[i].aging = 0;
				if (run_check) {
					run_p = i;
					run_check = false;
				}
				else
					run_p = P[i].cpu_data[P[i].now] >= P[run_p].cpu_data[P[run_p].now] ? run_p : i;
			}
			if (P[i].now % 2 == 0)									//I/O�����ϋ� I/O�� �Һ��ϴ� �Լ�
				P[i].cpu_data[P[i].now]--;
			P[i].aging++;											//Readyť�� ���μ����� ��ȭ��Ű�� ����
		}
		if (run_check == false)
			end_p_num = run(time, &P[run_p], end_p_num, &run_check);
		time++;
	}
}
int main(int argc, char** argv) {
	char *filename = argv[1];
	char tmp[1000];
	FILE *fp;
	int p_num=0;
	float avr_time = 0;
	Process P[10];

	if (!(fp = fopen(filename, "r"))) { printf("ERROR: file open\n"); exit(0); }
	while (fgets(tmp, 1000, fp)) {
		printf("%s",tmp);
		read_p(&P[p_num],tmp);
		p_num++;
	}
	fclose(fp);
	SJF(P, p_num);

	printf("\n--------------------------\n");
	for (int i = 0; i < p_num; i++) {
		printf("%s	: %3d\n", P[i].name, P[i].waiting_time - P[i].total_time);
		avr_time += P[i].waiting_time - P[i].total_time;
	}
	printf("--------------------------\n");
	printf("Average Waiting Time	: %3.2f\n",avr_time/p_num);

	return 0;
}