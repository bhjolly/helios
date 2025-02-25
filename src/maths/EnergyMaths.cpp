#include <EnergyMaths.h>


// ***  EMITTED / RECEIVED POWER  *** //
// ********************************** //
// Space distribution equation to calculate the beam energy decreasing the further away from the center (Carlsson et al., 2001)
double EnergyMaths::calcEmittedPower(
    double const I0,
    double const lambda,
    double const R,
    double const R0,
    double const r,
    double const w0
){
    double const denom = lambda*lambda * (R0*R0 + R*R);
    return I0 / exp(PI_SQUARED_2 * r*r * w0*w0 / denom);
}

double EnergyMaths::calcEmittedPowerLegacy(
    double const I0,
    double const lambda,
    double const R,
    double const R0,
    double const r,
    double const w0
){
    double const denom = M_PI * w0 * w0;
    double const omega = (lambda * R) / denom;
    double const omega0 = (lambda * R0) / denom;
    double const w = w0 * sqrt(omega0 * omega0 + omega * omega);
    return I0 * exp((-2 * r * r) / (w * w));
}

// Laser radar equation "Signature simulation..." (Carlsson et al., 2000)
double EnergyMaths::calcReceivedPower(
    double const I0,
    double const lambda,
    double const R,
    double const R0,
    double const r,
    double const w0,
    double const Dr2,
    double const Bt2,
    double const etaSys,
    double const ae,
    double const sigma
){
    double const Rsquared = R*R;
    double const numer = I0 * Dr2 * etaSys * sigma;
    double const expon = exp(
        (PI_SQUARED_2 * r*r * w0*w0) / (lambda*lambda * (R0*R0 + Rsquared)) +
        2*R*ae
    );
    double const denom = PI_4 * Rsquared*Rsquared * Bt2 * expon;
    return numer / denom;
}

double EnergyMaths::calcReceivedPowerLegacy(
    double const Pe,
    double const Dr2,
    double const R,
    double const Bt2,
    double const etaSys,
    double const etaAtm,
    double const sigma
){
    return (Pe * Dr2) / (PI_4 * pow(R, 4) * Bt2) * etaSys * etaAtm * sigma;
}


// ***  ATMOSPHERIC STUFF  *** //
// *************************** //
// Energy left after attenuation by air particles in range [0,1]
double EnergyMaths::calcAtmosphericFactor(double const R, double const ae){
    return exp(-2 * R * ae);
}


// ***  CROSS-SECTION  *** //
// *********************** //
// ALS Simplification "Radiometric Calibration..." (Wagner, 2010) Eq. 14
double EnergyMaths::calcCrossSection(
    double const f,
    double const Alf,
    double const theta
){
    return PI_4 * f * Alf * cos(theta);
}


// ***  LIGHTING  *** //
// ****************** //
// Phong reflection model "Normalization of Lidar Intensity..." (Jutzi and Gross, 2009)
double EnergyMaths::phongBDRF(
    double const incidenceAngle,
    double const targetSpecularity,
    double const targetSpecularExponent
){
    double const ks = targetSpecularity;
    double const kd = (1 - ks);
    double const diffuse = kd * cos(incidenceAngle);
    double const specularAngle = (incidenceAngle <= PI_HALF) ?
                                 incidenceAngle : incidenceAngle - PI_HALF;
    double const specular = ks * pow(
        abs(cos(specularAngle)),
        targetSpecularExponent
    );
    return diffuse + specular;
}
