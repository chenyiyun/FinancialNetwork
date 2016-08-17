#ifndef NODE_H_INCLUDED
#define NODE_H_INCLUDED


typedef struct NodeType Node;//declaration of Structtype Node
typedef struct QueueNode *QNode;//declaration of Queue Node

struct NodeType //definition of Structtype Node
{
	//***** node characteristics *****//*
	int id;			  // id of the node
	double origin_mV;        // original market value of the node
	double mV;          //current market value of the node
    int infln;		// 0 or 1, current situation of influence,0Ã»ÆÆ²ú£»1ÆÆ²ú
    double beta;    //bankcrupting cost

	/***** neighbor attributes *****/

	int numNeib;		// number of neighbors of the node
	Node **neighbors;		// Node pointers of neighbors
	int *neibIDs;		// ids of neighbors
	double *weightA;		// the weights of the neighbors

	//double sumweight;	// sum of the weights
	int *neibInfln;		// 0-1 vector of current influence situation of neighbors

	Node *next;
};

// extern Node ori_graph;
// extern double **dissim_graph;
// extern int dissim_type;

// extern Node getNodepointer(Node graphnodes, int id);
// //extern void influenceOne(Node graphnodes, int id);
// extern int influenceOne(Node graphnodes, int id);
// extern void destroy_graph_nodes(Node graphnodes);
// extern Node copy_graph_nodes(Node graphnodes);
// //extern void initializeGraph();
// extern double **initializeGraph();
// extern void loadGraph();
// extern void destroy_graph( double **gra );
// extern Node transform_graph(double **gra);

#endif
