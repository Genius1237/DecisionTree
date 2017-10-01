#ifndef DECISION_TREE_H_
#define DECISION_TREE_H_

#include <string>
#include <unordered_map>
#include <vector>
#include <set>
#include <map>

typedef long long ll;

const std::string missing_attr= "?";

class DecisionTreeNode {
	public:
		DecisionTreeNode();
		void setAttrName(const std::string& attr_name);
		std::string getAttrName();
		void setType(const std::string& type);
		std::string getType();
	protected:
		std::string attr_name; // if type is 'leaf' then this will be a target value
		std::string type; // one of 'discrete', 'continuous' and 'leaf'
};

class DiscAttrDecisionTreeNode: public DecisionTreeNode {
	public:
		DiscAttrDecisionTreeNode();

		// attr_val must be a possible value of attr_name
		DecisionTreeNode*& operator[](const std::string& attr_val);
		std::pair<std::vector<std::string>, std::vector<DecisionTreeNode*>> getChildPointers();
	private:
		std::unordered_map<std::string, DecisionTreeNode*> child;
};

class ContAttrDecisionTreeNode: public DecisionTreeNode {
	public:
		ContAttrDecisionTreeNode();

		DecisionTreeNode*& operator[](const double& attr_val);

		// this must be called before using operator[]
		void setDividers(const std::vector<double>& dividers);

		// return value 'r' is such that attr_val will belong to child[r]
		ll getIndex(const double& attr_val);

		std::vector<DecisionTreeNode*> getChildPointers();

		DecisionTreeNode*& getChildPointer(const ll& index);
	private:
		std::vector<double> dividers;
		std::vector<DecisionTreeNode*> child;
};

class Example;

class Instance {
	public:
		Instance();

		Instance(const Example& exmp);

		// Precondition: Size of 'attr_names' qual to size of 'attr_vals'
		Instance(
			const std::vector<std::string>& attr_names, const std::vector<std::string>& attr_vals);

		// Used to access value of a particular attribute
    std::string operator[](const std::string& attr_name) const;

    //std::string& operator[](const std::string& attr_name);

    void setAttrVal(const std::string& attr_name, const std::string& attr_val);

		friend std::ostream& operator<<(std::ostream& out, const Instance& inst);

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
		// Used to populate 'pos_vals'
		void addAttrInfo(const std::string& attr_name,
			const std::vector<std::string>& attr_vals);

		// Used to populate 'target_values'
		void addTargetValues(const std::set<std::string>& target_values);

		// 'addAttrInfo' and 'addTargetValues' must be used before calling
		// this function
		void build(const std::vector<Example>& train_data);

		// 'build' must be called before calling this function
		void prune(const std::vector<Example>& validation_data);

		// 'build' must be called before calling this function
		double test(const std::vector<Example>& test_data);

		// Returns the target value given to the instance 'inst' using the
		// already built decision tree
		std::string classify(const Instance& inst);

		void print();


	private:

		// used by pubic 'classify'
		std::string classify(const Instance& inst, DecisionTreeNode *p);

		// Used by public 'build'
		void build(std::vector<Example> train_data, DecisionTreeNode*& p,
			std::vector<std::string> check_attr, ll& nodes);

		// Returns information gain
		// 'attr_name' must be a discrete-valued attribute
    double discInfoGain(std::vector<Example>& els_ref,
      const std::string& attr_name, bool in_place);

		// Returns a pair (info gain, vector of dividers)
		// 'attr_name' must be a continous-valued attribute
		std::pair<double, std::vector<double>> contInfoGain(
      const std::vector<Example>& els, const std::string& attr_name);

		// Returns entropy given a map
		// (target value, num of occurrences of that target value)
		double calcEntropy(const std::map<std::string, ll>& els);

		void print(DecisionTreeNode *p);

		// key is attribute
		// value is vector of all possible values for that attribute
		// If the attribute is continuous then value will be empty vector
		std::unordered_map<std::string, std::vector<std::string> > pos_vals;

		DecisionTreeNode *root;

    std::set<std::string> target_values;
};

class Reader {
	public:
		// file name and number of attributes
		static std::vector<std::vector<std::string> > readData(std::string fileloc);
		static std::set <std::string> readTargetVal(std::string fileloc, ll n);
};


#endif
