#include<stdio.h>
int main()
{
	FILE *fp;
	char filename[20];
	scanf("%s",filename);
	fp=fopen(filename,"r");
	char ch;
	int i=0;
	while((ch=fgetc(fp))!=EOF)
	{
		printf("%c",ch);
		if(ch=='/')
			fseek(fp,-2,SEEK_CUR); 
		i++;
		if(i==6)
			break;
	}
 } 
