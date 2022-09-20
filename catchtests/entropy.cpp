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
    CHECK(entropy(vector<float>{.25,.25,.25,.25})==1);
    CHECK(entropy(vector<float>{1})==0);
    CHECK(weights_entropy(vector<unsigned int>{1,1,1,1})==1);
    CHECK(labels_entropy(vector<string>{"uno","dos","tres","cuatro"})==1);
    CHECK(labels_entropy(vector<int>{331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331, 331})==0);
}