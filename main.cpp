#include "color.h"
#include <iostream>
#include "ray.h"
#include "vec3.h"

bool hit_sphere(const point3& center,double radius ,const ray& r){
    vec3 oc =center -r.origin();
    auto a = dot(r.direction(),r.direction());
    auto b =-2.*dot(r.direction(),oc);
    auto c =dot(oc,oc)-radius*radius;
    auto discriminant =b*b -4*a*c;
    return (discriminant >=0);
}

color ray_color(const ray& r){
    if (hit_sphere(point3(0,0,-1), 0.5, r)) return {1.,0,0};
    vec3 unit_direction = unit_vector(r.direction());
    auto a = 0.5 * (unit_direction.y()+1.);
    return (1.-a)*color (1.,1.,1.)+a*color(.5,.7,1.);
}

int main (){
    auto aspect_ratio = 16.0/9.0;
    int imageWidth =400;

    int imageHeight = int(imageWidth/aspect_ratio);
    imageHeight = (imageHeight <1 )? 1 : imageHeight;

    auto focalLength = 1.0;
    auto viewportHeight =2.0;
    auto viewportWidth = viewportHeight * (double(imageWidth)/imageHeight);
    auto camera_center = point3{0,0,0};

    auto viewport_u = vec3{viewportWidth,0,0};
    auto viewport_v = vec3{0,-viewportHeight,0};

    auto pixel_delta_u = viewport_u/imageWidth;
    auto pixel_delta_v =viewport_v/imageHeight;

    auto viewport_upper_left = camera_center - vec3{0,0,focalLength} - viewport_u/2 - viewport_v/2;
    auto pixel00_loc =viewport_upper_left + 0.5 * (pixel_delta_u+pixel_delta_v);
    
    std::cout << "P3\n" << imageWidth << ' '<< imageHeight << "\n255\n";

    for (int j= 0;j <= imageHeight; ++j){
        std::clog << "\rScanlines remaining: " <<  (imageHeight -j) << ' ' << std::flush;
        for (int i=0; i < imageWidth; ++i) {
            auto pixel_center = pixel00_loc+ (i * pixel_delta_u) + (j * pixel_delta_v);
            auto ray_direction = pixel_center -camera_center;
            ray r (camera_center , ray_direction);
            
            color pixel_color = ray_color(r);
            
            write_color(std::cout, pixel_color);
        }
    }
    std::clog << "\rDone.           \n";
}