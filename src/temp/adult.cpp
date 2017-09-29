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

std::pair<std::vector<Instance>, std::vector<std::string>> getTestData(
	const std::string& fileloc, const std::vector<std::string>& attr_names) {
	
	std::vector<Instance> instances;
	std::vector<std::string> target_values;
	/*std::map<std::string, int> fill_missing_val[2][14];
	for(int i=0; i<data.size(); i++){

			std::vector<std::string> temp = data[i];
			for(int j = 0; j<temp.size(); j++){
					if(temp[j] != missing_attr){
							int idx=0;
							for(auto it= target_vals.begin(); it!= target_vals.end(); it++){

									if(*it == temp[temp.size()-2]){
											fill_missing_val[idx][j][temp[j]]++;
											break;
									}
									idx++;
							}
					}
			}
	}

	for(int i=0; i< data.size(); i++){

			std::vector<std::string> temp =data[i];
			for(int j=0; j< temp.size(); j++){
					
					if(temp[j] == missing_attr){
							
							int idx=0;
							for(auto it = target_vals.begin(); it!=target_vals.end(); it++){
									
									if(*it == temp[temp.size()-2]){
										int m=-1;
											for(auto itr= fill_missing_val[idx][j].begin(); itr!= fill_missing_val[idx][j].end(); itr++){
														if(itr->second > m){
																m = itr->second;
																temp[j]=itr->first;
															//	std::cout<<m<<"   "<<temp[j]<<std::endl;
														}

											}
										break;
									}
									idx++;		
							} 
					}
			}
			data[i] = temp;
}

	for(int i=0; i<data.size(); i++){
			for(int j=0; j< data[i].size(); j++)	std::cout<<data[i][j]<<" ";
			std::cout<<std::endl;
	}*/
	std::vector<Example>els = getExamples(fileloc, attr_names);
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
	for (auto x: els) {
		std::vector<std::string> attr_vals;
		for (auto y: x.getEls()) {
			attr_vals.push_back(y.second);
		}
		instances.push_back(Instance(attr_names, attr_vals));
		target_values.push_back(x.getTargetClass());
	}
	std::cout<<instances.size()<<" "<<target_values.size()<<std::endl;
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