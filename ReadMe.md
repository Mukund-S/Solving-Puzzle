# Solving Puzzle

## Enter the following command at the location of the file:

> $g++ -std=c++11 SolvingPuzzle.cpp -o SolvingPuzzle

> $./SolvingPuzzle

---

---

## Once the above command executes, enter the input values as follows:

> problem.txt method tmax solution.txt

1. problem.txt => name of the output file with extension
2. method => name of the method where both lower and upper case is allowed
   - BFS (Breadth First Search)
   - DFS (Depth First Search)
   - AStarH0 (A Star Algorithm with 0 heursitics)
   - AStarH1 (A Star Algorithm with misplaced tiles)
   - AStarH2 (A Star Algorithm with manhattan distance)
   - AStarH3 (A Star Algorithm with euclidean distance)
     **Implemented A Star with Euclidean Distance for extra credits**
3. tmax => upper bound on the runtime (in seconds)
4. solution.txt => name of the output file with extension

---

---

## Example:

> sample.txt AStarH3 120 file.txt

---

---

## The output file is created as per name requested by user and it contains all the positions of zero when it moves UP, Down, Left or Right, or even when it backtracks it gets printed as a check.
