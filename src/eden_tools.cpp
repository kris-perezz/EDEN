#include "eden_tools.hpp"
#include "imgui/imgui.h"
#include "imgui/implot.h"
#include <cstdlib>
#include <cstring>

// Constructor
EdenTools::EdenTools(ImGuiIO &eden_io) : io(eden_io) {};

void EdenTools::RenderFpsCounter() {

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
  ImGui::Text("Avg. FPS: %.2f", io.Framerate);
  if (ImPlot::BeginPlot("##Scrolling", ImVec2(-1, 150))) {
    ImPlot::SetupAxes(nullptr, nullptr, ImPlotAxisFlags_NoTickLabels,
                      ImPlotAxisFlags_AutoFit);
    ImPlot::SetupAxisLimits(ImAxis_X1, t - history, t, ImGuiCond_Always);
    ImPlot::SetupAxisLimits(
        ImAxis_Y1, frameTimeBuffer.minFloat - (frameTimeBuffer.minFloat * 0.33),
        frameTimeBuffer.maxFloat + (frameTimeBuffer.maxFloat * 0.33),
        ImGuiCond_Always);
    ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, 0.5f);
    ImPlot::PlotLine("Frametime (ms)", &frameTimeBuffer.Data[0].x,
                     &frameTimeBuffer.Data[0].y, frameTimeBuffer.Data.size(), 0,
                     frameTimeBuffer.Offset, 2 * sizeof(float));

    ImGui::Text("Max: %.2f (ms)", frameTimeBuffer.maxFloat);
    ImGui::Text("Min: %.2f (ms)", frameTimeBuffer.minFloat);

    ImPlot::EndPlot();
  }
}

void EdenTools::RenderPromptInput() const {
  ImGui::Text("Enter a scene you would like to create");
  ImGui::InputTextMultiline("##edenprompt", *this->promptBuffer, 300);
  if (ImGui::Button("Create Scene Data")) {
    char buffer[409];
    const char command[] =
        "python ~/Documents/code/cpp/EDEN/api.py"; // PYTHON SCRIPT HERE IS
                                                   // HARDCODED
    sprintf(buffer, "%s '%s'", command, *this->promptBuffer);
    system(buffer);
  }
}

void EdenTools::RenderMenu() {
  if (ImGui::Begin("EDEN")) {
    // Tabs
    if (ImGui::BeginTabBar("Tools")) {
      if (ImGui::BeginTabItem("Prompt")) {
        this->RenderPromptInput();
        ImGui::EndTabItem();
      }

      ImGui::EndTabBar();
    }

    ImGui::Separator();

    if (ImGui::BeginTabBar("Diagnostics")) {

      if (ImGui::BeginTabItem("FPS")) {
        this->RenderFpsCounter();
        ImGui::EndTabItem();
      }

      ImGui::EndTabBar();
    }

    ImGui::End();
  };
}
