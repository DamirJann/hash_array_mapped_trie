
#include "include/hamt.h"


int main(){
    int count = 5'000'000;
    {
        set<int> set;
        for (int j = 0; j < count; j++){
            set.insert(j);
        }
    }
    cout << "set\n";
    {
        Hamt<int, int> hamt;
        for (int j = 0; j < count; j++){
            hamt.insert(j,j);
        }
    }

    cout << "hamt\n";

}