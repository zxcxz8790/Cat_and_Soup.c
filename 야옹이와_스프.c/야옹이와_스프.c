#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include <time.h>

#define ROOM_WIDTH 10
#define HME_POS 1
#define BWL_POS (ROOM_WIDTH - 2)

void printStatus(int soupCount, int level, int cp, char name[], int mood);//상태출력
void interaction(char name[], int* level, int* mood, int hasMouse, int hasLaser);//2-5 상호작용 옵션 추가, 2-6 옵션별 친밀도,기분 증가
void room(int cat,int previous);//방그리기
void moveCat(char name[], int* cat, int mood);//2-3 이동v2
void action(char name[], int cat, int previous, int* soup, int* mood);//2-4 기분 증가 행동
void badMood(char name[], int level, int* mood);//2-2 기분 나빠짐
void store(int* cp, int* hasMouse, int* hasLaser);//2-8 상점
void setScratcherPosition();//2-8 수정된 스크래쳐 위치 설정
void setTowerPosition();//2-8 수정된 타워 위치 설정
void quest(int* cp, int* soup, int* mood, char name[]);//2-9 돌발 퀘스트

int towerPos = -1, scratcherPos = -1; //2-1 방그리기 현재 위치 미정

int main(void) {
	srand((unsigned int)time(NULL));
	char name[20];

	int hasMouse = 0, hasLaser = 0;
	int turnCount = 0;

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
		turnCount++;
		badMood(name, level, &mood);
		interaction(name, &level, &mood, hasMouse, hasLaser);
		previous = cat;
		moveCat(name, &cat, mood);
		if (mood == 1 && towerPos == -1 && scratcherPos == -1) mood = 0;
		room(cat, previous);
		action(name, cat, previous, &soup, &mood);

		int cp_add = (mood > 1 ? mood - 1 : 0) + level;//2-7 CP 생산
		cp += cp_add;
		printf("%s의 기분과 친밀도에 따라 CP가 %d 포인트 생산되었습니다.\n", name, cp_add);
		printf("보유 CP: %d 포인트\n", cp);
		Sleep(1000);

		if (turnCount == 3 || (turnCount > 3 && rand() % 100 < 20)) {
			quest(&cp, &soup, &mood, name);
		}
		
		store(&cp, &hasMouse, &hasLaser);

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

	printf("\n=============== 현재 상태 ===============\n");
	printf("현재까지 만든 수프: %d개\n", soupCount);
	printf("CP : %d 포인트\n", cp);
	printf("%s 의 기분 (0~3) : %d\n", name, mood);
	printf("\t%s\n",moodMessages[mood]);
	printf("집사와의 친밀도(0~4): %d\n", level);
	printf("\t%s\n", levelMessages[level]);
	printf("=========================================\n");
	
	Sleep(500);
}

void interaction(char name[], int* level, int* mood, int hasMouse, int hasLaser) {//2-5 상호작용 옵션 추가, 2-6 옵션별 친밀도,기분 증가
	int choice;
	int maxOption = 1;
	int laserOption = -1, mouseOption = -1;

	printf("\n어떤 상호작용을 하시겠습니까?\n");
	printf("0. 아무것도 하지 않음\n");
	printf("1. 긁어주기\n");

	int nextOption = 2;

	if (hasMouse) {
		printf("%d. 장난감 쥐로 놀아주기\n", nextOption);
		mouseOption = nextOption++;
	}

	if (hasLaser) {
		printf("%d. 레이저 포인터로 놀아주기\n", nextOption);
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
		printf("\n아무것도 하지 않습니다.\n");

		if (*mood > 0) {
			printf("%s의 기분이 나빠졌습니다: %d -> %d\n", name, *mood, *mood - 1);
			(*mood)--;
		}
		else {
			printf("이미 기분이 최저입니다.\n");
		}

		printf("\n주사위를 굴립니다. 또르륵...\n");
		printf("%d이(가) 나왔습니다!\n", dice);
		if (dice <= 5 && *level > 0) {
			(*level)--;
			printf("집사와의 관계가 나빠집니다.\n");
		}
		else {
			printf("친밀도는 그대로입니다.\n");
		}
	}

	else if (choice == 1) {
		printf("%s의 턱을 긁어주었습니다.\n", name);
		printf("주사위를 굴립니다. 또르륵...\n");
		printf("%d이(가) 나왔습니다!\n", dice);
		if (dice >= 5 && *level < 4) {
			(*level)++;
			printf("친밀도가 높아집니다!\n");
		}
		else {
			printf("친밀도는 그대로입니다.\n");
		}
	}

	else if (choice == mouseOption) {
		printf("장난감 쥐로 %s와 놀아주었습니다.\n", name);

		if (*mood < 3) {
			printf("기분이 조금 좋아졌습니다: %d -> %d\n", *mood, *mood + 1);
			(*mood)++;
		}
		else {
			printf("기분은 이미 최고입니다.\n");
		}

		printf("주사위를 굴립니다. 또르륵...\n");
		printf("%d이(가) 나왔습니다!\n", dice);
		if (dice >= 4 && *level < 4) {
			(*level)++;
			printf("친밀도가 높아집니다!\n");
		}
		else {
			printf("친밀도는 그대로입니다.\n");
		}
	}

	else if (choice == laserOption) {
		printf("레이저 포인터로 %s와 신나게 놀아주었습니다.\n", name);

		int oldMood = *mood;
		*mood += 2;
		if (*mood > 3) *mood = 3;
		printf("기분이 꽤 좋아졌습니다: %d -> %d\n", oldMood, *mood);

		printf("주사위를 굴립니다. 또르륵...\n");
		printf("%d이(가) 나왔습니다!\n", dice);
		if (dice >= 2 && *level < 4) {
			(*level)++;
			printf("친밀도가 높아집니다!\n");
		}
		else {
			printf("친밀도는 그대로입니다.\n");
		}
	}
	
	Sleep(1000);
}


void room(int cat, int previous) {
	printf("\n");
	for (int i = 0; i < ROOM_WIDTH; i++) {
		printf("#");
	}
	printf("\n");

	printf("#");
	for (int i = 1; i < ROOM_WIDTH-1; i++) {
		if (i == HME_POS) printf("H");
		else if (i == BWL_POS) printf("B");
		else if (towerPos != -1 && i == towerPos) printf("T");//2-8 수정된 타워 출력
		else if (scratcherPos != -1 && i == scratcherPos) printf("S");//2-8 수정된 스크래쳐 출력
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

void moveCat(char name[], int* cat, int mood) {//2-3 기분에 따라 이동하는 경우 생성
	printf("\n");
		if (mood == 0) {
		printf("기분이 매우 나쁜 %s은(는) 집으로 향합니다.\n", name);
		if (*cat > HME_POS) {
			(*cat)--;
			printf("집 쪽으로 한 칸 이동했습니다.\n");
		}
		else {
			printf("이미 집에 도착해 제자리에 있습니다.\n");
		}
	}
	else if (mood == 1) {
		if (towerPos == -1 && scratcherPos == -1) {
			printf("놀 거리가 없어서 %s의 기분이 매우 나빠집니다: 1 -> 0\n", name);
		}
		else {
			int targetPos;
			const char* targetName;

			if (towerPos != -1 && scratcherPos != -1) {
				int d1 = abs(*cat - towerPos);
				int d2 = abs(*cat - scratcherPos);
				if (d1 <= d2) {
					targetPos = towerPos;
					targetName = "캣타워";
				}
				else {
					targetPos = scratcherPos;
					targetName = "스크래처";
				}
			}
			else if (towerPos != -1) {
				targetPos = towerPos;
				targetName = "캣타워";
			}
			else {
				targetPos = scratcherPos;
				targetName = "스크래처";
			}

			printf("%s은(는) 심심해서 %s 쪽으로 이동합니다.\n", name, targetName);

			if (*cat < targetPos) (*cat)++;
			else if (*cat > targetPos) (*cat)--;
			else printf("이미 %s에 도착해 제자리에 있습니다.\n", targetName);
		}
	}
	else if (mood == 2) {
		printf("%s은(는) 기분 좋게 식빵을 굽고 있습니다.\n", name);
	}
	else if (mood == 3) {
		printf("%s은(는) 골골송을 부르며 수프를 만들러 갑니다.\n", name);
		if (*cat < BWL_POS) {
			(*cat)++;
			printf("냄비 쪽으로 한 칸 이동했습니다.\n");
		}
		else {
			printf("이미 냄비에 도착해 제자리에 있습니다.\n");
		}
	}

	Sleep(500);
}


void action(char name[], int cat, int previous, int* soup, int* mood) { //2-4 위치별 기분 증가
	printf("\n");
	if (cat == HME_POS && previous == HME_POS) {
		if (*mood < 3) {
			printf("%s(은)는 집에서 편안히 쉬며 기분이 좋아집니다: %d -> %d\n", name, *mood, *mood + 1);
			(*mood)++;
		}
		else {
			printf("%s(은)는 집에서 편안히 쉬고 있습니다.\n", name);
		}
	}
	else if (cat == BWL_POS) {
		int soupnum = rand() % 3;

		printf("%s(이)가 ", name);
		switch (soupnum) {
		case 0: printf("감자 수프"); break;
		case 1: printf("양송이 수프"); break;
		case 2: printf("브로콜리 수프"); break;
		}
		printf("를 만들었습니다!\n");
		(*soup)++;
	}
	else if (cat == scratcherPos && scratcherPos != -1) {
		if (*mood < 3) {
			printf("%s(은)는 스크래처를 긁고 놀았습니다. 기분이 조금 좋아졌습니다: %d -> %d\n", name, *mood, *mood + 1);
			(*mood)++;
		}
		else {
			printf("%s(은)는 스크래처를 긁지만 이미 매우 기분이 좋습니다.\n", name);
		}
	}
	else if (cat == towerPos && towerPos != -1) {
		int increase = (*mood <= 1) ? 2 : (3 - *mood);
		if (increase > 0) {
			printf("%s(은)는 캣타워를 뛰어다닙니다. 기분이 제법 좋아졌습니다: %d -> %d\n", name, *mood, *mood + increase);
			(*mood) += increase;
		}
		else {
			printf("%s(은)는 캣타워에서 즐겁게 놀고 있습니다.\n", name);
		}
	}

	Sleep(1000);
}

//2-1 가구 배치, 2-8 상점 기능에 맞춰 품목마다 개별적으로 적용되도록 수정
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

void badMood(char name[],int level, int *mood){//2-2 기분 나빠짐
	printf("\n아무 이유 없이 기분이 나빠집니다. 고양이니까?\n");

	int limit = 6 - level;
	printf("6 - %d : 주사위 눈이 %d 이하이면 그냥 기분이 나빠집니다.\n",level,limit);

	int dice = rand() % 6 + 1;
	printf("주사위를 굴립니다. 또르륵...\n");
	printf("%d이(가) 나왔습니다!\n", dice);

	if (dice <= limit) {
		if (*mood > 0) {
			printf("%s의 기분이 나빠집니다: %d -> %d\n", name, *mood, *mood - 1);
			(*mood)--;
		}
		else {
			printf("이미 기분이 최저입니다.\n");
		}
	}
	else {
		printf("기분은 그대로입니다.\n");
	}

	Sleep(1000);
}

void store(int* cp, int* hasMouse, int* hasLaser) {//2-8 상점
	static int hasScratcher = 0, hasTower = 0;

	printf("\n상점에서 물건을 살 수 있습니다.\n");
	printf("보유 CP: %d 포인트\n", *cp);
	printf("어떤 물건을 구매할까요?\n");
	printf("0. 아무 것도 사지 않는다\n");
	printf("1. 장난감 쥐: 1 CP %s\n", *hasMouse ? "(품절)" : "");
	printf("2. 레이저 포인터: 2 CP %s\n", *hasLaser ? "(품절)" : "");
	printf("3. 스크래처: 4 CP %s\n", hasScratcher ? "(품절)" : "");
	printf("4. 캣 타워: 6 CP %s\n", hasTower ? "(품절)" : "");

	int choice;

	while (1) {
		printf(">> ");
		scanf_s("%d", &choice);

		if (choice == 0) {
			printf("아무 것도 구매하지 않았습니다.\n");
			break;
		}

		else if (choice == 1) {
			if (*hasMouse) {
				printf("이미 구매했습니다.\n");
			}
			else if (*cp >= 1) {
				*hasMouse = 1;
				*cp -= 1;
				printf("장난감 쥐를 구매했습니다. 남은 CP: %d\n", *cp);
				break;
			}
			else {
				printf("CP가 부족합니다.\n");
			}
		}

		else if (choice == 2) {
			if (*hasLaser) {
				printf("이미 구매했습니다.\n");
			}
			else if (*cp >= 2) {
				*hasLaser = 1;
				*cp -= 2;
				printf("레이저 포인터를 구매했습니다. 남은 CP: %d\n", *cp);
				break;
			}
			else {
				printf("CP가 부족합니다.\n");
			}
		}

		else if (choice == 3) {
			if (hasScratcher) {
				printf("이미 구매했습니다.\n");
			}
			else if (*cp >= 4) {
				hasScratcher = 1;
				*cp -= 4;
				setScratcherPosition();
				printf("스크래처를 구매했습니다. 남은 CP: %d\n", *cp);
				break;
			}
			else {
				printf("CP가 부족합니다.\n");
			}
		}

		else if (choice == 4) {
			if (hasTower) {
				printf("이미 구매했습니다.\n");
			}
			else if (*cp >= 6) {
				hasTower = 1;
				*cp -= 6;
				setTowerPosition();
				printf("캣 타워를 구매했습니다. 남은 CP: %d\n", *cp);
				break;
			}
			else {
				printf("CP가 부족합니다.\n");
			}
		}
		else {
			printf("");
		}
	}

	Sleep(1000);
}

void quest(int* cp, int* soup, int* mood, char name[]) {//2-9 돌발 퀘스트
	int choice;
	int successRate[] = { 0, 80, 60, 40, 25 };
	int gain[] = { 0,  1,  2,  3,  5 };
	int cost[] = { 0,  2,  4,  6,  8 };

	printf("\n돌발 퀘스트 고양이 부스터 발생!\n\n");
	printf("%s이가 갑자기 폭주하기 시작합니다!\n", name);
	printf("기분이 너무 좋아진 %s이(가) 수프를 왕창 만들 수도\n", name);
	printf("혹은 왕창 엎을 수도 있습니다!\n\n");

	printf("CP 투자\n");
	printf("0. 아무 것도 하지 않는다\n");
	printf("1. 2 CP 안전한 투자 (+1 / -2)\n");
	printf("2. 4 CP 중간 투자 (+2 / -4)\n");
	printf("3. 6 CP 위험한 투자 (+3 / -6)\n");
	printf("4. 8 CP 도박 (+5 / -10)\n");

	while (1) {
		printf(">> ");
		scanf_s("%d", &choice);

		if (choice < 0 || choice > 4) {
			printf(">> ");
			continue;
		}

		if (choice == 0) {
			printf("아무 행동도 하지 않았습니다.\n");
			break;
		}

		if (*cp < cost[choice]) {
			printf("CP가 부족합니다.\n");
			printf(">> ");
			continue;
		}

		*cp -= cost[choice];

		int prob = rand() % 100 + 1;
		printf("\n%s이(가) 빠르게 움직입니다...!\n", name);

		if (prob <= successRate[choice]) {
			*soup += gain[choice];
			printf("성공! 수프를 %d개 추가로 만들었습니다!\n", gain[choice]);
		}
		else {
			int loss = gain[choice] * 2;
			if (*soup < loss) *soup = 0;
			else *soup -= loss;
			printf("실패! %s이(가) 수프 통을 엎질러 수프 %d개를 잃었습니다!\n", name, loss);

			if (*mood > 0) {
				printf("%s의 기분이 나빠졌습니다: %d -> %d\n", name, *mood, *mood - 1);
				(*mood)--;
			}
		}

		printf("현재 수프: %d개\n", *soup);
		break;
	}

	Sleep(1000);
}
