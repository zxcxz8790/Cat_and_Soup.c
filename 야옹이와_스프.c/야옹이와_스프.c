#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include <time.h>

#define ROOM_WIDTH 10
#define HME_POS 1
#define BWL_POS (ROOM_WIDTH - 2)

void printStatus(int soupCount, int level, int cp, char name[], int mood);//�������
void interaction(char name[], int* level, int* mood, int hasMouse, int hasLaser);//2-5 ��ȣ�ۿ� �ɼ� �߰�, 2-6 �ɼǺ� ģ�е�,��� ����
void room(int cat,int previous);//��׸���
void moveCat(char name[], int* cat, int mood);//2-3 �̵�v2
void action(char name[], int cat, int previous, int* soup, int* mood);//2-4 ��� ���� �ൿ
void badMood(char name[], int level, int* mood);//2-2 ��� ������
void store(int* cp, int* hasMouse, int* hasLaser);//2-8 ����
void setScratcherPosition();//2-8 ������ ��ũ���� ��ġ ����
void setTowerPosition();//2-8 ������ Ÿ�� ��ġ ����

int towerPos = -1, scratcherPos = -1; //2-1 ��׸��� ���� ��ġ ����

int main(void) {
	srand((unsigned int)time(NULL));
	char name[20];

	int hasMouse = 0, hasLaser = 0;

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
	int mood = 3, cp = 0;

	printStatus(soup, level, cp, name, mood);
	room(cat, previous);

	while (1) {
		badMood(name, level, &mood);
		interaction(name, &level, &mood, hasMouse, hasLaser);
		previous = cat;
		moveCat(name, &cat, mood);
		if (mood == 1 && towerPos == -1 && scratcherPos == -1) mood = 0;
		room(cat, previous);
		action(name, cat, previous, &soup, &mood);

		int cp_add = (mood > 1 ? mood - 1 : 0) + level;//2-7 CP ����
		cp += cp_add;
		printf("%s�� ��а� ģ�е��� ���� CP�� %d ����Ʈ ����Ǿ����ϴ�.\n", name, cp_add);
		printf("���� CP: %d ����Ʈ\n", cp);
		Sleep(1000);
		
		store(&cp, &hasMouse, &hasLaser);

		Sleep(2500);
		system("cls");
		printStatus(soup, level, cp, name, mood);
		room(cat, previous);
	}

	return 0;
}

void printStatus(int soupCount, int level, int cp, char name[], int mood) {//2-1 ����â ����, CP,��� ���
	const char* levelMessages[] = {
		"0: �翡 ���� �͵� �Ⱦ��մϴ�.",
		"1: ���� ���Ǳ� ����Դϴ�.",
		"2: �׷����� �� ���� �����Դϴ�.",
		"3: �Ǹ��� ����� �����ް� �ֽ��ϴ�.",
		"4: ���� �������Դϴ�."
	};

	const char* moodMessages[] = {
		"0: ����� �ſ� ���޴ϴ�.",
		"1: �ɽ������ϴ�.",
		"2: �Ļ��� �����ϴ�.",
		"3: ������ �θ��ϴ�."
	};

	printf("=============== ���� ���� ===============\n");
	printf("������� ���� ����: %d��\n", soupCount);
	printf("CP : %d ����Ʈ\n", cp);
	printf("%s �� ��� (0~3) : %d\n", name, mood);
	printf("\t%s\n",moodMessages[mood]);
	printf("������� ģ�е�(0~4): %d\n", level);
	printf("\t%s\n", levelMessages[level]);
	printf("=========================================\n");
	
	Sleep(500);
}

void interaction(char name[], int* level, int* mood, int hasMouse, int hasLaser) {//2-5 ��ȣ�ۿ� �ɼ� �߰�, 2-6 �ɼǺ� ģ�е�,��� ����
	int choice;
	int maxOption = 1;
	int laserOption = -1, mouseOption = -1;

	printf("\n� ��ȣ�ۿ��� �Ͻðڽ��ϱ�?\n");
	printf("0. �ƹ��͵� ���� ����\n");
	printf("1. �ܾ��ֱ�\n");

	int nextOption = 2;

	if (hasMouse) {
		printf("%d. �峭�� ��� ����ֱ�\n", nextOption);
		mouseOption = nextOption++;
	}

	if (hasLaser) {
		printf("%d. ������ �����ͷ� ����ֱ�\n", nextOption);
		laserOption = nextOption++;
	}

	maxOption = nextOption - 1;

	while (1) {
		printf(">> ");
		scanf_s("%d", &choice);
		if (choice >= 0 && choice <= maxOption) break;
	}

	int dice = rand() % 6 + 1;

	if (choice == 0) {
		printf("�ƹ��͵� ���� �ʽ��ϴ�.\n");

		if (*mood > 0) {
			printf("%s�� ����� ���������ϴ�: %d -> %d\n", name, *mood, *mood - 1);
			(*mood)--;
		}
		else {
			printf("�̹� ����� �����Դϴ�.\n");
		}

		printf("�ֻ����� �����ϴ�. �Ǹ���...\n");
		printf("%d��(��) ���Խ��ϴ�!\n", dice);
		if (dice <= 5 && *level > 0) {
			(*level)--;
			printf("������� ���谡 �������ϴ�.\n");
		}
		else {
			printf("ģ�е��� �״���Դϴ�.\n");
		}
	}

	else if (choice == 1) {
		printf("%s�� ���� �ܾ��־����ϴ�.\n", name);
		printf("�ֻ����� �����ϴ�. �Ǹ���...\n");
		printf("%d��(��) ���Խ��ϴ�!\n", dice);
		if (dice >= 5 && *level < 4) {
			(*level)++;
			printf("ģ�е��� �������ϴ�!\n");
		}
		else {
			printf("ģ�е��� �״���Դϴ�.\n");
		}
	}

	else if (choice == mouseOption) {
		printf("�峭�� ��� %s�� ����־����ϴ�.\n", name);

		if (*mood < 3) {
			printf("����� ���� ���������ϴ�: %d -> %d\n", *mood, *mood + 1);
			(*mood)++;
		}
		else {
			printf("����� �̹� �ְ��Դϴ�.\n");
		}

		printf("�ֻ����� �����ϴ�. �Ǹ���...\n");
		printf("%d��(��) ���Խ��ϴ�!\n", dice);
		if (dice >= 4 && *level < 4) {
			(*level)++;
			printf("ģ�е��� �������ϴ�!\n");
		}
		else {
			printf("ģ�е��� �״���Դϴ�.\n");
		}
	}

	else if (choice == laserOption) {
		printf("������ �����ͷ� %s�� �ų��� ����־����ϴ�.\n", name);

		int oldMood = *mood;
		*mood += 2;
		if (*mood > 3) *mood = 3;
		printf("����� �� ���������ϴ�: %d -> %d\n", oldMood, *mood);

		printf("�ֻ����� �����ϴ�. �Ǹ���...\n");
		printf("%d��(��) ���Խ��ϴ�!\n", dice);
		if (dice >= 2 && *level < 4) {
			(*level)++;
			printf("ģ�е��� �������ϴ�!\n");
		}
		else {
			printf("ģ�е��� �״���Դϴ�.\n");
		}
	}
	
	Sleep(1000);
}


void room(int cat, int previous) {
	for (int i = 0; i < ROOM_WIDTH; i++) {
		printf("#");
	}
	printf("\n");

	printf("#");
	for (int i = 1; i < ROOM_WIDTH-1; i++) {
		if (i == HME_POS) printf("H");
		else if (i == BWL_POS) printf("B");
		else if (towerPos != -1 && i == towerPos) printf("T");//2-8 ������ Ÿ�� ���
		else if (scratcherPos != -1 && i == scratcherPos) printf("S");//2-8 ������ ��ũ���� ���
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

void moveCat(char name[], int* cat, int mood) {//2-3 ��п� ���� �̵��ϴ� ��� ����
	if (mood == 0) {
		printf("����� �ſ� ���� %s��(��) ������ ���մϴ�.\n", name);
		if (*cat > HME_POS) {
			(*cat)--;
			printf("�� ������ �� ĭ �̵��߽��ϴ�.\n");
		}
		else {
			printf("�̹� ���� ������ ���ڸ��� �ֽ��ϴ�.\n");
		}
	}
	else if (mood == 1) {
		if (towerPos == -1 && scratcherPos == -1) {
			printf("�� �Ÿ��� ��� %s�� ����� �ſ� �������ϴ�: 1 -> 0\n", name);
		}
		else {
			int targetPos;
			const char* targetName;

			if (towerPos != -1 && scratcherPos != -1) {
				int d1 = abs(*cat - towerPos);
				int d2 = abs(*cat - scratcherPos);
				if (d1 <= d2) {
					targetPos = towerPos;
					targetName = "ĹŸ��";
				}
				else {
					targetPos = scratcherPos;
					targetName = "��ũ��ó";
				}
			}
			else if (towerPos != -1) {
				targetPos = towerPos;
				targetName = "ĹŸ��";
			}
			else {
				targetPos = scratcherPos;
				targetName = "��ũ��ó";
			}

			printf("%s��(��) �ɽ��ؼ� %s ������ �̵��մϴ�.\n", name, targetName);

			if (*cat < targetPos) (*cat)++;
			else if (*cat > targetPos) (*cat)--;
			else printf("�̹� %s�� ������ ���ڸ��� �ֽ��ϴ�.\n", targetName);
		}
	}
	else if (mood == 2) {
		printf("%s��(��) ��� ���� �Ļ��� ���� �ֽ��ϴ�.\n", name);
	}
	else if (mood == 3) {
		printf("%s��(��) ������ �θ��� ������ ���鷯 ���ϴ�.\n", name);
		if (*cat < BWL_POS) {
			(*cat)++;
			printf("���� ������ �� ĭ �̵��߽��ϴ�.\n");
		}
		else {
			printf("�̹� ���� ������ ���ڸ��� �ֽ��ϴ�.\n");
		}
	}

	Sleep(500);
}


void action(char name[], int cat, int previous, int* soup, int* mood) { //2-4 ��ġ�� ��� ����
	if (cat == HME_POS && previous == HME_POS) {
		if (*mood < 3) {
			printf("%s(��)�� ������ ����� ���� ����� �������ϴ�: %d -> %d\n", name, *mood, *mood + 1);
			(*mood)++;
		}
		else {
			printf("%s(��)�� ������ ����� ���� �ֽ��ϴ�.\n", name);
		}
	}
	else if (cat == BWL_POS) {
		int soupnum = rand() % 3;

		printf("%s(��)�� ", name);
		switch (soupnum) {
		case 0: printf("���� ����"); break;
		case 1: printf("����� ����"); break;
		case 2: printf("����ݸ� ����"); break;
		}
		printf("�� ��������ϴ�!\n");
		(*soup)++;
	}
	else if (cat == scratcherPos && scratcherPos != -1) {
		if (*mood < 3) {
			printf("%s(��)�� ��ũ��ó�� �ܰ� ��ҽ��ϴ�. ����� ���� ���������ϴ�: %d -> %d\n", name, *mood, *mood + 1);
			(*mood)++;
		}
		else {
			printf("%s(��)�� ��ũ��ó�� ������ �̹� �ſ� ����� �����ϴ�.\n", name);
		}
	}
	else if (cat == towerPos && towerPos != -1) {
		int increase = (*mood <= 1) ? 2 : (3 - *mood);
		if (increase > 0) {
			printf("%s(��)�� ĹŸ���� �پ�ٴմϴ�. ����� ���� ���������ϴ�: %d -> %d\n", name, *mood, *mood + increase);
			(*mood) += increase;
		}
		else {
			printf("%s(��)�� ĹŸ������ ��̰� ��� �ֽ��ϴ�.\n", name);
		}
	}

	Sleep(1000);
}

//2-1 ���� ��ġ, 2-8 ���� ��ɿ� ���� ǰ�񸶴� ���������� ����ǵ��� ����
void setScratcherPosition() {
	scratcherPos = -1;
	do {
		scratcherPos = rand() % (ROOM_WIDTH - 2) + 1;
	} while (scratcherPos == HME_POS || scratcherPos == BWL_POS || scratcherPos == towerPos);
}

void setTowerPosition() {
	towerPos = -1;
	do {
		towerPos = rand() % (ROOM_WIDTH - 2) + 1;
	} while (towerPos == HME_POS || towerPos == BWL_POS || towerPos == scratcherPos);
}

void badMood(char name[],int level, int *mood){//2-2 ��� ������
	printf("�ƹ� ���� ���� ����� �������ϴ�. ����̴ϱ�?\n");

	int limit = 6 - level;
	printf("6 - %d : �ֻ��� ���� %d �����̸� �׳� ����� �������ϴ�.\n",level,limit);

	int dice = rand() % 6 + 1;
	printf("�ֻ����� �����ϴ�. �Ǹ���...\n");
	printf("%d��(��) ���Խ��ϴ�!\n", dice);

	if (dice <= limit) {
		if (*mood > 0) {
			printf("%s�� ����� �������ϴ�: %d -> %d\n", name, *mood, *mood - 1);
			(*mood)--;
		}
		else {
			printf("�̹� ����� �����Դϴ�.\n");
		}
	}
	else {
		printf("����� �״���Դϴ�.\n");
	}

	Sleep(1000);
}

void store(int* cp, int* hasMouse, int* hasLaser) {//2-8 ����
	static int hasScratcher = 0, hasTower = 0;

	printf("\n�������� ������ �� �� �ֽ��ϴ�.\n");
	printf("���� CP: %d ����Ʈ\n", *cp);
	printf("� ������ �����ұ��?\n");
	printf("0. �ƹ� �͵� ���� �ʴ´�\n");
	printf("1. �峭�� ��: 1 CP %s\n", *hasMouse ? "(ǰ��)" : "");
	printf("2. ������ ������: 2 CP %s\n", *hasLaser ? "(ǰ��)" : "");
	printf("3. ��ũ��ó: 4 CP %s\n", hasScratcher ? "(ǰ��)" : "");
	printf("4. Ĺ Ÿ��: 6 CP %s\n", hasTower ? "(ǰ��)" : "");

	int choice;

	while (1) {
		printf(">> ");
		scanf_s("%d", &choice);

		if (choice == 0) {
			printf("�ƹ� �͵� �������� �ʾҽ��ϴ�.\n");
			break;
		}

		else if (choice == 1) {
			if (*hasMouse) {
				printf("�̹� �����߽��ϴ�.\n");
			}
			else if (*cp >= 1) {
				*hasMouse = 1;
				*cp -= 1;
				printf("�峭�� �㸦 �����߽��ϴ�. ���� CP: %d\n", *cp);
				break;
			}
			else {
				printf("CP�� �����մϴ�.\n");
			}
		}

		else if (choice == 2) {
			if (*hasLaser) {
				printf("�̹� �����߽��ϴ�.\n");
			}
			else if (*cp >= 2) {
				*hasLaser = 1;
				*cp -= 2;
				printf("������ �����͸� �����߽��ϴ�. ���� CP: %d\n", *cp);
				break;
			}
			else {
				printf("CP�� �����մϴ�.\n");
			}
		}

		else if (choice == 3) {
			if (hasScratcher) {
				printf("�̹� �����߽��ϴ�.\n");
			}
			else if (*cp >= 4) {
				hasScratcher = 1;
				*cp -= 4;
				setScratcherPosition();
				printf("��ũ��ó�� �����߽��ϴ�. ���� CP: %d\n", *cp);
				break;
			}
			else {
				printf("CP�� �����մϴ�.\n");
			}
		}

		else if (choice == 4) {
			if (hasTower) {
				printf("�̹� �����߽��ϴ�.\n");
			}
			else if (*cp >= 6) {
				hasTower = 1;
				*cp -= 6;
				setTowerPosition();
				printf("Ĺ Ÿ���� �����߽��ϴ�. ���� CP: %d\n", *cp);
				break;
			}
			else {
				printf("CP�� �����մϴ�.\n");
			}
		}
		else {
			printf("");
		}
	}

	Sleep(1000);
}
