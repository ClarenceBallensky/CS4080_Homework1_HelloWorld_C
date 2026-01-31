#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//doubly linked list node
//typedef creates an alias for "Node" to avoid typing "struct Node" repeatedly 
typedef struct Node {
    char *data;
    struct Node* prev;
    struct Node* next;
} Node;

//create a new node with a heap-allocated copy of the string
Node *create_node(const char *str) {
    Node *node = malloc(sizeof(Node));
    if (!node) {
        perror("malloc"); //perror = last error that happened in a system call or library function 
        exit(EXIT_FAILURE);
    }

    node->data = malloc(strlen(str) + 1); //allocate enough memory for the string + the null terminator (\0), assign the pointer to the data field of the node
    if (!node->data) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    strcpy(node->data, str); //copies the provided string to memory, pointed to by node->data
    node->prev = NULL;
    node->next = NULL;
    return node;
}

//insert a node at the head
void insert_at_head(Node **head, const char *str) { //double asterisk means a pointer that points to a pointer
    Node *new_node = create_node(str);

    new_node->next = *head;
    if (*head != NULL) {
        (*head)->prev = new_node; //make the previous pointer of the old node point to the new node (aka, make the new node the new head)
    }

    *head = new_node; //update the head pointer to point to the new_node 
}

//find the first node containing a given string 
Node *find_items(Node *head, const char *str) {
    for (Node *curr = head; curr != NULL; curr = curr->next) { //loop through all the nodes 
        if(strcmp(curr->data, str) == 0) { //strcmp returns 0 when two strings are exactly equal, so this if statement says "if curr->data and str are equal"
            return curr;
        }
    }
    return NULL;
}

//delete the first node containing a given string
int delete(Node **head, const char *str) {
    Node *node = find_items(*head, str);
    if (!node) {
        return 0; //node not found (so it was not deleted)
    }

    if(node->prev) { //if node has a previous node
        node->prev->next = node->next; //make the previous node point to the next node, skipping over node
    }
    else {
        *head = node->next; //delete the head
    }

    if(node->next) { //if node has a next node (means we're not at the tail yet)
        node->next->prev = node->prev; //next node points backwards to the previous node, skipping over node
    }

    free(node->data);
    free(node);
    return 1; //signals that the function successfully deleted the node 
}

//print list from head to tail
void print_list(Node *head) {
    printf("List: ");
    for (Node *curr = head; curr != NULL; curr = curr->next) {
        printf("\"%s\" ", curr->data);
    }
    printf("\n");
}

//free the entire list
void free_list(Node *head) {
    while(head) { //loop while head is not null
        Node *next = head->next; //store a pointer to the next node in the list because we are about to free head 
        free(head->data);
        free(head);
        head = next; //move to the next node
    }
}

//test the list 
//this turned out more emo than I anticipated 
int main(void) {
    
    Node *list = NULL;

    insert_at_head(&list, "hopes and dreams");
    insert_at_head(&list, "human souls");
    insert_at_head(&list, "long nights");

    print_list(list);

    Node *found = find_items(list, "human souls");
    if (found) {
        printf("Found: %s\n", found->data);
    }
    else {
        printf("Not found\n");
    }

    printf("Deleting human souls...\n");
    delete(&list, "human souls");
    print_list(list);

    printf("Deleting hopes and dreams...\n");
    delete(&list, "hopes and dreams");
    print_list(list);

    printf("Deleting long nights...\n");
    delete(&list, "long nights");
    print_list(list);

    free_list(list);
    return 0;
}