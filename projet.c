#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int f1[20] = {44, 77, 5,7,3,33,11,31,18,35,27,55,10,28,8,20,2,14,9,16};


int main(int argc, char *argv[]){
	my_rand();
	return 0;	
}


int my_rand(){
	int min = 1800000;
	int max = 2700000;
	int m, s, ms;
	int i;
	int temps;
	static int first = 0;
	char tempsConv[10];
	char tempsConcat[10];

	if(first==0){
		srand(time(NULL));
		first = 1;
	}

	for(i = 0; i<3; i++){
		int random = rand()%(max+1-min)+min;
		s = random/1000;
		ms = random%1000;
		m = s/60;
		s= s%60;

		sprintf(tempsConv, "%d %d %d", m,s,ms);
		sprintf(tempsConcat, "%s", tempsConv);

		printf("%s \n",tempsConcat);
	}
}
