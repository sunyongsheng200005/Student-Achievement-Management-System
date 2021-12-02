/*
	�ļ����ƣ�ѧ���ɼ�����ϵͳ.cpp
	�ļ�������ʵ������ѧ���ɼ�����ϵͳ
	�ļ����ߣ�
				������
				����
				������
    *******************��Ȩ���У�����ؾ� *******************
*/ 


 
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <conio.h>
//����ѧ���Ľṹ��
struct data 
{
	double	num;			//ѧ�� 
	char	name[500];		//���� 
	char 	classes[500];	//�༶ 
	float 	math;			//�����ɼ� 
	float	english;		//Ӣ��ɼ� 
	float	datastr;		//���ݽṹ�ɼ� 
	float 	toal;			//�ܷ� 
	float	average;		//ƽ���� 
	
};

typedef struct stu
{
	struct data data;
	struct stu *pnext;
}STU;

STU *pstu = NULL;			//�����ͷ�ڵ� 
FILE *fp = NULL;			//�ļ�ָ�� 

void CreateList();			//�������� 


void openfile();			//��鴴�� �ɼ���.txt�ļ� �����ļ�������д������ 
void SaveData();			//�������ݵ� �ɼ���.txt�ļ� 

void Remind(); 				//��ܰ��ʾ���� 
void  menu();				//�˵����� 
void PrintfAll();			//������� 
void PrintfAlign(STU *&ptemp);			//�����������ݱ�� 

/********** ���Һ��� **********/ 
void SearchMain();				//���������� 
void SearchNum();				//��ѧ�Ų��� 
void SearchName(STU *ptemp);	//���������� 
void SearchClass(STU *ptemp);	//���༶���� 
void SearchMath();				//�������ɼ����� 
void SearchEnglish();			//��Ӣ��ɼ����� 
void SearchDatastr();			//�����ݽṹ�ɼ����� 
void SearchAverage();			//��ƽ���ɼ����� 
void SearchToal();				//���ܳɼ����� 


void AddData();				//���ѧ����Ϣ 
void DeleteData();			//ɾ��ѧ����Ϣ 
void ModifyData(); 			//�޸�ѧ����Ϣ 

/********** ������ **********/ 
void SortStu();				//���������� 
int ListLength();  			//������� 
void SortName();			//���������� 
void SortNum();				//��ѧ������ 
void SortClasses();			//���༶���� 
void SortMath();			//���ߵ���ѧ�ɼ����� 
void SortEngilsh();			//��Ӣ��ɼ����� 
void Sortdatastr();			//�����ݽṹ�ɼ����� 
void SortAve();				//��ƽ�������� 
void SortToal();			//���ܷ����� 



/******************************************************************************************************************************************/ 
void CreateList() 				//�������� 
{
	pstu = (STU*)malloc(sizeof(STU));
	pstu->pnext = NULL;
	
}
void openfile()   				//�򿪳ɼ���.txt�ļ�   ���� �����ݵ�������;��û��  �򴴽� �ɼ���.txt�ļ� 
{
	printf("\t\t\t\t\t\t ������������������������������������������������������������������������������������������������\n"); 
	printf("\t\t\t\t\t\t ��                                              ��\n");
	printf("\t\t\t\t\t\t ��           ��ӭʹ��ѧ���ɼ�����ϵͳ           ��\n");
	printf("\t\t\t\t\t\t ��                                              ��\n");
	printf("\t\t\t\t\t\t ������������������������������������������������������������������������������������������������\n"); 
	STU *pnew;
	STU *ptemp;
	CreateList();
	ptemp = pstu;
	if((fp = fopen("�ɼ���.txt","r")) == NULL)   //��ֻ���ķ�ʽ����ļ��Ƿ���� 
	{
		fp = fopen("�ɼ���.txt","wb"); 
		printf("\t\t\t\t\t\t��������һ�δ򿪸�ϵͳ����Ϊ���������ɼ���.txt���ļ�!\n");
		printf("\t\t\t\t\t\t�����������...");
		getch();
	}
	while(1)
	{
		pnew = (STU*)malloc(sizeof(STU));
		
		if(fscanf(fp,"%lf\t%s\t%s\t%f\t%f\t%f\t%f\t%f\n",&pnew->data.num,&pnew->data.name,&pnew->data.classes,&pnew->data.math,&pnew->data.english,&pnew->data.datastr,&pnew->data.average,&pnew->data.toal) == EOF)  //����ȡ���ļ���β���ͷŵ�pnew������ 
		{
			free(pnew); 
			break;
		}
		
		pnew->pnext = NULL;
		ptemp->pnext = pnew;
		ptemp = ptemp->pnext; //ָ����һ���ڵ� 
	}
	
	fclose(fp);
}
void SearchNum()  				//��ѧ�Ų��� 
{
	STU *ptemp;
	double num;
	system("cls");
	printf("\t\t\t\t\t\t ������������������������������������������������������������������������������������������������\n"); 
	printf("\t\t\t\t\t\t ��                                              ��\n");
	printf("\t\t\t\t\t\t ��            ���ڰ�ѧ�Ų���ѧ����Ϣ            ��\n");
	printf("\t\t\t\t\t\t ��                                              ��\n");
	printf("\t\t\t\t\t\t ������������������������������������������������������������������������������������������������\n");
	printf("\t\t\t\t\t\t ��������Ҫ���ҵ�ѧ�ţ�");
	scanf("%lf",&num);
	ptemp = pstu->pnext;
	while(ptemp!=NULL)
	{
		if(ptemp->data.num == num)
		{
			printf("\n\n");
			printf("\t\t\tѧ��\t\t����\t�༶\t\t\t\t�����ɼ�\tӢ��ɼ�\t���ݽṹ�ɼ�\tƽ����\t�ܷ�\n");
			PrintfAlign(*&ptemp); 
			printf("\t\t\t�����������...\n");
			getch();
			return;
		}
		ptemp = ptemp->pnext;
	}
	printf("\t\t\t���޴��ˣ����س�������...");
	getch();
	return;
	
}
 void SearchName(STU *ptemp)	//���������� 
{
	char name[500];
	system("cls");
	printf("\t\t\t\t\t\t ������������������������������������������������������������������������������������������������\n"); 
	printf("\t\t\t\t\t\t ��                                              ��\n");
	printf("\t\t\t\t\t\t ��            ���ڰ���������ѧ����Ϣ            ��\n");
	printf("\t\t\t\t\t\t ��                                              ��\n");
	printf("\t\t\t\t\t\t ������������������������������������������������������������������������������������������������\n");
	printf("\t\t\t\t\t\t ��������Ҫ���ҵ�������");
	scanf("%s",name);
	ptemp = ptemp->pnext;
	int n = 0;
	while(ptemp !=NULL)
	{
		if(!strcmp(ptemp->data.name,name))//�������������������stemp��������ͬʱ 
		{
			if(n == 0)//ֻ�ڵ�һ���ҵ�������ʱ���ѡ����ʾ 
			{
				printf("\n\n");
				printf("\t\t\tѧ��\t\t����\t�༶\t\t\t\t�����ɼ�\tӢ��ɼ�\t���ݽṹ�ɼ�\tƽ����\t�ܷ�\n");
			}
			PrintfAlign(*&ptemp); 
			n++;
		}
		ptemp = ptemp->pnext;
	}
	if(ptemp == NULL)//�����������������нڵ�� 
	{
		if(n == 0)//���һ��������û���ҵ� 
		{
			printf("\t\t\t���޴��ˣ����س�������...");
		}
		else
		{
			printf("\t\t\t�����������������"); 
		}
	 } 
	getch();
	return;

}
void SearchClass(STU *ptemp)	//���༶���� 
{
	char clas[500];
	system("cls");
	printf("\t\t\t\t\t\t ������������������������������������������������������������������������������������������������\n"); 
	printf("\t\t\t\t\t\t ��                                              ��\n");
	printf("\t\t\t\t\t\t ��            ���ڰ��༶����ѧ����Ϣ            ��\n");
	printf("\t\t\t\t\t\t ��                                              ��\n");
	printf("\t\t\t\t\t\t �ǩ���������������������������������������������������������������������������������������������\n"); 
	printf("\t\t\t\t\t\t ��ʾ��:                                         ��\n");
	printf("\t\t\t\t\t\t ��     2019���������3��                        ��\n");
	printf("\t\t\t\t\t\t ������������������������������������������������������������������������������������������������\n"); 
	printf("\t\t\t\t\t\t ��������Ҫ���ҵİ༶��");
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
				printf("\t\t\tѧ��\t\t����\t�༶\t\t\t\t�����ɼ�\tӢ��ɼ�\t���ݽṹ�ɼ�\tƽ����\t�ܷ�\n");
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
			printf("\t\t\t���޴��ˣ����س�������...");
		}
		else
		{
			printf("\t\t\t�����������������"); 
		}
	 } 
	getch();
	return;

}
void SearchMath()				//�������ɼ����� 
{
	STU *ptemp;
	double math;
	int n = 0; 
	system("cls");
	printf("\t\t\t\t\t\t ������������������������������������������������������������������������������������������������\n"); 
	printf("\t\t\t\t\t\t ��                                              ��\n");
	printf("\t\t\t\t\t\t ��          ���ڰ������ɼ�����ѧ����Ϣ          ��\n");
	printf("\t\t\t\t\t\t ��                                              ��\n");
	printf("\t\t\t\t\t\t ������������������������������������������������������������������������������������������������\n");
	printf("\t\t\t\t\t\t ��������Ҫ���ҵķ�����");
	scanf("%lf",&math);
	ptemp = pstu->pnext;
	while(ptemp!=NULL)
	{
		if(ptemp->data.math == math)
		{
			if(n == 0)
			{
				printf("\n\n");
				printf("\t\t\tѧ��\t\t����\t�༶\t\t\t\t�����ɼ�\tӢ��ɼ�\t���ݽṹ�ɼ�\tƽ����\t�ܷ�\n");
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
			printf("\t\t\t���޴��ˣ����س�������...");
		}
		else
		{
			printf("\t\t\t�����������������"); 
		}
	 } 
	getch();
	return;
	
}
void SearchEnglish()			//��Ӣ��ɼ����� 
{
	STU *ptemp;
	double english;
	int n = 0; 
	system("cls");
	printf("\t\t\t\t\t\t ������������������������������������������������������������������������������������������������\n"); 
	printf("\t\t\t\t\t\t ��                                              ��\n");
	printf("\t\t\t\t\t\t ��          ���ڰ�Ӣ��ɼ�����ѧ����Ϣ          ��\n");
	printf("\t\t\t\t\t\t ��                                              ��\n");
	printf("\t\t\t\t\t\t ������������������������������������������������������������������������������������������������\n");
	printf("\t\t\t\t\t\t ��������Ҫ���ҵķ�����");
	scanf("%lf",&english);
	ptemp = pstu->pnext;
	while(ptemp!=NULL)
	{
		if(ptemp->data.english == english)
		{
		if(n == 0)
		{
			printf("\n\n");
			printf("\t\t\tѧ��\t\t����\t�༶\t\t\t\t�����ɼ�\tӢ��ɼ�\t���ݽṹ�ɼ�\tƽ����\t�ܷ�\n");
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
				printf("\t\t\t���޴��ˣ����س�������...");
			}
			else
			{
				printf("\t\t\t�����������������"); 
			}
		 } 
		getch();
		return;
	
}
void SearchDatastr()			//�����ݽṹ�ɼ�����                                  
{
	STU *ptemp;
	double data;
	int n = 0; 
	system("cls");
	printf("\t\t\t\t\t\t ������������������������������������������������������������������������������������������������\n"); 
	printf("\t\t\t\t\t\t ��                                              ��\n");
	printf("\t\t\t\t\t\t ��        ���ڰ����ݽṹ�ɼ�����ѧ����Ϣ        ��\n");
	printf("\t\t\t\t\t\t ��                                              ��\n");
	printf("\t\t\t\t\t\t ������������������������������������������������������������������������������������������������\n");
	printf("\t\t\t\t\t\t ��������Ҫ���ҵķ�����");
	scanf("%lf",&data);
	ptemp = pstu->pnext;
	while(ptemp!=NULL)
	{
		if(ptemp->data.datastr == data)
		{
		if(n == 0)
			{
				printf("\n\n");
				printf("\t\t\tѧ��\t\t����\t�༶\t\t\t\t�����ɼ�\tӢ��ɼ�\t���ݽṹ�ɼ�\tƽ����\t�ܷ�\n");
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
				printf("\t\t\t���޴��ˣ����س�������...");
			}
			else
			{
				printf("\t\t\t�����������������"); 
			}
		 } 
		getch();
		return;
	
	
}
void SearchAverage()			//��ƽ���ɼ����� 
{
	STU *ptemp;
	double ave;
	int n = 0;
	system("cls");
	printf("\t\t\t\t\t\t ������������������������������������������������������������������������������������������������\n"); 
	printf("\t\t\t\t\t\t ��                                              ��\n");
	printf("\t\t\t\t\t\t ��          ���ڰ�ƽ���ɼ�����ѧ����Ϣ          ��\n");
	printf("\t\t\t\t\t\t ��                                              ��\n");
	printf("\t\t\t\t\t\t ������������������������������������������������������������������������������������������������\n");
	printf("\t\t\t\t\t\t ��������Ҫ���ҵķ�����");
	scanf("%lf",&ave);
	ptemp = pstu->pnext;
	while(ptemp!=NULL)
	{
		if(ptemp->data.average == ave)
		{
			if(n == 0)
			{
				printf("\n\n");
				printf("\t\t\tѧ��\t\t����\t�༶\t\t\t\t�����ɼ�\tӢ��ɼ�\t���ݽṹ�ɼ�\tƽ����\t�ܷ�\n");
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
				printf("\t\t\t���޴��ˣ����س�������...");
			}
			else
			{
				printf("\t\t\t�����������������"); 
			}
		 } 
		getch();
		return;
	
	
}
void SearchToal()				//���ܳɼ����� 
{
	STU *ptemp;
	double toal;
	int n = 0; 
	system("cls");
	printf("\t\t\t\t\t\t ������������������������������������������������������������������������������������������������\n"); 
	printf("\t\t\t\t\t\t ��                                              ��\n");
	printf("\t\t\t\t\t\t ��           ���ڰ��ܳɼ�����ѧ����Ϣ           ��\n");
	printf("\t\t\t\t\t\t ��                                              ��\n");
	printf("\t\t\t\t\t\t ������������������������������������������������������������������������������������������������\n");
	printf("\t\t\t\t\t\t ��������Ҫ���ҵķ�����");
	scanf("%lf",&toal);
	ptemp = pstu->pnext;
	while(ptemp!=NULL)
	{
		if(ptemp->data.toal == toal)
		{
			if(n == 0)
			{
				printf("\n\n");
				printf("\t\t\tѧ��\t\t����\t�༶\t\t\t\t�����ɼ�\tӢ��ɼ�\t���ݽṹ�ɼ�\tƽ����\t�ܷ�\n");
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
				printf("\t\t\t���޴��ˣ����س�������...");
			}
			else
			{
				printf("\t\t\t�����������������"); 
			}
		 } 
		getch();
		return;
	
	
}
void SearchMain()				//���������� 
{
	int select;
	do
	{
 	system("cls");
	printf("\t\t\t\t\t\t ������������������������������������������������������������������������������������������������\n"); 
	printf("\t\t\t\t\t\t ��                                              ��\n");
	printf("\t\t\t\t\t\t ��              ���ڲ���ѧ����Ϣ                ��\n");
	printf("\t\t\t\t\t\t ��                                              ��\n");
	printf("\t\t\t\t\t\t �ǩ���������������������������������������������������������������������������������������������\n"); 
	printf("\t\t\t\t\t\t ��           1.�鿴����ѧ������Ϣ               ��\n");
	printf("\t\t\t\t\t\t �ǩ���������������������������������������������������������������������������������������������\n"); 
	printf("\t\t\t\t\t\t ��           2.��ѧ�Ų�ѯ                       ��\n");
	printf("\t\t\t\t\t\t �ǩ���������������������������������������������������������������������������������������������\n"); 
	printf("\t\t\t\t\t\t ��           3.��������ѯ                       ��\n");
	printf("\t\t\t\t\t\t �ǩ���������������������������������������������������������������������������������������������\n"); 
	printf("\t\t\t\t\t\t ��           4.���༶��ѯ                       ��\n");
	printf("\t\t\t\t\t\t �ǩ���������������������������������������������������������������������������������������������\n"); 
	printf("\t\t\t\t\t\t ��           5.����ѧ�ɼ���ѯ                   ��\n");
	printf("\t\t\t\t\t\t �ǩ���������������������������������������������������������������������������������������������\n"); 
	printf("\t\t\t\t\t\t ��           6.��Ӣ��ɼ���ѯ                   ��\n");
	printf("\t\t\t\t\t\t �ǩ���������������������������������������������������������������������������������������������\n"); 
	printf("\t\t\t\t\t\t ��           7.�����ݽṹ�ɼ���ѯ               ��\n");
	printf("\t\t\t\t\t\t �ǩ���������������������������������������������������������������������������������������������\n"); 
	printf("\t\t\t\t\t\t ��           8.��ƽ���ֲ�ѯ                     ��\n");
	printf("\t\t\t\t\t\t �ǩ���������������������������������������������������������������������������������������������\n"); 
	printf("\t\t\t\t\t\t ��           9.���ܳɼ���ѯ                     ��\n");
	printf("\t\t\t\t\t\t �ǩ���������������������������������������������������������������������������������������������\n"); 
	printf("\t\t\t\t\t\t ��           0.����������                       ��\n");
	printf("\t\t\t\t\t\t ������������������������������������������������������������������������������������������������\n");
	printf("\t\t\t\t\t\t��ѡ��");
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
void AddData()					//���ѧ����Ϣ 
{
	STU *ptemp;
	int i = 1; 
	system("cls");
	printf("\t\t\t\t\t\t ������������������������������������������������������������������������������������������������\n"); 
	printf("\t\t\t\t\t\t ��                                              ��\n");
	printf("\t\t\t\t\t\t ��               ���ڲ���ѧ����Ϣ               ��\n");
	printf("\t\t\t\t\t\t ��                                              ��\n");
	printf("\t\t\t\t\t\t �ǩ���������������������������������������������������������������������������������������������\n"); 
	printf("\t\t\t\t\t\t ��  �����ʽʾ����                              ��\n");
	printf("\t\t\t\t\t\t ��20191308097 ������ 2019���������3�� 80 70 90 ��\n");
	printf("\t\t\t\t\t\t ������������������������������������������������������������������������������������������������\n");
	STU *pnew = NULL;
	pnew = (STU*)malloc(sizeof(STU));
	printf("\t\t\t���ڲ���ѧԱ��Ϣ��\n");
	printf("\t\t\t������ѧ�ţ�");
	scanf("%lf",&pnew->data.num);
	ptemp = pstu->pnext;
	while(ptemp!=NULL)
	{
		if(pnew->data.num == ptemp->data.num)
		{
			printf("\t\t\tѧ���ظ�,���������룺");
			scanf("%lf",&pnew->data.num);
			continue;
		}
		else
			ptemp = ptemp->pnext; 
	}
	printf("\t\t\t������������");
	scanf("%s",&pnew->data.name);
	printf("\t\t\t������༶��");
	scanf("%s",&pnew->data.classes);
	printf("\t\t\t����������ɼ���");
	scanf("%f",&pnew->data.math);
	printf("\t\t\t������Ӣ��ɼ���");
	scanf("%f",&pnew->data.english);
	printf("\t\t\t���������ݽṹ�ɼ���");
	scanf("%f",&pnew->data.datastr);
	pnew->data.toal = (pnew->data.math + pnew->data.english + pnew->data.datastr);
	pnew->data.average = (pnew->data.toal)/3;
	if(pstu->pnext == NULL)     //����Ϊ�� 
	{
		pnew->pnext = NULL;
		pstu->pnext = pnew;
	} 
	else //����Ϊ�� 
	{
		pnew->pnext = pstu->pnext;
		pstu->pnext = pnew;
	}
	printf("\t\t\t�Ƿ�������ѧ����Ϣ��(1/0)");
	scanf("%d",&i);
	if(i == 1)
	{
		AddData(); //������ӣ����ٴε��øú��� 
	} 
	else
		SortNum();	//����ʱ��ѧ������ 
		return;

}
void DeleteData()				//ɾ��ѧ����Ϣ 
{
	STU *ptemp;
	STU *pnew;
	double num;
	int i;
	system("cls");
	printf("\t\t\t\t\t\t ������������������������������������������������������������������������������������������������\n"); 
	printf("\t\t\t\t\t\t ��                                              ��\n");
	printf("\t\t\t\t\t\t ��               ����ɾ��ѧ����Ϣ               ��\n");
	printf("\t\t\t\t\t\t ��                                              ��\n");
	printf("\t\t\t\t\t\t ������������������������������������������������������������������������������������������������\n");
	printf("\t\t\t\t\t\t ��������Ҫɾ����ѧ�ţ�");
	scanf("%lf",&num);
	ptemp = pstu->pnext; //Ŀ��ڵ� 
	pnew = pstu;  //Ŀ��ڵ��ǰ���ڵ� 
	while(ptemp!=NULL)
	{
		if(ptemp->data.num == num)
		{
			printf("\n\n");
			printf("\t\t\tѧ��\t\t����\t�༶\t\t\t\t�����ɼ�\tӢ��ɼ�\t���ݽṹ�ɼ�\tƽ����\t�ܷ�\n");
			PrintfAlign(*&ptemp); 
			printf("\t\t\t��Ϊ�����ҵ���ѧ������Ϣ���Ƿ�ȷ��ɾ����(1/0)"); 
			scanf("%d",&i);
			if(i == 1)
			{
				pnew->pnext = ptemp->pnext;
				free(ptemp);
				printf("\t\t\tɾ���ɹ����������������");
				getch();
				return; 
			}
			else
				return;
		}
		ptemp = ptemp->pnext;
		pnew = pnew->pnext;
	}
	printf("\t\t\t���޴��ˣ����س�����������...");
	getch(); 
	DeleteData(); 
}
void ModifyData()				//�޸�ѧ����Ϣ 
{
	STU *ptemp;
	double num;
	int i;
	system("cls");
	printf("\t\t\t\t\t\t ������������������������������������������������������������������������������������������������\n"); 
	printf("\t\t\t\t\t\t ��                                              ��\n");
	printf("\t\t\t\t\t\t ��               �����޸�ѧ����Ϣ               ��\n");
	printf("\t\t\t\t\t\t ��                                              ��\n");
	printf("\t\t\t\t\t\t ������������������������������������������������������������������������������������������������\n");
	printf("\t\t\t\t\t\t ��������Ҫ�޸ĵ�ѧ�ţ�");
	scanf("%lf",&num);
	ptemp = pstu->pnext;
	while(ptemp!=NULL)
	{
		if(ptemp->data.num == num)
		{
			printf("\n\n");
			printf("\t\t\tѧ��\t\t����\t�༶\t\t\t\t�����ɼ�\tӢ��ɼ�\t���ݽṹ�ɼ�\tƽ����\t�ܷ�\n");
			PrintfAlign(*&ptemp); 
			printf("\t\t\t��Ϊ�����ҵ���ѧ������Ϣ���Ƿ�ȷ���޸ģ�(1/0)"); 
			scanf("%d",&i);
			if(i == 1)
			{
				printf("\t\t\t�����������ѧ������Ϣ��\n");
				printf("\t\t\t������������");
				scanf("%s",&ptemp->data.name);
				printf("\t\t\t������༶��");
				scanf("%s",&ptemp->data.classes);
				printf("\t\t\t����������ɼ���");
				scanf("%f",&ptemp->data.math);
				printf("\t\t\t������Ӣ��ɼ���");
				scanf("%f",&ptemp->data.english);
				printf("\t\t\t���������ݽṹ�ɼ���");
				scanf("%f",&ptemp->data.datastr);
				ptemp->data.toal = (ptemp->data.math + ptemp->data.english + ptemp->data.datastr);
				ptemp->data.average = (ptemp->data.toal)/3;
				printf("\n\n");
				printf("\t\t\tѧ��\t\t����\t�༶\t\t\t�����ɼ�\tӢ��ɼ�\t���ݽṹ�ɼ�\tƽ����\t�ܷ�\n");
				PrintfAlign(*&ptemp);
				printf("\t\t\t�޸ĳɹ����������������");
				getch();
				return; 
			}
			else
				return;
		}
		ptemp = ptemp->pnext;
	}
	printf("\t\t\t���޴��ˣ����س�����������...");
	getch();
	ModifyData();
}
void SortStu()					//���������� 
{
	int select;
	do
	{
	system("cls");
	printf("\t\t\t\t\t\t ������������������������������������������������������������������������������������������������\n"); 
	printf("\t\t\t\t\t\t ��                                              ��\n");
	printf("\t\t\t\t\t\t ��               ��ѡ������ʽ                 ��\n");
	printf("\t\t\t\t\t\t ��                                              ��\n");
	printf("\t\t\t\t\t\t �ǩ���������������������������������������������������������������������������������������������\n"); 
	printf("\t\t\t\t\t\t ��           1.����������                       ��\n");
	printf("\t\t\t\t\t\t �ǩ���������������������������������������������������������������������������������������������\n"); 
	printf("\t\t\t\t\t\t ��           2.��ѧ������                       ��\n");
	printf("\t\t\t\t\t\t �ǩ���������������������������������������������������������������������������������������������\n"); 
	printf("\t\t\t\t\t\t ��           3.���༶����                       ��\n");
	printf("\t\t\t\t\t\t �ǩ���������������������������������������������������������������������������������������������\n"); 
	printf("\t\t\t\t\t\t ��           4.���ߵ���ѧ�ɼ�����               ��\n");
	printf("\t\t\t\t\t\t �ǩ���������������������������������������������������������������������������������������������\n"); 
	printf("\t\t\t\t\t\t ��           5.��Ӣ��ɼ�����                   ��\n");
	printf("\t\t\t\t\t\t �ǩ���������������������������������������������������������������������������������������������\n"); 
	printf("\t\t\t\t\t\t ��           6.�����ݽṹ�ɼ�����               ��\n");
	printf("\t\t\t\t\t\t �ǩ���������������������������������������������������������������������������������������������\n"); 
	printf("\t\t\t\t\t\t ��           7.��ƽ��������                     ��\n");
	printf("\t\t\t\t\t\t �ǩ���������������������������������������������������������������������������������������������\n"); 
	printf("\t\t\t\t\t\t ��           8.���ܷ�����                       ��\n");
	printf("\t\t\t\t\t\t �ǩ���������������������������������������������������������������������������������������������\n"); 
	printf("\t\t\t\t\t\t ��           0.����������                       ��\n");
	printf("\t\t\t\t\t\t ������������������������������������������������������������������������������������������������\n"); 
	printf("\t\t\t\t\t\t��ѡ��");
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
void SortName()					//���������� 
{
	system("cls");
	printf("\t\t\t\t\t\t ������������������������������������������������������������������������������������������������\n"); 
	printf("\t\t\t\t\t\t ��                                              ��\n");
	printf("\t\t\t\t\t\t ��                ���ڰ���������                ��\n");
	printf("\t\t\t\t\t\t ��                                              ��\n");
	printf("\t\t\t\t\t\t ������������������������������������������������������������������������������������������������\n");
	struct data t;  //������ʱ�������   
	STU *ptemp;
	STU *ptemp1;
	
	for(ptemp = pstu->pnext;ptemp != NULL;ptemp = ptemp->pnext)  
		for(ptemp1 = ptemp->pnext;ptemp1 != NULL;ptemp1 = ptemp1->pnext)
		{
			if(strcmp(ptemp1->data.name,ptemp->data.name)<0) //���ǰ��Ľڵ���ں���Ľڵ��򽻻����� 
			{
				t=ptemp->data;
				ptemp->data = ptemp1->data;
				ptemp1->data = t;
			}
		}
	PrintfAll();
} 
void SortNum()					//��ѧ������ 
{
	system("cls");
	printf("\t\t\t\t\t\t ������������������������������������������������������������������������������������������������\n"); 
	printf("\t\t\t\t\t\t ��                                              ��\n");
	printf("\t\t\t\t\t\t ��                ���ڰ�ѧ������                ��\n");
	printf("\t\t\t\t\t\t ��                                              ��\n");
	printf("\t\t\t\t\t\t ������������������������������������������������������������������������������������������������\n");
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
void SortClasses()				//���༶���� 
{
	system("cls");
	printf("\t\t\t\t\t\t ������������������������������������������������������������������������������������������������\n"); 
	printf("\t\t\t\t\t\t ��                                              ��\n");
	printf("\t\t\t\t\t\t ��                ���ڰ��༶����                ��\n");
	printf("\t\t\t\t\t\t ��                                              ��\n");
	printf("\t\t\t\t\t\t ������������������������������������������������������������������������������������������������\n");
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
void SortMath()					//���������� 
{
	system("cls");
	printf("\t\t\t\t\t\t ������������������������������������������������������������������������������������������������\n"); 
	printf("\t\t\t\t\t\t ��                                              ��\n");
	printf("\t\t\t\t\t\t ��              ���ڰ������ɼ�����              ��\n");
	printf("\t\t\t\t\t\t ��                                              ��\n");
	printf("\t\t\t\t\t\t ������������������������������������������������������������������������������������������������\n");
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
void SortEngilsh()				//��Ӣ������ 
{
	system("cls");
	printf("\t\t\t\t\t\t ������������������������������������������������������������������������������������������������\n"); 
	printf("\t\t\t\t\t\t ��                                              ��\n");
	printf("\t\t\t\t\t\t ��              ���ڰ�Ӣ��ɼ�����              ��\n");
	printf("\t\t\t\t\t\t ��                                              ��\n");
	printf("\t\t\t\t\t\t ������������������������������������������������������������������������������������������������\n");
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
void Sortdatastr()				//�����ݽṹ���� 
{
	system("cls");
	printf("\t\t\t\t\t\t ������������������������������������������������������������������������������������������������\n"); 
	printf("\t\t\t\t\t\t ��                                              ��\n");
	printf("\t\t\t\t\t\t ��            ���ڰ����ݽṹ�ɼ�����            ��\n");
	printf("\t\t\t\t\t\t ��                                              ��\n");
	printf("\t\t\t\t\t\t ������������������������������������������������������������������������������������������������\n");
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
void SortAve()					//��ƽ�������� 
{
	system("cls");
	printf("\t\t\t\t\t\t ������������������������������������������������������������������������������������������������\n"); 
	printf("\t\t\t\t\t\t ��                                              ��\n");
	printf("\t\t\t\t\t\t ��               ���ڰ�ƽ��������               ��\n");
	printf("\t\t\t\t\t\t ��                                              ��\n");
	printf("\t\t\t\t\t\t ������������������������������������������������������������������������������������������������\n");
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
void SortToal()					//���ܷ����� 
{
	system("cls");
	printf("\t\t\t\t\t\t ������������������������������������������������������������������������������������������������\n"); 
	printf("\t\t\t\t\t\t ��                                              ��\n");
	printf("\t\t\t\t\t\t ��                ���ڰ��ܷ�����                ��\n");
	printf("\t\t\t\t\t\t ��                                              ��\n");
	printf("\t\t\t\t\t\t ������������������������������������������������������������������������������������������������\n");
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
void SaveData()					//�����ļ� 
{
	STU *ptemp = pstu->pnext;
	
	if ((fp = fopen("�ɼ���.txt","w")) == NULL)
	{
		printf("\t\t\t�ļ���ʧ�ܣ�"); 
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
void PrintfAll()				//�������������Ϣ 
{
	system("cls");
	printf("\t\t\t\t\t\t ������������������������������������������������������������������������������������������������\n"); 
	printf("\t\t\t\t\t\t ��                                              ��\n");
	printf("\t\t\t\t\t\t ��              ���ڲ鿴ѧ����Ϣ                ��\n");
	printf("\t\t\t\t\t\t ��                                              ��\n");
	printf("\t\t\t\t\t\t ������������������������������������������������������������������������������������������������\n"); 
	int i;
	STU *ptemp = pstu->pnext;
	if(ptemp == NULL)
	{
		printf("\t\t\tû�����ݣ���ѡ���Ƿ�¼������:(1/0)");
		scanf("%d",&i);
		if(i == 1)
		{
			AddData();
			return;
		}
		else
		{
			printf("\t\t\t�����������...\n");
			getch();
			return; 
		} 
	}
	printf("\t\t\tѧ��\t\t����\t�༶\t\t\t\t�����ɼ�\tӢ��ɼ�\t���ݽṹ�ɼ�\tƽ����\t�ܷ�\n");
	while(ptemp!= NULL)
	{
		PrintfAlign(*&ptemp); 
		ptemp = ptemp->pnext;
	}
		printf("\t\t\t�����������...\n");
		getch();
}
void PrintfAlign(STU *&ptemp)   //������� 
{
	int i;
	if(strlen(ptemp->data.classes)==25)//25Ϊ�����ѧԺרҵ���ַ����רҵ���ַ����� 
	{
			printf("\t\t\t%.lf\t%s\t%s\t%.2f\t\t%.2f\t\t%.2f\t\t%.2f\t%.2f\n",ptemp->data.num,ptemp->data.name,ptemp->data.classes,ptemp->data.math,ptemp->data.english,ptemp->data.datastr,ptemp->data.average,ptemp->data.toal);
	}
	else if(strlen(ptemp->data.classes)<25)//�����רҵ���ַ����ȱ�25�� 
	{
		i = 25-strlen(ptemp->data.classes);//��רҵ���ַ����ȱ�25С���پ�������ٿո�   �Ա�֤�����ʱ������� 
		printf("\t\t\t%.lf\t%s\t%s",ptemp->data.num,ptemp->data.name,ptemp->data.classes);
		for(i;i>0;i--)
		{
			printf(" ");
		}
		printf("\t%.2f\t\t%.2f\t\t%.2f\t\t%.2f\t%.2f\n",ptemp->data.math,ptemp->data.english,ptemp->data.datastr,ptemp->data.average,ptemp->data.toal);
	}
}
void Remind()					//��ܰ��ʾ���� 
{
	system("cls");
	printf("\t\t\t\t\t\t ������������������������������������������������������������������������������������������������\n"); 
	printf("\t\t\t\t\t\t ��                   ��ܰ��ʾ                   ��\n");
	printf("\t\t\t\t\t\t �ǩ���������������������������������������������������������������������������������������������\n"); 
	printf("\t\t\t\t\t\t ��       Ϊ���ⲻ��Ҫ���鷳������������뾡���� ��\n");
	printf("\t\t\t\t\t\t �� ������Ҫ�����롣                             ��\n");
	printf("\t\t\t\t\t\t ��          ������������㣬�����½⣡          ��\n"); 
	printf("\t\t\t\t\t\t ������������������������������������������������������������������������������������������������\n"); 
	printf("\t\t\t\t\t\t�����������������") ; 
	getch();
}
void  menu()					//���˵� 
{
	system("cls");
	printf("\t\t\t\t\t\t ������������������������������������������������������������������������������������������������\n"); 
	printf("\t\t\t\t\t\t ��                                              ��\n");
	printf("\t\t\t\t\t\t ��           ��ӭʹ��ѧ���ɼ�����ϵͳ           ��\n");
	printf("\t\t\t\t\t\t ��                                              ��\n");
	printf("\t\t\t\t\t\t �ǩ���������������������������������������������������������������������������������������������\n"); 
	printf("\t\t\t\t\t\t ��                   �˵���                     ��\n");
	printf("\t\t\t\t\t\t �ǩ���������������������������������������������������������������������������������������������\n"); 
	printf("\t\t\t\t\t\t ��           1.�鿴ѧ����Ϣ                     ��\n");
	printf("\t\t\t\t\t\t �ǩ���������������������������������������������������������������������������������������������\n"); 
	printf("\t\t\t\t\t\t ��           2.����ѧ����Ϣ                     ��\n");
	printf("\t\t\t\t\t\t �ǩ���������������������������������������������������������������������������������������������\n"); 
	printf("\t\t\t\t\t\t ��           3.ɾ��ѧ����Ϣ                     ��\n");
	printf("\t\t\t\t\t\t �ǩ���������������������������������������������������������������������������������������������\n"); 
	printf("\t\t\t\t\t\t ��           4.�޸�ѧ����Ϣ                     ��\n");
	printf("\t\t\t\t\t\t �ǩ���������������������������������������������������������������������������������������������\n"); 
	printf("\t\t\t\t\t\t ��           5.������Ϣ����ʽ                 ��\n");
	printf("\t\t\t\t\t\t �ǩ���������������������������������������������������������������������������������������������\n"); 
	printf("\t\t\t\t\t\t ��           0.�˳�ϵͳ                         ��\n");
	printf("\t\t\t\t\t\t ������������������������������������������������������������������������������������������������\n"); 
}
/*****************������ *****************/ 
int main()
{
	openfile();
	Remind();
	int select;
	do
	{
		menu();
		printf("\t\t\t\t\t\t��ѡ��");
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

