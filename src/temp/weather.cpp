/*#include <bits/stdc++.h>
#include "decision_tree.h"

void func(std::vector<std::vector<std::string>>& dat, std::vector<std::string>& attr_names, DecisionTree& dt){
	for (int i = 0; i < dat.size(); i++) {
		attr_names.push_back(dat[i][0]);
		std::vector<std::string> temp;
		for (int j = 1; j < dat[i].size(); j++) {
			if (dat[i][j] == "continuous") {
				break;
			} else {
				temp.push_back(dat[i][j]);
			}
		}
		dt.addAttrInfo(dat[i][0], temp);
	}
}

int main(){

	DecisionTree dt;
	std::set<std::string> target_vals;
	std::vector<std::string> attr_names;

	// add target value info
	target_vals.insert("yes");
	target_vals.insert("no");
	dt.addTargetValues(target_vals);

	// add attr info
	std::vector<std::vector<std::string>> dat = {
		{
			{"outlook","sunny","overcast","rain"},
			{"temperature","hot","cool","mild"},
			{"humidity","high","normal"},
			{"wind","weak","strong"}
		}
	};
	func (dat, attr_names, dt);

	// create example vector
	std::vector<Example> t;
	std::vector<std::string> ex = {"sunny","hot","high","weak"};
	t.push_back(Example(attr_names, ex, "no"));
	ex = {"sunny","hot","high","strong"};
	t.push_back(Example(attr_names, ex, "no"));
	ex = {"overcast","?","high","weak"};
	t.push_back(Example(attr_names, ex, "yes"));
	ex = {"rain","mild","high","weak"};
	t.push_back(Example(attr_names, ex, "yes"));
	ex = {"rain","cool","normal","weak"};
	t.push_back(Example(attr_names, ex, "yes"));
	ex = {"rain","cool","normal","strong"};
	t.push_back(Example(attr_names, ex, "no"));
	ex = {"overcast","cool","normal","strong"};
	t.push_back(Example(attr_names, ex, "yes"));
	ex = {"sunny","mild","high","weak"};
	t.push_back(Example(attr_names, ex, "no"));
	ex = {"sunny","cool","normal","weak"};
	t.push_back(Example(attr_names, ex, "yes"));
	ex = {"rain","mild","normal","weak"};
	t.push_back(Example(attr_names, ex, "yes"));
	ex = {"sunny","mild","normal","strong"};
	t.push_back(Example(attr_names, ex, "yes"));
	ex = {"overcast","mild","high","strong"};
	t.push_back(Example(attr_names, ex, "yes"));
	ex = {"overcast","hot","normal","weak"};
	t.push_back(Example(attr_names, ex, "yes"));
	ex = {"rain","mild","high","strong"};
	t.push_back(Example(attr_names, ex, "no"));

	dt.build(t);

	dt.print();

	Instance inst(attr_names, {"overcast","hot","high","weak"});
	std::cout << dt.classify(inst) << "\n";
	return 0;
}
*/