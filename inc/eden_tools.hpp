#ifndef EDEN_TOOLS_H_
#define EDEN_TOOLS_H_

#include "entity.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/implot.h"
#include "imgui/implot_internal.h"
#include <cstring>
#include <vector>

// utility structure for realtime plot
struct ScrollingBuffer {
  int MaxSize;
  int Offset;
  float maxFloat{0.0f};
  float minFloat{10000.0f};
  ImVector<ImVec2> Data;
  ScrollingBuffer(int max_size = 1000) {
    MaxSize = max_size;
    Offset = 0;
    Data.reserve(MaxSize);
  }

  inline void SetMaxFloat(float newFloat) { maxFloat = newFloat; }
  inline void SetMinFloat(float newFloat) { minFloat = newFloat; }

  void AddPoint(float x, float y) {
    if (Data.size() < MaxSize)
      Data.push_back(ImVec2(x, y));
    else {
      Data[Offset] = ImVec2(x, y);
      Offset = (Offset + 1) % MaxSize;
    }
  }
  void Erase() {
    if (Data.size() > 0) {
      Data.shrink(0);
      Offset = 0;
    }
  }
};

class EdenTools {
private:
  ImGuiIO &io;
  char prompt[300];
  char *promptBuffer[300];

public:
  // Constructor
  EdenTools(ImGuiIO &eden_io);

  // Methods
  // Getters
  inline ImGuiIO getEdenIo() { return this->io; }
  inline char *getPrompt() { return this->prompt; }

  // Setters

  // Render Tool Windows
  void RenderMenu(std::vector<Entity> *sceneObjects);
  void RenderFpsCounter();
  void RenderPromptInput(std::vector<Entity> *sceneObjects) const;
};

#endif // !EDEN_TOOLS_H_
