#include "eden_tools.hpp"
#include "imgui/imgui.h"
#include "imgui/implot.h"
#include <algorithm>
#include <deque>
#include <iostream>
#include <vector>
//
// Constructor
EdenTools::EdenTools(ImGuiIO &eden_io) : io(eden_io) {
  frameTimes.reserve(100);
  std::fill(frameTimes.begin(), frameTimes.begin() + 99, 0);
};

void EdenTools::RenderFpsCounter(std::vector<float> &frameTimes,
                                 int maxFrames) {

  float frameTime = 1000.0f / io.Framerate;
  frameTimes.push_back(frameTime);

  if (frameTimes.size() > maxFrames) {
    frameTimes.erase(frameTimes.begin());
  }

  // FPS Counter
  ImGui::Begin("EDEN: FPS");
  if (ImPlot::BeginPlot("Performance")) {
    // Setup

    ImPlot::SetupAxis(ImAxis_X1, "Time (ms)");
    ImPlot::SetupAxisLimits(ImAxis_X1, 0, maxFrames);
    ImPlot::SetupAxis(ImAxis_Y1, "Avg Frame Time (ms)",
                      ImPlotAxisFlags_AutoFit);
    ImPlot::SetupAxisLimits(ImAxis_Y1, 0, 1000);

    ImPlot::SetNextMarkerStyle(ImPlotMarker_Circle);

    ImPlot::PlotLine("Avg Frame Time (ms)", frameTimes.data(),
                     frameTimes.size(), 10);

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
