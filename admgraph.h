#ifndef ADMGRAPH_H
#define ADMGRAPH_H
#include "iostream"

#define INF 0x3f3f3f3f
#define tpT template<class T>
using namespace std;

tpT
class AdmGraph{
private:
    T *vertex;                                  //顶点数组
    int **arc;                                  //邻接矩阵
    int MaxSize, vertexNum, arcNum;             //MaxSize最大值,vertexNum顶点数,arcNum边数
public:
    AdmGraph(T a[],int n);    //构造函数,a[]顶点,n顶点数,m边数,type边类型(0/1->有向/无向)
    ~AdmGraph();                               //析构函数
    int findNum(T);                             //查找顶点编号
    T findVertex(int);                          //查找编号对应顶点
    int getVertexNum();                         //返回顶点数
    int getArcNum();                            //返回边数
    bool add(int a,int b,int v,int type=0);     //添加边,a顶点1,b顶点2,v权值,type边类型
    bool del(int a,int b,int type=0);           //删除边,a顶点1,b顶点2,type边类型
    std::string dijkstra(int,int,int&);          //dijkstra算法求s到t的最短路
};

template<class T>
AdmGraph<T>::AdmGraph(T *a, int n) {
    //初始化成员变量，获取内存空间
    MaxSize = n + 10, vertexNum = n, arcNum = 0;
    vertex = new T[MaxSize];
    arc = new int *[MaxSize];
    for(int i=0;i<MaxSize;i++) arc[i] = new int[MaxSize];

    //初始化顶点
    for(int i=0;i<n;i++) vertex[i+1] = a[i];
    //初始化邻接矩阵
    for(int i=0;i<MaxSize;i++){
        for(int j=0;j<MaxSize;j++){
            arc[i][j]=INF;
        }
    }
}

//析构函数
template<class T>
AdmGraph<T>::~AdmGraph() {
    delete[] vertex;
    for(int i=0;i<MaxSize;i++) delete[] arc[i];
    delete[] arc;
}

//查找顶点编号
template<class T>
int AdmGraph<T>::findNum(T val) {
    int i=1;
    while(vertex[i]!=val && i<=vertexNum) i++;
    return i;
}

//查找编号对应顶点
template<class T>
T AdmGraph<T>::findVertex(int Num) {
    return vertex[Num];
}

template<class T>
int AdmGraph<T>::getVertexNum() {
    return vertexNum;
}

template<class T>
int AdmGraph<T>::getArcNum() {
    return arcNum;
}

//添加路径
template<class T>
bool AdmGraph<T>::add(int a, int b, int v,int type) {
    arc[a][b]=v,arcNum++;
    if(type) arc[b][a]=v,arcNum++;
    return 1;

}

//删除路径
template<class T>
bool AdmGraph<T>::del(int a, int b, int type) {
    arc[a][b]=INF,arcNum--;
    if(type) arc[b][a]=INF,arcNum--;
    return 1;
}

//dijkstra算法
template<class T>
string AdmGraph<T>::dijkstra(int s,int t,int& pathCnt) {
    string PathString = "";
    int *dist = new int[MaxSize];                       //存起点到各点最短距离
    int *st = new int[MaxSize];                         //记录个点访问状态
    int *Path = new int[MaxSize];                       //存最短路径
    //初始化
    for(int i=0;i<MaxSize;i++) dist[i] = INF, st[i] = 0;
    dist[s] = 0;                                        //起点距离为0
    //dijk
    for(int i=1;i<=vertexNum;i++){
        int k = -1;
        for(int j=1;j<=vertexNum;j++){
            if(!st[j] && (k==-1 || dist[k]>dist[j])){   //找到未访问过且距离最近的点
                k = j;
            }
        }
        st[k] = 1;                                      //标记为已访问
        for(int j=1;j<=vertexNum;j++){                  //遍历所有点更新距离
            if(dist[j]>dist[k]+arc[k][j]){
                dist[j]=dist[k]+arc[k][j];              //若该点存在与k连接的边且距离更短，更新该点与起点的最短距离
                Path[j] = k;                            //记录下最短路径
            }
        }
    }

    pathCnt = dist[t];

    if(pathCnt!=INF){                               //存在最短路
        int *stack = new int[vertexNum+10];
        int top = -1;
        for(int i=t;i!=s;i=Path[i]) stack[++top]=i;         //取出最短路径
        PathString += vertex[s];
        while(top>=0){                                  //输出最短路径
            PathString += "->";
            PathString += vertex[stack[top--]];
        }
    }

    //析构
    delete[] dist;
    delete[] st;
    delete[] Path;

    return (pathCnt==INF)?"无":PathString;
}

#endif // ADMGRAPH_H
