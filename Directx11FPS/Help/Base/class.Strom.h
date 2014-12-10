#include "../Help/Base/class.DynArray.h"

/*
Directed Graph: A directed graph is one in which edges connect nodes in only one direction.
Undirected Graph: An undirected graph is one in which edges connect nodes bidirectionally (in both directions). Node: A node, usually drawn as a circle, represents an item that can be related to other items or nodes. Nodes are sometimes referred to as vertices.
Edge: An edge represents a connection between nodes. Edges can be either directed or undirected, depending on the type of graph. Edges can also have weights, which may correspond to strength of relationship or distance between edges. (For instance, if a graph represents a map, then the weights of each edge will represent the distance between two nodes.)
Connected: A graph is connected if from any node you can reach any other node.
Disconnected: A graph is disconnected if certain groups of nodes from an island that has no connection to the rest of the graph.
*/

class LinkedList
{
	protected :
		LinkedList *m_next;

	public : 
		void AddHere(LinkedList *a);
		void AddToEnd(LinkedList *a);
		void DelThis();	
		void DelAllToEnd();
		void RunToAll( bool (*p)() ); // true ak break

		LinkedList* getFromPosition(int i);
};

class BinarnyStrom
{
	// vzdy vytvara 2 vetvy naraz, nieco ako heap
};

/*
	S prehladavanie, haldanie, run do SIRKY
	H Hlbkovo

*/
class Strom
{
	protected :
		DynArray<Strom*> m_data;
		bool m_madeti = false;

	public : 
		void AddVetva(Strom *a); // prida novu vetvu do zoznamu
		bool AddVetvaS(Strom *a, int max); // ak deti je menej ako max prida do zoznamu inak na dieta
		bool AddVetvaH(Strom *a, int max); // ak deti je menej ako max prida do zoznamu inak na dieta

		void DelVetva(int children);
		Strom *a;
		a->getVetva(3)->DelAll(); // musi sa to zabezspecit pre rodica, cize objekt sa nevymaze cely ale len jeho deti a deti sa nastavy na false


		void DelAll(); // vymaze vsetke deti aj dalsie vetvy
		
		
		void setVetva(Strom *a, int children); // nastavy vetvu X novou vetvou, inak nic
		Strom* getVetva(int i); // throw exception
		Strom* getVetva(DynArray<int> z); // zada 1, 4, 5, 3, 4, 5 a hlada poda tej pozicie, throw exception
		DynArray<int> getID(Strom *a); // zada sa rodic, a zisti sa ze je to id 1, 4, 5, 6, 3 cize taka vzdialenost

		int Count(); // spocita pocet vetviev a podveti
		int getVetvy(); // pocet deti

		Strom* PrehladavanieS( bool (*p)() );
		Strom* PrehladavanieH( bool (*p)() ); // true ak break
		void RunToAllS( bool (*p)() ); // true ak break
		void RunToAllH( bool (*p)() ); // true ak break
};
