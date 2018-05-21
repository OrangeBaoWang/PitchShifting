/*д���ļ���Ŀ����Ϊ�˺����ѷ���MATLAB��interp1��interp2תΪc�Ĺ��ܡ�
ע�����������˺ܾ�û�ҵ�������¶������Լ�д��*/
/**************���Բ�ֵ***********
(x0,y0)->(x1,y1)
*********************************/
double inter_linear(double x0,double x1,double y0,double y1,double x) 
{ 
	double a0,a1,y; 
	a0=(x-x1)/(x0-x1); 
	a1=(x-x0)/(x1-x0); 
	y=a0*y0+a1*y1; 

	return (y); 
} 
/**************һά���Բ�ֵ*************
x[],y[]Ϊ����Ϊn��������һά���飩
***************************************/
double interp1(double x[],double y[],int n,double pp)
{
	double logx[18],logp,z;
	int i,tmp;
	for(i = 0;i<n;i++)
	{
		logx[i] = log(x[i]);
	}
	logp = log(pp);
	for(i = 0;i<n;i++)
	{
		if((logp>=logx[i])&&(logp<logx[i+1]))
		{
			tmp = i;
			break;
		}
	}
	//printf("i = %d\n",i);
	//if(i == 0)
	//{
	//	z = y[0];
	//	return z;
	//}
	if(i == 18)
	{
		z = y[i-1];
		return z;
	}
	z = inter_linear(logx[i],logx[i+1],y[i],y[i+1],logp);

	return z;
}
/**************˫���Բ�ֵ************
x[],y[]Ϊ����Ϊn��������һά���飩��zΪ���󣨶�Ӧ�Ķ�ά���飬�˴�Ϊ����ָ�룩
************************************/
double interp2(double x[],double y[],double **z,int m,int n,
				double a,double b)
{
	int i,j,tmpi,tmpj;
	double w,w1,w2;
	tmpi = 0;
	tmpj = 0;
	w = 0.0;
	/***********ȷ��(a,b)����λ��***********/
	for(i=0; i<m; i++)                    //ȷ��a��x���λ��
	{
		if( (a<=x[i])&&(a>x[i+1]) )     //xΪ����  
		{
			tmpi = i;
			break;
		}
		//if( (a>=x[i])&&(a<x[i+1]) )       //xΪ����
		//{
		//	tmpi = i;
		//	break;
		//}
	}
	for(j=0; j<n; j++)                    //ȷ��b��y���λ��
	{
		if( (b>=y[j])&&(b<y[j+1]) )       
		{
			tmpj = j;
			break;
		}
	}
	/**************x������в�ֵ*************/
	if(x[tmpi] == a)
	{
		//ȡ����ڵ�ֵ
		w1 = z[tmpi][tmpj];
		w2 = z[tmpi][tmpj+1];

		/*******y������в�ֵ********/
		if(y[tmpj] == b)
		{
			//ȡ����ڵ�ֵ
			w = w1;
		}
		else
		{
			//���в�ֵ(y)
			w = inter_linear(y[tmpj],y[tmpj+1],w1,w2,b);
		}
		/***************************/
	}
	else
	{
		//���в�ֵ(x)
		w1 = inter_linear(x[tmpi],x[tmpi+1],z[tmpi][tmpj],z[tmpi+1][tmpj],a);
		w2 = inter_linear(x[tmpi],x[tmpi+1],z[tmpi][tmpj+1],z[tmpi+1][tmpj+1],a);

		/*******y������в�ֵ********/
		if(y[tmpj] == b)
		{
			//ȡ����ڵ�ֵ
			w = w1;
		}
		else
		{
			//���в�ֵ(y)
			w = inter_linear(y[tmpj],y[tmpj+1],w1,w2,b);
		}
		/****************************/
	}

	return w;
}
//C������ȡtxt�ļ�����
int read_data(const char * path, double a[])
{
	/***�������***/
	double f_temp;
	int i;
	//int j;
	FILE *fp;

	/*****���ļ�����ȡ����*****/
	fp = fopen(path, "rb");
	if(fp == NULL)						//���ʧ����
	{
		printf("���ļ�ʧ��!");
		return -1;
	}
	i = 0;
	while(fscanf(fp, "%lf\n", &f_temp) == 1)
	{
		a[i] = f_temp;
		i++;
	}
	//printf("i = %d\n",i);
	///*******��ӡ��������*******/
	//for( j = 0; j<i; j++)
	//{
	//	printf("a[%d", j);
	//	printf("] = ");
	//	printf("%14.14f\n", a[j]);
	//}

	return 0;
}

//C������ȡtxt�ļ����� 2
int read_data2(const char * path, double *a)
{
	/***�������***/
	double f_temp;
	int i;
	//int j;
	FILE *fp;

	/*****���ļ�����ȡ����*****/
	fp = fopen(path, "rb");
	if(fp == NULL)						//���ʧ����
	{
		printf("���ļ�ʧ��!");
		return -1;
	}
	i = 0;
	while(fscanf(fp, "%lf\n", &f_temp) == 1)
	{
		*a = f_temp;
		a = a+1;
	}
	//printf("i = %d\n",i);
	///*******��ӡ��������*******/
	//for( j = 0; j<i; j++)
	//{
	//	printf("a[%d", j);
	//	printf("] = ");
	//	printf("%14.14f\n", a[j]);
	//}

	return 0;
}

