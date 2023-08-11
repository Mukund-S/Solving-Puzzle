//
//  Name :  Mukund Sharma
//  Date : 9th October 2022
//

#include <math.h>
#include <time.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

using namespace std;

int pos = 0;
vector<vector<int> > goalState;
vector<vector<int> > initial;
map<vector<vector<int> >, bool> visited;
string solution;
ofstream outputFile;
int tmax;
time_t start, stop;

class Node {
 public:
  // Declaring children for object and it also acts as open loop
  map<int, vector<vector<int> > > children;
  // Declaring puzzle vector for storing base case of puzzle
  vector<vector<int> > puzzle;
  // Constructor call with parameters
  Node(vector<vector<int> > _puzzle) { this->puzzle = _puzzle; }
  // find postiton of Zero
  void findZero(int arr[]);
  // Perform move up operation
  vector<vector<int> > moveUp();
  // Perform move down operation
  vector<vector<int> > moveDown();
  // Perform move Right operation
  vector<vector<int> > moveRight();
  // Perform move Left operation
  vector<vector<int> > moveLeft();
};

void Node::findZero(int arr[]) {
  vector<vector<int> >::iterator it;
  int z = 0, i = 0;
  for (it = puzzle.begin(); it != puzzle.end(); it++) {
    if (find(it->begin(), it->end(), 0) != it->end()) {
      auto val = find(it->begin(), it->end(), 0);
      z = (int)(val - it->begin());
      arr[0] = i;
      arr[1] = z;
      break;
    }
    i++;
  }
}

vector<vector<int> > Node::moveUp() {
  int arr[2];
  findZero(arr);
  vector<vector<int> > temp = puzzle;
  if (arr[0] > 0 && temp[arr[0] - 1][arr[1]] != -1)  // m=2
    std::swap(temp[arr[0]][arr[1]], temp[arr[0] - 1][arr[1]]);
  return temp;
}

vector<vector<int> > Node::moveDown() {
  int arr[2];
  findZero(arr);
  vector<vector<int> > temp = puzzle;
  if (arr[0] < puzzle.size() - 1 && temp[arr[0] + 1][arr[1]] != -1)  // m=2
    std::swap(temp[arr[0]][arr[1]], temp[arr[0] + 1][arr[1]]);
  return temp;
}

vector<vector<int> > Node::moveRight() {
  int arr[2];
  findZero(arr);
  vector<vector<int> > temp = puzzle;
  if (arr[1] < puzzle[arr[1]].size() - 1 &&
      temp[arr[0]][arr[1] + 1] != -1)  // m=2
    std::swap(temp[arr[0]][arr[1]], temp[arr[0]][arr[1] + 1]);
  return temp;
}

vector<vector<int> > Node::moveLeft() {
  int arr[2];
  findZero(arr);
  vector<vector<int> > temp = puzzle;
  if (arr[1] > 0 && temp[arr[0]][arr[1] - 1] != -1)  // m=2
    std::swap(temp[arr[0]][arr[1]], temp[arr[0]][arr[1] - 1]);
  return temp;
}

// Calculate Euclidean distance
int euclidean(vector<vector<int> > a) {
  int h = 0, val = 0;
  for (int i = 0; i < a.size(); i++) {
    for (int j = 0; j < a[i].size(); j++) {
      val = 0;
      if (a[i][j] != 0 && a[i][j] != goalState[i][j] && goalState[i][j] != -1) {
        for (int k = 0; k < a.size(); k++) {
          for (int l = 0; l < a[i].size(); l++) {
            if (a[i][j] == goalState[k][l]) {
              h += (int)sqrt(pow(i - k, 2) + pow(j - l, 2));
              val = 1;
              break;
            }
          }
          if (val == 1) break;
        }
      }
    }
  }
  return h;
}

// Calculate Manhattan Distance
int manhattan(vector<vector<int> > a) {
  int h = 0;
  for (int i = 0; i < a.size(); i++) {
    for (int j = 0; j < a[i].size(); j++) {
      if (a[i][j] != 0) {
        for (int k = 0; k < a.size(); k++) {
          for (int l = 0; l < a[i].size(); l++) {
            if (a[i][j] == goalState[k][l]) h += abs(i - k) + abs(j - l);
          }
        }
      }
    }
  }
  return h;
}
// Create structure in order to sort the priority queue in increasing order
struct cmp {
  bool operator()(pair<int, vector<vector<int> > > &a,
                  pair<int, vector<vector<int> > > &b) {
    return a.first > b.first;
  }
};

// Check if reached goal state
bool checkGoalReached(vector<vector<int> > a, vector<vector<int> > b) {
  for (int i = 0; i < a.size(); i++) {
    for (int j = 0; j < a[i].size(); j++) {
      if (a[i][j] != b[i][j]) return false;
    }
  }
  return true;
}
// Print the puzzle in order to check output
void printPuzzle(vector<vector<int> > v) {
  for (int i = 0; i < v.size(); i++) {
    for (int j = 0; j < v[i].size(); j++) outputFile << v[i][j] << " ";
    outputFile << "\n";
  }
}

// Calculate number of misplaced tiles for
int calMisplacedTiles(vector<vector<int> > a) {
  int h = 0;
  for (int i = 0; i < a.size(); i++) {
    for (int j = 0; j < a[i].size(); j++)
      if (a[i][j] != goalState[i][j]) h++;
  }
  return h;
}

// Calling move functions for operations
vector<vector<int> > optimizeMove(Node *currentNode, int flag) {
  if (flag == 0)
    return currentNode->moveUp();
  else if (flag == 1)
    return currentNode->moveLeft();
  else if (flag == 2)
    return currentNode->moveDown();
  else
    return currentNode->moveRight();
}

// Calling heuristic functions according to user request
int calculateHeuristics(vector<vector<int> > a, int flag) {
  if (flag == 0)
    return 0;
  else if (flag == 1)
    return calMisplacedTiles(a);
  else if (flag == 2)
    return manhattan(a);
  else
    return euclidean(a);
}

// Printing to file the postion of zero in the puzzle for moves performed
void findZeroValue(vector<vector<int> > temp) {
  for (int i = 0; i < temp.size(); i++)
    for (int j = 0; j < temp[i].size(); j++)
      if (temp[i][j] == 0) {
        outputFile << "Empty Tile Position : " << i << " " << j << "\n";
        return;
      }
}

// Printing to file the postion of zero in the puzzle for backtracking
void NfindZeroValue(vector<vector<int> > temp) {
  for (int i = 0; i < temp.size(); i++)
    for (int j = 0; j < temp[i].size(); j++)
      if (temp[i][j] == 0) {
        outputFile << "Check : " << i << " " << j << "\n";
        return;
      }
}

// Perform DFS operation
void DFS(vector<vector<int> > initial, vector<vector<int> > goalState) {
  stack<pair<int, vector<vector<int> > > > s;
  pair<int, vector<vector<int> > > m;
  m.first = pos;
  m.second = initial;
  Node *currentNode = new Node(initial);
  currentNode->children[pos++] = initial;
  s.push(m);
  visited[initial] = true;
  bool value = false;
  int flag = 0;
  vector<vector<int> > temp;
  while (!s.empty()) {
    flag = 0;
    for (int i = 0; i < 4; i++) {
      currentNode->children[pos] = optimizeMove(currentNode, i);
      if (visited[currentNode->children[pos]]) {
        currentNode->children.erase(pos);
      } else {
        temp = currentNode->children[pos];
        findZeroValue(temp);
        m.first = pos;
        m.second = temp;
        value = checkGoalReached(currentNode->children[pos], goalState);
        if (value == true) break;
        s.push(m);
        pos++;
        currentNode->puzzle = temp;
        visited[temp] = true;
        flag = 1;
        stop = time(NULL);
        if (stop - start > tmax) {
          cout << "Time Exceeded";
          exit(0);
        }
        break;
      }
    }
    if (value == true) break;
    if (flag == 1) continue;
    m = s.top();
    currentNode->puzzle = currentNode->children[m.first - 1];
    NfindZeroValue(currentNode->puzzle);
    s.pop();
  }
  if (value == true) {
    outputFile << "Final Puzzle:\n";
    printPuzzle(currentNode->children[pos]);
  } else {
    cout << "\nError\n";
  }
  return;
}

// Perform BFS operation
void BFS(vector<vector<int> > initial, vector<vector<int> > goalState) {
  queue<pair<int, vector<vector<int> > > > r;
  pair<int, vector<vector<int> > > m;
  m.first = pos;
  m.second = initial;
  Node *currentNode = new Node(initial);
  currentNode->children[pos++] = initial;
  r.push(m);
  visited[m.second] = true;
  bool value = false;
  int j = 0;
  vector<vector<int> > temp;
  while (!r.empty()) {
    for (int i = 0; i < 4; i++) {
      currentNode->children[pos] = optimizeMove(currentNode, i);
      if (visited[currentNode->children[pos]]) {
        currentNode->children.erase(pos);
      } else {
        temp = currentNode->children[pos];
        findZeroValue(temp);
        m.first = pos;
        m.second = temp;
        value = checkGoalReached(currentNode->children[pos], goalState);
        if (value == true) break;
        r.push(m);
        visited[temp] = true;
        pos++;
        stop = time(NULL);
        if (stop - start > tmax) {
          cout << "Time Exceeded";
          exit(0);
        }
      }
    }
    if (value == true) break;
    j++;
    currentNode->puzzle = currentNode->children[j];
    NfindZeroValue(currentNode->puzzle);
    r.pop();
  }

  if (value == true) {
    outputFile << "Final Puzzle:\n";
    printPuzzle(currentNode->children[pos]);
  } else {
    cout << "\nError\n";
  }
}

// Perform AStar with 4 different heuristics operation on the basis of flag
// value
void A_Star_Algorithm(vector<vector<int> > a, vector<vector<int> > goal,
                      int flag) {
  int moves = calculateHeuristics(a, flag);
  pair<int, vector<vector<int> > > m;
  vector<vector<int> > temp;
  // Creating closed loop
  priority_queue<pair<int, vector<vector<int> > >,
                 vector<pair<int, vector<vector<int> > > >, cmp>
      Q;  // Priority queue named "Q".
  m.first = moves;
  m.second = a;
  Q.push(m);
  // Creating object for class
  Node *currentNode = new Node(a);
  bool value = false;
  if (m.second == goal) {
    findZeroValue(m.second);
    return;
  }
  while (!Q.empty()) {
    m = Q.top();
    Q.pop();
    moves = m.first;
    visited[m.second] = true;
    currentNode->puzzle = m.second;
    NfindZeroValue(currentNode->puzzle);
    for (int i = 0; i < 4; i++) {
      currentNode->children[pos] = optimizeMove(currentNode, i);
      if (visited[currentNode->children[pos]]) {
        currentNode->children.erase(pos);
      } else {
        temp = currentNode->children[pos];
        findZeroValue(temp);
        moves = calculateHeuristics(a, flag);
        m.first = moves;
        m.second = temp;
        if (checkGoalReached(currentNode->children[pos], goal)) {
          value = true;
          break;
        }
        Q.push(m);
        pos++;
        stop = time(NULL);
        if (stop - start > tmax) {
          cout << "Time Exceeded";
          exit(0);
        }
      }
    }
    if (value == true) {
      break;
    }
  }
  if (value == true) {
    outputFile << "Final Puzzle:\n";
    printPuzzle(currentNode->children[pos]);
  } else {
    cout << "\nError\n";
  }
  return;
}

int main() {
  int x, y;
  string line, problem, method, z;
  cout << "Solving Puzzle ";
  cin >> problem;
  cin >> method;
  cin >> tmax;
  cin >> solution;
  // problem.txt method tmax solution.txt
  // sample1.txt astarh0 120 file.txt
  ifstream myfile(problem);
  if (myfile.is_open()) {
    myfile >> x >> y;
    vector<int> v1, v2;
    getline(myfile, line);
    getline(myfile, line);
    getline(myfile, line);
    stringstream ss(line);
    for (int i = 0; i < x; i++) {
      for (int j = 0; j < y; j++) {
        ss >> z;
        v1.push_back(stoi(z));
      }
      initial.push_back(v1);
      v1.clear();
    }
    getline(myfile, line);
    getline(myfile, line);
    stringstream gg(line);
    for (int i = 0; i < x; i++) {
      for (int j = 0; j < y; j++) {
        gg >> z;
        v2.push_back(stoi(z));
      }
      goalState.push_back(v2);
      v2.clear();
    }
  }

  outputFile.open(solution);
  start = time(NULL);
  outputFile << "Initial Puzzle:\n";
  printPuzzle(initial);
  outputFile << "Moves Performed to reach Goal via " << method << ":\n";
  char a;
  for (int i = 0; i < method.size(); i++) {
    a = tolower(method[i]);
    method[i] = a;
  }
  if (method == "bfs")
    BFS(initial, goalState);
  else if (method == "dfs")
    DFS(initial, goalState);
  else if (method == "astarh0")
    A_Star_Algorithm(initial, goalState, 0);
  else if (method == "astarh1")
    A_Star_Algorithm(initial, goalState, 1);
  else if (method == "astarh2")
    A_Star_Algorithm(initial, goalState, 2);
  else if (method == "astarh3")
    A_Star_Algorithm(initial, goalState, 3);
  else
    cout << "Invalid Method Name";
  outputFile.close();
  return 0;
}
