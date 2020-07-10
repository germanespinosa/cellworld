#pragma once
#include <any>

namespace cell_world{
    enum class Output_format{
        List,
        Object
    };

    enum class Json_value_type{
        Boolean_value,
        Unsigned_int_value,
        Int_value,
        Double_value,
        String_value,
        Json_value
    };

    void set_output_format(Output_format);

    struct Json_util {
        static void discard(std::istream &);
        static char skip_blanks(std::istream &, bool);
        static char skip_blanks(std::istream &);
        static std::string read_string(std::istream &);
        static double read_double(std::istream &);
        static int read_int(std::istream &);
        static bool read_name(std::string &, std::istream &);
    };

    struct Json_base {
        virtual void json_parse(std::istream &) = 0;
        virtual void json_write(std::ostream &) = 0;
        friend std::istream & operator >> (std::istream &, Json_base &);
        friend std::ostream& operator << (std::ostream& , Json_base&);
    };

    struct Json_parser : Json_base{
        struct Json_descriptor {
            std::string name;
            bool mandatory;
            Json_value_type type;
            unsigned long _index{};
        };
        void json_add_member(std::string, bool, bool &);
        void json_add_member(std::string, bool, double &);
        void json_add_member(std::string, bool, int &);
        void json_add_member(std::string, bool, unsigned int &);
        void json_add_member(std::string, bool, std::string &);
        void json_add_member(std::string, bool, Json_base &);
        std::vector<std::reference_wrapper<bool>> bool_members;
        std::vector<std::reference_wrapper<double>> double_members;
        std::vector<std::reference_wrapper<int>> int_members;
        std::vector<std::reference_wrapper<unsigned int>> uint_members;
        std::vector<std::reference_wrapper<std::string>> string_members;
        std::vector<std::reference_wrapper<Json_base>> json_members;
        void json_parse(std::istream &) override;
        void json_write(std::ostream &) override;
        std::vector<Json_descriptor> _json_descriptors;
    private:
        Json_descriptor & _json_descriptor(const std::string &);
        void _json_set_member_value(Json_descriptor &, std::istream &);

    };

    struct Json_object : Json_base{
        virtual void json_set_parser(Json_parser &) = 0;
        void json_parse(std::istream &) override;
        void json_write(std::ostream &) override;
    };

    template <class T>
    struct Json_parsable_vector : std::vector<T>, Json_base {
        static_assert(std::is_base_of<Json_object, T>::value, "Vector item type must derive from Json_object");
        void json_parse (std::istream &i) override {
            if (Json_util::skip_blanks(i) != '[') throw std::logic_error("format error");
            Json_util::discard(i);
            Json_parsable_vector<T> &o = *this;
            o.clear();
            while ((Json_util::skip_blanks(i) != ']')){
                T value;
                i >> value;
                o.push_back(value);
                if (Json_util::skip_blanks(i) != ',') break;
                Json_util::discard(i);
            }
            if (Json_util::skip_blanks(i, true) != ']') throw std::logic_error("format error");
        }
        void json_write(std::ostream &o) override {
            o << "[" ;
            bool first = true;
            for (auto &i:*this){
                if (!first) o << ",";
                first = false;
                o<<i;
            }
            o<<"]";
        }
    };

}