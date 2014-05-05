//------------------------------------------------------------------------------//
//  Engi OpenGL Framework v0.0.1                                                //
//  Program.cpp                                                                 //
//                                                                              //
//  This file is part of Engi OpenGL Framework                                  //
//                                                                              //
//  This program is free software: you can redistribute it and/or modify        //
//      it under the terms of the GNU General Public License as published by    //
//      the Free Software Foundation, either version 3 of the License, or       //
//      (at your option) any later version.                                     //
//                                                                              //
//      This program is distributed in the hope that it will be useful,         //
//      but WITHOUT ANY WARRANTY; without even the implied warranty of          //
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           //
//      GNU General Public License for more details.                            //
//                                                                              //
//      You should have received a copy of the GNU General Public License       //
//      along with this program.  If not, see <http://www.gnu.org/licenses/>    //
//------------------------------------------------------------------------------//

#include "Logger.hpp"
#include "Utility.h"

#include "Keyboard.hpp"
#include "Mouse.hpp"

#include "Graphics.hpp"
#include "Camera.hpp"
#include "Timer.hpp"

#include "Math.hpp"
#include "GlobalVariables.h"

void InputProcess();
void UpdateModel();

using namespace std;
using namespace Keyboard;
using namespace Mouse;

static KeyboardClient   *kbdc   = nullptr;
static MouseClient      *mc     = nullptr;
static Graphics         *gfx    = nullptr;
static Camera           *cam    = nullptr;
static Timer            *tmr    = nullptr;

static std::vector<glm::vec3> cube_vs =
{
    glm::vec3(+0.5, +0.5, +0.5),
    glm::vec3(+0.5, -0.5, +0.5),
    glm::vec3(-0.5, -0.5, +0.5),
    glm::vec3(-0.5, +0.5, +0.5),
    glm::vec3(+0.5, +0.5, -0.5),
    glm::vec3(+0.5, -0.5, -0.5),
    glm::vec3(-0.5, -0.5, -0.5),
    glm::vec3(-0.5, +0.5, -0.5)

};

static std::vector<unsigned> cube_is =
{
    0, 1, 2, 0, 3, 2, // Front face
    4, 5, 6, 4, 7, 6, // Back face
    0, 1, 5, 0, 4, 5, // Right face
    3, 2, 6, 3, 7, 6, // Left face
    2, 1, 5, 2, 6, 5, // Bottom face
    3, 0, 4, 3, 7, 4  // Top face
};
// Camera controls
static float move_speed = .025f;
static float angular_speed = 1.0f;
static float mouse_sensitivity = 1.0f;
// Movement controls
static float r = 0.0;
static float sphere_rotation = 0.0;
// Textures
static unsigned spaceID = 0;
static unsigned sunID = 0;
static unsigned mercuryID = 0;
static unsigned venusID = 0;
static unsigned earthID = 0;
static unsigned marsID = 0;
static unsigned moonID = 0;
//////////////////////////////////////////////////////////////////////////
// Planets
//////////////////////////////////////////////////////////////////////////
// Sun -- not really a planet.. haha
static float sun_speed = 2.5f;
static float sun_distance = 5.0f;
// Mercury
static float mercury_speed = 5.0f;
static float mercury_distance = 150.0f;
// Venus
static float venus_speed = 10.0f;
static float venus_distance = 250.0f;
// Earth
static float earth_speed = 3.0f;
static float earth_distance = 500.0f;
// Mars
static float mars_speed = 2.0f;
static float mars_distance = 600.0f;
// Moon
static float moon_speed = 7.5f;
static float moon_distance = 35.0f;

unsigned long long fc = 0;

std::vector<glm::vec3> *mesh = new std::vector<glm::vec3>();
std::vector<glm::vec3> *c_sphere = new std::vector<glm::vec3>();

//#define MATLAB

static void UVSphereTest(unsigned textureID, glm::vec2 &start, glm::vec2 &end, unsigned quality)
{
    // Untransformed vertexes, stored for UV mapping
    glm::vec4 vec(0.0f, 0.0f, 0.0f, 1.0f);
    // UV map
    glm::vec2 uv;
    // Step size
    float d = M_PI / quality;
    glBindTexture(GL_TEXTURE_2D, textureID);
    glBegin(GL_QUADS);
#if defined(MATLAB)
    FILE *f;
    fopen_s(&f, "C:\\Users\\Christian\\Documents\\MATLAB\\data.out", "w");
#endif
    float quality_sq = quality*quality;
    for (float theta = start.y; theta <= end.y; theta+=d)
    {
        for (float phi = start.x; phi <= end.x; phi+=d)
        {
            // Values of the next iteration
            float sum_p = phi + d;
            float sum_t = theta + d;

            // Vertex 01
            vec.x = glm::sin<float>(sum_p) * glm::sin<float>(sum_t);
            vec.y = glm::cos<float>(sum_p);
            vec.z = glm::sin<float>(sum_p) * glm::cos<float>(sum_t);
            uv.x = ((glm::atan(vec.x, vec.z) / M_PI) + 1.0f) * 0.5f;
            uv.y = 0.5f - glm::asin<float>(vec.y) / M_PI;

            glTexCoord2f(uv.x, uv.y);
            glVertex3f(vec.x, vec.y, vec.z);

            // Vertex 02
            vec.x = glm::sin<float>(phi) * glm::sin<float>(sum_t);
            vec.y = glm::cos<float>(phi);
            vec.z = glm::sin<float>(phi) * glm::cos<float>(sum_t);
            uv.x = ((glm::atan(vec.x, vec.z) / M_PI) + 1.0f) * 0.5f;
            uv.y = 0.5f - glm::asin<float>(vec.y) / M_PI;

            glTexCoord2f(uv.x, uv.y);
            glVertex3f(vec.x, vec.y, vec.z);

            // Vertex 03
            vec.x = glm::sin<float>(phi) * glm::sin<float>(theta);
            vec.y = glm::cos<float>(phi);
            vec.z = glm::sin<float>(phi) * glm::cos<float>(theta);
            uv.x = ((glm::atan(vec.x, vec.z) / M_PI) + 1.0f) * 0.5f;
            uv.y = 0.5f - glm::asin<float>(vec.y) / M_PI;

            glTexCoord2f(uv.x, uv.y);
            glVertex3f(vec.x, vec.y, vec.z);

            // Vertex 04
            vec.x = glm::sin<float>(sum_p) * glm::sin<float>(theta);
            vec.y = glm::cos<float>(sum_p);
            vec.z = glm::sin<float>(sum_p) * glm::cos<float>(theta);
            uv.x = ((glm::atan(vec.x, vec.z) / M_PI) + 1.0f) * 0.5f;
            uv.y = 0.5f - glm::asin<double>(vec.y) / M_PI;

            glTexCoord2f(uv.x, uv.y);
            glVertex3f(vec.x, vec.y, vec.z);

#if defined(MATLAB)
            {
                fprintf(f, "%f ", uv.x);
            }
#endif
        }
    }
    glEnd();
#if defined(MATLAB)
    fclose(f);
    exit(0);
#endif
}

static void drawcube(int tiles, unsigned textureID)
{
    // Cube positions
    float x = -1.0f;
    float y = -1.0f;
    float z = -1.0f;
    // How much we step
    float dr = 2.0f / tiles;
    // Vertexes
    std::vector<glm::vec3> vertexes;
    std::vector<glm::vec2> texels;

    //glColor3f(0.0f, 1.0f, 1.0f);
    //for (float row = 0.0f; row <= 1.0f; row+=dr)
    //{
    //    for (float col = 0.0f; col <= 1.0f; col+=dr)
    //    {
    //        // p(s,t) / abs(p(s,t))
    //        // p(s,t) = (1 - 2s, 1 - 2t, 1)
    //        glm::vec3 vec = glm::vec3(1.0f - 2.0f * row, 1.0f - 2.0f * col, 1.0f);
    //        vec = vec / (vec.x * vec.x + vec.y * vec.y + 1);
    //        vertexes.push_back(vec);
    //        glVertex3f(vec.x, vec.y, vec.z);
    //    }
    //}

    FILE *f; fopen_s(&f, "C:\\Users\\Christian\\Documents\\MATLAB\\data.out", "w");
    for (int row = 0; row <= tiles; row++)
    {
        for (int col = 0; col <= tiles; col++)
        {
            float dcol = dr * col;
            float drow = dr * row;
            float ldcol = dcol - dr;
            float ldrow = drow - dr;

            if (drow == M_PI - (M_PI) / tiles)
                continue;

            // Front face
            vertexes.push_back(glm::vec3(x + ldcol, y + ldrow, z));
            vertexes.push_back(glm::vec3(x + dcol, y + ldrow, z));
            vertexes.push_back(glm::vec3(x + dcol, y + drow, z));
            vertexes.push_back(glm::vec3(x + ldcol, y + drow, z));
            // Back face
            vertexes.push_back(glm::vec3(x + ldcol, y + ldrow, -z));
            vertexes.push_back(glm::vec3(x + dcol, y + ldrow, -z));
            vertexes.push_back(glm::vec3(x + dcol, y + drow, -z));
            vertexes.push_back(glm::vec3(x + ldcol, y + drow, -z));
            // Right face
            vertexes.push_back(glm::vec3(x, y + ldrow, z + ldcol));
            vertexes.push_back(glm::vec3(x, y + ldrow, z + dcol));
            vertexes.push_back(glm::vec3(x, y + drow, z + dcol));
            vertexes.push_back(glm::vec3(x, y + drow, z + ldcol));
            // Left face
            vertexes.push_back(glm::vec3(-x, y + ldrow, z + ldcol));
            vertexes.push_back(glm::vec3(-x, y + ldrow, z + dcol));
            vertexes.push_back(glm::vec3(-x, y + drow, z + dcol));
            vertexes.push_back(glm::vec3(-x, y + drow, z + ldcol));
            // Top face
            vertexes.push_back(glm::vec3(x + ldcol, y, z + ldrow));
            vertexes.push_back(glm::vec3(x + dcol, y, z + ldrow));
            vertexes.push_back(glm::vec3(x + dcol, y, z + drow));
            vertexes.push_back(glm::vec3(x + ldcol, y, z + drow));
            // Bottom face
            vertexes.push_back(glm::vec3(x + ldcol, -y, z + ldrow));
            vertexes.push_back(glm::vec3(x + dcol, -y, z + ldrow));
            vertexes.push_back(glm::vec3(x + dcol, -y, z + drow));
            vertexes.push_back(glm::vec3(x + ldcol, -y, z + drow));
        }
    }
    glBindTexture(GL_TEXTURE_2D, textureID);
    glBegin(GL_QUADS);
    int c = 0;
    glm::vec2 luv; // Keeps track of last coordinates calculated
    for (auto itr = vertexes.begin(); itr != vertexes.end(); itr++, c++)
    {
        // Makes into a sphere
        *itr = glm::normalize(*itr);
        // UV mapping
        //glm::vec2 texel;
        glm::vec2 uv = glm::vec2(((glm::atan<float>(itr->x, itr->z) / M_PI) + 1.0f) * 0.5f, 0.5f - glm::asin<float>(itr->y) / M_PI);
        //fprintf(f, "%f ", luv.y - uv.y);
        glTexCoord2f(uv.x, uv.y);
        //glColor3f(1, 1, 1);
        glVertex3f(itr->x, itr->y, itr->z);

        luv = uv;
    }fclose(f);
    glEnd();
    //exit(0);
}

static std::vector<glm::vec3> buildCubedSphere(int quality)
{
    std::vector<glm::vec3> vertexes;
    vertexes.reserve(24 * quality * quality);
    // Cube positions
    float x = -1.0f;
    float y = -1.0f;
    float z = -1.0f;
    // How much we step
    float dr = 2.0f / quality;
    for (int row = 1; row <= quality; row++)
    {
        for (int col = 1; col <= quality; col++)
        {
            float dcol = dr * col;
            float drow = dr * row;
            float ldcol = dcol - dr;
            float ldrow = drow - dr;

            // Front face
            vertexes.push_back(glm::normalize(glm::vec3(x + ldcol, y + ldrow, z)));
            vertexes.push_back(glm::normalize(glm::vec3(x + dcol, y + ldrow, z)));
            vertexes.push_back(glm::normalize(glm::vec3(x + dcol, y + drow, z)));
            vertexes.push_back(glm::normalize(glm::vec3(x + ldcol, y + drow, z)));
            // Back face
            vertexes.push_back(glm::normalize(glm::vec3(x + ldcol, y + ldrow, -z)));
            vertexes.push_back(glm::normalize(glm::vec3(x + dcol, y + ldrow, -z)));
            vertexes.push_back(glm::normalize(glm::vec3(x + dcol, y + drow, -z)));
            vertexes.push_back(glm::normalize(glm::vec3(x + ldcol, y + drow, -z)));
            // Right face
            vertexes.push_back(glm::normalize(glm::vec3(x, y + ldrow, z + ldcol)));
            vertexes.push_back(glm::normalize(glm::vec3(x, y + ldrow, z + dcol)));
            vertexes.push_back(glm::normalize(glm::vec3(x, y + drow, z + dcol)));
            vertexes.push_back(glm::normalize(glm::vec3(x, y + drow, z + ldcol)));
            // Left face
            vertexes.push_back(glm::normalize(glm::vec3(-x, y + ldrow, z + ldcol)));
            vertexes.push_back(glm::normalize(glm::vec3(-x, y + ldrow, z + dcol)));
            vertexes.push_back(glm::normalize(glm::vec3(-x, y + drow, z + dcol)));
            vertexes.push_back(glm::normalize(glm::vec3(-x, y + drow, z + ldcol)));
            // Top face
            vertexes.push_back(glm::normalize(glm::vec3(x + ldcol, y, z + ldrow)));
            vertexes.push_back(glm::normalize(glm::vec3(x + dcol, y, z + ldrow)));
            vertexes.push_back(glm::normalize(glm::vec3(x + dcol, y, z + drow)));
            vertexes.push_back(glm::normalize(glm::vec3(x + ldcol, y, z + drow)));
            // Bottom face
            vertexes.push_back(glm::normalize(glm::vec3(x + ldcol, -y, z + ldrow)));
            vertexes.push_back(glm::normalize(glm::vec3(x + dcol, -y, z + ldrow)));
            vertexes.push_back(glm::normalize(glm::vec3(x + dcol, -y, z + drow)));
            vertexes.push_back(glm::normalize(glm::vec3(x + ldcol, -y, z + drow)));
        }
    }
    return vertexes;
}

void Init(Graphics *_gfx, KeyboardServer *kbds, MouseServer *ms)
{
    kbdc = new KeyboardClient(*kbds);
    mc = new MouseClient(*ms);
    gfx = _gfx;
    cam = new Camera();
    tmr = new Timer();
    // TODO: Initialization code here

    spaceID   = GraphicsUtility::LoadTexture(".\\Assets\\Images\\Space.jpg");
    sunID     = GraphicsUtility::LoadTexture(".\\Assets\\Images\\Sun.jpg");
    mercuryID = GraphicsUtility::LoadTexture(".\\Assets\\Images\\Mercury.jpg");
    venusID   = GraphicsUtility::LoadTexture(".\\Assets\\Images\\Venus.jpg");
    earthID   = GraphicsUtility::LoadTexture(".\\Assets\\Images\\Earth.jpg");
    marsID    = GraphicsUtility::LoadTexture(".\\Assets\\Images\\Mars.jpg");
    moonID    = GraphicsUtility::LoadTexture(".\\Assets\\Images\\Moon.jpg");

    *mesh = {
        glm::vec3(+1.0f, +0.0f, +0.0f),
        glm::vec3(+0.0f, +1.0f, +0.0f),
        glm::vec3(+0.0f, 00.0f, +1.0f),
    };
    // Texturing regular spheres doesn't work out well due to singularity points on the poles
    *c_sphere = buildCubedSphere(50);
    // Allow textures
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHT0);
    // Draw wireframes
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Loop()
{
    gfx->BeginFrame();

    InputProcess();
    UpdateModel();

    glm::mat4 earth_position = Matrix::Translate(1.5 * earth_distance* glm::sin<float>(r * earth_speed * sphere_rotation / 50.0f), 0.0f, earth_distance * glm::cos<float>(r * earth_speed * sphere_rotation / 50.0f));

    gfx->DrawMesh(*c_sphere, Matrix::Scale(2000.0f), spaceID, GL_QUADS);
    gfx->DrawMesh(*c_sphere, Matrix::Scale(100.0f) * Matrix::RotateY(r * sphere_rotation * sun_speed) * Matrix::Translate(1.5 * sun_distance * glm::sin<float>(r * sun_speed * sphere_rotation / 50.0f), 0.0f, sun_distance * glm::cos<float>(r * sun_speed * sphere_rotation / 50.0f)), sunID, GL_QUADS);
    gfx->DrawMesh(*c_sphere, Matrix::Scale(10.0f) * Matrix::RotateY(r * sphere_rotation * mercury_speed) * Matrix::Translate(1.5 * mercury_distance* glm::sin<float>(r * mercury_speed * sphere_rotation / 50.0f), 0.0f, mercury_distance * glm::cos<float>(r * mercury_speed * sphere_rotation / 50.0f)), mercuryID, GL_QUADS);
    gfx->DrawMesh(*c_sphere, Matrix::Scale(15.0f) * Matrix::RotateY(r * sphere_rotation * venus_speed) * Matrix::Translate(1.5 * venus_distance* glm::sin<float>(r * venus_speed * sphere_rotation / 50.0f), 0.0f, venus_distance * glm::cos<float>(r * venus_speed * sphere_rotation / 50.0f)), venusID, GL_QUADS);
    gfx->DrawMesh(*c_sphere, Matrix::Scale(20.0f) * Matrix::RotateY(r * sphere_rotation * earth_speed) * earth_position, earthID, GL_QUADS);
    gfx->DrawMesh(*c_sphere, Matrix::Scale(17.5f) * Matrix::RotateY(r * sphere_rotation * mars_speed) * Matrix::Translate(1.5 * mars_distance* glm::sin<float>(r * mars_speed * sphere_rotation / 50.0f), 0.0f, mars_distance * glm::cos<float>(r * mars_speed * sphere_rotation / 50.0f)), marsID, GL_QUADS);
    gfx->DrawMesh(*c_sphere, Matrix::Scale(2.5f) * Matrix::RotateY(r * sphere_rotation * moon_speed) * Matrix::Translate(1.5 * moon_distance* glm::sin<float>(r * moon_speed * sphere_rotation / 50.0f), 0.0f, moon_distance * glm::cos<float>(r * moon_speed * sphere_rotation / 50.0f)) * earth_position, moonID, GL_QUADS);

    gfx->Axis(2);

    gfx->EndFrame();
    if (tmr->Elapsed_ms() > 500)
    {
        tmr->Reset();
        fc++;
    }
}

void Exit()
{
    // TODO: Cleanup code here
    SafeDelete(mesh);
    SafeDelete(c_sphere);
    // Releasing VRAM
    glDeleteTextures(1, &spaceID);
    glDeleteTextures(1, &sunID);
    glDeleteTextures(1, &mercuryID);
    glDeleteTextures(1, &venusID);
    glDeleteTextures(1, &earthID);
    glDeleteTextures(1, &marsID);
    // Deleting HIDs
    SafeDelete(kbdc);
    SafeDelete(mc);
    SafeDelete(cam);
    SafeDelete(tmr);
}

void InputProcess()
{
    ///////////////////////
    // Keyboard commands //
    ///////////////////////
    // Movement controls
    if (kbdc->IsPressed('W'))
        cam->Forward(-move_speed);
    if (kbdc->IsPressed('S'))
        cam->Forward(move_speed);;
    if (kbdc->IsPressed('A'))
        cam->Strafe(-move_speed);
    if (kbdc->IsPressed('D'))
        cam->Strafe(move_speed);
    // Roll camera
    if (kbdc->IsPressed('Q'))
        cam->RotateZ(angular_speed);
    if (kbdc->IsPressed('E'))
        cam->RotateZ(-angular_speed);
    // Acceleration
    if (kbdc->IsPressed('1'))
        move_speed *= 2.0f;
    if (kbdc->IsPressed('2'))
        move_speed *= 0.5f;

    // Request to quit
    if (kbdc->IsPressed(27))
        bQuit = true;
    auto point = mc->GetDifference();
    cam->RotateX(mouse_sensitivity * point.y);
    cam->RotateY(mouse_sensitivity * point.x);
}

void UpdateModel()
{
    r += 0.01f;
    // 1 Hour to orbit around the sun
    sphere_rotation += (M_PI * 2) / (FPS * 360);
    cam->Render();
}