//Drake Shannon-Sonderman
//COP3502C Assignment 2
//Professor Ahmed
//Section 04

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#define MAX_CUSTOMERS 500

typedef struct Customer {

  char name[51];
  int num_tickets;
  int arrival_time;
  int line_number;
}Customer;

typedef struct Node {

  Customer* customer;
  struct Node* next;
}Node;

typedef struct Queue {

  Node* front;
  Node* back;
  int size;
}Queue;

void process_customer(Queue queues[], int b, char name[], int nt, int t); //processes a customer
void init(Queue queues[]); //initialize a queue
void enqueue(Queue* q, Customer* c); //add customer to queue
void assign_queues_booths(Queue queues[], int b, int k); //assigns customers to queues
Customer dequeue(Queue* q); //remove customer from queue
int empty(Queue* q); //check if queue is empty
int size(Queue* q); //get size of queue
Customer front(Queue* q); //get front of queue

int main() 
{
  //variable declaration
  int n, b, nt, t, i;
  char name[51];
  Queue queues[12];

  //data collection
  scanf("%d", &n);
  scanf("%d", &b);

  for (i = 0; i < 12; i++) //initialize queues
  {
    init(&queues[i]);
  }

  for (i = 0; i < n; i++) //collect data and create customers
  {
    scanf("%s %d %d", name, &nt, &t);
    process_customer(queues, b, name, nt, t);
  }

  assign_queues_booths(queues, b, n); //assign customers to queues

  for (i = 0; i < 12; i++) //process customers and print checkout times
  {
    while (!empty(&queues[i])) 
    {
      Customer c = dequeue(&queues[i]);
      printf("%s from line %d checks out at time %d.\n", c.name, i + 1, c.arrival_time);
    }
  }

  for (i = 1; i <= b; i++) //print checkout times for each booth
  {
    printf("Booth %d\n", i);

    for (int j = 0; j < 12; j++) 
    {
      if (queues[j].size > 0) 
      {
        Node* current = queues[j].front;

        while (current) 
        {
          if ((current->customer->line_number % b) == (i - 1)) 
          {
            Customer c = dequeue(&queues[j]); //dequeue function call
            printf("%s from line %d checks out at time %d.\n", c.name, j + 1, c.arrival_time + c.num_tickets);
          }

          current = current->next;
        }
      }
    }

    printf("\n");
  }

  //free memory for customers and nodes
  for (int i = 0; i < 12; i++) 
  {
    while (!empty(&queues[i])) 
    {
      dequeue(&queues[i]);
    }
  }

  return 0;
}

void process_customer(Queue queues[], int b, char name[], int nt, int t) 
{
  //variable declaration
  int p = name[0] - 'A';
  int q;

  if ((p % 13) != 0) //if necessary booths are not equal to 0
  {
    q = (p % 13);
  } 
  else //else create empty booth
  {
    int min_size = MAX_CUSTOMERS;
    int min_queue = 1;

    for (int i = 1; i <= 12; i++) 
    {
      if (queues[i].size < min_size) 
      {
        min_size = queues[i].size;
        min_queue = i;
      }
    }

    q = min_queue;
  }

  for (int i = 0; i < strlen(name); i++) //change to all uppercase
  {
    name[i] = toupper(name[i]);
  }

  Customer* new_customer = (Customer*)malloc(sizeof(Customer)); //allocate necessary memory
  strcpy(new_customer->name, name); //copy input name

  new_customer->num_tickets = nt;
  new_customer->arrival_time = t;
  new_customer->line_number = q;

  enqueue(&queues[q], new_customer); //enqueue function call
}

void init(Queue *queues) 
{
  queues->front = NULL;
  queues->back = NULL;
  queues->size = 0;
}

void enqueue(Queue* q, Customer* c) 
{
  Node* new_node = (Node*)malloc(sizeof(Node)); //allocate necessary memory
  new_node->customer = c;
  new_node->next = NULL;

  if (empty(q)) //queue is empty
  {
    q->front = new_node;
  } 
  else //make next in queue
  {
    q->back->next = new_node;
  }

  q->back = new_node;
  q->size++;
}

void assign_queues_booths(Queue queues[], int b, int k) 
{
  //variable declaration
  int booths_assigned = 0;
  int booth_index = 1;

  for (int i = 0; i < 12 && booths_assigned < k; i++) //iterate over the queues
  {
    //check if queue is not empty
    if (queues[i].size > 0) 
    {
      booths_assigned++;

      //move to next booth if necessary
      if (booths_assigned % (k / b) == 0 && booth_index < b) 
      {
        booth_index++;
      }
    }
  }
}

Customer dequeue(Queue* q) 
{
  if (empty(q)) 
  {
    printf("Queue is empty\n");

    Customer empty_customer;
    strcpy(empty_customer.name, "");

    empty_customer.num_tickets = 0;
    empty_customer.arrival_time = 0;
    empty_customer.line_number = 0;

    return empty_customer;
  }

  Customer c = *(q->front->customer);
  Node* temp = q->front;
  q->front = q->front->next;
  q->size--;

  //update back pointer if queue becomes empty
  if (empty(q)) 
  {
    q->back = NULL;
  }
  
  free(temp->customer); //free memory for customer
  free(temp); //free memory for node

  return c;
}


int empty(Queue* q) 
{
  return q->size == 0;
}

int size(Queue* q) 
{
  return q->size;
}

Customer front(Queue* q) 
{
  if (empty(q)) //if queue is empty
  {
    printf("Queue is empty\n");

    Customer empty_customer;
    return empty_customer;
  }

  return *(q->front->customer);
}