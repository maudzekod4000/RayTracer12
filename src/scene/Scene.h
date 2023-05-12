#ifndef SCENE_H
#define SCENE_H

#include <cstdint>
#include <string_view>
#include <fstream>
#include <vector>

#include "rapidjson/istreamwrapper.h"
#include "rapidjson/document.h"

#include "utils/TypeDefs.h"

struct ImageSettings {
  int32_t width;
  int32_t heigth;
};

struct Settings {
  Vec3 backgroundColor;
  ImageSettings imageSettings;
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
  std::vector<Triangle> triangles;
  std::vector<Light> lights;
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

      const Value& objectsVal = doc.FindMember("objects")->value;
      if (!objectsVal.IsNull() && objectsVal.IsArray()) {
        GenericArray objectsArr = objectsVal.GetArray();
        for (SizeType i = 0; i < objectsArr.Size(); i++) {
          const Value& verticesVal = objectsArr[i].FindMember("vertices")->value;
          if (!verticesVal.IsNull() && verticesVal.IsArray()) {
            GenericArray verticesArr = verticesVal.GetArray();

            const Value& trianglesVal = objectsArr[i].FindMember("triangles")->value;
            if (!trianglesVal.IsNull() && trianglesVal.IsArray()) {
              GenericArray trianglesArr = trianglesVal.GetArray();
              int vIndex = 0;
              int vArr[3];

              for (SizeType i = 0; i < trianglesArr.Size(); i++) {
                vArr[vIndex] = trianglesArr[i].GetInt();
                vIndex++;

                if (vIndex == 3) {
                  vIndex = 0;
                  std::vector<Vec3> vertices;
                  for (SizeType i = 0; i < 3; i++) {
                    int index = vArr[i] * 3;
                    float x = verticesArr[index].GetFloat();
                    float y = verticesArr[index + 1].GetFloat();
                    float z = verticesArr[index + 2].GetFloat();
                    vertices.push_back(Vec3(x, y, z));
                  }

                  InternalColor col = InternalColor{};
                  Triangle t = Triangle(vertices[0], vertices[1], vertices[2], col);
                  
                  triangles.push_back(t);
                }
              }
            }
          }
        }
      }
    }
  }
};



#endif // !SCENE_H
