#include <Application/Camera.h>
#include <Simulation/Preset/SimulationPresetEditor.h>
#include <Simulation/Preset/SimulationPresetManager.h>
#include <Utils/Utils.h>
#include <imgui.h>
#include <imgui_stdlib.h>

SimulationPresetEditor& SimulationPresetEditor::Get()
{
    static SimulationPresetEditor presetEditor;
    return presetEditor;
}

bool SimulationPresetEditor::OnImGuiUpdate()
{
    SimulationPresetManager& presetManager = SimulationPresetManager::Get();
    bool isNameValid = !presetManager.IsNameTaken(m_preset.m_name) ||
        !presetManager.GetPreset(m_preset.m_name).m_isBuiltIn;

    if (!isNameValid)
    {
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
    }
    ImGui::InputText("Preset Name", &m_preset.m_name);
    if (!isNameValid)
    {
        ImGui::PopStyleColor();
    }

    ImGui::PushItemWidth(200.0f);
    if (ImGui::BeginCombo("Entities", m_selectedEntityLabel.c_str()))
    {
        if (ImGui::Selectable("Create New Entity"))
        {
            CreateNewEntity();
        }

        for (size_t index = 0; index < m_preset.m_entities.size(); ++index)
        {
            std::string label = CreateEntityLabel(index);
            if (ImGui::Selectable(label.c_str()))
            {
                SelectEntity(index);
            }
        }
        ImGui::EndCombo();
    }
    ImGui::PopItemWidth();

    if (m_isEntitySelected &&
        ImGui::CollapsingHeader(m_selectedEntityLabel.c_str()))
    {
        ImGui::Indent(16.0f);
        CircEntity& selectedEntity = m_preset.m_entities[m_selectedEntityIndex];
        Uni::Math::Vector2f position = selectedEntity.GetPosition();
        Uni::Math::Vector2f velocity = selectedEntity.GetVelocity();
        float radius = selectedEntity.GetRadius();

        ImGui::SliderFloat(
            "Radius", &radius, 1.0f, CircEntity::MaxEntityRadius);

        float imguiPosition[2] = { position.m_x, position.m_y };
        ImGui::InputFloat2("Position", imguiPosition);

        float imguiVelocity[2] = { velocity.m_x, velocity.m_y };
        ImGui::InputFloat2("Velocity", imguiVelocity);

        if (ImGui::Button("Remove"))
        {
            m_preset.m_entities.erase(
                m_preset.m_entities.begin() +
                static_cast<long>(m_selectedEntityIndex));

            ClearSelection();
        }

        ImGui::Unindent(16.0f);

        selectedEntity = CircEntity{
            { imguiPosition[0], imguiPosition[1] },
            { imguiVelocity[0], imguiVelocity[1] },
            radius,
        };

        UpdateSelectedShape();
    }

    return isNameValid;
}

void SimulationPresetEditor::Draw(sf::RenderWindow& window)
{
    for (const CircEntity& entity : m_preset.m_entities)
    {
        entity.Draw(window);
        if (m_isEntitySelected)
        {
            window.draw(
                m_selectedLines.data(), m_selectedLines.size(), sf::Lines);
        }
    }
}

void SimulationPresetEditor::LoadPreset(const SimulationPreset& preset)
{
    m_preset = preset;
    m_preset.m_isBuiltIn = false;
    ClearSelection();
}

const SimulationPreset& SimulationPresetEditor::GetPreset() const
{
    return m_preset;
}

std::string SimulationPresetEditor::CreateEntityLabel(size_t index)
{
    return std::string("Entity") + std::to_string(index);
}

void SimulationPresetEditor::CreateNewEntity()
{
    m_preset.m_entities.emplace_back();
    SelectEntity(m_preset.m_entities.size() - 1LU);
}

void SimulationPresetEditor::SelectEntity(size_t index)
{
    m_selectedEntityIndex = index;
    m_selectedEntityLabel = CreateEntityLabel(m_selectedEntityIndex);
    m_isEntitySelected = true;
    Camera::Get().SetPosition(
        m_preset.m_entities[m_selectedEntityIndex].GetPosition());
    UpdateSelectedShape();
}

void SimulationPresetEditor::ClearSelection()
{
    m_selectedEntityIndex = 0LU;
    m_selectedEntityLabel = "";
    m_isEntitySelected = false;
}

void SimulationPresetEditor::UpdateSelectedShape()
{
    static const sf::Color Color = { 0, 255, 0, 255 };

    CircEntity& selectedEntity = m_preset.m_entities[m_selectedEntityIndex];
    const Uni::Math::Vector2f center = selectedEntity.GetPosition();
    const float InnerRadius = selectedEntity.GetRadius() * 1.1f;
    const float OuterRadius = selectedEntity.GetRadius() * 1.2f;

    m_selectedLines[0] = {
        Utils::CreateSfVectorFromUniVector(
            center +
            Uni::Math::Vector2f{
                0.0f,
                -InnerRadius,
            }),
        Color,
    };

    m_selectedLines[1] = {
        Utils::CreateSfVectorFromUniVector(
            center +
            Uni::Math::Vector2f{
                0.0f,
                -OuterRadius,
            }),
        Color,
    };

    m_selectedLines[2] = {
        Utils::CreateSfVectorFromUniVector(
            center +
            Uni::Math::Vector2f{
                0.0f,
                InnerRadius,
            }),
        Color,
    };

    m_selectedLines[3] = {
        Utils::CreateSfVectorFromUniVector(
            center +
            Uni::Math::Vector2f{
                0.0f,
                OuterRadius,
            }),
        Color,
    };

    m_selectedLines[4] = {
        Utils::CreateSfVectorFromUniVector(
            center +
            Uni::Math::Vector2f{
                -InnerRadius,
                0.0f,
            }),
        Color,
    };

    m_selectedLines[5] = {
        Utils::CreateSfVectorFromUniVector(
            center +
            Uni::Math::Vector2f{
                -OuterRadius,
                0.0f,
            }),
        Color,
    };

    m_selectedLines[6] = {
        Utils::CreateSfVectorFromUniVector(
            center +
            Uni::Math::Vector2f{
                InnerRadius,
                0.0f,
            }),
        Color,
    };

    m_selectedLines[7] = {
        Utils::CreateSfVectorFromUniVector(
            center +
            Uni::Math::Vector2f{
                OuterRadius,
                0.0f,
            }),
        Color,
    };
}
