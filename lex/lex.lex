%{
int fuckCount=0;
%}
fuck [Ff]uck
%%
{fuck} { fuckCount++;}
%%
int main()
{
	yylex();	//begin
	printf("number of fuck:%d!", fuckCount);
	return 0;
}
int yywrap()
{
	return 1;
}
