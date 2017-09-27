#include<bits/stdc++.h>
#include "decision_tree.h"

std::vector<Example> getExamples(const std::string& fileloc, const std::vector<std::string>& attrNames) {
	std::vector<Example> res;
	std::vector<std::vector<std::string>> data = Reader::readData(fileloc, attrNames.size());
	for (int i = 0; i < data.size(); i++) {
		std::string target_value = data[i][data[i].size() - 1];
		data[i].pop_back();
		res.push_back(Example(attrNames, data[i], target_value));
	}
	return res;
}

int main(){
	
	getExamples("../data/adult_data", {});
	return 0;
}
