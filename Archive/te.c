#include <stdio.h>

#define MIN room[a][b]<room[c][d]?a:b

int main(void){

int room[2][2] = {0,1,3,4,};
int room2[2][2] = {0,};
int i = 0;

i = MIN(room[1][1],room[0][0]);

printf("%d " , i );

return;

}
