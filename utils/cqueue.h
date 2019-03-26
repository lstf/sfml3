#ifndef _CQUEUE_H
#define _CQUEUE_H

#include <iostream>

#define INDEX_INC(n, s) (n + s >= ar_size ? n + s - ar_size : n + s)

using namespace std;

template <typename T>
class CQueue {
private:
	T* ar;
	int ar_size;

	int e_size;
	int index;
	int base_index;

public:
	CQueue();

	void set_size(int n);

	void push(T e);

	T front() const;

	T at(int n) const;

	void pop();

	void clear();

	int size() const;

	~CQueue();
};

template <typename T>
CQueue<T>::CQueue() {
	ar = NULL;
	ar_size = 0;

	e_size = 0;
	index = 0;
	base_index = 0;
}

template <typename T>
void CQueue<T>::set_size(int n) {
	if (n < 1) {
		cout << "[CQUE] request for zero or negative sized CQueue" << endl;
		return;
	}
	if (ar) {
		delete [] ar;
	}
	ar = new T[n];
	ar_size = n;
}

template <typename T>
void CQueue<T>::push(T e) {
	if (INDEX_INC(index, 1) == base_index) {
		cout << "[CQUE] request for push on full CQueue" << endl;
		return;
	}
	ar[index] = e;
	index = INDEX_INC(index, 1);
	e_size++;
}

template <typename T>
T CQueue<T>::front() const {
	if (e_size == 0) {
		cout << "[CQUE] request for front on empty CQueue" << endl;
	}
	return ar[base_index];
}

template <typename T>
T CQueue<T>::at(int n) const {
	if (n < 0 || n > e_size - 1) {
		cout << "[CQUE] request for out of bounds at on CQueue" << endl;
		n = 0;
	}
	return ar[INDEX_INC(base_index, n)];
}

template <typename T>
void CQueue<T>::pop() {
	if (e_size == 0) {
		cout << "[CQUE] request for pop on empty CQueue" << endl;
		return;
	}
	base_index = INDEX_INC(base_index, 1);
	e_size--;
}

template <typename T>
void CQueue<T>::clear() {
	e_size = 0;
	index = 0;
	base_index = 0;
}

template <typename T>
int CQueue<T>::size() const {
	return e_size;
}

template <typename T>
CQueue<T>::~CQueue() {
	if (ar) {
		delete ar;
	}
}

#endif 
