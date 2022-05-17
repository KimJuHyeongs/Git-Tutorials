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
            case 1:
                enroll_num();
            case 2:
                login_out();
            case 4:
                disp_calendar();
                break;
          default:
               printf("Wrong Enter.\nPlease Enter again.\n");
        }
    }

}

int disp_menu(){
    int tmp;
    printf("-------------[ ?šŒ?˜?‹¤ ?˜ˆ?•½ ?‹œ?Š¤?…œ ]-------------\n");
    printf("1. ?‚¬ë²? ?“±ë¡?/?‚­? œ\n");
    printf("2. ë¡œê·¸?¸/ë¡œê·¸?•„?›ƒ\n");
    printf("3. ?šŒ?˜?‹¤ ?˜ˆ?•½\n");
    printf("4. ?˜ˆ?•½ ?ƒ?ƒœ ì¡°íšŒ\n");
    printf("0. ì¢…ë£Œ\n");
    printf("-----------------------------------------------\n");
    printf("> ë²ˆí˜¸ ?„ ?ƒ : ");
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
    total++;    //1?¼ ?ˆ„? 

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
    char *tmp_id, *tmp_pw;
    int idx;

    if(login_check == 1){
        printf("@ ë¡œê·¸?•„?›ƒ ?›„ ?‚¬?š©?•  ?ˆ˜ ?ˆ?Šµ?‹ˆ?‹¤!\n");
        return;
    }
    if(cur_user >= MAX_USER){
        printf("@ ?“±ë¡ëœ ?¸?›?´ ì´ˆê³¼?˜?—ˆ?Šµ?‹ˆ?‹¤.\n");
        return;
    }

    printf("?‚¬ë²ˆì„ ?…? ¥?•˜?„¸?š” : ");
    fflush(stdin);
    fgets(tmp_id,20,stdin);
    tmp_id[strlen(tmp_id)-1] = '\0';
    if(strlen(tmp_id)>MAX_ID){
        printf("?‚¬ë²ˆì˜ ê¸¸ì´?Š” %d ?´?•˜ ?…?‹ˆ?‹¤.\n",MAX_ID);
        return;
    }

    for(idx=0;idx<cur_user-1;idx++){
        if(!strcmp(user_id[idx],tmp_id)){
            deleteID(idx);
            return;
        }
    }
    

    printf("ë¹„ë??ë²ˆí˜¸ë¥? ?…? ¥?•˜?„¸?š” : ");
    fflush(stdin);
    fgets(tmp_pw,20,stdin);
    tmp_pw[strlen(tmp_pw)-1] = '\0';
    if(strlen(tmp_pw) > MAX_PW){
        printf("ë¹„ë??ë²ˆí˜¸?˜ ê¸¸ì´?Š” %d ?´?•˜ ?…?‹ˆ?‹¤. \n",MAX_PW);
        return;
    }
    
    strcpy(user_id[cur_user],tmp_id);
    strcpy(user_pw[cur_user],tmp_pw);
    cur_user++;
    
    printf("?“±ë¡ëœ ?‚¬ë²? ? •ë³?\n");
    for(idx=0;idx<cur_user-1;idx++) printf("%s  ",user_id[idx]);
    printf("\n");
    
}

void deleteID(const int del_idx){
    int idx;
    char del_check;

    printf("?´ë¯? ?“±ë¡ëœ ?‚¬ë²ˆì…?‹ˆ?‹¤.\n?“±ë¡ëœ ?•„?´?””ë¥? ?‚­? œ?•˜?‹œê² ìŠµ?‹ˆê¹?? Y/N ");
    scanf("%c",&del_check);
    if(del_check == 'y' || del_check =='Y'){
        for(idx=del_idx;idx<cur_user-2;idx++){
            strcpy(user_id[idx], user_id[idx+1]);
            strcpy(user_pw[idx],user_pw[idx+1]);
            cur_user--;
            printf("@ ?‚­? œ ?˜?—ˆ?Šµ?‹ˆ?‹¤!\n");
        }
    }
    printf("?“±ë¡ëœ ?‚¬ë²? ? •ë³?\n");
    for(idx=0;idx<cur_user-1;idx++) printf("%s  ", user_id[idx]);
    printf("\n");
}

void login_out(){
    char *tmp_id, *tmp_pw;
    int tmp_login_check = 0;

    if(login_check==1){
        printf("# ë¡œê·¸?•„?›ƒ ?˜?—ˆ?Šµ?‹ˆ?‹¤!\n");
        login_check = 0;
        return;
    }

    printf("?‚¬ë²ˆì„ ?…? ¥?•˜?„¸?š” : ");
    fflush(stdin);
    fgets(tmp_id,MAX_ID+1,stdin);
    tmp_id[strlen(tmp_id)-1] = '\0';
    tmp_login_check = is_enroll_num(tmp_id);
    if(tmp_login_check){
        printf("@ %s?Š” ?“±ë¡ë˜ì§? ?•Š??? ?‚¬ë²ˆì…?‹ˆ?‹¤.\n",tmp_id);
        return;
    }
    
    printf("ë¹„ë??ë²ˆí˜¸ë¥? ?…? ¥?•˜?„¸?š” : ");
    fflush(stdin);
    fgets(tmp_pw,MAX_PW+1,stdin);
    tmp_pw[strlen(tmp_pw)-1] = '\0';
    if(strcmp(user_pw[tmp_login_check],tmp_pw)){
        printf("@ ë¹„ë??ë²ˆí˜¸ê°? ???? ¸?Šµ?‹ˆ?‹¤!\n");
        return;
    }

    printf("# ë¡œê·¸?¸ ?˜?—ˆ?Šµ?‹ˆ?‹¤!\n");
    login_check = 1;
}

int is_enroll_num(const char* id){
    int idx;

    for(idx=0;idx<MAX_USER;idx++) if(!strcmp(user_id[idx], id)) return idx;
    return 0;
}