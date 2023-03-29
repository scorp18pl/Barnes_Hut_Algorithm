#include <Application/Application.h>
#include <Simulation/Preset/SimulationPresetGenerator.h>

int main()
{
    SimulationPresetGenerator::GenerateBuiltInSimulations();
    Application::GetApplication().Start();
    return 0;
}