#ifndef ZFTOS_DEV_VECTOR_H
#define ZFTOS_DEV_VECTOR_H



template <class T>
class  Vector
{
public:

    typedef T* iterator;

    Vector();
    Vector(T* array, unsigned int size);
    Vector(unsigned int size);
    Vector(unsigned int size, const T& initial);
    Vector(const Vector<T>& v);
    ~Vector();

    unsigned int capacity() const;
    unsigned int size() const;
    bool empty() const;
    iterator begin();
    iterator end();
    T& front();
    T& back();
    void remove(iterator i);
    void push_back(const T& value);
    void pop_back();
    bool contains(const T& value);
    void reserve(unsigned int capacity);
    void resize(unsigned int size);

    T& operator[](unsigned int index);
    Vector<T>& operator=(const Vector<T>&);
    void rawCopy(Vector<T>&);
    void clear();
private:
    unsigned int _size;
    unsigned int _capacity;
    T* buffer;
};



template <class T>
Vector<T>::Vector(T *array, unsigned int size) {
    _capacity = size;
    _size = size;
    if (size != 0) {
        buffer = new T[size];
    }
    else {
        buffer = nullptr;
    }
    for (unsigned i = 0; i < size; ++i) {
        push_back(array[i]);
    }
}



template<class T>
Vector<T>::Vector() {
    _capacity = 0;
    _size = 0;
    buffer = nullptr;
}



template<class T>
Vector<T>::Vector(const Vector<T>& v)
{
    _size = v._size;
    _capacity = v._capacity;
    buffer = new T[_size];
    for (unsigned int i = 0; i < _size; i++) {
        buffer[i] = v.buffer[i];
    }
}



template<class T>
Vector<T>::Vector(unsigned int size) {
    if (size != 0) {
        buffer = new T[size];
    }
    else {
        buffer = nullptr;
    }
    _capacity = size;
    _size = size;
}



template <class T>
bool Vector<T>::contains(T const & value) {
    for (Vector<T>::iterator it = begin(); it != end(); ++it) {
        if (*it == value) {
            return true;
        }
    }
    return false;
}



template<class T>
Vector<T>::Vector(unsigned int size, const T& initial) {
    _size = size;
    _capacity = size;
    buffer = new T [size];
    for (unsigned int i = 0; i < size; i++) {
        buffer[i] = initial;
    }
}



template<class T>
Vector<T>& Vector<T>::operator= (const Vector<T>& v) {
    delete[] buffer;
    _size = v._size;
    _capacity = v._capacity;
    buffer = new T [_size];
    for (unsigned int i = 0; i < _size; i++) {
        buffer[i] = v.buffer[i];
    }
    return *this;
}



template<class T>
typename Vector<T>::iterator Vector<T>::begin() {
    return buffer;
}



template<class T>
typename Vector<T>::iterator Vector<T>::end() {
    return buffer + size();
}



template<class T>
T& Vector<T>::front() {
    return buffer[0];
}



template<class T>
T& Vector<T>::back() {
    return buffer[_size - 1];
}



template<class T>
void Vector<T>::push_back(const T& v) {
    if (_size >= _capacity) {
        reserve(_capacity + 5);
    }
    buffer [_size++] = v;
}



template<class T>
void Vector<T>::pop_back() {
    _size--;
}



template<class T>
void Vector<T>::reserve(unsigned int capacity) {
    if (buffer == 0) {
        _size = 0;
        _capacity = 0;
    }
    T * Newbuffer = new T [capacity];
    //assert(Newbuffer);
    unsigned int l_Size = capacity < _size ? capacity : _size;
    //copy (buffer, buffer + l_Size, Newbuffer);
    for (unsigned int i = 0; i < l_Size; i++) {
        Newbuffer[i] = buffer[i];
    }
    _capacity = capacity;
    delete[] buffer;
    buffer = Newbuffer;
}



template<class T>
unsigned int Vector<T>::size() const {
    return _size;
}



template<class T>
void Vector<T>::resize(unsigned int size) {
    reserve(size);
    _size = size;
}



template<class T>
T& Vector<T>::operator[](unsigned int index) {
    return buffer[index];
}



template<class T>
unsigned int Vector<T>::capacity() const {
    return _capacity;
}



template<class T>
Vector<T>::~Vector() {
    delete[] buffer;
}



template <class T>
void Vector<T>::clear() {
    _capacity = 0;
    _size = 0;
    delete[] buffer;
    buffer = 0;
}



template <class T>
bool Vector<T>::empty() const {
    return size() == 0;
}



template <class T>
void Vector<T>::rawCopy(Vector& vector) {
    buffer = vector.buffer;
    _size = vector._size;
    _capacity = vector._capacity;
}


/*TODO: implement this*/
template <class T>
void Vector<T>::remove(Vector<T>::iterator it){
    if (it == end() - 1) {
       pop_back();
    }
    else {

    }
}



#endif //ZFTOS_DEV_VECTOR_H

