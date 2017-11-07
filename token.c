#include<stdio.h>

char outtoken[200][50]; //token格納用配列
char sign[10]={'+','-','*','/','(',')','=',',','.',';'};
char res[13][20]={"begin","end","if","then","while","do","return","function","var","const","odd","write","writeln"};
FILE *fi;
int check_word(char);
int check_sign_1(char);
int check_token(char*);

int main()
{
	int i,j,token_num,word;
	char c;
	fi=fopen("program1","r");
	token_num=0;  //トークンの数の初期化
	j=0;
	while(1){
		if(j==0)word=0;
		c=fgetc(fi);  //1文字ずつ入力
		printf("%c\n",c);
		if(c==EOF)break;

		if(check_word(c)==0){//数字なら 
			outtoken[token_num][j]=c;
			j++;
		}
		else if(check_word(c)==1){//文字なら 
			if(j!=0 && word==0){
				token_num++;
				j=0;
			}
			word=1;
			outtoken[token_num][j]=c;
			j++;
		}
		else if(check_word(c)==2){  //改行，スペース，タブなら
			if(j!=0){
				outtoken[token_num][j]='\0';
				j=0;
				token_num++;
			}
			else{
				//do nothing!!
			}
		}else if(check_word(c)==3){  //記号なら(1文字)
			if(j!=0)token_num++;
			outtoken[token_num][0]=c;
			token_num++;
			j=0;
		} else if(check_word(c)==4){  //記号なら(2文字)
			if(j!=0)token_num++;
			outtoken[token_num][0]=c;
			outtoken[token_num][1]=fgetc(fi);
			token_num++;
			j=0;
		}
	}
	//output	
	for(i=0;i<token_num;i++){
		switch(check_token(outtoken[i])){
			case 0:
				printf("%3d  %s <整数>\n",i,outtoken[i]);
				break;
			case 1:
				printf("%3d  %s <記号>\n",i,outtoken[i]);
				break;
			case 2:
				printf("%3d  %s <予約語>\n",i,outtoken[i]);
				break;
			case 3:
				printf("%3d  %s <名前>\n",i,outtoken[i]);
				break;
			default:
				break;
		}
	}
	return 0;
}

int check_word(char c)
{
	if(c>='0' && c<='9') // 数字なら 
		return 0;
	else if(c>='a' && c<='z')        //アルファベット
		return 1;
	else if(c=='\n' || c==' ' || c=='\t')        //改行，スペース，タブ
		return 2;
	else if(check_sign_1(c))        //1文字のみの記号
		return 3;
	else if(c==':')      //コロンは'='と必ず一緒になる
		return 4;
	else if(c=='<' || c=='>'){  //この場合は次の文字を見る必要がある
		c=fgetc(fi);
		fseek(fi,-1,1);   //ファイルポインタを1つ戻す 
		if(c=='=' || c=='>'){
			return 4;
		}else{
			return 3;
		}
	}
}

int check_sign_1(char c){
	int i;
	for (i=0;i<10;i++) if(c==sign[i])return 1;
	return 0;
}

int check_token(char *token){
	int i;
	if(token[0]>='0' && token[0]<='9')return 0;//整数
	else if(check_sign_1(token[0])==1 || token[0]==':' || token[0]=='<'|| token[0]=='>')return 1;//記号
	else{   
		for(i=0;i<13;i++){
			if(strcmp(token,res[i])==0)return 2;//予約語
		}
		return 3;//名前
	}
}
