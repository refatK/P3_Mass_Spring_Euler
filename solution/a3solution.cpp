#include "a3solution.h"

#include "dependencies/Eigen/Dense"
#include "QDebug"
#include "QElapsedTimer"

using Eigen::Vector2f;
using Eigen::Vector4f;
using Eigen::VectorXf;
using Eigen::MatrixXd;
using Eigen::MatrixXf;

A3Solution::A3Solution(std::vector<Joint2D*>& joints, std::vector<Spring2D*>& springs, float& gravity, float& positional_damping, float& mass, float& timestep, bool& implicit, float& stiffness)
    :m_joints(joints),
    m_links(springs),
    m_gravity(gravity),
    m_positional_damping(positional_damping),
    m_mass(mass),
    m_timestep(timestep),
    m_implicit(implicit),
    m_stiffness(stiffness)
    {

}


void A3Solution::update(Joint2D* selected, QVector2D mouse_pos){
    selected->set_position(mouse_pos);

    // for external force
}

void A3Solution::update(){

    for (Joint2D* joint : m_joints) {
        // skip locked joints
        if (joint->is_locked()) { continue; }

        // compute forces for all joints

        Vector2f force_gravity = this->calcGravitationalForce(this->m_mass);
        std::vector<Vector2f> forces_spring = this->calcSpringForces(joint, joint->get_springs());
        // TODO: damping force

    }

    // do Explict Euler
}

Vector2f A3Solution::calcDampingForcw(Vector2f velocity) {
    return -1 * this->m_positional_damping * velocity;
}

std::vector<Vector2f> A3Solution::calcSpringForces(Joint2D* joint, std::vector<Spring2D*> springs) {
    std::vector<Vector2f> forces;

    for(Spring2D* spring : springs) {
        float currL = spring->get_length();
        float restL = spring->get_rest_length();

        Joint2D* other = spring->get_other_joint(joint);
        Vector2f vec_to_joint = this->qtToEigenMath(joint->get_position() - other->get_position());
        Vector2f force = -1 * this->m_stiffness * (currL-restL) * vec_to_joint.normalized();
        forces.push_back(force);
    }

    return forces;
}


Vector2f A3Solution::calcGravitationalForce(float mass) {
    Vector2f vec_unit_down = -1*Vector2f::UnitY();
    return mass * this->m_gravity * vec_unit_down;


Vector2f A3Solution::qtToEigenMath(QVector2D qtVec) {
    QVector2D mathVec = qtVec;
    mathVec.setY(-qtVec.y());
    return Vector2f(mathVec.x(), mathVec.y());
}

QVector2D A3Solution::eigenMathToQt(Vector2f mathVec) {
    QVector2D qtVec = QVector2D(mathVec.x(), mathVec.y());
    qtVec.setY(-mathVec.y());
    return qtVec;
}


void A3Solution::test_eigen_library(){

    // create a simple matrix 5 by 6
    MatrixXd mat(5,6);

    // Fills in matrix
    // Important Note: Eigen matrices are row major
    // so mat(0,1) references the 0-th column and 1-th row
    for(unsigned int row=0;row<mat.rows();row++){
        for(unsigned int col=0;col<mat.cols();col++){
            mat(row,col) = row+col;
        }
    }

    // create the pseudoinverse
    MatrixXd pseudo_inv = mat.completeOrthogonalDecomposition().pseudoInverse();

    // print the pseudoinverse
    std::cout << "--------------------------" << std::endl;
    std::cout << pseudo_inv << std::endl;
    std::cout << "--------------------------" << std::endl;

}
