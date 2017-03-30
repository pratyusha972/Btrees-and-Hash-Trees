#include<stdio.h>
#include<algorithm>
#include<iostream>
#include<fstream>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<vector>
using namespace std;
int n=2,rl=0,sp=0,buf_sz, B;
vector<vector<int> > hashtable;
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
int find(int inx)
{
	int i,add,flag = 0;
	int power1 =  n*pow(2,rl);
	int power2 =  n*pow(2,rl+1);
	int hash1 = inx % power1;
	int hash2 = inx % power2;
	if(hash1 < 0)
		hash1 = hash1*(-1);
	if(hash2 < 0)
		hash2 = hash2*(-1);
	if(hash1 < sp) 
		add = hash2;
	else if(hash1 >= sp)
		add = hash1;
	for(i = 0;i<hashtable[add].size();i++)
	{
		if(hashtable[add][i] == inx)
		{
			flag = 1;
			break;
		}

	}
	if(flag == 1)
		return 1;
	else
		return 0;

}
void insert(int inx)
{
	int i,j,add;
	int hashi,temp;
	int power1 =  n*pow(2,rl);
	int power2 =  n*pow(2,rl+1);
	int hash1 = inx % power1;
	int hash2 = inx % power2;
	if(hash1 < 0) //
		hash1 = hash1*(-1);
	if(hash2 < 0)
		hash2 = hash2*(-1);
	//hash function changes according to the condition whether hash appears before sp or after sp
	if(hash1 < sp) 
		add = hash2;
	else if(hash1 >= sp)
		add = hash1;
	int sum = 0;
	for(i=0;i<=hashtable.size()-1;i++)
		sum = sum + hashtable.size()-1;
	sum = sum+1;
	if((hashtable[add].size() > buf_sz) || (float(sum) >= 0.75*float(buf_sz)*float(hashtable.size()))) //no of elements in the block are >= default then overflow buffer
	{
		hashtable[add].push_back(inx);
		vector<int>a(buf_sz,0);
		hashtable.push_back(a);
		for(j=0;j<buf_sz;j++)
			hashtable[hashtable.size()-1].push_back(0);
		for(i=hashtable[sp].size()-1;i>=0;i--)
		{
			temp = hashtable[sp][i];
			int power3 = n*pow(2,rl+1);
			hashi = hashtable[sp][i]%power3; //arranging elements in the sp row into various rows
			if(hashi < 0)
				hashi = hashi*(-1);
			if(hashi != sp)  //if so then remove from sp and rearrange into corresponding row
			{
				hashtable[hashi].push_back(temp);
				hashtable[sp].erase(std::remove(hashtable[sp].begin(), hashtable[sp].end(), temp), hashtable[sp].end());
			}
		}
		sp = sp+1;
		if(sp == (pow(2,rl))*n)
		{	
			sp = 0;
			rl = rl+1;
		}
	}
	else
		hashtable[add].push_back(inx);
	return;
}
int main(int argc,char const *argv[])
{
	char *tokens, *y;
	int qt,t,l,i,j,k,M,B,degree,curr_inp_buff=0,curr_inp_sz=0,curr_out_sz=0,inx,iny;
	string line, fline,outp,filename;
	M = atoi(argv[2]);
	B = atoi(argv[3]);
	buf_sz = (B-8)/12;
	for(i=0;i<n;i++)
	{
		vector<int>a(buf_sz,0);	
		hashtable.push_back(a);
		for(j=0;j<buf_sz;j++)
			hashtable[i].push_back(0);
	}
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
					inx = atoi(y);
//					cout << "inx" << inx << endl;
					int ret=0;
					ret = find(inx);
					if(ret == 0)
					{	
						insert(inx);
						outp += y;
						//						cout << "yess " << outp << endl;

						if((curr_out_sz + outp.length())<=B)
						{
							curr_out_sz += outp.length();
							output_buffer.push_back(outp);
						}
						else
						{
							int k;
							if(output_buffer.size()-1>0)
							{
								for(k=0; k<= output_buffer.size()-1;k++)
								{
									cout<< output_buffer[k]<< endl; 
								}

								output_buffer.clear();
								output_buffer.push_back(outp);
								curr_out_sz = outp.length();
							}
						}
					}

				}
			}
			for(i=0; i<= curr_inp_buff;i++)
				input_buffer[i].clear();
			//vector<vector<string> > input_buffer(M-1);
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
			inx = atoi(y);
//			cout << "y" << inx << endl;
			int ret=0;
			ret = find(inx);
			if(ret==0)	
			{

//	cout << "found" << inx << endl;
				insert(inx);
//				cout << "inserted" << endl;
				outp += y;
				if((curr_out_sz + outp.length())<=B)
				{
					curr_out_sz += outp.length();
					output_buffer.push_back(outp);
				}
				else{
					int k;
	if(output_buffer.size()-1>0)
	{
						for(k=0; k<= output_buffer.size()-1;k++)
						{
							cout<< output_buffer[k] << endl;
						}
	}
					output_buffer.clear();
					output_buffer.push_back(outp);
					curr_out_sz = outp.length();
				}
			}
		}
	}
	if(output_buffer.size()-1>0)
	{
		int k;
		for(k=0; k<= output_buffer.size()-1;k++)
		{
			cout<< output_buffer[k] << endl;
		}
	}
	return 0;

}
