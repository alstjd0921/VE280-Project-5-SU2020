#ifndef PROJECT5_DLIST_IMPL_H
#define PROJECT5_DLIST_IMPL_H

#include <iostream>
#include "dlist.h"

template<class T>
bool Dlist<T>::isEmpty() const {
    return first == nullptr;
}

template<class T>
void Dlist<T>::insertFront(T *op) {
    if (isEmpty()) {
        node *temp = new node;
        temp->op = op;
        temp->next = nullptr;
        temp->prev = nullptr;
        first = temp;
        last = temp;
    } else {
        node *temp = new node;
        temp->op = op;
        temp->prev = nullptr;
        temp->next = first;
        first->prev = temp;
        first = temp;
    }
}

template<class T>
void Dlist<T>::insertBack(T *op) {
    if (last == nullptr) {
        node *temp = new node;
        temp->op = op;
        temp->next = nullptr;
        temp->prev = nullptr;
        first = temp;
        last = temp;
    } else {
        node *temp = new node;
        temp->op = op;
        temp->prev = last;
        temp->next = nullptr;
        last->next = temp;
        last = temp;
    }
}

template<class T>
T *Dlist<T>::removeFront() {
    if (isEmpty()) {
        throw emptyList{};
    }

    node *ptr = first;
    T *temp = first->op;
    if (first == last) {
        first = nullptr;
        last = nullptr;
    } else {
        first = first->next;
        first->prev = nullptr;
    }
    delete ptr;

    return temp;
}

template<class T>
T *Dlist<T>::removeBack() {
    if (isEmpty()) {
        throw emptyList{};
    }

    node *ptr = last;
    T *temp = last->op;
    if (first == last) {
        first = nullptr;
        last = nullptr;
    } else {
        last = last->prev;
        last->next = nullptr;
    }
    delete ptr;

    return temp;
}

template<class T>
T *Dlist<T>::remove(bool (*cmp)(const T *, const T *), T *ref) {
    node *ptr = first;
    while (ptr != nullptr) {
        if (cmp(ptr->op, ref)) {
            T *ret = ptr->op;
            ptr->prev->next = ptr->next;
            ptr->next->prev = ptr->prev;
            delete ptr;
            return ret;
        }
        ptr = ptr->next;
    }
    return nullptr;
}

template<class T>
Dlist<T>::Dlist():
        first(nullptr), last(nullptr) {}

template<class T>
Dlist<T>::Dlist(const Dlist<T> &l):
        first(nullptr), last(nullptr) {
    copyAll(l);
}

template<class T>
Dlist<T> &Dlist<T>::operator=(const Dlist<T> &l) {
    copyAll(1);
    return *this;
}

template<class T>
Dlist<T>::~Dlist() {
    removeAll();
}

template<class T>
void Dlist<T>::removeAll() {
    while (first != nullptr && last != nullptr && first != last) {
        removeFront();
        removeBack();
    }
    if (first == last && first != nullptr) {
        removeFront();
    }
}

template<class T>
void Dlist<T>::copyAll(const Dlist<T> &l) {
    if (l.isEmpty()) {
        return;
    }
    if (!isEmpty()) {
        removeAll();
    }

    node *ptr = l.first;
    while (ptr != nullptr) {
        insertBack(ptr->op);
        ptr = ptr->next;
    }
}

#endif //PROJECT5_DLIST_IMPL_H
