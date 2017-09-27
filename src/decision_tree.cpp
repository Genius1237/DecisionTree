#include "decision_tree.h"
#include <fstream>	
#include <sstream>
#include <algorithm>
#include <cmath>

std::string missing_attr= "?";
int num_target_val=2;


// --------------------- DecisionTreeNode Class ---------------------------

DecisionTreeNode::DecisionTreeNode() {
	;
}

void DecisionTreeNode::setAttrName(const std::string& attr_name) {
	this -> attr_name = attr_name;
}

std::string DecisionTreeNode::getAttrName() {
	return attr_name;
}

void DecisionTreeNode::setType(const std::string& type) {
	this -> type = type;
}

// --------------------- discAttrDecisionTreeNode Class ---------------------------
DiscAttrDecisionTreeNode::DiscAttrDecisionTreeNode() {
	type = "discrete";
}

DecisionTreeNode*& DiscAttrDecisionTreeNode::operator[](const std::string& attr_val) {
	return child[attr_val];
}


// --------------------- contAttrDecisionTreeNode Class ---------------------------
ContAttrDecisionTreeNode::ContAttrDecisionTreeNode() {
	type = "continous";
}

DecisionTreeNode*& ContAttrDecisionTreeNode::operator[](const double &attr_val) {

	auto it = std::upper_bound(dividers.begin(), dividers.end(), attr_val);
	if (it == dividers.end()) {
		return child[child.size() - 1];
	} else {
		return child[it - dividers.begin()];
	}
}

void ContAttrDecisionTreeNode::setDividers(const std::vector<double> dividers) {
	this -> dividers = dividers;
}

// --------------------- Instance Class ---------------------------

Instance::Instance() {
	;
}

Instance::Instance(
	const std::vector<std::string>& attr_names, const std::vector<std::string>& attr_vals) {
	for (int i = 0; i < attr_names.size(); i++) {
		els[attr_names[i]] = attr_vals[i];
	}
}

std::string Instance::operator[](const std::string& attr_name) {
	return els[attr_name];
}

// --------------------- Example Class ---------------------------

Example::Example() {
	;
}

Example::Example(
	const std::vector<std::string>& attr_names, const std::vector<std::string>& attr_vals,
	const std::string& target_class) {
	for (int i = 0; i < attr_names.size(); i++) {
		els[attr_names[i]] = attr_vals[i];
	}
	this -> target_class = target_class;
}

std::string Example::getTargetClass() const {
	return target_class;
}

// --------------------- DecisionTree Class ---------------------------

void DecisionTree::addAttrInfo(
	const std::string& attr_name, const std::vector<std::string>& attr_vals) {
	pos_vals[attr_name] = attr_vals;
}

void DecisionTree::build(const std::vector<Example>& train_data) {
	std::vector<std::string> all_attr;
	for (auto it = pos_vals.begin(); it != pos_vals.end(); it++) {
		all_attr.push_back(it -> first);
	}
	build(train_data, root, all_attr);
}

void DecisionTree::build(std::vector<Example> train_data, 
	DecisionTreeNode*& p, std::vector<std::string> check_attr) {

	// check if there is any training data. if not assign a target class randomly
	if (train_data.size() == 0) {
		p = new DecisionTreeNode;
		p -> setAttrName("random");
		p -> setType("leaf");
	}
	return;

	// check if all examples have same target class
	bool leaf = true;
	std::string target_class = train_data[0].getTargetClass();
	for (int i = 1; i < train_data.size(); i++) {
		if (train_data[i].getTargetClass() != target_class) {
			leaf = false;		
			break;
		}
	}

	if (leaf) {
		p = new DecisionTreeNode;
		p -> setAttrName(target_class);
		p -> setType("leaf");
	} else {
		double max_gain = -1;
		int max_index = 0;
		std::vector<double> dividers;
		bool is_cont;
		
		// find which attribute should be at the node
    for (int i = 0; i < check_attr.size(); i++) {
			if (pos_vals[check_attr[i]].size() == 0) {
				std::pair<double, std::vector<double>>temp = contInfoGain(
					train_data, check_attr[i]);
				double cand_gain = temp.first;
				if (cand_gain > max_gain) {
					max_gain = cand_gain;
					max_index = i;
					is_cont = true;
					dividers = temp.second;
				}
			} else {
				double cand_gain = infoGain(train_data, check_attr[i]);
				if (cand_gain > max_gain) {
					max_gain = cand_gain;
					max_index = i;
					is_cont = false;
				}
			}
		}
    std::string attr_name = check_attr[max_index];

    if (is_cont) {
      p = new ContAttrDecisionTreeNode;
      p -> setType("continous");  
      p -> setAttrName(attr_name);
      check_attr.erase(check_attr.begin() + max_index);
      
      ContAttrDecisionTreeNode *pp = static_cast<ContAttrDecisionTreeNode*>(p);
      pp -> setDividers(dividers);
      (*pp)[0] = NULL;
      for (int i = 1; i <= dividers.size(); i++) {
        (*pp)[i] = NULL;
      }
      for (int i = 0; i < train_data.size(); i++) {
        ;
      }

    } else {
      p = new DiscAttrDecisionTreeNode;
      p -> setType("discrete");
      p -> setAttrName(attr_name);
      check_attr.erase(check_attr.begin() + max_index);

      DiscAttrDecisionTreeNode *pp = static_cast<DiscAttrDecisionTreeNode*>(p);
      for (int i = 0; i < pos_vals[attr_name].size(); i++) {
        (*pp)[check_attr[i]] = NULL;
        std::vector<Example> sub_train_data;
        for (int j = 0; j < train_data.size(); j++) {
          if (train_data[j][attr_name] == pos_vals[attr_name][i]) {
            sub_train_data.push_back(train_data[j]);
          }
        }
				build(sub_train_data, (*pp)[check_attr[i]], check_attr);
			}
		}
	}
}

double DecisionTree::infoGain(std::vector<Example> els, const std::string& attr_name){

		std::set<std::string> target_val_set = readTargetVal();
		std::map< std::string,int > num_attr[num_target_val];

		std::vector< std::string> temp_tar_val;
		for(auto it =target_val_set.begin(); it!= target_val_set.end(); it++){
					temp_tar_val.push_back(*it);
		}

		for(int i=0; i<els.size(); i++){
				Example temp =els[i];

				if(temp[attr_name] != missing_attr){
						for(auto it =temp_tar_val.begin(); it!=temp_tar_val.end(); it++){

								if(*it == temp.getTargetClass()){
										int pos= it-temp_tar_val.begin();
										num_attr[pos][temp[attr_name]]++;
										break;
								}
						}
				}
		}

		for( int i=0; i< els.size(); i++){
				Example temp = els[i];

				int pos;
				if(temp[attr_name] == missing_attr){
					for( auto it =temp_tar_val.begin(); it!= temp_tar_val.end(); it++){

							if(*it == temp.getTargetClass()){
									pos= it - temp_tar_val.begin();
									break;
							}
				}
		}
		int m = -1;
		for(auto it= num_attr[pos].begin(); it!=num_attr[pos].end(); it++){
				if(m <= it->second){
								m = it->second;
								temp[attr_name]= it->first;
					}
			}
			els[i]=temp;
	 }

	 double sum = els.size();

	 std::map< std::string, int > calc_entropy_map;
	 for(int i=0; i< els.size(); i++)
		 		calc_entropy_map[els[i].getTargetClass()]++;

	 double entropy_1 = calcEntropy(calc_entropy_map);

	 double entropy_2 = 0.0;
	 for(int i=0;i<pos_vals[attr_name].size();i++){

			double numerator = 0;
			calc_entropy_map.clear();

			std::string attr = pos_vals[attr_name][i];
			for(std::set<std::string> :: iterator it = target_val_set.begin(); it!= target_val_set.end(); it++){

					for(int j=0; j<els.size(); j++){

							if(attr == els[j][attr_name]){
									calc_entropy_map[*it]++;
									numerator++;
							}
					}
			}

			double temp_entropy = calcEntropy(calc_entropy_map);
			entropy_2 += (numerator/sum)* temp_entropy;
	 }
	 return (entropy_1-entropy_2);
}

double DecisionTree::calcEntropy(const std::map< std::string, int>& els){
		double sum=0.0,ans=0.0;
		std::vector<double> temp;
		for( auto it = els.begin(); it!=els.end(); it++){
				sum+= it->second;
				temp.push_back((double)it->second);
		}
		for(int i=0 ; i<temp.size(); i++)
				ans+= (temp[i]/sum)*(log2(temp[i]/sum));
		ans*=-1.00;
		return ans;
}

// -------------------- Data Extraction --------------------------------------

std::vector<std::vector<std::string> > readData(){
	int n=15;
	std::string fileloc="../data/adult_data";
	//std::string fileloc="../data/zoo.data";
	std::ifstream fin(fileloc,std::ios::in);
	std::vector<std::vector<std::string> > data;
	while(!fin.eof()){
		std::string s;
		std::vector<std::string> att;
		fin>>s;
		std::stringstream str(s);
		//std::cout<<s<<std::endl;
		for(int i=0;i<n;i++){
			std::string temp;
			std::getline(str,temp,',');
			//std::cout<<temp<<"\n";
			att.push_back(temp);
		}
		//std::cout<<att.size()<<"\n";
		data.push_back(att);
	}
	return data;
}

std::set<std::string> readTargetVal(){
	int n=15;
	std::string fileloc="../data/adult_data";
	std::ifstream fin(fileloc,std::ios::in);
	std::set<std::string> att;
	while(!fin.eof()){
		std::string s;
		fin>>s;
		std::stringstream str(s);
		for(int i=0;i<n;i++){
			std::string temp;
			std::getline(str,temp,',');
			if(i==n-1)
				att.insert(temp);
		}
	}
	return att;
}