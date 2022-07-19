#include<catch.h>
#include<cell_world.h>
#include<iostream>

using namespace cell_world;
using namespace std;


float l_entropy(std::vector<string> labels, float base = M_E){
    if (labels.empty()) return 0;
    std::unordered_map<string, unsigned int> labels_weight;
    for (auto label:labels) {
        if (labels_weight.find(label) == labels_weight.end()) {
            labels_weight[label] = 1;
        } else {
            labels_weight[label]++;
        }
    }
    std::vector<unsigned int> weights;
    for (auto itr = labels_weight.begin(); itr != labels_weight.end(); itr++)
        weights.push_back(itr->second);
    return weights_entropy(weights, base);
}

TEST_CASE("entropy")
{
    cout << entropy(vector<float>{.25,.25,.25,.25}) << endl;
    cout << entropy(vector<float>{1}) << endl;
    cout << weights_entropy(vector<unsigned int>{1,1,1,1}) << endl;
    cout << labels_entropy(vector<string>{"uno","dos","tres","cuatro"}) << endl;
    cout << labels_entropy(vector<int>{331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331}) << endl;
}