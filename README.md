• Problem Statement:
Implement a multi-threaded matrix multiplication program,
there are three cases:
1. A thread per matrix.
2. A thread per row.
3. A thread per element.
• Organization:
The program is composed of 7 important function including
the main function:
1. void Take_Input(char *ar[]).
2. void *CaseA().
3. void *CaseB(void *arg).
4. void *CaseC(void *arg).
5. Void print_Output(double mat[][MAX_LENGTH],char
*methodName, __u_long t, __u_long ts).
6. void free_Resources(matrix_minimized *m,
matrix_minimized **m3).
7. int main(int argc, char *argv[]).• Functions’ Description:
1. void Take_Input(char *ar[]):
This function takes the name of the files that contains the
input arrays, then store the array elements and number of
rows and columns in global variables.
2. void *CaseA():
This function implements matrix multiplication, which is
used by a single thread to compute the whole new matrix.
3. void *CaseB(void *arg):
this function implement matrix multiplication, which is used
by number of threads, where each thread compute a single
row in the new matrix. The parameter includes a struct
contains the row number.
4. void *CaseC(void *arg):
This function implements matrix multiplication, used by
multiple threads, where each thread computes each single
element in the new matrix. The parameter includes a struct
contains the row and column number.
5. Void print_Output(double mat[][MAX_LENGTH],char
*methodName, __u_long t, __u_long ts):
This method is responsible for printing the content of the new
matrix produced from each method(by matrix, by row, byelement), as well as the number of rows , columns , threads
and execution time, it takes the matrix as a parameter ,
method name and the execution time.
6. void free_Resources(matrix_minimized *m,
matrix_minimized **m3):
This method is used to free the resources located in the
dynamic heap by the structs.
7. int main(int argc, char *argv[]):
The main function contains the thread creation and calling all
the above functions.
• How to compile and run code:
1. Open terminal and cd to project file.
2. Write “make” in the terminal.
3. Write “./matMultp a b c” where “a” and “b” are the
files that contain the input arrays, while “c” is the
output file.
