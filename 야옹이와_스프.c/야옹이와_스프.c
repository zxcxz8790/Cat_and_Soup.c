#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include <time.h>

#define ROOM_WIDTH 10
#define HME_POS 1
#define BWL_POS (ROOM_WIDTH - 2)

void printStatus(int soupCount, int level, int cp, char name[], int mood);//상태출력
void interaction(char name[], int* level);//상호작용
void room(int cat,int previous);//방그리기
void moveCat(char name[], int* cat, int level);//이동
void action(char name[], int cat, int* soup);//행동

int towerPos = -1, scratcherPos = -1; //2-1 방그리기 현재 위치 미정

int main(void) {
	srand((unsigned int)time(NULL));
	char name[20];

	printf("****야옹이와 스프****\n");

	printf("        /\\_/\\\n");
	printf("  /\\   / o o \\\n");
	printf(" //\\\\  \\~(*)~/\n");
	printf(" `   \\/   ^ /\n");
	printf("     | \\|| ||\n");
	printf("     \\ `|| ||\n");
	printf("      \\)()-())\n");

	printf("야옹이의 이름을 지어 주세요:");
	fgets(name, sizeof(name), stdin);
	name[strcspn(name, "\n")] = '\0';

	printf("야옹이의 이름은 %s입니다.\n", name);

	Sleep(1000);
	system("cls");

	int soup = 0, level = 2;
	int cat = 1, previous = 0;
	int mood = 3, cp = 0;

	printStatus(soup, level, cp, name, mood);
	room(cat, previous);

	while (1) {
		interaction(name, &level);
		previous = cat;
		moveCat(name, &cat, level);
		room(cat, previous);
		action(name, cat, &soup);

		Sleep(2500);
		system("cls");
		printStatus(soup, level, cp, name, mood);
		room(cat, previous);
	}

	return 0;
}

void printStatus(int soupCount, int level, int cp, char name[], int mood) {//2-1 상태창 변경, CP,기분 출력
	const char* levelMessages[] = {
		"0: 곁에 오는 것도 싫어합니다.",
		"1: 간식 자판기 취급입니다.",
		"2: 그럭저럭 쓸 만한 집사입니다.",
		"3: 훌륭한 집사로 인정받고 있습니다.",
		"4: 집사 껌딱지입니다."
	};

	const char* moodMessages[] = {
		"0: 기분이 매우 나쁩니다.",
		"1: 심심해집니다.",
		"2: 식빵을 굽습니다.",
		"3: 골골송을 부릅니다."
	};

	printf("=============== 현재 상태 ===============\n");
	printf("현재까지 만든 수프: %d개\n", soupCount);
	printf("CP : %d 포인트\n", cp);
	printf("%s 의 기분 (0~3) : %d\n", name, mood);
	printf("\t%s\n",moodMessages[mood]);
	printf("집사와의 관계(0~4): %d\n", level);
	printf("%s\n", levelMessages[level]);
	printf("=========================================\n");
	
	Sleep(500);
}

void interaction(char name[], int* level) {
	int choice;
	printf("\n어떤 상호작용을 하시겠습니까?  0. 아무것도 하지 않음 1. 긁어주기\n");
	while (1) {
		printf(">>");
		scanf_s("%d", &choice);

		if (choice == 0 || choice == 1) break;
	}

	int dice = rand() % 6 + 1;

	if (choice == 0) {
		printf("아무것도 하지 않습니다.\n");
		printf("4/6의 확률로 친밀도가 떨어집니다.\n");
		printf("주사위를 굴립니다. 또르륵...\n");
		printf("%d이(가) 나왔습니다!\n", dice);

		if (dice <= 4 && *level > 0) {
			(*level)--;
			printf("친밀도가 떨어집니다.\n");
		}
		else {
			printf("다행히 친밀도가 떨어지지 않았습니다.\n");
		}
	}
	else if (choice == 1) {
		printf("%s의 턱을 긁어주었습니다.\n", name);
		printf("2/6의 확률로 친밀도가 높아집니다.\n");
		printf("주사위를 굴립니다. 또르륵...\n");
		printf("%d이(가) 나왔습니다!\n", dice);

		if (dice >= 5 && *level < 4) {
			(*level)++;
			printf("친밀도가 높아집니다.\n");
		}
		else {
			printf("친밀도는 그대로입니다.\n");
		}
	}
	printf("현재 친밀도 : %d\n", *level);
	printf("\n");

	Sleep(500);
}

void room(int cat, int previous) {
	int tower = 0, scratcher = 0; //2-1 방그리기 가구 놀이기구 미구매 상태

	for (int i = 0; i < ROOM_WIDTH; i++) {
		printf("#");
	}
	printf("\n");

	printf("#");
	for (int i = 1; i < ROOM_WIDTH-1; i++) {
		if (i == HME_POS) printf("H");
		else if (i == BWL_POS) printf("B");
		else if (tower && i == towerPos) printf("T"); //캣타워 출력, 현재는 미반영
		else if (scratcher && i == scratcherPos) printf("S");// 스크래쳐 출력, 현재는 미반영
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

	printf("\n%s 이동: 집사와 친밀할수록 냄비 쪽으로 갈 확률이 높아집니다.\n", name);
	printf("주사위 눈이 3 이상이면 냄비 쪽으로 이동합니다.\n");
	printf("주사위를 굴립니다. 또르륵...\n");
	printf("%d이(가) 나왔습니다!\n", dice);
	
	if (dice >= (6 - level)) {
		if (*cat < ROOM_WIDTH - 2) {
			(*cat)++;
			printf("냄비 쪽으로 움직입니다.\n");
		}
		else {
			printf("벽입니다.\n");
		}
	}
	else {
		if (*cat > 1) {
			(*cat)--;
			printf("집 쪽으로 움직입니다.\n");
		}
		else {
			printf("벽입니다.\n");
		}
	}

	Sleep(500);
}

void action(char name[], int cat, int* soup) {
	if (cat == HME_POS) {
		printf("%s(은)는 자신의 집에서 편안함을 느낍니다.\n", name);
	}
	else if (cat == BWL_POS) {
		int soupnum = rand() % 3;

		printf("%s(이)가 ", name);
		switch (soupnum) {
		case 0:
			printf("감자 수프"); break;
		case 1:
			printf("양송이 수프"); break;
		case 2:
			printf("브로콜리 수프"); break;
		}
		
		printf("를 만들었습니다!\n");
		(*soup)++;
	}

	Sleep(500);
}

void FurniturePosition() {//2-1 방그리기 위치 무작위 설정
	do {
		towerPos = rand() % (ROOM_WIDTH - 2) + 1;
	} while (towerPos == HME_POS || towerPos == BWL_POS);

	do {
		scratcherPos = rand() % (ROOM_WIDTH - 2) + 1;
	} while (scratcherPos == HME_POS || scratcherPos == BWL_POS || scratcherPos == towerPos);
}