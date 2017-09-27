#ifndef DECISION_TREE_H_
#define DECISION_TREE_H_

#include <string>
#include <unordered_map>
#include <vector>

class DecisionTreeNode {
	public:
		DecisionTreeNode();
		DecisionTreeNode(std::string attrName);

		// attrVal must be a possible value of attrName
		DecisionTreeNode*& operator[](std::string attrVal);

		std::string getAttrName();

	private:
		std::string attrName;
		std::unordered_map<std::string, DecisionTreeNode*> child;
};

class Instance {
	public:
		Instance();

		// Precondition: Size of 'attrNames' qual to size of 'attrVals'
		Instance(
			std::vector<std::string> attrNames, std::vector<std::string> attrVals);

		// Used to access value of a particular attribute
		std::string operator[](std::string attrName);

	protected:
		std::unordered_map<std::string, std::string> els;
};

class Example: public Instance {
	public:
		Example();

		// Precondition: Size of 'attrNames' qual to size of 'attrVals'
		Example(
			std::vector<std::string> attrNames, std::vector<std::string> attrVals,
			std::string target_class);

		std::string getTargetClass();

	private:
		std::string target_class;
};

class DecisionTree {
	public:
		void addAttrInfo(std::string attrName, std::vector<std::string> attrVals);
		// all attributes' info must be added before this is called
		void build(std::vector<Example> trainData);
		void prune(std::vector<Example> validationData);
		double test(std::vector<Example> testData);

	private:
		std::string classify(Instance inst);
		// key is attribute
		// value is vector of all possible values for that attribute
		// If the attribute is continuous then value will be empty vector
		std::unordered_map<std::string, std::vector<std::string>> posVals;
};

std::vector<std::vector<std::string> > readData();

#endif