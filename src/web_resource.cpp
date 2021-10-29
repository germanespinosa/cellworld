#include <cell_world/web_resource.h>
#include <string>
#include <json_cpp.h>
#include <chrono>

using namespace std;

namespace cell_world {
    Web_resource Web_resource::from(const string resource) {
        Web_resource wr;
        wr._resource = resource;
        return wr;
    }

    Web_resource &Web_resource::key(const string &k) {
        _keys.push_back(k);
        return *this;
    }

    Web_resource &Web_resource::key(int k) {
        return key(to_string(k));
    }

    Web_resource &Web_resource::key(unsigned int k) {
        return key(to_string(k));
    }

    Web_resource &Web_resource::key(const char *k) {
        return key(string(k));
    }

    string _cache_invalidation(){
        stringstream ss;
        using namespace std::chrono;
        ss << "?r=" << duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
        return ss.str();
    }

    std::istream &Web_resource::get() {
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

    std::string Web_resource::url() {
        string url = "https://raw.githubusercontent.com/germanespinosa/cellworld_data/master/" + _resource + "/" +
                     _file_name();
        return url;
    }

    std::string Web_resource::_file_name() {
        string fn;
        bool first = true;
        for (auto &k : _keys) {
            if (!first) fn += '.';
            first = false;
            fn += k;
        }
        return fn;
    }

    std::string &Web_resource::cache_folder() {
        static string folder(std::getenv("CELLWORLD_CACHE")?std::getenv("CELLWORLD_CACHE"):".");
        return folder;
    }
}