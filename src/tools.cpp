#include <iostream>
#include "tools.h"

using Eigen::VectorXd;
using Eigen::MatrixXd;
using std::vector;

Tools::Tools() {}

Tools::~Tools() {}

VectorXd Tools::CalculateRMSE(const vector<VectorXd> &estimations,
                              const vector<VectorXd> &ground_truth) {
    VectorXd rmse(4);
    rmse << 0,0,0,0;
    
    // check the validity of the following inputs:
    //  * the estimation vector size should not be zero
    //  * the estimation vector size should equal ground truth vector size
    if(estimations.size() != ground_truth.size()
       || estimations.size() == 0){
        cout << "Invalid estimation or ground_truth data" << endl;
        return rmse;
    }
    
    //accumulate squared residuals
    for(unsigned int i=0; i < estimations.size(); ++i){
        
        VectorXd residual = estimations[i] - ground_truth[i];
        
        //cout<<"residual in x"<<residual[0]<<endl;
        
        //coefficient-wise multiplication
        residual = residual.array()*residual.array();
        rmse += residual;
    }
    
    //calculate the mean
    rmse = rmse/estimations.size();
    
    //calculate the squared root
    rmse = rmse.array().sqrt();
    
    //return the result
    return rmse;
}



VectorXd Tools::CalculatePredMeas(const VectorXd& x_state) {
   
    float px = x_state[0];
    float py = x_state[1];
    float vx = x_state[2];
    float vy = x_state[3];
    
    float r = sqrt(px*px+py*py);
    
    
    if (fabs(r)<0.00001){
        
        px += 0.001;
        py += 0.001;
        r = sqrt(px*px+py*py);
    }
 
    VectorXd h(3);
    h << r,atan2(py,px),(px*vx+py*vy)/r;
    
    return h;
     

}



MatrixXd Tools::CalculateJacobian(const VectorXd& x_state) {
    
    
    MatrixXd Hj(3,4);
    
    float px = x_state[0];
    float py = x_state[1];
    float vx = x_state[2];
    float vy = x_state[3];
    
    float r = sqrt(pow(px,2) +pow(py,2));
    
    float tmp = (vx*px - vy*py);
    
    if (fabs(r)< 0.001){
        
        px = 0.001;
        py = 0.001;
        r = sqrt(px*px +py*py);
    }
    
    
    Hj << px/r,py/r,0,0,
    -py/pow(r,2),px/pow(r,2),0,0,
    (py*tmp)/pow(r,3),(px*tmp)/pow(r,3),px/r,py/r;
    
    return Hj;
    
}

