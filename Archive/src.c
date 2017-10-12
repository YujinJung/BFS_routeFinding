#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXOFPATH 100
#define OBSTACLE 100
#define EXVALUE 21

int room[EXVALUE+1][EXVALUE+1] = {0,  };
int path[EXVALUE+1][EXVALUE+1] = { 0, };
char hiddenObstacle[EXVALUE+1][EXVALUE+1] = {0,};
char recordPath[MAXOFPATH][2] = {0,};
int qu[1000][3] = {0,};
int destination[3] = { 0, 0};
int count = 0;

void printRoom(void); // print room
void printPath(void); // print path
void findPathValue(int, int);
void setThePath(int); 
void init(int, int);
void Move(int, int, int);
void self(int, int, int, int);

void enQueue(int srcX, int srcY, int l){
    qu[count][0] = srcX;
    qu[count][1] = srcY;
    qu[count][2] = l;
    count++;
}

int main(void)
{
    int i = 0;
    int j = 0;
    char ob = 0;
    int x = 0;
    int y = 0;
    int pathValue;
    int currentValue = 0;
	
    path[3][3] = path[7][3] = path[10][2] = path[4][2]= path[5][3] = path[5][6] = path[4][8] = path[4][9] 
       = path[8][3] = path[3][8] = path[6][5] = path[9][2] = path[9][7] = path[6][10] = path[5][9] = 100;
    
    init(0,0);
    printRoom();
    findPathValue(0,0);
    printRoom();
    // Add Obstacle
    while(1){

        printf("장애물을 추가해주십시오 End : 0 0");
        scanf("%d %d", &x, &y);
            
        if(x == 0 && y == 0)
            break;

        hiddenObstacle[x][y] = 100;
        system("clear");
        printf("%d %d\n",x, y);
        printRoom();
    }
    Move(0,0,0);
}

void init(int srcX, int srcY){
    int i, j;

    for(i = 0; i < EXVALUE; i++){
        for(j = 0; j < EXVALUE; j++){
            if(path[i][j] < OBSTACLE)
                room[i][j] = 0;
            else
                room[i][j] = OBSTACLE;
        }
     }
     room[0][0] = 1;

   /* //기준점으로 부터 벽치기
    for(i = 0; srcX + i < EXVALUE || srcY + i < EXVALUE; i++){
        if(srcX + i < EXVALUE){
            room[srcX + i][srcY - 1] = 100;
        }    
        if(srcY + i < EXVALUE) {
            room[srcX - 1][srcY + i] = 100;
        }
    }*/
}

void printRoom() {
    int i, j;
     
    printf("Room\n");   
    for (i = 0; i < EXVALUE; i++) {
        for (j = 0; j < EXVALUE; j++) {
      	    printf(" %3d ", room[i][j]);
        }
    printf("\n");
    }
    printf("\n\n");
    for(i = 0; i < room[destination[0]][destination[1]]; i ++)
        printf("(x : %d, y : %d)",recordPath[i][0], recordPath[i][1]);
    printf("\n\n\n");
}

void printPath() {
    int i, j;
    
    for (i = 0; i < EXVALUE; i++) {
        for (j = 0; j < EXVALUE; j++) {
            if(path[i][j] == 1)
                printf(" v ");
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

void findPathValue(int srcX, int srcY) {
    int i, j;
    int pos = 0;
    

    init(srcX, srcY);
     
    enQueue(srcX, srcY, 1);
    
    while(pos < count && (qu[pos][0] != EXVALUE - 1 || qu[pos][1] != EXVALUE - 1)){
        
        if (qu[pos][0] > 0 && room[qu[pos][0] - 1][qu[pos][1]] == 0 ){
            enQueue(qu[pos][0] - 1, qu[pos][1], qu[pos][2] + 1);
            room[qu[pos][0] - 1][qu[pos][1]] = qu[pos][2] + 1;
        }
        if(qu[pos][0] < EXVALUE - 1 && room[qu[pos][0] + 1][qu[pos][1]] == 0){
            enQueue(qu[pos][0] + 1, qu[pos][1], qu[pos][2] + 1);
            room[qu[pos][0] + 1][qu[pos][1]] = qu[pos][2] + 1;
        }

        if (qu[pos][1] > 0 && room[qu[pos][0]][qu[pos][1] - 1] == 0){
            enQueue(qu[pos][0], qu[pos][1] - 1, qu[pos][2] + 1);
            room[qu[pos][0]][qu[pos][1] - 1] = qu[pos][2] + 1;
        }
        if(qu[pos][1] < EXVALUE - 1 && room[qu[pos][0]][qu[pos][1] + 1] == 0){
            enQueue(qu[pos][0], qu[pos][1] + 1, qu[pos][2] + 1);
            room[qu[pos][0]][qu[pos][1] + 1] = qu[pos][2] + 1;
        }
        pos++;
    }
    destination[2] = qu[pos][2];

    /*
    for (i = 0; i < EXVALUE; i++) {
       for (j = 0; j < EXVALUE; j++) {
            // 초기 위치라면 1로 set
	    if (i == 0 && j == 0) { 
                room[srcX + i][srcY + j] = 1;
		continue;
	    }
            
            // 현재 위치가 장애물이라면 path array에도 표시 / 아니라면 '0'을 표시
	    //if (room[srcX + i][srcY + j] >= OBSTACLE){  
            //   path[srcX + i][srcY + j] = OBSTACLE;   
            //    room[srcX + i][srcY + j] = 0;
            //    path[srcX + i][srcY + j] = 0;   
            //}
 
            if(room[srcX + i][srcY + j] >= OBSTACLE)
                continue;
            
            // 현재 위의 값이 장애물이거나 지나온 곳이 아니라면
            // 왼쪽도 그런 상태인지 chk 후 여기는 못 오는 곳이면 continue;
            // 왼쪽은 정상적인 길이라면 왼쪽 값 + 1  
            if (room[srcX + i][srcY + j - 1] >= OBSTACLE || room[srcX + i][srcY + j - 1] == 0){ // 있는 곳의 왼쪽 위쪽을 확인하여 
                if (room[srcX + i - 1][srcY + j] >= OBSTACLE || room[srcX + i - 1][srcY + j] == 0)
                    continue;
                else 
                    room[srcX + i][srcY + j] = room[srcX + i - 1][srcY + j] + 1;
            }
            // 위쪽이 정상적인 길이라면 왼쪽이 정상인지 chk 후
            // 정상이 아니라면 위의 값에서 + 1
	    else if (room[srcX + i - 1][srcY + j] >= OBSTACLE || room[srcX + i - 1][srcY + j] == 0){ 
                room[srcX + i][srcY + j] = room[srcX + i][srcY + j - 1] + 1;
            }
            // 둘 다 정상이라면 둘 중 작은거 + 1
            else 
                room[srcX + i][srcY + j] = (room[srcX + i][srcY + j - 1] <= room[srcX + i - 1][srcY + j]) ? (room[srcX + i][srcY + j - 1] + 1) : (room[srcX + i - 1][srcY + j] + 1);
        }
    }*/
}

void Move(int srcX, int srcY, int i){
    int j;
    int pathValue; 
    int curX = srcX;
    int curY = srcY;

    if(i == 0){
         while (1) {
                printf("어디로 가시겠습니까? x y : ");
                scanf("%d %d", &curX, &curY);
                printf("%d\n\n", room[curX][curY]);
                pathValue = room[curX][curY];
                if (pathValue >= 100) {
                    printf("선택하신 곳은 장애물이 있습니다. 다시 선택해주십시오.");
                }
                else{
                    destination[0] = curX;
                    destination[1] = curY;
                    break;
                }
        }
    }
    printf("aaa\n");
    findPathValue(srcX, srcY);
    if(room[destination[0]][destination[1]] <= 0){
        i--;
        Move(recordPath[i][0],recordPath[i][1], i);
    }
    
    setThePath(i);
    printPath();
        
    curX = srcX;
    curY = srcY;   
 
    //Print Move
    while(1){
        system("clear");
        printRoom();
        //printf("\n\n\ncurrent Path : %d %d , Count : %d \n", curX, curY, count);
        printf("\n\n\ncurrent Path : %d %d , Count : %d \n",recordPath[i][0], recordPath[i][1], i);
        
        
        if(path[curX][curY] > 100)
            continue;
        else
            path[curX][curY] = -1; //Current location
        
        printPath();  //Print patha
        
        if(path[recordPath[i+1][0]][recordPath[i+1][1]] >= OBSTACLE){
            path[recordPath[i+1][0]][recordPath[i+1][1]] = OBSTACLE;
            Move(recordPath[i][0], recordPath[i][1], i);

        }
        
        //path[curX][curY] = 0; //Current Path
/*
        if(path[curX + 1][curY] == 1){  //Down path is Path?
            if(hiddenObstacle[curX + 1][curY] >= OBSTACLE){ //Obstacle?
                path[curX + 1][curY] = OBSTACLE;  
                room[curX + 1][curY] = OBSTACLE;
                if(path[curX][curY + 1] >= OBSTACLE){//Right path check
                    path[curX + 1][curY] = OBSTACLE;
                    path[curX][curY] = OBSTACLE;   // 현재 위치를 장애물 표시 후 한칸 뒤로
                    count--;
                    Move(recordPath[count][0],recordPath[count][1]);
                    sleep(1);
                    //그리고 뭔가 호출해야되는데..
                }else{
                    findPathValue(curX,curY);
                    if(path[destination[0]][destination[1]] > 0){
                        Move(curX, curY);
                        sleep(1);
                    } else{
                        count--;
                        Move(recordPath[count][0],recordPath[count][1]);
                        sleep(1);
                    }
                }
            }        //이쪽으로 가는 경로 찾기
            else{
                path[curX][curY] = 0;
                curX = curX + 1;
            }
        }else if(path[curX][curY + 1] == 1){
            if(hiddenObstacle[curX][curY + 1] >= OBSTACLE){
                path[curX][curY + 1] = OBSTACLE;
                if(path[curX + 1][curY] >= OBSTACLE){
                    path[curX][curY] = OBSTACLE;
                    path[curX + 1][curY] = OBSTACLE;
                    count--;
                    Move(recordPath[count][0],recordPath[count][1]);
                    sleep(1);
                } else {
                    findPathValue(curX, curY);
                    if(path[destination[0]][destination[1]] > 0){
                        Move(curX, curY);
                        sleep(1);
                    }
                    else{
                        count--;
                        Move(recordPath[count][0], recordPath[count][1]);
                        sleep(1);
                    }
                }
            }else{
                path[curX][curY] = 0;
                curY = curY + 1;
            }
        }
  */ 
        if(room[curX][curY] == pathValue)
            break;
        i++;
        sleep(1);
    }       // find하고 어떻게 하지 무브 다시 돌림? 
    
}

void setThePath(int cnt){
    int x = destination[0];
    int y = destination[1];
    int i = 0;
    int pathValue = room[x][y];
    int tempX, tempY;
    
    if(destination[2] != room[x][y]){
        printf("경로가 없습니다\n");
        return;
    }
    //Set the path
    for (i = 0; i < pathValue - cnt; i++) {
        path[x][y] = 1;
        recordPath[pathValue - i - 1][0] = x;
        recordPath[pathValue - i - 1][1] = y;
        //if(room[x - 1][y] < OBSTACLE && room[x][y - 1] < OBSTACLE)
        if(room[x][y-1] < OBSTACLE && y-1 > 0 && room[x][y-1] > 0 && room[x - 1][y] > room[x][y-1]){
            tempX = x; tempY = y-1;
        }else if(room[x-1][y] < OBSTACLE && x-1 > 0 && room[x-1][y] > 0){
            tempX = x-1; tempY = y-1;
        }
        if(room[x+1][y] < OBSTACLE && room[x+1][y] > 0&& x+1 < EXVALUE-1 && room[tempX][tempY] > room[x+1][y]){
            tempX = x+1; tempY = y;
        }
        if(room[x][y+1] < OBSTACLE && room[x][y+1] > 0&& y+1 < EXVALUE-1 && room[tempX][tempY] > room[x][y+1]){
            tempX = x; tempY = y+1;
        } 
        x = tempX;
        y = tempY;
        //(room[x - 1][y] < room[x][y- 1] ) ? (x = x - 1) : (y = y - 1);
        //else
            
    }

}

void self(int i, int x, int y, int pathValue){
    if(i >= pathValue)
        return;

    if(room[x - 1][y] >= OBSTACLE){
        if(room[x][y - 1] >= OBSTACLE){
            return;
        }
        else{
            if(path[x][y] - path[x][y - 1] == -1){
                path[x][y] = 1;
                recordPath[pathValue - i - 1][0] = x;
                recordPath[pathValue - i - 1][1] = y;
                self(i+1, x, y-1, pathValue);
            }else
                return;
        }
    }else if(path[x][y] - path[x - 1][y] == -1){
        path[x][y] = 1;
        recordPath[pathValue - i - 1][0] = x;
        recordPath[pathValue - i - 1][1] = y;
        self(i+1, x-1, y, pathValue);
    }

}
