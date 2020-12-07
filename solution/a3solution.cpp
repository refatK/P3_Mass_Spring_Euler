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

    this->isInitialized = false;
}


void A3Solution::update(Joint2D* selected, QVector2D mouse_pos){
    if (selected->is_locked()) {
        return;
    }

    this->selected = selected;
    this->mousePos = mouse_pos;
//    selected->set_position(mouse_pos);
//    this->selected = nullptr;
}

void A3Solution::update(){

    if (this->changesWereMade()) {
        this->isInitialized = false;
    }

    if (!isInitialized) {
        this->initializeYk();
    }

    for (int i=0; i<this->m_moving_joints.size(); ++i) {
        Joint2D* joint = m_moving_joints[i];

        if (joint == this->selected) {
            this->selectedIndex = i;
            continue;
        }


        // compute forces for all joints and set
        Vector2f force_gravity = this->calcGravitationalForce(this->m_mass);

        std::vector<Vector2f> forces_spring = this->calcSpringForces(joint, joint->get_springs());
        Vector2f force_spring_total = Vector2f(0.0f,0.0f);
        for (Vector2f sForce : forces_spring) {
            force_spring_total.x() += sForce.x();
            force_spring_total.y() += sForce.y();
        }

        Vector2f force_damp = this->calcDampingForce(this->getVelocity(this->m_yk, i));

        Vector2f net_force = force_gravity + force_spring_total + force_damp; // TODO: add all forces
        Vector2f net_accel = (1.0f / this->m_mass) * net_force;
        this->setAcceleration(this->m_yk_prime, i, net_accel);
    }

    // do Explict Euler
    this->doExplicitEuler(this->m_yk, this->m_yk_prime);

    // update positions in Ui
    this->updatePositionsInUi(this->m_moving_joints, this->m_yk);
}


bool A3Solution::changesWereMade() {
    if (!isInitialized) {
        this->allCurrentJoints.clear();
        for (Joint2D* joint : this->m_joints) {
            this->allCurrentJoints.push_back(joint);
        }

        this->jointCount = this->m_joints.size();
        return true;
    }

    if (this->jointCount != this->m_joints.size() || this->allCurrentJoints != this->m_joints) {
        this->allCurrentJoints.clear();
        for (Joint2D* joint : this->m_joints) {
            this->allCurrentJoints.push_back(joint);
        }

        this->jointCount = this->m_joints.size();
        return true;
    }


    return false;
}


void A3Solution::updatePositionsInUi(std::vector<Joint2D*>& allJointsToUpdate, VectorXf newYk) {
    for (int i=0; i<allJointsToUpdate.size(); ++i) {
        allJointsToUpdate[i]->set_position(this->eigenMathToQt(this->getPosition(newYk, i)));
    }
}

void A3Solution::doExplicitEuler(VectorXf& yk, VectorXf& yk_prime){
    VectorXf newYk = yk + (yk_prime * this->m_timestep);
    yk = newYk;

    // need to update yk_prime too
    for (int i=0; i<this->m_moving_joints.size(); ++i) {
        if (i == selectedIndex) {
            continue;
        }

        Vector2f newV = this->getVelocity(yk, i);
        this->setVelocity(yk, yk_prime,i, newV);
    }

    // selection of mouse case
    if (selectedIndex >= 0) {
        int index = selectedIndex;
        Joint2D* sel = this->m_moving_joints[index];
//        this->setPosition(yk, index, this->qtToEigenMath(sel->get_position()));
        this->setPosition(yk, index, this->qtToEigenMath(this->mousePos));
        this->setVelocity(yk, yk_prime, index, Vector2f(0.0f, 0.0f));
        this->setAcceleration(yk_prime, index, Vector2f(0.0f, 0.0f));

        this->selectedIndex = -1;
        this->selected = nullptr;
    }
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
    int yk_length = 4*this->m_moving_joints.size();
    VectorXf yk(yk_length);
    VectorXf yk_prime(yk_length);

    for (int i=0; i<yk_length; i=i+4) {
        int jointIndex = i/4;
        Joint2D* joint = m_moving_joints[jointIndex];
        Vector2f jointMathPos = this->qtToEigenMath(joint->get_position());

        yk[i+xPOS] = jointMathPos.x();
        yk[i+yPOS] = jointMathPos.y();

        if (!isInitialized) {
            yk[i+xV] = 0.0f;
            yk[i+yV] = 0.0f;
        }

        yk_prime[i+p_xV] = yk[i+xV];
        yk_prime[i+p_yV] = yk[i+yV];
        // this is ok as the acceleration must always be calculated anyways
        yk_prime[i+p_xA] = 0.0f;
        yk_prime[i+p_yA] = 0.0f;
    }

    this->m_yk = yk;
    this->m_yk_prime = yk_prime;
    isInitialized = true;
}

void A3Solution::setPosition(VectorXf& yk, int i, Vector2f pos) {
    int x = i*4;
    yk[x+xPOS] = pos.x();
    yk[x+yPOS] = pos.y();
}

void A3Solution::setVelocity(VectorXf& yk, VectorXf& yk_prime, int i, Vector2f v) {
    int x = i*4;
    yk[x+xV] = v.x();
    yk[x+yV] = v.y();
    yk_prime[x+p_xV] = v.x();
    yk_prime[x+p_yV] = v.y();
}

void A3Solution::setAcceleration(VectorXf& yk_prime, int i, Vector2f a) {
    int x = i*4;
    yk_prime[x+p_xA] = a.x();
    yk_prime[x+p_yA] = a.y();
}

Vector2f A3Solution::getPosition(VectorXf yk, int i) {
    int x = i*4;
    return Vector2f(yk[x+xPOS], yk[x+yPOS]);
}

Vector2f A3Solution::getVelocity(VectorXf yk, int i) {
    int x = i*4;
    return Vector2f(yk[x+xV], yk[x+yV]);
}

Vector2f A3Solution::getAcceleration(VectorXf yk_prime, int i) {
    int x = i*4;
    return Vector2f(yk_prime[x+p_xA], yk_prime[x+p_yA]);
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
