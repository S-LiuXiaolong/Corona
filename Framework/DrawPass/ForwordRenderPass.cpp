#include "ForwardRenderPass.h"
#include "GraphicsManager.h"
#include "IShaderManager.h"

using namespace Corona;
using namespace std;

void ForwardRenderPass::Draw(Frame& frame)
{
    auto shaderProgram = g_pShaderManager->GetDefaultShaderProgram(DefaultShaderIndex::Forward);

    // Set the color shader as the current shader program and set the matrices that it will use for rendering.
    g_pGraphicsManager->UseShaderProgram(shaderProgram);
    g_pGraphicsManager->SetPerFrameConstants(frame.frameContext);

    g_pGraphicsManager->SetShadowMap(frame.shadowMap);

    for (auto dbc : frame.batchContexts)
    {
        g_pGraphicsManager->DrawBatch(*dbc);
    }
}

