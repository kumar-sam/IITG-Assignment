#include<bits/stdc++.h>
using namespace std;
unordered_set<int> is_taken;
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
    void treap_print(); //treap print wrapper function.
    void find_height(); //finding height wrapper member function.
    void print_eval_metrics(); //prints evaluation metrics for each of the tree
};

Treap::Treap() {
    root = NULL;
    total_key_comp_insert_delete=0;
	total_rotation =0;
	cnt_element = 0;
	height_final=0;
	avg_height_each_element=0;
}

void Treap::find_height() {
    height_final = maxDepth(root);
}

void Treap::print_eval_metrics(){
	cout << center("total key comp",20) << " | " << center("height final",20) << " | " << center("avg height element",20) << " | " << center("totalrotation",20)<<"\n";
	std::cout << std::string(20*4 + 3*3, '-') << "\n";
	cout<<prd(total_key_comp_insert_delete,0,20)<<" | "<<prd(height_final,0,20)<<" | "<<prd(avg_height_each_element/cnt_element, 0, 20)<<" | "<<prd(total_rotation,0,20)<<endl;
	return;
}

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

void Treap::leftRotate(Node* &p) {
	total_rotation++;
    Node *k = p->right;
    p->right = k->left;
    k->left = p;
    p = k;
}

void Treap::rightRotate(Node* &p) {
	total_rotation++;
    Node *k = p->left;
    p->left = k->right;
    k->right = p;
    p = k;
}

void Treap::insert(int value) {
    insert(root, value);
}

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
        if(p->left && p->left->priority < p->priority) {
            rightRotate(p);
        } else if(p->right && p->right->priority < p->priority) {
            leftRotate(p);
        }
    }
}

void Treap::remove(int value) {
    remove(root, value);
}

void Treap::treap_print(){
	ofstream fout;
  	string dot_file = "data.dot";
  	string png_file = "image.png";

  	fout.open(dot_file.c_str()); // open dot file for writing

  	fout << "digraph g {\n";
  	fout << "node [shape=record, height=0.1];\n";
  	treap_print_helper(root, fout);
  	fout << "}";
  	fout.close(); // close dot file

  	string str = "dot -Tpng data.dot -o image.png";
  	const char *command = str.c_str();
  	system(command); // system call to run the dot file using graphviz

  	cout << "Success!! check the " << png_file << " file.\n";
}

//helper function for print
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

// This function traverse the tree and prints the elements from smallest to largest in sorted order
void Treap::inorder()
{
    stack<Node *> s;
    Node *curr = root;
 
    while (curr != NULL || s.empty() == false)
    {
        /* Reach the left most Node of the
           curr Node */
        while (curr !=  NULL)
        {
            s.push(curr);
            curr = curr->left;
        }
 
        /* Current must be NULL at this point */
        curr = s.top();
        s.pop();
 
        cout << curr->data << " ";
 
        /* we have visited the node and its
           left subtree.  Now, it's right
           subtree's turn */
        curr = curr->right;
 
    }
}

void Treap::remove(Node* &p, int value) {
	total_key_comp_insert_delete++;
	if(!p){
		//cout <<"Data not found. "<<endl;
		return;
	}
    else if(p->data == value) {	
        if (p->left == NULL) {
            p = p->right;
        } else if (p->right == NULL) {
            p = p->left;
        } else {
            if (p->left->priority < p->right->priority) {
                rightRotate(p);
                remove(p->right, value);
            } else if (p->left->priority > p->right->priority) {
                leftRotate(p);
                remove(p->left, value);
            }
        }
    } else{
        if (value < p->data) {
            remove(p->left, value);
        } else {
            remove(p->right, value);
        }
    }
}

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
