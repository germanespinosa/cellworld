#pragma once
#include <cell_world/transformation.h>
#include <cell_world/location.h>

namespace cell_world{

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
        Polygon (const Location &, unsigned int sides, float radius, float rotation);
        Polygon (const Location &,const Shape &, const Transformation &);
        Location center;
        Location_list vertices;
        float radius;
        Polygon move(const Location &) const;
        Polygon move(float, float) const;
        bool is_between (const Location &, const Location &) const;
        bool is_between (const Location &, float, float ) const;
        bool contains(const Location &) const;
        bool contains(const Polygon &) const;
        bool overlaps(const Polygon &) const;
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
        Polygon_list (const Location_list &, const Shape &, const Transformation &);
        bool contains(const Location &) const;
    };

    using Scale = Location;

    struct Space : json_cpp::Json_object{
        enum Orientation{
            Flipped = -1,
            Not_flipped = 1
        };
        Space ();
        Space (const Location &, const Shape &, const Transformation &);
        Location center;
        Shape shape;
        Transformation transformation;
        Location transform(const Location &, const Space &) const;
        Location scale(const Location &location, const Scale &scale);
        Json_object_members({
                                Add_member(center);
                                Add_member(shape);
                                Add_member(transformation);
                            })
    };
}