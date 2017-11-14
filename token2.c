#include<stdio.h>
#include<stdlib.h>
#include<string.h>

char outtoken[200][10];  //トークン出力用配列
int tnum;

char token[20];
char res[13][20]={"begin","end","if","then","while","do",
				  "return","function","var","const","odd",
				  "write","writeln"};
char sign[10]={'+','-','*','/','(',')','=',',','.',';'};
FILE *fi;
//関数宣言
void nexttoken(int);  //次のトークンを読み込む関数
int ch(char[]);    //引数の文字列との比較　同じ:1　違う:0
int ttype();       //トークンのタイプを返す 1:予約語 2:記号
		   //		      3:名前   4:整数
void exit_func();  //構文解析エラー　強制終了
void backtoken();  //現在のトークンの番号を1つ前に戻す

void block(), varDecl(), constDecl(), funcDecl(),
	 statement(), condition(), expression(), term(),
	 factor();              //構文規則を指定

void get_token();
int check_word(char);
int check_sign_1(char);
int check_token(char*);

int main(int argc, char **argv)
{
	get_token();  //1週目で行ったトークン分割プログラム
	tnum=0;
	block();
	nexttoken(1);
	if(ch(".")) printf("Complete\n");
	else printf("Syntax Error\n");

	return 0;
}

void block()
{
	int c;
	c=0;
	nexttoken(0);
	while(ch("const")||ch("var")||ch("function")){
		c++;
		if(ch("const")) constDecl();
		else if(ch("var")) varDecl();
		else if(ch("function")) funcDecl();
	}
	if(!c) backtoken();
	statement();
}

void varDecl()
{
	nexttoken(1);
	if(ttype()!=3) exit_func("verdecl");
	nexttoken(1);
	while(ch(",")){
		nexttoken(1);
		if(ttype()!=3) nexttoken(1);
		else exit_func("verdecl");
	}
	if(ch(";")){}
	else exit_func("verdecl");
}

void funcDecl()
{
	nexttoken(3);
	if(ttype()!=3) exit_func("funcdecl"); 
	nexttoken(3);
	if(!ch("("))exit_func("funcdecl");
	nexttoken(3);
	if(ttype()!=3) 
			exit_func("funcdecl"); 
	nexttoken(3);
	while(ch(",")){
		nexttoken(3);
		if(ttype()==3) nexttoken(3);
		else exit_func("funcdecl");
	}
	if(ch(")")) block();
	else exit_func("funcdec3");
	nexttoken(3);
	if(ch(";")) nexttoken(3);
	else exit_func("funcdec4");
}

void constDecl()
{
	do{
		nexttoken(2);
		if(ttype()!=3) exit_func("constdecl");
		nexttoken(2);
		if(!ch("=")) exit_func("constdecl");
		nexttoken(2);
		if(ttype()!=4) exit_func("constdecl");
		nexttoken(2);
	}while(ch(","));
	if(ch(";"))nexttoken(2);
	else exit_func("constdecl");
}

void statement()
{
	int a,b,n;
	int t_now, t_next;

	nexttoken(4);
	if(ttype()==3){
		nexttoken(4);
		if(ch(":=")!=1) exit_func("Syntax Error");
		expression();
	}
	else if(ch("begin")){
		statement();
		nexttoken(4);
		while(ch(";")==1){
			statement();
			nexttoken(4);
		}
		if(ch("end")!=1) exit_func("Syntax Error");
	}
	else if(ch("if")){
		condition();
		nexttoken(4);
		if(ch("then")!=1) exit_func("Syntax Error");
		statement();
	}
	else if(ch("while")){
		condition();
		nexttoken(4);
		if(ch("do")!=1) exit_func("Syntax Error");
	}
	else if(ch("return")){
		expression();
	}else if(ch("write")){
		expression();
	}else if(ch("writeln")){
	}else{   //何もせずにスルー 
		backtoken();
	}
}

void condition(){
	nexttoken(5);
	if(ch("odd")==1)expression();
	else{
		expression();
		nexttoken(5);
		if(ch("=")==1)expression();
		else if(ch("<>")==1)expression();
		else if(ch("<")==1)expression();
		else if(ch(">")==1)expression();
		else if(ch("<=")==1)expression();
		else if(ch(">=")==1)expression();
		else exit_func("Syntax Error");
	}
}

void expression(){
	nexttoken(6);
	if(ch("+")!=1 || ch("-")!=1)backtoken();
	term();
	nexttoken(6);
	if(ch("+")==1 || ch("-")==1)term();
	else backtoken();
}

void term(){
	factor();
	nexttoken(7);
	if(ch("*")==1 || ch("/")==1)factor();
	else backtoken();
}

void factor(){
	nexttoken(8);
	if(ttype()==3||ttype()==4)backtoken();
	else if(ttype()==1){
		nexttoken(8);
		if(ch("(")==1){
			
		}
		else exit_func("Syntax Error");
	}
	else if(ch("(")==1){
		expression();
		nexttoken(8);
		if(ch(")")==1)backtoken();
	}
	else exit_func("Syntax Error");
}

void nexttoken(int s){
	char state[30];
	switch(s){
		case 0: sprintf(state,"block");
				break;
		case 1: sprintf(state, "constDecl");
				break;
		case 2: sprintf(state, "verDecl");
				break;
		case 3: sprintf(state, "funcDecl");
				break;
		case 4: sprintf(state, "statement");
				break;
		case 5: sprintf(state, "condition");
				break;
		case 6: sprintf(state, "expression");
				break;
		case 7: sprintf(state, "term");
				break;
		case 8: sprintf(state, "factor");
				break;
	}
	strcpy(token,outtoken[tnum]);
	tnum++;
	printf("%-13s : [%s]\n",state,token);
}

int ch(char str[20]) //文字列の比較
{
	if(strcmp(token,str)==0) return 1;
	else return 0;
}

int ttype()  //トークンのタイプを返す
{
	int i,c;
	for(i=0;i<13;i++){
		c=strcmp(token,res[i]);
		if(c==0){
			return 1;
			break;
		}
	}

	for(i=0;i<16;i++){
		c=strcmp(token,sign);
		if(c==0){
			return 2;
			break;
		}
	}

	if(token[0] >='0' && token[0]<='9')
		return 4;
	else
		return 3;
}

void backtoken() //トークン番号を1つ前に戻す
{
	tnum--;
}

void exit_func(char str[20])  //文法規則に合わないため終了
{
	printf("Syntax Error Near the Word \"%s\"\n",token);
	printf("%s\n",str);
	exit(1);  //強制終了
}

void get_token()
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
