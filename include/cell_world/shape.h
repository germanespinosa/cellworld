#pragma once
#include <cell_world/core.h>

namespace cell_world{

    struct Transformation : json_cpp::Json_object{
        Transformation ();
        Transformation (double size, double rotation);
        double theta() const;
        double size{};
        double rotation{};
        Json_object_members({
                                Add_member(size);
                                Add_member(rotation);
                            });
    };

    struct Shape : json_cpp::Json_object{
        Shape ();
        Shape (int);
        int sides{};
        Json_object_members({
                                Add_member(sides);
                            })
    };


    struct Polygon : json_cpp::Json_object {
        Polygon ();
        Polygon (const Polygon &);
        Polygon (const Location &, unsigned int sides, double radius, double rotation);
        Polygon (const Location &,const Shape &, const Transformation &);
        Location center;
        Location_list vertices;
        double radius;
        Polygon move(const Location&) const;
        Polygon move(double, double) const;
        bool is_between (const Location &, const Location &) const;
        bool is_between (const Location &, double, double ) const;
        bool contains(const Location &) const;
        Json_object_members({
                                Add_member(vertices);
                                Add_member(center);
                                Add_member(radius);
                            })
        Polygon operator +=(const Location &);
        Polygon & operator = (const Polygon &);
    };

    struct Polygon_list : json_cpp::Json_vector<Polygon> {
        Polygon_list () = default;
        Polygon_list (const Location_list &centers, const Shape &, const Transformation &);
        bool contains(const Location &) const;
    };

    struct Space : json_cpp::Json_object{
        Space ();
        Space (const Location &, const Shape &, const Transformation &);
        Location center;
        Shape shape;
        Transformation transformation;
        Location transform(const Location &, const Space &) const;
        Json_object_members({
                                Add_member(center);
                                Add_member(shape);
                                Add_member(transformation);
                            })
    };
}