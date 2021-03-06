#include <algorithm>
#include <iostream>
#include <vector>
#include <cstdio>
#include <cmath>
using namespace std;

typedef vector<vector<long double>> matrix;
typedef vector<long double> Column;
typedef vector<long double> Row;
typedef vector<Row> Matrix;
const int PRECISION = 8;
const int PRECISION2 = 18;
const long double VeryBigNumber = 10000000000;
const long double SmallNumber = 1E-3;

struct Equation {
    Equation(const Matrix &a, const Column &b):
        a(a),
        b(b)
    {}

    Matrix a;
    Column b;
};

struct Position {
    Position(int column, int row):
        column(column),
        row(row)
    {}

    int column;
    int row;
};

Equation ReadEquation() {
    int size;
    std::cin >> size;
    Matrix a(size, std::vector <long double> (size, 0.0));
    Column b(size, 0.0);
    for (int row = 0; row < size; ++row) {
        for (int column = 0; column < size; ++column)
            std::cin >> a[row][column];
        std::cin >> b[row];
    }
    return Equation(a, b);
}

Position SelectPivotElement(
  const Matrix &a, 
  std::vector <bool> &used_rows, 
  std::vector <bool> &used_columns) {
    // This algorithm selects the first free element.
    // You'll need to improve it to pass the problem.
    int size=a.size();
	Position pivot_element(0, 0);
	//cout<<"pivot_element.row:"<<pivot_element.row<<", "<<"pivot_element.column:" <<pivot_element.column<<endl;    
	
	while ((used_rows[pivot_element.row]) && (pivot_element.row < (size-1))){
        ++pivot_element.row;
		//cout<<"pivot_element.row:"<<pivot_element.row<<", "<<"pivot_element.column:" <<pivot_element.column<<endl;
	}
	
    while ((used_columns[pivot_element.column]) && (pivot_element.column < (size-1))){
        ++pivot_element.column;
		//cout<<"pivot_element.row:"<<pivot_element.row<<", "<<"pivot_element.column:" <<pivot_element.column<<endl;
	}
	
	int pivotRow = pivot_element.row;
	long double maxpivot = abs(a[pivot_element.row][pivot_element.column]);
	
	for(int i=(pivot_element.row + 1); i < size; ++i){ // skip the row with potential pivot element equal to zero
		if (abs(a[i][pivot_element.column]) > maxpivot ){
			maxpivot = abs(a[i][pivot_element.column]); //the pivot to have largest absolute value among unused rows
			pivotRow = i;
		}			
	}
	
	pivot_element.row = pivotRow;
    //cout<<"pivot_element:(" <<pivot_element.row<<","<<pivot_element.column<<")"<<endl;
	return pivot_element;
}

void SwapLines(Matrix &a, Column &b, std::vector <bool> &used_rows, Position &pivot_element) {
    std::swap(a[pivot_element.column], a[pivot_element.row]); // pivot_element.column denotes the pivot row in the line to be processed
    std::swap(b[pivot_element.column], b[pivot_element.row]);
    std::swap(used_rows[pivot_element.column], used_rows[pivot_element.row]);
    pivot_element.row = pivot_element.column;
}

void ProcessPivotElement(Matrix &a, Column &b, const Position &pivot_element) {
    // Write your code here
	int size = a.size();
	//for(int i = pivot_element.column; i < size; ++i)
	//	a[pivot_element.row][i]/=a[pivot_element.row][pivot_element.column];
	
	//b[pivot_element.row]/=a[pivot_element.row][pivot_element.column];
	
	for(int i = pivot_element.row+1; i < size; ++i){
		long double multiplier = 0;
		if(a[i][pivot_element.column]!=0 ){ //avoid dividing zero later on (no need for elimination if the coefficient is already zero)
			multiplier = -a[i][pivot_element.column]/a[pivot_element.row][pivot_element.column];
			a[i][pivot_element.column] = 0; // this doesn't matter b/c it will not be used
			for(int j = pivot_element.column+1; j < size; ++j){			
				a[i][j] = a[i][j] + multiplier * a[pivot_element.row][j];
			}
			b[i] =  b[i] + multiplier * b[pivot_element.row];
		}		
	}			
}

void CalculateSolution(Matrix &a, Column &b){ //backtrace the solution from bottom
	int size = a.size();
	if ((size > 0) && (a[size-1][size-1]!=0)){ 
		b[size-1]/=a[size-1][size-1];
		if(size>1)
			for(int i = size - 2; i >= 0; --i){
				for(int j = size - 1; j > i; --j)
					b[i] -= a[i][j]*b[j];
				b[i] /=a[i][i];		
			}
	}
}

void MarkPivotElementUsed(const Position &pivot_element, std::vector <bool> &used_rows, std::vector <bool> &used_columns) {
    used_rows[pivot_element.row] = true;
    used_columns[pivot_element.column] = true;
}

void PrintMatrix(const Matrix &a) {
    int row_size = a.size();
	int column_size = a[0].size();
	cout<<"************"<<endl;
    std::cout.precision(PRECISION);
	cout<<fixed;
    for (int row = 0; row < row_size; ++row){
		for(int column = 0; column < column_size; ++column)
			cout <<a[row][column]<<"\t";
		cout<<endl;
	}
	cout<<"************"<<endl;
}

void PrintColumn(const Column &column) {
    int size = column.size();
    std::cout.precision(PRECISION);
	cout<<fixed;
    for (int row = 0; row < size; ++row)
        std::cout <<column[row] << std::endl;
}

pair<int, Column> SolveEquation(Equation equation) {
    Matrix &a = equation.a;
    Column &b = equation.b;
    int size = a.size();

    std::vector <bool> used_columns(size, false);
    std::vector <bool> used_rows(size, false);
    for (int step = 0; step < size; ++step) {
		//cout<<"step:"<<step<<endl;
        Position pivot_element = SelectPivotElement(a, used_rows, used_columns);
		//cout<<"pivot:"<<"a["<<pivot_element.row<<"]["<<pivot_element.column<<"]"<<endl;
        SwapLines(a, b, used_rows, pivot_element);
		//cout<<"matrix after SwapLines:"<<endl;
		//PrintMatrix(a);
		if(a[pivot_element.row][pivot_element.column]!=0){ // if there is a solution
			ProcessPivotElement(a, b, pivot_element);
			//cout<<"matrix after ProcessPivotElement:"<<endl;
			//PrintMatrix(a);
			MarkPivotElementUsed(pivot_element, used_rows, used_columns);
		}else
			return {-1, {}};
    }
	
	//PrintMatrix(a);
	
	CalculateSolution(a, b);	
	
	
    return {0, b};
}


long double DotMultiply(const Column & column, const Row & row){
	long double sum=0;
	int size = column.size();
	for (int i=0; i<size; ++i)
		sum+=column[i]*row[i];
	return sum;
}



pair<int, vector<long double>> solve_diet_problem(
    int n, 
    int m, 
    matrix A, 
    vector<long double> b, 
    vector<long double> c) {

  // Write your code here
	long double maxPleasure;
	int maxSubset;
	int matrixNum;
	//PrintSubset(FindSubset(n));
	
	//cout<<"matrix A before expansion:"<<endl;
	//PrintMatrix(A);
	
	
	//matrix A2(m,vector<long double>(m,0));

	
	//for(int i=0; i<m; ++i) // m is the number of foods, n is the number of (explicit)inequalities
	//	A2[i][i]=-1; // -1 is used to reverse the "greater than" 0 sign to "no greater than"
	
	//cout<<"matrix A2:"<<endl;
	//PrintMatrix(A2);
	
	
	//A.insert(A.end(), A2.begin(), A2.end()); // to union the two equal-dimention vectors
	
	//vector<long double> bigRow(m, 1); // for the additional equation to determine infinity
	
	//A.push_back(bigRow); // add the additional row to A
	
	//for(int i=0; i<m; ++i) // m is the number of foods, n is the number of (explicit)inequalities
	//	b.push_back(0);   // to enforce amounti >=0
	//b.push_back(VeryBigNumber); // RHS of last row
	
	for(int i =0; i<b.size(); ++i )
		b[i] /= 4916.666666666667; // 29.5e4*100/6000 = 4916.666666666667

	cout<<"matrix A after expansion:"<<endl;
	PrintMatrix(A);
	cout<<"expanded RHS"<<endl;
	PrintColumn(b);

	Equation equation(A, b);
	pair<int, Column> solution = SolveEquation(equation);
	
/* 	cout<<"solution: ";		
	cout.precision(PRECISION);
	cout<<fixed;
	for(auto ele : solution.second)
		cout<<ele<<", ";
	cout<<endl;	 */
  

    return solution;
}

int main(){
  int n, m;
  cin >> n >> m;
  matrix A(n, vector<long double>(m));
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      cin >> A[i][j];
    }
  }
  vector<long double> b(n);
  for (int i = 0; i < n; i++) {
    cin >> b[i];
  }
  vector<long double> c(m);
  for (int i = 0; i < m; i++) {
    cin >> c[i];
  }

  pair<int, vector<long double>> ans = solve_diet_problem(n, m, A, b, c);

  switch (ans.first) {
    case -1: 
      printf("No solution\n");
      break;
    case 0: 
      printf("Bounded solution\n");
	  cout.precision(PRECISION2);
	  cout<<fixed;
      for (int i = 0; i < m; i++) {
        cout<<ans.second[i]<<" ";
		//printf("%.18f%c", ans.second[i], " \n"[i + 1 == m]);
      }
	  cout<<endl;
	  
      break;
    case 1:
      printf("Infinity\n");
      break;      
  }
  return 0;
}
