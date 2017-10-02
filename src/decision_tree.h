#ifndef DECISION_TREE_H_
#define DECISION_TREE_H_

#include <string>
#include <unordered_map>
#include <vector>
#include <set>
#include <map>

const std::string missing_attr= "?";

class DecisionTreeNode {
	public:
		DecisionTreeNode();

		// Sets 'this -> attr_name' to 'attr_name'
		void setAttrName(const std::string& attr_name);

		// Returns 'attr_name'
		std::string getAttrName();

		// Sets 'this -> type' to 'type'
		void setType(const std::string& type);

		// Returns 'type'
		std::string getType();

		// Sets 'this -> max_target_val' to 'target_val'
		void setMaxTargetVal(const std::string& max_target_val);

		// Returns 'max_target_val'
		std::string getMaxTargetVal();
	protected:
		std::string attr_name; // if type is 'leaf' then this will be a target value
		std::string type; // one of 'discrete', 'continuous' and 'leaf'
		std::string max_target_val; // max occuring target value
};

class DiscAttrDecisionTreeNode: public DecisionTreeNode {
	public:
		DiscAttrDecisionTreeNode();

		// Returns pointer to the child that corresponds to 'attr_val'
		// attr_val must be a possible value of attr_name
		DecisionTreeNode*& operator[](const std::string& attr_val);

		// Returns all child pointers of the current node
		std::pair<std::vector<std::string>, std::vector<DecisionTreeNode*>> getChildPointers();
	private:
		std::unordered_map<std::string, DecisionTreeNode*> child;
};

class ContAttrDecisionTreeNode: public DecisionTreeNode {
	public:
		ContAttrDecisionTreeNode();

		// Returns pointer to the child that corresponds to 'attr_val'
		DecisionTreeNode*& operator[](const double& attr_val);

		// Sets 'this -> dividers' to 'dividers'
		// This must be called before using operator[]
		void setDividers(const std::vector<double>& dividers);

		// Returns value 'r' such that 'attr_val' will belong to 'child[r]'
		int getIndex(const double& attr_val);

		// Returns pointers to all children of the current node
		std::vector<DecisionTreeNode*> getChildPointers();

		// Returns child corresponding to index 'index' (which is child[index])
		DecisionTreeNode*& getChildPointer(const int& index);
	private:
		std::vector<double> dividers;
		std::vector<DecisionTreeNode*> child;
};

class Example;

class Instance {
	public:
		Instance();

		// Creates an instance of 'Instance' from an instance of 'Example'
		Instance(const Example& exmp);

		// Creates an instance of 'Instance' by associating corresponding elements from
		//'attr_names' and 'attr_vals' with each other
		Instance(
			const std::vector<std::string>& attr_names, const std::vector<std::string>& attr_vals);

		// Used to access value corresponding to the attribute 'attr_name'
    std::string operator[](const std::string& attr_name) const;

    // Associates the value 'attr_val' to the attribute name 'attr_name'
    void setAttrVal(const std::string& attr_name, const std::string& attr_val);

		friend std::ostream& operator<<(std::ostream& out, const Instance& inst);

	protected:
		std::unordered_map<std::string, std::string> els;
};

class Example: public Instance {
	public:
		Example();

		// Creates an instance of 'Instance' by associating corresponding elements from
		//'attr_names' and 'attr_vals' with each other. Also assigns 'this -> target_class'
		// to 'target_class'
		Example(
			const std::vector<std::string>& attr_names, const std::vector<std::string>& attr_vals,
			const std::string& target_class);

		// Returns 'target_class'
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
		void addTargetValues(const std::vector<std::string>& target_values);

		// Builds the tree using the training data 'train_data'
		// 'addAttrInfo' and 'addTargetValues' must be used before calling
		// this function
		void build(const std::vector<Example>& train_data);

		// Prunes the tree using the pruning data 'prune_data'
		// 'build' must be called before calling this function
		void prune(const std::vector<Example>& prune_data);

		// Returns the accuracy by testing the tree with 'test_data'
		// 'build' must be called before calling this function
		double test(const std::vector<Example>& test_data);

		// Returns the target value assigned to the instance 'inst' using the
		// already built decision tree
		std::string classify(const Instance& inst);

		// Prints the preorder traversal of the tree
		void print();

		void printStats(const std::vector<Example>& test_data);

	protected:
		// Prunes the tree rooted at 'p' using the pruning data 'prune_data'
		int prune(DecisionTreeNode* p, std::vector<Example> prune_data);

		// Classifies the instance 'inst' using the tree rooted at 'p'
		std::string classify(const Instance& inst, DecisionTreeNode *p);

		// Used by public 'build'
		void build(std::vector<Example> train_data, DecisionTreeNode*& p,
			std::vector<std::string> check_attr, int& nodes);

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
		double calcEntropy(const std::map<std::string, int>& els);

		// Prints the tree rooted at 'p'
		void print(DecisionTreeNode *p);

		// key is attribute
		// value is vector of all possible values for that attribute
		// If the attribute is continuous then value will be empty vector
		std::unordered_map<std::string, std::vector<std::string> > pos_vals;

		DecisionTreeNode *root;

    std::vector<std::string> target_values;
};

class Reader {
	public:
		// Returns data read from the file 'fileloc'
		static std::vector<std::vector<std::string> > readData(std::string fileloc);
		static std::set <std::string> readTargetVal(std::string fileloc, int n);
};

class RandomForest : public DecisionTree{
	private:
		std::vector<DecisionTreeNode*> trees;

		int no_of_trees;

		void build(std::vector<Example> train_data,
	DecisionTreeNode*& p, std::vector<std::string> check_attr, int& nodes);

	public:
		//Constructor that takes the number of trees in the forest
		RandomForest(int no_of_trees);

		void build(const std::vector<Example>& train_data);

		std::string classify(const Instance& inst);

		void print();

		double test(const std::vector<Example>& test_data);

		void printStats(const std::vector<Example>& test_data);

};

#endif
