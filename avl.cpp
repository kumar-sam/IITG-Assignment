/*
A class based program on AVL tree which implements the following basic functionality.
1. Insertion -x
2. Deletion -x
3. search -x
4. print tree

*/
#include<iostream>
#include<bits/stdc++.h>

using namespace std;


// Class of the TreeNode
class TreeNode {
private:
    int data;
    int bf;
    TreeNode  *LChild, *RChild; 
 
public:
    // Default Constructor of tree node.
    TreeNode()
    {
        data = INT_MIN;
        bf = INT_MIN;
        LChild = RChild = NULL;
    }
    
    // Constructor of tree node with one param.
    TreeNode(int x)
    {
        data = x;
        bf = 0;
        LChild = RChild = NULL;
    }
    
    friend class AVL_Tree;
};

class AVL_Tree {
private:
    TreeNode* root; //root contains the pointer to the root of the avl tree.
 
public:
    // Default Constructor of the AVL_Tree
    AVL_Tree()
    {
        root = NULL;
    }
    
	//copy constructor of the AVL_Tree
	AVL_Tree(const AVL_Tree &T){
		root = NULL;
		this -> helperFunc(T.root);
	}
	
    TreeNode* getRoot(); //getter function to get root .
    void insert_data(); // wrapper function for insert(x) for inserting few default value in beginning.
    void insert(int ); // member function used to insert data x in the avl tree.
    void inorder(); // member function for inorder traversal and printing the tree elements.
    void printTree(); // member function for printing the tree using graphviz.
    bool delete_node(int ); // member function to delete a node x from the tree.
    bool search_node(int ); // member function to search for a node with value x.
	~AVL_Tree(); //destructor for class AVL Tree.
	
private:
    void helperFunc(TreeNode *); /* helper function being called by the copy constructor*/   
    void R_Rotate(TreeNode *&p); /* Right Rotation */	
	void L_Rotate(TreeNode *&p); /* Left Rotation */
	void RL_Rotate(TreeNode *&p); /* RL Rotation - Rotate right then left */
	void LR_Rotate(TreeNode *&p); /* LR Rotation - Rotate left then right */
	
};


//destructor for avl_tree to free up dma memory space.
AVL_Tree::~AVL_Tree()
{
    queue<TreeNode*> qu;
    qu.push(root);
    while(!qu.empty())
    {
        TreeNode *temp=qu.front();
        qu.pop();
        
        if(temp->LChild!=NULL)
        {
            qu.push(temp->LChild);
        }
        if(temp->RChild!=NULL)
        {
            qu.push(temp->RChild);
        }
        delete temp;
    }
    
}
//helper function using copy constructor.
void AVL_Tree::helperFunc(TreeNode *root){
	this->insert(root->data);
	
	if(root->LChild)
		this->helperFunc(root->LChild);
	if(root->RChild)
		this->helperFunc(root->RChild);
}

/* RL Rotation - Rotate right then left */
void AVL_Tree::RL_Rotate(TreeNode *&p)
{
	TreeNode *left = p;
	TreeNode *right = p->RChild;
	p = right->LChild;

	right->LChild = p->RChild;
	p->RChild = right;
		
	/* adjusting the balancing factor right->bf */
	(p->bf >= 0) ? right->bf = 0: right->bf = 1;

	left->RChild = p->LChild;
	p->LChild = left;
		
	/* adjusting the balancing factor left->bf */
	(p->bf <= 0) ? left->bf = 0 : left->bf = -1;
		
	/* adjusting the balancing factor ptr */
	p->bf = 0;
}

/* LR Rotation - Rotate left then right */
void AVL_Tree::LR_Rotate(TreeNode *&p)
{
	TreeNode *left = p->LChild;
	TreeNode *right = p;
	p = left->RChild;

	left->RChild = p->LChild;
	p->LChild = left;
		
	/* adjusting the balancing factor left->bf */
	(p->bf <= 0) ? left->bf = 0: left->bf = -1;
		
	right->LChild = p->RChild;
	p->RChild = right;
		
	/* adjusting the balancing factor right->bf */
	(p->bf >= 0) ? right->bf = 0 : right->bf = 1;

			
	/* adjusting the balancing factor ptr */
	p->bf = 0;
}
	
/* Right Rotation */
void AVL_Tree::R_Rotate(TreeNode *&p)
{
	TreeNode *right = p;
	p = right->LChild;
	right->LChild = p->RChild;
	p->RChild = right;
		
	/* adjusting the balancing factor */
	p->bf = 0;
	right->bf = 0;
}
	
/* Left Rotation */
void AVL_Tree::L_Rotate(TreeNode *&p)
{
	TreeNode *left = p;
	p = left->RChild;
	left->RChild = p->LChild;
	p->LChild = left;
		
	/* adjusting the balancing factor */
	p->bf = 0;
	left->bf = 0;
}

// intitilazing the tree with few values ..
void AVL_Tree::insert_data(){
    this->insert(51);
    this->insert(8);
    this->insert(72);
    this->insert(12);
    this->insert(11231);
    this->insert(9);
    this->insert(111);
    this->insert(15);
    this->insert(1);
}



// will return the root of the class 
TreeNode* AVL_Tree::getRoot(){
    return this->root;
}

/* member function used to insert data x in the avl tree. */
void AVL_Tree::insert(int x){
	
	stack<TreeNode *> stk;
	TreeNode *ptr = root, *t = root, *pr = NULL;
	
	//finding the location to which data needs to be inserted.
	while (ptr){
		if (x == ptr->data) return ;
		pr = ptr;
		stk.push(pr);
		(x < ptr->data) ? ptr = ptr->LChild : ptr = ptr->RChild;
	}
	ptr = new TreeNode(x);
	if (pr == NULL){
		root = ptr;
		return ;
	}

	//Link new Node with the tree
	(x < pr->data) ? pr->LChild = ptr : pr->RChild = ptr;
	//Adjusting the balancing factor and tree also.
	while (!stk.empty()){
		pr = stk.top();
		stk.pop();

		(pr->LChild == ptr) ? pr->bf-- : pr->bf++;
		// balanced tree case.
		if (pr->bf == 0) break;
		//Increased Subtree Height
		if (pr->bf == 1 || pr->bf == -1) ptr = pr;
		else{   // |bf| == 2 Balance adjustment required
		
			(pr->bf > 0) ? ((ptr->bf > 0) ? L_Rotate(pr): RL_Rotate(pr)) : ((ptr->bf < 0) ? R_Rotate(pr): LR_Rotate(pr)); 
			break;
		}
	}
	if (stk.empty()) root = pr;
	else{ // handling the edge case.
		TreeNode *q = stk.top();
		(pr->data < q->data) ? q->LChild = pr : q->RChild = pr;
	}
	
	return ;
}



// This function traverse the tree and prints the elements from smallest to largest in sorted order
void AVL_Tree::inorder()
{
    stack<TreeNode *> s;
    TreeNode *curr = root;
 
    while (curr != NULL || s.empty() == false)
    {
        /* Reach the left most Node of the
           curr Node */
        while (curr !=  NULL)
        {
            s.push(curr);
            curr = curr->LChild;
        }
 
        /* Current must be NULL at this point */
        curr = s.top();
        s.pop();
 
        cout << curr->data << " ";
 
        /* we have visited the node and its
           left subtree.  Now, it's right
           subtree's turn */
        curr = curr->RChild;
 
    }
}


// this function prints the graph in graphviz -- image.png file in the same folder of code.
void AVL_Tree::printTree(){
	FILE * fp = fopen("data.dot","w");
	FILE * fp2 = fopen("image.png","w");
	queue<TreeNode*> q;
	fclose(fp2);
	
	TreeNode* temp=root;
	fprintf(fp,"digraph g{\n");
	
	if(temp!=NULL){
		fprintf(fp," label = \" rooted at %d \" ;\n%d [root = true]\n",temp->data,temp->data);
	}
	
	q.push(temp);
	
		
	//now the main logic starts here
	while(!q.empty()){
		temp = q.front();
		q.pop();
        if(temp->LChild!=NULL){
			
			fprintf(fp,"%d %s %d;\n",temp->data,"->",temp->LChild->data);
			q.push(temp->LChild);
			
		}
		
		if(temp->RChild!=NULL){
			fprintf(fp,"%d %s %d;\n",temp->data,"->",temp->RChild->data);
			q.push(temp->RChild);
		}
        
    }
	fprintf(fp,"}");
	fclose(fp);
	system("dot -Tpng data.dot -o image.png");
}

// this function used to search element in the avl tree -  returns true if found else false.
bool AVL_Tree::search_node(int key){
	TreeNode *pr = root;
	
	while(pr){
		
		if(pr->data == key){
			cout<< "Data found at node address: "<<pr<<endl;
			return true;
		}
		else if(pr->data > key){
			pr = pr->LChild;
		}
		else{
			pr = pr->RChild;
		}
		
	}
	return false;
}


/* member function to delete a node x from the tree. */
bool AVL_Tree::delete_node(int key)
{
	/* ptr - stores the pointer to be deleted; pr- stores the parent of ptr (pointer to be deleted); q-stores the child of ptr (pointer node to be deleted) */
	TreeNode *ptr = root, *pr = NULL, *q; 
 	stack<TreeNode*> st;
	
	/*we iterate through this loop until the key matches with the value of any node*/
	while(ptr != NULL){
		if(key == ptr->data)
			break;

		pr = ptr;
		st.push(pr);
		
		(key < ptr->data) ? ptr = ptr->LChild : ptr = ptr->RChild;
			
	}
	
	
	if(ptr == NULL) //it means that element is not found in the tree.
		return false;
	
	/*the node to be deleted has both child. i.e. it is an internal node*/
	if(ptr->LChild && ptr->RChild){
		pr = ptr;
		st.push(pr);

		q = ptr->RChild;
		
		//to find the inorder successor of the node to be deleted
		while(q->LChild){
			pr = q;
			st.push(pr);
			q = q->LChild;
		}
		
		ptr->data = q->data; //copying the value
		ptr = q; //reassigning ptr to the inorder successor
	}
	
	//assigning q to the child node of the node to be deleted
	(ptr->LChild) ? q = ptr->LChild : q = ptr->RChild;

	//ptr- deleted node, q- deleted child node
	if(pr){
		(ptr == pr->LChild) ? pr->LChild = q : pr->RChild = q;
		
		/* Adjust Balancing factor and balancing the tree simulatneoulsy. here we track back to parents till the root node. */
		while(!st.empty())
		{
			pr = st.top();
			st.pop();

			//setting balancing factor of parent.
			(ptr->data < pr->data) ? pr->bf++ : pr->bf--;
			
			//if after setting, if parent bf is -1 or 1, exit.
			if(pr->bf==1 || pr->bf==-1)
				break;

			//if parent bf not equals -1 or 1, then.
			if(pr->bf != 0){
				//Let q point to a higher subtree
				(pr->bf < 0) ? q = pr->LChild : q = pr->RChild;

				if(q->bf == 0){
					if(pr->bf < 0){// handling case 5: having bf <child= 0, parent = -2>
						//R_Rotate(pr);
						TreeNode *right = pr;
						pr = right->LChild;
						right->LChild = pr->RChild;
						pr->RChild = right;
							
						/* adjusting the balancing factor */
						pr->bf = 0;
						right->bf = 0;
						
						pr->bf = 1;
						pr->RChild->bf = -1;
					}
					else{ // handling case 6: having bf <child = 0, parent= 2>
						//L_Rotate(pr);
						TreeNode *left = pr;
						pr = left->RChild;
						left->RChild = pr->LChild;
						pr->LChild = left;
							
						/* adjusting the balancing factor */
						pr->bf = 0;
						left->bf = 0;
						
						
						pr->bf = -1;
						pr->LChild->bf = 1;
					}

					if(!st.empty()){
						TreeNode *ppr = st.top();
						(ppr->data < pr->data) ? ppr->RChild = pr : ppr->LChild = pr;		
					}
					else
						root = pr;
					
					break;
				}

				(pr->bf < 0) ? ((q->bf < 0) ? R_Rotate(pr) : LR_Rotate(pr)) : ((q->bf > 0) ? L_Rotate(pr) : RL_Rotate(pr));
				
				
				if(st.empty()) root = pr;
				else{
					TreeNode *ppr = st.top();
					(ppr->data < pr->data) ? ppr->RChild = pr : ppr->LChild = pr;
				}
				
			}
			q = pr;

		}
	}else{
		root = q;
	}
	
	delete ptr; //deleting the ptr node
	return true;
}

//-----------------------------------------------------------------------------------------------------------------

int main()
{
	AVL_Tree t1;
    t1.insert_data();
    cout << "Inorder Traversal of AVL Tree to start with... \n";
    t1.inorder();
    cout<<endl;
    while(1){
        cout<<"Welcome to the first Assignment on BST. Choose the below option to implement the functionality.\n";
        cout<<"1. insert(x)\n";
        cout<<"2. search(x)\n";
        cout<<"3. delete(x)\n";
        cout<<"4. printTree()\n";
        cout<<"5. Exit Program\n";
        int choice,temp;
        bool is_present;
        cin>> choice;
        
        switch(choice) {
            case 1:
            	cout<<"Enter the data you want to insert!!"<<endl;
            	cin>> temp;
                t1.insert(temp);
                cout<<"after insertion the data is:\n";
                t1.inorder();
                cout<<endl;
                break;
            case 2:
            	cout<<"Enter the data you want to search!!"<<endl;
            	cin>> temp;
                is_present = t1.search_node(temp);
                if(is_present){
					cout<< "data is present.. "<<endl;
				}else{
					cout<< "data is not present.."<<endl;
				}
                break;
            case 3:
                cout<<"Enter the data you want to delete!!"<<endl;
            	cin>> temp;
                t1.delete_node(temp);
                cout<<"after deletion the data is:\n";
                t1.inorder();
                cout<<endl;
                break;
            case 4:
                t1.printTree();
                break;
            case 5:
                cout << "BYE BYE !!\n";
                exit(0);
                break;
            default:
                cout<<"Enter a valid choice.\n";
        }
    }
    
    return 0;
}


