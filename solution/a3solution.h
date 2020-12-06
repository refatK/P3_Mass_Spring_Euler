#ifndef A2SOLUTION_H
#define A2SOLUTION_H

#include <vector>
#include <algorithm>

#include "OpenGL/elements/joint2D.h"
#include "OpenGL/elements/spring2d.h"
#include "dependencies/Eigen/Dense"

using Eigen::Vector2f;
using Eigen::VectorXf;
using Eigen::MatrixXf;

class A3Solution
{
public:
    A3Solution(std::vector<Joint2D*>& joints, std::vector<Spring2D*>& springs, float& gravity, float& positional_damping, float& mass, float& timestep, bool& implicit, float& stiffness);

    // OpenGL members (these are updated for you)
    std::vector<Joint2D*>& m_joints;
    std::vector<Spring2D*>& m_links;
    float& m_gravity;
    float& m_positional_damping;
    float& m_mass;
    float& m_timestep;
    bool& m_implicit;
    float& m_stiffness;

    // Separate tracking of positions and velocities
    void update();
    void update(Joint2D* selected, QVector2D mouse_pos);

    static void test_eigen_library();

private:

    // Utils
    Vector2f qtToEigenMath(QVector2D qtVec);
    QVector2D eigenMathToQt(Vector2f mathVec);

    // Force Calculation
    Vector2f calcGravitationalForce(float mass);
    std::vector<Vector2f> calcSpringForces(Joint2D* joint, std::vector<Spring2D*> springs);
    Vector2f calcDampingForcw(Vector2f velocity);

};

#endif // A2SOLUTION_H
