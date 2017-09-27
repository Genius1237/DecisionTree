#include<bits/stdc++.h>
#include "decision_tree.h"

using namespace std;

vector <vector<string> > input_data= readData();
set <string> tar_val_set = readTargetVal();
const int num_attr = 15;
string missing_attr = "?";
//string continuous_val[]= {};


void change_missing_val(string target_val){

	map<string,int> attr_count[num_attr];

	for(vector <vector<string> > :: iterator it =input_data.begin(); it!= input_data.end();it++){

		vector<string> instance=*it;
		for(int i=0;i < instance.size()-1; i++){
			if(instance[i]!=missing_attr){
				if(instance[instance.size()-1] == target_val){
					attr_count[i][instance[i]]++;
				}
			}
		}
	}

	for(vector<vector <string> > :: iterator it =input_data.begin(); it!= input_data.end();it++){

		vector<string> instance =*it;
		for(int i=0;i< instance.size()-1;i++){
			if(instance[i]==missing_attr){
				if(instance[instance.size()-1]== target_val){
					int m=-1;
					for(map<string,int>:: iterator itr= attr_count[i].begin(); itr!=attr_count[i].end(); itr++){
						if(m <= itr->second){
							 instance[i]= itr->first;
							 m=itr->second;
							}
						}
					}
				}
		}
		*it= instance;
	}
}


int main(){

	for(set<string>:: iterator it = tar_val_set.begin();it != tar_val_set.end(); it++)
			change_missing_val(*it);

	int idx= 1;
	for(vector<vector<string> > :: iterator it = input_data.begin(); it!= input_data.end() ; it++){

			vector<string> temp = *it;
			for(int i=0 ; i< temp.size(); i++){
					if(temp[i]== missing_attr)		cout<<idx<<" "<<i<<endl;
			}
			idx++;
	}
	return 0;
}
