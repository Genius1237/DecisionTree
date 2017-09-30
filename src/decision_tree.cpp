#include "decision_tree.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iostream>




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

std::pair<std::vector<std::string>, std::vector<DecisionTreeNode*>>
	DiscAttrDecisionTreeNode::getChildPointers() {

	std::vector<std::string> attr_vals;
	std::vector<DecisionTreeNode*> p;
	for (auto const &x: child) {
		attr_vals.push_back(x.first);
		p.push_back(x.second);
	}
	return make_pair(attr_vals, p);
}

// --------------------- contAttrDecisionTreeNode Class ---------------------------
ContAttrDecisionTreeNode::ContAttrDecisionTreeNode() {
	type = "continuous";
}

int ContAttrDecisionTreeNode::getIndex(const double& attr_val) {
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

void ContAttrDecisionTreeNode::setDividers(const std::vector<double>& dividers) {
	this -> dividers = dividers;
	sort((this -> dividers).begin(), (this -> dividers).end());
	child.resize((this -> dividers).size() + 1);
}

std::vector<DecisionTreeNode*> ContAttrDecisionTreeNode::getChildPointers() {
	return child;
}

DecisionTreeNode*& ContAttrDecisionTreeNode::getChildPointer(const int& index) {
	return child[index];
}

// --------------------- Instance Class ---------------------------

Instance::Instance() {
	;
}

Instance::Instance(const Example& exmp) {
	els = exmp.els;
}

Instance::Instance(
	const std::vector<std::string>& attr_names, const std::vector<std::string>& attr_vals) {
	for (int i = 0; i < attr_names.size(); i++) {
		els[attr_names[i]] = attr_vals[i];
	}
}

std::string Instance::operator[](const std::string& attr_name) const {
	return els.at(attr_name);
}

void Instance::setAttrVal(const std::string& attr_name, const std::string& attr_val) {
  els[attr_name] = attr_val;
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

void DecisionTree::addTargetValues(const std::set<std::string>& target_values) {
	this -> target_values = target_values;
}

void DecisionTree::build(const std::vector<Example>& train_data) {
	std::vector<std::string> all_attr;
	for (auto it = pos_vals.begin(); it != pos_vals.end(); it++) {
		all_attr.push_back(it -> first);
	}
	int nodes = 0;
	build(train_data, root, all_attr, nodes);
}

void DecisionTree::build(std::vector<Example> train_data,
	DecisionTreeNode*& p, std::vector<std::string> check_attr, int& nodes) {

	// check if there is any training data. if not assign a target class randomly
	if (check_attr.empty()) {
		p = new DecisionTreeNode;++nodes;
		if (train_data.empty()) {
			//std::cout << "unknown" << std::endl;
			p -> setAttrName(*(target_values.begin()));
		} else {
			p -> setAttrName(train_data[0].getTargetClass());
		}
		p -> setType("leaf");
		return;
	}

	if (train_data.empty()) {
		p = new DecisionTreeNode;++nodes;
		//std::cout << "train_data.size() == 0" << std::endl;
		p -> setAttrName(*(target_values.begin()));
		p -> setType("leaf");
		return;
	}

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
		p = new DecisionTreeNode;++nodes;
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
				double cand_gain = discInfoGain(train_data, check_attr[i], false);

				if (cand_gain > max_gain) {
					max_gain = cand_gain;
					max_index = i;
					is_cont = false;
				}
			}
		}

		// now, the attribute to be placed has been found
    std::string attr_name = check_attr[max_index];
    check_attr.erase(check_attr.begin() + max_index);

    if (is_cont) {
      p = new ContAttrDecisionTreeNode;++nodes;
      p -> setType("continuous");
      p -> setAttrName(attr_name);

      ContAttrDecisionTreeNode *pp = static_cast<ContAttrDecisionTreeNode*>(p);
      pp -> setDividers(dividers);

      std::vector<std::vector<Example>> bins;
      bins.resize(dividers.size() + 1);
      for (int i = 0; i < train_data.size(); i++) {
      	bins[pp -> getIndex(atof(train_data[i][attr_name].c_str()))].push_back(train_data[i]);
      }

      // iterating through each child
      for (int i = 0; i <= dividers.size(); i++) {
        build(bins[i], pp -> getChildPointer(i), check_attr, nodes);
      }

    } else {
      discInfoGain(train_data, attr_name, true);

      p = new DiscAttrDecisionTreeNode;++nodes;
      p -> setType("discrete");
      p -> setAttrName(attr_name);

      DiscAttrDecisionTreeNode *pp = static_cast<DiscAttrDecisionTreeNode*>(p);

      std::map<std::string, std::vector<Example>> bins;
      for (int i = 0; i < train_data.size(); i++) {
      	bins[train_data[i][attr_name]].push_back(train_data[i]);
      }

			//std::cout << "build 3b1b" << std::endl;
      for (int i = 0; i < pos_vals[attr_name].size(); i++) {
				build(bins[pos_vals[attr_name][i]], (*pp)[pos_vals[attr_name][i]],
					check_attr, nodes);
			}
		}
	}
}

double DecisionTree::test(const std::vector<Example>& test_data) {

	int correct = 0, wrong = 0;
	for (int i = 0; i < test_data.size(); i++) {
		Instance temp(test_data[i]);
		if (classify(temp) == test_data[i].getTargetClass()) {
			++correct;
		} else {
			++wrong;
		}
	}
	return ((static_cast<double>(correct) / (wrong + correct)) * 100);
}

void DecisionTree::print() {
	print(root);
}

std::string DecisionTree::classify(const Instance& inst) {
	return classify(inst, root);
}

std::string DecisionTree::classify(const Instance& inst, DecisionTreeNode *p) {
	if (p -> getType() == "leaf") {
		return p -> getAttrName();
	} else if (p -> getType() == "continuous") {
		ContAttrDecisionTreeNode *pp = static_cast<ContAttrDecisionTreeNode*>(p);
		return classify(inst, (*pp)[atof((inst[p -> getAttrName()]).c_str())]);
	} else {
		DiscAttrDecisionTreeNode *pp = static_cast<DiscAttrDecisionTreeNode*>(p);
		return classify(inst, (*pp)[inst[p -> getAttrName()]]);
	}
}


void DecisionTree::print(DecisionTreeNode *p) {
	std::cout << (p -> getAttrName()) << "\n";
	if (p -> getType() == "discrete") {
		DiscAttrDecisionTreeNode* pp = static_cast<DiscAttrDecisionTreeNode*>(p);
		std::vector<DecisionTreeNode*> child_pointers;
		auto temp = pp -> getChildPointers();
		for (int i = 0; i < temp.first.size(); i++) {
			std::cout << temp.first[i] << "\n";
			print(temp.second[i]);
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

std::pair<double, std::vector<double> > DecisionTree::contInfoGain(const std::vector<Example>& els,const std::string& attr_name){
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

double DecisionTree::discInfoGain(std::vector<Example>& els_ref, const std::string& attr_name, bool in_place){
  if (in_place) {
    std::vector<Example>& els = els_ref;
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
  			els[i].setAttrVal(attr_name, attr_val);
  		}
  	}
    return 0;
  
  } else {
    std::vector<Example> els = els_ref;
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
        els[i].setAttrVal(attr_name, attr_val);
      }
    }

    //        attr_val              target_val  occ
    std::map<std::string, std::map<std::string, int>> bins2;
    for (int i = 0; i < els.size(); i++) {
      bins2[els[i][attr_name]][els[i].getTargetClass()] += 1;
    }

    //        target_val  occ
    std::map<std::string, int> temp;
    for (int i = 0; i < els.size(); i++) {
      temp[els[i].getTargetClass()]++;
    }

    // info gain calculation
    double ans1 = calcEntropy(temp);
    //std::cout << attr_name << std::endl;
    double ans2 = 0;
    for (auto const &x: pos_vals[attr_name]) {
      int local_cnt = 0;
      for (auto const &y: bins2[x]) {
        local_cnt += y.second;
      }
      ans2 += (local_cnt * calcEntropy(bins2[x]));
    }
    ans2 /= els.size();
    return ans1 - ans2;
  }

}

double DecisionTree::calcEntropy(const std::map< std::string, int>& els){
	double sum=0.0,ans=0.0;
	std::vector<double> temp;
	for( auto it = els.begin(); it!=els.end(); it++){
			sum+= it->second;
			temp.push_back((double)it->second);
	}
	for(int i=0 ; i<temp.size(); i++) {
		if (temp[i] != 0) {
			ans+= (temp[i]/sum)*(log2(temp[i]/sum));
		}
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