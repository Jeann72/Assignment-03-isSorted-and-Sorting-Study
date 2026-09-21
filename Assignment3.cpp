#include <algorithm> //swap,shuffle,reverse,copy
#include <chrono> // timing
#include <cstdio> //printf
#include <cstdlib> //exit
#include <numeric> 
#include <random>
#include <string>
#include<ratio> //iota

using namespace std;

bool isSorted(const int*a, int n)
{
    //start at index 1, compare each pair
    for(int i=1 ; i < n; i++)
        {
            //found 
            if(a[i] < a[i-1])
            {
                //out of order = not sorted
                return false;
            }
        }
        //all pairs in order = sorted
        return true;
}
 //swap adjacent out-of-order pairs, repeat
void bubbleSort(int* a, int n)
{
    //each pass bubbles one max to end
    for(int i= 0;i < n-1 ; i++)
        {
            //track if any swap happened
            bool swapped= false;

            //skip last i sorted elements
            for(int j= 0;j < n-1-i ; j++)
                {
                    //fix adjacent pair
                    if(a[j] > a[j+1])
                    {
                        swap(a[j], a[j+1]);
                        swapped = true;
                    }
  
                }
                //no swaps = already sorted
                if(!swapped) break;
        }
}

void selectionSort(int*a, int n)
{
    //position to fill
    for(int i= 0;i < n-1 ; i++)
        {
            //current is min
            int min= i;

            for(int j= i+1; j < n; j++)
                {
                    //find real min
                    if(a[j] < a[min])
                    {
	                    min = j ;
                    }
                }
            if(min != i)
            {
                swap(a[i], a[min]);
            }
         }
}

//insert each element into sorted prefix
void insertionSort(int*a, int n)
{
    //element to insert
    for(int i= 1; i < n; i++)
        {
            //save key
            int key= a[i];
            //scan left
            int j= i-1;
            
            while(j >=0 && a[j] > key)
                  {
                      //shift larger elements to the right
                      a[j+1]= a[j];
                        j--;
                  }
                  //place key
                  a[j+1]= key;
        }
}

//insert element into sorted prefix
void quickSort(int* a, int low, int hi)
{
    if(low >= hi)
    {
        //0 or 1 element,done
        return;
    }

    //pivot=middle
    int pivot = a[(low+hi)/2];
    //2 pointers
    int i = low;
    int j =hi;

    //loop 
    while(i <= j)
        {
            //skip small values
            while(a[i] < pivot) i++;
            //skip large values
            while(a[j] > pivot) j--;
            if(i <= j)
            {
                //swap and advance
                swap(a[i],a[j]);
                i++;
                j--;
            }
            
        }
        //sort left half
        quickSort(a,low,j);
        //sort right haf
        quickSort(a,i,hi);

}

//function pointer type for sorts
using Fn=void(*)(int*,int);

//copy, sort, time, and verify; return ms
double bench(Fn f, const int* src, int n, const char* name)
{
    //working copy
    int* a = new int[n];
    //copy input
    copy(src, src+n , a);
    //elapsed time(ms)
    auto t0= chrono::steady_clock::now();
    //runs sort
    f(a , n);
    auto ms= chrono::duration <double,milli>(chrono::steady_clock::now() - t0).count();
    //check result
    bool okay = isSorted(a,n);
    //free
    delete[] a;
    //stop at failure
    if(!okay)
    {
        //identify failing sort
        printf("Sort Error in %s\n", name);
        puts("SORT ERROR");
        exit(1);
    }
    //return time
    return ms;
    
}

int main()
{
    //3 sizes
    const int sizes[]={100,500,20000};
    Fn fns[]=
    {
        bubbleSort,
         selectionSort,
          insertionSort,
          [](int* a,int n) { quickSort(a, 0, n-1); }
    };
    
    mt19937 rng(47);
    
    //header
    printf("%8s %8s | %10s %10s %10s %10s\n ",
              "n","input","Bubble","Selection","Insertion","Quicksort");
    //separator
    printf("%s\n",string(68,'-').c_str());
    
    //each size
    for (int n : sizes)
        {
            for (int c = 0; c < 3; c++)
        {
            int* a = new int[n];
            iota(a, a+n, 0);
            //c=0 sorted
            if (!c) shuffle(a, a + n, rng);
            //c=2 reversed
            if (c == 2) reverse(a, a+n);

            //s=shuffled/random, r=reversed, R=sorted
            printf("%-8d %-8c", n, "sRr"[c]);
            for (int i = 0; i < 4; i++)
                {
                    printf(" %10.3f", bench(fns[i], a, n, "Quicksort"));
                }
            printf("\n");
            delete[] a;
        }
        }
        return 0;
}