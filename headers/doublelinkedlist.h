#pragma once
#include <cstddef>
#include <stdexcept>

template<typename T>
class DoubleLinkedList {
private:
    struct Node {
        T data;
        Node* prev;
        Node* next;
        Node(const T& value) : data(value), prev(nullptr), next(nullptr) {}
    };

    Node* head = nullptr;
    Node* tail = nullptr;

public:
   DoubleLinkedList() {
                   this->head = this->tail = NULL;
         }
   DoubleLinkedList(const DoubleLinkedList& other) : head(nullptr), tail(nullptr) {
          // Копируем все элементы из другого списка
          Node* current = other.head;
          while (current != nullptr) {
              push_back(current->data);
              current = current->next;
          }
      }
  int size() const {
          size_t count = 0;
          Node* current = head;
          while (current) {
              count++;
              current = current->next;
          }
          return count;
      }

      DoubleLinkedList& operator=(const DoubleLinkedList& other) {
          if (this != &other) {

              while (head != nullptr)
                  pop_front();


              Node* current = other.head;
              while (current != nullptr) {
                  push_back(current->data);
                  current = current->next;
              }
          }
          return *this;
      }


      DoubleLinkedList(DoubleLinkedList&& other) noexcept
          : head(other.head), tail(other.tail) {
          other.head = nullptr;
          other.tail = nullptr;
      }


      DoubleLinkedList& operator=(DoubleLinkedList&& other) noexcept {
          if (this != &other) {

              while (head != nullptr)
                  pop_front();


              head = other.head;
              tail = other.tail;

              other.head = nullptr;
              other.tail = nullptr;
          }
          return *this;
      }
   ~DoubleLinkedList() {
       while (head != NULL)
           pop_front();
   }

   Node* push_front(T data) {
       Node* ptr = new Node(data);
       ptr->next = head;
       if (head != NULL)
           head->prev = ptr;
       if (tail == NULL)
           tail = ptr;
       head = ptr;

       return ptr;
   }
   Node* push_back(T data) {
       Node* ptr = new Node(data);
       ptr->prev = tail;
       if (tail != NULL)
           tail->next = ptr;
       if (head == NULL)
           head = ptr;
       tail = ptr;

       return ptr;
   }
   void pop_front() {
       if (head == NULL) return;

       Node* ptr = head->next;
       if (ptr != NULL)
           ptr->prev = NULL;
       else
           tail = NULL;

       delete head;
       head = ptr;
   }
   void pop_back() {
       if (tail == NULL) return;

       Node* ptr = tail->prev;
       if (ptr != NULL)
           ptr->next = NULL;
       else
           head = NULL;

       delete tail;
       tail = ptr;
   }

   Node* getAt(int index) const {
       if (index < 0) {
           return nullptr;
       }

       Node* ptr = head;
       int n = 0;

       while (ptr != nullptr && n < index) {
           ptr = ptr->next;
           ++n;
       }


       if (n != index) {
           return nullptr;
       }

       return ptr;
   }
   T get(int index) const {
           Node* node = getAt(index);
           if (node == nullptr) {
               throw std::out_of_range("Index out of bounds");
           }
           return node->data;
       }
   Node* operator[](int index) {
       return getAt(index);
   }
   Node* insert(int index, T data) {
       Node* right = getAt(index);
       if (right == NULL)
           return push_back(data);

       Node* left = right->prev;
       if (left == NULL)
           return push_front(data);

       Node* ptr = new Node(data);

       ptr->prev = left;
       ptr->next = right;
       right->prev = ptr;
       left->next = ptr;

       return ptr;
   }
   void erase(int index) {
       Node* ptr = getAt(index);
       if (ptr == NULL) {
           return;
       }

       if (ptr->prev == NULL) {
           pop_front();
           return;
       }

       if (ptr->next == NULL) {
           pop_back();
           return;
       }

       Node* left = ptr->prev;
       Node* right = ptr->next;
       left->next = right;
       right->prev = left;
       delete ptr;
   }
   public:
       class Iterator {
       private:
           Node* current;

       public:
           Iterator(Node* node) : current(node) {}


           Iterator& operator++() {
               if (current) current = current->next;
               return *this;
           }


           Iterator operator++(int) {
               Iterator temp = *this;
               if (current) current = current->next;
               return temp;
           }


           T& operator*() const {
               return current->data;
           }


           bool operator!=(const Iterator& other) const {
               return current != other.current;
           }

           bool operator==(const Iterator& other) const {
               return current == other.current;
           }
       };


       Iterator begin() { return Iterator(head); }
       Iterator end()   { return Iterator(nullptr); }


   };


