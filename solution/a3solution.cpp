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
}

void A3Solution::update(){
    // Students implement solution here
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
