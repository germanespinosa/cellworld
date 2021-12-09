#include <cell_world/resources.h>
#include <string>
#include <json_cpp.h>
#include <chrono>
#include <cell_world/core.h>

using namespace std;

namespace cell_world {
    Resources Resources::from(const string resource) {
        Resources wr;
        wr._resource = resource;
        return wr;
    }

    Resources &Resources::key(const string &k) {
        _keys.push_back(k);
        return *this;
    }

    Resources &Resources::key(int k) {
        return key(std::to_string(k));
    }

    Resources &Resources::key(unsigned int k) {
        return key(std::to_string(k));
    }

    Resources &Resources::key(const char *k) {
        return key(string(k));
    }

    string _cache_invalidation(){
        stringstream ss;
        using namespace std::chrono;
        ss << "?r=" << duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
        return ss.str();
    }

    std::istream &Resources::get() {
        string cfn = cache_folder() + "/" + _resource + "." + _file_name();
        ifstream cache(cfn);
        if (cache.good()) {
            _resource_stream = std::move(cache);
            return _resource_stream;
        }
        { // time to download the resource
            ofstream cache_file;
            cache_file.open(cfn);
            auto wr = Json_web_get(url() + _cache_invalidation());
            cache_file << wr.get_string();
            cache_file.close();
        }
        return _resource_stream = ifstream(cfn);
    }

    std::string Resources::url() {
        string url = "https://raw.githubusercontent.com/germanespinosa/cellworld_data/master/" + _resource + "/" +
                     _file_name();
        return url;
    }

    std::string Resources::_file_name() {
        string fn;
        bool first = true;
        for (auto &k : _keys) {
            if (!first) fn += '.';
            first = false;
            fn += k;
        }
        return fn;
    }

    std::string &Resources::cache_folder() {
        static string folder(std::getenv("CELLWORLD_CACHE")?std::getenv("CELLWORLD_CACHE"):".cellworld_cache");
        create_folder(folder);
        return folder;
    }

    std::string Resources::to_string() {
        stringstream ss;
        ss << get().rdbuf();
        return ss.str();
    }
}