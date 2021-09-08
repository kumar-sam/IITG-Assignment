#include<iostream>
#include<bits/stdc++.h>

using namespace std;

struct Node{
	int val;
	struct Node * next;
};

// Class of the TreeNode
class TreeNode {
    int lthread, rthread;
    int rcount;
    int value;
    TreeNode *left, *right;
 
public:
    // Constructor of tree node.
    TreeNode()
    {
        lthread = -1; rthread = -1;
        value = 0;
        left = right = NULL;
    }
    friend class ThreadedBT;
};

class ThreadedBT {
    TreeNode* root;
 
public:
    // Default Constructor of the ThreadedBT
    ThreadedBT()
    {
        root = NULL;
    }
    
    //copy constructor of the ThreadedBT
	ThreadedBT(const ThreadedBT &T){
		root = NULL;
		//TreeNode *root2 = x.root;
		this -> helperFunc(T.root);
	}
	
    TreeNode* inorderSuccessor(TreeNode*); 
    TreeNode* inorderPredecessor(TreeNode*);
    TreeNode* getRoot();
    void insert_data();
    void insert(int value);
    void inorder();
    void insert_one();
    void delete_one();
    void search();
    void reverseInorder();
    void kthElement();
    void allElementsBetweenk1_k2();
    void split();
    vector<TreeNode  *> splitTree(TreeNode* , int );
    void printGraph();
    void helperFunc(TreeNode *);
};


//helper function using copy constructor.
void ThreadedBT::helperFunc(TreeNode *root){
	this->insert(root->value);
	
	if(root->lthread==0)
		this->helperFunc(root->left);
	if(root->rthread == 0)
		this->helperFunc(root->right);
}


// Function to find the inorder
// successor of the node
TreeNode* ThreadedBT::inorderSuccessor(TreeNode* ptr)
{
    // If rthread is set, we can quickly find.
    if (ptr -> rthread == 1)
        return ptr->right;
 
    // Else return leftmost child of right subtree.
    ptr = ptr -> right;
    while (ptr -> lthread == 0)
        ptr = ptr -> left;
    return ptr;


}

// finding the just smallest element of the given node and return ptr.
TreeNode* ThreadedBT::inorderPredecessor(TreeNode* ptr)
{
    if (ptr->lthread == 1)
        return ptr->left;
 
    ptr = ptr->left;
    while (ptr->rthread == 0)
        ptr = ptr->right;
    return ptr;
}


//searching for an element in the Threaded Binary search tree.
void ThreadedBT::search(){
    cout<<"Enter the data you want to search:\n";
    int data;
    cin>>data;
    
    TreeNode* ptr = new TreeNode();
    ptr = root;
    
    if(ptr==NULL){
        cout<<"DT BST is empty\n";
        return;
    }
    int found=0;
    while (ptr != NULL) {
        if (data == ptr->value) {
            found = 1;
            break;
        }

        if (data < ptr->value) {
            if (ptr->lthread == 0)
                ptr = ptr->left;
            else
                break;
        }
        else {
            if (ptr->rthread == 0)
                ptr = ptr->right;
            else
                break;
        }
    }
    if(found==1){
        cout<<"Data found at address: "<<ptr<<endl;
        return;
    }else{
        cout<<"Data not found\n";
        return;
    }
    
}

// intitilazing the tree with few values ..
void ThreadedBT::insert_data(){
    this->insert(51);
    this->insert(8);
    this->insert(72);
    this->insert(11);
    this->insert(11231);
    this->insert(9);
    this->insert(111);
    this->insert(31);
    this->insert(1);
}


// this method will take user input and will delete the element if present and will print the tree.
void ThreadedBT::delete_one(){
    cout<< "enter the data you want to delete\n";
    int key;
    cin >> key;

    TreeNode* par = NULL;
    TreeNode* ptr = root;
 
    // Set true if key is found
    int found = 0;
 
    while (ptr != NULL) {
        if (key == ptr->value) {
            found = 1;
            break;
        }
        par = ptr;
        if (key < ptr->value) {
            if (ptr->lthread == 0)
                ptr = ptr->left;
            else
                break;
        }
        else {
            if (ptr->rthread == 0)
                ptr = ptr->right;
            else
                break;
        }
    }
 
    if (found == 0){
    	printf("key not present in tree\n");
        return;
    }
    
    //both child exists --------------------------------------------------------------    
    else if (ptr->lthread == 0 && ptr->rthread == 0)
    {
        // Find inorder successor and its parent.
        TreeNode* parsucc = ptr;
        TreeNode* succ = ptr->right;
     
        // Find leftmost child of successor
        while (succ->lthread==0) {
            parsucc = succ;
            succ = succ->left;
        }
     
        ptr->value = succ->value;
     
        if (succ->lthread == 1 && succ->rthread == 1){
            TreeNode* par = parsucc;
            TreeNode* ptr = succ;
            
            if (par == NULL)
                root = NULL;
         
            else if (ptr == par->left) {
                par->lthread = 1;
                par->left = ptr->left;
            }
            else {
                par->rthread = 1;
                par->right = ptr->right;
            }
         
            free(ptr);
            return;
        }

        else{
            TreeNode* par = parsucc;
            TreeNode* ptr = succ;
            TreeNode* child;
     
            if (ptr->lthread == 0)
                child = ptr->left;
         
            else
                child = ptr->right;
         
            if (par == NULL)
                root = child;
         
            else if (ptr == par->left)
                par->left = child;
            else
                par->right = child;
         
            TreeNode* s = inorderSuccessor(ptr);
            TreeNode* p = inorderPredecessor(ptr);
         
            if (ptr->lthread == 0)
                p->right = s;
         
            else {
                if (ptr->rthread == 0)
                    s->left = p;
            }
         
            free(ptr);
            return;
        }
            
     
        return;
    }
 
    // Either Left or Right Child exists -----------------------------------------------------------
    else if (ptr->lthread == 0 || ptr->rthread == 0){
        TreeNode* child;
     
        if (ptr->lthread == 0)
            child = ptr->left;
     
        else
            child = ptr->right;
     
        if (par == NULL)
            root = child;
     
        else if (ptr == par->left)
            par->left = child;
        else
            par->right = child;
     
        TreeNode* s = inorderSuccessor(ptr);
        TreeNode* p = inorderPredecessor(ptr);
     
        if (ptr->lthread == 0)
            p->right = s;
     
        else {
            if (ptr->rthread == 0)
                s->left = p;
        }
     
        free(ptr);
        return;
    }
 
    // No child exists --------------------------------------------------------------------------------
    else{
        
        if (par == NULL)
            root = NULL;
     
        else if (ptr == par->left) {
            par->lthread = 1;
            par->left = ptr->left;
        }
        else {
            par->rthread = 1;
            par->right = ptr->right;
        }
     
        free(ptr);
        return;
    }
 
}


// will return the root of the class 
TreeNode* ThreadedBT::getRoot(){
    return this->root;
}


// this method will take user input and will inssert the element if not present and will print the tree.
void ThreadedBT::insert_one(){
    int data;
    cout<<"Enter the data you want to insert:\n";
    cin>>data;
    this->insert(data);
    cout<<"after insertion the data is:\n";
    this->inorder();
}


// return the head of the linked list
void ThreadedBT::reverseInorder(){
    cout<<"Reverse Inorder data is: \n";
    TreeNode* temp = new TreeNode();
    Node *head = NULL;
    temp = root;
    while(temp->rthread!=1){
        temp = temp->right;
    }
    
    while(temp!=NULL){
    	Node *ptr = new Node;
        if(head==NULL){
        	ptr->next = NULL;	
		}else{
			ptr->next = head;
		}
		ptr-> val = temp->value;
		head = ptr;
        cout<<" "<<temp->value;
        temp = inorderPredecessor(temp);
        
    }
    cout<<endl;
    cout<<"Head of linked list is "<< head<<endl;
    
}

//finding the kth element of the Threaded Binary Search Tree...
void ThreadedBT::kthElement(){
    cout<<"Enter value of k: \n";
    int k;
    cin>>k;
    TreeNode* temp = new TreeNode();
    temp = root;
    while(temp->rthread!=1){
        temp = temp->right;
    }
    
    while(k!=1){
        k--;
        temp = inorderPredecessor(temp);
    }
    cout<<"kth largest value is: "<<temp->value;
    cout<<endl;
}

//finding all element between k1 and k2...
void ThreadedBT::allElementsBetweenk1_k2(){
    cout<<"Enter value of k1 and k2: \n";
    int k1,k2;
    cin>>k1>>k2;
    TreeNode* temp = new TreeNode();
    Node *head = NULL;
    // TreeNode* parent = NULL;
    temp = root;
    
    
    while (temp != NULL) {
        // parent = temp;
        if ( temp->value <= k2) {
            if (temp->rthread == 0)
                temp = temp->right;
            else
                break;
        }else{
            if (temp->lthread == 0)
                temp = temp->left;
            else
                break;
        }
    }
    
    cout<<endl;
    // temp  = parent;
    // temp = temp->left;
    while(temp != NULL){
        if(temp->value >=k1){
        	Node *ptr = new Node;
        	if(head==NULL){
        		ptr->next = NULL;	
			}else{
				ptr->next = head;
			}
			ptr-> val = temp->value;
			head = ptr;
            cout<<temp->value<<' ';
            temp = inorderPredecessor(temp);
        }else{
            break;
        }
        
    }
    cout<<endl;
    cout<<"Head of linked list is "<< head<<endl;
    
}


// Function to insert the nodes into the threaded binary search tree
void ThreadedBT::insert(int data)
{
    // Condition to check if there
    // is no node in the binary tree
    if (root == NULL) {
 
        TreeNode* p = new TreeNode();
        p->value = data;
        p->left = NULL;
        p->lthread = -1;
        p->rthread = -1;
        p->right = NULL;
 
        root = p;
        return ;
    }
 
    // New node
    TreeNode* ptr = new TreeNode;
    TreeNode* par = new TreeNode;
    ptr = root; par = NULL;

    while (ptr != NULL)
    {
        // If key already exists, return
        if (data == (ptr->value))
        {
            printf("Duplicate Key !\n");
            return ;
        }
 
        par = ptr; // Update parent pointer
 
        // Moving on left subtree.
        if (data < ptr->value)
        {
            if (ptr -> lthread == 0)
                ptr = ptr -> left;
            else
                break;
        }
 
        // Moving on right subtree.
        else
        {
            if (ptr->rthread == 0)
                ptr = ptr -> right;
            else
                break;
        }
    }
 
    // Create a new node
    TreeNode* tmp = new TreeNode();
    tmp -> value = data;
    tmp -> lthread = 1;
    tmp -> rthread = 1;
 
   
    if (data < (par -> value))
    {
        tmp -> left = par -> left;
        tmp -> right = par;
        par -> lthread = 0;
        par -> left = tmp;
    }
    else
    {
        tmp -> left = par;
        tmp -> right = par -> right;
        par -> rthread = 0;
        par -> right = tmp;
    }
 
    return ;
}
 

// This function traverse the tree and prints the elements from smallest to largest in sorted order
void ThreadedBT::inorder()
{
    TreeNode* ptr;
    ptr = root;

    if (root == NULL)
        printf("Tree is empty\n");

    while (ptr -> lthread == 0)
        ptr = ptr -> left;

    // One by one print successors
    while (ptr != NULL)
    {
        cout << " "<< ptr->value;
        ptr = inorderSuccessor(ptr);
    }
    cout<<endl;
}



// this function prints the graph in graphviz -- image.png file in the same folder of code.
void ThreadedBT::printGraph(){
	FILE * fp = fopen("data.dot","w");
	FILE * fp2 = fopen("image.png","w");
	queue<TreeNode*> q;
	fclose(fp2);
	
	TreeNode* temp=root;
	fprintf(fp,"digraph g{\n");
	
	if(temp!=NULL){
		fprintf(fp," label = \" rooted at %d \" ;\n%d [root = true]\n",temp->value,temp->value);
	}
	
	q.push(temp);
	
		
	//now the main logic starts here
	while(!q.empty()){
		temp = q.front();
		q.pop();
        if(temp->left!=NULL){
			if(temp -> lthread == 1){
				fprintf(fp,"%d %s %d [style = dotted];\n",temp->value,"->",temp->left->value);
			}else{
				fprintf(fp,"%d %s %d;\n",temp->value,"->",temp->left->value);
				q.push(temp->left);
			}
		}
		
		if(temp->right!=NULL){
			if(temp->rthread == 1){
				fprintf(fp,"%d %s %d [style = dotted];\n",temp->value,"->",temp->right->value);
			}else{
				fprintf(fp,"%d %s %d;\n",temp->value,"->",temp->right->value);
				q.push(temp->right);
			}
		}
        //temp = inorderPredecessor(temp);
        
    }
	fprintf(fp,"}");
	fclose(fp);
	system("dot -Tpng data.dot -o image.png");
}


//helper function of split
vector<TreeNode  *> ThreadedBT::splitTree(TreeNode* r, int v){
	vector<TreeNode *> ans(2, NULL);
	
	if(r == NULL) return ans;
	
	int x,y;
	
	if(r->value > v){
		x = 1;
		y = 0;
	}
	else{
		x=0;
		y=1;
	}

	TreeNode * node = r->value > v ? (r->lthread==0 ? r->left: NULL ) : (r->rthread ==0 ? r->right: NULL );
	
	ans[x] = r;
	auto t = splitTree(node, v);
	
	if(r->value>v){
		r->left = t[x];
	}else{
		r->right = t[x];
	}
	ans[y] = t[y];
	
	return ans;
}

//this method splits the tree based on k and prints the tree. and before this copy constructor will be called.
void ThreadedBT::split(){
	int k;
	cout<<"Enter the value of k" << endl;
	cin>>k;
	
	vector<TreeNode *> v = splitTree(root, k);
	
	for(int i=1; i>=0; i--){
		TreeNode* ptr;
	    ptr = v[i];
		
	    if (ptr == NULL)
	        printf("Tree is empty\n");
		
	    while (ptr -> lthread == 0)
	        ptr = ptr -> left;
	
	    // One by one print successors
	    while (ptr != NULL)
	    {
	        cout << " "<< ptr->value;
	        ptr = inorderSuccessor(ptr);
	    }
	    cout<<endl;
	}
	return;
}

//-----------------------------------------------------------------------------------------------------------------
int main()
{
	ThreadedBT t1;
    t1.insert_data();
    ThreadedBT t2 = t1;
    cout << "Inorder Traversal of Threaded BST to start with... \n";
    t1.inorder();
    while(1){
        cout<<"Welcome to the first Assignment on BST. Choose the below option to implement the functionality.\n";
        cout<<"1. insert (x)\n";
        cout<<"2. search(x)\n";
        cout<<"3. delete(x)\n";
        cout<<"4. reverseInorder()\n";		
        cout<<"5. successor(ptr)\n";
        cout<<"6. split(k)\n";
        cout<<"7. allElementsBetween(k1, k2)\n";
        cout<<"8. kthElement(k)\n";
        cout<<"9. printTree()\n";
        cout<<"10. Exit Program\n";
        int choice;
        cin>> choice;
        
        switch(choice) {
            case 1:
                t1.insert_one();
                break;
            case 2:
                t1.search();
                break;
            case 3:
                t1.delete_one();
                t1.inorder();
                break;
            case 4:
                t1.reverseInorder();
                break;
            case 5:
                TreeNode *temp;
     			temp = t1.inorderSuccessor(t1.getRoot());
    			cout<< "Inorder successor is :"<<temp<<endl;
                break;
            case 6:
            	t2.inorder();
                t2.split();
                break;
            case 7:
                t1.allElementsBetweenk1_k2();
                break;
            case 8:
                t1.kthElement();
                break;
            case 9:
                t1.printGraph();
                break;
            case 10:
                cout << "BYE BYE !!\n";
                exit(0);
            default:
                cout<<"Enter a valid choice.\n";
        }
    }

    return 0;
}


