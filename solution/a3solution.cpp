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

    if (!this->isInitialized) {
        this->initializeYk();
    }


    for (int i=0; i<this->m_moving_joints.size(); ++i) {
        Joint2D* joint = m_moving_joints[i];

        // compute forces for all joints
        Vector2f force_gravity = this->calcGravitationalForce(this->m_mass);
        std::vector<Vector2f> forces_spring = this->calcSpringForces(joint, joint->get_springs());
        Vector2f force_damp = this->calcDampingForce(this->getVelocity(i));
    }

    // do Explict Euler
}

void A3Solution::initializeYk(){
    // set all moveable joints
    this->m_moving_joints.clear();

    for (Joint2D* joint : this->m_joints) {
        if (!joint->is_locked()) {
            this->m_moving_joints.push_back(joint);
        }
    }

    // now set yk
    float yk_length = 4*this->m_moving_joints.size();
    VectorXf yk(yk_length);
    VectorXf yk_prime(yk_length);

    for (int i=0; i<yk_length; i=i+4) {
        int jointIndex = i/4;
        Joint2D* joint = m_moving_joints[jointIndex];

        yk[i+xPOS] = joint->get_position().x();
        yk[i+yPOS] = joint->get_position().y();
        yk[i+xV] = 0.0f;
        yk[i+yV] = 0.0f;

        // TODO: probs should be removed
        yk_prime[i+p_xV] = yk[i+xV];
        yk_prime[i+p_yV] = yk[i+yV];
        yk_prime[i+p_xA] = 0.0f;
        yk_prime[i+p_yA] = 0.0f;
    }

    this->m_yk = yk;
    this->m_yk_prime = yk_prime;
    isInitialized = true;
}


Vector2f A3Solution::getPosition(int i) {
    int x = i*4;
    return Vector2f(this->m_yk[x+xPOS], this->m_yk[x+yPOS]);
}

Vector2f A3Solution::getVelocity(int i) {
    int x = i*4;
    return Vector2f(this->m_yk[x+xV], this->m_yk[x+yV]);
}

Vector2f A3Solution::getAcceleration(int i) {
    int x = i*4;
    return Vector2f(this->m_yk_prime[x+p_xA], this->m_yk_prime[x+p_yA]);
}


Vector2f A3Solution::calcDampingForce(Vector2f velocity) {
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
}


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
