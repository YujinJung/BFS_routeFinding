#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXOFPATH 400
#define OBSTACLE 100
#define EXVALUE 21

int room[EXVALUE+1][EXVALUE+1] = {0,};
int path[EXVALUE+1][EXVALUE+1] = {0,};
int destination[2] = {0,};
char hiddenObstacle[EXVALUE+1][EXVALUE+1] = {0,};


int queue[1000][3] = {0,}; //경로값용 큐
int stackPath[1000][3] = {0,}; //경로이동용 스택

int countQ = 0;

void init(int, int);
void printRoom(void);
void setThePath(void);
void printPath(void);
void findPathValue(int, int);
void findPathRoute(int, int, int);
void inQueue(int, int, int);
void inStack(int, int, int);
void resetStack(int);
void Move(int, int, int);

int main(void){
    int curX, curY;



    path[3][3] = path[7][3] = path[10][2] = path[4][2]= path[5][3] = path[5][6] = path[4][8] = path[4][9]
       = path[8][3] = path[3][8] = path[6][5] = path[9][2] = path[9][7] = path[6][10] = path[5][9] = 100;

    hiddenObstacle[6][4] = OBSTACLE;
    hiddenObstacle[7][5] = 1;
    init(0,0);
    printRoom();
    findPathValue(0,0);
    printRoom();
    printPath();


    while (1) {
        printf("어디로 가시겠습니까? x y : ");
        scanf("%d %d", &curX, &curY);
        printf("%d\n\n", room[curX-1][curY-1]);     
        if (room[curX][curY] >= 100) {
            printf("선택하신 곳은 장애물이 있습니다. 다시 선택해주십시오.");
        }
        else{
            destination[0] = curX;
            destination[1] = curY;
            break;
        }
    }
   
   printf("위치 : %d, %d \n", destination[0], destination[1]);
   findPathRoute(destination[0], destination[1], room[destination[0]][destination[1]]);
   for(curX = 0; curX < room[destination[0]][destination[1]]; curX++){
       printf("(x : %d, y : %d)\n", stackPath[curX][0], stackPath[curX][1]);
    }   
   setThePath();
   printPath();   
    
   Move(0,0,0);
   return 0;
}

void init(int srcX, int srcY){ // srcX, srcY = 시작점
    int i, j;

    for(i = 0; i < EXVALUE; i++){
        for(j = 0; j < EXVALUE; j++){
            if(path[srcX + i][srcY + j] < OBSTACLE)
                room[srcX + i][srcY + j] = 0;
            else
                room[srcX + i][srcY + j] = OBSTACLE;
         }
        for(j = 0; j < 3; j++){
            queue[i][j] = 0;
        }
    }
    room[srcX][srcY] = 1; // 초기값
    countQ = 0;
}

void printRoom(void){
    int i, j;

    printf("Room\n");
    for(i = 0; i < EXVALUE; i++) {
        for(j = 0; j < EXVALUE; j++) 
            printf(" %3d ", room[i][j]);
    printf("\n");
    }
    printf("\n\n");
//        printf("(x : %d, y : %d)",recordPath[i][0], recordPath[i][1]);
//    for(i = 0; i < room[destination[0]][destination[1]]; i ++)
    printf("\n\n\n");
}

void setThePath(){
    int i;
    int maxLegth;

    maxLegth = room[destination[0]][destination[1]];
    for(i = 0; i < maxLegth; i++){
       queue[i][0] = stackPath[maxLegth-i][0];
       queue[i][1] = stackPath[maxLegth-i][1];
       queue[i][2] = stackPath[maxLegth-i][2];
    }
    for(i = 0; i < maxLegth; i++){
       path[queue[i][0]][queue[i][1]] = 1;
    }

}

void printPath() {
    int i, j;

    for (i = 0; i < EXVALUE; i++) {
        for (j = 0; j < EXVALUE; j++) {
            if(path[i][j] == 1)
                printf(" $ ");
            else if(path[i][j] > 99)
                printf(" o ");
            else if(path[i][j] < 0)
                printf(" M ");
            else
                printf("   ");
        }
        printf("\n");
    }
    printf("\n\n\n");
}

void findPathValue(int srcI, int srcJ){
    int i, j, l;
    int curPos = 0;

    init(0,0);
    inQueue(srcI, srcJ, 1);
    
    while(curPos < countQ && (i != EXVALUE || j != EXVALUE)){
        i = queue[curPos][0];
        j = queue[curPos][1];
        l = queue[curPos][2];
        
        //right
        if(i < EXVALUE - 1 && room[i][j+1] == 0){
            inQueue(i, j+1, l+1);
            room[i][j+1] = l+1;
        }
        //down
        if(i < EXVALUE - 1 && room[i+1][j] == 0){
            inQueue(i+1,j,l+1);
            room[i+1][j] = l+1;
        }
        //left
        if(j > 0 && room[i][j-1] == 0){
            inQueue(i,j-1,l+1);
            room[i][j-1] = l+1;
        }
        //up
        if(i > 0 && room[i-1][j] == 0){
            inQueue(i-1,j,l+1);
            room[i-1][j] = l+1;
        }
        curPos++; 
        //system("clear");
        //printRoom();
        //sleep(1);
    }   
    printRoom();
    scanf("%d", &l);
}

//curPos 받을 때 최대 값으로
void findPathRoute(int srcI, int srcJ, int curPos){
    int i, j, l, k;

    inStack(srcI, srcJ, curPos);

    if((srcI == 0 && srcJ == 0) || curPos == 0){
        return;
    }

    /*for(i = 0; i < room[destination[0]][destination[1]]; i++){
       printf("(x : %d, y : %d)\n", stackPath[i][0], stackPath[i][1]);
    }*/


    if(curPos > 0){
        //up
        if(srcI - 1 > 0 && room[srcI-1][srcJ] < OBSTACLE && room[srcI-1][srcJ] != 0 && stackPath[curPos][2] - 1 == room[srcI-1][srcJ]){
            findPathRoute(srcI-1,srcJ,curPos-1);
        }
        //left
        if(srcJ - 1 > 0 && room[srcI][srcJ-1] < OBSTACLE && room[srcI][srcJ-1] != 0 && stackPath[curPos][2] -1 == room[srcI][srcJ-1]){
            findPathRoute(srcI,srcJ-1,curPos-1);
        }
        //right
        if(srcJ + 1 < EXVALUE && room[srcI][srcJ+1] < OBSTACLE && room[srcI][srcJ+1] != 0 && stackPath[curPos][2] -1 == room[srcI][srcJ+1]){
            findPathRoute(srcI,srcJ+1,curPos-1);
        }
        //down
        if(srcI + 1 < EXVALUE && room[srcI+1][srcJ] < OBSTACLE && room[srcI+1][srcJ] != 0 && stackPath[curPos][2] - 1 == room[srcI+1][srcJ]){
            findPathRoute(srcI+1,srcJ,curPos-1);
        }
       
    }
}

void Move(int srcI, int srcJ, int count){
    int i, j;

    findPathValue(srcI, srcJ);
    findPathRoute(srcI, srcJ, count);
    setThePath();
    
    while(count<room[destination[0]][destination[1]]){
        system("clear");
        printRoom();
        i = stackPath[count][0];
        j = stackPath[count][1];

        printf("\n\n\ncurrent Path : %d %d , Count : %d \n", i, j, count);

        if(hiddenObstacle[i][j] == OBSTACLE){
            room[i][j] = OBSTACLE;
            path[i][j] = OBSTACLE;
            resetStack(count);
            Move(stackPath[count-1][0], stackPath[count-1][1], count-1);
        }
        else{
            path[i][j] = -1;
            printPath();
            count++;
        }
        path[i][j] = 0;
        sleep(1);
    }

}


void inQueue(int srcI, int srcJ, int l){
    queue[countQ][0] = srcI;
    queue[countQ][1] = srcJ;
    queue[countQ][2] = l;
    countQ++;
}

void inStack(int srcI, int srcJ, int curPos){
    stackPath[curPos][0] = srcI;
    stackPath[curPos][1] = srcJ;
    stackPath[curPos][2] = room[srcI][srcJ];
}

void resetStack(int src){
    int k, m;
   
    for(m = 0; src + m < room[destination[0]][destination[1]]; m++){
        for(k = 0; k < 3; k++){
            stackPath[src + m][k] = 0;
         }
     }
}
