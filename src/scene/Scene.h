#ifndef SCENE_H
#define SCENE_H

#include <cstdint>
#include <string_view>
#include <fstream>
#include <vector>
#include <unordered_map>

#include "rapidjson/istreamwrapper.h"
#include "rapidjson/document.h"

#include "utils/TypeDefs.h"
#include "sampling/Object.h"
#include "sampling/Material.h"
#include "sampling/Triangle.h"
#include "sampling/Lighting.h"

struct ImageSettings {
  int32_t width;
  int32_t heigth;
};

struct Settings {
  Vec3 backgroundColor;
  ImageSettings imageSettings;
};

struct TempTriangle {
  Vec3 a;
  Vec3 b;
  Vec3 c;
  int vertices[3];

  inline Vec3 normal() const {
    return glm::normalize(glm::cross(b - a, c - a));
  }
};

struct Scene {
  Scene(std::string_view path) {
    parseSceneFile(path);
  }

  struct Camera {
    glm::mat3 matrix;
    Vec3 position;
  };

  Settings settings;
  Camera camera;
  std::vector<Object> objects;
  std::vector<Light> lights;
  std::vector<Material> materials;
  int triangleCount = 0;
private:
  void parseSceneFile(std::string_view sceneFileName) {
    std::ifstream ifs(sceneFileName.data());

    using namespace rapidjson;

    IStreamWrapper isw(ifs);
    Document doc;
    doc.ParseStream(isw);

    if (!doc.HasParseError() && doc.IsObject()) {
      const Value& settingsVal = doc.FindMember("settings")->value;

      if (!settingsVal.IsNull() && settingsVal.IsObject()) {
        const Value& imageSettingsVal = settingsVal.FindMember("image_settings")->value;
        const Value& imageWidthVal = imageSettingsVal.FindMember("width")->value;
        if (!imageWidthVal.IsNull() && imageWidthVal.IsInt()) {
          settings.imageSettings.width = imageWidthVal.GetInt();
        }
        const Value& imageHeightVal = imageSettingsVal.FindMember("height")->value;
        if (!imageHeightVal.IsNull() && imageHeightVal.IsInt()) {
          settings.imageSettings.heigth = imageHeightVal.GetInt();
        }

        const Value& bgColorVal = settingsVal.FindMember("background_color")->value;
        if (!bgColorVal.IsNull() && bgColorVal.IsArray()) {
          GenericArray bgColorArr = bgColorVal.GetArray();
          if (bgColorArr.Size() == 3) {
            for (SizeType i = 0; i < bgColorArr.Size(); i++) {
              settings.backgroundColor[i] = bgColorArr[i].GetFloat();
            }
          }
        }
      }

      const Value& cameraVal = doc.FindMember("camera")->value;
      if (!cameraVal.IsNull() && cameraVal.IsObject()) {
        const Value& positionVal = cameraVal.FindMember("position")->value;
        if (!positionVal.IsNull() && positionVal.IsArray()) {
          GenericArray positionArr = positionVal.GetArray();
          if (positionArr.Size() == 3) {
            for (SizeType i = 0; i < positionArr.Size(); i++) {
              camera.position[i] = positionArr[i].GetFloat();
            }
          }
        }

        const Value& matrixVal = cameraVal.FindMember("matrix")->value;
        if (!matrixVal.IsNull() && matrixVal.IsArray()) {
          GenericArray matrixArr = matrixVal.GetArray();
          if (matrixArr.Size() == 9) {
            for (SizeType i = 0; i < 3; i++) {
              Vec3 v = Vec3(matrixArr[i].GetFloat(), matrixArr[i + 3].GetFloat(), matrixArr[i + 6].GetFloat());
              camera.matrix[i] = v;
            }

            camera.matrix = glm::transpose(camera.matrix);
          }
        }
      }

      const Value& lightsVal = doc.FindMember("lights")->value;
      if (!lightsVal.IsNull() && lightsVal.IsArray()) {
        GenericArray lightsArr = lightsVal.GetArray();

        for (SizeType i = 0; i < lightsArr.Size(); i++) {
          Light light;

          const auto& lightVal = lightsArr[i].GetObject();
          const Value& intensityVal = lightVal.FindMember("intensity")->value;
          light.intensity = intensityVal.GetFloat();

          const Value& positionVal = lightVal.FindMember("position")->value;
          if (!positionVal.IsNull() && positionVal.IsArray()) {
            GenericArray positionArr = positionVal.GetArray();
            if (positionArr.Size() == 3) {
              light.pos = Vec3(positionArr[0].GetFloat(), positionArr[1].GetFloat(), positionArr[2].GetFloat());
            }
          }

          lights.push_back(light);
        }
      }

      const Value& materialsVal = doc.FindMember("materials")->value;
      if (!materialsVal.IsNull() && materialsVal.IsArray()) {
        GenericArray materialsArr = materialsVal.GetArray();

        for (SizeType i = 0; i < materialsArr.Size(); i++) {
          Material mat;

          const auto& materialVal = materialsArr[i].GetObject();
          const Value& typeVal = materialVal.FindMember("type")->value;
          std::string type = typeVal.GetString();

          if (type == "reflective") {
            mat.type = MaterialType::REFLECTIVE;
          }
          else if (type == "diffuse") {
            mat.type = MaterialType::DIFFUSE;
          }
          else if (type == "refractive") {
            mat.type = MaterialType::REFRACTIVE;
          }
          else {
            mat.type = MaterialType::NONE;
          }

          const Value& albedoVal = materialVal.FindMember("albedo")->value;

          if (albedoVal.IsArray()) {
            GenericArray albedoArr = albedoVal.GetArray();
            Vec3 albedo{ albedoArr[0].GetFloat(), albedoArr[1].GetFloat(), albedoArr[2].GetFloat() };
            mat.albedo = albedo;
          }

          const Value& smoothShadingVal = materialVal.FindMember("smooth_shading")->value;
          mat.smoothShading = smoothShadingVal.GetBool();

          materials.push_back(mat);
        }
      }

      const Value& objectsVal = doc.FindMember("objects")->value;
      if (!objectsVal.IsNull() && objectsVal.IsArray()) {
        GenericArray objectsArr = objectsVal.GetArray();
        for (SizeType i = 0; i < objectsArr.Size(); i++) {
          const Value& verticesVal = objectsArr[i].FindMember("vertices")->value;
          std::vector<Vertex> vertices;
          std::unordered_map<int, std::vector<TempTriangle*>> vertexIndxToTriangles;
          std::vector<TempTriangle*> tempTriangles;
          Object currentObject;

          const Value& materialIdxVal = objectsArr[i].FindMember("material_index")->value;

          if (!materialIdxVal.IsNull() && materialIdxVal.IsInt() && materials.size() > 0) {
            int materialIdx = materialIdxVal.GetInt();
            currentObject.mat = materials[materialIdx];
          }
          if (!verticesVal.IsNull() && verticesVal.IsArray()) {
            GenericArray verticesArr = verticesVal.GetArray();

            for (SizeType i = 0; i < verticesArr.Size(); i+=3) {
              Vec3 pos(verticesArr[i].GetFloat(), verticesArr[i + 1].GetFloat(), verticesArr[i + 2].GetFloat());
              Vec3 smoothNormal{};
              Vertex v{ pos, smoothNormal };
              vertices.push_back(v);
            }

            const Value& trianglesVal = objectsArr[i].FindMember("triangles")->value;
            if (!trianglesVal.IsNull() && trianglesVal.IsArray()) {
              GenericArray trianglesArr = trianglesVal.GetArray();
              int vIndex = 0;
              int vArr[3];

              for (SizeType i = 0; i < trianglesArr.Size(); i++) {
                vArr[vIndex] = trianglesArr[i].GetInt();
                vIndex++;
                TempTriangle* tempTri = new TempTriangle;

                if (vIndex == 3) {
                  vIndex = 0;
                  std::vector<Vec3> vertices;
                  for (SizeType i = 0; i < 3; i++) {
                    int index = vArr[i] * 3;
                    tempTri->vertices[i] = vArr[i];
                    float x = verticesArr[index].GetFloat();
                    float y = verticesArr[index + 1].GetFloat();
                    float z = verticesArr[index + 2].GetFloat();
                    vertices.push_back(Vec3(x, y, z));
                  }

                  int indexInVerticesArr = trianglesArr[i].GetInt();
                  
                  tempTri->a = vertices[0];
                  tempTri->b = vertices[1];
                  tempTri->c = vertices[2];
                  tempTriangles.push_back(tempTri);
                  triangleCount++;

                  vertexIndxToTriangles[vArr[0]].push_back(tempTri);
                  vertexIndxToTriangles[vArr[1]].push_back(tempTri);
                  vertexIndxToTriangles[vArr[2]].push_back(tempTri);
                }
              }
            }
          }

          // Here we should calculate the smooth normals and create the final triangles
          for (const auto& [vertexId, triangles] : vertexIndxToTriangles) {
            Vertex& vert = vertices[vertexId];
            Vec3 smoothNormal{};

            for (TempTriangle* tri : triangles) {
              Vec3 n = tri->normal();
              smoothNormal += n;
            }

            vert.smoothNormal = glm::normalize(smoothNormal);
          }

          // Create the final triangle collection and delete all other data.
          for (TempTriangle* tri : tempTriangles) {
            Triangle renderingTriangle(vertices[tri->vertices[0]], vertices[tri->vertices[1]], vertices[tri->vertices[2]]);
            renderingTriangle.objIdx = objects.size();
            currentObject.triangles.push_back(renderingTriangle);
          }

          if (currentObject.mat.type == MaterialType::NONE) {
            // Set a random color if the object has no material
            currentObject.mat.albedo = NormalizedColor{ rand() / static_cast<float>(RAND_MAX), rand() / static_cast<float>(RAND_MAX), rand() / static_cast<float>(RAND_MAX) };
          }

          objects.push_back(currentObject);

          for (TempTriangle* tempTri : tempTriangles) {
            delete tempTri;
          }
          tempTriangles.clear();
          vertices.clear();
          vertexIndxToTriangles.clear();
        }
      }
    }
  }
};



#endif // !SCENE_H
