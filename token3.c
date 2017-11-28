#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define Token_Max 20
#define Stack_Max 50
char token[Token_Max][5];
char stack[Stack_Max][50];
char outstring[Token_Max][5];
int stptr,tnum,onum;
void push_stack(), pop_stack(),write_stack();
int comp_token();
void three_num_code();

int main()
{
	//トークンは標準入力するとする 
	//変数はA〜Z に簡略化するとする
	//入力エラーは考慮しない
	int i,j;
	tnum=onum=stptr=0;
	
	for(;;){
		printf("token[%d] : ",tnum);
		scanf("%s",token[tnum]);
		if(token[tnum][0]=='q') break;
		tnum++;
		if(tnum > Token_Max){
			printf("Overflow\n");
			exit(1);
		}
	}

	for(i=0;i<tnum;i++){
		printf("%s",token[i]);
	}
	printf("\n");

	for(i=0;i<tnum;i++){
		//scanf("%d",&j);
		if(token[i][0]>='A' && token[i][0]<='Z'){  //トークンが変数の場合 
			strcpy(outstring[onum],token[i]);
			onum++;

		}else if(strcmp(token[i],"(")==0){  //トークンが"("の場合
			push_stack(token[i]);

		}else if(strcmp(token[i],")")==0){
			while(strcmp(stack[stptr-1],"(")!=0){
				strcpy(outstring[onum],stack[stptr-1]);
				pop_stack();
				onum++;
			}
			pop_stack();

		}else{
			if(stptr==0){  //スタックが空の場合無条件にプッシュ
				push_stack(token[i]);
			}else if(comp_token(i)==1){   //tokenの優先>topの優先
				push_stack(token[i]);

			}else{                       //tokenの優先<=topの優先
				while(comp_token(i)!=1){
					strcpy(outstring[onum],stack[stptr-1]);
					pop_stack();
					onum++;
					i++;
				}
				push_stack(token[i]);
			}
		}
	}
	for(i=stptr-1;i>=0;i--){
		strcpy(outstring[onum],stack[i]);
		onum++;
	}


	printf("outstring : ");
	for(i=0;i<onum;i++) printf("%s ",outstring[i]);
	printf("\n");

	three_num_code();	

	return 0;
}


void push_stack(char str[5])
{
	if(stptr==Token_Max){
		printf("オーバーフロー\n");
	}else{
		strcpy(stack[stptr++],str);
	}
}

void pop_stack()
{
	if(stptr<=0){
		printf("No Such Data\n");
	}else{
		--stptr;
	}
}

void
write_stack()
{
	int i;
	for(i=0;i<stptr;i++){
		printf("%s\n",stack[i]);
	}
}

//演算子の優先順位
int comp_token(int i)
{
	int t,s;
	switch(token[i][0]){
		case '*' :
		case '/' :  t=3;
					break;
		case '+' :
		case '-' :  t=2;
					break;
		case '=' :
		case '(' :  t=1;
					break;
		case ')' :  t=0;
					break;
		default: break;
	}
	switch(stack[stptr-1][0]){
		case '*' :
		case '/' :  s=3;
					break;
		case '+' :
		case '-' :  s=2;
					break;
		case '=' :
		case '(' :  s=1;
					break;
		case ')' :  s=0;
					break;
		default: break;
	}

	if(t>=s) return 1;
	else return 0;
}

void three_num_code(){
	int i,j;
	char c,ss[5];
	char threecode[4][5],twocode[3][5];
		//threecodeは3番地コード　+ A B a  の形  a=A+B
		//twocodeは２番地コード　 = A a		 A=a
	stptr=0;
	c='a';
	for(i=0;i<onum;i++){
		if((outstring[i][0]>='A' && outstring[i][0]<='Z')){ //変数ならば
			push_stack(outstring[i]);
		}else if(strcmp(outstring[i],"=")!=0){ //演算子（=ではない）なら
			strcpy(threecode[0],outstring[i]);  //op (演算子）
			strcpy(threecode[1],stack[stptr-2]);
			strcpy(threecode[2],stack[stptr-1]);
			pop_stack();
			pop_stack();
			ss[0]=c; ss[1]='\0';  //新しい変数を作る
			push_stack(ss);
			strcpy(threecode[3],ss);
			c++;
			printf("%s (%s %s %s)\n",
				threecode[0],threecode[1],threecode[2],threecode[3]);
		}else if(strcmp(outstring[i],"=")==0){  //＝なら２番地コード
			strcpy(twocode[0],outstring[i]);
			strcpy(twocode[1],stack[stptr-2]);
			strcpy(twocode[2],stack[stptr-1]);
			pop_stack();
			pop_stack();
			printf("%s (%s %s)\n",twocode[0],twocode[1],twocode[2]);
		}
	}
}
