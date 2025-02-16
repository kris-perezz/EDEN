#ifndef EDEN_TOOLS_H_
#define EDEN_TOOLS_H_

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/implot.h"
#include "imgui/implot_internal.h"
#include <cstring>
#include <deque>
#include <queue>
#include <vector>

class EdenTools {
private:
  ImGuiIO &io;
  char prompt[300];
  char *promptBuffer[300];

  // Frametime Attributes
  std::vector<float> frameTimes;
  int maxFrames = 500;

public:
  // Constructor
  EdenTools(ImGuiIO &eden_io);

  // Methods
  // Getters
  inline ImGuiIO getEdenIo() { return this->io; }
  inline char *getPrompt() { return this->prompt; }
  inline std::vector<float> &getFrameTimes() { return this->frameTimes; }
  inline int getMaxFrames() { return this->maxFrames; }

  // Setters
  inline void setPrompt() { strncpy(prompt, *promptBuffer, 300); }
  inline void setFrameTimes() {
    /*for (int i = 0; i < 30; i++) {*/
    /*  this->frameTimes.push_back(0);*/
    /*}*/
  }

  // Render Tool Windows
  void RenderFpsCounter(std::vector<float> &frameTimes, int maxFrames);
  void RenderPromptInput() const;
};

#endif // !EDEN_TOOLS_H_
