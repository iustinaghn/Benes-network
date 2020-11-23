#include <cstdio>
#include <cstring>
#include <vector>

using namespace std;

const int max_dim = 13;


int directP[1 << max_dim];
int invereP[1 << max_dim];


char switched[2 * max_dim - 1][1 << (max_dim - 1)];
char path[1 << max_dim];                          


int shuffle(int i, int n) { 
	return ((i & 1) << (n - 1)) | (i >> 1); 
}

void DFS(int idx, int route) {

  path[idx] = route;

  if (path[idx ^ 1] < 0)
  	DFS(idx ^ 1, route ^ 1);

  idx = directP[invereP[idx] ^ 1];
 
  if (path[idx] < 0)
  	DFS(idx, route ^ 1);
}


void benes_route(int n, int lvl_p, int directP_idx, const vector<int> &src, const vector<int> &dest) {

  int values, levels, i, j, x, s;
  vector<int> bottom1;
  vector<int> top1;
  

  if (n == 1) {
  	switched[lvl_p][directP_idx] = src[0] != dest[0]; 
  	return; 
  }
  
  
  values = 1 << n;
  levels = 2 * n - 1;
  
  vector<int> bottom2(values / 2);
  vector<int> top2(values / 2);


  for (i = 0; i < values; ++i)
  	invereP[src[i]] = i;
 
  for (i = 0; i < values; ++i)
  	directP[i] = invereP[dest[i]];
 
  for (i = 0; i < values; ++i)
  	invereP[directP[i]] = i;
 

  memset(path, -1, sizeof(path));
  for (i = 0; i < values; ++i)
  	if (path[i] < 0)
  		DFS(i, 0);
  

  for (i = 0; i < values; i += 2) {
    switched[lvl_p][directP_idx + i / 2] = path[i];
    for (j = 0; j < 2; ++j) {
      x = shuffle((i | j) ^ path[i], n);
      if (x < values / 2) bottom1.push_back(i | j); 
      else top1.push_back(i | j);
    }
  }

  for (i = 0; i < values; i += 2) {
    s = switched[lvl_p + levels - 1][directP_idx + i / 2] = path[directP[i]];
    for (j = 0; j < 2; ++j) {
      x = shuffle((i | j) ^ s, n);
      if (x < values / 2) bottom2[x] = directP[i | j]; 
      else top2[x - values / 2] = directP[i | j];
    }
  }

  benes_route(n - 1, lvl_p + 1, directP_idx, bottom1, bottom2);
  benes_route(n - 1, lvl_p + 1, directP_idx + values / 4, top1, top2);
}

int main() {

  int N, i, j, values, levels;
  bool first = true;

  while (scanf("%i", &N) == 1, N) {
    values = 1 << N;
    levels = 2 * N- 1;
    
    vector<int> dest(values);
    vector<int> src(values);
    
    for (i = 0; i < values; ++i)
    	src[i] = i;

    for (i = 0; i < values; ++i)
    	scanf("%i", &dest[i]);

    benes_route(N, 0, 0, src, dest);

    if (!first)
    	printf("\n");
    else first = false;

    for (i = 0; i < levels; ++i) {
      for (j = 0; j < values / 2; ++j) {
      	if (switched[i][j] == 0)
      		printf("S ");
      	else
      		printf("C ");
      }
      printf("\n");
    }
  }
  return 0;
}

