#pragma once

#include <sigpack.h>
#include "AudioFile.h"
#include <iostream>


typedef float SAMPLE;


class spectogram {
public:
    spectogram();
    arma::mat generate_spectogram(const AudioFile<SAMPLE> *file)
    {
        arma::mat output;
        if(file->getNumChannels() == 2)
        {
            std::cout << "I am here" << std::endl;
            arma::Col<SAMPLE> ch1 = file->samples[0];
            arma::Col<SAMPLE> ch2 = file->samples[1];
            arma::mat output = sp::specgram(ch1);
            return output;
        }
        else
        {
            std::cout << "I am am here" << std::endl;
            arma::Col<SAMPLE> ch1 = file->samples[0];
            arma::mat output = sp::specgram(ch1);
            std::cout << output << std::endl;
            return output;
        }

    }
};
