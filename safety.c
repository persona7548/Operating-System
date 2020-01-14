#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int Allocation[100][100], Max[100][100], Available[100], Need[100][100];
int Finish[100], Work[100],safe_p[100], unsafe_p[100];
char P_name[100][100];
int n, m;

int getIntNum(char *str) { //텍스트파일에서 숫자만 추출하는 함수
	int len, num;
	len = strlen(str);
	for (int i = 0; i < len; i++)
		if ('0' <= str[i] && str[i] <= '9') {
			if ('0' <= str[i -1] && str[i -1] <= '9')
				num = ((int)str[i-1] - '0') * 10 + (int)str[i] - '0';
			else
				num = (int)str[i] - '0';
		}
	return num;
}
void read_p(char *filename) {
	
	FILE *fp;
	char tmp[1000];

	if (!(fp = fopen(filename, "r"))) { printf("ERROR: file open\n"); exit(0); }
	fgets(tmp, 1000, fp);

	n = getIntNum(strtok(tmp," "));
	m = getIntNum(strtok(NULL," "));
	printf("%d	%d\n\n", n,m);


	printf("%s", fgets(tmp, 1000, fp));
	printf("%s", fgets(tmp, 1000, fp));
	
	for (int i = 0; i < m; i++) {
		fgets(tmp, 1000, fp);
		printf("%s",tmp);
		strcpy(&P_name[i][0],strtok(tmp, " "));
		for(int j = 0;j < n;j++)
			Allocation[i][j] = getIntNum(strtok(NULL," "));
	}

	printf("%s", fgets(tmp, 1000, fp));
	printf("%s", fgets(tmp, 1000, fp));
	
	for (int i = 0; i < m; i++) {
		fgets(tmp, 1000, fp);
		printf("%s", tmp);
		strtok(tmp, "  ");
		for (int j = 0; j < n; j++)
			Max[i][j] = getIntNum(strtok(NULL, " "));
	}
	printf("%s", fgets(tmp, 1000, fp));
	printf("%s", fgets(tmp, 1000, fp));
	printf("%s", fgets(tmp, 1000, fp));

	Available[0]= getIntNum(strtok(tmp, " "));

	for (int i = 1; i < n; i++) 
		Available[i] = getIntNum(strtok(NULL, " "));
	for (int i = 0; i < m; i++) 
		for (int j = 0; j < n; j++)
			Need[i][j] = Max[i][j] - Allocation[i][j];
}
void safety_check() {
	int p = 0;
	for (int i = 0; i < m; i++)
		if (Finish[i] != 0) {
			unsafe_p[p] = i;
			p++;
		}
	printf("\n-------------------------------------------------------------------------------------\n");
	if (p == 0) {
		printf("Result: Safe --> ");
		for (int i = 0; i < m; i++)
			printf("%s ", P_name[safe_p[i]]);
	}
	else
	{
		printf("Result: UnSafe --> ");
		for (int i = 0; i < p; i++)
			printf("%s ", P_name[unsafe_p[i]]);
	}
	printf("\n-------------------------------------------------------------------------------------\n");
}
void Finish_check(int p) {
	int i, j;
	int bool_check;
	
	for (j = 0; j < m; j++) {
		bool_check = 0;
		for (i = 0; i < n; i++)
			if (Need[j][i] <= Work[i])
				bool_check++;

		if (Finish[j] == -1 && bool_check == n) {
			for (int i = 0; i < n; i++) {
				Work[i] += Allocation[j][i];
			}
			safe_p[p] = j;
			Finish[j] = 0;
			p++;
			Finish_check(p);
		}
	}
}
int main(int argc, char** argv) {
	int i;

	read_p(argv[1]);

	for (i = 0; i < n; i++)
		Work[i] = Available[i];
	for (i = 0; i < m; i++)
		Finish[i] = -1;

	Finish_check(0);
	safety_check();

	return 0;
}