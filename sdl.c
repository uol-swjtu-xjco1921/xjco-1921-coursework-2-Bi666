#include<stdio.h>
#include<stdlib.h>
#include<SDL.h>
//定义结点结构体
SDL_Rect * graphicPoints;
int ** g;//邻接矩阵方式
int * vis;
int ** path;
int * dis;
int n,m,s,t;//n个结点，m条边,s为起点,t为终点
SDL_Window * window;
SDL_Renderer * renderer;
void val_init(){
for(int i = 0;i<n;i++){
dis[i] = 0x3f3f3f3f;
vis[i] = 0;
for(int j = 0;j<n;j++){
g[i][j] = 0x3f3f3f3f;//代表距离为无穷
path[i][j] = -1;//代表源结点为自己
}
}
}
void init(){
//初始化相应window、renderer、texture
SDL_Init(SDL_INIT_VIDEO);
window = SDL_CreateWindow("Path Draw", SDL_WINDOWPOS_CENTERED,
SDL_WINDOWPOS_CENTERED, 600, 600, SDL_WINDOW_SHOWN);
renderer = SDL_CreateRenderer(window, -1, 0);
freopen("graph.txt","r",stdin);//更换标准输入流
//初始化图
scanf("%d %d %d %d",&n,&m,&s,&t);
graphicPoints = (SDL_Rect *)malloc(sizeof(SDL_Rect) * n);//n个结点
g = (int **)malloc(sizeof(int *) * n);//生成对应数据
vis = (int *)malloc(sizeof(int) * n);
path = (int **)malloc(sizeof(int *) * n);
dis = (int *)malloc(sizeof(int) * n);
for(int i = 0;i < n;i++){
//生成二维数组
g[i] = (int *)malloc(sizeof(int) * n);
path[i] = (int *)malloc(sizeof(int) * n);
//读入对应的结点坐标
scanf("%d %d",&graphicPoints[i].x,&graphicPoints[i].y);
graphicPoints[i].x = graphicPoints[i].x - 5;
graphicPoints[i].y = graphicPoints[i].y - 5;
graphicPoints[i].w = 10;//设置矩阵长宽
graphicPoints[i].h = 10;
}
//初始化相关数组
val_init();
int node1,node2;
for(int i = 0;i < m;i++){
scanf("%d %d",&node1,&node2);
int a = (graphicPoints[node1].x - graphicPoints[node2].x) *
(graphicPoints[node1].x - graphicPoints[node2].x);
int b = (graphicPoints[node1].y - graphicPoints[node2].y) *
(graphicPoints[node1].y - graphicPoints[node2].y);
g[node1][node2] = (a + b);
g[node2][node1] = (a + b);//无向图
}
}
void quit(){//销毁所有堆中的数据，避免内存溢出
SDL_DestroyRenderer(renderer);
SDL_DestroyWindow(window);
free(graphicPoints);
for(int i = 0;i<n;i++){
free(g[i]);
free(path[i]);
}
free(g);
free(vis);
free(path);
free(dis);
}
void render(){
SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);//red
SDL_RenderClear(renderer);
//绘制基础路径
SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);//黑色
for(int i=0;i<n;i++){
for(int j=0;j<n;j++){
if(g[i][j]!=0x3f3f3f3f){//有连接，那么则绘制
SDL_RenderDrawLine(renderer,graphicPoints[i].x +
5,graphicPoints[i].y + 5,graphicPoints[j].x + 5,graphicPoints[j].y + 5);
}
}
}
SDL_SetRenderDrawColor(renderer, 0, 255, 200, 255);//
//绘制最短路径即s到t的最短路径
int pos = t;
while(path[s][pos] != -1){
int p = path[s][pos];
SDL_RenderDrawLine(renderer,graphicPoints[p].x + 5,graphicPoints[p].y +
5,graphicPoints[pos].x + 5,graphicPoints[pos].y + 5);
pos = path[s][pos];
}
//绘制基础顶点
for(int i=0;i<n;i++){
SDL_RenderDrawRect(renderer,&graphicPoints[i]);
}
//给顶点填充颜色
for(int i = 0;i<n;i++){
SDL_SetRenderDrawColor(renderer, 255, 255, 0 , 255);
SDL_RenderFillRect(renderer,&graphicPoints[i]);
}
SDL_RenderPresent(renderer);//渲染到屏幕
SDL_Delay(10000);
}
int main(){
init();
dijkstra();
render();
quit();
return 0;
}