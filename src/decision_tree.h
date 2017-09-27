#ifndef DECISION_TREE_H_
#define DECISION_TREE_H_

#include <string>
#include <unordered_map>
#include <vector>
#include <set>
#include <map>

class DecisionTreeNode {
	public:
		DecisionTreeNode();
		void setAttrName(const std::string& attr_name);
		std::string getAttrName();
		void setType(const std::string& type);
	protected:
		std::string attr_name;
		std::string type; // one of 'discrete', 'continuous' and 'leaf'
};

class DiscAttrDecisionTreeNode: public DecisionTreeNode {
	public:
		DiscAttrDecisionTreeNode();
		// attr_val must be a possible value of attr_name
		DecisionTreeNode*& operator[](const std::string& attr_val);
	private:
		std::unordered_map<std::string, DecisionTreeNode*> child;
};

class ContAttrDecisionTreeNode: public DecisionTreeNode {
	public:
		ContAttrDecisionTreeNode();
		DecisionTreeNode*& operator[](const double& attr_val);
		void setDividers(const std::vector<double> dividers);
	private:
		std::vector<double> dividers;
		std::vector<DecisionTreeNode*> child;
};

class Instance {
	public:
		Instance();

		// Precondition: Size of 'attr_names' qual to size of 'attr_vals'
		Instance(
			const std::vector<std::string>& attr_names, const std::vector<std::string>& attr_vals);

		// Used to access value of a particular attribute
		std::string operator[](const std::string& attr_name);

	protected:
		std::unordered_map<std::string, std::string> els;
};

class Example: public Instance {
	public:
		Example();

		// Precondition: Size of 'attr_names' qual to size of 'attr_vals'
		Example(
			const std::vector<std::string>& attr_names, const std::vector<std::string>& attr_vals,
			const std::string& target_class);

		std::string getTargetClass() const;

	private:
		std::string target_class;
};

class DecisionTree {
	public:
		void addAttrInfo(const std::string& attr_name,
			const std::vector<std::string>& attr_vals);
		// all attributes' info must be added before this is called
		void build(const std::vector<Example>& train_data);
		void prune(const std::vector<Example>& validation_data);
		double test(const std::vector<Example>& test_data);

	private:
		std::string classify(const Instance& inst);
		void build(std::vector<Example> train_data, DecisionTreeNode*& p,
			std::vector<std::string> check_attr);
		double infoGain(std::vector<Example> els, const std::string& attr_name);
		double calcEntropy(const std::map<std::string, int>& els);
		// key is attribute
		// value is vector of all possible values for that attribute
		// If the attribute is continuous then value will be empty vector
		std::unordered_map<std::string, std::vector<std::string> > pos_vals;
		DecisionTreeNode *root;
};

std::vector<std::vector<std::string> > readData();
std::set <std::string> readTargetVal();

#endif
