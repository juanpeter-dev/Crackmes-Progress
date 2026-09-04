#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
int calculate(char *character, int length){
	int table[10] = {4, 79, 129, 171, 254, 123, 224, 204, 70, 53};
        int result = 0;
	int i;
        for (i = 0;i<length;i++) {
            result += character[i] ^ table[i];
        }
        return result;

}
int char_gen(char *character){
	char charset[] ={"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"};
        int i;
        int n= 5;
        n+= rand() % 6;
        int r;
        for(i=0;i<n;i++){
                r= rand()%strlen(charset);
                character[i]= charset[r];
        }
        character[n]='\0';
	return n;
}
void display(char *c, int r){
	printf("String of characters(with no space): %s \n", c);
        printf("correct number(no spaces): %d\n",r);
}
int main(){

	char character[11];
	srand(time(NULL));
	int n= char_gen(character);
	int end_result=calculate(character,n);
	display(character, end_result);
	return 0;
}

