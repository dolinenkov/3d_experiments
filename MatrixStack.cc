#include "MatrixStack.hh"

MatrixStackSet::MatrixStackSet(const MatrixStackSetSettings & settings): needUpdateMatrixGroup(false)
{
    projectionMatrixStack.reserve(settings.projectionMatrixStackSize);
    viewMatrixStack.reserve(settings.viewMatrixStackSize);
    modelMatrixStack.reserve(settings.modelMatrixStackSize);
}

const MatrixStack & MatrixStackSet::getProjectionMatrixStack() const
{
    return projectionMatrixStack;
}

MatrixStack & MatrixStackSet::getProjectionMatrixStack()
{
    needUpdateMatrixGroup = true;
    return projectionMatrixStack;
}

const MatrixStack & MatrixStackSet::getViewMatrixStack() const
{
    return viewMatrixStack;
}

MatrixStack & MatrixStackSet::getViewMatrixStack()
{
    needUpdateMatrixGroup = true;
    return viewMatrixStack;
}

const MatrixStack & MatrixStackSet::getModelMatrixStack() const
{
    return modelMatrixStack;
}

MatrixStack & MatrixStackSet::getModelMatrixStack()
{
    needUpdateMatrixGroup = true;
    return modelMatrixStack;
}

const MatrixGroup & MatrixStackSet::getMatrixGroup()
{
    if (needUpdateMatrixGroup)
    {
        needUpdateMatrixGroup = false;

        if (projectionMatrixStack.empty())
            SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "cannot build matrix group: projection matrix stack is empty\n");

        if (viewMatrixStack.empty())
            SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "cannot build matrix group: view matrix stack is empty\n");

        if (modelMatrixStack.empty())
            SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "cannot build matrix group: model matrix stack is empty\n");

        if (!projectionMatrixStack.empty() && !viewMatrixStack.empty() && !modelMatrixStack.empty())
        {
            matrixGroup.modelMatrix = modelMatrixStack.top();
            matrixGroup.modelViewMatrix = viewMatrixStack.top() * matrixGroup.modelMatrix;
            matrixGroup.modelViewProjectionMatrix = projectionMatrixStack.top() * matrixGroup.modelViewMatrix;

            matrixGroup.normalMatrix = transpose(inverse(matrixGroup.modelMatrix));
        }
    }

    return matrixGroup;
}
