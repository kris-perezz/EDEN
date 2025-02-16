#include "eden_tools.hpp"
#include "imgui/imgui.h"
#include "imgui/implot.h"
#include <algorithm>
#include <deque>
#include <iostream>
#include <vector>
//
// Constructor
EdenTools::EdenTools(ImGuiIO &eden_io) : io(eden_io) {};

void EdenTools::RenderFpsCounter() {

  /**
   * Framerate
   */
  static FrameRateBuffer frameRateBuffer;

  if (!frameRateBuffer.CanAddFrame()) {
    frameRateBuffer.Flush();
  } else {
    frameRateBuffer.AddFrame(io.Framerate);
  }

  /**
   * Frametimes
   */
  static ScrollingBuffer frameTimeBuffer;

  ImVec2 mouse = ImGui::GetMousePos();
  float frameTime = 1000.0f / io.Framerate;
  static float t = 0;

  t += ImGui::GetIO().DeltaTime;
  frameTimeBuffer.AddPoint(t, frameTime);

  // Set "padding"
  if (frameTime > frameTimeBuffer.maxFloat) {
    frameTimeBuffer.SetMaxFloat(frameTime);
  }

  if (frameTime < frameTimeBuffer.minFloat) {
    frameTimeBuffer.SetMinFloat(frameTime);
  }

  static float history = 5.0f;

  // FPS Counter
  ImGui::Begin("EDEN: FPS");
  ImGui::Text("Avg. FPS: %d", frameRateBuffer.getAvgFps());
  if (ImPlot::BeginPlot("##Scrolling", ImVec2(-1, 150))) {
    ImPlot::SetupAxes(nullptr, nullptr, ImPlotAxisFlags_NoTickLabels,
                      ImPlotAxisFlags_AutoFit);
    ImPlot::SetupAxisLimits(ImAxis_X1, t - history, t, ImGuiCond_Always);
    ImPlot::SetupAxisLimits(
        ImAxis_Y1, frameTimeBuffer.minFloat - (frameTimeBuffer.minFloat * 0.33),
        frameTimeBuffer.maxFloat + (frameTimeBuffer.maxFloat * 0.33),
        ImGuiCond_Always);
    ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, 0.5f);
    ImPlot::PlotLine("Avg. Frame Time (ms)", &frameTimeBuffer.Data[0].x,
                     &frameTimeBuffer.Data[0].y, frameTimeBuffer.Data.size(), 0,
                     frameTimeBuffer.Offset, 2 * sizeof(float));
    ImPlot::EndPlot();
  }
  ImGui::End();
}

void EdenTools::RenderPromptInput() const {
  ImGui::Begin("EDEN: PROMPT");
  ImGui::InputTextMultiline("##edenprompt", *this->promptBuffer,
                            sizeof(this->promptBuffer));
  ImGui::End();
}
