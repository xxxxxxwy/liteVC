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



//�����ֱ�
static char reserveWord[][20] = {
    "auto", "break", "case", "const", "continue",
    "default", "do", "else", "extern", "for", "goto", "if",
    "register", "return", "sizeof", "static",
    "switch", "typedef", "unsigned","show", "while","main"
};

//���͹ؼ��ֱ�
static char type[][20] = {
    "int", "long", "char", "double", "short",
    "struct", "void", "enum", "union"
};

//�������
static char operatorer[][5] = {
    "+", "-", "*", "/",
    ";", "^", ",", "\"", "\'", "#", "&",
    "|", "%", "~", "<<", ">>",
    "\\", ".", "\?", ":", "!", "="
};

//�����
static char limiter[][5] = {
    "(", ")", "{", "}", "[", "]"
};

//�߼����ű�
static char logicer[][5] = {
    "<", "<=", ">", ">=", "==", "!=", "&&", "||"
};

char IDentifier[][20] = { "" };  //��ű�ʶ��
static int Project_layer[10000];//��Ŵ�����������
char Project[10000];     //��ų������
char token[20] = "";            //ÿ��ɨ���ʱ��洢�Ѿ�ɨ��Ľ��
int syn = -1;                  //�ֱ��룬��syn=0ʱ��ΪԴ�����������
int p = 0;
FILE* fp1;


//�ж��Ƿ�Ϊ������
int searchReserve(char reserveWord[][20], char s[])
{
    for (int i = 0; i < reserveN; i++)
    {
        if (strcmp(s, reserveWord[i]) == 0)
            return i + 1;
    }
    return -1;
}

//�ж��Ƿ�Ϊ���ͷ�
int searchType(char type[][20], char s[])
{
    for (int i = 0; i < 9; i++)
    {
        if (strcmp(s, type[i]) == 0)
            return i + 23;
    }
    return -1;
}

//�ж��Ƿ�Ϊ��ĸ
bool Isletter(char letter)
{
    if ((letter <= 'z' && letter >= 'a') || (letter <= 'Z' && letter >= 'A') || letter == '_')
        return true;
    else return false;
}

//�ж��Ƿ�Ϊ����
bool Isnumber(char digit)
{
    int num = digit - '0';
    if (num <= 9 && num >= 0)
        return true;
    else return false;
}

//���˵������е��޹��ַ��������з�'\n'��ע�ʹ��룩
void filter(char Project[], int p)
{
    int code_layer_recent = 1;
    char temp[10000];
    int count = 0;
    for (int i = 0; i <= p; i++)
    {
        if (Project[i] == '/' && Project[i + 1] == '/')  //��Ϊ����ע�͡�//��,��ȥ��ע�ͺ���Ķ�����ֱ�������س�����
        {
            while (Project[i] != '\n')
            {
                i++;
            }
            code_layer_recent++;//��Ȼע�ͺ���Ļ���Ҳ����������
        }
        if (Project[i] == '/' && Project[i + 1] == '*')  //��Ϊ����ע�͡�/* ������*/����ȥ��������
        {
            i += 2;
            while (Project[i] != '*' || Project[i + 1] != '/')
            {
                if (Project[i] == '\n')code_layer_recent++;//����ע����Ҳ�л���
                i++;
                if (Project[i] == EOF)
                {
                    printf("ע�ͳ���û���ҵ� */��\n");
                    fprintf(fp1, "Error��ע�ͳ���û���ҵ� */��\n");
                    throw("ע�ͳ���û���ҵ� */");
                }
            }
            i += 2;//�����*/��
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

//ɨ����

int recent_layer = 1;

void Scanner(int& syn, char Project[], char token[], int& p)
{
    recent_layer = Project_layer[p];//���project[p]��Ӧ������
    int i, count = 0;
    char ch; 		   //��Ϊ�ж�ʹ��
    ch = Project[p];
    syn = -1;
    while (ch == ' ')  //���˵���ͷ�Ŀո�
    {
        p++;
        ch = Project[p];
    }
    for (i = 0; i < 20; i++)
    {
        token[i] = '\0';
    }
    if (Isletter(Project[p]) || Project[p] == '_')  //��ͷΪ��ĸ�����»���
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
        if (syn == -1)  //�����Ǳ������������͹ؼ��ֻ����Լ�����ı���
        {
            syn = searchType(type, token);
            if (syn == -1)
                syn = 69;  //�Լ�����ı���
        }
        return;
    }
    else if (Isnumber(Project[p]))  //���ַ�Ϊ����
    {
        if (Project[p] == 0)
        {
            if (Project[p + 1] != ' ')
            {
                cout << "Error���������������" << "\n";
                fprintf(fp1, "Error���������������\n");
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

    //��Ϊ�����
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
        p++;//��ǰ����
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
    else  if (Project[p] == EOF) //�ļ�������
    {
        syn = 0;
    }
    else  //���ܱ����ϴʷ�����ʶ��
    {
        cout << "ERROR���������޷�ʶ����ַ�" << endl;
        fprintf(fp1, "ERROR���������޷�ʶ����ַ�\n");
        throw("�������޷�ʶ����ַ�");


    }
}
//gram head

struct elem          //��Ŵʷ������Ľ��
{
    char token[50];
    char value[50];
    int layer = 0;
}sym;

struct table {             //���ű�
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
struct em {          //�����Ԫʽ
    string sop;
    string a;
    string b;
    string label;
}em[10000];//�������޸����±� ԭ����100

struct label_t {      //������ص���Ϣ
    int label_n[100] = { -1 };//��Ǹò㻹δ�滻���к�
    int size = 0;
    int num = 0;  //��������
}label_t[100];
struct elem elems[200000];
int line_g = 1;//һ������
int cline_g = 1;//Ŀǰ�ڼ���
int cnt = 1; //��Ԫʽ����
int temp = 1; //��ʱ�������
int temp_l = 1;//label�ı��
int cen = 0;//����
int shun = 1;//˳��
int s_l = 1;
char mmm[500];
char* l = mmm;
char mml[500];
char* t_l = mml;
FILE* fp_g;//�������޸���fp
int  xunhuan = 0; //�Ƿ���ѭ������
int hanshu_cnt = 0;  //��������������
char mmml[500];
char* ll = mmml;

char show_list[50][50] ;
int show_list_count = 0;

stack<string> var_t;//ֵջ
stack<string> op_t;//�����ջ
stack<string> lo_t;//�߼������ջ
stack<string> temp_kv;//ֵջ�����ֳ�
stack<string> temp_ko;//����������ֳ�
stack<string> temp_fh; //����ջ�����ֳ�............................
stack<int> temp_fh_gs; //����ջ���������ֳ�............................

//��������
void SS();
int HD();
int CS();
int P();
int M();
//�������岿��
int D();
int D1();
int D2();
//������䲿��
int S();
int X();
int K();
//������ʽ����
char* E();
char* E1();
char* T();
char* T1();
char* F();
//�������ʽ����
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



int readf()  //���ʷ������������ݴ���ṹ��������
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

elem getsym() //ȡ�ʺ��� ��ͬʱ��������һ�еĹ���
{
    return elems[cline_g++];
}

void r_v(const char* s)  //�������ʵ�valueֵ�Ƿ�ƥ��
{
    if (strcmp(sym.value, s) == 0)
    {
        sym = getsym();
    }
    else { cout << "��" << sym.layer << "��"; cout << "ERROR:ȱ�� " << s << "." << endl; fprintf(fp_g, "ERROR:ȱ��%s .\n", s);
        char m[50];
            sprintf(m, "%d", sym.layer);
            strcat(m, "�д���ȱ��");
            strcat(m, s);
            cout<<m<<endl;
            throw m;


     exit(0);
    }
}

void r_t(const char* s)  //�������ʵ�tokenֵ�Ƿ�ƥ��
{
    if (strcmp(sym.token, s) == 0)
    {
        sym = getsym();
    }
    else
    {
        cout << "��" << sym.layer << "��"; cout << "ERROR:ȱ�� " << s << "." << endl;
        fprintf(fp_g, "ERROR:ȱ��%s .\n", s);

        char m[50];
            sprintf(m, "%d", sym.layer);
            strcat(m, "�д���ȱ��");
            strcat(m, s);
            cout<<m<<endl;
            throw m;

    }
}
//������ʱ����tx
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
char* oldfanhuizhi(char*a)       //����ֱ��������
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
char* newl()//����+3 ��ת
{
    creat_s[0] = '/0';
    sprintf(creat_s, "%d", cnt + 3);
    return creat_s;
}
//�����
void backtrack(int i)//���ǰ����
{
    char s[50];
    sprintf(s, "%d", cnt);
    em[i].label = s;
}
//��int��ת��Ϊchar*����

char* creat(int i)
{

    creat_s[0] = '/0';
    sprintf(creat_s, "%d", i);
    return creat_s;
}
//�����������ӽ����ű�
int push_t(char* s)
{
    int i = 1;
    while (i <= table.size)
    {
        if (strcmp(table.var[i], s) == 0)
        {
            cout << "��" << sym.layer << "��";
            cout << "ERROR:�ض���" << s << "." << endl;
            fprintf(fp_g, "ERROR:�ض���%s .\n", s);

            char m[50];
            sprintf(m, "%d", sym.layer);
            strcat(m, "�д����ض���");
            strcat(m, s);
            cout<<m<<endl;
            throw m;


        }
        i++;
    }
    strcpy(table.var[++table.size], s);
    return 1;
}
//��ѯ���ű�
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
cout << "��" << sym.layer << "��";
    cout << "ERROR:δ����" << s << "." << endl;
    fprintf(fp_g, "ERROR:δ����%s .\n", s);
    char m[50];
        sprintf(m, "%d", sym.layer);
        strcat(m, "�д���δ����");
        strcat(m, s);
        cout<<m<<endl;
        throw m;
}
//������Ԫʽ����
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
        cout << endl << "���ɵ���ԪʽΪ��" << endl;
        while (i < cnt)
        {
            cout << i << "(" << em[i].sop << "," << em[i].a << "," << em[i].b << "," << em[i].label << ")" << endl;
            fprintf(fp_g, "%d  (%s,%s,%s,%s)\n", i, em[i].sop.c_str(), em[i].a.c_str(), em[i].b.c_str(), em[i].label.c_str());//�������޸���i++
            i++;
        }
        cout << endl << "�﷨��ȷ" << endl;
        fprintf(fp_g, "#right\n");
    }
    else { cout << "��" << sym.layer << "��"; cout << "ERROR:δ�ҵ�����"; fprintf(fp_g, "ERROR:δ�ҵ�����\n");
    throw("�޷���ɶ�Ԫ�������");}
}
//HD->P | type_w ID( CS|��){ D S return Y3 ;}HD
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
                while (table.size)        //�����ֳ�
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
             while (table.size != jj)      //huifu�ֳ�
                {
                 pp =  temp_fh.top().c_str();
                 strcpy(table.var[++table.size], pp);
                 temp_fh.pop();


                }

        }
        //���������ú���
        semit("jp", "_", "_",newfanhuidizhi(hanshu_cnt) );
        HD();
    }
    return 0;
}

//CS->type_w ID (, CS| ��)
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

        em[1].label = creat(cnt);   //����������ִ�л���


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
        while (table.size)        //�����ֳ�
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
        while (table.size != jj)      //huifu�ֳ�
        {
            pp = temp_fh.top().c_str();
            strcpy(table.var[++table.size], pp);
            temp_fh.pop();


        }

    }
    return 0;
}

//D->type_w IDD1;D2
//D1->,IDD1|��
//D2->D|��
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
//D1->,IDD1|��
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
        cout << "��" << sym.layer << "��";
        cout << "ERROR:ȱ��';'." << endl;
        fprintf(fp_g, "ERROR:ȱ��';' .\n");
        char m[50];
        sprintf(m, "%d", sym.layer);
        strcat(m, "�д���ȱ��';'");

        cout<<m<<endl;
        throw m;
    }
}
//D2->D|��
int D2()
{
    if (strcmp(sym.token, "type_w") == 0)
    {
        D();
    }
    else if (strcmp(sym.token, "ID") != 0 && strcmp(sym.value, "while") != 0 && strcmp(sym.value, "if") != 0 && strcmp(sym.value, "}") != 0 && strcmp(sym.value, "for") != 0)
    {
        cout << "��" << sym.layer << "��";
        cout << "ERROR:���Ͷ������." << endl;
        fprintf(fp_g, "ERROR:���Ͷ������.\n");
        char m[50];
        sprintf(m, "%d", sym.layer);
        strcat(m, "�д������Ͷ������");

        cout<<m<<endl;
        throw m;
    }
}

//S->ID=E;S| while(B){S}S | if(B){SK}XS | �� |for(ID=E;ID=E;B){S}S   |  show<<ID ;S
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
        cen++;			//����+1
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
            em[label_t[cen].label_n[++label_t[cen].num]].label = creat(cnt + 1); 	//����while��N
        }
        cen++;
        if (label_t[cen].num < label_t[cen].size)
        {
            em[label_t[cen].label_n[++label_t[cen].num]].label = creat(cnt + 1);
            //����break��N
        }
        cen--;
        semit("jp", "_", "_", creat(i));
        cen--;
        xunhuan -= 1;
        S();
    }

    else if (strcmp(sym.value, "for") == 0) //�������Ը�
    {
        int i;
        xunhuan += 1;
        cen++;			//����+1
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
            if (label_t[cen].num < label_t[cen].size)		//��||֮ǰ��N
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
            em[label_t[cen].label_n[++label_t[cen].num]].label = creat(cnt + 1); 	//����for��N
        }
        cen++;
        if (label_t[cen].num < label_t[cen].size)
        {
            em[label_t[cen].label_n[++label_t[cen].num]].label = creat(cnt + 1);
            //����break��N
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
        cen++;			//����+1
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
        //����N��
        X();
        S();
    }

      //S-> show<<ID;S
    else if (strcmp(sym.value, "show") == 0)
    {
        r_v("show");
        r_v("<<");
        strcpy(show_list[show_list_count++], sym.value);
        //cout <<"show˭��"<< show_list[show_list_count];
        r_t("ID");
        r_v(";");
        S();
    }

    else if (strcmp(sym.value, "}") != 0&&strcmp(sym.value, "break") != 0 && strcmp(sym.value, "return") != 0)
    {
    cout << "��" << sym.layer << "��";
        cout << "ERROR:ȱ��'}' in S" << endl;
        fprintf(fp_g, "ERROR:ȱ��'}' in S.\n");
        char m[50];
        sprintf(m, "%d", sym.layer);
        strcat(m, "�д�������S����follow��");

        cout<<m<<endl;
        throw m;


    }
}
//K->break ;|��
int K()
{

    if (strcmp(sym.value, "break") == 0)
    {

        if (xunhuan == 0) {
            cout << "ERROR:breakֻ����ѭ����ʹ��." << endl;
            fprintf(fp_g, "ERROR:breakֻ����ѭ����ʹ��.\n");
            throw("breakֻ����ѭ���е�if���ʹ��");
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
        cout << "ERROR:if���break�����﷨����." << endl;
        fprintf(fp_g, "ERROR:if���break�����﷨����.\n");
        char m[50];
        sprintf(m, "%d", sym.layer);
        strcat(m, "�д���if���break�����﷨����");

        cout<<m<<endl;
        throw m;
    }

    return 0;

}
//X->��|else {S}
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
        if (label_t[cen].num < label_t[cen].size && sym.token)	//��else��N
            backtrack(label_t[cen].label_n[++label_t[cen].num]);
        cen--;
    }
    else if (strcmp(sym.token, "ID") != 0 && strcmp(sym.value, "while") != 0 && strcmp(sym.value, "if") != 0 && strcmp(sym.value, "}") != 0)
    {
        cout << "��" << sym.layer << "��";
        cout << "ERROR:if����﷨����." << endl;
        fprintf(fp_g, "ERROR:if����﷨����.\n");
        char m[50];
        sprintf(m, "%d", sym.layer);
        strcat(m, "�д���if����﷨����");

        cout<<m<<endl;
        throw m;

    }
}


//E->TE1
//E1->+TE1|-TE1|��
//T->FT1
//T1->*FT1|/FT1|��
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
    else { cout << "��" << sym.layer << "��"; cout << "ERROR:���ʽȱ�ٶ���." << endl;
        fprintf(fp_g, "ERROR:���ʽȱ�ٶ���.\n");
        char m[50];
        sprintf(m, "%d", sym.layer);
        strcat(m, "�д����������ʽȱ�ٶ���");

        cout<<m<<endl;
        throw m;
    }
}
//E1->+TE1|-TE1|��
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
        cout << "��" << sym.layer << "��";
        cout << "ERROR:�����������." << endl;
        fprintf(fp_g, "ERROR:�����������.\n");
        char m[50];
        sprintf(m, "%d", sym.layer);
        strcat(m, "�д�����������ʽ����1");

        cout<<m<<endl;
        throw m;

    }
    else if (strcmp(sym.value, ")") == 0 || strcmp(sym.value, ";") == 0)
    {//������Ԫʽ
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
    else { cout << "��" << sym.layer << "��"; cout << "ERROR:���ʽȱ�ٶ���." << endl; fprintf(fp_g, "ERROR:���ʽȱ�ٶ���.\n");
        char m[50];
        sprintf(m, "%d", sym.layer);
        strcat(m, "�д����������ʽȱ�ٶ���");

        cout<<m<<endl;
        throw m; }
}
//T1->*FT1|/FT1|��
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
        cout << "��" << sym.layer << "��";

        cout << "ERROR:�����������." << endl;
        fprintf(fp_g, "ERROR:�����������.\n");
        char m[50];
        sprintf(m, "%d", sym.layer);
        strcat(m, "�д������������������");

        cout<<m<<endl;
        throw m;
    }
    else if (strcmp(sym.value, "+") == 0 || strcmp(sym.value, "-") == 0 || strcmp(sym.value, ")") == 0 || strcmp(sym.value, ";") == 0)
    {//������Ԫʽ
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
//B1->||CB1|��
//C->YC1
//C1->&&YC1|��
//Y->IDY1
//Y1->relop ID|��

int B()
{
    if (strcmp(sym.token, "ID") == 0)
    {
        C();
        B1();
    }
    else { cout << "��" << sym.layer << "��"; cout << "ERROR:�������ʽȱ��ID.";
        fprintf(fp_g, "ERROR:�������ʽȱ��ID.\n");
        char m[50];
        sprintf(m, "%d", sym.layer);
        strcat(m, "�д��󣺲������ʽȱ�ٶ���");

        cout<<m<<endl;
        throw m;
        }
}
//B1->||CB1|��
int B1()
{
    if (strcmp(sym.value, "||") == 0)
    {

        if (label_t[cen].num < label_t[cen].size)
        {		//��||֮ǰ��N
            backtrack(label_t[cen].label_n[++label_t[cen].num]);
        }
        lo_t.push("||");
        r_v("||");
        C();
        if (label_t[cen].num < label_t[cen].size)		//��||֮ǰ��N
        {
            backtrack(label_t[cen].label_n[++label_t[cen].num]);
        }
        B1();
    }
    else if (strcmp(sym.value, ")") != 0 && strcmp(sym.value, ";") != 0)
    {
        cout << "ERROR:�������ʽȱ��'('. "<< endl;
        fprintf(fp_g, "ERROR:�������ʽȱ��'('.\n");
        char m[50];
        sprintf(m, "%d", sym.layer);
        strcat(m, "�д��󣺲������ʽȱ������");

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
    else { cout << "ERROR:�������ʽȱ��ID."; fprintf(fp_g, "ERROR:�������ʽȱ��ID.\n");  char m[50];
        sprintf(m, "%d", sym.layer);
        strcat(m, "�д��󣺲������ʽȱ��ID");

        cout<<m<<endl;
        throw m;}
}
//C1->&&YC1|��
int C1()
{
    if (strcmp(sym.value, "&&") == 0)
    {
        if (label_t[cen].num < label_t[cen].size)		//��&&֮ǰ��N
        {
            backtrack(label_t[cen].label_n[++label_t[cen].num]);
        }
        lo_t.push("&&");
        r_v("&&");
        Y();
        if (label_t[cen].num < label_t[cen].size)		//��&&֮ǰ��N
        {
            backtrack(label_t[cen].label_n[++label_t[cen].num]);
        }
        C1();

    }
    else if (strcmp(sym.value, "||") != 0 && strcmp(sym.value, ")") != 0 && strcmp(sym.value, ";") != 0)
    {
        cout << "��" << sym.layer << "��";
        cout << "ERROR:�������ʽ����." << endl;
        fprintf(fp_g, "ERROR:�������ʽ����.\n");
        char m[50];
        sprintf(m, "%d", sym.layer);
        strcat(m, "�д��󣺲������ʽ����");

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
//Y1->relop Y2|��
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
        cout << "��" << sym.layer << "��";
        cout << "ERROR:�߼����������." << endl;
        fprintf(fp_g, "ERROR:�߼����������.\n");
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
//xx-> ,J | ��
void xx(char *a)
{

    if (strcmp(sym.value, ",") == 0)
    {
        r_v(",");
        J(a);
    }
}
//explain head
struct elems_e {          //���������������result2.txt��ȡ���Ľ��
    char op[10];
    char s1[50] = "\0";
    char s2[50] = "\0";
    char rs[50] = "\0";
}elems_e[20000];

struct table_e {      //��Ϊ���ű���ڣ��ṩ���ű���빦�ܣ�����֮��Ĳ���Լ�������������
    string s;
    string type = "\0";
    int value;
}table_e[500];

FILE *fp_e;     //ָ��result2.txt��ָ��

int line = 1;		//������
int cline = 1;	//��ǰ�к�
int size1 = 0;

//�����������������Ԫ�����ṹ������elems_e��
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

        //�������仰����һ�������ص�bug
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
            //	cout << "  k=" << k;//�������ڸ���bug
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
//��ѯ�����Ƿ���ڸñ������������ڣ����ñ���������ű���
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
//�ı���ű��е��ض�������ֵ ,���ı�value
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
//���ؿ��������valueֵ �������ݱ���������������ֵ
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

//���ĺ�����������Ԫʽ��ִ�������߼��ϵ�����
void carry(int i)
{
    char* s;
    s = elems_e[i].op;
    if (strcmp(s, "=") == 0)
    {
        lo_p(elems_e[i].rs);     //��ѯ�����Ƿ����
        changevalue(elems_e[i].rs, value(elems_e[i].s1));    //�޸ı�����ֵ
        cline++;
    }
    else if (strcmp(s, "+") == 0)
    {
        int m;
        lo_p(elems_e[i].rs);
        m = value(elems_e[i].s1) + value(elems_e[i].s2);   //���ر�����ֵ
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

//�˵�������
void Widget::on_pushButton_clicked()
{
    int index = ui->comboBox->currentIndex()+1;
    qDebug() << index << endl;

    switch (index)
    {
     static  int  code_layer[10000];
        case 1://��ȡ�ļ�
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
                cout << endl << "���ļ����Ϊ:" << endl;
                cout << Project << endl;
                ui->textEdit->setText(Project);
                ui->stackedWidget->setCurrentIndex(0);
        read_suc=1;
        break;
    }
       case 2://�ʷ�����
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
                QMessageBox::information(this, "��ʾ", "���ļ�ʧ��");

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
            cout << endl << "���ļ����Ϊ:" << endl;
            cout << Project << endl;
            try{
                filter(Project, p);}
            catch(char *i)
            {
               QString ts(QString::fromLocal8Bit(i));
                QMessageBox::information(this, "ERROR", ts);
                break;
            }
            cout << endl << "����֮��ĳ���Ϊ:" << endl;
            cout << Project << endl << endl;
            p = 0;
            if ((fp1 = fopen("D:\\result1.txt", "w+")) == NULL)
            {
                cout << "���ļ�����";
 QMessageBox::information(this, "��ʾ", "���ļ�ʧ��");
                break;
            }
            cout <<endl<< "���ɵĶ�Ԫ��Ϊ:" << endl;
            int layer_count = 1;

                while (syn != 0)//��ӡ��Ԫ��
            {

                    Scanner(syn, Project, token, p);

                cout << recent_layer;
                code_layer[layer_count] = recent_layer;//����������Ԫ��ǰ������һһ��Ӧ
                layer_count++;
                if (syn == 69)   //����
                {
                    cout << "<" << token << ",ID>" << endl;
                    fprintf(fp1, "<ID,%s>\n", token);
                }
                else if (syn >= 1 && syn <= 22)  //������
                {
                    cout << "<" << reserveWord[syn - 1] << ",reserve_w>" << endl;
                    fprintf(fp1, "<reserve_w,%s>\n", reserveWord[syn - 1]);
                }
                else if (syn >= 23 && syn <= 31)  //����
                {
                    cout << "<" << type[syn - 23] << ",type_w>" << endl;
                    fprintf(fp1, "<type_w,%s>\n", type[syn - 23]);
                }
                else if (syn >= 32 && syn <= 53) //�����
                {
                    cout << "<" << operatorer[syn - 32] << ",oper>" << endl;
                    fprintf(fp1, "<oper,%s>\n", operatorer[syn - 32]);
                }
                else if (syn >= 54 && syn <= 59) //���
                {
                    cout << "<" << limiter[syn - 54] << ",limit>" << endl;
                    fprintf(fp1, "<limit,%s>\n", limiter[syn - 54]);
                }
                else if (syn >= 60 && syn <= 67) //�߼������
                {
                    cout << "<" << logicer[syn - 60] << ",logic>" << endl;
                    fprintf(fp1, "<logic,%s>\n", logicer[syn - 60]);
                }
                else if (syn == 68)		 //����
                {
                    cout << "<" << token << ",NUM>" << endl;
                    fprintf(fp1, "<NUM,%s>\n", token);
                }
                else if (syn == -2)
                {
                    cout << "Error��������������� " << "\n";
                    fprintf(fp1, "Error���������������\n");
                     QMessageBox::information(this, "��ʾ", "�������������");break;
                }

           }

                fclose(fp1);
            cout << endl;
            code_layer[layer_count - 1] = 0;
            code_layer[layer_count - 1] = 0;
            word_suc=1;
//�ʷ��������ļ�

        p = 0;
        FILE *fp2;
        if ((fp2 = fopen("D:\\result1.txt", "r")) == NULL)
        {
            cout << "can't open this file";
             QMessageBox::information(this, "��ʾ", "���ļ�ʧ��");break;
        }
        Project[p] = fgetc(fp2);
        while (Project[p] != EOF)
        {
            p++;
            Project[p] = fgetc(fp2);
        }
        Project[++p] = '\0';
        fclose(fp2);
        cout << endl << "���ļ����Ϊ:" << endl;
        cout << Project << endl;
        ui->textEdit_2->setText(Project);
        ui->stackedWidget->setCurrentIndex(1);
        break;
    }
    case 3://�﷨�������
    {
         if(!(read_suc&&word_suc))
         {
             QMessageBox::information(this, "ERROR", "you can not do this now!");break;}

         if(gram_suc){ui->stackedWidget->setCurrentIndex(2);break;}
        //��ӡ��Ԫ��
        fp_g = fopen("D:\\result2.txt", "w+");

        if (fp_g == NULL)
        {
            cout << "can't open this file";
             QMessageBox::information(this, "��ʾ", "���ļ�ʧ��");break;
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
    //�﷨����������ļ�

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
                cout << endl << "���ļ����Ϊ:" << endl;
                cout << Project << endl;
                ui->textEdit_3->setText(Project);
                ui->stackedWidget->setCurrentIndex(2);
                break;
    }
    case 4://���ͳ���
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
        cout << endl << "������£�" << endl;
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
        //���ͳ�����ļ�

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
                cout << endl << "���ļ����Ϊ:" << endl;
                cout << Project << endl;
                ui->textEdit_4->setText(Project);
                ui->stackedWidget->setCurrentIndex(3);
                break;
    }
}
}

