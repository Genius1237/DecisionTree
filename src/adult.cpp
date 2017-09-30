#include <bits/stdc++.h>
#include "decision_tree.h"


std::vector<Example> getExamples(const std::string& fileloc, const std::vector<std::string>& attr_names) {
	std::vector<Example> ret;
	std::vector<std::vector<std::string>> data = Reader::readData(fileloc);
	for (int i = 0; i < data.size(); i++) {
		std::string target_value = data[i][data[i].size() - 2];
		data[i].pop_back();
		data[i].pop_back();
		ret.push_back(Example(attr_names, data[i], target_value));
	}
	return ret;
}

std::vector<Example> getTestData(const std::string& fileloc,
	const std::vector<std::string>& attr_names, bool fill_unknown) {

	std::vector<Example> els = getExamples(fileloc, attr_names);

 	if (fill_unknown) {
	  for (auto const& attr_name: attr_names) {
		  //       target_val ,      attr_val, cnt
			std::map<std::string, std::map<std::string, int>> bins;
			for (int i = 0; i < els.size(); i++) {
				if (els[i][attr_name] != missing_attr) {
					bins[els[i].getTargetClass()][els[i][attr_name]]++;
				}
			}

			// filling of missing vals
			for (int i = 0; i < els.size(); i++) {
				if (els[i][attr_name] == missing_attr) {
					int max = -1;
					std::string attr_val;
					for (auto const &x: bins[els[i].getTargetClass()]) {
						if (x.second > max) {
							max = x.second;
							attr_val = x.first;
						}
					}
					els[i][attr_name] = attr_val;
				}
			}
		}
 	}

	return els;
}

void attrInfo(std::vector<std::vector<std::string>>& dat, std::vector<std::string>& attr_names, DecisionTree& dt){
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
	attrInfo(dat, attr_names, dt);

	// obtain test data
	std::vector<Example> test_data = getTestData(
		"../data/adult_test", attr_names, true);

	/*
	std::vector<Example> test_data = getTestData(
		"../data/adult_test_no_unknown", attr_names, false);
	*/

	// build
	dt.build(getExamples("../data/adult_data", attr_names));

	std::cout << dt.test(test_data) << "\n";
	return 0;
}

/*
Removing unknown
77.0518

real  0m32.348s
user  0m31.276s
sys 0m1.064s

Filling unknown
77.8576

real  0m29.611s
user  0m28.660s
sys 0m0.948s
*/