#pragma once
#include <cell_world/types.h>
#include <json_cpp.h>
#include <iostream>
#include <cinttypes>
#include <string>
#include <vector>

#define SIMILAR(A,B,M) ((A-M < B) && (A+M > B))

namespace cell_world{
    std::vector<unsigned int> histogram(std::vector<unsigned int>);
    std::vector<unsigned int> new_index(unsigned int);
    std::vector<unsigned int> new_index(std::vector<float>, bool);

    template<typename T>
    T max(const std::vector<T> &values){
        T m = values[0];
        for (auto &v:values) m = v>m?v:m;
        return m;
    }

    template<typename T>
    T sum(const std::vector<T> &values){
        T s{};
        for (auto &v:values) s+=v;
        return s;
    }

    template<typename T>
    T min(const std::vector<T> &values){
        T m = values[0];
        for (auto &v:values) m = v<m?v:m;
        return m;
    }

    std::string get_variable(const std::string &, const std::string &);

    bool file_exists (const std::string &);
    bool file_exists (const std::vector<std::string> &);
    bool file_exists (const std::string &, const std::vector<std::string> &);
    bool folder_exists(const std::string &path);
    bool create_folder(const std::string &path);

    float entropy(std::vector<float> probabilities, float base = M_E);

    template<typename T>
    float weights_entropy(std::vector<T> weights, float base = M_E){
        if (weights.empty()) return 0;
        auto total = sum(weights);
        if (total == 0) return 0;
        std::vector<float> probs(weights.size());
        for (unsigned int i = 0 ; i < weights.size(); i++) probs[i] = (float)weights[i] / total ;
        return entropy(probs, base);
    }

    template<typename T>
    float labels_entropy(std::vector<T> labels, float base = M_E){
        if (labels.empty()) return 0;
        std::unordered_map<T, unsigned int> labels_weight;
        for (auto label:labels) {
            if (labels_weight.find(label) == labels_weight.end()) {
                labels_weight[label] = 1;
            } else {
                labels_weight[label]++;
            }
        }
        std::vector<unsigned int> weights;
        for (auto itr = labels_weight.begin(); itr != labels_weight.end(); itr++)  weights.push_back(itr->second);
        return weights_entropy(weights, base);
    }


}