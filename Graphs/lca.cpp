#include<bits/stdc++.h>
using namespace std;

#define MAXV 2100000
#define MAXLOG 23
#define LINHA 1010

vector<int> v[MAXV]; // lista de adj

int st[MAXV][MAXLOG]; // sparse table (x,i) guardando ancestral de ordem 2^i de x
int lvl[MAXV]; // lvl=profundidade de cada no

void init_dfs(int x, int pai, bool jafoi[]){
    jafoi[x] = true; // marcando vertice como visitado
    st[x][0] = pai; // o ancestral de ordem 2^0 (1) de x eh o pai dele
    // se tiver pai valido (!= raiz), a prof aumenta em 1
    if(pai != -1) lvl[x] = lvl[pai]+1;

    for(int i = 0; i < v[x].size(); ++i) // visitar todos adjacentes
        if(!jafoi[v[x][i]]) init_dfs(v[x][i], x); // passo recursivo
}

void init_st(){
    // processo cada nivel de cada vertice
    for(int x = 1; x < MAXLOG; ++x) // 2^0 ja foi calculado, calculo pro resto
        for(int i = 0; i < MAXV; ++i){ // pra cada vertice
            // olho o ancestral 2^(i-1) do meu 2^(i-1), achando entao o meu 2^i
            st[i][x] = st[st[i][x-1]][x-1];
    }
}

void init(int root, int n){ // funcao init, raiz e #nos
    bool jafoi = new bool[n]; // visitados
    init_dfs(root, -1, jafoi); // calcular os ancestrais imediatos
    delete[] jafoi;
    init_st(); // init da sparse table
}


int lca(int x, int y){ // lca de x e y
    if(lvl[x] < lvl[y]) swap(x, y); // quero q x seja mais profundo q y

    int falta_subir = (lvl[x] - lvl[y]); // igualo as profundidades
    // simples representacao binaria de (falta_subir)
    for(int i = MAXLOG-1; i >= 0; --i){ // encontro os bits para representar o
        if((1<<i) <= falta_subir){ //      numero, dos mais signif. para os menos
            falta_subir -= (1<<i);
            x = st[x][i];
        }
    }

    if(x == y) return x; // ocorre quando x ta numa subarvore de y

    // acho o ponto abaixo do encontro (LCA)
    // se eu tentar subir 2^i e eles ja estiverem juntos, nao subo
    // tento entao subir 2^(i-1)
    for(int i = MAXLOG-1; i >= 0; --i){
        if(st[x][i] != st[y][i]){ // se continuarem diferentes, subo
            x = st[x][i]; // subindo pra x
            y = st[y][i]; // subindo pra y
        }
    }

    return st[x][0]; // retornando o ponto de encontro
}
