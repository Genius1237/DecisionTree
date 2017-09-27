#include<bits/stdc++.h>
#include "decision_tree.h"

using namespace std;

vector <vector<string> > input_data= readData();
const int num_attr = 14;
//string continuous_val[]= {};

void calculate_entropy(){


}


void change_missing_val(string target_val){

	map<string,int> attr_count[num_attr];
	
	for(vector <vector<string> > :: iterator it =input_data.begin(); it!= input_data.end();it++){
		
		vector<string> instance=*it;
		for(int i=0;i < instance.size()-2; i++){
			if(instance[i]!="?"){
				if(instance[instance.size()-2] == target_val){
					attr_count[i][instance[i]]++;
				}
			}
		}
	}

	for(vector<vector <string> > :: iterator it =input_data.begin(); it!= input_data.end();it++){

		vector<string> instance =*it;
		for(int i=0;i< instance.size()-1;i++){
			if(instance[i]=="?"){
				if(instance[instance.size()-2]== target_val){
					
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

void group_continuous_val(ll idx){

	set<pair<int , string> > con_val;
	for(vector<vector< string> >:: iterator it =input_data.begin(); it!= input_data.end(); it++){

		vector<string> instance =*it;
		con_val.insert(make_pair(instance[idx],instance[instance.size()-2]));
	}

	for(int i=1;i <con_val.size();i++){

		pair<int ,string> temp_l =con_val[i-1];
		pair<int, string> temp_r = con_val[i];
		if(temp_val.second != temp_val.first)
		//	calculate_entropy();
	}
}

int main(){

	change_missing_val("<=50K");
	change_missing_val(">50K");

	group_continuous_val();
	return 0;
}