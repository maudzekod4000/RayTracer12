#ifndef LIGHTING_H
#define LIGHTING_H

#include <vector>

#include "utils/TypeDefs.h"
#include "sampling/Ray.h"
#include "optimisations/AABBTree.h"

struct Light {
  Vec3 pos;
  float intensity;
};

struct LightOptions {
  float shadowBias;
  float albedo;
};

struct Lighting {
  Lighting(LightOptions& opts, const std::vector<Light>& lights, AABBTree& tree):
    options(opts), lights(lights), tree(tree) {}

  /// <summary>
  /// Calculates the shadow color from an intersection point.
  /// </summary>
  /// <param name="hit">Data about the currently traced intersection.</param>
  /// <returns>Color for the shadow on the pixel currently sampled.</returns>
  inline NormalizedColor light(const IntersectionData& hit) {
    NormalizedColor lightContributionColor(0.0f);

    for (const Light& light : lights) {
      Vec3 lightDir = light.pos - hit.p;
      const float sphereRadius = glm::length(lightDir);
      lightDir = glm::normalize(lightDir);
      const Ray shadowRay(hit.p + (hit.pN * options.shadowBias), lightDir);
      const IntersectionData intrData = tree.intersectAABBTree(shadowRay);

      if (intrData.t >= sphereRadius) {
        const float cosLaw = calcCosineLaw(hit.pN, lightDir);
        const float color = light.intensity / calcSphereArea(sphereRadius) * options.albedo * cosLaw;
        lightContributionColor += NormalizedColor(color);
      }
    }

    return lightContributionColor;
  }

  inline float calcCosineLaw(const Vec3& hitNormal, const Vec3& lightDir) const {
    return glm::max(0.0f, glm::dot(lightDir, hitNormal));
  }

  inline float calcSphereArea(float radius) const {
    return 4.0f * glm::pi<float>() * radius * radius;
  }
private:
  LightOptions options;
  std::vector<Light> lights;
  AABBTree& tree;
};


#endif // !LIGHTING_H
