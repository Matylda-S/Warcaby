#ifndef CHECKERSAI_HELPERS_H
#define CHECKERSAI_HELPERS_H

#include <cstdlib>

/*zamiana elementów*/
template<typename T>
inline void swap(T &a, T &b){
    T tmp = a;
    a = b;
    b = tmp;
}
/*zwraca prawde z prawdopodobieństem probility*/
inline bool randBool(double probability){
    return rand()/(RAND_MAX+1.0) < probability;
}
/*zwrócenie losowej wartości z przedziału from do to*/
inline int randInt(int from, int to){
    return from + (rand()%(to-from+1));
}
/*odwrócenie elementów vectora*/
template<typename T>
void reverse(T* begin, T* end){
    end--;
    while(begin<end){
        swap(*begin, *end);//zamiana elementów
        begin++;
        end--;
    }
}
/*zapełnienie tą samą wartością*/
template<typename T>
void fill(T* t, int n, T val){
    for(int i = 0; i<n; i++)
        t[i] = val;
}
/*zwraca wieksza wartosc*/
template<typename T>
T max(T a, T b){
    return (a>b ? a : b);
}
/*zwraca mniejsza wartosc*/
template<typename T>
T min(T a, T b){
    return (a<b ? a : b);
}

#endif //CHECKERSAI_HELPERS_H
