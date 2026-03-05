# 상태창 구현

- 첫 주차 과제입니다.
- 배열과 함수를 사용하여 데이터를 처리합니다.
- 사용자의 입력을 받아 출력을 하는 간단한 프로그램 설계 및 구현합니다.
  
 
## 주요 기능
- HP, MP를 입력받아 플레이어의 스텟으로 설정합니다.
  - HP와 MP 중 하나라도 50 이하를 입력 받은 경우, 안내 문구와 함께 다시 입력받습니다.

- 공격력과 방어력을 입력받아 플레이어의 스텟으로 설정합니다.
  - 공격력과 방어력 중 하나라도 0 이하를 입력 받으면 다시 입력 받습니다.

- 스텟 입력이 마무리되면 메뉴판이 출력됩니다.

``` 
1.HP UP / 2.MP UP / 3.공격력 UP / 4.방어력 UP / 5.현재 능력치 / 6.Level Up / 0.나가기
```
사용자로부터 입력 받아 선택지를 실행하며 각각의 선택지는 다음과 같습니다.

1번 : 포션을 사용하여 HP를 20 올립니다. 이때 포션의 개수는 감소합니다.  
2번 : 포션을 사용하여 MP를 20 올립니다. 이때 포션의 개수는 감소합니다.  
3번 : 공격력을 2배로 만듭니다.  
4번 : 방어력을 2배로 만듭니다.  
5번 : 현재 HP, MP, 공격력, 방어력 등 유저 정보를 출력합니다.  
6번 : 레벨업 : 포션을 1개씩 더 획득합니다.  
0번 : 프로그램을 종료합니다.



그 밖에 도전 기능으로 구현한 기능은 다음과 같습니다.

- level 변수를 만들었고 6번 선택지의 경우 레벨업이 됩니다.
- 포션을 충전하는 기능을 별도의 함수로 분리했습니다.
```C++
static void gainPotion(int count, int* potionCount) {
	*potionCount += count;
}
```
- 5번 선택시, 현재 레벨 및 남은 포션의 개수도 함께 출력합니다.
```
* HP : 60, MP : 60, 공격력 : 1, 방어력 : 1
* 현재 레벨 : 1
* 현재 포션 갯수 ( HP 포션 : 5, MP 포션 : 5)
```

- 확장  
  - 추가 스텟을 구현합니다.
```C++
// 메뉴판에 진입 전, 시간값에 따라 스텟이 설정됩니다.

characterStat[STAT_TYPE::LUCK] = static_cast<unsigned int>(time(NULL));

// 히든스텟은 랜덤 함수의 시드 값으로 사용됩니다.
srand(characterStat[STAT_TYPE::LUCK]);

...

// 공격력 증가 실행시, 일정 확률로 공격력이 더 추가로 증가 합니다.
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


// 메뉴판에 설명은 없지만, 1001 번을 입력하면 히든 스텟을 확인할 수 있습니다.  
// 메뉴판에 설명은 없지만, 2001 번을 입력하면 히든 스텟을 재설정할 수 있습니다.


// 레벨업을 할 때, 일정 확률로 추가 포션을 지급합니다.
case MENU_INTERFACE::LEVEL_UP:
    ...

    x = rand() % 10;
    if (x > 7) {
		gainPotion(1, &hpPotionCount);
		gainPotion(1, &mpPotionCount);

		std::cout << "* 히든 조건이 충족되었습니다. HP / MP 포션이 추가로 지급됩니다." << std::endl;
    }

```

- 기타

- 유저로부터 숫자 입력받는 함수
```C++

// 사용자가 숫자 입력하는 부분을 랩핑하여 함수를 만들었습니다.
// 이 방식을 통해 사용자가 숫자가 아닌 문자열을 입력할 경우, 
// 버퍼에 남은 문자열을 비우지 못하여 프로그램이 뻗는 것을 방지합니다.

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

```


- 수치가 커지면 StackOverFlow 발생할 수 있는 부분에 대해서는 따로 대응하지 않았습니다.
