#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<malloc.h>
#include<stdlib.h>
#include<iostream>
using namespace std;

#define INF 65535
#define ERROR -1

// 定义城市(顶点)
typedef struct
{
	int v; // 城市的编号
	char cou_name[11] ;
	char city_name[20] ;
	double Latitude, Longitude;
}scity; 

// 定义路径（存放路径的数据）
typedef struct 
{
	char ocity[20], dcity[20], note[20]; // original city, destination city, notes
	int trans, time, cost; // type of transport(1 bus , 2 Train, 3 Boat, 4 Plane), average time requried, average cost in USD
}sroute;
// 权的定义
typedef struct
{
	int time, cost;
}Weight;
// 边的定义
typedef struct
{
	int v1,v2;
	Weight weight;
}enode;
// 图结点的定义
typedef enode* edge;
typedef struct
{
	int Nv, Ne;
	Weight G[10000][10000];
	scity citydata[10000]; // 存储顶点内的数据
}GNode;
typedef GNode* MGraph;
// 分配结构体数组用于存储city和route
static scity city[10000]; 
static sroute route[100000]; 
// 分配静态数组存储最短路搜索结果
static int time[10000];
static int cost[10000];
static int path_fastest[10000];    // 最快路径
static int path_cheapest[10000];   // 最便宜路径
static int nstart = 0, ndest = 0;  // 出发点和目的地的编号
static int ncity_fastest[10000];   // 最快路径上各城市的编号
static int ncity_cheapest[10000];  // 最便宜路径上各城市的编号
static MGraph Graph;
edge e = (edge)malloc(sizeof(enode));       // 建立边结点

// 函数声明
int initialize(void);
MGraph BuildGraph(MGraph Graph); 
MGraph CreateGraph(void);
void InsertEdge(MGraph Graph, edge e);
int BinarySearch(int Array[], int low, int high, int key);
int FindMinDist(MGraph Graph, int dist[], bool collected[]);
bool Dijkstra_time(MGraph Graph, int time[], int path[], int s);
bool Dijkstra_cost(MGraph Graph, int cost[], int path[], int s);
void PrintPath(int ncity[], int path[],int start,int dest,int n);
void WriteToHTML(char *filename,int ncity[]);
/**************************************************************************************************************************************/
/*由于测试样例为稀疏图，而程序使用的存储方式是邻接矩阵，故而本程序运行速度有待提高（大约7秒左右）。运行时请稍稍耐心地等待几秒！！！   */
/**************************************************************************************************************************************/
int main(void)
{
	//将两个数组分别初始化，并将表格中的数据读入结构体
	initialize();
	// 建立图
	Graph = BuildGraph(Graph);
	// 输入起点城市，终点城市，运输条件(本题不必考虑运输条件）
	char start[20], dest[20];	
	printf("\nInput your original city: \n(您可以输入: fqgbusgmrimqchhjmej 进行测试)\n");
	scanf("%s",start);
	printf("\n\nInput your destination city: \n(您可以输入: vvswopmgenahdeonpnr 进行测试)\n");
	scanf("%s", dest);
	// 找出最短路
	while(strcmp(start, city[nstart].city_name) != 0)nstart++;
	while(strcmp(dest, city[ndest].city_name) != 0)ndest++;
	Dijkstra_time(Graph, time, path_fastest, nstart);
	Dijkstra_cost(Graph, cost, path_cheapest, nstart);
	printf("\nThe fastest way from %s to %s is: \n", start, dest);
	PrintPath(ncity_fastest,path_fastest,nstart,ndest,10000);
	printf("\n\nThe cheapest way from %s to %s is: \n", start, dest);
	PrintPath(ncity_cheapest,path_cheapest,nstart,ndest,10000);
	// 生成google地图
	printf("\n\nNow I will draw the path on a google map.");
	printf("\n Now input your html's name for the fastest path(您可以输入mymap1.html进行测试): ");
	char filename1[64] = "";  char filename2[64] = "";
	scanf("%s",filename1);
	WriteToHTML(filename1,ncity_fastest);
	printf("\n\n Now input your html's name for the cheapest path(您可以输入mymap2.html进行测试): ");
	scanf("%s",filename2);
	WriteToHTML(filename2,ncity_cheapest);
	return 0;
}

// 结构体初始化及数据读入
int initialize(void)
{
	for(int i=0;i<10000;i++)
	{
		for(int j=0;j<11;j++) city[i].cou_name[j] = '\0';
		for(int j=0;j<20;j++) city[i].city_name[j] = '\0';
		city[i].Latitude = city[i].Longitude = 0;
		city[i].v = i;
	}// city初始化完毕

	for(int i=0;i<100000;i++)
	{
		for(int j=0;j<20;j++) route[i].ocity[j] = route[i].dcity[j] = '\0';
		route[i].trans = 1;
		route[i].time = route[i].cost = 0;
		strcpy(route[i].note, "no comment");
	}// route初始化完毕
	for(int i=0;i<10000;i++)path_fastest[i] = path_cheapest[i] = ncity_fastest[i] = ncity_cheapest[i] = -1;
	 // path, ncity数组初始化完毕
	
	FILE* fp1 = fopen("cities_big.csv", "rt");
	FILE* fp2 = fopen("routes_big.csv", "rt");
	if(!fp1)
	{
		printf("Cannot open the cities_big.csv!"); 
	    return 0; 
	}
	if(!fp2)
	{
		printf("Cannot open the routes_big.csv!"); 
	    return 0; 
	}// open the files and read them

	for(int i=0; i<10000;i++)
	{
		for(int j=0;j<10;j++) city[i].cou_name[j] = fgetc(fp1);
		city[i].cou_name[10]='\0';
		fgetc(fp1);  // 空读一位，跳过分隔符
		for(int j=0;j<19;j++) city[i].city_name[j] = fgetc(fp1);
		city[i].city_name[19]='\0';
		fgetc(fp1);  // 空读一位，跳过分隔符
		fscanf(fp1,"%lf,%lf\n", &(city[i].Latitude), &(city[i].Longitude));
	}// cites_big 读取完毕  fscanf总是报错,只能这样一个一个读了，如果能用c++的in就会很简单

	for(int i=0;i<100000; i++)
	{
		for(int j=0;j<19;j++) route[i].ocity[j] = fgetc(fp2);
		route[i].ocity[19] = '\0';
		fgetc(fp2); // 空读一位，跳过分隔符
		for(int j=0;j<19;j++) route[i].dcity[j] = fgetc(fp2);
		route[i].dcity[19] = '\0';
		fgetc(fp2); // 空读一位，跳过分隔符
		//char tran[5];
		//for(int j=0;j<5;j++) tran[j] = '\0'; // 初始化tran
		//for(int j=0; j<5&&tran[j] !=',';j++)tran[j] = fgetc(fp2);
		//if(strcmp(tran,"bus,") == 1)route[i].trans = 1;
		//else if(strcmp(tran,"train") == 1) { route[i].trans = 2; fgetc(fp2); /* 跳过分隔符*/ }
		//else if(strcmp(tran,"boat,") == 1)route[i].trans = 3;
		//else if(strcmp(tran,"plane") == 1) { route[i].trans = 4; fgetc(fp2); /* 跳过分隔符*/ }
		//else { printf("ERROR!"); return 0; }   //读入交通工具种类,本题中全为bus，故可略去这个过程,如为一般情况，加入这段
		route[i].trans =1;
		for(int j=0;j<4;j++) fgetc(fp2);
		fscanf(fp2, "%d,%d,\"No Comment\"",&(route[i].time),&(route[i].cost));
		fscanf(fp2,"\n");
	} //routes_big读取完毕

	return 1;
}
// 建立数据结构：图
MGraph BuildGraph(MGraph Graph)
{
	Graph=CreateGraph(); // 初始化有10000个顶点但没有边的图
    for (int i=0; i<100000; i++)               // 插入邻接矩阵
	{
		int j=0,k=0;
		while(strcmp(route[i].ocity, city[j].city_name) != 0)j++;
		e->v1 = j;
		while(strcmp(route[i].dcity, city[k].city_name) != 0)k++;
		e->v2 = k;
		e->weight.time = route[i].time; e->weight.cost = route[i].cost;
		InsertEdge(Graph, e);
    }
	for(int v=0 ; v<10000; v++)            // 读入顶点数据
		{
			strcpy(Graph->citydata[v].city_name, city[v].city_name);
			strcpy(Graph->citydata[v].cou_name, city[v].cou_name);
			Graph->citydata[v].Latitude = city[v].Latitude;
			Graph->citydata[v].Longitude = city[v].Longitude;
	     }
	return Graph;
}
// 辅助函数：初始化有顶点而无边的图
MGraph CreateGraph(void)
{
	MGraph Graph;
	Graph = (MGraph)malloc(sizeof(GNode)); // 建立图
	Graph->Nv = 10000; Graph->Ne = 0;     // 初始化邻接矩阵
	// 默认顶点编号从0开始，到Graph->Nv-1
	for(int v=0; v<Graph->Nv; v++)
		for(int w=0; w<Graph->Nv; w++)
		{
			Graph->G[v][w].time = INF;
			Graph->G[v][w].cost = INF;
		}
	return Graph;
}
// 辅助函数： 向图中插入一条边
void InsertEdge(MGraph Graph, edge e)
{
	Graph->G[e->v1][e->v2].time = e->weight.time;
	Graph->G[e->v1][e->v2].cost = e->weight.cost;
	Graph->Ne++;
}
// 辅助函数: 寻找未被收录的顶点中权重最小者
int FindMinDist(MGraph Graph, int dist[], int collected[]) // 返回未被收录顶点中dist 最小者
{
	int minv,v;
	int mindist = INF;
	for(v=0; v<10000; v++)
	{
		if(collected[v] == false && dist[v]<mindist) // 若v未被收录且dist[v]更小
		{
			mindist = dist[v]; // 更新最小距离
			minv = v; // 更新顶点
		}
	}
	if(mindist < INF) return minv; // 若找到了最小的dist, 返回对应的顶点下标
	else return ERROR;
} 
// 最快路径求解
bool Dijkstra_time(MGraph Graph, int time[], int path[], int s)
{
	static int collected[10000];
	int v,w;
	// 初始化
	for(v=0; v<10000; v++)
	{
		time[v] = Graph->G[s][v].time;
		if(time[v]<INF) path[v] = s;
		else path[v]=-1;
		collected[v] = false;
	}
	// 先将起点收入集合
	time[s]=0;
	collected[s]=true;  
	while(1)            // 核心算法！！！！
	{                   // v = 未被收录的顶点中权重最小者
		v= FindMinDist(Graph, time, collected);
		if(v == ERROR) break; // 若这样的v不存在，递归结束
		collected[v] = true;  // 收录v
		for(w=0;w<Graph->Nv;w++) // 对图中每个顶点w
			if(collected[w] == false && Graph->G[v][w].time < INF) // 若w是v的邻接点且未被收录
			{
				if(Graph->G[v][w].time < 0 ) return false;  // 若有负边,返回错误
				if(time[v] + Graph->G[v][w].time < time[w]) 
				{
					time[w] = time[v]+Graph->G[v][w].time; // 更新dist[w]
					path[w] = v;  // 更新s到w的路径
				}
			}
	}
	return true;  // 递归结束，返回正确，完成算法
}
// 最便宜路径求解
bool Dijkstra_cost(MGraph Graph, int cost[], int path[], int s)
{
	int collected[10000];
	int v,w;
	for(v=0; v<Graph->Nv; v++)
	{
		cost[v] = Graph->G[s][v].cost;
		if(cost[v]<INF) path[v] = s;
		else path[v]=-1;
		collected[v] = false;
	}  // 先将起点收入集合
	cost[s]=0;
	collected[s]=true;
	while(1)  // 核心算法！！！！
	{   // v = 未被收录的顶点中cost最小者
		v= FindMinDist(Graph, cost, collected);
		if(v == ERROR) break; // 若这样的v不存在，递归结束
		collected[v] = true;  // 收录v
		for(w=0;w<Graph->Nv;w++) // 对图中每个顶点w
			if(collected[w] == false && Graph->G[v][w].cost < INF) // 若w是v的邻接点且未被收录
			{
				if(Graph->G[v][w].cost< 0) return false;  // 若有负边,返回错误
				if(cost[v]+Graph->G[v][w].cost<cost[w]) 
				{
					cost[w] = cost[v]+Graph->G[v][w].cost; // 更新dist[w]
					path[w] = v;  // 更新s到w的路径
				}
			}
	}
	return true;  // 递归结束，返回正确，完成算法
}
// 打印最短路径
void PrintPath(int ncity[],int path[],int start,int dest,int n)
{
	int tmp = dest, i = 1;
	int *tmparr = new int[n]; // 存储路径
	for(int ii =0; ii<n; ii++)tmparr[ii] = -1; // 初始化完毕
	tmparr[0] = dest;  // 储存目的地编号
	for(int j=1; j<n; j++)
	{
		if(path[tmp] != -1 && tmp != -1)
		{
			tmparr[i] = path[tmp];
			tmp = path[tmp];
			i++;
		}
		else break;
	}  // path[]数组赋值完毕，下面打印路径，即将数组逆向输出
	int n_city = 0;
	for(int i=n-1; i>=0; i--)
	{
		if(tmparr[i] != -1)
		{
			int down = tmparr[i];
			ncity[n_city] = down; n_city++;
			printf("%s, %s (%f,%f)",city[down].city_name,city[down].cou_name,city[down].Latitude,city[down].Longitude);  
			// 打印路径上每个城市的名字，所在国家的名字及坐标
			if(i)printf("\n-->");
		}
	}
}
// 生成HTML文件
void WriteToHTML(char *filename,int ncity[])
{
	int n=0, count=0;
	while(ncity[n] != -1)n++;  // 计算路径上城市个数，为0~n-1共计n个

	FILE* fp = fopen(filename, "wt");
	if(!fp)
	{
		printf("Cannot open your file!"); 
	    return; 
	} // 检查你输入的文件是否已创建
	fprintf(fp,"<HTML><HEAD><TITLE>shortest path from %s to %s</TITLE></HEAD><script type='text/javascript' src='http://maps.google.com/maps/api/js?sensor=false'></script><script>function initialize() { var myOptions = { zoom: 3, center: new google.maps.LatLng(0, 0), mapTypeId: google.maps.MapTypeId.ROADMAP};var map=new google.maps.Map(document.getElementById('map'), myOptions);\n", city[ncity[0]].city_name, city[ncity[n-1]].city_name);
	for(int i = 0; i < n-1; i++){
		fprintf(fp, "var marker%d = new google.maps.Marker({ position: new google.maps.LatLng(%f,%f), map: map, title:'%s, %s'});\n",count++, city[ncity[i]].Latitude, city[ncity[i]].Longitude,city[ncity[i]].city_name, city[ncity[i]].cou_name);
		fprintf(fp, "var marker%d = new google.maps.Marker({ position: new google.maps.LatLng(%f,%f), map: map, title:'%s, %s'});\n",count++, city[ncity[i+1]].Latitude, city[ncity[i+1]].Longitude,city[ncity[i+1]].city_name, city[ncity[i+1]].cou_name);
		fprintf(fp, "var contentString%d = \"%s, %s --> %s, %s (bus, %d hour, $%d)\"; var path%d = new google.maps.Polyline({ path: [new google.maps.LatLng(%f, %f), new google.maps.LatLng(%f, %f)], strokeColor: '#0000FF', strokeOpacity: 1.0, strokeWeight: 2}); path%d.setMap(map); google.maps.event.addListener(path%d, 'click', function(event) { alert(contentString%d); });\n", i, city[ncity[i]].city_name, city[ncity[i]].cou_name, city[ncity[i+1]].city_name, city[ncity[i+1]].cou_name, Graph->G[ncity[i]][ncity[i+1]].time, Graph->G[ncity[i]][ncity[i+1]].cost, i, city[ncity[i]].Latitude, city[ncity[i]].Longitude, city[ncity[i+1]].Latitude, city[ncity[i+1]].Longitude, i, i, i);
	}
	fprintf(fp,"} google.maps.event.addDomListener(window, 'load', initialize); </script></HEAD><BODY><div id='map' style='width:100%%;height:100%%;'></div></BODY></HTML>");
}