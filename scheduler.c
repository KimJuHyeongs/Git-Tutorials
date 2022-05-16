#include<stdio.h>
#include<stdlib.h>

int print_menu();
void print_calendar();
int leap_check(int);

int mdays[12] = {31,28,31,30,31,30,31,31,30,31,30,31};

int main(void){
    int input;
    while(1){
        input = print_menu();

        switch(input){
           case 0:
               exit(0);
           case 1:
                print_calendar();
                break;
          default:
               printf("Wrong Enter.\nPlease Enter again.\n");
        }
    }

}

int print_menu(){
    int tmp;
    printf("-------------------------------\n");
    printf("0. End\n");
    printf("1. Enter day and print calendar\n");
    printf("Enter Menu = ");
    scanf("%d",&tmp);
    return tmp;
}

void print_calendar(){
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
    total++;    //1일 누적

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

int leap_check(int y){
    if((y%4==0 && y%100!=0) || y%400==0) return 1;
    else return 0;
}