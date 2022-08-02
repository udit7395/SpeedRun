#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>

#include <Eigen/Core>
#include <pangolin/pangolin.h>
#include <unistd.h> //sleep function

using std::cerr;
using std::cout;
using std::endl;
using std::ifstream;
using std::string;
using std::vector;

using namespace Eigen;

namespace fs = std::filesystem;

void drawTrajectory(const vector<Eigen::Isometry3d> &poses);

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        cerr << "Error: input arguments missing " << endl;
        cerr << "Example usage ./plotTrajectories <file_name>.txt" << endl;
        return 1;
    }

    const auto filePath = fs::path{argv[1]};

    // check if file exists or not
    if (!fs::exists(filePath))
    {
        cerr << "File does not exists" << endl;
        return 1;
    }

    cout << "Filepath: " << filePath << endl;
    ifstream fin(filePath);
    vector<Eigen::Isometry3d> poses;
    while (!fin.eof())
    {
        double a, b, c, d, e, f, g, h, i, j, k, l;
        fin >> a >> b >> c >> d >> e >> f >> g >> h >> i >> j >> k >> l;
        Eigen::Matrix4d T_mat;
        T_mat << a, b, c, d,
            e, f, g, h,
            i, j, k, l,
            0, 0, 0, 1;
        Eigen::Isometry3d T;
        T.matrix() = T_mat;
        poses.push_back(T);
    }

    cout << "number of poses: " << poses.size() << endl;

    drawTrajectory(poses);

    return 0;
}

void drawTrajectory(const vector<Eigen::Isometry3d> &poses)
{
    constexpr float w = 640.0f;
    constexpr float h = 480.0f;

    pangolin::CreateWindowAndBind("Trajectory Viewer", w, h);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    pangolin::OpenGlRenderState s_cam(
        pangolin::ProjectionMatrix(w, h, 500, 500, 512, 389, 0.1, 1000),
        pangolin::ModelViewLookAt(0, -0.1, -1.8, 0, 0, 0, 0.0, -1.0, 0.0));

    pangolin::View &d_cam = pangolin::CreateDisplay()
                                .SetBounds(0.0, 1.0, 0.0, 1.0, -w / h)
                                .SetHandler(new pangolin::Handler3D(s_cam));

    while (!pangolin::ShouldQuit())
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        d_cam.Activate(s_cam);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glLineWidth(2);

        for (size_t i = 0; i < poses.size(); i++)
        {
            Vector3d Ow = poses[i].translation();
            Vector3d Xw = poses[i] * (0.1 * Vector3d(1, 0, 0));  
            Vector3d Yw = poses[i] * (0.1 * Vector3d(0, 1, 0));
            Vector3d Zw = poses[i] * (0.1 * Vector3d(0, 0, 1));

            glBegin(GL_LINES);

            glColor3f(1.0, 0.0, 0.0);
            glVertex3d(Ow[0], Ow[1], Ow[2]);
            glVertex3d(Xw[0], Xw[1], Xw[2]);

            glColor3f(0.0, 1.0, 0.0);
            glVertex3d(Ow[0], Ow[1], Ow[2]);
            glVertex3d(Yw[0], Yw[1], Yw[2]);

            glColor3f(0.0, 0.0, 1.0);
            glVertex3d(Ow[0], Ow[1], Ow[2]);
            glVertex3d(Zw[0], Zw[1], Zw[2]);

            glEnd();
        }

        for (size_t i = 0; i < poses.size(); i++)
        {
            glColor3f(0.0, 0.0, 0.0);
            glBegin(GL_LINES);
            auto p1 = poses[i], p2 = poses[i + 1];
            glVertex3d(p1.translation()[0], p1.translation()[1], p1.translation()[2]);
            glVertex3d(p2.translation()[0], p2.translation()[1], p2.translation()[2]);
            glEnd();
        }
        pangolin::FinishFrame();
        usleep(5000);
    }
}