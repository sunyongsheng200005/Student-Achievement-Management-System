/*
	文件名称：学生成绩管理系统.cpp
	文件描述：实现链表学生成绩管理系统
	文件作者：
				孙永生
				王月
				王海阔
    *******************版权所有，盗版必究 *******************
*/ 


 
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <conio.h>
//定义学生的结构体
struct data 
{
	double	num;			//学号 
	char	name[500];		//姓名 
	char 	classes[500];	//班级 
	float 	math;			//高数成绩 
	float	english;		//英语成绩 
	float	datastr;		//数据结构成绩 
	float 	toal;			//总分 
	float	average;		//平均分 
	
};

typedef struct stu
{
	struct data data;
	struct stu *pnext;
}STU;

STU *pstu = NULL;			//链表的头节点 
FILE *fp = NULL;			//文件指针 

void CreateList();			//创建链表 


void openfile();			//检查创建 成绩单.txt文件 ，将文件内数据写入链表 
void SaveData();			//保存数据到 成绩单.txt文件 

void Remind(); 				//温馨提示界面 
void  menu();				//菜单界面 
void PrintfAll();			//输出函数 
void PrintfAlign(STU *&ptemp);			//输出对齐的数据表格 

/********** 查找函数 **********/ 
void SearchMain();				//查找主函数 
void SearchNum();				//按学号查找 
void SearchName(STU *ptemp);	//按姓名查找 
void SearchClass(STU *ptemp);	//按班级查找 
void SearchMath();				//按高数成绩查找 
void SearchEnglish();			//按英语成绩查找 
void SearchDatastr();			//按数据结构成绩查找 
void SearchAverage();			//按平均成绩查找 
void SearchToal();				//按总成绩查找 


void AddData();				//添加学生信息 
void DeleteData();			//删除学生信息 
void ModifyData(); 			//修改学生信息 

/********** 排序函数 **********/ 
void SortStu();				//排序主函数 
int ListLength();  			//计算表长度 
void SortName();			//按姓名排序 
void SortNum();				//按学号排序 
void SortClasses();			//按班级排序 
void SortMath();			//按高等数学成绩排序 
void SortEngilsh();			//按英语成绩排序 
void Sortdatastr();			//按数据结构成绩排序 
void SortAve();				//按平均分排序 
void SortToal();			//按总分排序 



/******************************************************************************************************************************************/ 
void CreateList() 				//建立链表 
{
	pstu = (STU*)malloc(sizeof(STU));
	pstu->pnext = NULL;
	
}
void openfile()   				//打开成绩单.txt文件   若有 则将数据导入链表;若没有  则创建 成绩单.txt文件 
{
	printf("\t\t\t\t\t\t ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n"); 
	printf("\t\t\t\t\t\t ┃                                              ┃\n");
	printf("\t\t\t\t\t\t ┃           欢迎使用学生成绩管理系统           ┃\n");
	printf("\t\t\t\t\t\t ┃                                              ┃\n");
	printf("\t\t\t\t\t\t ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n"); 
	STU *pnew;
	STU *ptemp;
	CreateList();
	ptemp = pstu;
	if((fp = fopen("成绩单.txt","r")) == NULL)   //用只读的方式检测文件是否存在 
	{
		fp = fopen("成绩单.txt","wb"); 
		printf("\t\t\t\t\t\t这是您第一次打开该系统，已为您创建“成绩单.txt”文件!\n");
		printf("\t\t\t\t\t\t按任意键继续...");
		getch();
	}
	while(1)
	{
		pnew = (STU*)malloc(sizeof(STU));
		
		if(fscanf(fp,"%lf\t%s\t%s\t%f\t%f\t%f\t%f\t%f\n",&pnew->data.num,&pnew->data.name,&pnew->data.classes,&pnew->data.math,&pnew->data.english,&pnew->data.datastr,&pnew->data.average,&pnew->data.toal) == EOF)  //当读取到文件结尾，释放掉pnew并返回 
		{
			free(pnew); 
			break;
		}
		
		pnew->pnext = NULL;
		ptemp->pnext = pnew;
		ptemp = ptemp->pnext; //指向下一个节点 
	}
	
	fclose(fp);
}
void SearchNum()  				//按学号查找 
{
	STU *ptemp;
	double num;
	system("cls");
	printf("\t\t\t\t\t\t ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n"); 
	printf("\t\t\t\t\t\t ┃                                              ┃\n");
	printf("\t\t\t\t\t\t ┃            正在按学号查找学生信息            ┃\n");
	printf("\t\t\t\t\t\t ┃                                              ┃\n");
	printf("\t\t\t\t\t\t ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n");
	printf("\t\t\t\t\t\t 请输入您要查找的学号：");
	scanf("%lf",&num);
	ptemp = pstu->pnext;
	while(ptemp!=NULL)
	{
		if(ptemp->data.num == num)
		{
			printf("\n\n");
			printf("\t\t\t学号\t\t姓名\t班级\t\t\t\t高数成绩\t英语成绩\t数据结构成绩\t平均分\t总分\n");
			PrintfAlign(*&ptemp); 
			printf("\t\t\t按任意键继续...\n");
			getch();
			return;
		}
		ptemp = ptemp->pnext;
	}
	printf("\t\t\t查无此人！按回车键继续...");
	getch();
	return;
	
}
 void SearchName(STU *ptemp)	//按姓名查找 
{
	char name[500];
	system("cls");
	printf("\t\t\t\t\t\t ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n"); 
	printf("\t\t\t\t\t\t ┃                                              ┃\n");
	printf("\t\t\t\t\t\t ┃            正在按姓名查找学生信息            ┃\n");
	printf("\t\t\t\t\t\t ┃                                              ┃\n");
	printf("\t\t\t\t\t\t ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n");
	printf("\t\t\t\t\t\t 请输入您要查找的姓名：");
	scanf("%s",name);
	ptemp = ptemp->pnext;
	int n = 0;
	while(ptemp !=NULL)
	{
		if(!strcmp(ptemp->data.name,name))//当输入的姓名与链表中stemp的姓名相同时 
		{
			if(n == 0)//只在第一次找到该姓名时输出选项提示 
			{
				printf("\n\n");
				printf("\t\t\t学号\t\t姓名\t班级\t\t\t\t高数成绩\t英语成绩\t数据结构成绩\t平均分\t总分\n");
			}
			PrintfAlign(*&ptemp); 
			n++;
		}
		ptemp = ptemp->pnext;
	}
	if(ptemp == NULL)//当查找完链表中所有节点后 
	{
		if(n == 0)//如果一次姓名都没有找到 
		{
			printf("\t\t\t查无此人！按回车键继续...");
		}
		else
		{
			printf("\t\t\t按任意键继续。。。"); 
		}
	 } 
	getch();
	return;

}
void SearchClass(STU *ptemp)	//按班级查找 
{
	char clas[500];
	system("cls");
	printf("\t\t\t\t\t\t ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n"); 
	printf("\t\t\t\t\t\t ┃                                              ┃\n");
	printf("\t\t\t\t\t\t ┃            正在按班级查找学生信息            ┃\n");
	printf("\t\t\t\t\t\t ┃                                              ┃\n");
	printf("\t\t\t\t\t\t ┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫\n"); 
	printf("\t\t\t\t\t\t ┃示例:                                         ┃\n");
	printf("\t\t\t\t\t\t ┃     2019级软件工程3班                        ┃\n");
	printf("\t\t\t\t\t\t ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n"); 
	printf("\t\t\t\t\t\t 请输入您要查找的班级：");
	scanf("%s",clas);
	ptemp = ptemp->pnext;
	int n = 0;
	while(ptemp !=NULL)
	{
		if(!strcmp(ptemp->data.classes,clas))
		{
			if(n == 0)
			{
				printf("\n\n");
				printf("\t\t\t学号\t\t姓名\t班级\t\t\t\t高数成绩\t英语成绩\t数据结构成绩\t平均分\t总分\n");
			}
			PrintfAlign(*&ptemp); 
			n++;
		}
		ptemp = ptemp->pnext;
	}
	if(ptemp == NULL)
	{
		if(n == 0)
		{
			printf("\t\t\t查无此人！按回车键继续...");
		}
		else
		{
			printf("\t\t\t按任意键继续。。。"); 
		}
	 } 
	getch();
	return;

}
void SearchMath()				//按高数成绩查找 
{
	STU *ptemp;
	double math;
	int n = 0; 
	system("cls");
	printf("\t\t\t\t\t\t ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n"); 
	printf("\t\t\t\t\t\t ┃                                              ┃\n");
	printf("\t\t\t\t\t\t ┃          正在按高数成绩查找学生信息          ┃\n");
	printf("\t\t\t\t\t\t ┃                                              ┃\n");
	printf("\t\t\t\t\t\t ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n");
	printf("\t\t\t\t\t\t 请输入您要查找的分数：");
	scanf("%lf",&math);
	ptemp = pstu->pnext;
	while(ptemp!=NULL)
	{
		if(ptemp->data.math == math)
		{
			if(n == 0)
			{
				printf("\n\n");
				printf("\t\t\t学号\t\t姓名\t班级\t\t\t\t高数成绩\t英语成绩\t数据结构成绩\t平均分\t总分\n");
			}
			PrintfAlign(*&ptemp); 
			n++;
		}
		ptemp = ptemp->pnext;
	}
	if(ptemp == NULL)
	{
		if(n == 0)
		{
			printf("\t\t\t查无此人！按回车键继续...");
		}
		else
		{
			printf("\t\t\t按任意键继续。。。"); 
		}
	 } 
	getch();
	return;
	
}
void SearchEnglish()			//按英语成绩查找 
{
	STU *ptemp;
	double english;
	int n = 0; 
	system("cls");
	printf("\t\t\t\t\t\t ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n"); 
	printf("\t\t\t\t\t\t ┃                                              ┃\n");
	printf("\t\t\t\t\t\t ┃          正在按英语成绩查找学生信息          ┃\n");
	printf("\t\t\t\t\t\t ┃                                              ┃\n");
	printf("\t\t\t\t\t\t ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n");
	printf("\t\t\t\t\t\t 请输入您要查找的分数：");
	scanf("%lf",&english);
	ptemp = pstu->pnext;
	while(ptemp!=NULL)
	{
		if(ptemp->data.english == english)
		{
		if(n == 0)
		{
			printf("\n\n");
			printf("\t\t\t学号\t\t姓名\t班级\t\t\t\t高数成绩\t英语成绩\t数据结构成绩\t平均分\t总分\n");
		}
		PrintfAlign(*&ptemp); 
		n++;
		}
			ptemp = ptemp->pnext;
		}
		if(ptemp == NULL)
		{
			if(n == 0)
			{
				printf("\t\t\t查无此人！按回车键继续...");
			}
			else
			{
				printf("\t\t\t按任意键继续。。。"); 
			}
		 } 
		getch();
		return;
	
}
void SearchDatastr()			//按数据结构成绩查找                                  
{
	STU *ptemp;
	double data;
	int n = 0; 
	system("cls");
	printf("\t\t\t\t\t\t ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n"); 
	printf("\t\t\t\t\t\t ┃                                              ┃\n");
	printf("\t\t\t\t\t\t ┃        正在按数据结构成绩查找学生信息        ┃\n");
	printf("\t\t\t\t\t\t ┃                                              ┃\n");
	printf("\t\t\t\t\t\t ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n");
	printf("\t\t\t\t\t\t 请输入您要查找的分数：");
	scanf("%lf",&data);
	ptemp = pstu->pnext;
	while(ptemp!=NULL)
	{
		if(ptemp->data.datastr == data)
		{
		if(n == 0)
			{
				printf("\n\n");
				printf("\t\t\t学号\t\t姓名\t班级\t\t\t\t高数成绩\t英语成绩\t数据结构成绩\t平均分\t总分\n");
			}
			PrintfAlign(*&ptemp); 
			n++;
			}
			ptemp = ptemp->pnext;
		}
		if(ptemp == NULL)
		{
			if(n == 0)
			{
				printf("\t\t\t查无此人！按回车键继续...");
			}
			else
			{
				printf("\t\t\t按任意键继续。。。"); 
			}
		 } 
		getch();
		return;
	
	
}
void SearchAverage()			//按平均成绩查找 
{
	STU *ptemp;
	double ave;
	int n = 0;
	system("cls");
	printf("\t\t\t\t\t\t ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n"); 
	printf("\t\t\t\t\t\t ┃                                              ┃\n");
	printf("\t\t\t\t\t\t ┃          正在按平均成绩查找学生信息          ┃\n");
	printf("\t\t\t\t\t\t ┃                                              ┃\n");
	printf("\t\t\t\t\t\t ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n");
	printf("\t\t\t\t\t\t 请输入您要查找的分数：");
	scanf("%lf",&ave);
	ptemp = pstu->pnext;
	while(ptemp!=NULL)
	{
		if(ptemp->data.average == ave)
		{
			if(n == 0)
			{
				printf("\n\n");
				printf("\t\t\t学号\t\t姓名\t班级\t\t\t\t高数成绩\t英语成绩\t数据结构成绩\t平均分\t总分\n");
			}
			PrintfAlign(*&ptemp); 
			n++;
			}
			ptemp = ptemp->pnext;
		}
		if(ptemp == NULL)
		{
			if(n == 0)
			{
				printf("\t\t\t查无此人！按回车键继续...");
			}
			else
			{
				printf("\t\t\t按任意键继续。。。"); 
			}
		 } 
		getch();
		return;
	
	
}
void SearchToal()				//按总成绩查找 
{
	STU *ptemp;
	double toal;
	int n = 0; 
	system("cls");
	printf("\t\t\t\t\t\t ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n"); 
	printf("\t\t\t\t\t\t ┃                                              ┃\n");
	printf("\t\t\t\t\t\t ┃           正在按总成绩查找学生信息           ┃\n");
	printf("\t\t\t\t\t\t ┃                                              ┃\n");
	printf("\t\t\t\t\t\t ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n");
	printf("\t\t\t\t\t\t 请输入您要查找的分数：");
	scanf("%lf",&toal);
	ptemp = pstu->pnext;
	while(ptemp!=NULL)
	{
		if(ptemp->data.toal == toal)
		{
			if(n == 0)
			{
				printf("\n\n");
				printf("\t\t\t学号\t\t姓名\t班级\t\t\t\t高数成绩\t英语成绩\t数据结构成绩\t平均分\t总分\n");
			}
			PrintfAlign(*&ptemp); 
			n++;
			}
			ptemp = ptemp->pnext;
		}
		if(ptemp == NULL)
		{
			if(n == 0)
			{
				printf("\t\t\t查无此人！按回车键继续...");
			}
			else
			{
				printf("\t\t\t按任意键继续。。。"); 
			}
		 } 
		getch();
		return;
	
	
}
void SearchMain()				//查找主函数 
{
	int select;
	do
	{
 	system("cls");
	printf("\t\t\t\t\t\t ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n"); 
	printf("\t\t\t\t\t\t ┃                                              ┃\n");
	printf("\t\t\t\t\t\t ┃              正在查找学生信息                ┃\n");
	printf("\t\t\t\t\t\t ┃                                              ┃\n");
	printf("\t\t\t\t\t\t ┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫\n"); 
	printf("\t\t\t\t\t\t ┃           1.查看所有学生的信息               ┃\n");
	printf("\t\t\t\t\t\t ┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫\n"); 
	printf("\t\t\t\t\t\t ┃           2.按学号查询                       ┃\n");
	printf("\t\t\t\t\t\t ┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫\n"); 
	printf("\t\t\t\t\t\t ┃           3.按姓名查询                       ┃\n");
	printf("\t\t\t\t\t\t ┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫\n"); 
	printf("\t\t\t\t\t\t ┃           4.按班级查询                       ┃\n");
	printf("\t\t\t\t\t\t ┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫\n"); 
	printf("\t\t\t\t\t\t ┃           5.按数学成绩查询                   ┃\n");
	printf("\t\t\t\t\t\t ┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫\n"); 
	printf("\t\t\t\t\t\t ┃           6.按英语成绩查询                   ┃\n");
	printf("\t\t\t\t\t\t ┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫\n"); 
	printf("\t\t\t\t\t\t ┃           7.按数据结构成绩查询               ┃\n");
	printf("\t\t\t\t\t\t ┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫\n"); 
	printf("\t\t\t\t\t\t ┃           8.按平均分查询                     ┃\n");
	printf("\t\t\t\t\t\t ┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫\n"); 
	printf("\t\t\t\t\t\t ┃           9.按总成绩查询                     ┃\n");
	printf("\t\t\t\t\t\t ┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫\n"); 
	printf("\t\t\t\t\t\t ┃           0.返回主界面                       ┃\n");
	printf("\t\t\t\t\t\t ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n");
	printf("\t\t\t\t\t\t请选择：");
	scanf("%d",&select);
	switch(select)
	{
		case 1:
			PrintfAll();
			break;
			
		case 2:
			SearchNum();
			break;
		case 3:
			SearchName(pstu);
			break;
		case 4:
			SearchClass(pstu);
			break;
		case 5:
			SearchMath();
			break;
		case 6:
			SearchEnglish();
			break;
		case 7:
			SearchDatastr(); 
			break;
		case 8:
			SearchAverage();
			break;
		case 9:
			SearchToal();
			break;
		case 0:
			break;
		default:
			break;
	}
	}while(select != 0);
  
	
}
void AddData()					//添加学生信息 
{
	STU *ptemp;
	int i = 1; 
	system("cls");
	printf("\t\t\t\t\t\t ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n"); 
	printf("\t\t\t\t\t\t ┃                                              ┃\n");
	printf("\t\t\t\t\t\t ┃               正在插入学生信息               ┃\n");
	printf("\t\t\t\t\t\t ┃                                              ┃\n");
	printf("\t\t\t\t\t\t ┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫\n"); 
	printf("\t\t\t\t\t\t ┃  输入格式示例：                              ┃\n");
	printf("\t\t\t\t\t\t ┃20191308097 孙永生 2019级软件工程3班 80 70 90 ┃\n");
	printf("\t\t\t\t\t\t ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n");
	STU *pnew = NULL;
	pnew = (STU*)malloc(sizeof(STU));
	printf("\t\t\t正在插入学员信息：\n");
	printf("\t\t\t请输入学号：");
	scanf("%lf",&pnew->data.num);
	ptemp = pstu->pnext;
	while(ptemp!=NULL)
	{
		if(pnew->data.num == ptemp->data.num)
		{
			printf("\t\t\t学号重复,请重新输入：");
			scanf("%lf",&pnew->data.num);
			continue;
		}
		else
			ptemp = ptemp->pnext; 
	}
	printf("\t\t\t请输入姓名：");
	scanf("%s",&pnew->data.name);
	printf("\t\t\t请输入班级：");
	scanf("%s",&pnew->data.classes);
	printf("\t\t\t请输入高数成绩：");
	scanf("%f",&pnew->data.math);
	printf("\t\t\t请输入英语成绩：");
	scanf("%f",&pnew->data.english);
	printf("\t\t\t请输入数据结构成绩：");
	scanf("%f",&pnew->data.datastr);
	pnew->data.toal = (pnew->data.math + pnew->data.english + pnew->data.datastr);
	pnew->data.average = (pnew->data.toal)/3;
	if(pstu->pnext == NULL)     //链表为空 
	{
		pnew->pnext = NULL;
		pstu->pnext = pnew;
	} 
	else //链表不为空 
	{
		pnew->pnext = pstu->pnext;
		pstu->pnext = pnew;
	}
	printf("\t\t\t是否继续添加学生信息：(1/0)");
	scanf("%d",&i);
	if(i == 1)
	{
		AddData(); //继续添加，则再次调用该函数 
	} 
	else
		SortNum();	//结束时按学号排序 
		return;

}
void DeleteData()				//删除学生信息 
{
	STU *ptemp;
	STU *pnew;
	double num;
	int i;
	system("cls");
	printf("\t\t\t\t\t\t ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n"); 
	printf("\t\t\t\t\t\t ┃                                              ┃\n");
	printf("\t\t\t\t\t\t ┃               正在删除学生信息               ┃\n");
	printf("\t\t\t\t\t\t ┃                                              ┃\n");
	printf("\t\t\t\t\t\t ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n");
	printf("\t\t\t\t\t\t 请输入您要删除的学号：");
	scanf("%lf",&num);
	ptemp = pstu->pnext; //目标节点 
	pnew = pstu;  //目标节点的前驱节点 
	while(ptemp!=NULL)
	{
		if(ptemp->data.num == num)
		{
			printf("\n\n");
			printf("\t\t\t学号\t\t姓名\t班级\t\t\t\t高数成绩\t英语成绩\t数据结构成绩\t平均分\t总分\n");
			PrintfAlign(*&ptemp); 
			printf("\t\t\t已为您查找到该学生的信息！是否确定删除？(1/0)"); 
			scanf("%d",&i);
			if(i == 1)
			{
				pnew->pnext = ptemp->pnext;
				free(ptemp);
				printf("\t\t\t删除成功！按任意键继续！");
				getch();
				return; 
			}
			else
				return;
		}
		ptemp = ptemp->pnext;
		pnew = pnew->pnext;
	}
	printf("\t\t\t查无此人！按回车键重新输入...");
	getch(); 
	DeleteData(); 
}
void ModifyData()				//修改学生信息 
{
	STU *ptemp;
	double num;
	int i;
	system("cls");
	printf("\t\t\t\t\t\t ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n"); 
	printf("\t\t\t\t\t\t ┃                                              ┃\n");
	printf("\t\t\t\t\t\t ┃               正在修改学生信息               ┃\n");
	printf("\t\t\t\t\t\t ┃                                              ┃\n");
	printf("\t\t\t\t\t\t ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n");
	printf("\t\t\t\t\t\t 请输入您要修改的学号：");
	scanf("%lf",&num);
	ptemp = pstu->pnext;
	while(ptemp!=NULL)
	{
		if(ptemp->data.num == num)
		{
			printf("\n\n");
			printf("\t\t\t学号\t\t姓名\t班级\t\t\t\t高数成绩\t英语成绩\t数据结构成绩\t平均分\t总分\n");
			PrintfAlign(*&ptemp); 
			printf("\t\t\t已为您查找到该学生的信息！是否确定修改？(1/0)"); 
			scanf("%d",&i);
			if(i == 1)
			{
				printf("\t\t\t请重新输入该学生的信息！\n");
				printf("\t\t\t请输入姓名：");
				scanf("%s",&ptemp->data.name);
				printf("\t\t\t请输入班级：");
				scanf("%s",&ptemp->data.classes);
				printf("\t\t\t请输入高数成绩：");
				scanf("%f",&ptemp->data.math);
				printf("\t\t\t请输入英语成绩：");
				scanf("%f",&ptemp->data.english);
				printf("\t\t\t请输入数据结构成绩：");
				scanf("%f",&ptemp->data.datastr);
				ptemp->data.toal = (ptemp->data.math + ptemp->data.english + ptemp->data.datastr);
				ptemp->data.average = (ptemp->data.toal)/3;
				printf("\n\n");
				printf("\t\t\t学号\t\t姓名\t班级\t\t\t高数成绩\t英语成绩\t数据结构成绩\t平均分\t总分\n");
				PrintfAlign(*&ptemp);
				printf("\t\t\t修改成功！按任意键继续！");
				getch();
				return; 
			}
			else
				return;
		}
		ptemp = ptemp->pnext;
	}
	printf("\t\t\t查无此人！按回车键重新输入...");
	getch();
	ModifyData();
}
void SortStu()					//排序主函数 
{
	int select;
	do
	{
	system("cls");
	printf("\t\t\t\t\t\t ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n"); 
	printf("\t\t\t\t\t\t ┃                                              ┃\n");
	printf("\t\t\t\t\t\t ┃               请选择排序方式                 ┃\n");
	printf("\t\t\t\t\t\t ┃                                              ┃\n");
	printf("\t\t\t\t\t\t ┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫\n"); 
	printf("\t\t\t\t\t\t ┃           1.按姓名排序                       ┃\n");
	printf("\t\t\t\t\t\t ┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫\n"); 
	printf("\t\t\t\t\t\t ┃           2.按学号排序                       ┃\n");
	printf("\t\t\t\t\t\t ┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫\n"); 
	printf("\t\t\t\t\t\t ┃           3.按班级排序                       ┃\n");
	printf("\t\t\t\t\t\t ┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫\n"); 
	printf("\t\t\t\t\t\t ┃           4.按高等数学成绩排序               ┃\n");
	printf("\t\t\t\t\t\t ┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫\n"); 
	printf("\t\t\t\t\t\t ┃           5.按英语成绩排序                   ┃\n");
	printf("\t\t\t\t\t\t ┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫\n"); 
	printf("\t\t\t\t\t\t ┃           6.按数据结构成绩排序               ┃\n");
	printf("\t\t\t\t\t\t ┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫\n"); 
	printf("\t\t\t\t\t\t ┃           7.按平均分排序                     ┃\n");
	printf("\t\t\t\t\t\t ┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫\n"); 
	printf("\t\t\t\t\t\t ┃           8.按总分排序                       ┃\n");
	printf("\t\t\t\t\t\t ┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫\n"); 
	printf("\t\t\t\t\t\t ┃           0.返回主界面                       ┃\n");
	printf("\t\t\t\t\t\t ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n"); 
	printf("\t\t\t\t\t\t请选择：");
	scanf("%d",&select);
	switch(select)
	{
		case 1:
			SortName();
			break;
			
		case 2:
			SortNum(); 
			break;
		case 3:
			SortClasses();
			break;
		case 4:
			SortMath();
			break;
		case 5:
			SortEngilsh();
			break;
		case 6:
			Sortdatastr();
			break;
		case 7:
			SortAve();
			break;
		case 8:
			SortToal();
			break;
		case 0:
			break;
		default:
			break;
	}
	}while(select != 0);
  
	
} 
void SortName()					//按姓名排序 
{
	system("cls");
	printf("\t\t\t\t\t\t ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n"); 
	printf("\t\t\t\t\t\t ┃                                              ┃\n");
	printf("\t\t\t\t\t\t ┃                正在按姓名排序                ┃\n");
	printf("\t\t\t\t\t\t ┃                                              ┃\n");
	printf("\t\t\t\t\t\t ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n");
	struct data t;  //用来暂时存放数据   
	STU *ptemp;
	STU *ptemp1;
	
	for(ptemp = pstu->pnext;ptemp != NULL;ptemp = ptemp->pnext)  
		for(ptemp1 = ptemp->pnext;ptemp1 != NULL;ptemp1 = ptemp1->pnext)
		{
			if(strcmp(ptemp1->data.name,ptemp->data.name)<0) //如果前面的节点大于后面的节点则交换数据 
			{
				t=ptemp->data;
				ptemp->data = ptemp1->data;
				ptemp1->data = t;
			}
		}
	PrintfAll();
} 
void SortNum()					//按学号排序 
{
	system("cls");
	printf("\t\t\t\t\t\t ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n"); 
	printf("\t\t\t\t\t\t ┃                                              ┃\n");
	printf("\t\t\t\t\t\t ┃                正在按学号排序                ┃\n");
	printf("\t\t\t\t\t\t ┃                                              ┃\n");
	printf("\t\t\t\t\t\t ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n");
	struct data t;
	STU *ptemp;
	STU *ptemp1;
	
	for(ptemp = pstu->pnext;ptemp != NULL;ptemp = ptemp->pnext) 
		for(ptemp1 = ptemp->pnext;ptemp1 != NULL;ptemp1 = ptemp1->pnext)
		{
			if(ptemp1->data.num < ptemp->data.num)
			{
				t=ptemp->data;
				ptemp->data = ptemp1->data;
				ptemp1->data = t;
			}
		}
	PrintfAll();
} 
void SortClasses()				//按班级排序 
{
	system("cls");
	printf("\t\t\t\t\t\t ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n"); 
	printf("\t\t\t\t\t\t ┃                                              ┃\n");
	printf("\t\t\t\t\t\t ┃                正在按班级排序                ┃\n");
	printf("\t\t\t\t\t\t ┃                                              ┃\n");
	printf("\t\t\t\t\t\t ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n");
	struct data t;
	STU *ptemp;
	STU *ptemp1;
	
	for(ptemp = pstu->pnext;ptemp != NULL;ptemp = ptemp->pnext) 
		for(ptemp1 = ptemp->pnext;ptemp1 != NULL;ptemp1 = ptemp1->pnext)
		{
			if(strcmp(ptemp1->data.classes,ptemp->data.classes)<0)
			{
				t=ptemp->data;
				ptemp->data = ptemp1->data;
				ptemp1->data = t;
			}
		}
	PrintfAll();
}
void SortMath()					//按高数排序 
{
	system("cls");
	printf("\t\t\t\t\t\t ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n"); 
	printf("\t\t\t\t\t\t ┃                                              ┃\n");
	printf("\t\t\t\t\t\t ┃              正在按高数成绩排序              ┃\n");
	printf("\t\t\t\t\t\t ┃                                              ┃\n");
	printf("\t\t\t\t\t\t ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n");
	struct data t;
	STU *ptemp;
	STU *ptemp1;
	
	for(ptemp = pstu->pnext;ptemp != NULL;ptemp = ptemp->pnext) 
		for(ptemp1 = ptemp->pnext;ptemp1 != NULL;ptemp1 = ptemp1->pnext)
		{
			if(ptemp1->data.math > ptemp->data.math)
			{
				t=ptemp->data;
				ptemp->data = ptemp1->data;
				ptemp1->data = t;
			}
		} 
	PrintfAll();
}
void SortEngilsh()				//按英语排序 
{
	system("cls");
	printf("\t\t\t\t\t\t ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n"); 
	printf("\t\t\t\t\t\t ┃                                              ┃\n");
	printf("\t\t\t\t\t\t ┃              正在按英语成绩排序              ┃\n");
	printf("\t\t\t\t\t\t ┃                                              ┃\n");
	printf("\t\t\t\t\t\t ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n");
	struct data t;
	STU *ptemp;
	STU *ptemp1;
	
	for(ptemp = pstu->pnext;ptemp != NULL;ptemp = ptemp->pnext) 
		for(ptemp1 = ptemp->pnext;ptemp1 != NULL;ptemp1 = ptemp1->pnext)
		{
			if(ptemp1->data.english > ptemp->data.english)
			{
				t=ptemp->data;
				ptemp->data = ptemp1->data;
				ptemp1->data = t;
			}
		}
	PrintfAll();
}
void Sortdatastr()				//按数据结构排序 
{
	system("cls");
	printf("\t\t\t\t\t\t ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n"); 
	printf("\t\t\t\t\t\t ┃                                              ┃\n");
	printf("\t\t\t\t\t\t ┃            正在按数据结构成绩排序            ┃\n");
	printf("\t\t\t\t\t\t ┃                                              ┃\n");
	printf("\t\t\t\t\t\t ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n");
	struct data t;
	STU *ptemp;
	STU *ptemp1;
	
	for(ptemp = pstu->pnext;ptemp != NULL;ptemp = ptemp->pnext) 
		for(ptemp1 = ptemp->pnext;ptemp1 != NULL;ptemp1 = ptemp1->pnext)
		{
			if(ptemp1->data.datastr > ptemp->data.datastr)
			{
				t=ptemp->data;
				ptemp->data = ptemp1->data;
				ptemp1->data = t;
			}
		}
	PrintfAll();
}
void SortAve()					//按平均分排序 
{
	system("cls");
	printf("\t\t\t\t\t\t ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n"); 
	printf("\t\t\t\t\t\t ┃                                              ┃\n");
	printf("\t\t\t\t\t\t ┃               正在按平均分排序               ┃\n");
	printf("\t\t\t\t\t\t ┃                                              ┃\n");
	printf("\t\t\t\t\t\t ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n");
	struct data t;
	STU *ptemp;
	STU *ptemp1;
	
	for(ptemp = pstu->pnext;ptemp != NULL;ptemp = ptemp->pnext) 
		for(ptemp1 = ptemp->pnext;ptemp1 != NULL;ptemp1 = ptemp1->pnext)
		{
			if(ptemp1->data.average > ptemp->data.average)
			{
				t=ptemp->data;
				ptemp->data = ptemp1->data;
				ptemp1->data = t;
			}
		}
	PrintfAll();
}
void SortToal()					//按总分排序 
{
	system("cls");
	printf("\t\t\t\t\t\t ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n"); 
	printf("\t\t\t\t\t\t ┃                                              ┃\n");
	printf("\t\t\t\t\t\t ┃                正在按总分排序                ┃\n");
	printf("\t\t\t\t\t\t ┃                                              ┃\n");
	printf("\t\t\t\t\t\t ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n");
	struct data t;
	STU *ptemp;
	STU *ptemp1;
	
	for(ptemp = pstu->pnext;ptemp != NULL;ptemp = ptemp->pnext) 
	{
		for(ptemp1 = ptemp->pnext;ptemp1 != NULL;ptemp1 = ptemp1->pnext)
		{
			if(ptemp1->data.toal > ptemp->data.toal)
			{
				t=ptemp->data;
				ptemp->data = ptemp1->data;
				ptemp1->data = t;
			}
		}
	}
	PrintfAll();
}
void SaveData()					//保存文件 
{
	STU *ptemp = pstu->pnext;
	
	if ((fp = fopen("成绩单.txt","w")) == NULL)
	{
		printf("\t\t\t文件打开失败！"); 
	}
	else
	{
		while(ptemp!=NULL)
		{
			fprintf(fp,"%.lf\t%s\t%s\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\n",ptemp->data.num,ptemp->data.name,ptemp->data.classes,ptemp->data.math,ptemp->data.english,ptemp->data.datastr,ptemp->data.average,ptemp->data.toal);
			ptemp = ptemp->pnext;
		}
	}
	fclose(fp);
} 
void PrintfAll()				//输出所有数据信息 
{
	system("cls");
	printf("\t\t\t\t\t\t ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n"); 
	printf("\t\t\t\t\t\t ┃                                              ┃\n");
	printf("\t\t\t\t\t\t ┃              正在查看学生信息                ┃\n");
	printf("\t\t\t\t\t\t ┃                                              ┃\n");
	printf("\t\t\t\t\t\t ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n"); 
	int i;
	STU *ptemp = pstu->pnext;
	if(ptemp == NULL)
	{
		printf("\t\t\t没有数据！请选择是否录入数据:(1/0)");
		scanf("%d",&i);
		if(i == 1)
		{
			AddData();
			return;
		}
		else
		{
			printf("\t\t\t按任意键继续...\n");
			getch();
			return; 
		} 
	}
	printf("\t\t\t学号\t\t姓名\t班级\t\t\t\t高数成绩\t英语成绩\t数据结构成绩\t平均分\t总分\n");
	while(ptemp!= NULL)
	{
		PrintfAlign(*&ptemp); 
		ptemp = ptemp->pnext;
	}
		printf("\t\t\t按任意键继续...\n");
		getch();
}
void PrintfAlign(STU *&ptemp)   //对齐输出 
{
	int i;
	if(strlen(ptemp->data.classes)==25)//25为计算机学院专业中字符最长的专业的字符长度 
	{
			printf("\t\t\t%.lf\t%s\t%s\t%.2f\t\t%.2f\t\t%.2f\t\t%.2f\t%.2f\n",ptemp->data.num,ptemp->data.name,ptemp->data.classes,ptemp->data.math,ptemp->data.english,ptemp->data.datastr,ptemp->data.average,ptemp->data.toal);
	}
	else if(strlen(ptemp->data.classes)<25)//如果该专业的字符长度比25短 
	{
		i = 25-strlen(ptemp->data.classes);//该专业的字符长度比25小多少就输出多少空格   以保证在输出时各项对齐 
		printf("\t\t\t%.lf\t%s\t%s",ptemp->data.num,ptemp->data.name,ptemp->data.classes);
		for(i;i>0;i--)
		{
			printf(" ");
		}
		printf("\t%.2f\t\t%.2f\t\t%.2f\t\t%.2f\t%.2f\n",ptemp->data.math,ptemp->data.english,ptemp->data.datastr,ptemp->data.average,ptemp->data.toal);
	}
}
void Remind()					//温馨提示界面 
{
	system("cls");
	printf("\t\t\t\t\t\t ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n"); 
	printf("\t\t\t\t\t\t ┃                   温馨提示                   ┃\n");
	printf("\t\t\t\t\t\t ┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫\n"); 
	printf("\t\t\t\t\t\t ┃       为避免不必要的麻烦，输入过程中请尽量按 ┃\n");
	printf("\t\t\t\t\t\t ┃ 照所给要求输入。                             ┃\n");
	printf("\t\t\t\t\t\t ┃          如给您带来不便，敬请谅解！          ┃\n"); 
	printf("\t\t\t\t\t\t ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n"); 
	printf("\t\t\t\t\t\t按任意键继续。。。") ; 
	getch();
}
void  menu()					//主菜单 
{
	system("cls");
	printf("\t\t\t\t\t\t ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n"); 
	printf("\t\t\t\t\t\t ┃                                              ┃\n");
	printf("\t\t\t\t\t\t ┃           欢迎使用学生成绩管理系统           ┃\n");
	printf("\t\t\t\t\t\t ┃                                              ┃\n");
	printf("\t\t\t\t\t\t ┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫\n"); 
	printf("\t\t\t\t\t\t ┃                   菜单栏                     ┃\n");
	printf("\t\t\t\t\t\t ┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫\n"); 
	printf("\t\t\t\t\t\t ┃           1.查看学生信息                     ┃\n");
	printf("\t\t\t\t\t\t ┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫\n"); 
	printf("\t\t\t\t\t\t ┃           2.插入学生信息                     ┃\n");
	printf("\t\t\t\t\t\t ┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫\n"); 
	printf("\t\t\t\t\t\t ┃           3.删除学生信息                     ┃\n");
	printf("\t\t\t\t\t\t ┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫\n"); 
	printf("\t\t\t\t\t\t ┃           4.修改学生信息                     ┃\n");
	printf("\t\t\t\t\t\t ┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫\n"); 
	printf("\t\t\t\t\t\t ┃           5.更改信息排序方式                 ┃\n");
	printf("\t\t\t\t\t\t ┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫\n"); 
	printf("\t\t\t\t\t\t ┃           0.退出系统                         ┃\n");
	printf("\t\t\t\t\t\t ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n"); 
}
/*****************主函数 *****************/ 
int main()
{
	openfile();
	Remind();
	int select;
	do
	{
		menu();
		printf("\t\t\t\t\t\t请选择：");
		scanf("%d",&select);
		switch(select)
		{
			case 1:
				SearchMain();
				break;
			case 2:
				AddData();
				break;
			case 3:
				DeleteData();
				break;
			case 4:
				ModifyData();
				break;
			case 5:
				SortStu(); 
				break;
			case 0:
				SaveData();
				break;
			default:
				break;
		}
	}while(select != 0);

	
	return 0;
}

