#include "decision_tree.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iostream>

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

std::string DecisionTreeNode::getType() {
	return type;
}

// --------------------- discAttrDecisionTreeNode Class ---------------------------
DiscAttrDecisionTreeNode::DiscAttrDecisionTreeNode() {
	type = "discrete";
}

DecisionTreeNode*& DiscAttrDecisionTreeNode::operator[](const std::string& attr_val) {
	return child[attr_val];
}

std::vector<DecisionTreeNode*> DiscAttrDecisionTreeNode::getChildPointers() {
	std::vector<DecisionTreeNode*> ret;
	for (auto it = child.begin(); it != child.end(); it++) {
		ret.push_back(it -> second);
	}
	return ret;
}

// --------------------- contAttrDecisionTreeNode Class ---------------------------
ContAttrDecisionTreeNode::ContAttrDecisionTreeNode() {
	type = "continuous";
}

int ContAttrDecisionTreeNode::getIndex(double attr_val) {
	auto it = std::upper_bound(dividers.begin(), dividers.end(), attr_val);
	if (it == dividers.end()) {
		return child.size() - 1;
	} else {
		return it - dividers.begin();
	}
}

DecisionTreeNode*& ContAttrDecisionTreeNode::operator[](const double &attr_val) {
	return child[getIndex(attr_val)];
}

void ContAttrDecisionTreeNode::setDividers(const std::vector<double> dividers) {
	this -> dividers = dividers;
	sort((this -> dividers).begin(), (this -> dividers).end());
	child.resize((this -> dividers).size() + 1);
}

std::vector<DecisionTreeNode*> ContAttrDecisionTreeNode::getChildPointers() {
	return child;
}

DecisionTreeNode*& ContAttrDecisionTreeNode::getChildPointer(int index) {
	return child[index];
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

std::string& Instance::operator[](const std::string& attr_name) {
	return els[attr_name];
}

std::ostream& operator<<(std::ostream& out, const Instance& inst) {
	for (auto it = inst.els.begin(); it != inst.els.end(); it++) {
		out << it -> first;
		out << ": ";
		out << it -> second;
		out << ", ";
	}
	return out;
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

void DecisionTree::addTargetValues(std::set<std::string> target_values) {
	this -> target_values = target_values;
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
	if (check_attr.size() == 0) {
		std::cout << "check_attr.size() == 0" << std::endl;
		return;
	}
	//std::cout << "build 1" << std::endl;
	if (train_data.size() == 0) {
		p = new DecisionTreeNode;
		p -> setAttrName("random");
		p -> setType("leaf");
		std::cout << "train_data.size() == 0" << std::endl;
		return;
	}

	// check if all examples have same target class
	bool leaf = true;
	std::string target_class = train_data[0].getTargetClass();
	for (int i = 1; i < train_data.size(); i++) {
		//std::cout << train_data[i].getTargetClass() << "\n";
		if (train_data[i].getTargetClass() != target_class) {
			leaf = false;
			break;
		}
	}

	//std::cout << "build 2" << std::endl;

	if (leaf) {
		p = new DecisionTreeNode;
		p -> setAttrName(target_class);
		p -> setType("leaf");
		//std::cout << "build 3a" << std::endl;
	} else {
		double max_gain = -1;
		int max_index = 0;
		std::vector<double> dividers;
		bool is_cont;

		//std::cout << "build 3b1" << std::endl;
		// find which attribute should be at the node
    for (int i = 0; i < check_attr.size(); i++) {
			if (pos_vals[check_attr[i]].size() == 0) {
				// if continous attribute
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
				double cand_gain = discInfoGain(train_data, check_attr[i]);
				if (cand_gain > max_gain) {
					max_gain = cand_gain;
					max_index = i;
					is_cont = false;
				}
			}
		}
		//std::cout << "build 3b2" << std::endl;
		// now, the attribute to be placed has been found
    std::string attr_name = check_attr[max_index];
    check_attr.erase(check_attr.begin() + max_index);

    if (is_cont) {
      p = new ContAttrDecisionTreeNode;
      p -> setType("continuous");
      p -> setAttrName(attr_name);

      ContAttrDecisionTreeNode *pp = static_cast<ContAttrDecisionTreeNode*>(p);
      pp -> setDividers(dividers);

      std::vector<std::vector<Example>> bins;
      bins.resize(dividers.size() + 1);
      for (int i = 0; i < train_data.size(); i++) {
      	bins[pp -> getIndex(atof(train_data[i][attr_name].c_str()))].push_back(train_data[i]);
      }
  
			//std::cout << "build 3b1a" << std::endl;
      // iterating through each child
      for (int i = 0; i <= dividers.size(); i++) {
        build(bins[i], pp -> getChildPointer(i), check_attr);
      }

    } else {
      p = new DiscAttrDecisionTreeNode;
      p -> setType("discrete");
      p -> setAttrName(attr_name);

      DiscAttrDecisionTreeNode *pp = static_cast<DiscAttrDecisionTreeNode*>(p);
 
      std::map<std::string, std::vector<Example>> bins;
      for (int i = 0; i < train_data.size(); i++) {
      	bins[train_data[i][attr_name]].push_back(train_data[i]);
      }

			//std::cout << "build 3b1b" << std::endl;
      for (int i = 0; i < pos_vals[attr_name].size(); i++) {
				build(bins[pos_vals[attr_name][i]], (*pp)[pos_vals[attr_name][i]], check_attr);
			}
		}
	}
}

void DecisionTree::print() {
	print(root);
}

void DecisionTree::print(DecisionTreeNode *p) {
	std::cout << (p -> getAttrName());
	if (p -> getType() == "discrete") {
		DiscAttrDecisionTreeNode* pp = static_cast<DiscAttrDecisionTreeNode*>(p);
		std::vector<DecisionTreeNode*> child_pointers;
		child_pointers = pp -> getChildPointers();
		for (int i = 0; i < child_pointers.size(); i++) {
			print(child_pointers[i]);
		}
	} else if (p -> getType() == "continuous") {
		ContAttrDecisionTreeNode* pp = static_cast<ContAttrDecisionTreeNode*>(p);
		std::vector<DecisionTreeNode*> child_pointers;
		child_pointers = pp -> getChildPointers();
		for (int i = 0; i < child_pointers.size(); i++) {
			print(child_pointers[i]);
		}
	}
}

std::pair<double, std::vector<double> > DecisionTree::contInfoGain(std::vector<Example> els,const std::string& attr_name){

		//std::cout << "continfogain start: " << attr_name << "\n";
		std::set<std::pair<double,std::string> > cont_val_set;
		for(int i=0;i<els.size(); i++)
				cont_val_set.insert(make_pair(atof(els[i][attr_name].c_str()),els[i].getTargetClass()));

		std::vector<std::pair<double,std::string> > cont_val_list;
		for(auto it = cont_val_set.begin(); it!= cont_val_set.end(); it++){
				cont_val_list.push_back(*it);
		}

		double pos,info_gain=-1;
		double sum = els.size();

		std::map<std::string, int> calc_entropy_map;
		for(int i=0; i< els.size(); i++)
 		 		calc_entropy_map[els[i].getTargetClass()]++;

 	 double entropy_1 = calcEntropy(calc_entropy_map);

 	 std::map<std::string, int> left, right = calc_entropy_map;
 	 left[cont_val_list[0].second] += 1;
	 right[cont_val_list[0].second] -= 1;
	 double max_divider, max_info_gain = -1;
	 for(int i=1; i< cont_val_list.size(); i++){

				if(cont_val_list[i].second != cont_val_list[i-1].second) {
					double curr_info_gain = entropy_1 - 
					((left.size() * calcEntropy(left) + right.size() * calcEntropy(right)) / sum);
					if (curr_info_gain > max_info_gain) {
						max_info_gain = curr_info_gain;
						max_divider = (cont_val_list[i].first + cont_val_list[i - 1].first) / 2;
					}
				}
				right[cont_val_list[i].second] -= 1;
				left[cont_val_list[i].second] += 1;
		}
		std::vector<double> temp;
		temp.push_back(max_divider);
		//std::cout << "continfogain end\n";
		return make_pair(max_info_gain,temp);
}

double DecisionTree::discInfoGain(std::vector<Example> els, const std::string& attr_name){
		//std::cout << "discinfogain start: " << attr_name << "\n";

		//***************************
		//       target_val ,      attr_val, cnt
		std::map<std::string, std::map<std::string, int>> bins;
		for (int i = 0; i < els.size(); i++) {
			if (els[i][attr_name] != missing_attr) {
				bins[els[i].getTargetClass()][els[i][attr_name]]++;
			}
		}

		// filling of missing vals
		for (int i = 0; i < els.size(); i++) {
			if (els[i][attr_name] == missing_attr) {
				int max = -1;
				std::string attr_val;
				for (auto const &x: bins[els[i].getTargetClass()]) {
					if (x.second > max) {
						max = x.second;
						attr_val = x.first;
					}
				}
				els[i][attr_name] = attr_val;
			}
		}

		//        attr_val              target_val  occ
		std::map<std::string, std::map<std::string, int>> bins2;
		for (int i = 0; i < els.size(); i++) {
			bins2[els[i][attr_name]][els[i].getTargetClass()]++;
		}

		//        target_val  occ
		std::map<std::string, int> temp;
		for (int i = 0; i < els.size(); i++) {
			temp[els[i].getTargetClass()]++;
		}

		// info gain calculation
		int num_els = els.size();
		double ans1 = calcEntropy(temp);
		double ans2 = 0;
		for (auto const &x: pos_vals[attr_name]) {
			ans2 += (bins2[x].size() * calcEntropy(bins2[x]));
		}

		ans2 /= num_els;
		//std::cout << "discinfogain stop: " << attr_name << "\n";
		return ans1 - ans2;

		//***************/
		/*

		std::set<std::string> target_val_set = target_values;
		std::map< std::string,int > num_attr[num_target_val];

		std::vector< std::string> temp_tar_val;
		for(auto it =target_val_set.begin(); it!= target_val_set.end(); it++){
					temp_tar_val.push_back(*it);
					//std::cout<<*it<<"\n";
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
				//std::cout<<"AAAAAA\n";
				int pos;
				//std::cout<<temp[attr_name]<<"\n";
				if(temp[attr_name] == missing_attr){
						//std::cout<<temp[attr_name]<<"\n";
					for( auto it =temp_tar_val.begin(); it!= temp_tar_val.end(); it++){
							//std::cout<<*it<<"\n";
							if(*it ==els[i].getTargetClass()){
									//std::cout<<"AAAAAA\n";
									pos=it-temp_tar_val.begin();
									break;
							}
					}
					int m = -1;
					//std::cout<<pos<<"\n";
					for(auto it= num_attr[pos].begin(); it!=num_attr[pos].end(); it++){
					//std::cout<<"AAAAAAAA\n";
					if(m <= it->second){
								m = it->second;
								//std::cout<<temp[attr_name]<<" "<<it->first<<"\n";
								temp[attr_name]= it->first;
					}
				}
				els[i]=temp;
			}
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
	 std::cout << "discinfogain end\n";
	 return (entropy_1-entropy_2);*/
}

double DecisionTree::calcEntropy(const std::map< std::string, int>& els){
		double sum=0.0,ans=0.0;
		std::vector<double> temp;
		for( auto it = els.begin(); it!=els.end(); it++){
				sum+= it->second;
				temp.push_back((double)it->second);
		}
		for(int i=0 ; i<temp.size(); i++)
				if (temp[i] != 0) {
					ans+= (temp[i]/sum)*(log2(temp[i]/sum));
				}
		ans*=-1.00;
		return ans;
}

// -------------------- Data Extraction --------------------------------------
std::vector<std::vector<std::string> > Reader::readData(std::string fileloc){
	std::ifstream fin(fileloc,std::ios::in);
	std::vector<std::vector<std::string> > data;
	while(!fin.eof()){
		std::string s;
		std::vector<std::string> att;
		fin>>s;
		std::stringstream str(s);
		while (str) {
			std::string temp;
			std::getline(str,temp,',');
			att.push_back(temp);
		}
		data.push_back(att);
	}
	return data;
}

std::set<std::string> Reader::readTargetVal(std::string fileloc, int n){
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