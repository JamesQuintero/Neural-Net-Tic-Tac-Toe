#ifndef NODE_H
#define NODE_H



struct node
{
	int node_id=0;

	int** board;
	// double good = default_good;
	double good=0;
	double bad=0;
	double okay=0;
	
	int next_index=0;
	int prev_index=0;
	node** next;
	node* prev;
};

// void newNode(node* ptr, int** board, int side_len);


//links a new node to ptr, with initialized board and the size of one side of the board
inline void newNode(node* ptr, int** board, int side_len)
{
	ptr->next[ptr->next_index] = new node();

	//ptr to new next node
	node* next = ptr->next[ptr->next_index];
	next->board = board;
	next->next = new node*[side_len*side_len];

	//increments number of next nodes
	ptr->next_index++;

	//pointer to parent
	next->prev = ptr;	
}

#endif