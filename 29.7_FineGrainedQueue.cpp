﻿// 29.7_FineGrainedQueue.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//Функция должна вставить узел с переданным значением value в позицию pos. Чтобы не переусложнять реализацию, предполагается следующее:
//*очередь не пустая,
//*вставляется элемент в середину или конец списка, то есть вставку в начало списка не нужно рассматривать,
//*если pos больше длины списка, то нужно вставить элемент в конец списка.

#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

template <typename T>
struct Node
{
    T value;
    Node* next;
    std::mutex* node_mutex;
};

template <class T>
class FineGrainedQueue
{
    Node<T>* head;
    std::mutex* queue_mutex;

public:
    void insertIntoMiddle(T value, int pos) {
        Node* head_, * tail_;// указатели на предыдущий и текущий элемент
        head_ = this->head; 
        tail_ = this->head->next;
        int count = 0;

        head_->node_mutex->lock();
        tail_->node_mutex->lock();

        while (tail_) // пока указатель на текущий элемент не нулевой
        {
            if (count == pos)
            {
                queue_mutex->lock();

                Node<T> newNode;

                newNode.value = value;
                newNode.next = tail_;
                tail_ = *newNode;

                head_->node_mutex->unlock();
                tail_->node_mutex->unlock();

                queue_mutex->unlock();
                
                return;
            }

            // если не нашли элемент для удаления, то двигаемся дальше по очереди
            Node* oldHead_ = head_;
            head_ = tail_;
            tail_ = head_->next;

            oldHead_->node_mutex->unlock();// обратите внимание, после сдвига что анлочится "старый" предыдущий элемент
            tail_->node_mutex->lock(); // а потом лочится новый текущий элемент

            count++;
        }
        queue_mutex->lock();

        Node<T> newNode;

        newNode.value = value;
        newNode.next = nullptr;
        tail_ = *newNode;

        queue_mutex->unlock();
        head_->node_mutex->unlock();
        tail_->node_mutex->unlock();
   }
};


int main()
{

    FineGrainedQueue<int> myQueue;
   
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
