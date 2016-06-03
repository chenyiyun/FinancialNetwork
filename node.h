#ifndef NODE_H_INCLUDED
#define NODE_H_INCLUDED


typedef struct NodeType *Node;//declaration of Structtype Node


struct NodeType //definition of Structtype Node
{
	//***** node characteristics *****//*
	int id;			  // id of the node
	//double Drhld;         // direct holdings
    //double *Crhld;       // cossholdings,Cij=gra[i][j],j=0-NumNodes-1
	//double bV;        // book value of the node
	double mV;        // market value of the node
    int infln;		// 0 or 1, current situation of influence,0没破产；1破产
    double beta;    //破产成本，beta=mV*（1-ThrHold）


	/***** neighbor characteristics *****/
	//+++++数组个数一样+++++

	int numNeib;		// number of neighbors of the node
	Node *neighbor;		// Node pointers of neighbors
	int *neibID;		// ids of neighbors
	double *weight;		// the weights of the neighbors
	//int *whereNeibPutMe;		// the position where the neighbor puts me
	//+++++数组个数一样+++++

	//double sumweight;	// sum of the weights
	int *neibInfln;		// 0-1 vector of current influence situation of neighbors

	Node next;
};

extern Node ori_graph;
extern double **dissim_graph;
extern int dissim_type;

extern Node getNodepointer(Node graphnodes, int id);
extern void influenceOne(Node graphnodes, int id);
extern void destroy_graph_nodes(Node graphnodes);
extern Node copy_graph_nodes(Node graphnodes);
//extern void initializeGraph();
extern double **initializeGraph();
extern void loadGraph();
extern void destroy_graph( double **gra );
extern Node transform_graph(double **gra);

#endif // NODE_H_INCLUDED
