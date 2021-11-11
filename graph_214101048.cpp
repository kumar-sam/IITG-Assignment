#include<bits/stdc++.h>
using namespace std;

//Class for edge in adajacency list
class Edges
{
		int id;	//id of node towards which the edge is directed
		int weight;	//edge weight
		char type_of_edge; //type of edge : tree,back,cross,forward;
		Edges *next_edge; //pointer to next edge
		
	public:
		Edges(int node_id , int node_weight); //contructor
		friend class Graph; 
};

//Contructor for edge class
Edges::Edges(int node_id , int node_weight)
{
	//Initializing the edge
	id = node_id;
	weight = node_weight;
	next_edge = NULL;
	type_of_edge = 't';
}

class Graph
{
	int nodes;
	vector<Edges*> edgeList; //Adjacency list for graph
	int num_scc; //number of strongly connected components in graph
	vector<vector<int> > strongly_connected_components;	//stores the strongly connected components
	vector<int> component_indegree; //stores indegree of each SCC
	vector<int> start_time;	//dfs start time of each node 
	vector<int> end_time;	//dfs end time of each node
	
	//Member functions
	public:
		Graph(int n);
		void CreateEdge(int u , int v , int w);
		void PrintAdjacencyList();
		void DFS();
		void NodeVisitDFS(int v,vector<bool> &visited,vector<int> &d,vector<int> &f,vector<int> &pi,int &time ,vector<int> &dfs_order);
		void findEdgeType(vector<int> &d , vector<int> &f , vector<int> &pi);
		void PrintEdgeClassification();
		void PrintTime();
		void GraphPrint(const char* file);
		void NodePrint(int v , FILE* f , vector<bool> &printed);
		void FindSCC();
		void TraverseSCC(int i,vector<bool> &visited,vector<bool> &present_in_stack,stack<int> &scc_stack,vector<int> &low,int &disc,vector<int> &dv);
		void PrintSCC();
		void AssignComponentsToNodes();
		void Dijkstra(int source_node);
		void PrintDistance(vector<int> distance , int source);
		void ComponentGraph();
		bool CheckSemiConnected();
		vector<int> assigned_component;
		vector<vector<int> > component_graph;
};

//Constructor for graph class
Graph::Graph(int v)
{
	//Initializing the class variables
	nodes = v;
	edgeList.resize(v,NULL);
	assigned_component.resize(v);
	num_scc = 0;
}

//Creates edge and stores it in the adjacency list of graph
void Graph::CreateEdge(int source_node , int dest_node , int node_weight)
{
	//new edge : source_node------>dest_node
	Edges *head_node = edgeList[source_node];
	
	Edges *new_edge = new Edges(dest_node , node_weight);
	
	//Inserting the new edge in the beginning of the list;
	edgeList[source_node] = new_edge;

	if(head_node)
	{
		new_edge->next_edge = head_node;
	}
	
}

//Prints adajacency list on terminal
void Graph::PrintAdjacencyList()
{
	cout<<"ADJACENCY LIST : \n";
	
	//Iterating over all nodes and printing the edges outgoing from that edge 
	//source_node-->(destination_node,weight_of_edge_from_source_to_destination)
	for(int i = 0 ; i < nodes ; i++)
	{
		Edges *edges = edgeList[i];
		cout<<i<<"--> ";
		while(edges)
		{
			cout<<"("<<edges->id<<","<<edges->weight<<")";
			if(edges->next_edge)
				cout<<"-->";
			edges = edges->next_edge;
		}
		cout<<"\n";
	}

}

void Graph::DFS()
{
	//Initializing required variables
	vector<bool> visited(nodes , false); //checks if a node has already been visited
	vector<int> d(nodes); //time at which a node is visited
	vector<int> f(nodes); //time at which a node and all its adjacent edges are explored
	vector<int> pi(nodes , -1); //parent node for a node
	vector<int> dfs_order; //stores the order in which nodes are visited
	
	
	int time = 0; //time variable 
	
	//Iterating over all the nodes and applying dfs on the unvisited ones
	for(int i = 0 ; i < nodes ; i++)
	{
		if(visited[i]==false)
		{
			visited[i] = true;
			dfs_order.push_back(i);
			NodeVisitDFS(i , visited , d , f , pi , time ,dfs_order);
		}
	}
	start_time = d;
	end_time = f;
	//printing the dfs traversal
	cout<<"dfs traversal: ";
	for(int i = 0 ; i < nodes ; i++)
	{
		cout<<dfs_order[i]<<" ";
	}
	cout<<endl<<endl;
	findEdgeType(d,f,pi);
}

//Applying DFS on the node v
void Graph::NodeVisitDFS(int v,vector<bool> &visited,vector<int> &d,vector<int> &f,vector<int> &pi,int &time ,vector<int> &dfs_order)
{
	d[v] = ++time;
	Edges *edge = edgeList[v];
	
	//checkinf if the adjacent nodes of nodes v is unvisited and applying DFS on them
	while(edge)
	{
		if(visited[edge->id]==false)
		{
			visited[edge->id] = true;
			dfs_order.push_back(edge->id);
			pi[edge->id] = v;
			NodeVisitDFS(edge->id , visited , d , f , pi , time ,dfs_order);
			//scc_stack.push(edge->id);
		}
		edge = edge->next_edge;
	}
	f[v] = ++time;
}

//Classifying edges of the graph as tree edge,back edge,cross edge,forward edge
void Graph::findEdgeType(vector<int> &d , vector<int> &f , vector<int> &pi)
{
	for(int u = 0 ; u < nodes ; u++)
	{
		Edges *edge = edgeList[u];
		while(edge)
		{
			int v = edge->id;
			
			if(d[v]<d[u] && f[u]<f[v]) //back edge
				edge->type_of_edge = 'b';
			else if(d[u]<d[v] && f[v]<f[u] && pi[v]==u) //tree edge
				edge->type_of_edge = 't';
			else if(d[u]<d[v] && f[v]<f[u] && pi[v]!=u) //forward edge
				edge->type_of_edge = 'f';
			else
				edge->type_of_edge = 'c';	//cross edge
			//cout<<u<<"->"<<v<<" : "<<edge->type_of_edge<<endl;
			edge = edge->next_edge;
		}
	}
}

void Graph::PrintEdgeClassification()
{
	//DFS();
	cout<<"Edge classification : \n";
	Edges* edge;
	for(int i= 0 ; i < nodes  ; i++)
	{
		edge = edgeList[i];
		while(edge)
		{
			cout<<i<<"->"<<edge->id<<" : ";
			if(edge->type_of_edge=='t')
				cout<<"TREE EDGE";
			else if(edge->type_of_edge=='f')
				cout<<"FORWARD EDGE";
			else if(edge->type_of_edge=='b')
				cout<<"BACK EDGE";
			else
				cout<<"CROSS EDGE";
			
			cout<<endl;
			edge = edge->next_edge;
		}
	}
}

//Prints dfs start and end time of each node
void Graph::PrintTime()
{
	cout<<"START AND END TIME :\n";
	cout<<"_____________________________________________\n";
	cout<<setw(10)<<"*"<<setw(8)<<"START"<<setw(8)<<"END\n";
	for(int i = 0 ; i < nodes ; i++)
	{
		cout<<"NODE "<<i<<" : ";
		cout<<setw(7)<<start_time[i];
		cout<<setw(8)<<end_time[i]<<endl;
	}
	cout<<"_____________________________________________\n";
	cout<<endl;
}

//Iterates over nodes and applies dfs on them to check for strongly conneceted components
void Graph::FindSCC()
{
	
	//Initializing the required variables
	vector<bool> visited(nodes , false); //checks if a node is visited
	vector<bool> present_in_stack(nodes , false); //checks of a node is presend in the scc stack
	stack<int> scc_stack; //stack to store the nodes as they are visited
	vector<int> low(nodes); //stores low number for each node
	int disc = -1; //discovery time
	vector<int> dv(nodes); //stores discovery time for each node
	
	//Iterarting over all nodes and applying dfs to find SCC
	for(int i = 0 ; i <nodes ; i++)
	{
		if(visited[i]==false)
		{
			visited[i] = true;
			scc_stack.push(i);
			present_in_stack[i] = true;
			TraverseSCC(i,visited,present_in_stack,scc_stack,low,disc,dv);
		}
	}
}

void Graph::TraverseSCC(int i,vector<bool> &visited,vector<bool> &present_in_stack,stack<int> &scc_stack,vector<int> &low,int &disc,vector<int> &dv)
{
	//cout<<"trav\n";
	Edges *edge = edgeList[i];
	//cout<<"edge";
	
	dv[i] = ++disc;
	low[i] = disc;
	
	//Checking for all nodes outgoing from node i
	while(edge)
	{
		int v = edge->id;
		if(visited[v]==false)
		{
			visited[v] = true;
			scc_stack.push(v);
			present_in_stack[v] = true;
			TraverseSCC(v,visited,present_in_stack,scc_stack,low , disc , dv);
			if(low[i]>low[v])
			{
				//updating low value in case of tree edge
				low[i] = low[v];
			}
		}
		//updating low value in case of back edge
		else if(present_in_stack[v]==true)
				low[i] = min(dv[v],low[i]);
			
		edge = edge->next_edge;
	}
	
	
	if(low[i]==dv[i])
	{
		//A SCC found
		//Storing the scc by poping out the elements of stack until node i is found
		//cout<<endl;
		++num_scc;
		vector<int> comp;
		while(!scc_stack.empty())
		{
			int node = scc_stack.top();
			comp.push_back(node);
			present_in_stack[node]=false;
			scc_stack.pop();
			if(node==i)
				break;
		}
		strongly_connected_components.push_back(comp);
	}
}

//Prints all the strongly connected components of graph on the terminal
void Graph::PrintSCC()
{
	//FindSCC();
	cout<<"Number of components in the graph : "<<num_scc<<endl;
	for(int i = 0; i < num_scc ; i++)
	{
		cout<<"COMPONENT "<<i<<" : ";
		for(auto n:strongly_connected_components[i])
		{
			cout<<n<<" ";
		}
		cout<<endl;
	}
}

//Assigns component number to each node
void Graph::AssignComponentsToNodes()
{
	for(int i = 0 ; i < num_scc ; i++)
	{
		for(auto n:strongly_connected_components[i])
		{
			assigned_component[n] = i; 
		}
	}
}

//Creates component graph 
void Graph::ComponentGraph()
{
	//vector to store the component graph
	component_graph.resize(num_scc , vector<int>(num_scc , 0));
	
	for(int i = 0 ; i < num_scc ; i++)
	{
		for(int j = 0 ; j < num_scc ; j++)
		{
			component_graph[i][j] = 0;
		}
	}

	vector<int> incoming_edges(num_scc,0);
	
	//iterating over all the nodes 
	//and checking if its outgoing edges are going to nodes in same component or different component
		
	for(int i = 0 ; i < nodes ; i++)
	{
		Edges *edge = edgeList[i];
		int src = assigned_component[i];
		while(edge)
		{
			int x = edge->id;
			int dest = assigned_component[x];
			if(src!=dest)
			{
				//adding edge in component graph if the edge goes from one component to another
				++component_graph[src][dest]; 
				++incoming_edges[dest];
			}
			edge = edge->next_edge;
		}
	}
	component_indegree = incoming_edges;
	//CheckSemiConnected(incoming_edges);
}

//Checks if the graph is semi connected or not
bool Graph::CheckSemiConnected()
{
	if(num_scc==1 )
	{
		return true;
	}
	if(num_scc==0)
	{
		return false;
	}
	vector<vector<int> > cg = component_graph; //copying component graph
	vector<int> indegree = component_indegree; //copying the indegree of nodes 
	
	vector<int> topological_order; //stores the topological ordering of the graph
	
	//sorting the component graph topologically
	//and storing the topological ordering until it contains all the components
	while(topological_order.size()<num_scc)
	{
		for(int i = 0 ; i < num_scc ; i++)
		{
			if(indegree[i]==0)
			{
				topological_order.push_back(i);
				for(int j = 0 ; j < num_scc ; j++)
				{
					if(cg[i][j]>0)
					{
						int x = cg[i][j];
						indegree[j] -= x;
						cg[i][j] = 0;
					}
				}
				indegree[i] = num_scc+1;
			}
		}
	}
	
	/*
	If there is an edge between every two components adjacaent
	in the topological ordering then the graph is semi connected
	*/ 
	for(int i = 0 ; i < num_scc-1 ; i++)
	{
		if(component_graph[topological_order[i]][topological_order[i+1]]==0)
		{
			//cout<<"Graph is not semi-connected!\n";
			return false;
		}
	}
	
	return true;
	//cout<<"Graph is semi-connected!\n";
		
}


//Prints the graph using graphviz
void Graph::GraphPrint(const char* filename)
{
	//DFS();
	vector<bool> printed(nodes , false);
	string df = "";
	df = df + filename + ".dot"; //dot file name
	string pngf = "" ;
	pngf =  pngf + filename + ".png"; //png file name
	FILE *file = fopen(df.c_str() , "w");
	fprintf(file , "digraph G{\nnode[shape = oval];\nedge[dir = forward]\n" );
	
	//Applying dfs traversal on the graph and printing the nodes one by one
	for(int i = 0 ; i < nodes ; i++)
	{
		if(printed[i]==false)
		{
			printed[i] = true;
			NodePrint(i , file , printed);
		}	
	}
	
	fprintf(file,"}");
	
	fclose(file);
	
	//Generating image file from dot file
	string cmnd = "dot -Tpng " + df + " -o " + pngf ;
	
	system(cmnd.c_str());
	
}

//Prints th nodes of the tree
void Graph::NodePrint(int i , FILE *fp , vector<bool> &printed)
{
	
	//printf("print %d\n" , i);
	fprintf(fp,"%d[label = \"%d[%d,%d]\"  ,fillcolor = \"aquamarine\",style = filled];\n" , i , i,start_time[i],end_time[i]);
	
	Edges *edge = edgeList[i];
	
	//Checking all the outgoing edges of node i
	while(edge!=NULL)
	{
		if(printed[edge->id]==false)
		{
			printed[edge->id] = true;
			NodePrint(edge->id , fp , printed);
		}
		
		//Printing the edges	
		if(edge->type_of_edge=='f') //forward edge
		{
			fprintf(fp,"%d->%d[label = \" %d,forward\" , decorate = true , " , i , edge->id  , edge->weight );
			fprintf(fp , "color = \"green\",penwidth = \"2\"]\n");
		}
		else if(edge->type_of_edge=='b') //back edge
		{
			fprintf(fp,"%d->%d[label = \" %d,back\" , decorate = true , " , i , edge->id  , edge->weight );
			fprintf(fp , "color = \"darkblue\",penwidth = \"2\"]\n");
		}
		else if(edge->type_of_edge=='c') //cross edge
		{
			fprintf(fp,"%d->%d[label = \" %d,cross\" , decorate = true , " , i , edge->id  , edge->weight);
			fprintf(fp , "color = \"deeppink\",penwidth = \"2\"]\n");
		}
		else //tree edge
		{
			fprintf(fp,"%d->%d[label = \" %d,tree\" , decorate = true , " , i , edge->id  , edge->weight );
			fprintf(fp , "color = \"black\",penwidth = \"3\"]\n");
		}	
		edge = edge->next_edge;
	}
	
}

//Dijkstra single source shortest path 
void Graph::Dijkstra(int source_node)
{
	//Priority queue to extract the minimum distance node 
	priority_queue<pair<int,int>,vector<pair<int,int> >,greater<pair<int ,int> > > min_dist;
	min_dist.push(make_pair(0,source_node));
	
	//Vector to store the distance to all the nodes
	vector<int> dist(nodes , INT_MAX);
	dist[source_node] = 0;
	
	//contains true for nodes for which distance has already been calculated
	vector<bool> path_found(nodes,false);
	path_found[source_node] = true;
	
	cout<<"Distance from source node :\n";
	cout<<setw(15)<<"SOURCE NODE"<<setw(20)<<"TARGET NODE"<<setw(15)<<"DISTANCE"<<endl;
	cout<<"________________________________________________________________\n\n";
	
	//calculating distance until the priority queue is empty
	while(!min_dist.empty())
	{
		//extracting minimum distance node from priority queue
		int node = min_dist.top().second;
		path_found[node] = true;
		
		int distance = min_dist.top().first;
		
		min_dist.pop();
		
		Edges *edge = edgeList[node];
		
		//updating distance for the adjacent nodes 
		while(edge)
		{
			if(path_found[ edge->id]==false)
			{
				if(dist[edge->id]>dist[node]+edge->weight)
				{
					dist[edge->id] = dist[node]+edge->weight;
					min_dist.push(make_pair(edge->weight , edge->id));
				}
			}
			edge = edge->next_edge;
		}
	}
	
	PrintDistance(dist,source_node);
}

//Prints the distance to each node from source node
void Graph::PrintDistance(vector<int> distance , int source)
{
	
	for(int  i = 0 ; i < nodes ; i++)
	{
		cout<<setw(10)<<source<<setw(18)<<i;
		distance[i]==INT_MAX?cout<<setw(20)<<"INF\n\n":cout<<setw(18)<<distance[i]<<"\n\n";
	}
}

//Main function
//Menu based implementation
int main()
{
	int vertices , edges;
	FILE *file ;
	string filename;
	int ch;
	while(true)
	{
		cout<<"1. Enter a test file\n";
		cout<<"2. EXIT\n";
		cout<<"Enter your choice (1/2) : ";
		cin>>ch;
		if(!cin) 
		{
			cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			cout<<"Please enter a valid input\n";
			continue;
		}
		switch(ch)
		{
			case 1 :
			{
				cout<<"Enter the name of test file(ex: abc.txt) : ";
				cin>>filename;
				file = fopen(filename.c_str() , "r");
				if(!file)
					cout<<"Error in opening file\n";
				else
				{
					fscanf(file , "%d %d\n" , &vertices , &edges);
					Graph *graph = new Graph(vertices); //Graph object
					Graph *subgraph = new Graph(vertices);//Subgraph object
					for(int i = 0 ; i < edges ; i++)
					{
						//creating edges
						int u , v , w;
						fscanf(file , "%d %d %d\n" , &u , &v , &w);
						if(u>=vertices ||v>=vertices)
						{
							//node id is greater than n-1
							cout<<"Please input a valid test file.\nThe nodes should be numbered from 0 to n-1 , where n is the number of vertices\n";
							fclose(file);
							exit(0);
						}
						graph->CreateEdge(u , v , w);
					}
					graph->FindSCC();
					graph->AssignComponentsToNodes();
					graph->ComponentGraph();
					bool is_semi_connected = graph->CheckSemiConnected();
					rewind(file);
		
					fscanf(file , "%d %d\n" , &vertices , &edges);
					vector<int> a = graph->assigned_component;
					vector<vector<int> > cg = graph->component_graph;
					
					//inserting edges into subgraph 
					for(int i = 0 ; i < edges ; i++)
					{
						int u , v , w;
						fscanf(file , "%d %d %d\n" , &u , &v , &w);
						if(a[u]==a[v] || cg[a[u]][a[v]]==1)
						{
							subgraph->CreateEdge(u , v , w);
						}
						else
						{
							cg[a[u]][a[v]]--;
							continue;
						}
										
					}
					fclose(file);
					cout<<"GRAPH ";
					graph->PrintAdjacencyList();				
					cout<<endl;
					int c;
					while(true)
					{
						//The menu
						cout<<"\n------------------------------------------------\n";
						cout<<"------------------------------------------------\n\n";
						cout<<"1. Print DFS traversal, classify edges and print graph image (Q.1)\n";
						cout<<"2. Print Strongly connected components (Q.2)\n";
						cout<<"3. Check if graph is semi-connected (Q.4)\n";
						cout<<"4. Create subgraph and print adjacency list for subgraph and its image(Q.3)\n";
						cout<<"5. Print distance(Dijkstra) (Q.5)\n";
						cout<<"6. Select another test file\n";
						cout<<"7. Exit program\n";
						cout<<"\n------------------------------------------------\n";
						cout<<"------------------------------------------------\n";
						
						cout<<"Enter your choice : ";
						cin>>c;
						
						if(!cin) 
						{
							cin.clear();
							cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
							cout<<"Please enter a valid input\n";
							continue;
						}
						
						switch(c)
						{
							case 1:
								{
									graph->DFS();
									cout<<endl;
									graph->PrintTime();
									graph->PrintEdgeClassification();
									string file1 ;
									cout<<"To generate image file for graph , ";
									cout<<"Enter filename without extension : ";
									cin>>file1;
									file1 = file1 + "_graph";
									graph->GraphPrint(file1.c_str());
									cout<<"Image has been generated!\nCheck "<<file1<<".png to view the image.\n";
					
									break;
								}
								
								
							case 2:
								{
									graph->PrintSCC();
									break;
								}
								
							case 3:
								{
									if(is_semi_connected)
										cout<<"The graph is semi-connected\n";
									else
										cout<<"The graph is not semi-connected\n";
									break;
								}
								
							case 4:
								{
									cout<<"SUBGRAPH ";
									subgraph->PrintAdjacencyList();
									cout<<"Subgraph ";
									subgraph->DFS();
									string file2 ;
									cout<<"Enter filename without extension : ";
									cin>>file2;
									file2 = file2 + "_subgraph";
									subgraph->GraphPrint(file2.c_str());
									cout<<"Image has been generated!\nCheck "<<file2<<".png to view the image.\n";
									break;
								}
									
							case 5:
								{
									int source;
									cout<<"Enter source vertex : ";
									cin>>source;
									if(!cin) 
									{
									    cin.clear();
									    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
									    cout<<"Please enter a valid node in range [0,"<<vertices-1<<"]\n";
									    break;
									}
									if(source>=0 && source<vertices)
									{
										graph->Dijkstra(source);
										break;
									}
									else
									{
										cout<<"Please enter a valid node in range [0,"<<vertices-1<<"]\n";
										//exit(0);
									}
								}
								
								
							case 6:
								break;
								
							case 7:
								exit(0);
								
							default:
								cout<<"Enter a valid choice\n";	
						}
						if(c==6)
							break;
					}
					
				}
				break;
			}
			case 2 :
				exit(0);
			default :
				{
					cout<<"Please enter a valid choice\n";
					//exit(0);
					break;
				}
		}
	}
	
	return 0;
}
