#include<iostream>
#include<bits/stdc++.h>
#include "avl_header.h"
#include "bst_header.h"
#include "treap_header.h"

using namespace std;


const char * fname[5] = {"sample_a.txt","sample_b.txt","sample_c.txt","sample_d.txt","sample_e.txt"};

/*this function used to generate the test cases. */
void generate_testcase(){
	int cntr=0;
	for(int k=0; k<5;k++){
		vector<int> inserted_data;
		cntr++;
		FILE* file = fopen(fname[k], "w");
		for(int i=1; i<= (6000+k*1000); i++){
			int val = rand()%100000;
			if(i%9 == 0 || i%8 == 0){
				int index = rand() % inserted_data.size();
				int data = inserted_data[index];
				int operation = 0; 
				fprintf(file,"%d\t%d\n",operation,data);
			}else{
				inserted_data.push_back(val);
				int operation = 1; 
				fprintf(file,"%d\t%d\n",operation,val);
			}
		}
	    fclose(file);
	}
}


//-----------------------------------------------------------------------------------------------------------------

int main()
{
	srand(time(NULL));
	//printf("TGenetating Test case dataset.....\n");
	//generate_testcase();  // uncomment this line to change the sample testcase file.
	//printf("Test case data generated successfully.\n");
	
	for(int i=0; i<5; i++){ //iterates for each file.
	
		int operation;
		int key;
		int cnt_insert, cnt_delete;
		//for avl----------------------------------------------------------------------
		cnt_insert=0; cnt_delete=0;
		AVL_Tree avl;
		FILE* file = fopen(fname[i], "r");
		while (fscanf(file,"%d\t%d\n",&operation,&key)!= EOF)
	    {	
	    	if(operation==1){
	    		cnt_insert++;
	    		avl.insert(key);
			}else{
				cnt_delete++;
				avl.delete_node(key);
			}
	    }
		fclose(file);
		cout<<"\nAVL - file "<< fname[i]<<" Insert: "<<cnt_insert<< " Delete: "<<cnt_delete<<endl;
		avl.find_height();
		avl.print_eval_metrics();
		
		//for bst----------------------------------------------------------------------------
		cnt_insert=0; cnt_delete=0;
		BST bst;
		file = fopen(fname[i], "r");
		while (fscanf(file,"%d\t%d\n",&operation,&key)!= EOF)
	    {	
	    	if(operation==1){
	    		cnt_insert++;
	    		bst.insert(key);
			}else{
				cnt_delete++;
				bst.remove(key);
			}
	    }
		fclose(file);
		cout<<"\nBST - file "<< fname[i]<<" Insert: "<<cnt_insert<< " Delete: "<<cnt_delete<<endl;
		bst.find_height();
		bst.print_eval_metrics();
		
		
		//for treap---------------------------------------------------------------------------------
		cnt_insert=0; cnt_delete=0;
		Treap treap;
		file = fopen(fname[i], "r");
		while (fscanf(file,"%d\t%d\n",&operation,&key)!= EOF)
	    {	
	    	if(operation==1){
	    		cnt_insert++;
	    		treap.insert(key);
			}else{
				cnt_delete++;
				treap.remove(key);
			}
	    }
		fclose(file);
		cout<<"\nTreap - file "<< fname[i]<<" Insert: "<<cnt_insert<< " Delete: "<<cnt_delete<<endl;
		treap.find_height();
		treap.print_eval_metrics();
		
	}
   
    return 0;
}




