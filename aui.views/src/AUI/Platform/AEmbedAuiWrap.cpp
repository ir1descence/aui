/**
 * =====================================================================================================================
 * Copyright (c) 2021 Alex2772
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 
 * Original code located at https://github.com/aui-framework/aui
 * =====================================================================================================================
 */

//
// Created by alex2 on 6/6/2021.
//


#include "AEmbedAuiWrap.h"
#include "ABaseWindow.h"
#include <glm/ext/matrix_clip_space.hpp>
#include <AUI/GL/State.h>
#include <AUI/Platform/AWindow.h>

class AEmbedAuiWrap::FakeWindow: public ABaseWindow {
    friend class AEmbedAuiWrap;
public:
    FakeWindow() {
        currentWindowStorage() = this;
        checkForStencilBits();
    }
};

AEmbedAuiWrap::AEmbedAuiWrap()
{
    auto r = glewInit();
    assert(r == 0);
    mContainer = _new<FakeWindow>();
    mContainer->setPosition({ 0, 0 });
}

void AEmbedAuiWrap::setContainer(const _<AViewContainer>& container) {
    mContainer->setContents(container);
    mContainer->makeCurrent();
}

void AEmbedAuiWrap::setViewportSize(int width, int height) {
    mContainer->makeCurrent();
    mSize = { width, height };
    mContainer->setSize(width, height);

    resetGLState();
}

void AEmbedAuiWrap::render() {
    mContainer->makeCurrent();
    AThread::current()->processMessages();

    glEnable(GL_STENCIL_TEST);
    glDisable(GL_DEPTH_TEST);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
    GL::State::activeTexture(0);
    GL::State::useProgram(0);
    GL::State::bindVertexArray(0);
    GL::State::bindTexture(GL_TEXTURE_2D, 0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Render::inst().setColorForced(1.f);
    Render::inst().setTransformForced(glm::ortho(0.f,
                                                 static_cast<float>(mSize.x),
                                                 static_cast<float>(mSize.y),
                                                 0.f));
    mContainer->render();

    resetGLState();
}

void AEmbedAuiWrap::resetGLState() {
    glBindTexture(GL_TEXTURE_2D, 0);
    if (glBindVertexArray) {
        glBindVertexArray(0);
    }
}

void AEmbedAuiWrap::onMousePressed(int x, int y, AInput::Key button) {
    mContainer->makeCurrent();
    AThread::current()->processMessages();
    mContainer->onMousePressed(glm::ivec2{ x, y }, button);
}

void AEmbedAuiWrap::onMouseReleased(int x, int y, AInput::Key button) {
    mContainer->makeCurrent();
    mContainer->onMouseReleased(glm::ivec2{ x, y }, button);
}

bool AEmbedAuiWrap::isUIConsumesMouseAt(int x, int y) {
    return mContainer->consumesClick(glm::ivec2{ x, y });
}

void AEmbedAuiWrap::onMouseMove(int x, int y) {
    mContainer->makeCurrent();
    mContainer->onMouseMove(glm::ivec2{ x, y });
}


void AEmbedAuiWrap::onCharEntered(wchar_t c) {
    mContainer->makeCurrent();
    mContainer->onCharEntered(c);
}

void AEmbedAuiWrap::onKeyPressed(AInput::Key key) {
    mContainer->makeCurrent();
    mContainer->onKeyDown(key);
}

void AEmbedAuiWrap::onKeyReleased(AInput::Key key) {
    mContainer->makeCurrent();
    mContainer->onKeyUp(key);
}

void AEmbedAuiWrap::clearFocus() {
    mContainer->setFocusedView(nullptr);
}

ABaseWindow* AEmbedAuiWrap::getWindow() {
    return mContainer.get();
}