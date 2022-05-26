
#include "include/hamt.h"


int main(){
    for (int i = 0 ; i < 1000; i++){
        Hamt<int, int> hamt;
        for (int j = 0; j < 500'000; j++){
            hamt.insert(j,j);
        }
    }
}