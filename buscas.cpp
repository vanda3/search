#include <iostream>
#include <cstdlib>
#include <queue>
#include <list>
#include <stack>
#include <stdio.h>
#include <cstdio>
#include <ctime>

using namespace std;

////////////////////////// Nodes /////////////////////////////

struct Node {
	int state[4][4];
	Node *pai; 
	int depth;
    int cost;
	char move;
};


typedef pair<int, int> pos;
typedef pair<int, Node*> hn;
int answer, limit=0, heur=0, conff[4][4]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,0};
bool finalState = false;

list<char> path;
priority_queue<hn> pq;
stack <Node*> s;
queue<Node*> q;
pos zero;

clock_t start;
double duration;

bool compare(int a[4][4], int b[4][4]) { //Verifica se duas matrizes são iguais
	for (int i=0; i<4; i++) {
		for (int j=0; j<4;j++) {
			if (a[i][j] != b[i][j])
				return false;
		}
	}	
	return true; 
}

bool visited(Node* a) { //Verifica se nó foi visitado
    Node* parent = a->pai;
    while(parent != NULL) {
        if(compare(a->state, parent->state)) return true;
    parent = parent->pai;
  }
  
  return false;
}


//////////  Função auxiliar para mover o 0 de lugar

void mover(Node* a, int i, int j, char m){
    int k, l;
    if(m=='u'){
        k=i-1;
        l=j;
    }
    else if(m=='l'){
        k=i;
        l=j-1;
    }
    else if(m=='r'){
        k=i;
        l=j+1;
    }
    else{
        k=i+1;
        l=j;
    }
    a->state[i][j] = a->state[k][l];
    a->state[k][l] = 0;
}


//////////  Função auxiliar para preencher as matrizes

void fill(Node* a, Node* b) {
  for(int i = 0; i < 4; i++) {
    for(int j = 0; j < 4; j++) {
      a->state[i][j] = b->state[i][j];;
    }
  }
}



//////////  Descobrir a posição do 0

pos find_x(int tab[4][4],int val) {
    pos x;
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            if(tab[i][j] == val) {
                x = pos(i, j);
                break;
            }
    return x;
}


//////////   Heurística

int heuristic(Node* n){
    int c=0;
    if(heur==1){ // Contar nr pecas em posicoes erradas
        for(int i = 0; i<4; i++)
	       for(int j =0; j<4; j++)
		      if(n->state[i][j]!=conff[i][j]) 
                  c++;
    }
    if(heur==2){ // Manhattan Distance
        for(int i = 0; i<4; i++) {
            for(int j = 0; j<4; j++) {
                if(n->state[i][j] != 0) {
                    pos val=find_x(conff, n->state[i][j]); //procura cur->state[i][j] na solucao
	                c += abs(val.first-i) + abs(val.second-j);
                }
            }
        }
    }
    return c;
}

//////////   Criar os nós descendentes

void createChild(Node* cur) {
    pos zero = find_x(cur->state, 0);
    int i = zero.first, j = zero.second;
  
    Node* nodeup = new Node();
    Node* nodelft = new Node();
    Node* nodert = new Node();
    Node* nodedwn = new Node();
    
    fill(nodeup, cur); 
    fill(nodelft, cur); 
    fill(nodert, cur); 
    fill(nodedwn, cur);
    
    nodeup->pai = cur; 
    nodelft->pai = cur; 
    nodert->pai = cur; 
    nodedwn->pai = cur;
    
    nodeup->depth = cur->depth + 1; 
    nodelft->depth = cur->depth + 1; 
    nodert->depth = cur->depth + 1; 
    nodedwn->depth = cur->depth + 1;
  
    //a* cost-depth
    //greedy cost
    if(i != 0) {
        mover(nodeup, i, j, 'u');
        nodeup->move = 'u';
        if(answer==1){ 
            nodeup->cost=heuristic(nodeup);
            pq.push(hn(-nodeup->cost - nodeup->depth, nodeup));
        }
        else if(answer==2){
            nodeup->cost = heuristic(nodeup);
            pq.push(hn(-nodeup->cost, nodeup));
        }
        else{
            if(!visited(nodeup)){
                if(answer == 3 || answer == 5){
                    s.push(nodeup);
                }
                else if (answer ==4)
                    q.push(nodeup);
            }
        }
    }
    if(i != 3) {
        mover(nodedwn, i, j, 'd');
        nodedwn->move = 'd';
        if(answer==1){
            nodedwn->cost=heuristic(nodedwn);
            pq.push(hn(-nodedwn->cost - nodedwn->depth, nodedwn));
        }
        else if(answer==2){
            nodedwn->cost = heuristic(nodedwn);
            pq.push(hn(-nodedwn->cost, nodedwn));
        }
        else{
            if(!visited(nodedwn)){
                if(answer == 3 || answer == 5){
                    s.push(nodedwn);
                }
                else if (answer ==4)
                    q.push(nodedwn);
            }
        }
    }
    if(j != 0) {
        mover(nodelft, i, j, 'l');
        nodelft->move = 'l';
        if(answer==1){
            nodelft->cost=heuristic(nodelft);
            pq.push(hn(-nodelft->cost - nodelft->depth, nodelft));
        }
        else if(answer==2){
            nodelft->cost = heuristic(nodelft);
            pq.push(hn(-nodelft->cost, nodelft));
        }
        else{
            if(!visited(nodelft)){
                if(answer == 3 || answer == 5){
                    s.push(nodelft);
                }
                else if (answer ==4)
                    q.push(nodelft);
            }
        }
    }
    if(j != 3) {
        mover(nodert, i, j, 'r');
        nodert->move = 'r';
        if(answer==1){
            nodert->cost=heuristic(nodert);
            pq.push(hn(-nodert->cost - nodert->depth, nodert));
        }
        else if(answer==2){
            nodert->cost = heuristic(nodert);
            pq.push(hn(-nodert->cost, nodert));
        }
        else{
            if(!visited(nodert)){
                if(answer == 3 || answer == 5){
                    s.push(nodert);
                }
                else if (answer ==4)
                    q.push(nodert);
            }
        }
    }
}


//////////  Caminho desde o nó corrente até ao primeiro nó

void getPath(Node* cur) {
    cout << "Imprimir caminho para solução?" << endl;
    cout << "1) Sim" << endl;
    cout << "2) Não" << endl;
    int x;
    scanf("%d", &x);
    Node* parent = cur;
    while(parent != NULL) {
        path.push_front(parent->move);
        if(x == 1)
            cout << parent->move << ' ';
        parent = parent->pai;
    }
    cout << endl;
    return;
}



///////////////////////// Auxiliares /////////////////////////

////////// Testa se o input dado chega a uma configuração final possível

bool solvable(int conf[]){
    int blankRow=zero.first;
    int parity = 0;
    for (int i = 0; i < 15; i++){
        for (int j = i + 1; j < 16; j++){
            if (conf[i] > conf[j] && conf[j] != 0)
                parity++;
        }
    }
    if (blankRow % 2 == 1) // zero em linha impar a contar de baixo (1,2,3,4) == zero linha impar a contar de cima (0,1,2,3)
        return parity % 2 == 0;
    else // zero em linha par
        return parity % 2 != 0;
}



//////////////////////// Algoritmos //////////////////////////

///////////// A* e Greedy

int solution;
void informedSearch(int confi[4][4]){
    int nodes = 0;
    Node* root = new Node();
    Node* cur = new Node();
    root->pai = NULL;
    root->move = 'x'; //não há mov inicial
    root->depth = 0;
    
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            root->state[i][j] = confi[i][j];;
        }
    } 
    
    root->cost=heuristic(root);
    pq.push(hn(-root->cost,root));
    
    while(!pq.empty()){
        hn top = pq.top(); 
        cur = top.second;
        pq.pop();
        nodes++;
        if(compare(cur->state, conff)) {
            solution = cur->depth;
            getPath(cur);
            cout << "A solucao foi encontrada em " << solution << " jogadas." << endl;
            cout << "Nós visitados: " << nodes << endl;
            return;
        }
        createChild(cur);
    }
    cout << endl;
    
    
}

//////////////////     BFS DFS IDFS

void uninformedSearch(int confi[4][4]){
    int visited_nodes = 0;
    int ndepth=0;
    Node* removed = new Node();

//////////////////     DFS     //////////////////

    if (answer == 3) {
        while(!s.empty()) {
            removed = s.top();
            s.pop();            
            visited_nodes++;
           
            if (compare(removed->state, conff)) {
                ndepth = removed->depth;
                getPath(removed);
                cout << "A solucao foi encontrada em " << ndepth << " jogadas." << endl;
                cout << "Nós visitados: " << visited_nodes << endl;
                return;
            }
            createChild(removed);

        }
    }

//////////////////     BFS     //////////////////    

    else if (answer == 4) {
         while(!q.empty()) {
            removed = q.front();
            q.pop();              
            visited_nodes++;
           
            if (compare(removed->state, conff)) {
                ndepth = removed->depth;
                getPath(removed);
                finalState = true;
                cout << "A solucao foi encontrada em " << ndepth << " jogadas." << endl;
                cout << "Nós visitados: " << visited_nodes << endl;
                return;
            }
            createChild(removed);
        }
    }
}

//////////////////     IDFS     //////////////////  

Node* rem = new Node();
void iterative_dfs(int maxDepth) {
    int visited_nodes = 0;

     while(!s.empty()) {
        rem = s.top();
        s.pop();             
        visited_nodes++;
       
        if (compare(rem->state, conff)) {
            getPath(rem);
            finalState = true;
            cout << "A solucao foi encontrada em " << maxDepth << " jogadas." << endl;
            cout << "Nós visitados: " << visited_nodes << endl;
            return;
        }
        if (rem->depth < maxDepth) {
            createChild(rem);
        }
    }
}


void idfs(Node* nod) {
    for (int maxDepth = 1; finalState == false; maxDepth++) {
        s.push(nod);
        iterative_dfs(maxDepth);
    }
}



int main (){
    int i, j, k, flag;
    int confi[4][4]={1,2,3,4,5,6,8,12,13,9,0,7,14,11,10,15};
    int veci[16], vecf[16];
    
    Node* initial = new Node();
    initial->pai = NULL;
    initial->move = 'x';   // so para preencher
    initial->depth = 0;
    initial->cost = 0;
    cout << endl;
    cout << "Bem-vindo ao jogo dos 15!" << endl;
    cout << endl;
    cout << endl;
    cout << "Que algoritmo deseja usar?" << endl;
    cout << "1) A*" << endl;
    cout << "2) Greedy" << endl;
    cout << "3) Busca em profundidade" << endl;
    cout << "4) Busca em largura" << endl;
    cout << "5) Busca em profundidade iterativa" << endl;
    
    scanf("%d", &answer);
    
    cout << endl;
    
    if(answer!=1 && answer!=2 && answer!=3 && answer!=4 && answer!=5){
        cout << "Resposta invalida!" << endl;
        return 0;
    }
    
    cout << "Deseja usar as configurações iniciais de teste?" <<endl;
    cout << "1) Sim" << endl;
    cout << "2) Não" << endl;
    scanf("%d", &flag);
    
    cout << endl;
    
    if (flag == 2)
        cout << "Insira a conf. inicial:" << endl;
    
    int num;
    
    k=0;
    for(i=0; i<4; i++){
        for(j=0; j<4; j++){
            if (flag ==2){
                scanf("%d", &num);
                confi[i][j]=num;
            }
            veci[k]=confi[i][j];
            initial->state[i][j] = confi[i][j];
            k++;
        }
    }
    
    cout << endl;
    zero=find_x(confi,0);
    
    if(!solvable(veci))
        cout << "Nao tem solucao!" << endl;
    else{
        if (flag == 2)
            cout << "Insira a conf. final:" << endl;
        k=0;
        for(i=0; i<4; i++){
            for(j=0; j<4; j++){
                if (flag == 2){
                    scanf("%d", &num);
                    conff[i][j]=num;
                }
                vecf[k]=conff[i][j];
                k++;
            }
        }
       cout << endl;
        if(answer == 1 || answer == 2){
            cout << "Qual heuristica pretende usar?" << endl;
            cout << "1) Contar pecas" << endl;
            cout << "2) Manhattan distance" << endl;
            scanf("%d", &heur);
            cout << endl;
            
            start = clock(); //clock            
            informedSearch(confi);
            duration = (clock() - start) / (double)CLOCKS_PER_SEC;
        } 
        else{
            if (answer ==3){
                start = clock(); //clock
                s.push(initial);
                uninformedSearch(confi);
                duration = (clock() - start) / (double)CLOCKS_PER_SEC;
            }
            else if (answer == 4){
                start = clock(); //clock
                q.push(initial);
                uninformedSearch(confi);
                duration = (clock() - start) / (double)CLOCKS_PER_SEC;
            }
            else if (answer == 5){
                start = clock(); //clock
                idfs(initial);
                duration = (clock() - start) / (double)CLOCKS_PER_SEC;
            }
        }
        cout << "Tempo de execução: " << duration << " segundos" << endl;
    }
    return 0;
}