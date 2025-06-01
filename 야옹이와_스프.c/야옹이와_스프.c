#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include <time.h>

#define ROOM_WIDTH 10
#define HME_POS 1
#define BWL_PO (ROOM_WIDTH - 2)

void printStatus(int soupCount, int level);//�������
void interaction(char name[], int* level);//��ȣ�ۿ�
void room(int cat,int previous);//��׸���
void moveCat(char name[], int* cat, int level);//�̵�
void action(char name[], int cat, int* soup);//�ൿ

int main(void) {
	srand((unsigned int)time(NULL));
	char name[20];

	printf("****�߿��̿� ����****\n");

	printf("        /\\_/\\\n");
	printf("  /\\   / o o \\\n");
	printf(" //\\\\  \\~(*)~/\n");
	printf(" `   \\/   ^ /\n");
	printf("     | \\|| ||\n");
	printf("     \\ `|| ||\n");
	printf("      \\)()-())\n");

	printf("�߿����� �̸��� ���� �ּ���:");
	fgets(name, sizeof(name), stdin);
	name[strcspn(name, "\n")] = '\0';

	printf("�߿����� �̸��� %s�Դϴ�.\n", name);

	Sleep(1000);
	system("cls");

	int soup = 0, level = 2;
	int cat = 1, previous = 0;

	printStatus(soup, level);
	room(cat, previous);

	while (1) {
		interaction(name, &level);
		previous = cat;
		moveCat(name, &cat, level);
		room(cat, previous);
		action(name, cat, &soup);

		Sleep(2500);
		system("cls");
		printStatus(soup, level);
		room(cat, previous);
	}

	return 0;
}

void printStatus(int soupCount, int level) {
	const char* messages[] = {
		"0: �翡 ���� �͵� �Ⱦ��մϴ�.",
		"1: ���� ���Ǳ� ����Դϴ�.",
		"2: �׷����� �� ���� �����Դϴ�.",
		"3: �Ǹ��� ����� �����ް� �ֽ��ϴ�.",
		"4: ���� �������Դϴ�."
	};

	printf("=============== ���� ���� ===============\n");
	printf("������� ���� ����: %d��\n", soupCount);
	printf("������� ����(0~4): %d\n", level);
	printf("%s\n", messages[level]);
	printf("=========================================\n");
	
	Sleep(500);
}

void interaction(char name[], int* level) {
	int choice;
	printf("\n� ��ȣ�ۿ��� �Ͻðڽ��ϱ�?  0. �ƹ��͵� ���� ���� 1. �ܾ��ֱ�\n");
	while (1) {
		printf(">>");
		scanf_s("%d", &choice);

		if (choice == 0 || choice == 1) break;
	}

	int dice = rand() % 6 + 1;

	if (choice == 0) {
		printf("�ƹ��͵� ���� �ʽ��ϴ�.\n");
		printf("4/6�� Ȯ���� ģ�е��� �������ϴ�.\n");
		printf("�ֻ����� �����ϴ�. �Ǹ���...\n");
		printf("%d��(��) ���Խ��ϴ�!\n", dice);

		if (dice <= 4 && *level > 0) {
			(*level)--;
			printf("ģ�е��� �������ϴ�.\n");
		}
		else {
			printf("������ ģ�е��� �������� �ʾҽ��ϴ�.\n");
		}
	}
	else if (choice == 1) {
		printf("%s�� ���� �ܾ��־����ϴ�.\n", name);
		printf("2/6�� Ȯ���� ģ�е��� �������ϴ�.\n");
		printf("�ֻ����� �����ϴ�. �Ǹ���...\n");
		printf("%d��(��) ���Խ��ϴ�!\n", dice);

		if (dice >= 5 && *level < 4) {
			(*level)++;
			printf("ģ�е��� �������ϴ�.\n");
		}
		else {
			printf("ģ�е��� �״���Դϴ�.\n");
		}
	}
	printf("���� ģ�е� : %d\n", *level);
	printf("\n");

	Sleep(500);
}

void room(int cat, int previous) {
	for (int i = 0; i < ROOM_WIDTH; i++) {
		printf("#");
	}
	printf("\n");

	printf("#");
	for (int i = 1; i < ROOM_WIDTH-1; i++) {
		if (i == HME_POS) printf("H");
		else if (i == BWL_PO) printf("B");
		else printf(" ");
	}
	printf("#\n");

	printf("#");
	for (int i = 1; i < ROOM_WIDTH-1; i++) {
		if (i == cat) printf("C");
		else if (i == previous) printf(".");
		else printf(" ");
	}
	printf("#\n");

	for (int i = 0; i < ROOM_WIDTH; i++) {
		printf("#");
	}
	printf("\n");

	Sleep(500);
}

void moveCat(char name[], int* cat, int level) {
	int dice = rand() % 6 + 1;

	printf("\n%s �̵�: ����� ģ���Ҽ��� ���� ������ �� Ȯ���� �������ϴ�.\n", name);
	printf("�ֻ��� ���� 3 �̻��̸� ���� ������ �̵��մϴ�.\n");
	printf("�ֻ����� �����ϴ�. �Ǹ���...\n");
	printf("%d��(��) ���Խ��ϴ�!\n", dice);
	
	if (dice >= (6 - level)) {
		if (*cat < ROOM_WIDTH - 2) {
			(*cat)++;
			printf("���� ������ �����Դϴ�.\n");
		}
		else {
			printf("���Դϴ�.\n");
		}
	}
	else {
		if (*cat > 1) {
			(*cat)--;
			printf("�� ������ �����Դϴ�.\n");
		}
		else {
			printf("���Դϴ�.\n");
		}
	}

	Sleep(500);
}

void action(char name[], int cat, int* soup) {
	if (cat == HME_POS) {
		printf("%s(��)�� �ڽ��� ������ ������� �����ϴ�.\n", name);
	}
	else if (cat == BWL_PO) {
		int soupnum = rand() % 3;

		printf("%s(��)�� ", name);
		switch (soupnum) {
		case 0:
			printf("���� ����"); break;
		case 1:
			printf("����� ����"); break;
		case 2:
			printf("����ݸ� ����"); break;
		}
		
		printf("�� ��������ϴ�!\n");
		(*soup)++;
	}

	Sleep(500);
}