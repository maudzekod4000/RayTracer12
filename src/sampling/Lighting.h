#ifndef LIGHTING_H
#define LIGHTING_H

#include <vector>

#include <utils/TypeDefs.h>
#include "sampling/Ray.h"

struct Light {
  Vec3 pos;
  float intensity;
};

struct LightOptions {
  float shadowBias;
  float albedo;
};


struct Lighting {
  inline Lighting(LightOptions& opts, const std::vector<Light>& lights, const std::vector<Object>& objects) :
    options(opts), lights(lights), objects(objects) {}

  inline InternalColor light(const IntersectionData& intersectionData) {
    InternalColor lightContributionColor{};

    for (const Light& light : lights) {
      Vec3 lightDir = light.pos - intersectionData.p;
      float lightSphereRadius = glm::length(lightDir);
      lightDir = glm::normalize(lightDir);
      float cosineLaw = glm::max(0.0f, glm::dot(intersectionData.pN, lightDir));
      float lightSphereArea = 4 * M_PI * lightSphereRadius * lightSphereRadius;
      Ray shadowRay{ intersectionData.p + (intersectionData.pN * options.shadowBias), lightDir };
      bool intersects = false;
      IntersectionData intrData{};

      for (Object& obj : objects) {
        for (const auto& triangle : obj.triangles) {
          if (triangle.intersect(shadowRay, intrData)) {
            if (intrData.t < lightSphereRadius) {
              intersects = true;
              break;
            }
          }
        }
      }

      if (!intersects) {
        float color = light.intensity / lightSphereArea * options.albedo * cosineLaw;
        lightContributionColor += InternalColor(color);
      }
    }

    return lightContributionColor;
  }

  LightOptions options;
  std::vector<Light> lights;
  std::vector<Object> objects;
};


#endif // !LIGHTING_H
