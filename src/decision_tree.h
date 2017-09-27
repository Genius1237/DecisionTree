#ifndef DECISION_TREE_H_
#define DECISION_TREE_H_

#include <string>
#include <unordered_map>
#include <vector>
#include <set>

class DecisionTreeNode {
	public:
		DecisionTreeNode();
		void setAttrName(std::string attr_name);
		std::string getAttrName();
	protected:
		std::string attr_name;
		bool is_cont;
};

class discAttrDecisionTreeNode: public DecisionTreeNode {
	public:
		discAttrDecisionTreeNode();
		// attr_val must be a possible value of attr_name
		DecisionTreeNode*& operator[](std::string attr_val);
	private:
		std::unordered_map<std::string, DecisionTreeNode*> child;
};

class contAttrDecisionTreeNode: public DecisionTreeNode {
	public:
		contAttrDecisionTreeNode();
	private:
		std::vector<double> dividers;
		std::vector<DecisionTreeNode*> child;
};

class Instance {
	public:
		Instance();

		// Precondition: Size of 'attr_names' qual to size of 'attr_vals'
		Instance(
			std::vector<std::string> attr_names, std::vector<std::string> attr_vals);

		// Used to access value of a particular attribute
		std::string operator[](std::string attr_name);

	protected:
		std::unordered_map<std::string, std::string> els;
};

class Example: public Instance {
	public:
		Example();

		// Precondition: Size of 'attr_names' qual to size of 'attr_vals'
		Example(
			std::vector<std::string> attr_names, std::vector<std::string> attr_vals,
			std::string target_class);

		std::string getTargetClass();

	private:
		std::string target_class;
};

class DecisionTree {
	public:
		void addAttrInfo(std::string attr_name, std::vector<std::string> attr_vals);
		// all attributes' info must be added before this is called
		void build(std::vector<Example> train_data);
		void prune(std::vector<Example> validation_data);
		double test(std::vector<Example> test_data);

	private:
		std::string classify(Instance inst);
		void build(std::vector<Example> train_data, DecisionTreeNode*& p, 
			std::vector<std::string> check_attr);
		double infoGain(std::vector<Example> els, std::string attr_name);
		// key is attribute
		// value is vector of all possible values for that attribute
		// If the attribute is continuous then value will be empty vector
		std::unordered_map<std::string, std::vector<std::string>> pos_vals;
		DecisionTreeNode *root;
};

std::vector<std::vector<std::string> > readData();
std::set <std::string> readTargetVal();

#endif
