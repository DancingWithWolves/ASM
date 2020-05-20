#include <stdio.h>
#include <stdlib.h>

typedef char* Elem_t;

class List {
    
    private:
        struct Node {
            Node *next, *prev;
            Elem_t value;

            Node (Elem_t value) : value(value){}
        };
        

    public:

        int id;
        static int count (int add)
        {
            static int qty = 0;
            return qty += add;
        }
        size_t size;
        Node* head, *tail;
        
        List () : size(0), head (nullptr), tail(nullptr) 
        { 
            id = count(0);
            count(1); 
        }
        ~List ()
        {
            Node *cur = head, *next = head;
            while (cur != nullptr) {
                next = cur->next;
                delete cur;
                cur = next;
            }
        }
        void PushBack( Elem_t val )
        {
            Node *new_node = new Node (val);
            new_node->next = nullptr;
            new_node->prev = tail;
            if (size == 0) {
                head = new_node;
                tail = new_node;             
            } else {
                tail->next = new_node;
            }
            tail = new_node;
            size++;
        }


        void Dump_ ()
        {
            FILE *log = fopen ("ListLog.txt", "w");
            fprintf(log, "----------------------------\n");
            fprintf(log, "List %d\n", id);
            fprintf(log, "Size = %lu\n", size);
            fprintf(log, "Head = [%p], tail = [%p]\n\nData:\n", head, tail);
            Node *cur = head, *next = head;
            while (cur) {
                next = cur->next;
                fprintf(log, "[%p] ->\n\tvalue = %s,\n\tnext = [%p],\n\tprev = [%p]\n", cur, cur->value, cur->next, cur->prev );
                cur = next;
            }

            fprintf(log, "----------------------------\n");
            fclose(log);
        }
    
};