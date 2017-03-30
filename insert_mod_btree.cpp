#include<stdio.h>
#include<iostream>
#include<string.h>
#include<string>
#include<vector>
#include<fstream>
#include<stdlib.h>
using namespace std;
struct BTreeNode
{
	int *data;
	BTreeNode **child_ptr;
	bool leaf;
	bool root;
	int n;
}
*root = NULL, *np = NULL, *x = NULL;
int buf_sz, M=3, B;
BTreeNode * init() //init function (returns a btreenode)
{
	int i;
	np = new BTreeNode;
	np->data = new int[buf_sz+1]; // no of keys in a block
	np->child_ptr = new BTreeNode *[buf_sz+1]; //no of child pointers (n+1)
	np->leaf = true;
	np->root = false;	
	np->n = 0; //no filled
	for (i = 0; i < buf_sz+1; i++)
	{
		np->child_ptr[i] = NULL;
	}
	return np;
}
struct Tuple {
	int a;
	BTreeNode *b;
}*r = NULL;
Tuple * tuple_init()
{
	r = new Tuple;
	r->a = 0;
	r->b = init();
}
Tuple *find(int inx);
int count(int inx)
{
	int count_num =0,flag_find = 0;
	Tuple *find_result = find(inx);
	BTreeNode *temp_find = find_result->b;
	int k;	
/*	for(k=0;k<temp_find->n;k++)
	{
		cout << "temp_find->data[k]" <<  temp_find->data[k] << endl;
	}*/
	if(temp_find == NULL || find_result->a==0)
		return count_num;
	else
	{
		while(temp_find!=NULL)
		{
	/*		for(k=0;k<temp_find->n;k++)
			{
				cout << "temp_find->data[k]" <<  temp_find->data[k] << endl;
			}*/
			int lll;
			for(lll=0;lll<temp_find->n;lll++)
			{
			//	cout << "temp_find->data[lll]" <<  temp_find->data[lll] << endl;
				if(inx == temp_find->data[lll])
					count_num++;
				//		else if(inx > temp_find->data[lll] && count_num > 0)
				//		{
				//			flag_find = 1;
				//			break;
				//		}
				else if(inx < temp_find->data[lll])
					break;
			}
			//	if(flag_find == 1)
			//		break;
			//		else
			temp_find = temp_find->child_ptr[0];
		}	
		return count_num;
	}
}
int equal(BTreeNode *node1,BTreeNode *node2)
{
	if(node1->n != node2->n)
		return 0;
	if(node1->leaf!=node2->leaf)
		return 0;
	int pp,flag = 0;
	for(pp=0;pp<node1->n;pp++)
	{
		if(node1->data[pp]!=node2->data[pp])
			flag = 1;
		break;
	}
	if(flag == 1)
		return 0;
	return 1;	
}
void sort(int *p, int n)
{
	int i, j, temp;
	for (i = 0; i < n; i++)
	{
		for (j = i; j < n; j++)
		{
			if (p[i] > p[j])
			{
				temp = p[i];
				p[i] = p[j];
				p[j] = temp;
			}
		}
	}
}
BTreeNode *split_recur(BTreeNode *x)
{
	int j;
	BTreeNode *np11,*np33=init();
	np11 = init();
	np11->leaf = false;
	np33->leaf = x->leaf; 
	int half = buf_sz/2;
	int mid = x->data[half];
	for (j = half; j <= buf_sz; j++)
	{
		np33->data[j - half] = x->data[j];
		np33->n++;
		x->data[j] = 0;
		x->n--;
	}//remaining added to another block
	np11->data[0] = mid;
	np11->child_ptr[np11->n] = x;
	np11->child_ptr[np11->n + 1] = np33;
	np11->n++; //only one element i.e root
	if(x->leaf == true)
	{
		np33->child_ptr[0] = x->child_ptr[0];
		x->child_ptr[0] = np33;
	}
	if(x->root == true)
	{
		x->root = false;
		np11->root = true;
	}
	int k;
/*	for(k=0;k<np33->n;k++)
	{
		cout << "np33->data[k]" <<  np33->data[k] << endl;
	}
	for(k=0;k<np11->n;k++)
	{
		cout << "np11->data[k]" <<  np11->data[k] << endl;
	}
	for(k=0;k<x->n;k++)
	{
		cout << "x->data[k]" <<  x->data[k] << endl;
	}*/
	return np11;
}
BTreeNode *split_recur_type2(BTreeNode *x)
{
	int j,k;
	BTreeNode *np111,*np333=init();
	np111 = init();
	np111->leaf = false;
	np333->leaf = x->leaf; 
	int half = buf_sz/2;
	int mid = x->data[half];
	for (j = half+1; j <= buf_sz; j++)
	{
		np333->data[j - half-1] = x->data[j];
		np333->child_ptr[j - half-1] = x->child_ptr[j];
		np333->n++;
		x->data[j] = 0;
		x->n--;
	}//remaining added to another block
	np333->child_ptr[np333->n] = x->child_ptr[x->n+1];
	x->data[x->n]=0;
	x->n--;
	np111->data[0] = mid;
	np111->child_ptr[np111->n] = x;
	np111->child_ptr[np111->n + 1] = np333;
	np111->n++; //only one element i.e root
	if(x->leaf == true)
	{
		np333->child_ptr[0] = x->child_ptr[0];
		x->child_ptr[0] = np333;
	}
/*	for(k=0;k<np333->n;k++)
	{
		cout << "np333->data[k]" <<  np333->data[k] << endl;
	}
	for(k=0;k<np111->n;k++)
	{
		cout << "np111->data[k]" <<  np111->data[k] << endl;
	}
	for(k=0;k<x->n;k++)
	{
		cout << "x->data[k]" <<  x->data[k] << endl;
	}*/
	return np111;
}
Tuple *insert_recur(BTreeNode *f,int a)
{
	int i,k;
	if(f->leaf == true)
	{
/*		for(k=0;k<f->n;k++)
		{
			cout << "f->data[k]" <<  f->data[k] << endl;
		}*/
//		cout << "leaf" << f->leaf << endl;
		Tuple *ret = tuple_init();
		f->data[f->n] = a;
		f->n++;
		sort(f->data, f->n); //sorting inside a block
		int k;
//		cout << "buf_sz" << f->n << endl;
//		cout << "buf_sz" << buf_sz << endl;
		if(f->n >= buf_sz+1)
		{
			ret->b = split_recur(f);
			ret->a = 1;
			if(ret->b->root==true)
				root = ret->b;
			return ret;
		}
		ret->a = 0;
	/*	for(k=0;k<f->n;k++)
		{
			cout << "f->data[k]" <<  f->data[k] << endl;
		}*/
		return ret;
	}
	else
	{
		Tuple *rett;
		if(a >= f->data[f->n-1])
		{	
	//		cout << "no" << f->n << endl;
			i=f->n;
		}
		else if(a < f->data[0])
		{
	//		cout << "ues" << endl;
			i = 0;
		}
		else
		{
			for(i = 0; i <= (f->n-1); i++)
			{
				if ((a >= f->data[i]) && (a < f->data[i + 1]))
				{
					i++;
					break;
				}
			}
	//		cout << "other " << endl;
		}
		BTreeNode *g = f->child_ptr[i];
		Tuple *ret2 = tuple_init();
		rett = insert_recur(g,a);
	/*	for(k=0;k<g->n;k++)
		{
			cout << "g->data[k]" <<  g->data[k] << endl;
		}*/
		if( rett->a == 1)
		{
			f->data[f->n] = rett->b->data[0];
			f->n++;
			sort(f->data, f->n); 
			int l;
			for(l = f->n-1;l >=0 ;l--)
			{
				if(rett->b->data[0] == f->data[l])
					break;
			}
		//	cout << "l" << l << endl;
			int k;
			if(l != f->n-1)
			{
				for(k=f->n;k>l;k--)
				{
					f->child_ptr[k] = f->child_ptr[k-1];
				}

			}
			f->child_ptr[l] = rett->b->child_ptr[0];
			f->child_ptr[l+1] = rett->b->child_ptr[1];
		/*	for(k=0;k<f->child_ptr[1]->n;k++)
			{
				cout << "hey " << f->child_ptr[1]->data[k] << endl;
			}*/
	/*		for(k=0;k<f->n;k++)
			{
				cout << "f->data[k]" <<  f->data[k] << endl;
			}*/
			if(f->n <= buf_sz)
			{
				ret2->a = 0;
				return ret2;
			}
			else if(f->n > buf_sz)
			{
				ret2->b = split_recur_type2(f);
				ret2->a = 1;
				return ret2;
			}
		}
		else
		{	
			ret2->a = 0;
			return ret2;
		}
	}
}
void insert(int a)
{
	int i, temp;
	x = root;
	if (x == NULL)
	{
		root = init();
		x = root;
		x->data[x->n] = a;
		x->root = true;
		sort(x->data, x->n); //sorting inside a block
		x->n++;
	}
	else
	{
		Tuple *finall = insert_recur(root,a);
		if(finall->a == 1)
			root = finall->b;
	}
}
Tuple *find(int inx)
{
	int flag = 0,i;
	BTreeNode *find1 = init();
	find1 = root;
	Tuple *ret_tup = tuple_init();
	if(find1==NULL)
	{
		BTreeNode *temp_null;
		temp_null = init();
		ret_tup->b = temp_null;
		ret_tup->a = 0;
		return ret_tup;
	}
	else if(find1->leaf == true)
	{
		for(i = 0;i < find1->n;i++)
		{
			if(find1->data[i] == inx)
			{
				flag = 1;
				break;
			}
		}
	}
	else
	{
		int breakwhile = 0;
		while(find1->leaf==false)
		{
			if(inx > find1->data[find1->n-1])
			{	
				i=find1->n;
			}
			else if (inx < find1->data[0])
				i = 0;
			else
			{
				for(i = 0; i < (find1->n-1); i++)
				{
					if(inx == find1->data[i])
					{
						flag = 1;
						break;
					}
					else if ((inx > find1->data[i]) && (inx < find1->data[i + 1]))
					{
						i++;
						break;
					}
				}
			}
			if(inx == find1->data[find1->n-1] && flag == 0)
			{	
				i=find1->n-1;
				flag = 1;
			}
//			cout << "print" << i << endl;
			if(flag == 1 && find1->leaf==true)
				break;
			else
				find1 = find1->child_ptr[i];
		}
		if(flag == 0)
		{
			for(i=0;i< find1->n;i++)
			{
				if(find1->data[i]==inx)
				{
					flag = 1;
					break;
				}
			}
			if(flag == 0 )
			{
				if(find1->child_ptr[0]!=NULL)
				{
					BTreeNode *test = find1->child_ptr[0];int z;
					if(test->child_ptr[0]!=NULL)	{
						while(test->child_ptr[0]!=NULL)
						{
							for(z=0;z<test->n;z++)
								if(test->data[z] == inx)
								{	flag = 1;
									break;
								}
							if(flag == 1)
								break;
							test = test->child_ptr[0];
						}
					}
					if(flag == 0)
					{	
						if(test!=NULL)
						{
							for(i=0;i< test->n;i++)
							{
								if(test->data[i]==inx)
								{	flag = 1;
									break;
								}
							}
						}
					}

				}
			}
		}
	}
	ret_tup->b = find1;
	if(flag == 1)
		ret_tup->a = 1;
	else 
		ret_tup->a = 0;
	return ret_tup;

}
char *stringToChar(string line){

	int n=line.length();
	char *y = new char[n + 1];
	for(int i=0; i<=n; i++)
		y[i]='\0';

	while(n>0){	
		y[n-1]=line[n-1];
		n--;
	}
	return y;
}
int getQueryType(string s){
	if(s=="INSERT"){
		return 1;
	}else if(s=="FIND"){
		return 2;
	}else if(s=="COUNT"){
		return 3;
	}else if(s=="RANGE"){
		return 4;
	}
}
int main(int argc, char const *argv[])
{

	char insertstr[10] = "INSERT",findstr[10] = "FIND",countstr[10] = "COUNT",rangestr[10]="RANGE";
	char *tokens, *y;
	int qt,n,t,l,i,j,k,M,B,degree,curr_inp_buff=0,curr_inp_sz=0,curr_out_sz=0,inx,iny;
	string line, fline,outp;
	M = atoi(argv[2]);
	B = atoi(argv[3]);
	buf_sz = (B-8)/12;
	vector<vector<string> > input_buffer(M-1);
	vector<string> output_buffer; // assuming output will have only striings
	if(buf_sz<=0){
		cout<<"Given block size is too small\n";
		return 0;
	}
	ifstream infile(argv[1]);
	while(getline(infile, fline))
	{
		if(curr_inp_buff<(M-1) && (curr_inp_sz+fline.length())<=B)
		{
			input_buffer[curr_inp_buff].push_back(fline);
			curr_inp_sz += fline.length();
		}
		else if((curr_inp_buff<(M-2) && (curr_inp_sz + fline.length())>B))
		{
			curr_inp_buff++;
			input_buffer[curr_inp_buff].push_back(fline);
			curr_inp_sz = fline.length();
		}
		else{
			for(i=0; i<=curr_inp_buff;i++)
			{
				for(j=0; j<= input_buffer[i].size()-1;j++)
				{
					outp="";
					line = input_buffer[i][j];
					y = stringToChar(line); 
					tokens = strtok (y," ");
					qt = getQueryType(tokens);
					tokens = strtok(NULL, " ");
					inx = atoi(tokens);
					if(qt==1){
						//						cout << "inserting " << inx << endl;
						insert(inx);
					}
					else if(qt==2){
						if(find(inx)->a == 0)
							outp += "NO";
						else{
							outp += "YES";
						}
					}
					else if(qt==3){
						int count_num = 0;
						count_num = count(inx);
						//						cout << "countof" << inx << endl; 
						outp = to_string(count_num);
						//						cout << "count" << count_num << endl; 
					}
					else if(qt==4)
					{
						inx = atoi(tokens);
						tokens = strtok(NULL, " ");
						iny = atoi(tokens);
						int range_count = 0;
						if (inx > iny)
							range_count = 0;
						else if(inx == iny)
							range_count = count(inx);
						else
						{
							//cout << "inx1" << inx << endl;
							//cout << "inx2" << iny << endl;
							Tuple *find_inx = find(inx);
							Tuple *find_iny = find(iny);
							BTreeNode *temp_range1 = find_inx->b;
							BTreeNode *temp_range2 = find_iny->b;
							/*			for(k=0;k<find_inx->b->n;k++)
										{
										cout << "find_inx->data[k]" <<  find_inx->b->data[k] << endl;
										}
										for(k=0;k<find_iny->b->n;k++)
										{
										cout << "find_iny->data[k]" <<  find_iny->b->data[k] << endl;
										}*/
							int ttt,flagtotalbreak = 0;
							BTreeNode *temp_ran = temp_range1;
							for(ttt = 0;ttt < temp_range1->n;ttt++)
							{
								//							cout << "temp_ran->data[k]" <<  temp_range1->data[ttt] << endl;
								if((temp_range1->data[ttt] >= inx) && (temp_range1->data[ttt] <=iny))
									range_count++;
								if(temp_range1->data[ttt]>iny)
								{
									flagtotalbreak = 1;
									break;
								}
							}
							if(flagtotalbreak!=1)
							{
								if(equal(temp_range1,temp_range2)==0)
								{
									temp_ran = temp_ran->child_ptr[0];
									while(equal(temp_ran,temp_range2)==0 && temp_ran!=NULL)
									{
										//cout << "happening" << endl;
										range_count = range_count + temp_ran->n;
										temp_ran = temp_ran->child_ptr[0];
									}
								}

								//					cout << "done" << endl;
								int flagbreak = 0;
								temp_ran = temp_range2;
								while(temp_ran!=NULL)
								{
									for(ttt=0;ttt<temp_ran->n;ttt++)
									{
										//							cout << "temp_ran->data[k]" <<  temp_ran->data[ttt] << endl;
										if((temp_ran->data[ttt] <= iny) && (temp_ran->data[ttt] >= inx))
											range_count++;
										else if(temp_ran->data[ttt] > iny)
										{
											flagbreak = 1;
											break;
										}	
									}
									if(flagbreak == 1)
										break;
									temp_ran = temp_ran->child_ptr[0];
								}
							}}	
						outp = to_string(range_count);
					}

					/*if there is some output*/
					if(outp.length()>0){

						/*output buffer has space*/
						if((curr_out_sz + outp.length())<=B){
							curr_out_sz += outp.length();
							output_buffer.push_back(outp);
						}
						else{
							int k;
							for(k=0; k< output_buffer.size();k++)
							{
								cout<< output_buffer[k] << endl;
							}
							output_buffer.clear();
							output_buffer.push_back(outp);
							curr_out_sz = outp.length();
						}
					}
				}
			}
			for(i=0; i<= curr_inp_buff;i++)
				input_buffer[i].clear();
			input_buffer[0].push_back(fline);
			curr_inp_buff=0;
			curr_inp_sz = fline.length();
		}
	}
	for(i=0; i<=curr_inp_buff;i++)
	{
		for(j=0; j<= input_buffer[i].size()-1;j++)
		{
			outp="";
			line = input_buffer[i][j];
			y = stringToChar(line); 
			tokens = strtok (y," ");
			qt = getQueryType(tokens);
			tokens = strtok(NULL, " ");
			inx = atoi(tokens);
			if(qt==1){
				insert(inx);
			}
			else if(qt==2){
				if(find(inx)->a == 0)
					outp += "NO";
				else{
					outp += "YES";
				}
			}
			else if(qt==3){
				int count_num = 0;
				count_num = count(inx);
				//			cout << "countof" << inx << endl; 
				outp += std::to_string(count_num);
				//			cout << "count" << count_num << endl; 
			}
			else if(qt==4)
			{
				inx = atoi(tokens);
				tokens = strtok(NULL, " ");
				iny = atoi(tokens);
				int range_count = 0;
				if (inx > iny)
					range_count = 0;
				else if(inx == iny)
					range_count = count(inx);
				else
				{
					Tuple *find_inx = find(inx);
					Tuple *find_iny = find(iny);
					/*			for(k=0;k<find_inx->b->n;k++)
								{
								cout << "find_inx->data[k]" <<  find_inx->b->data[k] << endl;
								}
								for(k=0;k<find_iny->b->n;k++)
								{
								cout << "find_iny->data[k]" <<  find_iny->b->data[k] << endl;
								}*/
					BTreeNode *temp_range1 = find_inx->b;
					BTreeNode *temp_range2 = find_iny->b;
					int ttt,flagtotalbreak = 0;
					BTreeNode *temp_ran = temp_range1;
					for(ttt = 0;ttt < temp_range1->n;ttt++)
					{
						if(temp_range1->data[ttt] >= inx)
							range_count++;
						if(temp_range1->data[ttt]>iny)
						{
							flagtotalbreak = 1;
							break;
						}
					}

					if(flagtotalbreak!=1)
					{
						if(equal(temp_range1,temp_range2)==0)
						{
							temp_ran = temp_ran->child_ptr[0];
							while(equal(temp_ran,temp_range2)==0)
							{
								range_count = range_count + temp_ran->n;
								temp_ran = temp_ran->child_ptr[0];
							}
						}
						int flagbreak = 0;
						temp_ran = temp_range2;
						while(temp_ran!=NULL)
						{
							for(ttt=0;ttt<temp_ran->n;ttt++)
							{
//								cout << "temp_ran->data[k]" <<  temp_ran->data[ttt] << endl;
								if(temp_ran->data[ttt] <= iny)
									range_count++;
								else if(temp_ran->data[ttt] > iny)
								{
									flagbreak = 1;
									break;
								}	
							}
							if(flagbreak == 1)
								break;
							temp_ran = temp_ran->child_ptr[0];
						}
					}
				}
				outp += to_string(range_count);
			}
			if(outp.length()>0){

				/*output buffer has space*/
				if((curr_out_sz + outp.length())<=B){
					curr_out_sz += outp.length();
					output_buffer.push_back(outp);
				}
				else{

					int k;
					for(k=0; k< output_buffer.size();k++)
					{
						cout<< output_buffer[k] << endl;
					}
					output_buffer.clear();
					output_buffer.push_back(outp);
					curr_out_sz = outp.length();
				}
			}
		}
	}
	if(output_buffer.size()-1>=0)
	{
		int k;
		for(k=0; k<= output_buffer.size()-1;k++)
		{
			cout<< output_buffer[k] << endl;
		}
	}
	return 0;
}

