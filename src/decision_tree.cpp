#include "decision_tree.h"
#include <fstream>	
#include <sstream>
// --------------------- DecisionTree Class ---------------------------

DecisionTreeNode::DecisionTreeNode() {
	;
}

DecisionTreeNode::DecisionTreeNode(std::string attrName) {
	this -> attrName = attrName;
}

std::string DecisionTreeNode::getAttrName() {
	return attrName;
}

DecisionTreeNode*& DecisionTreeNode::operator[](std::string attrVal) {
	return child[attrVal];
}

// --------------------- Instance Class ---------------------------

Instance::Instance() {
	;
}

Instance::Instance(
	std::vector<std::string> attrNames, std::vector<std::string> attrVals) {
	for (int i = 0; i < attrNames.size(); i++) {
		els[attrNames[i]] = attrVals[i];
	}
}

std::string Instance::operator[](std::string attrName) {
	return els[attrName];
}

// --------------------- Example Class ---------------------------

Example::Example() {
	;
}

Example::Example(
	std::vector<std::string> attrNames, std::vector<std::string> attrVals,
	std::string target_class) {
	for (int i = 0; i < attrNames.size(); i++) {
		els[attrNames[i]] = attrVals[i];
	}
	this -> target_class = target_class;
}

std::string Example::getTargetClass() {
	return target_class;
}

// --------------------- DecisionTree Class ---------------------------

void DecisionTree::addAttrInfo(
	std::string attrName, std::vector<std::string> attrVals) {
	posVals[attrName] = attrVals;
}

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
