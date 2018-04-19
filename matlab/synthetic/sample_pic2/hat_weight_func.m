function [ value ] = hat_weight_func( intensity )
    %HAT_WEIGHT_FUNC Summary of this function goes here
    %   Detailed explanation goes here
    if(intensity <= 129)
        value = intensity;
    else
        value = 255 - intensity;
    end

