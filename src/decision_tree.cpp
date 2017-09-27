#include "decision_tree.h"

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