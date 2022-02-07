#include "widget.h"
#include "ui_widget.h"
#include<setjmp.h>
#include <QMessageBox>
#include <QDebug>
#include <QTextCodec>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define  reserveN  22
#define  operatorN 22
#include <fstream>
#include <iostream>
#include <string>
#include <stack>
#include <queue>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
using namespace std;


//word head



//保留字表
static char reserveWord[][20] = {
    "auto", "break", "case", "const", "continue",
    "default", "do", "else", "extern", "for", "goto", "if",
    "register", "return", "sizeof", "static",
    "switch", "typedef", "unsigned","show", "while","main"
};

//类型关键字表
static char type[][20] = {
    "int", "long", "char", "double", "short",
    "struct", "void", "enum", "union"
};

//运算符表
static char operatorer[][5] = {
    "+", "-", "*", "/",
    ";", "^", ",", "\"", "\'", "#", "&",
    "|", "%", "~", "<<", ">>",
    "\\", ".", "\?", ":", "!", "="
};

//界符表
static char limiter[][5] = {
    "(", ")", "{", "}", "[", "]"
};

//逻辑符号表
static char logicer[][5] = {
    "<", "<=", ">", ">=", "==", "!=", "&&", "||"
};

char IDentifier[][20] = { "" };  //存放标识符
static int Project_layer[10000];//存放代码所在行数
char Project[10000];     //存放程序代码
char token[20] = "";            //每次扫描的时候存储已经扫描的结果
int syn = -1;                  //种别码，当syn=0时，为源程序结束符号
int p = 0;
FILE* fp1;


//判断是否为保留字
int searchReserve(char reserveWord[][20], char s[])
{
    for (int i = 0; i < reserveN; i++)
    {
        if (strcmp(s, reserveWord[i]) == 0)
            return i + 1;
    }
    return -1;
}

//判断是否为类型符
int searchType(char type[][20], char s[])
{
    for (int i = 0; i < 9; i++)
    {
        if (strcmp(s, type[i]) == 0)
            return i + 23;
    }
    return -1;
}

//判断是否为字母
bool Isletter(char letter)
{
    if ((letter <= 'z' && letter >= 'a') || (letter <= 'Z' && letter >= 'A') || letter == '_')
        return true;
    else return false;
}

//判断是否为数字
bool Isnumber(char digit)
{
    int num = digit - '0';
    if (num <= 9 && num >= 0)
        return true;
    else return false;
}

//过滤掉程序中的无关字符（即换行符'\n'与注释代码）
void filter(char Project[], int p)
{
    int code_layer_recent = 1;
    char temp[10000];
    int count = 0;
    for (int i = 0; i <= p; i++)
    {
        if (Project[i] == '/' && Project[i + 1] == '/')  //若为单行注释“//”,则去除注释后面的东西，直至遇到回车换行
        {
            while (Project[i] != '\n')
            {
                i++;
            }
            code_layer_recent++;//显然注释后面的换行也增加了行数
        }
        if (Project[i] == '/' && Project[i + 1] == '*')  //若为多行注释“/* 。。。*/”则去除该内容
        {
            i += 2;
            while (Project[i] != '*' || Project[i + 1] != '/')
            {
                if (Project[i] == '\n')code_layer_recent++;//多行注释里也有换行
                i++;
                if (Project[i] == EOF)
                {
                    printf("注释出错，没有找到 */，\n");
                    fprintf(fp1, "Error：注释出错，没有找到 */，\n");
                    throw("注释出错，没有找到 */");
                }
            }
            i += 2;//跨过“*/”
        }
        if (Project[i] == '\n')
        {
            code_layer_recent++;
            continue;
        }
        else
        {
            temp[count] = Project[i];
            Project_layer[count] = code_layer_recent;
            count++;
        }

    }
    temp[count] = '\0';
    Project_layer[count] = code_layer_recent;
    //for (int i = 0; i < 100; i++)cout << temp[i] << endl << Project_layer[i] << endl;
    strcpy(Project, temp);
}

//扫描器

int recent_layer = 1;

void Scanner(int& syn, char Project[], char token[], int& p)
{
    recent_layer = Project_layer[p];//存放project[p]对应的行数
    int i, count = 0;
    char ch; 		   //作为判断使用
    ch = Project[p];
    syn = -1;
    while (ch == ' ')  //过滤掉开头的空格
    {
        p++;
        ch = Project[p];
    }
    for (i = 0; i < 20; i++)
    {
        token[i] = '\0';
    }
    if (Isletter(Project[p]) || Project[p] == '_')  //开头为字母或者下划线
    {
        token[count++] = Project[p];
        p++;
        while (Isletter(Project[p]) || Isnumber(Project[p]) || Project[p] == '_')
        {
            token[count++] = Project[p];
            p++;
        }
        token[count] = '\0';
        syn = searchReserve(reserveWord, token);
        if (syn == -1)  //若不是保留字则是类型关键字或者自己定义的变量
        {
            syn = searchType(type, token);
            if (syn == -1)
                syn = 69;  //自己定义的变量
        }
        return;
    }
    else if (Isnumber(Project[p]))  //首字符为数字
    {
        if (Project[p] == 0)
        {
            if (Project[p + 1] != ' ')
            {
                cout << "Error：变量名定义错误" << "\n";
                fprintf(fp1, "Error：变量名定义错误\n");
                exit(0);
            }
        }
        while (Isnumber(Project[p]))
        {
            token[count++] = Project[p];
            p++;
        }
        if (Isletter(Project[p]) || Project[p] == '_')
        {
            while (Project[p] != ';')
            {
                p++;
            }
            syn = -2;
            return;
        }
        token[count] = '\0';
        syn = 68;
    }

    //若为运算符
    else if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == ';' || ch == '^'
        || ch == ',' || ch == '\"' || ch == '\'' || ch == '~' || ch == '#' || ch == '%'
        || ch == '\\' || ch == '.' || ch == '\?' || ch == ':')
    {
        token[0] = Project[p];
        token[1] = '\0';
        for (i = 0; i < operatorN; i++)
        {
            if (strcmp(token, operatorer[i]) == 0)
            {
                syn = 32 + i;
                break;
            }
        }
        p++;
        return;
    }
    else  if (Project[p] == '<')	//<,<=,<<
    {
        p++;//超前搜索
        if (Project[p] == '=')
        {
            syn = 61;
        }
        else if (Project[p] == '<')
        {
            syn = 46;
        }
        else
        {
            p--;
            syn = 60;
        }
        p++;
        return;
    }
    else  if (Project[p] == '>')  //>,>=,>>
    {
        p++;
        if (Project[p] == '=')
        {
            syn = 63;
        }
        else if (Project[p] == '>')
        {
            syn = 47;
        }
        else
        {
            p--;
            syn = 62;
        }
        p++;
        return;
    }
    else  if (Project[p] == '=')  //=.==
    {
        p++;
        if (Project[p] == '=')
        {
            syn = 64;
        }
        else
        {
            p--;
            syn = 53;
        }
        p++;
        return;
    }
    else  if (Project[p] == '!')  //!,!=
    {
        p++;
        if (Project[p] == '=')
        {
            syn = 65;
        }
        else
        {
            syn = 52;
            p--;
        }
        p++;
        return;
    }
    else  if (Project[p] == '&')  //&,&&
    {
        p++;
        if (Project[p] == '&')
        {
            syn = 66;
        }
        else
        {
            p--;
            syn = 42;
        }
        p++;
        return;
    }
    else  if (Project[p] == '|')  //|,||
    {
        p++;
        if (Project[p] == '|')
        {
            syn = 67;
        }
        else
        {
            p--;
            syn = 43;
        }
        p++;
        return;
    }
    else if (ch == '(' || ch == ')' || ch == '{' || ch == '}' || ch == '[' || ch == ']')
    {
        switch (ch)
        {
        case '(':syn = 54; p++; return;
        case ')':syn = 55; p++; return;
        case '{':syn = 56; p++; return;
        case '}':syn = 57; p++; return;
        case '[':syn = 58; p++; return;
        case ']':syn = 59; p++; return;
        }
    }
    else  if (Project[p] == EOF) //文件结束符
    {
        syn = 0;
    }
    else  //不能被以上词法分析识别
    {
        cout << "ERROR：出现了无法识别的字符" << endl;
        fprintf(fp1, "ERROR：出现了无法识别的字符\n");
        throw("出现了无法识别的字符");


    }
}
//gram head

struct elem          //存放词法分析的结果
{
    char token[50];
    char value[50];
    int layer = 0;
}sym;

struct table {             //符号表
    char var[50][50];
    int  size = 0;
}table;

struct hanshu{
    char var[50][50];
    int  canshu[100];
    int  size = 0;
    char name[100];
    int  shiyongcishu=0;
    int shoudizhi = -1;
    int weidizhi[100] = { 0};
}hanshu[100];
struct em {          //存放四元式
    string sop;
    string a;
    string b;
    string label;
}em[10000];//邢文宇修改了下标 原来是100

struct label_t {      //回填相关的信息
    int label_n[100] = { -1 };//标记该层还未替换的行号
    int size = 0;
    int num = 0;  //回填的序号
}label_t[100];
struct elem elems[200000];
int line_g = 1;//一共几行
int cline_g = 1;//目前第几行
int cnt = 1; //四元式行数
int temp = 1; //临时变量标号
int temp_l = 1;//label的标号
int cen = 0;//层数
int shun = 1;//顺序
int s_l = 1;
char mmm[500];
char* l = mmm;
char mml[500];
char* t_l = mml;
FILE* fp_g;//邢文宇修改了fp
int  xunhuan = 0; //是否在循环体内
int hanshu_cnt = 0;  //函数个数计数器
char mmml[500];
char* ll = mmml;

char show_list[50][50] ;
int show_list_count = 0;

stack<string> var_t;//值栈
stack<string> op_t;//运算符栈
stack<string> lo_t;//逻辑运算符栈
stack<string> temp_kv;//值栈保存现场
stack<string> temp_ko;//运算符保存现场
stack<string> temp_fh; //符号栈保存现场............................
stack<int> temp_fh_gs; //符号栈个数保存现场............................

//程序整体
void SS();
int HD();
int CS();
int P();
int M();
//变量定义部分
int D();
int D1();
int D2();
//主控语句部分
int S();
int X();
int K();
//运算表达式部分
char* E();
char* E1();
char* T();
char* T1();
char* F();
//布尔表达式部分
int B();
int B1();
int C();
int C1();
int Y();
int Y1();
char* Y2();
char* Y3();

char* HS( char*);
char* J(char *);
void xx(char*a);
char q[100];
char* zhuhanshu_fanhuizhi(char* a);
char* oldfanhuizhi(char*);
char* newfanhuidizhi(int i);



int readf()  //将词法分析器的数据存入结构体数组中
{
    char filename[] = "D:\\result1.txt";
    FILE* fp = fopen(filename, "r");
    char buf[100];
    int i = 0, l;
    fgets(buf, 100, fp);
    while (!feof(fp))
    {
        int k = 1, m = 0;
        while (buf[k] != ',')
        {
            elems[line_g].token[m] = buf[k];
            k++; m++;
        }
        elems[line_g].token[m] = '\0';
        k++; m = 0;
        l = strlen(buf);
        while (k <= l - 3)
        {
            elems[line_g].value[m] = buf[k];
            k++; m++;
        }

        elems[line_g].value[m] = '\0';
        line_g++;
        fgets(buf, 100, fp);
    }
    fclose(fp);
    return 0;
}

elem getsym() //取词函数 ，同时具有下移一行的功能
{
    return elems[cline_g++];
}

void r_v(const char* s)  //分析单词的value值是否匹配
{
    if (strcmp(sym.value, s) == 0)
    {
        sym = getsym();
    }
    else { cout << "第" << sym.layer << "行"; cout << "ERROR:缺少 " << s << "." << endl; fprintf(fp_g, "ERROR:缺少%s .\n", s);
        char m[50];
            sprintf(m, "%d", sym.layer);
            strcat(m, "行错误：缺少");
            strcat(m, s);
            cout<<m<<endl;
            throw m;


     exit(0);
    }
}

void r_t(const char* s)  //分析单词的token值是否匹配
{
    if (strcmp(sym.token, s) == 0)
    {
        sym = getsym();
    }
    else
    {
        cout << "第" << sym.layer << "行"; cout << "ERROR:缺少 " << s << "." << endl;
        fprintf(fp_g, "ERROR:缺少%s .\n", s);

        char m[50];
            sprintf(m, "%d", sym.layer);
            strcat(m, "行错误：缺少");
            strcat(m, s);
            cout<<m<<endl;
            throw m;

    }
}
//生成临时变量tx
char* newtemp()
{
    char s[50], m[50] = "t";
    sprintf(s, "%d", temp);
    strcat(m, s);
    temp++;
    strcpy(l, m);
    return l;
}
char* newcanshu()
{
    char s[50];
    char m[50];
    strcpy(m,hanshu[hanshu_cnt].name);
    sprintf(s, "%d", hanshu[hanshu_cnt].size);
    strcat(m, s);

    strcpy(ll, m);
    return ll;
}

char* newfanhuizhi()
{
    char s[50];
    char m[50];
    strcpy(m, hanshu[hanshu_cnt].name);
    sprintf(s, "%d",0);
    strcat(m, s);

    strcpy(ll, m);
    return ll;
}
char* oldfanhuizhi(char*a)       //返回直给主函数
{
    char s[50];
    char m[50];
    strcpy(m, a);
    sprintf(s, "%d", 0);
    strcat(m, s);

    strcpy(ll, m);
    return ll;
}

int chuancan_xuhao = 0;

char* chuancan(char*a)
{
    chuancan_xuhao++;
    char s[50];
    char m[50];
    strcpy(m, a);
    sprintf(s, "%d", chuancan_xuhao);
    strcat(m, s);
    strcpy(ll, m);
    return ll;
}

char* newfanhuidizhi(int i)
{
    char s[50] = "ad";
    char m[50];
    strcpy(m, hanshu[i].name);
    strcat(m, s);
    strcpy(ll, m);
    return ll;
}

char* zhuhanshu_fanhuizhi(char *a)
{
    char s[50];
    char m[50];
    char ss[50];
    strcpy(m, a);
    sprintf(s, "%d", 0);
    strcat(m, s);
    for (int i = 1; i <= hanshu_cnt; i++)
        if (strcmp(a, hanshu[i].name) == 0)
        sprintf(ss, "%d", ++hanshu[i].shiyongcishu);
    strcat(m, ss);
    strcpy(ll, m);
    return ll;
}
static char creat_s[50];
char* newl()//行数+3 真转
{
    creat_s[0] = '/0';
    sprintf(creat_s, "%d", cnt + 3);
    return creat_s;
}
//回填函数
void backtrack(int i)//回填当前行数
{
    char s[50];
    sprintf(s, "%d", cnt);
    em[i].label = s;
}
//将int型转换为char*类型

char* creat(int i)
{

    creat_s[0] = '/0';
    sprintf(creat_s, "%d", i);
    return creat_s;
}
//将声明变量加进符号表
int push_t(char* s)
{
    int i = 1;
    while (i <= table.size)
    {
        if (strcmp(table.var[i], s) == 0)
        {
            cout << "第" << sym.layer << "行";
            cout << "ERROR:重定义" << s << "." << endl;
            fprintf(fp_g, "ERROR:重定义%s .\n", s);

            char m[50];
            sprintf(m, "%d", sym.layer);
            strcat(m, "行错误：重定义");
            strcat(m, s);
            cout<<m<<endl;
            throw m;


        }
        i++;
    }
    strcpy(table.var[++table.size], s);
    return 1;
}
//查询符号表
int lookup(char* s)
{
    for (int i = 1; i <= table.size; i++)
    {
        if (strcmp(table.var[i], s) == 0)
            return i;
    }
    for (int i = 1; i <= hanshu_cnt; i++)
        if (strcmp(s, hanshu[i].name) == 0)
            return i;
cout << "第" << sym.layer << "行";
    cout << "ERROR:未定义" << s << "." << endl;
    fprintf(fp_g, "ERROR:未定义%s .\n", s);
    char m[50];
        sprintf(m, "%d", sym.layer);
        strcat(m, "行错误：未定义");
        strcat(m, s);
        cout<<m<<endl;
        throw m;
}
//生成四元式函数
int semit(string s, string y, string z, string m)
{
    em[cnt].sop = s;
    em[cnt].a = y;
    em[cnt].b = z;
    em[cnt++].label = m;
    return 0;
}

void SS()
{
    sym = getsym();
    semit("jp", "_", "_", "N");
    HD();
    if ((cline_g - 1) == line_g)
    {
        //fprintf(fp_g, "#right\n");
        int i = 1;
        cout << endl << "生成的四元式为：" << endl;
        while (i < cnt)
        {
            cout << i << "(" << em[i].sop << "," << em[i].a << "," << em[i].b << "," << em[i].label << ")" << endl;
            fprintf(fp_g, "%d  (%s,%s,%s,%s)\n", i, em[i].sop.c_str(), em[i].a.c_str(), em[i].b.c_str(), em[i].label.c_str());//邢文宇修改了i++
            i++;
        }
        cout << endl << "语法正确" << endl;
        fprintf(fp_g, "#right\n");
    }
    else { cout << "第" << sym.layer << "行"; cout << "ERROR:未找到代码"; fprintf(fp_g, "ERROR:未找到代码\n");
    throw("无法完成二元组分析！");}
}
//HD->P | type_w ID( CS|ε){ D S return Y3 ;}HD
int HD()
{

    if (strcmp(sym.value, "void") == 0)
        P();
    else if (strcmp(sym.token, "type_w") == 0)
    {

        r_t("type_w");
        if (strcmp(sym.token, "ID") == 0)
        {
            if (!table.size)
            {
                temp_fh_gs.push(table.size);
                while (table.size)        //保存现场
                {
                    temp_fh.push(table.var[table.size--]);

                }
            }
            push_t(sym.value);
            strcpy(hanshu[++hanshu_cnt].name, sym.value);
            hanshu[hanshu_cnt].shoudizhi = cnt;


            r_t("ID");
        }

        r_v("(");
        if (strcmp(sym.value, ")") != 0)
            CS();
        r_v(")");
        r_v("{");
        D();
        S();
        r_v("return");
        if (strcmp(sym.token, "ID") == 0)    //Y3
        {
            lookup(sym.value);
            strcpy(l, sym.value);
            semit("=", sym.value, "_", newfanhuizhi());

            r_t("ID");

        }
        else if (strcmp(sym.token, "NUM") == 0)
        {
            strcpy(l, sym.value);
            semit("=", sym.value, "_", newfanhuizhi());
            r_t("NUM");

        }

        const char* pp;
        r_v(";");
        r_v("}");
        table.size = 0;
        if (!temp_fh_gs.empty())
        {
            int jj = temp_fh_gs.top();
            temp_fh_gs.pop();
             while (table.size != jj)      //huifu现场
                {
                 pp =  temp_fh.top().c_str();
                 strcpy(table.var[++table.size], pp);
                 temp_fh.pop();


                }

        }
        //返回所调用函数
        semit("jp", "_", "_",newfanhuidizhi(hanshu_cnt) );
        HD();
    }
    return 0;
}

//CS->type_w ID (, CS| ε)
int CS()
{
    r_t("type_w");

    if (strcmp(sym.token, "ID") == 0)
    {

        push_t(sym.value);
        hanshu[hanshu_cnt].size++;
        semit("=", newcanshu(), "_", sym.value);

        r_t("ID");



    }
    if (strcmp(sym.value, ",") == 0)
    {
        r_v(",");
        CS();
    }

  //	else if(  )  //ID follow
    return 0;

}
//P->void main()M
int P()
{

    r_v("void");
    r_v("main");
    r_v("(");
    r_v(")");

        em[1].label = creat(cnt);   //跳到主函数执行回填


    M();

    return 0;
}

//M->{DS}
int M()
{
    r_v("{");
    if (!table.size)
    {
        temp_fh_gs.push(table.size);
        while (table.size)        //保存现场
        {
            temp_fh.push(table.var[table.size--]);

        }
    }
    D();
    S();
    r_v("}");
    const char* pp;
    table.size = 0;
    if (!temp_fh_gs.empty())
    {
        int jj = temp_fh_gs.top();
        temp_fh_gs.pop();
        while (table.size != jj)      //huifu现场
        {
            pp = temp_fh.top().c_str();
            strcpy(table.var[++table.size], pp);
            temp_fh.pop();


        }

    }
    return 0;
}

//D->type_w IDD1;D2
//D1->,IDD1|ε
//D2->D|ε
int D()
{
    r_t("type_w");
    if (strcmp(sym.token, "ID") == 0)
    {
        push_t(sym.value);
        r_t("ID");
    }
    D1();
    r_v(";");
    D2();
    return 0;
}
//D1->,IDD1|ε
int D1()
{
    if (strcmp(sym.value, ",") == 0)
    {
        r_v(",");
        if (strcmp(sym.token, "ID") == 0)
        {
            push_t(sym.value);
            r_t("ID");
        }
        D1();
    }
    else if (strcmp(sym.value, ";") != 0)
    {
        cout << "第" << sym.layer << "行";
        cout << "ERROR:缺少';'." << endl;
        fprintf(fp_g, "ERROR:缺少';' .\n");
        char m[50];
        sprintf(m, "%d", sym.layer);
        strcat(m, "行错误：缺少';'");

        cout<<m<<endl;
        throw m;
    }
}
//D2->D|ε
int D2()
{
    if (strcmp(sym.token, "type_w") == 0)
    {
        D();
    }
    else if (strcmp(sym.token, "ID") != 0 && strcmp(sym.value, "while") != 0 && strcmp(sym.value, "if") != 0 && strcmp(sym.value, "}") != 0 && strcmp(sym.value, "for") != 0)
    {
        cout << "第" << sym.layer << "行";
        cout << "ERROR:类型定义错误." << endl;
        fprintf(fp_g, "ERROR:类型定义错误.\n");
        char m[50];
        sprintf(m, "%d", sym.layer);
        strcat(m, "行错误：类型定义错误");

        cout<<m<<endl;
        throw m;
    }
}

//S->ID=E;S| while(B){S}S | if(B){SK}XS | ε |for(ID=E;ID=E;B){S}S   |  show<<ID ;S
int S()
{
    int label_num;
    char p1[50], *p2;
    if (strcmp(sym.token, "ID") == 0)
    {
        lookup(sym.value);
        strcpy(p1, sym.value);
        r_t("ID");
        r_v("=");
        p2 = E();
        if (!var_t.empty())
        {
            semit("=", var_t.top(), "_", p1);
            var_t.pop();
        }
        else
        {
            semit("=", p2, "_", p1);
            var_t.pop();
        }
        r_v(";");
        S();
    }
    else if (strcmp(sym.value, "while") == 0)
    {
        int i;
        xunhuan += 1;
        cen++;			//层数+1
        r_v("while");
        i = cnt;
        r_v("(");
        B();
        r_v(")");
        r_v("{");
        S();
        r_v("}");
        if (label_t[cen].num < label_t[cen].size && sym.token)
        {
            em[label_t[cen].label_n[++label_t[cen].num]].label = creat(cnt + 1); 	//回填while的N
        }
        cen++;
        if (label_t[cen].num < label_t[cen].size)
        {
            em[label_t[cen].label_n[++label_t[cen].num]].label = creat(cnt + 1);
            //回填break的N
        }
        cen--;
        semit("jp", "_", "_", creat(i));
        cen--;
        xunhuan -= 1;
        S();
    }

    else if (strcmp(sym.value, "for") == 0) //刘旭亲自改
    {
        int i;
        xunhuan += 1;
        cen++;			//层数+1
        r_v("for");
        r_v("(");

        int label_num;
        char p1[50], *p2;
        if (strcmp(sym.token, "ID") == 0)
        {
            lookup(sym.value);
            strcpy(p1, sym.value);
            r_t("ID");
            r_v("=");
            p2 = E();
            if (!var_t.empty())
            {
                semit("=", var_t.top(), "_", p1);
                var_t.pop();
            }
            else
            {
                semit("=", p2, "_", p1);
                var_t.pop();
            }
            r_v(";");
        }
        label_t[cen].label_n[++label_t[cen].size] = cnt;
        semit("jp", "_", "_", "N");

        i = cnt;
        int label_num3;
        char p13[50], *p23;
        if (strcmp(sym.token, "ID") == 0)
        {
            lookup(sym.value);
            strcpy(p13, sym.value);
            r_t("ID");
            r_v("=");
            p23 = E();
            if (!var_t.empty())
            {
                semit("=", var_t.top(), "_", p13);
                var_t.pop();
            }
            else
            {
                semit("=", p23, "_", p13);
                var_t.pop();
            }
            r_v(";");
            if (label_t[cen].num < label_t[cen].size)		//填||之前的N
            {
                backtrack(label_t[cen].label_n[++label_t[cen].num]);
            }
            /*if (strcmp(sym.value, ")") == 0)
            {
            }
            else*/
                B();
            r_v(")");
        }
        r_v("{");
        S();
        r_v("}");

        if (label_t[cen].num < label_t[cen].size && sym.token)
        {
            em[label_t[cen].label_n[++label_t[cen].num]].label = creat(cnt + 1); 	//回填for的N
        }
        cen++;
        if (label_t[cen].num < label_t[cen].size)
        {
            em[label_t[cen].label_n[++label_t[cen].num]].label = creat(cnt + 1);
            //回填break的N
        }
        cen--;
        semit("jp", "_", "_", creat(i));
        cen--;
        xunhuan -= 1;
        S();

    }



    else if (strcmp(sym.value, "if") == 0)
    {
        int i,j;
        cen++;			//层数+1
        r_v("if");
        r_v("(");
        B();
        r_v(")");
        r_v("{");
        S();
        j=K();
        S();
        r_v("}");
        i = cnt + 1;
        if (label_t[cen].num < label_t[cen].size)
        {
            if (strcmp(sym.value, "else") == 0)
            {
                em[label_t[cen].label_n[++label_t[cen].num]].label = creat(i);
            }
            else if(j==0)
                backtrack(label_t[cen].label_n[++label_t[cen].num]);

        }
        //回填N项
        X();
        S();
    }

      //S-> show<<ID;S
    else if (strcmp(sym.value, "show") == 0)
    {
        r_v("show");
        r_v("<<");
        strcpy(show_list[show_list_count++], sym.value);
        //cout <<"show谁："<< show_list[show_list_count];
        r_t("ID");
        r_v(";");
        S();
    }

    else if (strcmp(sym.value, "}") != 0&&strcmp(sym.value, "break") != 0 && strcmp(sym.value, "return") != 0)
    {
    cout << "第" << sym.layer << "行";
        cout << "ERROR:缺少'}' in S" << endl;
        fprintf(fp_g, "ERROR:缺少'}' in S.\n");
        char m[50];
        sprintf(m, "%d", sym.layer);
        strcat(m, "行错误：请检查S语句的follow集");

        cout<<m<<endl;
        throw m;


    }
}
//K->break ;|ε
int K()
{

    if (strcmp(sym.value, "break") == 0)
    {

        if (xunhuan == 0) {
            cout << "ERROR:break只能在循环中使用." << endl;
            fprintf(fp_g, "ERROR:break只能在循环中使用.\n");
            throw("break只能在循环中的if语句使用");
        }
        r_v("break");
        r_v(";");
        label_t[cen].label_n[++label_t[cen].size] = cnt;
        semit("jp", "_", "_", "N");

        /*cen--;
        cen--;*/
        return 1;
    }
    else if ( strcmp(sym.value, "}") != 0 )
    {
        cout << "ERROR:if语句break处有语法错误." << endl;
        fprintf(fp_g, "ERROR:if语句break处有语法错误.\n");
        char m[50];
        sprintf(m, "%d", sym.layer);
        strcat(m, "行错误：if语句break处有语法错误");

        cout<<m<<endl;
        throw m;
    }

    return 0;

}
//X->ε|else {S}
int X()
{
    if (strcmp(sym.token, "ID") == 0 || strcmp(sym.value, "while") == 0 || strcmp(sym.value, "if") == 0 || strcmp(sym.value, "}") == 0)
    {
        if (label_t[cen].num <= label_t[cen].size && sym.token)
            backtrack(label_t[cen].label_n[++label_t[cen].num]);
        cen--;
    }
    else if (strcmp(sym.value, "else") == 0)
    {
        r_v("else");
        label_t[cen].label_n[++label_t[cen].size] = cnt;
        semit("jp", "_", "_", "N");
        r_v("{");
        S();
        r_v("}");
        if (label_t[cen].num < label_t[cen].size && sym.token)	//填else的N
            backtrack(label_t[cen].label_n[++label_t[cen].num]);
        cen--;
    }
    else if (strcmp(sym.token, "ID") != 0 && strcmp(sym.value, "while") != 0 && strcmp(sym.value, "if") != 0 && strcmp(sym.value, "}") != 0)
    {
        cout << "第" << sym.layer << "行";
        cout << "ERROR:if语句语法错误." << endl;
        fprintf(fp_g, "ERROR:if语句语法错误.\n");
        char m[50];
        sprintf(m, "%d", sym.layer);
        strcat(m, "行错误：if语句语法错误");

        cout<<m<<endl;
        throw m;

    }
}


//E->TE1
//E1->+TE1|-TE1|ε
//T->FT1
//T1->*FT1|/FT1|ε
//F->(E)|ID|NUM

//E->TE1
char* E()
{
    if (strcmp(sym.token, "ID") == 0 || strcmp(sym.value, "(") == 0 || strcmp(sym.token, "NUM") == 0)
    {
        T();
        strcpy(l, E1());
        return l;
    }
    else { cout << "第" << sym.layer << "行"; cout << "ERROR:表达式缺少对象." << endl;
        fprintf(fp_g, "ERROR:表达式缺少对象.\n");
        char m[50];
        sprintf(m, "%d", sym.layer);
        strcat(m, "行错误：算术表达式缺少对象");

        cout<<m<<endl;
        throw m;
    }
}
//E1->+TE1|-TE1|ε
char* E1()
{
    if (strcmp(sym.value, "+") == 0)
    {
        op_t.push("+");
        r_v("+");
        T();
        E1();
    }
    else if (strcmp(sym.value, "-") == 0)
    {
        op_t.push("-");
        r_v("-");
        T();
        E1();
    }
    else if (strcmp(sym.value, ")") != 0 && strcmp(sym.value, ";") != 0)
    {
        cout << "第" << sym.layer << "行";
        cout << "ERROR:运算符表达错误." << endl;
        fprintf(fp_g, "ERROR:运算符表达错误.\n");
        char m[50];
        sprintf(m, "%d", sym.layer);
        strcat(m, "行错误：运算符表达式错误1");

        cout<<m<<endl;
        throw m;

    }
    else if (strcmp(sym.value, ")") == 0 || strcmp(sym.value, ";") == 0)
    {//生成四元式
        if (!op_t.empty())
        {
            char x[50];
            string y, z;
            strcpy(x, newtemp());
            y = var_t.top(); var_t.pop();
            z = var_t.top(); var_t.pop();
            semit(op_t.top(), z, y, x);
            op_t.pop();
            strcpy(l, x);
            var_t.push(l);
        }
        return l;
    }

}
//T->FT1
char* T()
{
    if (strcmp(sym.value, "(") == 0 || strcmp(sym.token, "ID") == 0 || strcmp(sym.token, "NUM") == 0)
    {
        F();
        T1();
        return l;
    }
    else { cout << "第" << sym.layer << "行"; cout << "ERROR:表达式缺少对象." << endl; fprintf(fp_g, "ERROR:表达式缺少对象.\n");
        char m[50];
        sprintf(m, "%d", sym.layer);
        strcat(m, "行错误：算术表达式缺少对象");

        cout<<m<<endl;
        throw m; }
}
//T1->*FT1|/FT1|ε
char* T1()
{
    if (strcmp(sym.value, "*") == 0)
    {
        op_t.push("*");
        r_v("*");
        F();
        T1();
    }
    else if (strcmp(sym.value, "/") == 0)
    {
        op_t.push("/");
        r_v("/");
        F();
        T1();
    }
    else if (strcmp(sym.value, "+") != 0 && strcmp(sym.value, "-") != 0 && strcmp(sym.value, ")") != 0 && strcmp(sym.value, ";") != 0)
    {
        cout << "第" << sym.layer << "行";

        cout << "ERROR:运算符表达错误." << endl;
        fprintf(fp_g, "ERROR:运算符表达错误.\n");
        char m[50];
        sprintf(m, "%d", sym.layer);
        strcat(m, "行错误：算数运算符表达错误");

        cout<<m<<endl;
        throw m;
    }
    else if (strcmp(sym.value, "+") == 0 || strcmp(sym.value, "-") == 0 || strcmp(sym.value, ")") == 0 || strcmp(sym.value, ";") == 0)
    {//生成四元式
        if (!op_t.empty())
        {
            char x[50];
            string y, z;
            strcpy(x, newtemp());
            y = var_t.top(); var_t.pop();
            z = var_t.top(); var_t.pop();
            semit(op_t.top(), z, y, x);
            op_t.pop();
            strcpy(l, x);
            var_t.push(l);
        }
        return l;
    }
}
//F->(E)|ID|NUM   //HS
char* F()
{
    if (strcmp(sym.value, "(") == 0)
    {
        r_v("(");
        while (!var_t.empty())
        {
            temp_kv.push(var_t.top());
            var_t.pop();
        }
        while (!op_t.empty())
        {
            temp_ko.push(op_t.top());
            op_t.pop();
        }
        l = E();
        while (!temp_kv.empty())
        {
            var_t.push(temp_kv.top());
            temp_kv.pop();
        }
        while (!temp_ko.empty())
        {
            op_t.push(temp_ko.top());
            temp_ko.pop();
        }
        var_t.push(l);
        r_v(")");
    }
    else if (strcmp(sym.token, "ID") == 0)
    {
        lookup(sym.value);
        char a[100];
        char b[100];
        strcpy(a, sym.value);

        r_t("ID");
        if (strcmp(sym.value, "(") == 0)
        {
            strcpy(b,zhuhanshu_fanhuizhi(a));
            var_t.push(b);
            chuancan_xuhao = 0;
            HS(a);
            semit("=", oldfanhuizhi(a), "_", b);

    }
        else
            var_t.push(a);
    }
    else if (strcmp(sym.token, "NUM") == 0)
    {
        var_t.push(sym.value);
        r_t("NUM");
    }
    return 0;
}

//B->CB1
//B1->||CB1|ε
//C->YC1
//C1->&&YC1|ε
//Y->IDY1
//Y1->relop ID|ε

int B()
{
    if (strcmp(sym.token, "ID") == 0)
    {
        C();
        B1();
    }
    else { cout << "第" << sym.layer << "行"; cout << "ERROR:布尔表达式缺少ID.";
        fprintf(fp_g, "ERROR:布尔表达式缺少ID.\n");
        char m[50];
        sprintf(m, "%d", sym.layer);
        strcat(m, "行错误：布尔表达式缺少对象");

        cout<<m<<endl;
        throw m;
        }
}
//B1->||CB1|ε
int B1()
{
    if (strcmp(sym.value, "||") == 0)
    {

        if (label_t[cen].num < label_t[cen].size)
        {		//填||之前的N
            backtrack(label_t[cen].label_n[++label_t[cen].num]);
        }
        lo_t.push("||");
        r_v("||");
        C();
        if (label_t[cen].num < label_t[cen].size)		//填||之前的N
        {
            backtrack(label_t[cen].label_n[++label_t[cen].num]);
        }
        B1();
    }
    else if (strcmp(sym.value, ")") != 0 && strcmp(sym.value, ";") != 0)
    {
        cout << "ERROR:布尔表达式缺少'('. "<< endl;
        fprintf(fp_g, "ERROR:布尔表达式缺少'('.\n");
        char m[50];
        sprintf(m, "%d", sym.layer);
        strcat(m, "行错误：布尔表达式缺左括号");

        cout<<m<<endl;
        throw m;
    }
    else if (strcmp(sym.value, ")") == 0)
    {
        if (!lo_t.empty())
        {
            char x[50];
            string y, z;
            strcpy(x, newtemp());
            y = var_t.top(); var_t.pop();
            z = var_t.top(); var_t.pop();
            semit(lo_t.top(), z, y, x);
            lo_t.pop();
            l = newtemp();
            semit("jnz", x, "_", newl());
            semit("=", "0", "_", l);
            label_t[cen].label_n[++label_t[cen].size] = cnt;
            semit("jp", "_", "_", "N");
            semit("=", "1", "_", l);
            var_t.push(l);
        }
    }
}
//C->YC1
int C()
{
    if (strcmp(sym.token, "ID") == 0)
    {
        Y();
        C1();
    }
    else { cout << "ERROR:布尔表达式缺少ID."; fprintf(fp_g, "ERROR:布尔表达式缺少ID.\n");  char m[50];
        sprintf(m, "%d", sym.layer);
        strcat(m, "行错误：布尔表达式缺少ID");

        cout<<m<<endl;
        throw m;}
}
//C1->&&YC1|ε
int C1()
{
    if (strcmp(sym.value, "&&") == 0)
    {
        if (label_t[cen].num < label_t[cen].size)		//填&&之前的N
        {
            backtrack(label_t[cen].label_n[++label_t[cen].num]);
        }
        lo_t.push("&&");
        r_v("&&");
        Y();
        if (label_t[cen].num < label_t[cen].size)		//填&&之前的N
        {
            backtrack(label_t[cen].label_n[++label_t[cen].num]);
        }
        C1();

    }
    else if (strcmp(sym.value, "||") != 0 && strcmp(sym.value, ")") != 0 && strcmp(sym.value, ";") != 0)
    {
        cout << "第" << sym.layer << "行";
        cout << "ERROR:布尔表达式错误." << endl;
        fprintf(fp_g, "ERROR:布尔表达式错误.\n");
        char m[50];
        sprintf(m, "%d", sym.layer);
        strcat(m, "行错误：布尔表达式错误");

        cout<<m<<endl;
        throw m;
    }
    else if (strcmp(sym.value, "||") == 0 || strcmp(sym.value, ")") == 0)
    {
        if (!lo_t.empty())
        {
            char x[50];
            string y, z;
            strcpy(x, newtemp());
            y = var_t.top(); var_t.pop();
            z = var_t.top(); var_t.pop();
            semit(lo_t.top(), z, y, x);
            lo_t.pop();
            l = newtemp();
            semit("jnz", x, "_", newl());
            semit("=", "0", "_", l);
            label_t[cen].label_n[++label_t[cen].size] = cnt;
            semit("jp", "_", "_", "N");
            semit("=", "1", "_", l);
            var_t.push(l);
        }
    }
}
//Y->IDY1
int Y()
{
    if(strcmp(sym.token, "ID") == 0)
    {
        lookup(sym.value);
        char a[100];
        char b[100];
        strcpy(a, sym.value);

        r_t("ID");
        if (strcmp(sym.value, "(") == 0)
        {
            strcpy(b, zhuhanshu_fanhuizhi(a));
            var_t.push(b);
            chuancan_xuhao = 0;
            HS(a);
            semit("=", oldfanhuizhi(a), "_", b);

        }
        else
            var_t.push(a);
    }
    Y1();
    return 0;
    /*lookup(sym.value);
    var_t.push(sym.value);
    r_t("ID");
    Y1();
    return 0;*/
}
//Y1->relop Y2|ε
int Y1()
{
    if (strcmp(sym.value, ">") == 0)
    {
        char* s, *a, *c; int i;
        r_v(">");
        c = Y2();
        s = newl();
        semit("j>", var_t.top(), c, s);
        var_t.pop();
        a = newtemp();
        semit("=", "0", "_", a);
        label_t[cen].label_n[++label_t[cen].size] = cnt;
        semit("jp", "_", "_", "N");
        semit("=", "1", "_", a);
        var_t.push(a);
    }
    else if (strcmp(sym.value, "<") == 0)
    {
        char* s, *a, *c; int i;
        r_v("<");
        c = Y2();
        s = newl();
        semit("j<", var_t.top(), c, s);
        var_t.pop();
        a = newtemp();
        semit("=", "0", "_", a);
        label_t[cen].label_n[++label_t[cen].size] = cnt;
        semit("jp", "_", "_", "N");
        semit("=", "1", "_", a);
        var_t.push(a);
    }
    else if (strcmp(sym.value, ">=") == 0)
    {
        char* s, *a, *c; int i;
        r_v(">=");
        c = Y2();
        s = newl();
        semit("jn<", var_t.top(), c, s);
        var_t.pop();
        a = newtemp();
        semit("=", "0", "_", a);
        label_t[cen].label_n[++label_t[cen].size] = cnt;
        semit("jp", "_", "_", "N");
        semit("=", "1", "_", a);
        var_t.push(a);
    }
    else if (strcmp(sym.value, "<=") == 0)
    {
        char* s, *a, *c; int i;
        r_v("<=");
        c = Y2();
        s = newl();
        semit("jn>", var_t.top(), c, s);
        var_t.pop();
        a = newtemp();
        semit("=", "0", "_", a);
        label_t[cen].label_n[++label_t[cen].size] = cnt;
        semit("jp", "_", "_", "N");
        semit("=", "1", "_", a);
        var_t.push(a);
    }
    else if (strcmp(sym.value, "==") == 0)
    {
        char* s, *a, *c; int i;
        r_v("==");
        c = Y2();
        s = newl();
        semit("j=", var_t.top(), c, s);
        var_t.pop();
        a = newtemp();
        semit("=", "0", "_", a);
        label_t[cen].label_n[++label_t[cen].size] = cnt;
        semit("jp", "_", "_", "N");
        semit("=", "1", "_", a);
        var_t.push(a);
    }
    else if (strcmp(sym.value, "!=") == 0)
    {
        char* s, *a, *c; int i;
        r_v("!=");
        c = Y2();
        s = newl();
        semit("jn=", var_t.top(), c, s);
        var_t.pop();
        a = newtemp();
        semit("=", "0", "_", a);
        label_t[cen].label_n[++label_t[cen].size] = cnt;
        semit("jp", "_", "_", "N");
        semit("=", "1", "_", a);
        var_t.push(a);
    }
    else if (strcmp(sym.value, "&&") != 0 && strcmp(sym.value, "||") != 0 && strcmp(sym.value, ")") != 0 && strcmp(sym.value, ";") != 0)
    {
        cout << "第" << sym.layer << "行";
        cout << "ERROR:逻辑运算表达错误." << endl;
        fprintf(fp_g, "ERROR:逻辑运算表达错误.\n");
        exit(0);
    }
    else if (strcmp(sym.value, "&&") == 0 || strcmp(sym.value, "||") != 0 || strcmp(sym.value, ")") == 0)
    {
        char* s, *a; int i;
        s = newl();
        semit("jnz", var_t.top(), "_", s);
        var_t.pop();
        a = newtemp();
        semit("=", "0", "_", a);
        label_t[cen].label_n[++label_t[cen].size] = cnt;
        semit("jp", "_", "_", "N");
        semit("=", "1", "_", a);
        var_t.push(a);
    }
}

//Y2->ID|NUM |HS
char* Y2()
{
    if (strcmp(sym.token, "ID") == 0)
    {
        char a[100];
        char b[100];
        lookup(sym.value);
        strcpy(l, sym.value);
        strcpy(a, sym.value);
        r_t("ID");

            if (strcmp(sym.value, "(") == 0)
            {
                strcpy(b, zhuhanshu_fanhuizhi(a));
                chuancan_xuhao = 0;
                HS(a);
                semit("=", oldfanhuizhi(a), "_", b);
                strcpy(l, b);
                return l;
            }
            else

               return l;
    }

    else if (strcmp(sym.token, "NUM") == 0)
    {
        strcpy(l, sym.value);
        r_t("NUM");
        return l;
    }


}

char* Y3()    //WUYONG
{
    if (strcmp(sym.token, "ID") == 0)
    {
        lookup(sym.value);
        strcpy(l, sym.value);
        r_t("ID");
        return l;
    }
    else if (strcmp(sym.token, "NUM") == 0)
    {
        strcpy(l, sym.value);
        r_t("NUM");
        return l;
    }
}

//HS->ID( J)

char* HS(char *a)
{
    int i;
    char qq[20];
    r_v("(");
    J(a);
    r_v(")");
    for ( i = 1; i <= hanshu_cnt; i++)
        if (strcmp(a, hanshu[i].name) == 0)
        {
            sprintf(qq, "%d", hanshu[i].shoudizhi);
            break;
        }
    semit("=",creat(cnt+2), "_", newfanhuidizhi(i));
    semit("jp", "_", "_", qq);
    //hanshu[i].weidizhi[hanshu[i].shiyongcishu] = cnt;

    return l;
 }
//J->Y3 xx
char* J(char *a)
{
    if (strcmp(sym.token, "ID") == 0)
    {
        lookup(sym.value);
        strcpy(l, sym.value);
        semit("=", sym.value, "_", chuancan(a));
        r_t("ID");

    }
    else if (strcmp(sym.token, "NUM") == 0)
    {
        strcpy(l, sym.value);
        semit("=", sym.value, "_", chuancan(a));
        r_t("NUM");

    }
    xx(a);
    return l;
}
//xx-> ,J | ε
void xx(char *a)
{

    if (strcmp(sym.value, ",") == 0)
    {
        r_v(",");
        J(a);
    }
}
//explain head
struct elems_e {          //保存从语义分析结果result2.txt中取出的结果
    char op[10];
    char s1[50] = "\0";
    char s2[50] = "\0";
    char rs[50] = "\0";
}elems_e[20000];

struct table_e {      //作为符号表存在，提供符号表存入功能，方便之后的查表以及最后的输出结果。
    string s;
    string type = "\0";
    int value;
}table_e[500];

FILE *fp_e;     //指向result2.txt的指针

int line = 1;		//总行数
int cline = 1;	//当前行号
int size1 = 0;

//将语义分析产生的四元组存入结构体数组elems_e中
int readf_e()
{

    FILE *fp = fopen("D:\\result2.txt", "r");
    char buf[100];

    for (int mm = 0; mm < 100; mm++) buf[mm] = 'a';
    //cout << "0" << endl;

    int i = 0, l;
    fgets(buf, 100, fp);


    while (!feof(fp))
    {

        //for (int mm = 0; mm < 20; mm++) cout << buf[mm];

        //以下三句话修了一个很严重的bug
        for (int mm = 0; mm < 100; mm++)
            if (buf[mm] == '#')
                goto L1;


        //cout << "A" << endl;

        int k = 0;
        //printf("%d\n", buf);
        while (buf[k] != '(')
        {
            k++;

            //if (k > 200) {
            //	cout << "  k=" << k;//这里是在复现bug
            //}
            //if (k = 100) goto L1;
        }
        k++;
        int m = 0;
        while (buf[k] != ',')
        {
            elems_e[line].op[m] = buf[k];
            k++; m++;
        }
        elems_e[line].op[m] = '\0';

        k++; m = 0;
        while (buf[k] != ',')
        {
            elems_e[line].s1[m] = buf[k];
            k++; m++;
        }
        elems_e[line].s1[m] = '\0';


        k++; m = 0;
        while (buf[k] != ',')
        {
            elems_e[line].s2[m] = buf[k];
            k++; m++;
            //if (k >= 1788) {
            //	cout << "  k=" << k;
            //}
        }
        elems_e[line].s2[m] = '\0';

        k++; m = 0;
        while (buf[k] != ')')
        {
            elems_e[line].rs[m] = buf[k];
            k++; m++;
        }
        elems_e[line].rs[m] = '\0';
        line++;

        for (int mm = 0; mm < 100; mm++) buf[mm] = 'a';
        //for (int mm = 0; mm < 20; mm++) cout << buf[mm];
    //	cout << "B" << endl;

        fgets(buf, 100, fp);

        //for (int mm = 0; mm < 20; mm++) cout << buf[mm];
        //cout << "C" << endl;
    }
L1:line++;
    fclose(fp);

    return 0;
}
//查询表中是否存在该变量，若不存在，将该变量放入符号表中
int lo_p(string t)
{
    int i = 0;
    while (i < size1)
    {
        if (t == table_e[i].s)
        {
            return 0;
        }
        i++;
    }
    table_e[size1++].s = t;
    return 1;
}
//改变符号表中的特定变量的值 ,即改变value
void changevalue(string t, int value)
{
    int i = 0;
    while (i < size1)
    {
        if (t == table_e[i].s)
        {
            table_e[i].value = value;
            break;
        }
        i++;
    }
}
//返回可以运算的value值 ，即根据变量名，返回它的值
int value(string t)
{
    int i = 0, value;
    while (i < size1)
    {
        if (t == table_e[i].s)
        {
            value = table_e[i].value;
            return value;
        }
        i++;
    }
    value = atoi(t.c_str());
    return value;
}

//核心函数，根据四元式来执行算数逻辑上的运算
void carry(int i)
{
    char* s;
    s = elems_e[i].op;
    if (strcmp(s, "=") == 0)
    {
        lo_p(elems_e[i].rs);     //查询变量是否存在
        changevalue(elems_e[i].rs, value(elems_e[i].s1));    //修改变量的值
        cline++;
    }
    else if (strcmp(s, "+") == 0)
    {
        int m;
        lo_p(elems_e[i].rs);
        m = value(elems_e[i].s1) + value(elems_e[i].s2);   //返回变量的值
        changevalue(elems_e[i].rs, m);
        cline++;
    }
    else if (strcmp(s, "-") == 0)
    {
        int m;
        lo_p(elems_e[i].rs);
        m = value(elems_e[i].s1) - value(elems_e[i].s2);
        changevalue(elems_e[i].rs, m);
        cline++;
    }
    else if (strcmp(s, "*") == 0)
    {
        int m;
        lo_p(elems_e[i].rs);
        m = value(elems_e[i].s1)*value(elems_e[i].s2);
        changevalue(elems_e[i].rs, m);
        cline++;
    }
    else if (strcmp(s, "/") == 0)
    {
        int m;
        lo_p(elems_e[i].rs);
        m = value(elems_e[i].s1) / value(elems_e[i].s2);
        changevalue(elems_e[i].rs, m);
        cline++;
    }
    else if (strcmp(s, "j>") == 0)
    {
        bool b;
        b = (value(elems_e[i].s1) > value(elems_e[i].s2));
        if (b)
        {
            cline = value(elems_e[i].rs);
        }
        else cline++;
    }
    else if (strcmp(s, "j<") == 0)
    {
        bool b;
        b = (value(elems_e[i].s1) < value(elems_e[i].s2));
        if (b)
            cline = value(elems_e[i].rs);
        else cline++;
    }
    else if (strcmp(s, "j=") == 0)
    {
        bool b;
        b = (value(elems_e[i].s1) == value(elems_e[i].s2));
        if (b)
            cline = value(elems_e[i].rs);
        else cline++;
    }
    else if (strcmp(s, "jp") == 0)
    {
        cline = value(elems_e[i].rs);
    }
    else if (strcmp(s, "||") == 0)
    {
        bool b;
        lo_p(elems_e[i].rs);
        b = (value(elems_e[i].s1) || value(elems_e[i].s2));
        changevalue(elems_e[i].rs, b);
        cline++;
    }
    else if (strcmp(s, "&&") == 0)
    {
        bool b;
        lo_p(elems_e[i].rs);
        b = (value(elems_e[i].s1) && value(elems_e[i].s2));
        changevalue(elems_e[i].rs, b);
        cline++;
    }
    else if (strcmp(s, "jnz") == 0)
    {
        if (value(elems_e[i].s1))
        {
            cline = value(elems_e[i].rs);
        }
        else cline++;
    }
    else if (strcmp(s, "jn>") == 0)
    {
        bool b;
        b = (value(elems_e[i].s1) <= value(elems_e[i].s2));
        if (b)
            cline = value(elems_e[i].rs);
        else cline++;
    }
    else if (strcmp(s, "jn<") == 0)
    {
        bool b;
        b = (value(elems_e[i].s1) >= value(elems_e[i].s2));
        if (b)
            cline = value(elems_e[i].rs);
        else cline++;
    }
    else if (strcmp(s, "jn=") == 0)
    {
        bool b;
        b = (value(elems_e[i].s1) != value(elems_e[i].s2));
        if (b)
            cline = value(elems_e[i].rs);
        else cline++;
    }
}

int read_suc= 0;
int word_suc= 0;
int gram_suc =0;
int explain_suc= 0;



Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    remove("D:\\result1.txt");
    remove("D:\\result2.txt");
    remove("D:\\result3.txt");
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

//菜单在这里
void Widget::on_pushButton_clicked()
{
    int index = ui->comboBox->currentIndex()+1;
    qDebug() << index << endl;

    switch (index)
    {
     static  int  code_layer[10000];
        case 1://读取文件
    { ui->stackedWidget->setCurrentIndex(0);
        if(read_suc)break;
        char Project[10000];
                int p = 0;
                FILE *fp2;
                if ((fp2 = fopen("D:\\test.txt", "r")) == NULL)
                {
                    cout << "can't open this file";
                    QMessageBox::information(this, "ERROR", "can not open this file!");break;
                }
                Project[p] = fgetc(fp2);
                while (Project[p] != EOF)
                {
                    p++;
                    Project[p] = fgetc(fp2);
                }
                Project[++p] = '\0';
                fclose(fp2);
                cout << endl << "读文件结果为:" << endl;
                cout << Project << endl;
                ui->textEdit->setText(Project);
                ui->stackedWidget->setCurrentIndex(0);
        read_suc=1;
        break;
    }
       case 2://词法分析
       {
         if (!read_suc)
            { QMessageBox::information(this, "ERROR", "you haven't read file successfully!");break;}
         if(word_suc) {ui->stackedWidget->setCurrentIndex(1);break;}

            char Project[10000];
            char token[20];
            int syn = -1, i;
            int p = 0;
            FILE *fp;
            if ((fp = fopen("D:\\test.txt", "r")) == NULL)
            {
                cout << "can't open this file";
                QMessageBox::information(this, "提示", "打开文件失败");

               break;
            }
            Project[p] = fgetc(fp);
            while (Project[p] != EOF)
            {
                p++;
                Project[p] = fgetc(fp);
            }
            Project[++p] = '\0';
            fclose(fp);
            cout << endl << "读文件结果为:" << endl;
            cout << Project << endl;
            try{
                filter(Project, p);}
            catch(char *i)
            {
               QString ts(QString::fromLocal8Bit(i));
                QMessageBox::information(this, "ERROR", ts);
                break;
            }
            cout << endl << "过滤之后的程序为:" << endl;
            cout << Project << endl << endl;
            p = 0;
            if ((fp1 = fopen("D:\\result1.txt", "w+")) == NULL)
            {
                cout << "打开文件错误";
 QMessageBox::information(this, "提示", "打开文件失败");
                break;
            }
            cout <<endl<< "生成的二元组为:" << endl;
            int layer_count = 1;

                while (syn != 0)//打印二元组
            {

                    Scanner(syn, Project, token, p);

                cout << recent_layer;
                code_layer[layer_count] = recent_layer;//这个数组与二元组前的行数一一对应
                layer_count++;
                if (syn == 69)   //变量
                {
                    cout << "<" << token << ",ID>" << endl;
                    fprintf(fp1, "<ID,%s>\n", token);
                }
                else if (syn >= 1 && syn <= 22)  //保留字
                {
                    cout << "<" << reserveWord[syn - 1] << ",reserve_w>" << endl;
                    fprintf(fp1, "<reserve_w,%s>\n", reserveWord[syn - 1]);
                }
                else if (syn >= 23 && syn <= 31)  //类型
                {
                    cout << "<" << type[syn - 23] << ",type_w>" << endl;
                    fprintf(fp1, "<type_w,%s>\n", type[syn - 23]);
                }
                else if (syn >= 32 && syn <= 53) //运算符
                {
                    cout << "<" << operatorer[syn - 32] << ",oper>" << endl;
                    fprintf(fp1, "<oper,%s>\n", operatorer[syn - 32]);
                }
                else if (syn >= 54 && syn <= 59) //界符
                {
                    cout << "<" << limiter[syn - 54] << ",limit>" << endl;
                    fprintf(fp1, "<limit,%s>\n", limiter[syn - 54]);
                }
                else if (syn >= 60 && syn <= 67) //逻辑运算符
                {
                    cout << "<" << logicer[syn - 60] << ",logic>" << endl;
                    fprintf(fp1, "<logic,%s>\n", logicer[syn - 60]);
                }
                else if (syn == 68)		 //数字
                {
                    cout << "<" << token << ",NUM>" << endl;
                    fprintf(fp1, "<NUM,%s>\n", token);
                }
                else if (syn == -2)
                {
                    cout << "Error：变量名定义错误 " << "\n";
                    fprintf(fp1, "Error：变量名定义错误\n");
                     QMessageBox::information(this, "提示", "变量名定义错误");break;
                }

           }

                fclose(fp1);
            cout << endl;
            code_layer[layer_count - 1] = 0;
            code_layer[layer_count - 1] = 0;
            word_suc=1;
//词法分析读文件

        p = 0;
        FILE *fp2;
        if ((fp2 = fopen("D:\\result1.txt", "r")) == NULL)
        {
            cout << "can't open this file";
             QMessageBox::information(this, "提示", "打开文件失败");break;
        }
        Project[p] = fgetc(fp2);
        while (Project[p] != EOF)
        {
            p++;
            Project[p] = fgetc(fp2);
        }
        Project[++p] = '\0';
        fclose(fp2);
        cout << endl << "读文件结果为:" << endl;
        cout << Project << endl;
        ui->textEdit_2->setText(Project);
        ui->stackedWidget->setCurrentIndex(1);
        break;
    }
    case 3://语法语义分析
    {
         if(!(read_suc&&word_suc))
         {
             QMessageBox::information(this, "ERROR", "you can not do this now!");break;}

         if(gram_suc){ui->stackedWidget->setCurrentIndex(2);break;}
        //打印四元组
        fp_g = fopen("D:\\result2.txt", "w+");

        if (fp_g == NULL)
        {
            cout << "can't open this file";
             QMessageBox::information(this, "提示", "打开文件失败");break;
        }
        readf();
        for (int i = 1; code_layer[i] != 0; i++) {
            elems[i].layer = code_layer[i];
        }
       try
        {
        SS();
        }
        catch(char *i)
        {
           QString ts(QString::fromLocal8Bit(i));
            QMessageBox::information(this, "ERROR", ts);
            break;
        }
        fclose(fp_g);
        gram_suc=1;
    //语法语义分析读文件

                char Project[10000];
                int p = 0;
                FILE *fp3;
                if ((fp3 = fopen("D:\\result2.txt", "r")) == NULL)
                {
                    cout << "can't open this file";
                    QMessageBox::information(this, "ERROR", "can't open this file");
                    break;
                }
                Project[p] = fgetc(fp3);
                while (Project[p] != EOF)
                {
                    p++;
                    Project[p] = fgetc(fp3);
                }
                Project[++p] = '\0';
                fclose(fp3);
                cout << endl << "读文件结果为:" << endl;
                cout << Project << endl;
                ui->textEdit_3->setText(Project);
                ui->stackedWidget->setCurrentIndex(2);
                break;
    }
    case 4://解释程序
    {
        if(!(word_suc&&gram_suc&&read_suc))
        {
            QMessageBox::information(this, "ERROR", "you can not do this now!");break;
        }
        if(explain_suc) {ui->stackedWidget->setCurrentIndex(3);break;}
        if ((fp_e = fopen("D:\\result3.txt", "w+")) == NULL)
        {
            cout << "can't open this file";
            QMessageBox::information(this, "ERROR", "can't open this file");
            exit(0);
        }
        readf_e();
        int i = 0;
        line--; int cnt = 1;
        while (cline < line)
        {
            //cout << "(" << elems_e[cline].op << "," << elems_e[cline].s1 << "," << elems_e[cline].s2 << "," << elems_e[cline].rs << ")" << endl;
            carry(cline);
            cnt++;
            if (cnt > 100000)
            {
                 QMessageBox::information(this, "ERROR", "code is too big!");break;
            }
        }
        cout << endl << "输出如下：" << endl;
            for (int m = 0; m<=show_list_count; m++)

            {
                i = 0;
                while (i < size1)
                {
                    if (strcmp(show_list[m], table_e[i].s.c_str()) == 0)
                    {
                        cout << table_e[i].s << "    " << table_e[i].value << endl;
                        fprintf(fp_e, "ID: %2s : %2d\n", table_e[i].s.c_str(), table_e[i].value);
                    }
                    i++;
                }

            }
        fclose(fp_e);
        explain_suc=1;
        //解释程序读文件

        char Project[10000];
        int p = 0;
                FILE *fp4;
                if ((fp4 = fopen("D:\\result3.txt", "r")) == NULL)
                {
                    cout << "can't open this file";
                    QMessageBox::information(this, "ERROR", "can't open this file!");break;
                }
                Project[p] = fgetc(fp4);
                while (Project[p] != EOF)
                {
                    p++;
                    Project[p] = fgetc(fp4);
                }
                Project[++p] = '\0';
                fclose(fp4);
                cout << endl << "读文件结果为:" << endl;
                cout << Project << endl;
                ui->textEdit_4->setText(Project);
                ui->stackedWidget->setCurrentIndex(3);
                break;
    }
}
}

