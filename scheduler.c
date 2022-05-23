#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_USER 5
#define MAX_RESERVATION 100
#define MAX_ID 10
#define MAX_PW 10

int disp_menu();
int leap_check(const int);
int is_enroll_num(const char*);
void disp_calendar();
void enroll_num();
void deleteID(const int);
void login_out();

char user_id[MAX_USER][MAX_ID];
char user_pw[MAX_USER][MAX_PW];
int mdays[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
int login_check = 0;
int cur_user = 0;

int main(void){
    int input;
    while(1){
        input = disp_menu();

        switch(input){
            case 0:
                exit(0);
                break;
            case 1:
                enroll_num();
                break;
            case 2:
                login_out();
                break;
            case 4:
                disp_calendar();
                break;
          default:
               printf("Wrong Enter.\nPlease Enter again.\n\n");
               break;
        }
    }

}

int disp_menu(){
    int tmp;
    printf("-------------[ 회의실 예약 시스템 ]-------------\n");
    printf("1. 사번 등록/삭제\n");
    printf("2. 로그인/로그아웃\n");
    printf("3. 회의실 예약\n");
    printf("4. 예약 상태 조회\n");
    printf("0. 종료\n");
    printf("-----------------------------------------------\n");
    printf("> 번호 선택 : ");
    fflush(stdin);
    scanf("%d",&tmp);
    return tmp;
}

void disp_calendar(){
    int idx, y, m, check, start, day;
    int total = 0, cnt = 0;
    printf("Enter Year and Month = ");
    scanf("%d %d", &y, &m);
    
    for(idx=1; idx<y; idx++){
        if(leap_check(idx)) total += 366;
        else total += 365;
    }

    for(idx=0; idx<m-1; idx++){
        total += mdays[idx];
    }
    if(leap_check(y) && (m>2)) total++;
    total++;    

    start = total%7;
    day = mdays[m-1];
    if(m==2 && leap_check(y)) day++;

    
    printf("\n===========< %4d. %2d >===========\n\n", y,m);
	printf("------------------------------------\n");
	printf("%5s%5s%5s%5s%5s%5s%5s\n", "Sun","Mon","Tue","Wed","Thu","Fri","Sat");
	printf("------------------------------------\n");
    for(idx=0;idx<start;idx++){
        printf("%5c", ' ');
        cnt++;
    }
    for(idx=1;idx<=day;idx++){
        printf("%5d",idx);
        cnt++;
        if(cnt%7==0) printf("\n");
    }
    printf("\n------------------------------------\n\n");
}

int leap_check(const int y){
    if((y%4==0 && y%100!=0) || y%400==0) return 1;
    else return 0;
}

void enroll_num(){
    char tmp_id[100], tmp_pw[100];
    int idx;

    if(login_check == 1){
        printf("@ 로그아웃 후 사용할 수 있습니다!\n");
        return;
    }
    if(cur_user >= MAX_USER){
        printf("@ 아이디는 최대 %d개 등록할 수 있습니다.\n",MAX_USER);
        return;
    }

    printf("아이디를 입력하세요 : ");
    fflush(stdin);
    fgets(tmp_id,100,stdin);
    tmp_id[strlen(tmp_id)-1] = '\0';
    
    if(strlen(tmp_id)>MAX_ID){
        printf("아이디의 길이는 %d이하로 입력하세요.\n",MAX_ID);
        return;
    }

    for(idx=0;idx<cur_user;idx++){
        if(!strcmp(user_id[idx],tmp_id)){
            deleteID(idx);
            return;
        }
    }
    

    printf("비밀번호를 입력하세요 : ");
    fflush(stdin);
    fgets(tmp_pw,100,stdin);
    tmp_pw[strlen(tmp_pw)-1] = '\0';
    if(strlen(tmp_pw) > MAX_PW){
        printf("비밀번호의 길이는 %d이하로 입력하세요. \n",MAX_PW);
        return;
    }
    
    strcpy(user_id[cur_user],tmp_id);
    strcpy(user_pw[cur_user],tmp_pw);
    cur_user++;
    
    printf("등록된 사번 목록 : \n");
    for(idx=0;idx<cur_user;idx++) printf("%s  ",user_id[idx]);
    printf("\n");
    
}

void deleteID(const int del_idx){
    int idx;
    char del_check;
    printf("del_idx = %d\n",del_idx);

    printf("이미 등록된 아이디입니다.\n해당 아이디를 삭제하시겠습니까? Y/N ");
    fflush(stdin);
    scanf("%c",&del_check);
    if(del_check == 'y' || del_check =='Y'){
        if(cur_user == 1){
            strcpy(user_id[del_idx], "\0");
            strcpy(user_pw[del_idx],"\0");
            cur_user--;
        }
        else{
            for(idx=del_idx;idx<cur_user-1;idx++){
                strcpy(user_id[idx], user_id[idx+1]);
                strcpy(user_pw[idx],user_pw[idx+1]);
            }
            cur_user--;
            strcpy(user_id[cur_user], "\0");
            strcpy(user_pw[cur_user],"\0");
        }
        printf("@ 아이디가 삭제되었습니다!\n");
    }
    printf("등록된 사번 목록 : \n");
    for(idx=0;idx<cur_user;idx++) printf("%s  ", user_id[idx]);
    printf("\n");
}

void login_out(){
    char tmp_id[100], tmp_pw[100];
    int tmp_login_check = 0;

    if(login_check==1){
        printf("# 로그아웃 되었습니다!\n");
        login_check = 0;
        return;
    }

    printf("아이디를 입력하세요 : ");
    fflush(stdin);
    fgets(tmp_id,100,stdin);
    tmp_id[strlen(tmp_id)-1] = '\0';
    tmp_login_check = is_enroll_num(tmp_id);
    if(tmp_login_check < 0){
        printf("@ %s는 등록되지 않은 아이디입니다.\n",tmp_id);
        return;
    }
    
    printf("비밀번호를 입력하세요 : ");
    fflush(stdin);
    fgets(tmp_pw,100,stdin);
    tmp_pw[strlen(tmp_pw)-1] = '\0';
    if(strcmp(user_pw[tmp_login_check],tmp_pw)){
        printf("@ 비밀번호가 잘못되었습니다!\n");
        return;
    }

    printf("# 로그인 되었습니다!\n");
    login_check = 1;
}

int is_enroll_num(const char* id){
    int idx;

    for(idx=0;idx<cur_user;idx++) if(!strcmp(user_id[idx], id)) return idx;
    return -1;
}