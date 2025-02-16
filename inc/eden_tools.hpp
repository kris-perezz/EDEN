#ifndef EDEN_TOOLS_H_
#define EDEN_TOOLS_H_

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

struct FrameRateBuffer {
  int avgFps{0};
  int capacity{60};
  int currentSize{0};
  float frames[60];

  inline int getAvgFps() { return avgFps; }

  inline void AddFrame(float frame) {
    frames[currentSize] = frame;
    currentSize++;
  }

  inline bool CanAddFrame() { return currentSize + 1 < capacity; }

  // Clears buffer and returns the average of the buffer before it was cleared
  void Flush() {
    float avg = 0.0f;
    // Get avg of frames
    for (int i = 0; i < capacity; i++) {
      avg += frames[i];
      frames[i] = 0; // "Flushes" the current space
    }

    currentSize = 0; // Reset buffer
    avgFps = (int)(avg / capacity);
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
  void RenderFpsCounter();
  void RenderPromptInput() const;
};

#endif // !EDEN_TOOLS_H_
