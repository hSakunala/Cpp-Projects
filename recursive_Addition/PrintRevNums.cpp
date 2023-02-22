#include <iostream>
#include <string>

int max;
int count;

void PrintRevNums(int sum){
    int x;
    std::cin>>x;
    if (x== 0){
        std::cout << std::endl;
    }
    else{
        if(x>max)
            max=x;
        int newSum;
        newSum= sum+x;
        PrintRevNums(newSum);
        std::cout << x <<" Total: " << newSum << std::endl;
        if (x == newSum)
            std::cout << "The maximum value is: " << max << std::endl;
    }
}

// 12 Total: 156
// 32 Total: 144
// 67 Total: 112
// 45 Total: 45