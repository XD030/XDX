#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

//文字顏色
#define ANSI_COLOR_BLUE "\x1b[34;1m"
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_YELLOW "\x1b[33;1m"
#define ANSI_COLOR_GREEN "\x1b[32;1m"
#define ANSI_COLOR_PINK "\x1b[35;1m"
#define ANSI_COLOR_RESET "\x1b[0m"

//玩家結構
struct Player {
    int health;
    int mana;
    int wealth;
    int fatigue;
    int healthLimit;
    int manaLimit;
    int fatigueLimit;
};

//根據玩家選擇初始化角色屬性值
void initPlayer(struct Player *player, int choice) {
    switch (choice) {
        case 1:
            player->health = 200;
            player->mana = 0;
            player->wealth = 50;
            player->fatigue = 0;
            player->healthLimit = 200;
            player->manaLimit = 0;
            player->fatigueLimit = 150;
            break;
        case 2:
            player->health = 70;
            player->mana = 170;
            player->wealth = 80;
            player->fatigue = 0;
            player->healthLimit = 70;
            player->manaLimit = 170;
            player->fatigueLimit = 80;
            break;
        case 3:
            player->health = 90;
            player->mana = 50;
            player->wealth = 150;
            player->fatigue = 0;
            player->healthLimit = 50;
            player->manaLimit = 50;
            player->fatigueLimit = 110;
            break;
        case 4:
            player->health = 100;
            player->mana = 100;
            player->wealth = 100;
            player->fatigue = 0;
            player->healthLimit = 100;
            player->manaLimit = 100;
            player->fatigueLimit = 100;
            break;
        default:
            break;
    }
}

//顯示玩家的各項屬性值
void displayStatus(struct Player *player) {
    //文字上色
    printf("HP：" ANSI_COLOR_RED "%d/%d" ANSI_COLOR_RESET \
      " | ", player->health, player->healthLimit);
    printf("MP：" ANSI_COLOR_BLUE "%d/%d" ANSI_COLOR_RESET \
      " | ", player->mana, player->manaLimit);
    printf("金錢：" ANSI_COLOR_YELLOW "%dG" ANSI_COLOR_RESET \
      " | ", player->wealth);
    printf("疲勞：" ANSI_COLOR_GREEN "%d/%d\n" ANSI_COLOR_RESET\
      , player->fatigue >= 0 ? player->fatigue : 0, player->fatigueLimit);
}

//檢查或修正血量是否超過上限
void checkHealth(struct Player *player, int baseHealth) {
    if (player->health > baseHealth) {
        player->health = baseHealth;
    }
}

//檢查或修正魔力是否超過上限
void checkMana(struct Player *player, int baseMana) {
    //使戰士魔力恆為0
    if (player->mana < 0) {
        player->mana = 0;
    } else if (player->mana > baseMana) {
        player->mana = baseMana;
    }
}

//檢查或修正疲勞值是否低於下限
void checkFatigue(struct Player *player, int baseFatigue) {
    if (player->fatigue > baseFatigue) {
        player->fatigue = baseFatigue;
    }
}

//直接進入下 1 層
void luckyNextLevel(int *level) {
    printf("你成功前進到下一層！\n");
    (*level)++;
}

//隨機事件
void battle(struct Player *player, int *level, int choice) {
    int prevHealth = player->health;
    int prevMana = player->mana;
    int prevFatigue = player->fatigue;
    int prevWealth = player->wealth;
    int changeHealth = 0;
    int changeMana = 0;
    int changeFatigue = 0;
    int changeWealth = 0;

    srand(time(0));
    // 得到0到4的隨機數
    int random = rand() % 5; 
    switch (random) {
        case 0:
          // 隨機減少能力值或增加疲勞值
          printf("你遇到了一個不利的事件，減少能力值或增加疲勞值。\n");
          // 生成0到1的隨機數
          int negativeEvent = rand() % 2; 
          // 0會減少血量和魔力上限
          if (negativeEvent == 0) { 
            int healthDecrease = (rand() % 10) + 1; 
            
            printf(ANSI_COLOR_PINK "你感受到身體與靈魂的虛弱！\n"\
              ANSI_COLOR_RESET);
            printf("生命上限減少了：" ANSI_COLOR_RED "-%d" \
              ANSI_COLOR_RESET "\n", healthDecrease);
            player->healthLimit -= healthDecrease;
            //使戰士魔力恆為0
            if (choice != 1) {
              int manaDecrease = (rand() % 10) + 1;
              player->manaLimit -= manaDecrease;
              printf("魔法上限減少了：" ANSI_COLOR_BLUE "-%d" \
                ANSI_COLOR_RESET "\n", manaDecrease);
            }
          } 
            
          // 1會增加疲勞值
          else {
            int fatigueIncrease = (rand() % 10) + 1; 
            
            player->fatigue += fatigueIncrease;
            printf(ANSI_COLOR_PINK "你感受到昏昏欲睡Zzz...\n"\
              ANSI_COLOR_RESET);
            printf("疲勞值增加了：" ANSI_COLOR_GREEN "+%d" \
              ANSI_COLOR_RESET "\n", fatigueIncrease);
            
          }
            break;
        case 1:
            // 遇到寶箱
            printf(ANSI_COLOR_PINK "你發現了一個寶箱！\n" \
              ANSI_COLOR_RESET);
            // 生成10到50的隨機金錢
            int moneyFound = (rand() % 41) + 10;
          
            printf("你在寶箱裡找到了"ANSI_COLOR_YELLOW " %dG "\
              ANSI_COLOR_RESET "！\n", moneyFound);
            player->wealth += moneyFound;
            int changeWealth = player->wealth - prevWealth;
            printf("金錢：" ANSI_COLOR_YELLOW "%+dG" \
              ANSI_COLOR_RESET "\n", changeWealth);
            break;
        case 2:
            // 遇到敵人
            printf(ANSI_COLOR_PINK "你遇到了敵人！\n"\
              ANSI_COLOR_RESET); 
            // 可能的傷害值，此處用隨機數替代
            int damage = random;
          
            player->health -= damage * 20 / 6;
            player->mana -= damage * 20 / 6;

            // 根據受到的傷害計算疲勞值
            double fatigueIncrease = damage * 4.6;
            player->fatigue += (int)fatigueIncrease;
            player->wealth += damage * 10 / 3;

            // 使戰士魔力恆為0
            checkMana(player, player->manaLimit);


            changeHealth = player->health - prevHealth;
            changeMana = player->mana - prevMana;
            changeFatigue = player->fatigue - prevFatigue;
            changeWealth = player->wealth - prevWealth;

            printf("你進行了一場戰鬥。\n");
            printf("\n屬性值變化：\n");
            printf("HP：" ANSI_COLOR_RED "%+d" ANSI_COLOR_RESET\
              " | ", changeHealth);
            printf("MP：" ANSI_COLOR_BLUE "%+d" ANSI_COLOR_RESET \
              " | ", changeMana);
            printf("金錢：" ANSI_COLOR_YELLOW "%+dG" ANSI_COLOR_RESET \
              " | ", changeWealth);
            printf("疲勞：" ANSI_COLOR_GREEN "%+d\n" ANSI_COLOR_RESET\
              , changeFatigue);
            break;
        case 3:
            // 遇到金幣哥布林
            printf("你遇到了一個 " ANSI_COLOR_YELLOW "$$ 金幣 $$ " \
              ANSI_COLOR_RESET "哥布林!?\n");
            int halfMoney = 0; 
            //金錢剩餘1時，會直接歸0
            if (player->wealth > 1) {
              halfMoney = player->wealth / 2;
            }
            else
              halfMoney = 1;
          
            printf("你被偷走了一些錢!!\n");
            player->wealth -= halfMoney;
            changeWealth = player->wealth - prevWealth;
            printf("金錢：" ANSI_COLOR_YELLOW "%+dG" ANSI_COLOR_RESET \
              "\n", changeWealth);
            luckyNextLevel(level);
            break;
        case 4:
            // 隨機增加能力值、技能或祝福
            printf("你遇到了一個特殊事件，感受到力量加持。\n");
            // 生成0到1的隨機數
            int event = rand() % 2; 
            // 0會增加血量和魔力上限
            if (event == 0) {
              int healthIncrease = (rand() % 10) + 1;
              
              printf(ANSI_COLOR_PINK "你感受到身體與靈魂的強大增加！\n" \
                ANSI_COLOR_RESET);
              printf("生命上限增加了：" ANSI_COLOR_RED "+%d" \
                ANSI_COLOR_RESET "\n", healthIncrease);
              player->healthLimit += healthIncrease;
              // 使戰士魔力恆為0
              if(choice != 1){
              int manaIncrease = (rand() % 10) + 1;
              
              player->manaLimit += manaIncrease;
              printf("魔法上限增加了：" ANSI_COLOR_BLUE "+%d" \
                ANSI_COLOR_RESET "\n", manaIncrease);
              }
            }
            //1會減少疲勞值
            else {
                int fatigueReduction = (rand() % 5) + 1;
              
                player->fatigue -= fatigueReduction;
                printf(ANSI_COLOR_PINK "你感受到疲勞減輕了！\n" \
                  ANSI_COLOR_RESET);
                printf("疲勞減少了：" ANSI_COLOR_GREEN "-%d" \
                  ANSI_COLOR_RESET "\n", fatigueReduction);
            }  
            break;
        default:
            break;
    }
}

//前進會改變特定屬性值
void goforward(struct Player *player) {

    int prevFatigue = player->fatigue;
    int prevWealth = player->wealth;

    player->fatigue += 10;
    player->wealth -= 5;

    int changeFatigue = player->fatigue - prevFatigue;
    int changeWealth = player->wealth - prevWealth;

    printf("\n屬性值變化：\n");
    printf("金錢：" ANSI_COLOR_YELLOW "%+dG" ANSI_COLOR_RESET\
      " | ", changeWealth);
    printf("疲勞：" ANSI_COLOR_GREEN "%+d\n" ANSI_COLOR_RESET\
      , changeFatigue);
}

//休息會改變特定屬性值
void rest(struct Player *player) {
    int prevHealth = player->health;
    int prevMana = player->mana;
    int prevFatigue = player->fatigue;
    int prevWealth = player->wealth;

    player->health += 20;
    player->wealth -= 15;
    player->fatigue -= 30;
    //檢查並修正超過上限或低於下限的屬性值
    checkHealth(player, player->healthLimit);
    checkMana(player, player->manaLimit);
    checkFatigue(player, player->fatigueLimit);

    int changeHealth = player->health - prevHealth;
    int changeMana = player->mana - prevMana;
    int changeFatigue = player->fatigue - prevFatigue;
    int changeWealth = player->wealth - prevWealth;

      printf("你休息了一下。\n");
      printf("\n屬性值變化：\n");
      printf("HP：" ANSI_COLOR_RED "%+d" ANSI_COLOR_RESET \
        " | ", changeHealth);
      printf("MP：" ANSI_COLOR_BLUE "%+d" ANSI_COLOR_RESET \
        " | ", changeMana);
      printf("金錢：" ANSI_COLOR_YELLOW "%+dG" ANSI_COLOR_RESET \
        " | ", changeWealth);
      printf("疲勞：" ANSI_COLOR_GREEN "%+d\n" ANSI_COLOR_RESET\
        , changeFatigue);
}

//檢查輸入行動選項時是否正確
int getInput() {
    int action;
    while (true) {
        scanf("%d", &action);
        if ((action < 0 || action > 3)) {
            printf("請輸入有效選項（0-3）：");
        } else {
            break;
        }
    }
    return action;
}

//顯示各職業屬性值
void displayProfessions() {
    printf("請選擇職業：\n");
    printf("1. 戰士| HP：" ANSI_COLOR_RED "200" ANSI_COLOR_RESET\
      " | MP：" ANSI_COLOR_BLUE "0" ANSI_COLOR_RESET "   | 金錢："\
      ANSI_COLOR_YELLOW "50" ANSI_COLOR_RESET "  | 疲勞上限：" \
      ANSI_COLOR_GREEN "150" ANSI_COLOR_RESET "\n");
    printf("2. 法師| HP：" ANSI_COLOR_RED "70" ANSI_COLOR_RESET \
      "  | MP：" ANSI_COLOR_BLUE "170" ANSI_COLOR_RESET " | 金錢："\
      ANSI_COLOR_YELLOW "80" ANSI_COLOR_RESET "  | 疲勞上限：" \
      ANSI_COLOR_GREEN "80" ANSI_COLOR_RESET "\n");
    printf("3. 遊俠| HP：" ANSI_COLOR_RED "90" ANSI_COLOR_RESET \
      "  | MP：" ANSI_COLOR_BLUE "50" ANSI_COLOR_RESET "  | 金錢："\
      ANSI_COLOR_YELLOW "150" ANSI_COLOR_RESET " | 疲勞上限：" \
      ANSI_COLOR_GREEN "110" ANSI_COLOR_RESET "\n");
    printf("4. 標準| HP：" ANSI_COLOR_RED "100" ANSI_COLOR_RESET \
      " | MP：" ANSI_COLOR_BLUE "100" ANSI_COLOR_RESET " | 金錢："\
      ANSI_COLOR_YELLOW "100" ANSI_COLOR_RESET " | 疲勞上限："\
      ANSI_COLOR_GREEN "100" ANSI_COLOR_RESET "\n");
    printf("選擇（1-4）：");
}

//輸入前進選項
void attemptNextLevel(int *level) {
    srand(time(NULL));

    int success = rand() % 100;
    // 87% 的成功率
    if (success < 87) { 
        printf("你成功前進到下一層！\n");
        (*level)++;
    } else {
        printf("你嘗試了，但似乎進不去下一層。\n");
    }
}

//Boss事件
void boss(struct Player *player, int level){
    printf("真沒想到你能來到這裡...\n");
    printf("那麼去死吧!!!\n");
  
    int prevHealth = player->health;
    int prevMana = player->mana;
    int prevFatigue = player->fatigue;
    int prevWealth = player->wealth;
    //遇到Boss的次數+1
    int i = 1 + level / 9;
    
    player->health = player->health / i;
    player->mana = player->mana / i;
    player->wealth = player->wealth / i;
    player->fatigue = player->fatigue * i;
  
    int changeHealth = player->health - prevHealth;
    int changeMana = player->mana - prevMana;
    int changeFatigue = player->fatigue - prevFatigue;
    int changeWealth = player->wealth - prevWealth;

    printf("\n屬性值變化：\n");
    printf("HP：" ANSI_COLOR_RED "%+d" ANSI_COLOR_RESET \
      " | ", changeHealth);
    printf("MP：" ANSI_COLOR_BLUE "%+d" ANSI_COLOR_RESET \
      " | ", changeMana);
    printf("金錢：" ANSI_COLOR_YELLOW "%+dG" ANSI_COLOR_RESET \
      " | ", changeWealth);
    printf("疲勞：" ANSI_COLOR_GREEN "%+d\n" ANSI_COLOR_RESET\
      , changeFatigue);
}

//觸發女神祈禱事件
void goddessPrayer(struct Player *player) {
    int prayCost;
  
    printf("你想向女神祈禱嗎？\n");
    printf("請輸入花費的金額" ANSI_COLOR_YELLOW "（1G-%dG）" \
      ANSI_COLOR_RESET "，輸入" ANSI_COLOR_YELLOW " 0 " \
      ANSI_COLOR_RESET "為取消：", player->wealth);
    scanf("%d", &prayCost);
    //輸入0為取消
    if (prayCost == 0) {
        printf(ANSI_COLOR_PINK "你取消了祈禱，甚麼事情都沒發生。\n" \
          ANSI_COLOR_RESET);
        printf(ANSI_COLOR_PINK "但有一種違和感.....\n" \
          ANSI_COLOR_RESET);
        player->wealth = player->wealth / 2;
    } 
    else if (prayCost >= 1 && prayCost <= player->wealth) {
        int changeWealth = prayCost;
        //判斷玩家是否支付超過自身一半的金錢
        if (prayCost < (player->wealth) / 2){
            printf(ANSI_COLOR_PINK \
              "女神笑著對你說:「我來幫你承受一些負擔，ㄟ嘿~」\n" ANSI_COLOR_RESET);
            //先扣掉祈禱的花費
            player->wealth = player->wealth - prayCost;
            changeWealth = player->wealth / 2;
            //再扣至一半
            player->wealth = player->wealth - changeWealth;
            printf("你獲得了價值" ANSI_COLOR_YELLOW "%dG" \
              ANSI_COLOR_RESET "的贖罪劵...?!\n", changeWealth);
            printf("金錢：" ANSI_COLOR_YELLOW "-%dG" \
              ANSI_COLOR_RESET "\n", changeWealth + prayCost);
        }
        //根據花費的多寡決定成功的機率
        int success = rand() % 100;
        if (prayCost == player->wealth) {
          printf(ANSI_COLOR_PINK \
            "女神慈祥的對你說:「虔誠的教徒啊，願我與你同在」\n" ANSI_COLOR_RESET);
          printf(ANSI_COLOR_PINK "你的祈禱被接受了！\n"\
            ANSI_COLOR_RESET);
          player->health = player->healthLimit;
          player->mana = player->manaLimit;
          player->fatigue = 0;
          printf("你恢復了全部的血量、魔力和疲勞值。\n");
        }
        else if (success <= prayCost) {
            printf(ANSI_COLOR_PINK \
              "女神慈祥的對你說:「虔誠的教徒啊，願我與你同在」\n" ANSI_COLOR_RESET);
            printf(ANSI_COLOR_PINK "你的祈禱被接受了！\n" \
              ANSI_COLOR_RESET);
            player->health = player->healthLimit;
            player->mana = player->manaLimit;
            player->fatigue = 0;
            printf("你恢復了全部的血量、魔力和疲勞值。\n");
        } 
        else {
            printf(ANSI_COLOR_PINK \
              "很抱歉，你的祈禱沒有獲得回應。\n" ANSI_COLOR_RESET);
        }
        player->wealth -= prayCost;
    } 
    else {
        printf("無效的金額，祈禱失敗。\n");
        printf(ANSI_COLOR_PINK "但有一種違和感.....\n" ANSI_COLOR_RESET);
        player->wealth = player->wealth / 2;
    }
}

int main() {
    int choice;
    //初始層數
    int level = 1;
    //休息記數器和休息剩餘輪數
    int restCounter = 0, remainingRest = 3;

    displayProfessions();
    scanf("%d", &choice);
  
    //檢查玩家選擇職業時，是否輸入正確
    if (choice < 1 || choice > 4) {
        printf(ANSI_COLOR_PINK \
          "請輸入有效選項（1-4）\n你要不要輸入 Windows 標誌鍵 + L ...\n"\
          ANSI_COLOR_RESET);
        displayProfessions();
        scanf("%d", &choice);
    }

    struct Player player;
    //初始化選擇職業數值
    initPlayer(&player, choice);

    bool playAgain = true;
    while (playAgain) {
    int action;
    //清除畫面保持整潔
    system("clear");
    printf("屬性值：\n");
    //顯示玩家的數值和層數
    displayStatus(&player);
    printf(ANSI_COLOR_PINK \
      "\n*** level %d ***\n" ANSI_COLOR_RESET, level);

    //女神祈禱事件出現在7倍數的層數
    if (level % 7 == 0) {
      goddessPrayer(&player);
      luckyNextLevel(&level);
      printf("按 Enter 鍵繼續...\n");
      //緩衝直到按下Enter才會繼續
      getchar();
      getchar();
      //判斷玩家是否死亡
      if ((player.health <= 0) || ((player.mana <= 0) && (choice != 1)) \
        || (player.wealth < 0) || (player.fatigue >= player.fatigueLimit)) {
          playAgain = false;
          break;
      }
      system("clear");
      printf("屬性值：\n");
      displayStatus(&player);
      printf(ANSI_COLOR_PINK \
        "\n*** level %d ***\n" ANSI_COLOR_RESET, level);
    } 
    
    //Boss事件出現在9倍數的層數
    else if (level % 9 == 0) {
      boss(&player, level);
      printf("按 Enter 鍵繼續...\n");
      getchar();
      //判斷玩家是否死亡
      if ((player.health <= 0) || ((player.mana <= 0) && (choice != 1)) \
        || (player.wealth < 0) || (player.fatigue >= player.fatigueLimit)) {
          playAgain = false;
          break;
      }
      system("clear");
      printf(ANSI_COLOR_PINK \
        "\n你勉強的倖存下來了...\n" ANSI_COLOR_RESET);
      luckyNextLevel(&level);
      printf("屬性值：\n");
      displayStatus(&player);
      printf(ANSI_COLOR_PINK \
        "\n*** level %d ***\n" ANSI_COLOR_RESET, level);
    }
    //判斷玩家是否到達30層通關
    else if (level ==  30) {
      //使迴圈失效
      playAgain = false;
      break;
    }

    printf("\n選擇：\n");
    printf("1. 探索（G/SP↑  | HP/MP↓）\n");
    printf("2. 前進（SP↑    | G↓    ）\n");
    printf("3. 休息（HP/MP↑ | G/SP↓ ）\n");
    printf("0. 結束遊戲\n");
    printf("\n還剩下 " ANSI_COLOR_PINK "%d" \
      ANSI_COLOR_RESET " 輪才能休息。\n", remainingRest);
    printf("選擇操作（0-3）：");
    //確認輸入正確選項
    action = getInput();
      
    //如果選擇了休息且還不能使用休息
    if (action == 3 && restCounter < 3) { 
      //重新輸入選項
      continue; 
      } 
    else {
          //如果選擇了休息且可以使用休息
          if (action == 3) { 
            //重置計數器
            restCounter = 0; 
            //重置剩餘休息輪數
            remainingRest = 3; 
            
          } 
          //如果選擇了其他動作，增加計數器
          else {
              restCounter++; 
              //更新剩餘休息輪數，並確保不為負值
              remainingRest = (remainingRest > 0) ? (remainingRest - 1) : 0; 
          }
      }

    int changeHealth = 0;
    int changeMana = 0;
    int changeWealth = 0;
    int changeFatigue = 0;
      
    //根據輸入行動選擇做出對應
    switch (action) {
      //輸入探索
      case 1:
        battle(&player, &level, choice);
        break;
      //輸入前進
      case 2:
        goforward(&player);
        attemptNextLevel(&level);
        break;
      //輸入休息
      case 3:
        rest(&player);
        break;
      //輸入結束
      case 0:
        printf("遊戲結束\n");
        playAgain = false;
        break;
      default:
        break;
    }

    printf("按 Enter 鍵繼續...\n");
    getchar();
    getchar();
    //判斷玩家是否死亡
    if ((player.health <= 0) || ((player.mana <= 0) && (choice != 1)) \
      || (player.wealth < 0) || (player.fatigue >= player.fatigueLimit)) {
      playAgain = false;
      }
    }
  
    system("clear");
    //各種死亡結局
    if (player.health <= 0) {
        printf(ANSI_COLOR_PINK \
          "你失血過多，毫無血氣，被誤以為是不死族，不幸死於路過的菜鳥冒險家手中 (´◓Д◔`) \n"\
          ANSI_COLOR_RESET);
    } 
    else if ((player.mana <= 0) && (choice != 1)) {
        printf(ANSI_COLOR_PINK \
          "你的魔力耗盡了，缺乏魔力的你陷入無盡的幻境，最終昏迷不醒 (´～`)Zzz...\n" \
          ANSI_COLOR_RESET);
    } 
    else if (player.wealth < 0) {
        printf(ANSI_COLOR_PINK \
          "你的財富所剩無幾，無法購買食物或住宿的你 * 窮 * 死 * 了 * ∠( ᐛ 」∠)_ fuwahahaha!!!\n"\
          ANSI_COLOR_RESET);
    } 
    else if (player.fatigue >= player.fatigueLimit) {
        printf(ANSI_COLOR_PINK \
          "你過於疲憊，蹣跚的踏入了魔物的巢穴，成為它們口中的耶穌 Σ(ﾟдﾟ)\n"\
          ANSI_COLOR_RESET);
    } 
    else if(level == 30) {
        printf(ANSI_COLOR_PINK "\n***********\n" ANSI_COLOR_RESET);
        printf(ANSI_COLOR_PINK "\n!!恭喜過關!!\n" ANSI_COLOR_RESET);
        printf(ANSI_COLOR_PINK "\n***********\n" ANSI_COLOR_RESET);
        printf(ANSI_COLOR_PINK "非常感謝您的遊玩~\n" ANSI_COLOR_RESET);
    }
    else {
        printf(ANSI_COLOR_PINK \
          "看來又是個過不了關的，動動你的小腦袋吧 (΄ಢ◞౪◟ಢ‵) 笑死\n" \
          ANSI_COLOR_RESET);
    }
    //顯示最後結果
    printf("最終屬性值：\n");
    displayStatus(&player);
    printf(ANSI_COLOR_PINK "\n*** level %d ***\n" ANSI_COLOR_RESET, level);
    printf("\n剩下層數：" ANSI_COLOR_PINK "%d\n" ANSI_COLOR_RESET, 30 - level);

    return 0;
}




