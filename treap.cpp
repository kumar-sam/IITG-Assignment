#include<bits/stdc++.h>
using namespace std;
unordered_set<int> is_taken; //keeps track of generated priority number

// Class of the TreeNode
class Node {
private:
    int data;
    int priority;
    Node  *left, *right; 
 
public:
    // Constructor of tree node with two param.
    Node(int value, int level)
    {
        data = value;
        
        //handling priority----------
        while(1){
        	level = rand()%1000000;
			if(is_taken.find(level)== is_taken.end()){
				is_taken.insert(level);
				break;
			}
		}
        priority = level;
        left = right = NULL;
    }
    
    friend class Treap;
};

/* main treap class which contains the member variable root of tree and member functions to do operations */
class Treap {
private:
    Node *root; //stores root of treap
    int total_key_comp_insert_delete, height_final, cnt_element, avg_height_each_element, total_rotation; //metrics to use for analysis
    void leftRotate(Node* &p); //left rotation helper function
    void rightRotate(Node* &p); //right rotation helper function
    void insert(Node* &p, int value); //insert recursive function
    void remove(Node* &p, int value); //delete recursive function
    void treap_print_helper(const Node *, ofstream &); //helper function for treap_print_with_bf()
    int maxDepth(Node* node); //calculates max depth and depth of each node.
    
public:
    Treap(); //Treap constructor
    void insert(int value); //insert wrapper member function
    void remove(int x); //remover wrapper member function
    bool search(int x); //search wrapper member function
    int smallest(); //caluclates the smallest of all.
    void inorder(); // inorder print the element member function
    void treap_print(string); //treap print wrapper function.
    void find_height(); //finding height wrapper member function.
    void print_eval_metrics(); //prints evaluation metrics for each of the tree
};

/*Treap constructor*/
Treap::Treap() {
    root = NULL;
    total_key_comp_insert_delete=0;
	total_rotation =0;
	cnt_element = 0;
	height_final=0;
	avg_height_each_element=0;
}

/*find height is a wrapper function to find height*/
void Treap::find_height() {
    height_final = maxDepth(root);
}

/* prints evaluation metrics for each of the file. called at the end of insert/delete operation*/
void Treap::print_eval_metrics(){
	cout<<total_key_comp_insert_delete<<" "<<height_final<<" "<<cnt_element<<" "<<(avg_height_each_element/cnt_element)<<" "<<total_rotation<<" "<<endl;
	return;
}

/* calculates max depth of the tree*/
int Treap::maxDepth(Node* node)
{
    if (node == NULL)
        return 0;
    else
    {
        /* compute the depth of each subtree */
        cnt_element++;
        int lDepth = maxDepth(node->left);
        int rDepth = maxDepth(node->right);
     
        /* use the larger one */
        if (lDepth > rDepth){
        	avg_height_each_element+= (lDepth + 1);
        	return(lDepth + 1);
		}else{
			avg_height_each_element+= (rDepth + 1);
			return(rDepth + 1);
		}
            
    }
}

/*left rotates the tree based on priority*/
void Treap::leftRotate(Node* &p) {
	total_rotation++;
    Node *k = p->right;
    p->right = k->left;
    k->left = p;
    p = k;
}

/*right rotates the tree based on priority*/
void Treap::rightRotate(Node* &p) {
	total_rotation++;
    Node *k = p->left;
    p->left = k->right;
    k->right = p;
    p = k;
}

/*wrapper function to insert the element in treap*/
void Treap::insert(int value) {
    insert(root, value);
}

/*function to insert the value inside the treap*/
void Treap::insert(Node* &p, int value) {
	
    if (p == NULL) {
        p = new Node(value, rand());
    } else {
        total_key_comp_insert_delete++;
        if (value == p->data) {
            return;
        } 
		else if (value < p->data) {
            insert(p->left, value);
        } 
		else {
            insert(p->right, value);
        }
		total_key_comp_insert_delete++;
        if(p->left && p->left->priority < p->priority) {
            rightRotate(p);
        } else if(p->right && p->right->priority < p->priority) {
            leftRotate(p);
        }
    }
}

/*wrapper function to delete the element from the tree*/
void Treap::remove(int value) {
    remove(root, value);
}

/*wrapper function to print the treap*/
void Treap::treap_print(string png_file_name){
	ofstream fout;
  	string dot_file = "data.dot";
  	string png_file = png_file_name +".png";

  	fout.open(dot_file.c_str()); // open dot file for writing

  	fout << "digraph g {\n";
  	fout << "node [shape=record, height=0.1];\n";
  	treap_print_helper(root, fout);
  	fout << "}";
  	fout.close(); // close dot file

  	string str = "dot -Tpng data.dot -o "+png_file;
  	const char *command = str.c_str();
  	system(command); // system call to run the dot file using graphviz

  	cout << "Success!! check the " << png_file << " file.\n";
}

/*helper function for print */
void Treap::treap_print_helper(const Node *node, ofstream &fout)
{
  	if (!node) // if node is NULL
    	return;

  	if (node == root){ // add the label and root in the dot file
   		fout << "label = \" rooted at " << node->data << " \";\n";
    	fout << node->data << " [root = true]\n";
  	}
  
  	fout << node->data << " [label=\"<f0>|<f1>" << node->data << "|<f2> " << node->priority << " |<f3> \"];\n";

  	if (node->left){ // if left child exists
    	fout << node->data << ":f0 -> " << node->left->data << ":f1\n"; // write edge in dot file
    	treap_print_helper(node->left, fout);                            // recurse for left subtree
  	}
  	if (node->right){ // if right child exists
    	fout << node->data << ":f3 -> " << node->right->data << ":f1\n"; // write edge in dot file
    	treap_print_helper(node->right, fout);                            // recurse for right subtree
  	}
}

/* This function traverse the tree and prints the elements from smallest to largest in sorted order */
void Treap::inorder()
{
    stack<Node *> s;
    Node *curr = root;
 
    while (curr != NULL || s.empty() == false){
        while (curr !=  NULL){
            s.push(curr);
            curr = curr->left;
        }
        /* Current must be NULL at this point */
        curr = s.top();
        s.pop();
        cout << curr->data << " ";
        curr = curr->right;
    }
}

/*helper function to delete the element from tree*/
void Treap::remove(Node* &p, int value) {
	if(!p){
		cout <<"Data not found. "<<endl;
		return;
	}
    else if(p->data == value) { //data to be deleted found
    	total_key_comp_insert_delete++;
        if (p->left == NULL) {
            p = p->right;
        } else if (p->right == NULL) {
            p = p->left;
        } else {
            if (p->left->priority < p->right->priority) { //left priority less than right
                rightRotate(p); //rotation required
                remove(p->right, value);
            } else if (p->left->priority > p->right->priority) { //right priority less than left
                leftRotate(p); //left rotation required
                remove(p->left, value);
            }
        }
    } else{
    	total_key_comp_insert_delete+=2;
        if (value < p->data) {
            remove(p->left, value); //recursive call
        } else {
            remove(p->right, value); //recursive call
        }
    }
}

/*member function to search element in the tree*/
bool Treap::search(int value) {
    Node *p = root;
    while (p) {
        if (p->data == value) {
            return true;
        } else {
            p = p->data < value ? p->right : p->left;
        }
    }
    return false;
}

int Treap::smallest() {
    Node* p = root;
    int value;
    while (p) {
        value = p->data;
        p = p->left;
    }
    return value;
}

/*this is the point from where execution begins. */
int main(int argc, char const *argv[]) {
	srand(time(NULL));
    Treap treap;
    
     while(1){
        cout<<"Welcome to the Treap Program. Choose the below option to implement the functionality.\n";
        cout<<"1. insert(x)\n";
        cout<<"2. search(x)\n";
        cout<<"3. delete(x)\n";
        cout<<"4. printTree()\n";
        cout<<"5. Exit Program\n";
        int choice,temp;
        bool is_present;
        string print_file_name;
        cin>> choice;
        
        switch(choice) {
            case 1:
            	cout<<"Enter the data you want to insert!!"<<endl;
            	cin>> temp;
                treap.insert(temp);
                cout<<"after insertion the data is:\n";
                treap.inorder();
                cout<<endl;
                break;
            case 2:
            	cout<<"Enter the data you want to search!!"<<endl;
            	cin>> temp;
                is_present = treap.search(temp);
                if(is_present){
					cout<< "data is present.. "<<endl;
				}else{
					cout<< "data is not present.."<<endl;
				}
                break;
            case 3:
                cout<<"Enter the data you want to delete!!"<<endl;
            	cin>> temp;
                treap.remove(temp);
                cout<<"after deletion the data is:\n";
                treap.inorder();
                cout<<endl;
                break;
            case 4:
            	cout<<"Enter the file name without extension.\n"<<endl;
            	cin>>print_file_name;
                treap.treap_print(print_file_name); //--- full fledged function with bf implementation.
                break;
            case 5:
            	treap.find_height();
            	treap.print_eval_metrics();
                cout << "BYE BYE !!\n";
                exit(0);
                break;
            default:
                cout<<"Enter a valid choice.\n";
        }
    }
   
    return 0;
}

