/// Konieczna byla zamiana .cpp na plik .h bo linker nie radzi sobie z template, gdy jest rozbity na .cpp i .h
/// https://bytefreaks.net/programming-2/c/c-undefined-reference-to-templated-class-function

#include "DynamicArray.h"
#include <iostream>

/* Kopiowanie wektora */
template <typename T>
void DynamicArray<T>::copy(T *to) const{
    for (int i = 0; i < amountOfElements; i++) {
        to[i] = t[i]; //przypisanie kolejnym elementom wektora "to" odpowiadojących elementów wektora "t" 
    }
}
/*zmienia rozmiar wektora do size*/
template <typename T>
void DynamicArray<T>::resize(int size) {
    if(size < amountOfElements){//sprawdzenie czy zadany rozmiar jest mniejszy od ilosci elementów
        std::cout << "Nie można zmienić rozmiaru, więcej elementów niż podany rozmiar";
        exit(1);
    }

    length = size;
    T *newT = new T[length];//stworzenie nowego wektora o zadanej wilkosci
    copy(newT);//skopiowanie "starego " wektora do nowego
    delete[] t;//usuniecia "starego" wektora
    t = newT;
}
/*zmienia rozmiar wektora na 2 razy dłuższy*/
template <typename T>
void DynamicArray<T>::increaseLength() {
    resize(length*2);//zmiana długości wektora na 2 razy dłuższą
}
/*konstruktor*/
template <typename T>
DynamicArray<T>::DynamicArray() {
    length = 1;
    amountOfElements = 0;
    t = new T[length];
}

template<typename T>
DynamicArray<T>::DynamicArray(const DynamicArray<T>& toCopy) {
    length = toCopy.length;
    amountOfElements = toCopy.amountOfElements;
    t = new T[length];
    toCopy.copy(t);
}
/*umieszczenie nowego elementu*/
template <typename T>
void DynamicArray<T>::push(T elem) {
    amountOfElements++;//zwiekszenie ilości elementów
    if (amountOfElements > length)
        increaseLength();//zwiekszenie długości gdy ilosc elmentow jest wieksza od długości
    t[amountOfElements - 1] = elem;//dododanie elementu na końcu wektora
}
/*operator indeksu*/
template <typename T>
T &DynamicArray<T>::operator[](int idx) const {
    return t[idx];
}

// nie mozna tu zwalniac pamieci, bo by sie zrobila zlozonosc kwadratowa
// gydby na zmiane dodawac i odejmowac na granicy
/*usiniecie ostatniego elementu wektora*/
template <typename T>
T DynamicArray<T>::pop() {
    amountOfElements--;//zmniejszami o 1 ilość elementow
    return t[amountOfElements];
}

template <typename T>
DynamicArray<T>::~DynamicArray() {
    if(t!= nullptr){
        delete[] t;
        t = nullptr;
    }
}
/*zwrócenie ilości elementów wektora*/
template<typename T>
int DynamicArray<T>::size() const{
    return amountOfElements;
}
/*zwrócenie iteratora do pierwszego elementu wektora */
template<typename T>
T *DynamicArray<T>::begin() const{
    return t;
}
/*zwrócenie iteratora ustawionego za ostatnim elementem wektora */
template<typename T>
T *DynamicArray<T>::end() const{
    return t+amountOfElements;
}
/*usuwanie wszystkich elementy z wektora */
template<typename T>
void DynamicArray<T>::clear() {
    amountOfElements = 0;
    length = 1;
    delete[] t;//usuniecie wektora
    t = new T[length];
}
/*sklonowanie wektora*/
template<typename T>
DynamicArray<T> DynamicArray<T>::clone() const{
    DynamicArray<T> res =  DynamicArray<T>();
    res.resize(length);//zmienienie długości nowego wektora
    copy(res.t);//skopiowanie elementow do nowego wektora
    res.amountOfElements = amountOfElements;
    return res;//zwrócenie nowego wektora
}


/*umieszczenie wielu nowych elementów*/
template<typename T>
void DynamicArray<T>::push_many(const DynamicArray<T> &arr) {
    int new_amount_of_elements = amountOfElements + arr.amountOfElements;//zwiekszenie liczby elementow
    if(size() < new_amount_of_elements){
        resize(new_amount_of_elements*2);//gdy ilość elementow przekracza rozmar wektora to zwiekszamy rozmiar
    }
    for(int i = 0; i<arr.size(); i++){
        push(arr[i]);//dodanie kolejnych elementów
    }
}


