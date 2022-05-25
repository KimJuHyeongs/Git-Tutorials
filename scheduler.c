#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_USER 5
#define MAX_RESERVATION 100
#define MAX_ID 10
#define MAX_PW 10
#define MAX_SCHEDULE 100

static int disp_menu();
static int leap_check(const int);
static int is_enroll_num(const char[][MAX_ID], const char*, const int);
static void disp_calendar(const int, const int);
static void enroll_num(char[][MAX_ID], char[][MAX_PW], int*, int*);
static void deleteID(char[][MAX_ID], char[][MAX_PW], int*, const int);
static void login_out(char[][MAX_ID], char[][MAX_PW], int*, int*);
static int verify_pwd(const char[][MAX_PW], const int, const char*);
static int on_schedule(const int, const int, const int);
static void input_schedule(const char[][MAX_ID], const int);
static void search_schedule(const int);


int s_day[MAX_SCHEDULE][3];
char s_id[MAX_SCHEDULE][MAX_ID];
int mdays[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
int s_cnt = 0;

typedef enum{
    YEAR,
    MONTH,
    DAY
}DATE_IDX;


int main(void){
    int input;
    static char user_id[MAX_USER][MAX_ID];
    static char user_pw[MAX_USER][MAX_PW];
    static int cur_user = 0;
    static int login_check = -1;

    while(1){
        input = disp_menu();

        switch(input){
            case 0:
                exit(0);
                break;
            case 1:
                enroll_num(user_id, user_pw, &cur_user, &login_check);
                break;
            case 2:
                login_out(user_id, user_pw, &cur_user, &login_check);
                break;
            case 3:
                input_schedule(user_id, login_check);
                break;
            case 4:
                search_schedule(login_check);
                break;
          default:
               printf("Wrong Enter.\nPlease Enter again.\n\n");
               break;
        }
    }

}

static int disp_menu(){
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

static void disp_calendar(const int y, const int m){
    int idx, check, start, day;
    int total = 0, cnt = 0;
    
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
        if(on_schedule(y,m,idx) == 0) printf("%5d",idx);
        else{
            printf("%2c",' ');
            printf("*");
            printf("%2d",idx);
        }
        cnt++;
        if(cnt%7==0) printf("\n");
    }
    printf("\n------------------------------------\n\n");
}

static int leap_check(const int y){
    if((y%4==0 && y%100!=0) || y%400==0) return 1;
    else return 0;
}

static void enroll_num(char user_id[][MAX_ID], char user_pw[][MAX_PW]
                       ,int* cur_user, int* login_check){
    char tmp_id[100], tmp_pw[100];
    int idx;

    if(*login_check >= 0){
        printf("@ 로그아웃 후 사용할 수 있습니다!\n");
        return;
    }
    
    if(*cur_user >= MAX_USER){
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

    for(idx=0;idx<*cur_user;idx++){
        if(!strcmp(user_id[idx],tmp_id)){
            deleteID(user_id, user_pw, cur_user, idx);
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
    
    strcpy(user_id[*cur_user],tmp_id);
    strcpy(user_pw[*cur_user],tmp_pw);
    (*cur_user)++;
    
    printf("등록된 사번 목록 : \n");
    for(idx=0;idx<*cur_user;idx++) printf("%s  ",user_id[idx]);
    printf("\n");
    
}

static void deleteID(char user_id[][MAX_ID], char user_pw[][MAX_PW]
                     ,int *cur_user, const int del_idx){
    int idx, pw_check;
    char del_check;
    char tmp_pw[100];

    printf("이미 등록된 아이디입니다.\n해당 아이디를 삭제하시겠습니까? Y/N ");
    fflush(stdin);
    scanf("%c",&del_check);
    if(del_check == 'y' || del_check =='Y'){
        printf("> 비밀번호 입력 : ");
        fflush(stdin);
        fgets(tmp_pw,100,stdin);
        tmp_pw[strlen(tmp_pw)-1] = '\0';

        pw_check = verify_pwd(user_pw,del_idx,tmp_pw);
        if(pw_check==0){
            printf("@ 비밀번호가 다릅니다. 삭제할 수 없습니다.\n");
            return;
        }

        if(*cur_user == 1){
            strcpy(user_id[del_idx], "\0");
            strcpy(user_pw[del_idx],"\0");
            (*cur_user)--;
        }
        else{
            for(idx=del_idx;idx<*cur_user-1;idx++){
                strcpy(user_id[idx], user_id[idx+1]);
                strcpy(user_pw[idx],user_pw[idx+1]);
            }
            (*cur_user)--;
            strcpy(user_id[*cur_user], "\0");
            strcpy(user_pw[*cur_user],"\0");
        }
        printf("@ 아이디가 삭제되었습니다!\n");
    }
    printf("등록된 사번 목록 : \n");
    for(idx=0;idx<*cur_user;idx++) printf("%s  ", user_id[idx]);
    printf("\n");
}

static int verify_pwd(const char user_pw[][MAX_PW], const int tmp_login_check
                      ,const char* tmp_pw){
    if(strcmp(user_pw[tmp_login_check],tmp_pw)) return 0;
    else return 1;
}

static void login_out(char user_id[][MAX_ID] ,char user_pw[][MAX_PW]
                      ,int *cur_user, int *login_check){
    char tmp_id[100], tmp_pw[100];
    int tmp_login_check = 0, pw_check = 0;

    if(*login_check >= 0){
        printf("# 로그아웃 되었습니다!\n");
        *login_check = -1;
        return;
    }

    printf("아이디를 입력하세요 : ");
    fflush(stdin);
    fgets(tmp_id,100,stdin);
    tmp_id[strlen(tmp_id)-1] = '\0';
    tmp_login_check = is_enroll_num(user_id, tmp_id, *cur_user);
    if(tmp_login_check < 0){
        printf("@ %s는 등록되지 않은 아이디입니다.\n",tmp_id);
        return;
    }
    
    printf("비밀번호를 입력하세요 : ");
    fflush(stdin);
    fgets(tmp_pw,100,stdin);
    tmp_pw[strlen(tmp_pw)-1] = '\0';
    pw_check = verify_pwd(user_pw, tmp_login_check, tmp_pw);
    if(!pw_check){
        printf("@ 비밀번호가 틀렸습니다!\n");
        return;
    }

    printf("# 로그인 되었습니다!\n");
    *login_check = tmp_login_check;
}

static int is_enroll_num(const char user_id[][MAX_ID], const char* id, const int cur_user){
    int idx;

    for(idx=0;idx<cur_user;idx++) if(!strcmp(user_id[idx], id)) return idx;
    return -1;
}

static int on_schedule(const int y, const int m, const int d){
    int idx, tmp_cnt=0;

    for(idx=0;idx<s_cnt;idx++){
        if((s_day[idx][YEAR] == y) && (s_day[idx][MONTH] == m) && s_day[idx][DAY] == d) tmp_cnt++;
    }

    return tmp_cnt;
}

static void input_schedule(const char user_id[][MAX_ID], const int login_check){
    int tmp_y, tmp_m, tmp_d;

    if(login_check < 0){
        printf("@ 로그인 후 사용할 수 있습니다.\n");
        return;
    }

    printf("스케줄을 입력할 년, 월을 입력하세요 : ");
    fflush(stdin);
    scanf("%d %d", &tmp_y, &tmp_m);
    
    disp_calendar(tmp_y, tmp_m);

    printf("스케줄을 입력할 날짜(일)를 입력하세요 : ");
    fflush(stdin);
    scanf("%d",&tmp_d);

    s_day[s_cnt][YEAR] = tmp_y;
    s_day[s_cnt][MONTH] = tmp_m;
    s_day[s_cnt][DAY] = tmp_d;
    strcpy(s_id[s_cnt], user_id[login_check]);
    s_cnt++;
}

static void search_schedule(const int login_check){
    int tmp_y, tmp_m, tmp_d, tmp_cnt;

    if(login_check < 0){
        printf("@로그인 후 사용할 수 있습니다.\n");
        return;
    }

    printf("스케줄을 입력할 년, 월을 입력하세요 : ");
    fflush(stdin);
    scanf("%d %d", &tmp_y, &tmp_m);
    disp_calendar(tmp_y, tmp_m);

    printf("스케줄을 입력할 날짜(일)를 입력하세요 : ");
    fflush(stdin);
    scanf("%d",&tmp_d);

    tmp_cnt = on_schedule(tmp_y, tmp_m, tmp_d);
    printf("%d년 %d월 %d일에 등록된 스케줄 수는 %d개 입니다.\n", tmp_y, tmp_m, tmp_d, tmp_cnt);
}