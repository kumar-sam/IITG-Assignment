#include<bits/stdc++.h>
using namespace std;

// Class of the TreeNode
class BSTNode {
private:
    int data;
    BSTNode  *left, *right; 
 
public:
    // Constructor of tree node with two param.
    BSTNode(int value)
    {
        data = value;
        left = right = NULL;
    }
    
    friend class BST;
};


class BST {
private:
    BSTNode *root;
    int total_key_comp_insert_delete, height_final, cnt_element, avg_height_each_element, total_rotation;
    void insert(BSTNode*, int );
    struct BSTNode * minValueNode(struct BSTNode* node);
    struct BSTNode* delete_node(struct BSTNode* root, int data);
    void tree_print_helper(const BSTNode *, ofstream &); //helper function for treap_print_with_bf()
    int maxDepth(BSTNode* node);
    
public:
    BST();
    void insert(int value);
    void remove(int x);
    bool search(int x);
    void inorder();
    void tree_print();
    void find_height();
    void print_eval_metrics();
};

BST::BST() {
    root = NULL;
    total_key_comp_insert_delete=0;
    total_rotation =0;
    cnt_element = 0;
	height_final=0;
	avg_height_each_element=0;
}

void BST::insert(int value) {
    insert(root, value);
}

void BST::find_height() {
    height_final = maxDepth(root);
}

void BST::print_eval_metrics(){
	cout << center("total key comp",20) << " | " << center("height final",20) << " | " << center("avg height element",20) << " | " << center("totalrotation",20)<<"\n";
	std::cout << std::string(20*4 + 3*3, '-') << "\n";
	cout<<prd(total_key_comp_insert_delete,0,20)<<" | "<<prd(height_final,0,20)<<" | "<<prd(avg_height_each_element/cnt_element, 0, 20)<<" | "<<prd(total_rotation,0,20)<<endl;
	return;
}


int BST::maxDepth(BSTNode* node)
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
/*this member function used to insert values in bst*/
void BST::insert(BSTNode *treeNode, int data)
{
    if (!treeNode)
    {
        treeNode = new BSTNode(data);           
        root = treeNode;           
    }
    else
    {
    	total_key_comp_insert_delete++;
    	if(data == treeNode->data){
    		return;
		}
        else if (data < treeNode->data)
        {
            if (!treeNode->left)
            {
                BSTNode *treeTemp = new BSTNode(data);
                treeNode->left = treeTemp;
            }
            else
                insert(treeNode->left, data);
        }
        else
        {
            if (!treeNode->right)
            {
                BSTNode *treeTemp = new BSTNode(data);                         
                treeNode->right = treeTemp;
            }
            else
                insert(treeNode->right, data);
        }
    }
}

void BST::remove(int value) {
    delete_node(root, value);
}

void BST::tree_print(){
	ofstream fout;
  	string dot_file = "data.dot";
  	string png_file = "image.png";

  	fout.open(dot_file.c_str()); // open dot file for writing

  	fout << "digraph g {\n";
  	fout << "node [shape=record, height=0.1];\n";
  	tree_print_helper(root, fout);
  	fout << "}";
  	fout.close(); // close dot file

  	string str = "dot -Tpng data.dot -o image.png";
  	const char *command = str.c_str();
  	system(command); // system call to run the dot file using graphviz

  	cout << "Success!! check the " << png_file << " file.\n";
}

//helper function for print
void BST::tree_print_helper(const BSTNode *node, ofstream &fout)
{
  	if (!node) // if node is NULL
    	return;

  	if (node == root){ // add the label and root in the dot file
   		fout << "label = \" rooted at " << node->data << " \";\n";
    	fout << node->data << " [root = true]\n";
  	}
  
  	fout << node->data << " [label=\"<f0>|<f1>" << node->data << " |<f2> \"];\n";

  	if (node->left){ // if left child exists
    	fout << node->data << ":f0 -> " << node->left->data << ":f1\n"; // write edge in dot file
    	tree_print_helper(node->left, fout);                            // recurse for left subtree
  	}
  	if (node->right){ // if right child exists
    	fout << node->data << ":f2 -> " << node->right->data << ":f1\n"; // write edge in dot file
    	tree_print_helper(node->right, fout);                            // recurse for right subtree
  	}
}

// This function traverse the tree and prints the elements from smallest to largest in sorted order
void BST::inorder()
{
    stack<BSTNode *> s;
    BSTNode *curr = root;
 
    while (curr != NULL || s.empty() == false)
    {
        /* Reach the left most BSTNode of the
           curr BSTNode */
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

/* Function to find the minimum value node */
struct BSTNode * BST::minValueNode(struct BSTNode* node)
{
	struct BSTNode* current = node;
	/* loop down to find the leftmost leaf */
	while (current->left != NULL)
		current = current->left;
		
	return current;
}

/* Function to delete the given node */
struct BSTNode* BST::delete_node(struct BSTNode* root, int data)
{
	if (root == NULL)
		return root;
	total_key_comp_insert_delete++;	
	// If the key to be deleted is smaller than the root's key,
	if (data < root->data){
		root->left = delete_node(root->left, data);
	}
		
	// If the key to be deleted is greater than the root's key,
	else if (data > root->data){
		root->right = delete_node(root->right, data);
	}
		
	else
	{
		// node with only one child or no child
		if (root->left == NULL)
		{
			struct BSTNode *temp = root->right;
			free(root);
			return temp;
		}
		else if (root->right == NULL)
		{
			struct BSTNode *temp = root->left;
			free(root);
			return temp;
		}
		
		// node with two children:
		struct BSTNode* temp = minValueNode(root->right);
		
		// Copy the inorder successor's content to this node
		root->data = temp->data;
		
		// Delete the inorder successor
		root->right = delete_node(root->right, temp->data);
	}
	return root;
}


bool BST::search(int value) {
    BSTNode *p = root;
    while (p) {
        if (p->data == value) {
            return true;
        } else {
            p = p->data < value ? p->right : p->left;
        }
    }
    return false;
}

