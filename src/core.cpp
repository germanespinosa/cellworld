#include <cell_world/core.h>
#include <algorithm>
#include <cmath>
#include <sys/stat.h>
#include <filesystem>
#include <stdlib.h>
#include <cell_world/location.h>
#include <cell_world/geometry.h>

using namespace std;

namespace cell_world {

    string get_variable(const string &variable_name, const string &default_value = ""){
        char * val = getenv( variable_name.c_str() );
        return val == NULL ? default_value : std::string(val);
    }

    float logb(float a, float b)
    {
        return log(a) / log(b);
    }

    float entropy(vector<float> probabilities, float base) {
        if (probabilities.size()<=1) return 0;
        float ent = 0;
        for (auto p : probabilities){
            if (p>0)  ent -= p * logb(p, base);
        }
        return ent / logb((float)probabilities.size(), base);
    }

    bool file_exists(const string &file_path){
        struct stat buffer;
        return (stat (file_path.c_str(), &buffer) == 0);
    }

    bool file_exists(const std::vector<std::string> &file_paths){
        for (auto &file_path : file_paths) {
            if (!file_exists(file_path)) return false;
        }
        return true;
    }

    bool file_exists(const std::string &path, const std::vector<std::string> &file_names){
        string fixed_path = path;
        if (!fixed_path.ends_with('/')) fixed_path += '/';
        for (auto &file_name : file_names) {
            if (!file_exists(fixed_path + file_name)) return false;
        }
        return true;
    }

    bool folder_exists(const string &path){
        return filesystem::exists(path);
    }

    bool create_folder(const string &path){
        if (filesystem::exists(path)) return false;
        filesystem::create_directory(path);
        return true;
    }

    bool angle_between(float v, float l1, float l2, bool inclusive){
        if (inclusive) {
            if (angle_difference(v,l1)==0 || angle_difference(v,l2)==0) return true;
        }
        return (direction(v,l1) + direction(v,l2)) == 0;
    }

    bool segments_intersect(const Location &segment1_point1, const Location &segment1_point2, const Location &segment2_point1, const Location &segment2_point2)
    {
        auto t1 = segment1_point1.atan(segment1_point2);
        auto t11 = segment1_point1.atan(segment2_point1);
        auto t12 = segment1_point1.atan(segment2_point2);
        if (!angle_between(t1,t11,t12)) return false;
        auto t2 = segment2_point1.atan(segment2_point2);
        auto t21 = segment2_point1.atan(segment1_point1);
        auto t22 = segment2_point1.atan(segment1_point2);
        if (!angle_between(t2,t21,t22)) return false;
        return true;
    }

    json_cpp::Json_vector<float> Location_list::get_x() {
        auto x = json_cpp::Json_vector<float>();
        for (auto &l:*this){
            x.push_back(l.x);
        }
        return x;
    }

    json_cpp::Json_vector<float> Location_list::get_y() {
        auto y = json_cpp::Json_vector<float>();
        for (auto &l:*this){
            y.push_back(l.y);
        }
        return y;
    }

    float entropy(const std::vector<int>& histogram) {
        vector<float> prob;
        int c = 0;
        for (int h:histogram) c+=h;
        for (int h:histogram) if ( h > 0 ) prob.push_back((float)h/(float)c);
        float ent=0;
        for (float p:prob) ent += p * log2(p);
        return -ent;
    }

    vector<unsigned int> histogram(vector<unsigned int> values) {
        if (values.empty()) {
            vector<unsigned int> hist;
            return hist;
        } else {
            int min = values[0];
            int max = values[0];
            for (int v:values) if ( min > v ) min = v; else if ( max < v ) max = v;
            vector<unsigned int> hist(max - min + 1 );
            for (int v:values) hist[v-min]++;
            return hist;
        }
    }

    vector<unsigned int> new_index(unsigned int n) {
        std::vector<unsigned int> index(n);
        for(unsigned int i=0;i<n;i++) index[i]=i;
        return index;
    }

    vector<unsigned int> new_index(vector<float>values, bool ascending = true){
        auto index = new_index(values.size()); // creates an index vector for the options
        for (unsigned int i = 1; i < index.size(); i++) // sort the indexes of the options by expected reward descending
            for (unsigned int j = i; j > 0 && (ascending?values[index[j - 1]] > values[index[j]]:values[index[j - 1]] < values[index[j]]); j--)
                swap(index[j - 1], index[j]);
        return index;
    }

}

