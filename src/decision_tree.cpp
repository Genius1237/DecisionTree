#include "decision_tree.h"

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