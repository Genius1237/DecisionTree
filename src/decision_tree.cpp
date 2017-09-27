#include "decision_tree.h"
#include <fstream>	
#include <sstream>
// --------------------- DecisionTree Class ---------------------------

DecisionTreeNode::DecisionTreeNode() {
	;
}

void DecisionTreeNode::setAttrName(std::string attr_name) {
	this -> attr_name = attr_name;
}

std::string DecisionTreeNode::getAttrName() {
	return attr_name;
}


// --------------------- discAttrDecisionTreeNode Class ---------------------------
discAttrDecisionTreeNode::discAttrDecisionTreeNode() {
	is_cont = false;
}

DecisionTreeNode*& discAttrDecisionTreeNode::operator[](std::string attr_val) {
	return child[attr_val];
}


// --------------------- contAttrDecisionTreeNode Class ---------------------------
contAttrDecisionTreeNode::contAttrDecisionTreeNode() {
	is_cont = true;
}




// --------------------- Instance Class ---------------------------

Instance::Instance() {
	;
}

Instance::Instance(
	std::vector<std::string> attr_names, std::vector<std::string> attr_vals) {
	for (int i = 0; i < attr_names.size(); i++) {
		els[attr_names[i]] = attr_vals[i];
	}
}

std::string Instance::operator[](std::string attr_name) {
	return els[attr_name];
}

// --------------------- Example Class ---------------------------

Example::Example() {
	;
}

Example::Example(
	std::vector<std::string> attr_names, std::vector<std::string> attr_vals,
	std::string target_class) {
	for (int i = 0; i < attr_names.size(); i++) {
		els[attr_names[i]] = attr_vals[i];
	}
	this -> target_class = target_class;
}

std::string Example::getTargetClass() {
	return target_class;
}

// --------------------- DecisionTree Class ---------------------------

void DecisionTree::addAttrInfo(
	std::string attr_name, std::vector<std::string> attr_vals) {
	pos_vals[attr_name] = attr_vals;
}

void DecisionTree::build(std::vector<Example> train_data) {
	
	build(train_data, root, );
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