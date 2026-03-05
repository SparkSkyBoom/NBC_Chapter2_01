#include <iostream>
#include <Windows.h> // Windows API 헤더
#include <limits>
#include <string>
#include <random>
#include <ctime>

enum STAT_TYPE {
	HP, 
	MP,
	ATTACK_POINT,
	DEF_POINT,
	LUCK,
	LENGTH
};

// 추가한 스텟 LUCK
// 시드값을 담고 있으며 추가 이벤트 일으킴

enum MENU_INTERFACE {
	QUIT,
	HP_UP,
	MP_UP,
	ATTACK_UP,
	DEF_UP,
	PRINT_STAT,
	LEVEL_UP,	
	SHOW_HIDDEN_STAT = 1001,
	CHANGE_HIDDEN_STAT = 2001
};


static int userInputNumber(int bClearBuffer = 0) {
	static char buffer[1024];
	static char* p_prev_buffer = NULL;
	
	int result = -1;

	if (p_prev_buffer == NULL || bClearBuffer) {
		if (fgets(buffer, 1024, stdin) == NULL) {
			clearerr(stdin);
			return -1;
		}
		p_prev_buffer = buffer;
	}
		
	int offset = 0;
	if (sscanf_s(p_prev_buffer, "%d%n", &result, &offset) == 1) {
		p_prev_buffer = p_prev_buffer + offset;
				
		return result;
	};

	p_prev_buffer = NULL;
	return -1;
}



void setPotion(int count, int* p_HPPotion, int* p_MPPotion) {
	*p_HPPotion = count;
	*p_MPPotion = count;
}

static void gainPotion(int count, int* potionCount) {
	*potionCount += count;
}


int main(int argc, char** argv) {
	SetConsoleOutputCP(CP_UTF8); // Console에 한글 표시를 위하여 출력코드를 UTF-8로 설정

	int characterStat[STAT_TYPE::LENGTH] = { 0, };
	int hpPotionCount;
	int mpPotionCount;
	int level = 1;

	characterStat[STAT_TYPE::LUCK] = static_cast<unsigned int>(time(NULL));

	srand(characterStat[STAT_TYPE::LUCK]);
	

	std::cout << "HP와 MP를 입력해주세요: ";
	characterStat[HP] = userInputNumber(1);
	characterStat[MP] = userInputNumber();

	while (characterStat[HP] <= 50 || characterStat[MP] <= 50) {
		std::cout << "HP나 MP의 값이 너무 작습니다. 다시 입력해주세요." << std::endl;		
		std::cout << "HP와 MP를 입력해주세요: ";
		characterStat[HP] = userInputNumber(1);
		characterStat[MP] = userInputNumber();
	}

	std::cout << "공격력과 방어력을 입력해주세요:";
	characterStat[ATTACK_POINT] = userInputNumber(1);
	characterStat[DEF_POINT] = userInputNumber();

	while (characterStat[ATTACK_POINT] <= 0 || characterStat[DEF_POINT] <= 0) {
		std::cout << "공격력이나 방어력의 값이 너무 작습니다. 다시 입력해주세요." << std::endl;
		std::cout << "공격력과 방어력을 입력해주세요: ";		
		characterStat[ATTACK_POINT] = userInputNumber(1);
		characterStat[DEF_POINT] = userInputNumber();		
	}

	int userInput;
	int x;

	setPotion(5, &hpPotionCount, &mpPotionCount);
	
	// 메뉴판 출력
	while (true) {		
		std::cout << "1. HP UP / 2. MP UP / 3. 공격력 UP / 4. 방어력 UP / 5. 현재 능력치 / 6. Level Up / 0. 나가기 " << std::endl;

		std::cout << "번호를 선택해주세요 : ";
		// 입력받기
		userInput = userInputNumber(1);
		
		// switch 처리
		switch (userInput)
		{
		case MENU_INTERFACE::HP_UP:
			if (hpPotionCount > 0) {
				characterStat[STAT_TYPE::HP] += 20;
				--hpPotionCount;
				std::cout << "* HP가 20 증가되었습니다. 포션이 1개 차감됩니다." << std::endl;
				std::cout << "* 현재 HP : " << characterStat[STAT_TYPE::HP] << std::endl;
				std::cout << "* 남은 포션의 수 : " << hpPotionCount << std::endl;

			} else {
				std::cout << "포션이 부족합니다." << std::endl;
			}
			break;
		case MENU_INTERFACE::MP_UP:
			if (mpPotionCount > 0) {
				characterStat[STAT_TYPE::MP] += 20;
				--mpPotionCount;
				std::cout << "* MP가 20 증가되었습니다. 포션이 1개 차감됩니다." << std::endl;
				std::cout << "* 현재 MP : " << characterStat[STAT_TYPE::MP] << std::endl;
				std::cout << "* 남은 포션의 수 : " << mpPotionCount << std::endl;
			} else {
				std::cout << "포션이 부족합니다." << std::endl;
			}
			break;
		case MENU_INTERFACE::ATTACK_UP:
			characterStat[STAT_TYPE::ATTACK_POINT] *= 2;
			std::cout << "* 공격력이 2배로 증가되었습니다." << std::endl;
			std::cout << "* 현재 공격력 : " << characterStat[STAT_TYPE::ATTACK_POINT] << std::endl;

			x = rand() % 10;
			if (x > 7) {
				std::cout << "* 히든 조건이 충족되었습니다. 공격력이 절반 만큼 (소수점 버림) 더 증가합니다." << std::endl;
				characterStat[STAT_TYPE::ATTACK_POINT] += characterStat[STAT_TYPE::ATTACK_POINT] / 2;
				std::cout << "* 현재 공격력 : " << characterStat[STAT_TYPE::ATTACK_POINT] << std::endl;
			}

			break;
		case MENU_INTERFACE::DEF_UP:
			characterStat[STAT_TYPE::DEF_POINT] *= 2;
			std::cout << "* 방어력이 2배로 증가되었습니다." << std::endl;
			std::cout << "현재 방어력 : " << characterStat[STAT_TYPE::DEF_POINT] << std::endl;

			break;
		case MENU_INTERFACE::PRINT_STAT:
			std::cout << "* HP : " << characterStat[STAT_TYPE::HP] << ", MP : " << characterStat[STAT_TYPE::MP] << ", 공격력 : "
				<< characterStat[STAT_TYPE::ATTACK_POINT] << ", 방어력 : " << characterStat[STAT_TYPE::DEF_POINT] << std::endl;
			std::cout << "* 현재 레벨 : " << level << std::endl;
			std::cout << "* 현재 포션 갯수 ( HP 포션 : " << hpPotionCount << ", MP 포션 : " << mpPotionCount << ")" << std::endl;
			break;
		
		case MENU_INTERFACE::LEVEL_UP:
			++level;
			gainPotion(1, &hpPotionCount);
			gainPotion(1, &mpPotionCount);
			
			std::cout << "* 레벨업! HP / MP 포션이 지급됩니다." << std::endl;

			x = rand() % 10;
			if (x > 7) {
				gainPotion(1, &hpPotionCount);
				gainPotion(1, &mpPotionCount);

				std::cout << "* 히든 조건이 충족되었습니다. HP / MP 포션이 추가로 지급됩니다." << std::endl;
			}

			std::cout << "* 현재 레벨 : " << level << std::endl;
			std::cout << "* 현재 포션 갯수 ( HP 포션 : " << hpPotionCount << ", MP 포션 : " << mpPotionCount << ")" << std::endl;
			break;

		case MENU_INTERFACE::QUIT:
			std::cout << "프로그램을 종료합니다." << std::endl;
			return 0;			

		case SHOW_HIDDEN_STAT:
			std::cout << "* 현재 플레이어의 히든 수치는 [" << characterStat[STAT_TYPE::LUCK] << "] 입니다." << std::endl;
			break;

		case CHANGE_HIDDEN_STAT:
			characterStat[STAT_TYPE::LUCK] = static_cast<unsigned int>(time(NULL));
			srand(characterStat[STAT_TYPE::LUCK]);
			std::cout << "무슨 일이 일어났습니다." << std::endl;
			break;

		default:
			std::cout << "유효하지 않은 명령입니다. 다시 시도해주세요." << std::endl;
			break;
		}
	}
	


	// setPoition

}

