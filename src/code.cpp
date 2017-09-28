#include <bits/stdc++.h>
#include "decision_tree.h"

std::vector<Example> getExamples(const std::string& fileloc, const std::vector<std::string>& attr_names) {
	std::vector<Example> ret;
	std::vector<std::vector<std::string>> data = Reader::readData(fileloc);
	for (int i = 0; i < data.size(); i++) {
		std::string target_value = data[i][data[i].size() - 1];
		data[i].pop_back();
		ret.push_back(Example(attr_names, data[i], target_value));
	}
	return ret;
}

int main(){

	DecisionTree dt;
	std::set<std::string> target_vals;
	std::vector<std::string> attr_names;

	// addTargetValues
	target_vals.insert(">50K");
	target_vals.insert("<=50K");
	dt.addTargetValues(target_vals);

	// addAttrInfo
	std::vector<std::vector<std::string>> dat = Reader::readData("../data/adult_attr");
	for (int i = 0; i < dat.size(); i++) {
		attr_names.push_back(dat[i][0]);
		std::vector<std::string> temp;
		for (int j = 1; j < dat[i].size(); j++) {
			if (dat[i][j] == "continous") {
				break;
			} else {
				temp.push_back(dat[i][j]);
			}
		}
		dt.addAttrInfo(dat[i][0], temp);
	}

	// build
	dt.build(getExamples("../data/adult_data", attr_names));

	dt.print();
	return 0;
}
