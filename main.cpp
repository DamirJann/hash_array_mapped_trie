#include "include/hamt.h"
int main(){
    for (int i = 0; i < 10000;i++){
        Hamt<int, int> h;
        for (int j = 0; j  < 1'000'000; j++){
            h.insert(j,j );
        }
        cout << "finished" << endl;
    }
}