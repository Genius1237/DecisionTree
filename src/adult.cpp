#include <bits/stdc++.h>
#include "decision_tree.h"

std::vector<Example> getExamples(const std::string& fileloc, const std::vector<std::string>& attr_names) {
	std::vector<Example> ret;
	std::vector<std::vector<std::string>> data = Reader::readData(fileloc);
	for (int i = 0; i < data.size(); i++) {
		std::string target_value = data[i][data[i].size() - 2];
		//std::cout << target_value << std::endl;
		data[i].pop_back();
		data[i].pop_back();
		ret.push_back(Example(attr_names, data[i], target_value));
	}
	return ret;
}

std::pair<std::vector<Instance>, std::vector<std::string>> getTestData(
	const std::string& fileloc, const std::vector<std::string>& attr_names) {
	
	std::vector<Instance> instances;
	std::vector<std::string> target_values;
	std::vector<std::vector<std::string>> data = Reader::readData(fileloc);
	
	for (int i = 0; i < data.size(); i++) {
		target_values.push_back(data[i][data[i].size() - 2]);
		data[i].pop_back();
		data[i].pop_back();
		bool cont = false;
		for (int j = 0; j < data[i].size(); j++) {
			if (data[i][j] == "?") {
				target_values.pop_back();
				cont = true;
				break;
			}
		}
		if (cont) {
			continue;
		}
		
		instances.push_back(Instance(attr_names, data[i]));
	}
	return make_pair(instances, target_values);
}

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

	// addTargetValues
	target_vals.insert(">50K");
	target_vals.insert("<=50K");
	dt.addTargetValues(target_vals);

	// addAttrInfo
	std::vector<std::vector<std::string>> dat = Reader::readData("../data/adult_attr");
	func(dat, attr_names, dt);

	// obtain test data
	std::pair<std::vector<Instance>, std::vector<std::string>> test_data = 
		getTestData("../data/adult_test", attr_names);
	
	// build
	dt.build(getExamples("../data/adult_data", attr_names));

	std::cout << dt.test(test_data.first, test_data.second) << "\n";

	return 0;
}